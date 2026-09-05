#if 0 // GPU
# Build and run commands:
source /opt/intel/oneapi/setvars.sh intel64
icpx -DRTM_ENABLE_EMBREE_GPU -fsycl -fsycl-targets=spir64 -Xclang -fsycl-allow-func-ptr -I/home/constantine/Desktop/embree/include -L/home/constantine/Desktop/embree/build/ main.cpp -lembree4_sycl -lembree4 -o embree_macroroni
LD_LIBRARY_PATH=/home/constantine/Desktop/embree/build/:$LD_LIBRARY_PATH ./embree_macroroni
#endif
#if 0 // CPU
# Build and run commands:
source /opt/intel/oneapi/setvars.sh intel64
icpx -DRTM_ENABLE_EMBREE_CPU -I/home/constantine/Desktop/embree/include -L/home/constantine/Desktop/embree/build/ main.cpp -lembree4 -ltbb -o embree_macroroni
LD_LIBRARY_PATH=/home/constantine/Desktop/embree/build/:$LD_LIBRARY_PATH ./embree_macroroni
#endif

#include <iostream>
#include <vector>
#include <limits>

#include "../Common/ray_tracing_macroroni.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../Common/stb_image_write.h"

// Define image constants
const int WIDTH = 800;
const int HEIGHT = 600;

void embreeErrorCallback(void* userPtr, RTCError code, const char* str) {
    std::cerr << "Embree Error: " << str << " (Code: " << code << ")" << std::endl;
}

int main() {
    RTM_CREATE_EMBREE_DEVICE_AND_SYCL_QUEUE()

    std::cout << "Running on device: " << rtmSyclQueueGetDeviceName(rtm_sycl_queue) << std::endl;

    if (!rtm_rtc_device) {
        std::cerr << "Failed to create Embree device\n";
        return 1;
    }
    rtcSetDeviceErrorFunction(rtm_rtc_device, embreeErrorCallback, nullptr);

    // 2. Create the Embree scene and geometry
    RTCScene scene = rtcNewScene(rtm_rtc_device);
    RTCGeometry geom = rtcNewGeometry(rtm_rtc_device, RTC_GEOMETRY_TYPE_TRIANGLE);

    // 3. Allocate Shared/USM Memory for Vertices and Indices
    float* vertices = (float*)rtmMallocShared(3 * 3 * sizeof(float), rtm_sycl_queue);
    unsigned int* indices = (unsigned int*)rtmMallocShared(3 * sizeof(unsigned int), rtm_sycl_queue);

    // Hardcode a single simple triangle in front of the camera
    vertices[0] =  0.0f; vertices[1] =  0.5f; vertices[2] = 2.0f; // Top
    vertices[3] = -0.5f; vertices[4] = -0.5f; vertices[5] = 2.0f; // Bottom Left
    vertices[6] =  0.5f; vertices[7] = -0.5f; vertices[8] = 2.0f; // Bottom Right

    indices[0] = 0; indices[1] = 1; indices[2] = 2;

    // Assign buffers to Embree geometry
    rtcSetSharedGeometryBuffer(geom, RTC_BUFFER_TYPE_VERTEX, 0, RTC_FORMAT_FLOAT3, vertices, 0, 3 * sizeof(float), 3);
    rtcSetSharedGeometryBuffer(geom, RTC_BUFFER_TYPE_INDEX, 0, RTC_FORMAT_UINT3, indices, 0, 3 * sizeof(unsigned int), 1);

    rtcCommitGeometry(geom);
    rtcAttachGeometry(scene, geom);
    rtcReleaseGeometry(geom);
    rtcCommitScene(scene);

    // Get traversable handle needed for rtm_rtc_device-side tracing in Embree 4
    RTCTraversable traversable = rtcGetSceneTraversable(scene);

    // 4. Create an output frame buffer using USM shared allocation
    uint8_t* pixels = (uint8_t*)rtmMallocShared(WIDTH * HEIGHT * 3, rtm_sycl_queue);

    // 5. Submit the rendering kernel
    RTM_TBB_SYCL_PARALLEL_FOR_XY_BEGIN(WIDTH, HEIGHT, rtm_sycl_queue) {
      // Normalize coordinates to [-1, 1] range
      float u = (x + 0.5f) / WIDTH * 2.0f - 1.0f;
      float v = 1.0f - (y + 0.5f) / HEIGHT * 2.0f; // Flip Y for typical image coordinates
      float aspect = (float)WIDTH / (float)HEIGHT;
      u *= aspect;

      // Define Ray Structure
      RTCRayHit rayhit;
      rayhit.ray.org_x = 0.0f;
      rayhit.ray.org_y = 0.0f;
      rayhit.ray.org_z = 0.0f;
      rayhit.ray.dir_x = u;
      rayhit.ray.dir_y = v;
      rayhit.ray.dir_z = 1.0f; // Facing positive Z
      rayhit.ray.tnear = 0.0f;
      rayhit.ray.tfar  = std::numeric_limits<float>::infinity();
      rayhit.ray.mask  = -1;
      rayhit.ray.time  = 0.0f;
      rayhit.ray.flags = 0;

      rayhit.hit.geomID = RTC_INVALID_GEOMETRY_ID;
      rayhit.hit.primID = RTC_INVALID_GEOMETRY_ID;
      rayhit.hit.instID[0] = RTC_INVALID_GEOMETRY_ID;

      // Configure intersection arguments
      RTCIntersectArguments args;
      rtcInitIntersectArguments(&args);
      args.feature_mask = RTC_FEATURE_FLAG_TRIANGLE; // Performance flag optimization

      // Embree 4 Device-side Intersection
      rtcTraversableIntersect1(traversable, &rayhit, &args);

      int pixel_index = (y * WIDTH + x) * 3;
      if (rayhit.hit.geomID != RTC_INVALID_GEOMETRY_ID) {
          // Shading code if hit (Orange Triangle)
          pixels[pixel_index + 0] = 255; // Red
          pixels[pixel_index + 1] = 128; // Green
          pixels[pixel_index + 2] = 0;   // Blue
      } else {
          // Background color (Dark Gray)
          pixels[pixel_index + 0] = 32;
          pixels[pixel_index + 1] = 32;
          pixels[pixel_index + 2] = 32;
      }
    }
    RTM_TBB_SYCL_PARALLEL_FOR_XY_END()

    // 6. Wait for execution to finish and save image
    rtmSyclQueueWait(rtm_sycl_queue);
    stbi_write_png("output.png", WIDTH, HEIGHT, 3, pixels, WIDTH * 3);
    std::cout << "Image successfully written to output.png" << std::endl;

    // 7. Cleanup
    rtmFree(pixels, rtm_sycl_queue);
    rtmFree(vertices, rtm_sycl_queue);
    rtmFree(indices, rtm_sycl_queue);
    rtcReleaseScene(scene);
    rtcReleaseDevice(rtm_rtc_device);

    return 0;
}

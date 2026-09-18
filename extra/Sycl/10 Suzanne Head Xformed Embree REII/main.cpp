#if 0 // GPU
# Build and run commands:
source /opt/intel/oneapi/setvars.sh intel64
icx -c ../../../vkfast.c "../../../extra/CPU GPU Array/vkfast_extra_cpu_gpu_array.c" ../../../extra/REII/vkfast_extra_reii.c /home/linuxbrew/RedGpuSDK/redgpu.c /home/linuxbrew/RedGpuSDK/redgpu_2.c /home/linuxbrew/RedGpuSDK/redgpu_32.c -I/home/linuxbrew/.linuxbrew/include/ -I/home/linuxbrew/.linuxbrew/Cellar/xorgproto/2025.1/include/ -I/var/home/linuxbrew/.linuxbrew/Cellar/libxcb/1.17.0/include/
icpx -DRTM_ENABLE_EMBREE_GPU -fsycl -fsycl-targets=spir64 -Xclang -fsycl-allow-func-ptr -I/opt/intel-embree/include -L/opt/intel-embree/build/ main.cpp *.o -lembree4_sycl -lembree4 -I/home/linuxbrew/.linuxbrew/include/ -I/home/linuxbrew/.linuxbrew/Cellar/xorgproto/2025.1/include/ -I/var/home/linuxbrew/.linuxbrew/Cellar/libxcb/1.17.0/include/ /home/linuxbrew/.linuxbrew/Cellar/glfw/3.5.1/lib/libglfw3.a /home/linuxbrew/.linuxbrew/lib/libX11.so /home/linuxbrew/.linuxbrew/lib/libvulkan.so -lm -o a.out
LD_LIBRARY_PATH=/opt/intel-embree/build/:$LD_LIBRARY_PATH ./a.out
#endif
#if 0 // CPU
# Build and run commands:
source /opt/intel/oneapi/setvars.sh intel64
icx -c ../../../vkfast.c "../../../extra/CPU GPU Array/vkfast_extra_cpu_gpu_array.c" ../../../extra/REII/vkfast_extra_reii.c /home/linuxbrew/RedGpuSDK/redgpu.c /home/linuxbrew/RedGpuSDK/redgpu_2.c /home/linuxbrew/RedGpuSDK/redgpu_32.c -I/home/linuxbrew/.linuxbrew/include/ -I/home/linuxbrew/.linuxbrew/Cellar/xorgproto/2025.1/include/ -I/var/home/linuxbrew/.linuxbrew/Cellar/libxcb/1.17.0/include/
icpx -DRTM_ENABLE_EMBREE_CPU -I/opt/intel-embree/include -L/opt/intel-embree/build/ main.cpp *.o -lembree4 -ltbb -I/home/linuxbrew/.linuxbrew/include/ -I/home/linuxbrew/.linuxbrew/Cellar/xorgproto/2025.1/include/ -I/var/home/linuxbrew/.linuxbrew/Cellar/libxcb/1.17.0/include/ /home/linuxbrew/.linuxbrew/Cellar/glfw/3.5.1/lib/libglfw3.a /home/linuxbrew/.linuxbrew/lib/libX11.so /home/linuxbrew/.linuxbrew/lib/libvulkan.so -lm -o a.out
LD_LIBRARY_PATH=/opt/intel-embree/build/:$LD_LIBRARY_PATH ./a.out
#endif

#include <iostream>
#include <vector>
#include <limits>

// NOTE(Constantine)(Sep 18, 2026): X11/X.h defines None, so we include ray_tracing_macroroni.h before vkFast.
#include "../Common/ray_tracing_macroroni.h"

#include "../../../vkfast.h"
#include "../../../extra/REII/vkfast_extra_reii.h"
#include "../../../extra/vkFast Extensions/ReBAR/vkfast_ext_rebar.h"
#define VKFAST_EXAMPLES_COMMON_INCLUDE_GLFW3
#include "../../../examples/Common/vkfast_examples_common.h"

typedef struct float3 {
  float x, y, z;
} float3;

typedef struct uint3 {
  unsigned x, y, z;
} uint3;

typedef struct vec3 {
  float x, y, z;

  vec3 operator+(const vec3 & v) const { return {x + v.x, y + v.y, z + v.z}; }
  vec3 operator-(const vec3 & v) const { return {x - v.x, y - v.y, z - v.z}; }
  vec3 operator*(float scalar)   const { return {x * scalar, y * scalar, z * scalar}; }

  float dot(const vec3 & v) const { return x * v.x + y * v.y + z * v.z; }

  vec3 normalize() const {
    float len = std::sqrt(x*x + y*y + z*z);
    return len > 0.0f ? vec3{x / len, y / len, z / len} : vec3{0, 0, 0};
  }
} vec3;

void embreeErrorCallback(void * userPtr, RTCError code, const char * str) {
  std::cerr << "Embree Error: " << str << " (Code: " << code << ")" << std::endl;
  vfExit(1);
}

ssize_t gEmbreeSimpleMemoryLeakDetectionTracker = 0;

bool embreeMemoryMonitorCallback(void* userPtr, ssize_t bytes, bool post) {
  if (bytes > 0) {
    // Allocation
    //std::cout << "Embree allocated: " << bytes << " bytes\n";
    gEmbreeSimpleMemoryLeakDetectionTracker += bytes;
  } else if (bytes < 0) {
    // Deallocation
    //std::cout << "Embree freed: " << -bytes << " bytes\n";
    gEmbreeSimpleMemoryLeakDetectionTracker += bytes;
  }
  return true; // Return false to abort allocation if running out of memory
}

int main() {
  #define WIDTH 1920
  #define HEIGHT 1080

  int windowMonitorArea[4] = {0};
  vfGetMainMonitorAreaRectangle(windowMonitorArea, FF, LL);
  REDGPU_2_EXPECTFL(windowMonitorArea[2] == WIDTH);
  REDGPU_2_EXPECTFL(windowMonitorArea[3] == HEIGHT);

  gpu_handle_context_t ctx = vfContextInitNoDefaultAllocs(1, NULL, FF, LL);
  vfWindowFullscreen(ctx, NULL, "[vkFast] Suzanne Head Xformed Embree REII", WIDTH, HEIGHT, 0, RED_PRESENT_VSYNC_MODE_ON, FF, LL);

  const unsigned array65536[2] = {65536, 65536};

  gpu_thread_t gpu_thread = NULL;
  vfGpuThreadCreate(ctx, 1, &gpu_thread, NULL, FF, LL);

  struct Pixels {
    unsigned char pixels[HEIGHT][WIDTH][4];
  };
  VfeReBARMallocShared pixelsHandles = {};
  volatile struct Pixels * pix = (volatile struct Pixels *)vfeReBARMallocShared(ctx, sizeof(struct Pixels), &pixelsHandles);

  // Sycl start

  RTM_CREATE_EMBREE_DEVICE_AND_SYCL_QUEUE("memory_monitor=1")
  rtcSetDeviceMemoryMonitorFunction(rtm_rtc_device, embreeMemoryMonitorCallback, NULL);

  std::cout << "Running ray tracing on device: " << rtmSyclQueueGetDeviceName(rtm_sycl_queue) << std::endl;

  if (!rtm_rtc_device) {
    std::cerr << "Failed to create Embree device\n";
    return 1;
  }
  rtcSetDeviceErrorFunction(rtm_rtc_device, embreeErrorCallback, NULL);

  // 2. Create the Embree meshScene and geometry
  RTCScene meshScene = rtcNewScene(rtm_rtc_device);
  RTCGeometry geom = rtcNewGeometry(rtm_rtc_device, RTC_GEOMETRY_TYPE_TRIANGLE);

  float suzanne_head_mesh_vertices[] = {
    #include "../../../extra/3D Mesh Suzanne Head/3d_mesh_vertices_suzanne_head.h"
  };

  // 3. Allocate Shared/USM Memory for Vertices and Indices
  float3 * vertices = (float3 *)rtmMallocShared(2904 * sizeof(float3), rtm_sycl_queue);
  uint3 * indices = (uint3 *)rtmMallocShared((2904/3) * sizeof(uint3), rtm_sycl_queue);

  for (int i = 0, counter = 0; i < 2904; i += 1) {
    const float scale = 0.5f;
    vertices[i].x = suzanne_head_mesh_vertices[i * 3 + 0] * scale;
    vertices[i].y = suzanne_head_mesh_vertices[i * 3 + 1] * scale;
    vertices[i].z = suzanne_head_mesh_vertices[i * 3 + 2] * scale;
  }
  for (int i = 0, counter = 0; i < (2904/3); i += 1) {
    indices[i].x = counter++;
    indices[i].y = counter++;
    indices[i].z = counter++;
  }

  // Assign buffers to Embree geometry
  rtcSetSharedGeometryBuffer(geom, RTC_BUFFER_TYPE_VERTEX, 0, RTC_FORMAT_FLOAT3, vertices, 0, sizeof(float3), 2904);
  rtcSetSharedGeometryBuffer(geom, RTC_BUFFER_TYPE_INDEX, 0, RTC_FORMAT_UINT3, indices, 0, sizeof(uint3), (2904/3));

  rtcCommitGeometry(geom);

  rtcAttachGeometry(meshScene, geom);
  rtcReleaseGeometry(geom);
  rtcCommitScene(meshScene);

  RTCGeometry instanceGeom = rtcNewGeometry(rtm_rtc_device, RTC_GEOMETRY_TYPE_INSTANCE);
  rtcSetGeometryInstancedScene(instanceGeom, meshScene);

  float m3x4[3*4] = {0};

  // Column 0
  m3x4[0]  = 1.0f;
  m3x4[1]  = 0.0f;
  m3x4[2]  = 0.0f;

  // Column 1
  m3x4[3]  = 0.0f;
  m3x4[4]  = 1.0f;
  m3x4[5]  = 0.0f;

  // Column 2
  m3x4[6]  = 0.0f;
  m3x4[7]  = 0.0f;
  m3x4[8]  = 1.0f;

  // Column 3 (Translation vector)
  m3x4[9]  = 0.0f;
  m3x4[10] = 0.0f;
  m3x4[11] = 2.0f;

  rtcSetGeometryTransform(instanceGeom, 0, RTC_FORMAT_FLOAT3X4_COLUMN_MAJOR, m3x4);
  rtcCommitGeometry(instanceGeom);

  RTCScene parentScene = rtcNewScene(rtm_rtc_device);
  rtcAttachGeometry(parentScene, instanceGeom);
  rtcReleaseGeometry(instanceGeom);
  rtcCommitScene(parentScene);

  // Get traversable handle needed for rtm_rtc_device-side tracing in Embree 4
  RTCTraversable traversable = rtcGetSceneTraversable(parentScene);

  // 4. Create an output frame buffer using USM shared allocation
  uint8_t * pixels = (uint8_t *)rtmMallocShared(WIDTH * HEIGHT * 4, rtm_sycl_queue);

  while (vfWindowLoop(ctx)) {
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

      vec3 rayOrg = {rayhit.ray.org_x, rayhit.ray.org_y, rayhit.ray.org_z};
      vec3 rayDir = {rayhit.ray.dir_x, rayhit.ray.dir_y, rayhit.ray.dir_z};

      // Configure intersection arguments
      RTCIntersectArguments args;
      rtcInitIntersectArguments(&args);
      args.feature_mask = RTC_FEATURE_FLAG_TRIANGLE; // Performance flag optimization

      // Embree 4 Device-side Intersection
      rtcTraversableIntersect1(traversable, &rayhit, &args);

      int pixel_index = (y * WIDTH + x) * 4;
      if (rayhit.hit.geomID != RTC_INVALID_GEOMETRY_ID) {
        // Retrieve the unnormalized geometry normal returned by Embree
        vec3 normal = { rayhit.hit.Ng_x, rayhit.hit.Ng_y, rayhit.hit.Ng_z };
        normal = normal.normalize();

        // Calculate the exact world space hit point
        vec3 hitPoint = rayOrg + rayDir * rayhit.ray.tfar;

        // Define a simple directional light source
        vec3 lightDir = vec3{1.0f, -0.5f, 0.5f}.normalize();
        vec3 lightColor = {1.0f, 1.0f, 1.0f};
        vec3 objectColor = {1.f, 0.5f, 0.f}; // Orange

        // Lambertian diffuse lighting calculation: max(0, N . L)
        float cosTheta = std::max(0.0f, normal.dot(lightDir));
        vec3 diffuseShading = objectColor * cosTheta * 0.8f;

        // Add a small ambient value to keep unlit areas visible
        vec3 ambientShading = objectColor * 0.2f;

        vec3 color = diffuseShading + ambientShading;

        pixels[pixel_index + 0] = color.z * 255.f; // Blue
        pixels[pixel_index + 1] = color.y * 255.f; // Green
        pixels[pixel_index + 2] = color.x * 255.f; // Red
        pixels[pixel_index + 3] = 255;             // Alpha
      } else {
        // Background color (Dark Gray)
        pixels[pixel_index + 0] = 32;
        pixels[pixel_index + 1] = 32;
        pixels[pixel_index + 2] = 32;
        pixels[pixel_index + 3] = 255;
      }
    }
    RTM_TBB_SYCL_PARALLEL_FOR_XY_END()

    // 6. Wait for execution to finish
    rtmSyclQueueWait(rtm_sycl_queue);

    // 7. Copy pixels and draw
    memcpy((void *)&pix->pixels[0][0][0], pixels, WIDTH * HEIGHT * 4);

    gpu_thread_t gpu_threads[2] = {gpu_thread, 0};
    vfAsyncDrawPixelsRaw(ctx, &pixelsHandles.storageRaw, NULL, 2, gpu_threads, array65536, FF, LL);
  }

  // 8. Cleanup
  rtmFree(pixels, rtm_sycl_queue);
  rtmFree(vertices, rtm_sycl_queue);
  rtmFree(indices, rtm_sycl_queue);
  rtcReleaseScene(parentScene);
  rtcReleaseScene(meshScene);
  rtcReleaseDevice(rtm_rtc_device);
  vfAllQueuesWaitIdle(ctx, FF, LL);

  vfGpuThreadDestroy(ctx, gpu_thread);
  vfeReBARFreeShared(ctx, &pixelsHandles);
  vfContextDeinit(ctx, FF, LL);

  if (gEmbreeSimpleMemoryLeakDetectionTracker != 0) {
    std::cerr << "Embree Simple Memory Leak Detection Tracker Error: gEmbreeSimpleMemoryLeakDetectionTracker is " << gEmbreeSimpleMemoryLeakDetectionTracker << std::endl;
  }

  vfExit(0);
}

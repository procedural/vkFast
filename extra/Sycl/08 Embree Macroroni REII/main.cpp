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

void embreeErrorCallback(void * userPtr, RTCError code, const char * str) {
  std::cerr << "Embree Error: " << str << " (Code: " << code << ")" << std::endl;
  vfExit(1);
}

int main() {
  #define WIDTH 1920
  #define HEIGHT 1080

  int windowMonitorArea[4] = {0};
  vfGetMainMonitorAreaRectangle(windowMonitorArea, FF, LL);
  REDGPU_2_EXPECTFL(windowMonitorArea[2] == WIDTH);
  REDGPU_2_EXPECTFL(windowMonitorArea[3] == HEIGHT);

  gpu_handle_context_t ctx = vfContextInitNoDefaultAllocs(1, NULL, FF, LL);
  vfWindowFullscreen(ctx, NULL, "[vkFast] Embree Macroroni REII", WIDTH, HEIGHT, 0, RED_PRESENT_VSYNC_MODE_ON, FF, LL);

  const unsigned array65536[2] = {65536, 65536};

  gpu_thread_t gpu_thread = NULL;
  vfGpuThreadCreate(ctx, 1, &gpu_thread, NULL, FF, LL);

  struct Pixels {
    unsigned char pixels[HEIGHT][WIDTH][4];
  };
  VfeReBARMallocShared pixelsHandles = {};
  volatile struct Pixels * pix = (volatile struct Pixels *)vfeReBARMallocShared(ctx, sizeof(struct Pixels), &pixelsHandles);

  // Sycl start

  RTM_CREATE_EMBREE_DEVICE_AND_SYCL_QUEUE(NULL)

  std::cout << "Running ray tracing on device: " << rtmSyclQueueGetDeviceName(rtm_sycl_queue) << std::endl;

  if (!rtm_rtc_device) {
    std::cerr << "Failed to create Embree device\n";
    return 1;
  }
  rtcSetDeviceErrorFunction(rtm_rtc_device, embreeErrorCallback, NULL);

  // 2. Create the Embree scene and geometry
  RTCScene scene = rtcNewScene(rtm_rtc_device);
  RTCGeometry geom = rtcNewGeometry(rtm_rtc_device, RTC_GEOMETRY_TYPE_TRIANGLE);

  // 3. Allocate Shared/USM Memory for Vertices and Indices
  float * vertices = (float *)rtmMallocShared(3 * 3 * sizeof(float), rtm_sycl_queue);
  unsigned int * indices = (unsigned int *)rtmMallocShared(3 * sizeof(unsigned int), rtm_sycl_queue);

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

      // Configure intersection arguments
      RTCIntersectArguments args;
      rtcInitIntersectArguments(&args);
      args.feature_mask = RTC_FEATURE_FLAG_TRIANGLE; // Performance flag optimization

      // Embree 4 Device-side Intersection
      rtcTraversableIntersect1(traversable, &rayhit, &args);

      int pixel_index = (y * WIDTH + x) * 4;
      if (rayhit.hit.geomID != RTC_INVALID_GEOMETRY_ID) {
        // Shading code if hit (Orange Triangle)
        pixels[pixel_index + 0] = 0;   // Blue
        pixels[pixel_index + 1] = 128; // Green
        pixels[pixel_index + 2] = 255; // Red
        pixels[pixel_index + 3] = 255; // Alpha
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
  rtcReleaseScene(scene);
  rtcReleaseDevice(rtm_rtc_device);
  vfAllQueuesWaitIdle(ctx, FF, LL);

  vfGpuThreadDestroy(ctx, gpu_thread);
  vfeReBARFreeShared(ctx, &pixelsHandles);
  vfContextDeinit(ctx, FF, LL);
  vfExit(0);
}

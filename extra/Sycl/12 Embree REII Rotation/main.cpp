#if 0 // GPU
# Build and run commands:
source /opt/intel/oneapi/setvars.sh intel64
icx -c ../../../vkfast.c "../../../extra/CPU GPU Array/vkfast_extra_cpu_gpu_array.c" ../../../extra/REII/vkfast_extra_reii.c /home/linuxbrew/RedGpuSDK/redgpu.c /home/linuxbrew/RedGpuSDK/redgpu_2.c /home/linuxbrew/RedGpuSDK/redgpu_32.c -I/home/linuxbrew/.linuxbrew/include/ -I/home/linuxbrew/.linuxbrew/Cellar/xorgproto/2025.1/include/ -I/var/home/linuxbrew/.linuxbrew/Cellar/libxcb/1.17.0/include/
icpx -DRTM_ENABLE_EMBREE_GPU -fsycl -fsycl-targets=spir64 -Xclang -fsycl-allow-func-ptr -I/opt/intel-embree/include -L/opt/intel-embree/build/ main.cpp *.o -lembree4_sycl -lembree4 -I/home/linuxbrew/.linuxbrew/include/ -I/home/linuxbrew/.linuxbrew/Cellar/xorgproto/2025.1/include/ -I/var/home/linuxbrew/.linuxbrew/Cellar/libxcb/1.17.0/include/ /home/linuxbrew/.linuxbrew/Cellar/glfw/3.5.1/lib/libglfw3.a /home/linuxbrew/.linuxbrew/lib/libX11.so /home/linuxbrew/.linuxbrew/lib/libvulkan.so -lm -o a.out
XDG_SESSION_TYPE=x11 LD_LIBRARY_PATH=/opt/intel-embree/build/:$LD_LIBRARY_PATH ./a.out
#endif
#if 0 // CPU
# Build and run commands:
source /opt/intel/oneapi/setvars.sh intel64
icx -c ../../../vkfast.c "../../../extra/CPU GPU Array/vkfast_extra_cpu_gpu_array.c" ../../../extra/REII/vkfast_extra_reii.c /home/linuxbrew/RedGpuSDK/redgpu.c /home/linuxbrew/RedGpuSDK/redgpu_2.c /home/linuxbrew/RedGpuSDK/redgpu_32.c -I/home/linuxbrew/.linuxbrew/include/ -I/home/linuxbrew/.linuxbrew/Cellar/xorgproto/2025.1/include/ -I/var/home/linuxbrew/.linuxbrew/Cellar/libxcb/1.17.0/include/
icpx -DRTM_ENABLE_EMBREE_CPU -I/opt/intel-embree/include -L/opt/intel-embree/build/ main.cpp *.o -lembree4 -ltbb -I/home/linuxbrew/.linuxbrew/include/ -I/home/linuxbrew/.linuxbrew/Cellar/xorgproto/2025.1/include/ -I/var/home/linuxbrew/.linuxbrew/Cellar/libxcb/1.17.0/include/ /home/linuxbrew/.linuxbrew/Cellar/glfw/3.5.1/lib/libglfw3.a /home/linuxbrew/.linuxbrew/lib/libX11.so /home/linuxbrew/.linuxbrew/lib/libvulkan.so -lm -o a.out
XDG_SESSION_TYPE=x11 LD_LIBRARY_PATH=/opt/intel-embree/build/:$LD_LIBRARY_PATH ./a.out
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

void quaternion_to_matrix3x3_col_major(const float * q, float * m) {
  // Assumes q is a normalized quaternion in the order: [x, y, z, w]
  float x = q[0], y = q[1], z = q[2], w = q[3];

  float xx = x * x;
  float xy = x * y;
  float xz = x * z;
  float xw = x * w;

  float yy = y * y;
  float yz = y * z;
  float yw = y * w;

  float zz = z * z;
  float zw = z * w;

  // Column 0
  m[0] = 1.0f - 2.0f * (yy + zz);
  m[1] = 2.0f * (xy + zw);
  m[2] = 2.0f * (xz - yw);

  // Column 1
  m[3] = 2.0f * (xy - zw);
  m[4] = 1.0f - 2.0f * (xx + zz);
  m[5] = 2.0f * (yz + xw);

  // Column 2
  m[6] = 2.0f * (xz + yw);
  m[7] = 2.0f * (yz - xw);
  m[8] = 1.0f - 2.0f * (xx + yy);
}

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
  REDGPU_2_EXPECTFL(RTC_MAX_INSTANCE_LEVEL_COUNT >= 3 || !"Recompile Embree with -DEMBREE_MAX_INSTANCE_LEVEL_COUNT=3");

  #define WIDTH 700
  #define HEIGHT 700

  glfwInit();
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
  GLFWwindow * window = glfwCreateWindow(WIDTH, HEIGHT, "[vkFast] Embree REII Rotation", 0, 0);
  #if defined(_WIN32)
  void * window_handle = (void *)glfwGetWin32Window(window);
  #elif defined(__linux__) && !defined(__ANDROID__)
  // NOTE(Constantine): this struct's layout is defined in redgpu_32.c file of REDGPU 2 SDK.
  struct X11WindowData {
    Display * display;
    Window    window;
    Atom      wmDeleteMessage;
  };
  struct X11WindowData windowData = {0};
  windowData.display = glfwGetX11Display();
  windowData.window = glfwGetX11Window(window);
  windowData.wmDeleteMessage = 0;
  REDGPU_2_EXPECTFL(windowData.display != NULL || !"On Wayland, you need to run the app like this: XDG_SESSION_TYPE=x11 ./a.out");
  REDGPU_2_EXPECTFL(windowData.window  != 0    || !"On Wayland, you need to run the app like this: XDG_SESSION_TYPE=x11 ./a.out");
  void * window_handle = &windowData;
  #endif

  gpu_handle_context_t ctx = vfContextInitNoDefaultAllocs(1, NULL, FF, LL);
  vfWindowFullscreen(ctx, window_handle, "[vkFast] Embree REII Rotation", WIDTH, HEIGHT, 0, RED_PRESENT_VSYNC_MODE_ON, FF, LL);

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

  std::cout << "Running ray tracing on device: " << rtmSyclQueueGetDeviceName(rtm_sycl_queue) << std::endl;

  if (!rtm_rtc_device) {
    std::cerr << "Failed to create Embree device\n";
    return 1;
  }
  rtcSetDeviceErrorFunction(rtm_rtc_device, embreeErrorCallback, NULL);
  rtcSetDeviceMemoryMonitorFunction(rtm_rtc_device, embreeMemoryMonitorCallback, NULL);

  // Create Suzanne Head mesh
  float suzanne_head_mesh_vertices[] = {
    #include "../../../extra/3D Mesh Suzanne Head/3d_mesh_vertices_suzanne_head.h"
  };

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

  RTCGeometry suzanneMeshGeom = rtcNewGeometry(rtm_rtc_device, RTC_GEOMETRY_TYPE_TRIANGLE);
  rtcSetSharedGeometryBuffer(suzanneMeshGeom, RTC_BUFFER_TYPE_VERTEX, 0, RTC_FORMAT_FLOAT3, vertices, 0, sizeof(float3), 2904);
  rtcSetSharedGeometryBuffer(suzanneMeshGeom, RTC_BUFFER_TYPE_INDEX, 0, RTC_FORMAT_UINT3, indices, 0, sizeof(uint3), (2904/3));
  rtcCommitGeometry(suzanneMeshGeom);

  RTCScene suzanneMeshScene = rtcNewScene(rtm_rtc_device);
  rtcAttachGeometry(suzanneMeshScene, suzanneMeshGeom);
  rtcCommitScene(suzanneMeshScene);

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
  m3x4[11] = 0.0f;

  // Create an instance of Suzanne Head mesh
  RTCGeometry suzanneInstanceGeom = rtcNewGeometry(rtm_rtc_device, RTC_GEOMETRY_TYPE_INSTANCE);
  rtcSetGeometryInstancedScene(suzanneInstanceGeom, suzanneMeshScene);
  rtcSetGeometryTransform(suzanneInstanceGeom, 0, RTC_FORMAT_FLOAT3X4_COLUMN_MAJOR, m3x4);
  rtcCommitGeometry(suzanneInstanceGeom);

  RTCScene suzanneInstanceScene = rtcNewScene(rtm_rtc_device);
  unsigned suzanneInstanceId = rtcAttachGeometry(suzanneInstanceScene, suzanneInstanceGeom);
  rtcCommitScene(suzanneInstanceScene);

  // Create a world xform 1 instance of all mesh xform 0 instances
  RTCGeometry worldXform1Geom = rtcNewGeometry(rtm_rtc_device, RTC_GEOMETRY_TYPE_INSTANCE);
  rtcSetGeometryInstancedScene(worldXform1Geom, suzanneInstanceScene);
  //...
  rtcSetGeometryTransform(worldXform1Geom, 0, RTC_FORMAT_FLOAT3X4_COLUMN_MAJOR, m3x4);
  rtcCommitGeometry(worldXform1Geom);

  RTCScene worldXform1InstanceScene = rtcNewScene(rtm_rtc_device);
  unsigned worldXform1InstanceId = rtcAttachGeometry(worldXform1InstanceScene, worldXform1Geom);
  rtcCommitScene(worldXform1InstanceScene);

  // Create a world xform 2 instance of world xform 1 instance
  RTCGeometry worldXform2Geom = rtcNewGeometry(rtm_rtc_device, RTC_GEOMETRY_TYPE_INSTANCE);
  rtcSetGeometryInstancedScene(worldXform2Geom, worldXform1InstanceScene);
  rtcSetGeometryTransform(worldXform2Geom, 0, RTC_FORMAT_FLOAT3X4_COLUMN_MAJOR, m3x4);
  rtcCommitGeometry(worldXform2Geom);

  RTCScene worldXform2InstanceScene = rtcNewScene(rtm_rtc_device);
  unsigned worldXform2InstanceId = rtcAttachGeometry(worldXform2InstanceScene, worldXform2Geom);
  rtcCommitScene(worldXform2InstanceScene);

  // Update Suzanne Head's instance transform separately here
  {
    RTCGeometry geom = rtcGetGeometry(suzanneInstanceScene, suzanneInstanceId);
    // Column 3 (Translation vector)
    m3x4[9]  = 0.0f;
    m3x4[10] = 0.0f;
    m3x4[11] = 0.0f;
    rtcSetGeometryTransform(geom, 0, RTC_FORMAT_FLOAT3X4_COLUMN_MAJOR, m3x4);
    rtcCommitGeometry(geom);
    rtcCommitScene(suzanneInstanceScene);
  }

  // Get traversable handle needed for device-side tracing in Embree 4
  RTCTraversable traversable = rtcGetSceneTraversable(worldXform2InstanceScene);

  // Create an output frame buffer using USM shared allocation
  uint8_t * pixels = (uint8_t *)rtmMallocShared(WIDTH * HEIGHT * 4, rtm_sycl_queue);

  ReiiVec4   camera_pos  = {0, 0, -2.f};
  ReiiVec4   camera_quat = {0, 0, 0, 1};
  ReiiBool32 camera_is_enabled = 1;
  if (camera_is_enabled == 1) {
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  }

  glfwPollEvents();
  double mouse_x = 0;
  double mouse_y = 0;
  glfwGetCursorPos(window, &mouse_x, &mouse_y);
  double mouse_x_prev = mouse_x;
  double mouse_y_prev = mouse_y;
  int    mouse_right_mouse_button_state_prev = 0;

  while (glfwWindowShouldClose(window) == 0) {
    glfwPollEvents();

    int os_window_w = 0;
    int os_window_h = 0;
    glfwGetWindowSize(window, &os_window_w, &os_window_h);

    if (vfWindowIsMinimized(ctx) || os_window_w == 0 || os_window_h == 0) {
      continue;
    }

    glfwGetCursorPos(window, &mouse_x, &mouse_y);

    int mouse_right_mouse_button_state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2);

    // NOTE(Constantine):
    // Camera quaternion rotation and translation.
    const float mouse_move_sensitivity = 0.0035f;
    const float camera_move_speed      = 0.1f;
    if (mouse_right_mouse_button_state == GLFW_PRESS && mouse_right_mouse_button_state != mouse_right_mouse_button_state_prev) {
      camera_is_enabled = !camera_is_enabled;
      glfwSetInputMode(window, GLFW_CURSOR, camera_is_enabled == 1 ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
    } else if (camera_is_enabled == 1) {
      float mouse_move_x = (float)(mouse_x - mouse_x_prev) * mouse_move_sensitivity;
      float mouse_move_y = (float)(mouse_y - mouse_y_prev) * mouse_move_sensitivity;

      float key_f = glfwGetKey(window, GLFW_KEY_W);
      float key_b = glfwGetKey(window, GLFW_KEY_S);

      float key_r = glfwGetKey(window, GLFW_KEY_D);
      float key_l = glfwGetKey(window, GLFW_KEY_A);

      float key_u = glfwGetKey(window, GLFW_KEY_E);
      float key_d = glfwGetKey(window, GLFW_KEY_Q);

      float rot_x[4];
      float rot_y[4];

      float axis_x[3] = {1, 0, 0};
      float axis_y[3] = {0, 1, 0};
      quatFromAxisAngle(rot_y, axis_y, mouse_move_x);
      quatFromAxisAngle(rot_x, axis_x, mouse_move_y);

      quatMul(&camera_quat.x, &camera_quat.x, rot_x);
      quatMul(&camera_quat.x, rot_y, &camera_quat.x);

      float side_vec[3] = {1, 0, 0};
      float   up_vec[3] = {0, 1, 0};
      float  dir_vec[3] = {0, 0, 1};
      quatRotateVec3Fast(side_vec, side_vec, &camera_quat.x);
      quatRotateVec3Fast(  up_vec,   up_vec, &camera_quat.x);
      quatRotateVec3Fast( dir_vec,  dir_vec, &camera_quat.x);

      vec3Mulf(side_vec, side_vec, key_r - key_l);
      vec3Mulf(  up_vec,   up_vec, key_u - key_d);
      vec3Mulf( dir_vec,  dir_vec, key_f - key_b);

      float move_vec_normalized[3] = {0, 0, 0};

      vec3Add(move_vec_normalized, move_vec_normalized, side_vec);
      vec3Add(move_vec_normalized, move_vec_normalized,   up_vec);
      vec3Add(move_vec_normalized, move_vec_normalized,  dir_vec);

      float move_vec_len = sqrtf(
        move_vec_normalized[0] * move_vec_normalized[0] +
        move_vec_normalized[1] * move_vec_normalized[1] +
        move_vec_normalized[2] * move_vec_normalized[2]
      );
      if (move_vec_len != 0) {
        move_vec_normalized[0] /= move_vec_len;
        move_vec_normalized[1] /= move_vec_len;
        move_vec_normalized[2] /= move_vec_len;
      }

      vec3Mulf(move_vec_normalized, move_vec_normalized, camera_move_speed);

      vec3Add(&camera_pos.x, &camera_pos.x, move_vec_normalized);
    }

    // Rotate the world
    {
      RTCGeometry geom = rtcGetGeometry(worldXform2InstanceScene, worldXform2InstanceId);

      // Column 3 (Translation vector)
      m3x4[9]  = 0;
      m3x4[10] = 0;
      m3x4[11] = 0;

      ReiiVec4 camera_quat_inv = {-camera_quat.x, -camera_quat.y, -camera_quat.z, camera_quat.w};
      quaternion_to_matrix3x3_col_major(&camera_quat_inv.x, &m3x4[0]);

      rtcSetGeometryTransform(geom, 0, RTC_FORMAT_FLOAT3X4_COLUMN_MAJOR, m3x4);
      rtcCommitGeometry(geom);
      rtcCommitScene(worldXform2InstanceScene);
    }

    // Translate the world
    {
      RTCGeometry geom = rtcGetGeometry(worldXform1InstanceScene, worldXform1InstanceId);

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
      m3x4[9]  = -camera_pos.x;
      m3x4[10] = -camera_pos.y;
      m3x4[11] = -camera_pos.z;

      rtcSetGeometryTransform(geom, 0, RTC_FORMAT_FLOAT3X4_COLUMN_MAJOR, m3x4);
      rtcCommitGeometry(geom);
      rtcCommitScene(worldXform1InstanceScene);
    }

    #ifdef RTM_ENABLE_EMBREE_GPU
    auto syclQueueSubmitEvent =
    #endif
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
      rayhit.hit.instID[1] = RTC_INVALID_GEOMETRY_ID;
      rayhit.hit.instID[2] = RTC_INVALID_GEOMETRY_ID;

      // Configure intersection arguments
      RTCIntersectArguments args;
      rtcInitIntersectArguments(&args);
      args.feature_mask = (RTCFeatureFlags)(RTC_FEATURE_FLAG_TRIANGLE | RTC_FEATURE_FLAG_INSTANCE);

      // Embree 4 Device-side Intersection
      rtcTraversableIntersect1(traversable, &rayhit, &args);

      vec3 rayOrg = {rayhit.ray.org_x, rayhit.ray.org_y, rayhit.ray.org_z};
      vec3 rayDir = {rayhit.ray.dir_x, rayhit.ray.dir_y, rayhit.ray.dir_z};

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

    // Wait for execution to finish
    rtmSyclQueueWait(rtm_sycl_queue);

    #ifdef RTM_ENABLE_EMBREE_GPU
    if (0) {
      auto end   = syclQueueSubmitEvent.get_profiling_info<sycl::info::event_profiling::command_end>();
      auto start = syclQueueSubmitEvent.get_profiling_info<sycl::info::event_profiling::command_start>();

      std::cout << "Sycl queue submit elapsed time: " << (end - start) / 1000.0 << " microseconds.\n";
    }
    #endif

    // Copy pixels and draw
    memcpy((void *)&pix->pixels[0][0][0], pixels, WIDTH * HEIGHT * 4);

    gpu_thread_t gpu_threads[2] = {gpu_thread, 0};
    vfAsyncDrawPixelsRaw(ctx, &pixelsHandles.storageRaw, NULL, 2, gpu_threads, array65536, FF, LL);

    mouse_x_prev = mouse_x;
    mouse_y_prev = mouse_y;
    mouse_right_mouse_button_state_prev = mouse_right_mouse_button_state;
  }

  vfAllQueuesWaitIdle(ctx, FF, LL);

  // Cleanup
  rtmFree(pixels, rtm_sycl_queue);
  rtmFree(vertices, rtm_sycl_queue);
  rtmFree(indices, rtm_sycl_queue);
  rtcReleaseScene(worldXform2InstanceScene);
  rtcReleaseScene(worldXform1InstanceScene);
  rtcReleaseScene(suzanneInstanceScene);
  rtcReleaseScene(suzanneMeshScene);
  rtcReleaseGeometry(worldXform2Geom);
  rtcReleaseGeometry(worldXform1Geom);
  rtcReleaseGeometry(suzanneInstanceGeom);
  rtcReleaseGeometry(suzanneMeshGeom);
  rtcReleaseDevice(rtm_rtc_device);

  vfGpuThreadDestroy(ctx, gpu_thread);
  vfeReBARFreeShared(ctx, &pixelsHandles);
  vfContextDeinit(ctx, FF, LL);
  glfwTerminate();

  if (gEmbreeSimpleMemoryLeakDetectionTracker != 0) {
    std::cerr << "Embree Simple Memory Leak Detection Tracker Error: gEmbreeSimpleMemoryLeakDetectionTracker is " << gEmbreeSimpleMemoryLeakDetectionTracker << std::endl;
  }

  vfExit(0);
}

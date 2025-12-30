#if 0
cd vs2019
cl /EHsc ../main.cpp ../../../vkfast.c C:/RedGpuSDK/redgpu.c C:/RedGpuSDK/redgpu_2.c C:/RedGpuSDK/redgpu_32.c /arch:AVX2 /openmp /link /stack:64000000
exit
#endif

#include "../../vkfast.h"

#include <stdio.h> // For printf
#include <vector>  // For std::vector

#include <shellscalingapi.h>   // For SetProcessDpiAwareness
#pragma comment(lib, "shcore") // For SetProcessDpiAwareness

#ifdef _WIN32
#define GLFW_INCLUDE_NONE
#define GLFW_EXPOSE_NATIVE_WIN32
#include "glfw-3.4.bin.WIN64/include/GLFW/glfw3.h" // https://github.com/glfw/glfw/releases/download/3.4/glfw-3.4.bin.WIN64.zip
#include "glfw-3.4.bin.WIN64/include/GLFW/glfw3native.h" // https://github.com/glfw/glfw/releases/download/3.4/glfw-3.4.bin.WIN64.zip
#pragma comment(lib, "../glfw-3.4.bin.WIN64/lib-vc2019/glfw3_mt.lib") // https://github.com/glfw/glfw/releases/download/3.4/glfw-3.4.bin.WIN64.zip
#pragma comment(lib, "User32.lib")
#pragma comment(lib, "Shell32.lib")
#pragma comment(lib, "Gdi32.lib")
#endif

#define TINYBVH_IMPLEMENTATION
#include "../../extra/Modified TinyBVH/tiny_bvh.h"
#include "../../extra/Modified RadeonRays 2.0/RadeonRays/include/radeon_rays.h"
#if 0
#include "../../extra/profile/profile.h" // https://github.com/procedural/profile
#pragma comment(lib, "../../extra/profile/profiledll.lib") // https://github.com/procedural/profile
#else
#define profileBegin(x)
#define profileEnd(x)
#endif

using namespace tinybvh;

#define countof(x) (sizeof(x) / sizeof((x)[0]))

#define FF __FILE__
#define LL __LINE__

void vec3Mulf(float * out, const float * v, float f) {
  out[0] = v[0] * f;
  out[1] = v[1] * f;
  out[2] = v[2] * f;
}

void vec3Add(float * out, const float * v1, const float * v2) {
  out[0] = v1[0] + v2[0];
  out[1] = v1[1] + v2[1];
  out[2] = v1[2] + v2[2];
}

void vec3Cross(float * out, const float * v1, const float * v2) {
  float v1x = v1[0];
  float v1y = v1[1];
  float v1z = v1[2];

  float v2x = v2[0];
  float v2y = v2[1];
  float v2z = v2[2];

  out[0] = (v1y * v2z) - (v2y * v1z);
  out[1] = (v1z * v2x) - (v2z * v1x);
  out[2] = (v1x * v2y) - (v2x * v1y);
}

void quatFromAxisAngle(float * out, const float * axis, float angle_rad) {
  float s = (float)sin(angle_rad / 2.f);
  float c = (float)cos(angle_rad / 2.f);

  out[0] = axis[0] * s;
  out[1] = axis[1] * s;
  out[2] = axis[2] * s;
  out[3] = c;
}

void quatMul(float * out, const float * q1, const float * q2) {
  float q1x = q1[0];
  float q1y = q1[1];
  float q1z = q1[2];
  float q1w = q1[3];

  float q2x = q2[0];
  float q2y = q2[1];
  float q2z = q2[2];
  float q2w = q2[3];

  out[0] = q1x * q2w + q2x * q1w + (q1y * q2z - q2y * q1z);
  out[1] = q1y * q2w + q2y * q1w + (q1z * q2x - q2z * q1x);
  out[2] = q1z * q2w + q2z * q1w + (q1x * q2y - q2x * q1y);
  out[3] = q1w * q2w - (q1x * q2x + q1y * q2y + q1z * q2z);
}

void quatRotateVec3(float * out, const float * v, const float * q) {
  float v4[4] = { v[0],  v[1],  v[2], 0};
  float nq[4] = {-q[0], -q[1], -q[2], q[3]};

  float t1[4];
  quatMul(t1, q, v4);
  float t2[4];
  quatMul(t2, t1, nq);
  
  out[0] = t2[0];
  out[1] = t2[1];
  out[2] = t2[2];
}

void quatRotateVec3Fast(float * out, const float * v, const float * q) {
  // (cross(q.xyz, cross(q.xyz, v) + (v * q.w)) * 2.f) + v
  float vc[3] = {v[0], v[1], v[2]};
  vec3Cross(out, q, vc);
  float t1[3];
  vec3Mulf(t1, vc, q[3]);
  vec3Add(out, out, t1);
  vec3Cross(out, q, out);
  vec3Mulf(out, out, 2.f);
  vec3Add(out, out, vc);
}

int main() {
#ifdef __MINGW32__
  SetProcessDPIAware();
#else
  SetProcessDpiAwareness(PROCESS_PER_MONITOR_DPI_AWARE);
#endif

#if defined(_MSC_VER) && defined(_DEBUG)
  _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

  LARGE_INTEGER frequency = {0};
  REDGPU_2_EXPECTFL(QueryPerformanceFrequency(&frequency) == TRUE); // Query the frequency (ticks per second)

  #define window_w 800 // NOTE(Constantine): Hardcoded.
  #define window_h 450 // NOTE(Constantine): Hardcoded.
  const float aspect_ratio = 9.f / 16.f; // NOTE(Constantine): Hardcoded.

  glfwInit();
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
  GLFWwindow * window = glfwCreateWindow(window_w, window_h, "[vkFast] RT 3D Mesh Suzanne GPU RR 2.0", 0, 0);
  void * window_handle = (void *)glfwGetWin32Window(window);

  gpu_handle_context_t ctx = vfContextInit(1, NULL, FF, LL);
  vfWindowFullscreen(ctx, window_handle, "[vkFast] RT 3D Mesh Suzanne GPU RR 2.0", 700, 700, 0, FF, LL);

  struct Pixels {
    unsigned char pixels[window_h][window_w][4];
  };
  // To free
  struct Pixels * pix = (struct Pixels *)red32MemoryCalloc(sizeof(struct Pixels));
  REDGPU_2_EXPECTFL(pix != NULL);

  float suzanne[] = {
    #include "../../extra/3D Mesh Suzanne Head/3d_mesh_vertices_suzanne_head.h"
  };
  
  const uint64_t vertex_count = (sizeof(suzanne) / sizeof(float)) / 3;
  // To free
  bvhvec4 * vertex = (bvhvec4 *)red32MemoryCalloc(sizeof(bvhvec4) * vertex_count);
  REDGPU_2_EXPECTFL(vertex != NULL);

  for (uint64_t i = 0; i < vertex_count; i += 1) {
    vertex[i].x = suzanne[i * 3 + 0];
    vertex[i].y = suzanne[i * 3 + 1];
    vertex[i].z = suzanne[i * 3 + 2];
    vertex[i].w = 0;
  }

  BVH8_CPU bvh;
  bvh.Build(vertex, vertex_count / 3);

  BVH tlas;
  BLASInstance instances[1];
  BVHBase * blas[1] = {&bvh};
  instances[0] = BLASInstance(0);
  instances[0].transform[0]  = 1.f; // Scale X
  instances[0].transform[5]  = 1.f; // Scale Y
  instances[0].transform[10] = 1.f; // Scale Z
  instances[0].transform[3]  = 0.f; // Translation X
  instances[0].transform[7]  = 0.f; // Translation Y
  instances[0].transform[11] = 0.f; // Translation Z
  tlas.Build(instances, countof(instances), blas, countof(blas));

  // NOTE(Constantine): MSAA
  // "19.2.4 Specification of Sample Positions":
  // https://microsoft.github.io/DirectX-Specs/d3d/archive/D3D11_3_FunctionalSpec.htm#19.2.4%20Specification%20of%20Sample%20Positions
  float msaa_offsets_table_x2_x[2]   = { 4, -4};
  float msaa_offsets_table_x2_y[2]   = { 4, -4};
  float msaa_offsets_table_x4_x[4]   = {-2,  6, -6,  2};
  float msaa_offsets_table_x4_y[4]   = {-6, -2,  2,  6};
  float msaa_offsets_table_x8_x[8]   = { 1, -1,  5, -3, -5, -7,  3,  7};
  float msaa_offsets_table_x8_y[8]   = {-3,  3,  1, -5,  5, -1,  7, -7};
  float msaa_offsets_table_x16_x[16] = { 1, -1, -3,  4, -5,  2,  5,  3, -2,  0, -4, -6, -8,  7,  6, -7};
  float msaa_offsets_table_x16_y[16] = { 1, -3,  2, -1, -2,  5,  3, -5,  6, -7, -6,  4,  0, -4,  7, -8};
  // Converting to a -0.5 to 0.5 range:
  for (int i = 0; i < 2;  i += 1) { msaa_offsets_table_x2_x[i]  /= 16.f; }
  for (int i = 0; i < 2;  i += 1) { msaa_offsets_table_x2_y[i]  /= 16.f; }
  for (int i = 0; i < 4;  i += 1) { msaa_offsets_table_x4_x[i]  /= 16.f; }
  for (int i = 0; i < 4;  i += 1) { msaa_offsets_table_x4_y[i]  /= 16.f; }
  for (int i = 0; i < 8;  i += 1) { msaa_offsets_table_x8_x[i]  /= 16.f; }
  for (int i = 0; i < 8;  i += 1) { msaa_offsets_table_x8_y[i]  /= 16.f; }
  for (int i = 0; i < 16; i += 1) { msaa_offsets_table_x16_x[i] /= 16.f; }
  for (int i = 0; i < 16; i += 1) { msaa_offsets_table_x16_y[i] /= 16.f; }

  #define msaaSamplesCount 1

  #if msaaSamplesCount == 1
  float msaa_samples_offset_table_x[1] = {0};
  float msaa_samples_offset_table_y[1] = {0};
  #endif
  #if msaaSamplesCount == 2
  float * msaa_samples_offset_table_x = &msaa_offsets_table_x2_x[0];
  float * msaa_samples_offset_table_y = &msaa_offsets_table_x2_y[0];
  #endif
  #if msaaSamplesCount == 4
  float * msaa_samples_offset_table_x = &msaa_offsets_table_x4_x[0];
  float * msaa_samples_offset_table_y = &msaa_offsets_table_x4_y[0];
  #endif
  #if msaaSamplesCount == 8
  float * msaa_samples_offset_table_x = &msaa_offsets_table_x8_x[0];
  float * msaa_samples_offset_table_y = &msaa_offsets_table_x8_y[0];
  #endif
  #if msaaSamplesCount == 16
  float * msaa_samples_offset_table_x = &msaa_offsets_table_x16_x[0];
  float * msaa_samples_offset_table_y = &msaa_offsets_table_x16_y[0];
  #endif

  bvhvec3 camera_axis_x = bvhvec3(1, 0, 0);
  bvhvec3 camera_axis_y = bvhvec3(0, 1, 0);
  bvhvec3 camera_axis_z = bvhvec3(0, 0, 1);

  float camera_pos[3]  = {0, 0, -5};
  float camera_quat[4] = {0, 0, 0, 1};

  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  glfwPollEvents();
  double mouse_x = 0;
  double mouse_y = 0;
  glfwGetCursorPos(window, &mouse_x, &mouse_y);
  double mouse_x_prev = mouse_x;
  double mouse_y_prev = mouse_y;

  // To free
  int * vertex_index = (int *)red32MemoryCalloc(sizeof(int) * 3 * vertex_count);
  REDGPU_2_EXPECTFL(vertex_index != NULL);
  // To free
  int * indexes_per_triangle_count = (int *)red32MemoryCalloc(sizeof(int) * vertex_count);
  REDGPU_2_EXPECTFL(indexes_per_triangle_count != NULL);
  for (uint64_t i = 0, index = 0; i < vertex_count; i += 1) {
    vertex_index[i * 3 + 0] = index++;
    vertex_index[i * 3 + 1] = index++;
    vertex_index[i * 3 + 2] = index++;
    indexes_per_triangle_count[i] = 3;
  }

  // Radeon Rays
  RadeonRays::IntersectionApi * api = RadeonRays::IntersectionApi::Create(0);
  REDGPU_2_EXPECTFL(api != NULL);
  RadeonRays::Shape * shape = api->CreateMesh(suzanne, vertex_count, 3 * sizeof(float), vertex_index, 0, indexes_per_triangle_count, vertex_count / 3);
  REDGPU_2_EXPECTFL(shape != NULL);
  api->AttachShape(shape);
  api->Commit();

  profileBegin("rays buffer allocations");
  RadeonRays::Buffer * rays_buffer = api->CreateBuffer(window_h * window_w * msaaSamplesCount * sizeof(RadeonRays::ray), NULL);
  RadeonRays::Buffer * hits_buffer = api->CreateBufferReadback(window_h * window_w * msaaSamplesCount * sizeof(RadeonRays::Intersection), NULL);
  profileEnd("rays buffer allocations");

  while (glfwWindowShouldClose(window) == 0) {
    profileBegin("frame");

    glfwPollEvents();

    glfwGetCursorPos(window, &mouse_x, &mouse_y);

    profileBegin("camera xforms");
    // NOTE(Constantine):
    // Camera quaternion rotation and translation.
    const float mouse_move_sensitivity = 0.0035f;
    const float camera_move_speed = 0.1f;
    {
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
  
      quatMul(camera_quat, camera_quat, rot_x);
      quatMul(camera_quat, rot_y, camera_quat);

      float side_vec[3] = {1, 0, 0};
      float   up_vec[3] = {0, 1, 0};
      float  dir_vec[3] = {0, 0, 1};
      quatRotateVec3Fast(side_vec, side_vec, camera_quat);
      quatRotateVec3Fast(  up_vec,   up_vec, camera_quat);
      quatRotateVec3Fast( dir_vec,  dir_vec, camera_quat);

      camera_axis_x.x = side_vec[0];
      camera_axis_x.y = side_vec[1];
      camera_axis_x.z = side_vec[2];

      camera_axis_y.x = up_vec[0];
      camera_axis_y.y = up_vec[1];
      camera_axis_y.z = up_vec[2];

      camera_axis_z.x = dir_vec[0];
      camera_axis_z.y = dir_vec[1];
      camera_axis_z.z = dir_vec[2];

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

      vec3Add(camera_pos, camera_pos, move_vec_normalized);
    }
    profileEnd("camera xforms");

    bvhvec3 up = camera_axis_y * aspect_ratio;
    bvhvec3 C  = 2 * camera_axis_z;
    bvhvec3 p1 = C - camera_axis_x + up;
    bvhvec3 p2 = C + camera_axis_x + up;
    bvhvec3 p3 = C - camera_axis_x - up;

    std::vector<RadeonRays::ray> rays_vector;

    rays_vector.resize(window_h * window_w * msaaSamplesCount);

    profileBegin("rays prepare");
    #pragma omp parallel for
    for (int y = 0; y < window_h; y += 1)
    {
      #pragma omp parallel for
      for (int x = 0; x < window_w; x += 1)
      {
        float sum = 0;
        RadeonRays::ray rays[msaaSamplesCount];
        for (int s = 0; s < msaaSamplesCount; s += 1) {
          float u = (float)(x + 0.5f + msaa_samples_offset_table_x[s]) / (float)(window_w);
          float v = (float)(y + 0.5f + msaa_samples_offset_table_y[s]) / (float)(window_h);
          bvhvec3 P = p1 + u * (p2 - p1) + v * (p3 - p1);
          bvhvec3 P_normalized = tinybvh_normalize(P);
          RadeonRays::ray ray;
          ray.o.x = camera_pos[0];
          ray.o.y = camera_pos[1];
          ray.o.z = camera_pos[2];
          ray.d.x = P_normalized.x;
          ray.d.y = P_normalized.y;
          ray.d.z = P_normalized.z;

          rays[s] = ray;
        }
        for (int s = 0; s < msaaSamplesCount; s += 1) {
          rays_vector[y * (uint64_t)window_w * msaaSamplesCount + x * msaaSamplesCount + s] = rays[s];
        }
      }
    }
    profileEnd("rays prepare");

    profileBegin("rays data write");
    {
      RadeonRays::ray * rays_write = NULL;
      {
        RadeonRays::Event * e = NULL;
        profileBegin("api->MapBuffer();");
        api->MapBuffer(rays_buffer, RadeonRays::kMapWrite, 0, rays_vector.size() * sizeof(RadeonRays::ray), (void **)&rays_write, &e);
        profileEnd("api->MapBuffer();");
        profileBegin("e->Wait();");
        e->Wait();
        profileEnd("e->Wait();");
        api->DeleteEvent(e);
        e = NULL;
      }
      #pragma omp parallel for
      for (int64_t i = 0; i < rays_vector.size(); i += 1) {
        rays_write[i] = rays_vector[i];
      }
      {
        RadeonRays::Event * e = NULL;
        profileBegin("api->UnmapBuffer();");
        api->UnmapBuffer(rays_buffer, rays_write, &e);
        profileEnd("api->UnmapBuffer();");
        profileBegin("e->Wait();");
        e->Wait();
        profileEnd("e->Wait();");
        api->DeleteEvent(e);
        e = NULL;
        delete[] rays_write;
        rays_write = NULL;
      }
    }
    profileEnd("rays data write");

    profileBegin("QueryIntersection");
    api->QueryIntersection(rays_buffer, rays_vector.size(), hits_buffer, NULL, NULL);
    profileEnd("QueryIntersection");

    RadeonRays::Intersection * hits_readback = NULL;
    {
      RadeonRays::Event * e = NULL;
      profileBegin("api->MapBuffer();");
      api->MapBufferReadback(hits_buffer, RadeonRays::kMapRead, 0, window_h * window_w * msaaSamplesCount * sizeof(RadeonRays::Intersection), (void **)&hits_readback, &e);
      profileEnd("api->MapBuffer();");
      profileBegin("e->Wait();");
      e->Wait();
      profileEnd("e->Wait();");
      api->DeleteEvent(e);
      e = NULL;
    }
    
    profileBegin("pixels array color average and write");
    #pragma omp parallel for
    for (int y = 0; y < window_h; y += 1)
    {
      #pragma omp parallel for
      for (int x = 0; x < window_w; x += 1)
      {
        float color_sum = 0;
        for (int s = 0; s < msaaSamplesCount; s += 1) {
          const uint64_t hits_idx = y * (uint64_t)window_w * msaaSamplesCount + x * msaaSamplesCount + s;
          color_sum += hits_readback[hits_idx].uvwt.w > 9999999 ? 0 : (110.f / hits_readback[hits_idx].uvwt.w);
        }

        float color_average = color_sum / (float)msaaSamplesCount;

        pix->pixels[y][x][0] = color_average;
        pix->pixels[y][x][1] = color_average;
        pix->pixels[y][x][2] = color_average;
        pix->pixels[y][x][3] = 255;
      }
    }
    profileEnd("pixels array color average and write");

    {
      RadeonRays::Event * e = NULL;
      profileBegin("api->UnmapBuffer();");
      api->UnmapBufferReadback(hits_buffer, hits_readback, &e);
      profileEnd("api->UnmapBuffer();");
      profileBegin("e->Wait();");
      e->Wait();
      profileEnd("e->Wait();");
      api->DeleteEvent(e);
      e = NULL;
      hits_readback = NULL;
    }

    profileBegin("vfDrawPixels(); vfAsyncDrawWaitToFinish();");
    vfDrawPixels(ctx, pix->pixels, NULL, FF, LL);
    vfAsyncDrawWaitToFinish(ctx, FF, LL);
    profileEnd("vfDrawPixels(); vfAsyncDrawWaitToFinish();");

    mouse_x_prev = mouse_x;
    mouse_y_prev = mouse_y;

    rays_vector.clear();

    profileEnd("frame");
  }

  red32MemoryFree(pix);
  pix = NULL;

  red32MemoryFree(indexes_per_triangle_count);
  indexes_per_triangle_count = NULL;

  red32MemoryFree(vertex_index);
  vertex_index = NULL;

  red32MemoryFree(vertex);
  vertex = NULL;

  vfContextDeinit(ctx, FF, LL);
  glfwTerminate();
}

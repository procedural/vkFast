#if 0
cd vs2019
cl /EHsc ../main.cpp ../../../vkfast.c C:/RedGpuSDK/redgpu.c C:/RedGpuSDK/redgpu_2.c C:/RedGpuSDK/redgpu_32.c /arch:AVX2 /openmp /link /stack:64000000
exit
#endif

#include "../../vkfast.h"

#include <stdio.h> // For printf

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

static bool IntersectBoxByBboxIndex(bvhvec4 pos, bvhvec4 invDir, float * bvh, uint64_t bboxIdx, float t) {
  bvhvec4 bbox_min;
  bvhvec4 bbox_max;
  bbox_min.x = bvh[bboxIdx * 6 + 0];
  bbox_min.y = bvh[bboxIdx * 6 + 1];
  bbox_min.z = bvh[bboxIdx * 6 + 2];
  bbox_max.x = bvh[bboxIdx * 6 + 3];
  bbox_max.y = bvh[bboxIdx * 6 + 4];
  bbox_max.z = bvh[bboxIdx * 6 + 5];

  bvhvec4 t0 = (bbox_min - pos) * invDir;
  bvhvec4 t1 = (bbox_max - pos) * invDir;

  float tmin = max(max(min(t0.x, t1.x), min(t0.y, t1.y)), min(t0.z,t1.z));
  float tmax = min(min(max(t0.x, t1.x), max(t0.y, t1.y)), max(t0.z,t1.z));
  return tmax >= tmin && tmin < t;
}

static void IntersectTriangleByTriangleIndex_MollerTrumboreTest(Ray & ray, const bvhvec4 * verts, uint64_t triIdx, float camera_pos_x, float camera_pos_y, float camera_pos_z) {
  const uint64_t i0 = triIdx * 3 + 0;
  const uint64_t i1 = triIdx * 3 + 1;
  const uint64_t i2 = triIdx * 3 + 2;

  bvhvec4 vert0 = verts[i0];
  bvhvec4 vert1 = verts[i1];
  bvhvec4 vert2 = verts[i2];

  vert0 = vert0 - bvhvec4(camera_pos_x, camera_pos_y, camera_pos_z, 0);
  vert1 = vert1 - bvhvec4(camera_pos_x, camera_pos_y, camera_pos_z, 0);
  vert2 = vert2 - bvhvec4(camera_pos_x, camera_pos_y, camera_pos_z, 0);

  const bvhvec4 v0_ = vert0;
  const bvhvec3 v0 = v0_;
  const bvhvec3 e1 = vert1 - v0_;
  const bvhvec3 e2 = vert2 - v0_;
  const float tmax = ray.hit.t;

  const bvhvec3 h = tinybvh_cross( ray.D, e2 );
  const float a = tinybvh_dot( e1, h );
  if (fabs( a ) < 0.000001f) return;
  const float f = 1 / a;
  const bvhvec3 s = ray.O - v0;
  const float u = f * tinybvh_dot( s, h );
  const bvhvec3 q = tinybvh_cross( s, e1 );
  const float v = f * tinybvh_dot( ray.D, q );
  const bool miss = u < 0 || v < 0 || u + v > 1;
  if (miss) return;
  const float t = f * tinybvh_dot( e2, q );
  if (t < 0 || t > tmax) return;

  ray.hit.t = t;
  ray.hit.u = u;
  ray.hit.v = v;
  ray.hit.prim = triIdx;
  ray.hit.inst = ray.instIdx;
}

static void BVH_IntersectTriangles_MollerTrumboreTest(Ray & ray, const bvhvec4 * verts, uint64_t triangles_count, float * bvh, float camera_pos_x, float camera_pos_y, float camera_pos_z) {
  bvhvec4 ray_o = ray.O;
  ray_o.x += camera_pos_x;
  ray_o.y += camera_pos_y;
  ray_o.z += camera_pos_z;
  bvhvec4 ray_d_inv;
  ray_d_inv.x = 1.0f / ray.D.x;
  ray_d_inv.y = 1.0f / ray.D.y;
  ray_d_inv.z = 1.0f / ray.D.z;
  ray_d_inv.w = 0;
  float ray_t = ray.hit.t;
  float visited_bvh_nodes_count = 0;

#define IntersectTri(_triIdx) IntersectTriangleByTriangleIndex_MollerTrumboreTest(ray, verts, _triIdx, camera_pos_x, camera_pos_y, camera_pos_z)
#define IntersectBbox(_bboxIdx) IntersectBoxByBboxIndex(ray_o, ray_d_inv, bvh, _bboxIdx, ray_t)

  if (IntersectBbox(0))
  {
    visited_bvh_nodes_count += 1.f;
    for (uint64_t triIdx = 0; triIdx <= 50; triIdx += 1) {
      IntersectTri(triIdx);
    }
    if (IntersectBbox(1))
    {
      visited_bvh_nodes_count += 1.f;
      for (uint64_t triIdx = 51; triIdx <= 68; triIdx += 1) {
        IntersectTri(triIdx);
      }
      if (IntersectBbox(2))
      {
        visited_bvh_nodes_count += 1.f;
        for (uint64_t triIdx = 69; triIdx <= 83; triIdx += 1) {
          IntersectTri(triIdx);
        }
        if (IntersectBbox(3))
        {
          visited_bvh_nodes_count += 1.f;
          for (uint64_t triIdx = 84; triIdx <= 90; triIdx += 1) {
            IntersectTri(triIdx);
          }
          if (IntersectBbox(4))
          {
            visited_bvh_nodes_count += 1.f;
            for (uint64_t triIdx = 91; triIdx <= 93; triIdx += 1) {
              IntersectTri(triIdx);
            }
          }
          if (IntersectBbox(5))
          {
            visited_bvh_nodes_count += 1.f;
            for (uint64_t triIdx = 94; triIdx <= 97; triIdx += 1) {
              IntersectTri(triIdx);
            }
          }
        }
        if (IntersectBbox(6))
        {
          visited_bvh_nodes_count += 1.f;
          for (uint64_t triIdx = 98; triIdx <= 106; triIdx += 1) {
            IntersectTri(triIdx);
          }
          if (IntersectBbox(7))
          {
            visited_bvh_nodes_count += 1.f;
            for (uint64_t triIdx = 107; triIdx <= 111; triIdx += 1) {
              IntersectTri(triIdx);
            }
            if (IntersectBbox(8))
            {
              visited_bvh_nodes_count += 1.f;
              for (uint64_t triIdx = 112; triIdx <= 115; triIdx += 1) {
                IntersectTri(triIdx);
              }
            }
            if (IntersectBbox(9))
            {
              visited_bvh_nodes_count += 1.f;
              for (uint64_t triIdx = 116; triIdx <= 118; triIdx += 1) {
                IntersectTri(triIdx);
              }
              if (IntersectBbox(10))
              {
                visited_bvh_nodes_count += 1.f;
                for (uint64_t triIdx = 119; triIdx <= 120; triIdx += 1) {
                  IntersectTri(triIdx);
                }
              }
            }
          }
          if (IntersectBbox(11))
          {
            visited_bvh_nodes_count += 1.f;
            for (uint64_t triIdx = 121; triIdx <= 122; triIdx += 1) {
              IntersectTri(triIdx);
            }
            if (IntersectBbox(12))
            {
              visited_bvh_nodes_count += 1.f;
              for (uint64_t triIdx = 123; triIdx <= 125; triIdx += 1) {
                IntersectTri(triIdx);
              }
            }
          }
        }
      }
      if (IntersectBbox(13))
      {
        visited_bvh_nodes_count += 1.f;
        for (uint64_t triIdx = 126; triIdx <= 143; triIdx += 1) {
          IntersectTri(triIdx);
        }
        if (IntersectBbox(14))
        {
          visited_bvh_nodes_count += 1.f;
          for (uint64_t triIdx = 144; triIdx <= 157; triIdx += 1) {
            IntersectTri(triIdx);
          }
          if (IntersectBbox(15))
          {
            visited_bvh_nodes_count += 1.f;
            for (uint64_t triIdx = 158; triIdx <= 159; triIdx += 1) {
              IntersectTri(triIdx);
            }
            if (IntersectBbox(16))
            {
              visited_bvh_nodes_count += 1.f;
              for (uint64_t triIdx = 160; triIdx <= 163; triIdx += 1) {
                IntersectTri(triIdx);
              }
            }
          }
          if (IntersectBbox(17))
          {
            visited_bvh_nodes_count += 1.f;
            for (uint64_t triIdx = 164; triIdx <= 168; triIdx += 1) {
              IntersectTri(triIdx);
            }
            if (IntersectBbox(18))
            {
              visited_bvh_nodes_count += 1.f;
              for (uint64_t triIdx = 169; triIdx <= 172; triIdx += 1) {
                IntersectTri(triIdx);
              }
              if (IntersectBbox(19))
              {
                visited_bvh_nodes_count += 1.f;
                for (uint64_t triIdx = 173; triIdx <= 175; triIdx += 1) {
                  IntersectTri(triIdx);
                }
              }
              if (IntersectBbox(20))
              {
                visited_bvh_nodes_count += 1.f;
                for (uint64_t triIdx = 176; triIdx <= 178; triIdx += 1) {
                  IntersectTri(triIdx);
                }
              }
            }
            if (IntersectBbox(21))
            {
              visited_bvh_nodes_count += 1.f;
              for (uint64_t triIdx = 179; triIdx <= 179; triIdx += 1) {
                IntersectTri(triIdx);
              }
            }
          }
        }
        if (IntersectBbox(22))
        {
          visited_bvh_nodes_count += 1.f;
          for (uint64_t triIdx = 180; triIdx <= 189; triIdx += 1) {
            IntersectTri(triIdx);
          }
          if (IntersectBbox(23))
          {
            visited_bvh_nodes_count += 1.f;
            for (uint64_t triIdx = 190; triIdx <= 195; triIdx += 1) {
              IntersectTri(triIdx);
            }
            if (IntersectBbox(24))
            {
              visited_bvh_nodes_count += 1.f;
              for (uint64_t triIdx = 196; triIdx <= 196; triIdx += 1) {
                IntersectTri(triIdx);
              }
            }
            if (IntersectBbox(25))
            {
              visited_bvh_nodes_count += 1.f;
              for (uint64_t triIdx = 197; triIdx <= 200; triIdx += 1) {
                IntersectTri(triIdx);
              }
            }
          }
          if (IntersectBbox(26))
          {
            visited_bvh_nodes_count += 1.f;
            for (uint64_t triIdx = 201; triIdx <= 204; triIdx += 1) {
              IntersectTri(triIdx);
            }
            if (IntersectBbox(27))
            {
              visited_bvh_nodes_count += 1.f;
              for (uint64_t triIdx = 205; triIdx <= 206; triIdx += 1) {
                IntersectTri(triIdx);
              }
            }
            if (IntersectBbox(28))
            {
              visited_bvh_nodes_count += 1.f;
              for (uint64_t triIdx = 207; triIdx <= 208; triIdx += 1) {
                IntersectTri(triIdx);
              }
            }
          }
        }
      }
    }
    if (IntersectBbox(29))
    {
      visited_bvh_nodes_count += 1.f;
      for (uint64_t triIdx = 209; triIdx <= 220; triIdx += 1) {
        IntersectTri(triIdx);
      }
      if (IntersectBbox(30))
      {
        visited_bvh_nodes_count += 1.f;
        for (uint64_t triIdx = 221; triIdx <= 230; triIdx += 1) {
          IntersectTri(triIdx);
        }
        if (IntersectBbox(31))
        {
          visited_bvh_nodes_count += 1.f;
          for (uint64_t triIdx = 231; triIdx <= 235; triIdx += 1) {
            IntersectTri(triIdx);
          }
          if (IntersectBbox(32))
          {
            visited_bvh_nodes_count += 1.f;
            for (uint64_t triIdx = 236; triIdx <= 242; triIdx += 1) {
              IntersectTri(triIdx);
            }
            if (IntersectBbox(33))
            {
              visited_bvh_nodes_count += 1.f;
              for (uint64_t triIdx = 243; triIdx <= 246; triIdx += 1) {
                IntersectTri(triIdx);
              }
            }
          }
          if (IntersectBbox(34))
          {
            visited_bvh_nodes_count += 1.f;
            for (uint64_t triIdx = 247; triIdx <= 249; triIdx += 1) {
              IntersectTri(triIdx);
            }
            if (IntersectBbox(35))
            {
              visited_bvh_nodes_count += 1.f;
              for (uint64_t triIdx = 250; triIdx <= 253; triIdx += 1) {
                IntersectTri(triIdx);
              }
            }
          }
        }
        if (IntersectBbox(36))
        {
          visited_bvh_nodes_count += 1.f;
          for (uint64_t triIdx = 254; triIdx <= 261; triIdx += 1) {
            IntersectTri(triIdx);
          }
          if (IntersectBbox(37))
          {
            visited_bvh_nodes_count += 1.f;
            for (uint64_t triIdx = 262; triIdx <= 264; triIdx += 1) {
              IntersectTri(triIdx);
            }
            if (IntersectBbox(38))
            {
              visited_bvh_nodes_count += 1.f;
              for (uint64_t triIdx = 265; triIdx <= 268; triIdx += 1) {
                IntersectTri(triIdx);
              }
            }
            if (IntersectBbox(39))
            {
              visited_bvh_nodes_count += 1.f;
              for (uint64_t triIdx = 269; triIdx <= 270; triIdx += 1) {
                IntersectTri(triIdx);
              }
            }
          }
          if (IntersectBbox(40))
          {
            visited_bvh_nodes_count += 1.f;
            for (uint64_t triIdx = 271; triIdx <= 276; triIdx += 1) {
              IntersectTri(triIdx);
            }
            if (IntersectBbox(41))
            {
              visited_bvh_nodes_count += 1.f;
              for (uint64_t triIdx = 277; triIdx <= 278; triIdx += 1) {
                IntersectTri(triIdx);
              }
            }
            if (IntersectBbox(42))
            {
              visited_bvh_nodes_count += 1.f;
              for (uint64_t triIdx = 279; triIdx <= 281; triIdx += 1) {
                IntersectTri(triIdx);
              }
            }
          }
        }
      }
      if (IntersectBbox(43))
      {
        visited_bvh_nodes_count += 1.f;
        for (uint64_t triIdx = 282; triIdx <= 283; triIdx += 1) {
          IntersectTri(triIdx);
        }
        if (IntersectBbox(44))
        {
          visited_bvh_nodes_count += 1.f;
          for (uint64_t triIdx = 284; triIdx <= 287; triIdx += 1) {
            IntersectTri(triIdx);
          }
        }
        if (IntersectBbox(45))
        {
          visited_bvh_nodes_count += 1.f;
          for (uint64_t triIdx = 288; triIdx <= 291; triIdx += 1) {
            IntersectTri(triIdx);
          }
        }
      }
    }
  }
#undef IntersectTri
#undef IntersectBbox
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

  #define window_w 320 // NOTE(Constantine): Hardcoded.
  #define window_h 180 // NOTE(Constantine): Hardcoded.
  const float aspect_ratio = 9.f / 16.f; // NOTE(Constantine): Hardcoded.

  glfwInit();
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
  GLFWwindow * window = glfwCreateWindow(window_w, window_h, "[vkFast] RT 3D Mesh Stanford Bunny CPU Slow BVH", 0, 0);
  void * window_handle = (void *)glfwGetWin32Window(window);

  gpu_handle_context_t ctx = vfContextInit(1, NULL, FF, LL);
  vfWindowFullscreen(ctx, window_handle, "[vkFast] RT 3D Mesh Stanford Bunny CPU Slow BVH", 700, 700, 0, FF, LL);

  struct Pixels {
    unsigned char pixels[window_h][window_w][4];
  };
  // To free
  struct Pixels * pix = (struct Pixels *)red32MemoryCalloc(sizeof(struct Pixels));
  REDGPU_2_EXPECTFL(pix != NULL);

  float bunny[] = {
    #include "../../extra/3D Mesh Stanford Bunny/3d_mesh_vertices_stanford_bunny.h"
  };

  float bunny_bvh[] = {
    -23.890f,   5.254f, -45.197f,    84.233f, 112.514f,  41.428f,
    -23.890f,   5.254f, -45.197f,    30.041f, 112.514f,  39.777f,
    -20.512f,   5.254f, -26.427f,    29.227f,  58.637f,  39.777f,
    -17.363f,   5.275f, -26.427f,    29.227f,  58.637f,   2.177f,
     -5.434f,   5.275f, -18.550f,    22.396f,  14.842f,  -1.448f,
    -17.363f,  32.445f, -20.200f,     5.592f,  58.637f,   2.177f,
    -20.512f,   5.254f,   6.828f,    27.724f,  57.556f,  39.777f,
    -10.108f,   5.254f,   6.828f,    19.874f,  31.132f,  37.050f,
    -10.108f,   5.254f,   6.828f,    10.061f,  31.132f,  20.984f,
     -6.453f,   5.310f,  22.765f,    19.874f,  17.253f,  37.050f,
      7.338f,   5.310f,  24.843f,    19.874f,  17.253f,  37.050f,
    -20.512f,  41.254f,   9.215f,    12.460f,  57.556f,  30.415f,
    -20.512f,  41.254f,   9.215f,    -4.712f,  57.556f,  24.599f,
    -23.890f,  60.116f, -45.197f,    30.041f, 112.514f,  33.935f,
     -9.317f,  68.510f, -45.197f,    30.041f, 112.514f,  -7.294f,
     -7.096f,  68.510f, -36.359f,    11.323f,  89.171f,  -7.294f,
     -2.565f,  68.510f, -19.617f,    11.323f,  89.171f,  -7.294f,
     -4.462f,  90.644f, -45.197f,    30.041f, 112.514f,  -8.426f,
     -4.462f,  90.716f, -45.197f,    21.241f, 112.514f, -28.954f,
     -4.462f,  90.716f, -43.984f,     8.380f, 110.211f, -33.056f,
     10.608f,  93.621f, -45.197f,    21.241f, 112.514f, -28.954f,
      6.386f,  90.644f, -20.603f,    28.902f, 106.939f,  -8.426f,
    -23.890f,  60.116f,  -4.998f,    29.237f, 100.125f,  33.935f,
    -23.890f,  60.116f,  -3.292f,    -2.582f,  93.740f,  33.935f,
    -21.594f,  86.379f,  -3.292f,    -2.582f,  93.740f,   8.040f,
    -22.722f,  62.804f,  16.907f,    -5.818f,  88.406f,  33.935f,
      3.630f,  64.590f,  -2.459f,    29.237f,  96.787f,  20.211f,
     13.977f,  64.590f,  -1.372f,    29.237f,  71.507f,  20.211f,
      7.052f,  81.566f,  -2.459f,    22.617f,  96.787f,   3.888f,
     31.117f,   5.281f, -30.643f,    84.233f, 112.434f,  41.428f,
     31.181f,   5.281f, -26.832f,    84.233f,  55.116f,  41.428f,
     31.863f,   5.295f, -26.832f,    79.977f,  55.116f,   5.591f,
     31.863f,   5.295f, -21.494f,    79.977f,  29.367f,   5.591f,
     56.439f,   5.295f, -13.910f,    79.977f,  25.026f,   5.591f,
     36.519f,  34.695f, -26.357f,    76.141f,  55.116f,   2.216f,
     56.677f,  34.695f, -18.934f,    76.141f,  55.116f,   2.216f,
     31.181f,   5.281f,   9.293f,    84.233f,  51.513f,  41.428f,
     31.181f,   5.312f,  29.121f,    55.260f,  50.349f,  41.428f,
     31.181f,   5.312f,  29.121f,    55.260f,  27.114f,  41.307f,
     39.498f,  30.459f,  35.401f,    55.152f,  50.349f,  41.428f,
     62.886f,   5.281f,   9.293f,    84.233f,  51.513f,  34.118f,
     67.266f,   5.281f,  10.809f,    84.233f,  25.699f,  25.762f,
     62.886f,  30.358f,   9.293f,    82.997f,  51.513f,  34.118f,
     31.117f,  60.189f, -30.643f,    67.757f, 112.434f,  31.091f,
     31.117f,  63.727f, -30.643f,    59.223f, 112.434f,  -1.508f,
     42.551f,  60.189f,   6.074f,    67.757f,  75.249f,  31.091f
  };

  for (uint64_t i = 0, vertex_count = (sizeof(bunny) / sizeof(float)) / 3; i < vertex_count; i += 1) {
    float new_scale = 0.025f;
    bunny[i * 3 + 0] *= new_scale;
    bunny[i * 3 + 1] *= new_scale;
    bunny[i * 3 + 2] *= new_scale;
  }

  for (uint64_t i = 0, bbox_min_or_max_count = (sizeof(bunny_bvh) / sizeof(float)) / 3; i < bbox_min_or_max_count; i += 1) {
    float new_scale = 0.025f;
    bunny_bvh[i * 3 + 0] *= new_scale;
    bunny_bvh[i * 3 + 1] *= new_scale;
    bunny_bvh[i * 3 + 2] *= new_scale;
  }

  const uint64_t vertex_count = (sizeof(bunny) / sizeof(float)) / 3;
  // To free
  bvhvec4 * vertex = (bvhvec4 *)red32MemoryCalloc(sizeof(bvhvec4) * vertex_count);
  REDGPU_2_EXPECTFL(vertex != NULL);

  for (uint64_t i = 0; i < vertex_count; i += 1) {
    vertex[i].x = bunny[i * 3 + 0];
    vertex[i].y = bunny[i * 3 + 1];
    vertex[i].z = bunny[i * 3 + 2];
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

  while (glfwWindowShouldClose(window) == 0) {
    glfwPollEvents();

    glfwGetCursorPos(window, &mouse_x, &mouse_y);

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

    bvhvec3 up = camera_axis_y * aspect_ratio;
    bvhvec3 C  = 2 * camera_axis_z;
    bvhvec3 p1 = C - camera_axis_x + up;
    bvhvec3 p2 = C + camera_axis_x + up;
    bvhvec3 p3 = C - camera_axis_x - up;

    #pragma omp parallel for
    for (int y = 0; y < window_h; y += 1)
    {
      #pragma omp parallel for
      for (int x = 0; x < window_w; x += 1)
      {
        float sum = 0;
        Ray rays[msaaSamplesCount];
        for (int s = 0; s < msaaSamplesCount; s += 1) {
          float u = (float)(x + 0.5f + msaa_samples_offset_table_x[s]) / (float)(window_w);
          float v = (float)(y + 0.5f + msaa_samples_offset_table_y[s]) / (float)(window_h);
          bvhvec3 P = p1 + u * (p2 - p1) + v * (p3 - p1);
          bvhvec3 P_normalized = tinybvh_normalize(P);
          Ray ray(bvhvec3(0, 0, 0), P_normalized);
          //tlas.Intersect(ray, camera_pos[0], camera_pos[1], camera_pos[2]);
          BVH_IntersectTriangles_MollerTrumboreTest(ray, vertex, vertex_count / 3, bunny_bvh, camera_pos[0], camera_pos[1], camera_pos[2]);
          rays[s] = ray;
        }

        float color_sum = 0;
        for (int s = 0; s < msaaSamplesCount; s += 1) {
          color_sum += rays[s].hit.t > 9999999 ? 0 : (110.f / rays[s].hit.t);
        }

        float color_average = color_sum / (float)msaaSamplesCount;

        pix->pixels[y][x][0] = color_average;
        pix->pixels[y][x][1] = color_average;
        pix->pixels[y][x][2] = color_average;
        pix->pixels[y][x][3] = 255;
      }
    }

    vfDrawPixels(ctx, pix->pixels, NULL, FF, LL);
    vfAsyncDrawWaitToFinish(ctx, FF, LL);

    mouse_x_prev = mouse_x;
    mouse_y_prev = mouse_y;
  }

  red32MemoryFree(pix);
  pix = NULL;

  red32MemoryFree(vertex);
  vertex = NULL;

  vfContextDeinit(ctx, FF, LL);
  glfwTerminate();
}

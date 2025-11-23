#if 0
clang main.c ../../vkfast.c C:/RedGpuSDK/redgpu.c C:/RedGpuSDK/redgpu_2.c C:/RedGpuSDK/redgpu_32.c
exit
#endif

#include "../../vkfast.h"

#include <stdio.h> // For printf
#include <math.h>  // For sin, cos

#include <shellscalingapi.h>   // For SetProcessDpiAwareness
#pragma comment(lib, "shcore") // For SetProcessDpiAwareness

#define countof(x) (sizeof(x) / sizeof((x)[0]))

#define FF __FILE__
#define LL __LINE__

int some_texture_channels_count = 3; // NOTE(Constantine): Hardcoded.
int some_texture_w = 0;
int some_texture_h = 0;
unsigned char * some_texture = NULL;

// Math prims begin

typedef struct vec2 {
  float x, y;
} vec2;

typedef struct vec3 {
  float x, y, z;
} vec3;

typedef struct vec4 {
  float x, y, z, w;
} vec4;

typedef struct mat2 {
  float m[2][2];
} mat2;

typedef struct mat3 {
  float m[3][3];
} mat3;

vec2 makev2(float x, float y) { // make vec2
  vec2 result;
  result.x = x;
  result.y = y;
  return result;
}

vec3 makev3(float x, float y, float z) { // make vec3
  vec3 result;
  result.x = x;
  result.y = y;
  result.z = z;
  return result;
}

vec3 makev3fromv2(vec2 xy, float z) { // make vec3 from vec2
  vec3 result;
  result.x = xy.x;
  result.y = xy.y;
  result.z = z;
  return result;
}

vec4 makev4fromv3(vec3 xyz, float w) { // make vec4 from vec3
  vec4 result;
  result.x = xyz.x;
  result.y = xyz.y;
  result.z = xyz.z;
  result.w = w;
  return result;
}

mat2 makem2(float m00, float m01, float m10, float m11) { // make mat2
  mat2 result;
  result.m[0][0] = m00;
  result.m[0][1] = m01;
  result.m[1][0] = m10;
  result.m[1][1] = m11;
  return result;
}

vec2 v2a(vec2 a, vec2 b) { // vec2 add
  vec2 result;
  result.x = a.x + b.x;
  result.y = a.y + b.y;
  return result;
}

vec2 v2mf(vec2 a, float b) { // vec2 mul float
  vec2 result;
  result.x = a.x * b;
  result.y = a.y * b;
  return result;
}

vec2 v2df(vec2 a, float b) { // vec2 div float
  vec2 result;
  result.x = a.x / b;
  result.y = a.y / b;
  return result;
}

vec3 v3a(vec3 a, vec3 b) { // vec3 add
  vec3 result;
  result.x = a.x + b.x;
  result.y = a.y + b.y;
  result.z = a.z + b.z;
  return result;
}

vec3 v3s(vec3 a, vec3 b) { // vec3 sub
  vec3 result;
  result.x = a.x - b.x;
  result.y = a.y - b.y;
  result.z = a.z - b.z;
  return result;
}

vec3 v3mf(vec3 a, float b) { // vec3 mul float
  vec3 result;
  result.x = a.x * b;
  result.y = a.y * b;
  result.z = a.z * b;
  return result;
}

float v3length(vec3 a) { // vec3 length
  return sqrtf((a.x * a.x) + (a.y * a.y) + (a.z * a.z));
}

vec3 v3normalize(vec3 a) { // vec3 normalize
  float len = v3length(a);
  if (len == 0.f) {
    // Avoiding division by zero length
    vec3 zero_vec = makev3(0.f, 0.f, 0.f);
    return zero_vec;
  }
  vec3 result;
  result.x = a.x / len;
  result.y = a.y / len;
  result.z = a.z / len;
  return result;
}

float v3dot(vec3 a, vec3 b) { // vec3 dot
  return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
}

vec3 v3cross(vec3 a, vec3 b) { // vec3 cross
  vec3 result;
  result.x = (a.y * b.z) - (a.z * b.y);
  result.y = (a.z * b.x) - (a.x * b.z);
  result.z = (a.x * b.y) - (a.y * b.x);
  return result;
}

float v4dot(vec4 a, vec4 b) { // vec4 dot
  return (a.x * b.x) + (a.y * b.y) + (a.z * b.z) + (a.w * b.w);
}

vec2 m2mv2(mat2 m, vec2 v) { // mat2 mul vec2
  vec2 result;
  result.x = (m.m[0][0] * v.x) + (m.m[1][0] * v.y);
  result.y = (m.m[0][1] * v.x) + (m.m[1][1] * v.y);
  return result;
}

vec3 m3mv3(mat3 m, vec3 v) { // mat3 mul vec3
  vec3 result;
  result.x = (m.m[0][0] * v.x) + (m.m[1][0] * v.y) + (m.m[2][0] * v.z);
  result.y = (m.m[0][1] * v.x) + (m.m[1][1] * v.y) + (m.m[2][1] * v.z);
  result.z = (m.m[0][2] * v.x) + (m.m[1][2] * v.y) + (m.m[2][2] * v.z);
  return result;
}

// Math prims end

vec3 intersectTriangle(vec3 p0, vec3 p1, vec3 p2, vec3 ray_pos, vec3 ray_dir) {
  // Triangle normal
  vec3 N = v3cross(v3s(p1, p0), v3s(p2, p0));

  vec4 L = makev4fromv3(N, -v3dot(N, p0));

  // Ray triangle intersection distance
  float t = -v4dot(L, makev4fromv3(ray_pos, 1.f)) / v4dot(L, makev4fromv3(ray_dir, 0.f));
  if (t <= 0.f) {
    return makev3(0.f, 0.f, 0.f);
  }

  // Ray triangle intersection point
  vec3 p  = v3a(v3mf(ray_dir, t), ray_pos);
  vec3 R  = v3s(p,  p0);
  vec3 Q1 = v3s(p1, p0);
  vec3 Q2 = v3s(p2, p0);

  float Q1Q1 = v3dot(Q1, Q1);
  float Q1Q2 = v3dot(Q1, Q2);
  float Q2Q2 = v3dot(Q2, Q2);
  float RQ1  = v3dot(R,  Q1);
  float RQ2  = v3dot(R,  Q2);

  mat2 m1 = makem2(Q2Q2, -Q1Q2, -Q1Q2, Q1Q1);
  vec2 v1 = makev2(RQ1, RQ2);

  vec2 w1w2 = v2df(m2mv2(m1, v1), (Q1Q1 * Q2Q2 - Q1Q2 * Q1Q2));

  float w1 = w1w2.x;
  float w2 = w1w2.y;
  float w0 = 1.f - w1 - w2;

  if (w1 > 0.f && w2 > 0.f && w0 > 0.f) {
    return makev3(w0, w1, w2);
  }

  return makev3(0.f, 0.f, 0.f);
}

mat3 computeEyeRayXform(vec3 ro, vec3 ta, float roll) {
  vec3 ww = v3normalize(v3s(ta, ro));
  vec3 uu = v3normalize(v3cross(ww, makev3(sin(roll), cos(roll), 0.f)));
  vec3 vv = v3normalize(v3cross(uu, ww));
  mat3 result;
  result.m[0][0] = uu.x;
  result.m[0][1] = uu.y;
  result.m[0][2] = uu.z;
  result.m[1][0] = vv.x;
  result.m[1][1] = vv.y;
  result.m[1][2] = vv.z;
  result.m[2][0] = ww.x;
  result.m[2][1] = ww.y;
  result.m[2][2] = ww.z;
  return result;
}

vec3 colorTriangle(vec3 start, vec3 dir) {
  vec2 uv0 = makev2(0.f, 0.f);
  vec2 uv1 = makev2(1.f, 0.f);
  vec2 uv2 = makev2(0.f, 1.f);

  vec3 weights = intersectTriangle(makev3(0, 0, 0), makev3(0, 0, 1), makev3(0, 1, 0), start, dir);

  vec2 fetch = v2a(v2a(v2mf(uv0, weights.x), v2mf(uv1, weights.y)), v2mf(uv2, weights.z));

  uint64_t fetch_index_r = (int)((/*flip y*/1.f-fetch.y) * (some_texture_h-1)) * some_texture_w * some_texture_channels_count + (int)(fetch.x * (some_texture_w-1)) * some_texture_channels_count + 0;
  uint64_t fetch_index_g = (int)((/*flip y*/1.f-fetch.y) * (some_texture_h-1)) * some_texture_w * some_texture_channels_count + (int)(fetch.x * (some_texture_w-1)) * some_texture_channels_count + 1;
  uint64_t fetch_index_b = (int)((/*flip y*/1.f-fetch.y) * (some_texture_h-1)) * some_texture_w * some_texture_channels_count + (int)(fetch.x * (some_texture_w-1)) * some_texture_channels_count + 2;
  uint64_t fetch_index_a = (int)((/*flip y*/1.f-fetch.y) * (some_texture_h-1)) * some_texture_w * some_texture_channels_count + (int)(fetch.x * (some_texture_w-1)) * some_texture_channels_count + 3;

  REDGPU_2_EXPECTFL(fetch_index_r < (some_texture_w * some_texture_h * some_texture_channels_count));
  REDGPU_2_EXPECTFL(fetch_index_g < (some_texture_w * some_texture_h * some_texture_channels_count));
  REDGPU_2_EXPECTFL(fetch_index_b < (some_texture_w * some_texture_h * some_texture_channels_count));
  REDGPU_2_EXPECTFL(fetch_index_a < (some_texture_w * some_texture_h * some_texture_channels_count));

  uint8_t r = some_texture[fetch_index_r];
  uint8_t g = some_texture[fetch_index_g];
  uint8_t b = some_texture[fetch_index_b];
  uint8_t a = some_texture[fetch_index_a];

  return makev3(r / 255.f, g / 255.f, b / 255.f);
}

#if defined(_MSC_VER) && defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif

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

  #define window_w 1920
  #define window_h 1080

  int windowMonitorArea[4] = {0};
  vfGetMainMonitorAreaRectangle(windowMonitorArea, FF, LL);
  REDGPU_2_EXPECTFL(windowMonitorArea[2] == window_w);
  REDGPU_2_EXPECTFL(windowMonitorArea[3] == window_h);

  gpu_handle_context_t ctx = vfContextInit(1, NULL, FF, LL);
  vfWindowFullscreen(ctx, NULL, "[vkFast] RT Triangle Textured CPU Without GLM", window_w, window_h, 0, FF, LL);

  struct Pixels {
    unsigned char pixels[window_h][window_w][4];
  };
  // To free
  struct Pixels * pix = (struct Pixels *)red32MemoryCalloc(sizeof(struct Pixels));
  REDGPU_2_EXPECTFL(pix != NULL);

  void * texture_fd = NULL;
  void * texture_mh = NULL;
  size_t texture_data_bytes_count = 0;
  void * texture_data = NULL;
  // To close
  red32FileMap((const short unsigned int *)L"texture.ppm", &texture_fd, &texture_mh, &texture_data_bytes_count, &texture_data);
  REDGPU_2_EXPECTFL(texture_fd != NULL || !"texture.ppm not found, copy it from a previous example");
  REDGPU_2_EXPECTFL(texture_mh != NULL);
  REDGPU_2_EXPECTFL(texture_data_bytes_count != 0);
  REDGPU_2_EXPECTFL(texture_data != NULL);
  some_texture_w = 256; // NOTE(Constantine): Hardcoded.
  some_texture_h = 256; // NOTE(Constantine): Hardcoded.
  some_texture = (unsigned char *)texture_data;
  some_texture += 61; // NOTE(Constantine): Hardcoded, skipping .ppm's header part.

  while (vfWindowLoop(ctx)) {
    unsigned char * pixels = &pix->pixels[0][0][0];
    // Clear pixels:
    for (int y = 0; y < window_h; y += 1) {
      for (int x = 0; x < window_w; x += 1) {
        pixels[y * window_w * 4 + x * 4 + 0] = 0;
        pixels[y * window_w * 4 + x * 4 + 1] = 0;
        pixels[y * window_w * 4 + x * 4 + 2] = 0;
        pixels[y * window_w * 4 + x * 4 + 3] = 0;
      }
    }

    static float time = 0.f;

    LARGE_INTEGER t_start = {0};
    QueryPerformanceCounter(&t_start);

    // Draw pixels:
    vec3 eyePosition = makev3(3.f * sin(time), 0.f, 3.f * cos(time));
    vec3 eyeLookAt = makev3(0.f, 0.f, 0.f);
    mat3 eyeRayXform = computeEyeRayXform(eyePosition, eyeLookAt, 0.f);
    int y; // Great job, Visual Studio people :D https://www.reddit.com/r/C_Programming/comments/13v32z1/pragma_omp_parallel_for_not_compiling_when_it/
    #pragma omp parallel for
    for (y = 0; y < window_h; y += 1) {
      int x;
      #pragma omp parallel for
      for (x = 0; x < window_w; x += 1) {
        vec2 fragCoord  = {(float)x, (float)y};
        vec2 resolution = {window_w, window_h};

        vec2 pixelPosition = v2df(v2a(makev2(-resolution.x, -resolution.y), v2mf(fragCoord, 2.f)), -resolution.y);
        vec3 v1 = makev3fromv2(pixelPosition, 2.f);
        vec3 v2 = m3mv3(eyeRayXform, v1);
        vec3 eyeRayDirection = v3normalize(v2);
        vec3 color = colorTriangle(eyePosition, eyeRayDirection);
        vec4 outputColor = makev4fromv3(color, 1.f);

        pixels[y * window_w * 4 + x * 4 + 0] = outputColor.z * 255.f;
        pixels[y * window_w * 4 + x * 4 + 1] = outputColor.y * 255.f;
        pixels[y * window_w * 4 + x * 4 + 2] = outputColor.x * 255.f;
        pixels[y * window_w * 4 + x * 4 + 3] = outputColor.w * 255.f;
      }
    }

    LARGE_INTEGER t_end = {0};
    QueryPerformanceCounter(&t_end);

    {
      LONGLONG elapsedTicks = t_end.QuadPart - t_start.QuadPart;
      LONGLONG nanoseconds = (elapsedTicks * 1000000000LL) / frequency.QuadPart;
      double milliseconds_fp = (double)(nanoseconds) / 1000000.0;
      printf("Elapsed milliseconds: %f\n", milliseconds_fp);
    }

    time += 0.2f; // NOTE(Constantine): Speeded up.

    vfDrawPixels(ctx, pix->pixels, NULL, FF, LL);
    vfAsyncDrawWaitToFinish(ctx, FF, LL);
  }
  
  red32FileUnmap(texture_fd, texture_mh);
  texture_fd = NULL;
  texture_mh = NULL;
  texture_data_bytes_count = 0;
  texture_data = NULL;

  red32MemoryFree(pix);
  pix = NULL;

  vfContextDeinit(ctx, FF, LL);
  vfExit(0);
}

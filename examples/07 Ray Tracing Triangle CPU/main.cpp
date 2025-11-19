#if 0
clang -c ../../vkfast.c C:/RedGpuSDK/redgpu.c C:/RedGpuSDK/redgpu_2.c C:/RedGpuSDK/redgpu_32.c && clang++ main.cpp *.o
exit
#endif

#include "../../vkfast.h"

#include <stdio.h> // For printf

#include <shellscalingapi.h>   // For SetProcessDpiAwareness
#pragma comment(lib, "shcore") // For SetProcessDpiAwareness

#define countof(x) (sizeof(x) / sizeof((x)[0]))

#define FF __FILE__
#define LL __LINE__

#include "glm/glm/glm.hpp"
using namespace glm;

vec3 intersectTriangle(vec3 p0, vec3 p1, vec3 p2, vec3 ray_pos, vec3 ray_dir) {
  // Triangle normal
  vec3 N = cross(p1 - p0, p2 - p0);

  vec4 L = vec4(N, -dot(N, p0));

  // Ray triangle intersection distance
  float t = -dot(L, vec4(ray_pos, 1.f)) / dot(L, vec4(ray_dir, 0.f));
  if (t <= 0.f) {
    return vec3(0.f, 0.f, 0.f);
  }

  // Ray triangle intersection point
  vec3 p  = t * ray_dir + ray_pos;
  vec3 R  = p  - p0;
  vec3 Q1 = p1 - p0;
  vec3 Q2 = p2 - p0;

  float Q1Q1 = dot(Q1, Q1);
  float Q1Q2 = dot(Q1, Q2);
  float Q2Q2 = dot(Q2, Q2);
  float RQ1  = dot(R,  Q1);
  float RQ2  = dot(R,  Q2);

  vec2 w1w2 = mat2(Q2Q2, -Q1Q2, -Q1Q2, Q1Q1) * vec2(RQ1, RQ2) / (Q1Q1 * Q2Q2 - Q1Q2 * Q1Q2);

  float w1 = w1w2.x;
  float w2 = w1w2.y;
  float w0 = 1.f - w1 - w2;

  if (w1 > 0.f && w2 > 0.f && w0 > 0.f) {
    return vec3(w0, w1, w2);
  }

  return vec3(0.f, 0.f, 0.f);
}

mat3 computeEyeRayXform(vec3 ro, vec3 ta, float roll) {
  vec3 ww = normalize(ta - ro);
  vec3 uu = normalize(cross(ww, vec3(sin(roll), cos(roll), 0.f)));
  vec3 vv = normalize(cross(uu, ww));
  return mat3(uu, vv, ww);
}

vec3 colorTriangle(vec3 start, vec3 dir) {
  vec2 uv0 = vec2(0.f, 0.f);
  vec2 uv1 = vec2(1.f, 0.f);
  vec2 uv2 = vec2(0.f, 1.f);

  vec3 weights = intersectTriangle(vec3(0, 0, 0), vec3(0, 0, 1), vec3(0, 1, 0), start, dir);
  return vec3(weights.x, weights.y, weights.z); // texture(some_texture, weights.x * uv0 + weights.y * uv1 + weights.z * uv2).xyz;
}

int main() {
#ifdef __MINGW32__
  SetProcessDPIAware();
#else
  SetProcessDpiAwareness(PROCESS_PER_MONITOR_DPI_AWARE);
#endif

  #define window_w 1920
  #define window_h 1080

  int windowMonitorArea[4] = {0};
  vfGetMainMonitorAreaRectangle(windowMonitorArea, FF, LL);
  REDGPU_2_EXPECTFL(windowMonitorArea[2] == window_w);
  REDGPU_2_EXPECTFL(windowMonitorArea[3] == window_h);

  gpu_handle_context_t ctx = vfContextInit(1, NULL, FF, LL);
  vfWindowFullscreen(ctx, NULL, "[vkFast] Ray Tracing Triangle CPU", window_w, window_h, 0, FF, LL);

  struct Pixels {
    unsigned char pixels[window_h][window_w][4];
  };
  // To free
  struct Pixels * pix = (struct Pixels *)red32MemoryCalloc(sizeof(struct Pixels));
  REDGPU_2_EXPECTFL(pix != NULL);

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

    // Draw pixels:
    #pragma omp parallel for
    for (int y = 0; y < window_h; y += 1) {
      #pragma omp parallel for
      for (int x = 0; x < window_w; x += 1) {
        vec2 fragCoord  = {(float)x, (float)y};
        vec2 resolution = {window_w, window_h};

        vec2 pixelPosition = (-resolution + 2.f * fragCoord) / -resolution.y;
        vec3 eyePosition = vec3(3.f * sin(time), 0.f, 3.f * cos(time));
        vec3 eyeLookAt = vec3(0.f , 0.f , 0.f);
        mat3 eyeRayXform = computeEyeRayXform(eyePosition, eyeLookAt, 0.f);
        vec3 eyeRayDirection = normalize(eyeRayXform * vec3(pixelPosition, 2.f));
        vec3 color = colorTriangle(eyePosition, eyeRayDirection);
        vec4 outputColor = vec4(color, 1.f);

        pixels[y * window_w * 4 + x * 4 + 0] = outputColor.b * 255.f;
        pixels[y * window_w * 4 + x * 4 + 1] = outputColor.g * 255.f;
        pixels[y * window_w * 4 + x * 4 + 2] = outputColor.r * 255.f;
        pixels[y * window_w * 4 + x * 4 + 3] = outputColor.a * 255.f;
      }
    }

    time += 0.01f;

    vfDrawPixels(ctx, pix->pixels, NULL, FF, LL);
    vfAsyncDrawWaitToFinish(ctx, FF, LL);
  }
  
  red32MemoryFree(pix);
  pix = NULL;

  vfContextDeinit(ctx, FF, LL);
  vfExit(0);
}

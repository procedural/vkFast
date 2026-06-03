#if 0
gcc -O0 -g -c ../../vkfast.c /home/linuxbrew/RedGpuSDK/redgpu.c /home/linuxbrew/RedGpuSDK/redgpu_2.c /home/linuxbrew/RedGpuSDK/redgpu_32.c -I/home/linuxbrew/.linuxbrew/include/ -I/home/linuxbrew/.linuxbrew/Cellar/xorgproto/2025.1/include/ -I/var/home/linuxbrew/.linuxbrew/Cellar/libxcb/1.17.0/include/
g++ -O0 -g main.cpp *.o -I/home/linuxbrew/.linuxbrew/include/ -I/home/linuxbrew/.linuxbrew/Cellar/xorgproto/2025.1/include/ -I/var/home/linuxbrew/.linuxbrew/Cellar/libxcb/1.17.0/include/ /home/linuxbrew/.linuxbrew/lib/libX11.so /home/linuxbrew/.linuxbrew/lib/libvulkan.so -lm
exit
#endif
#if 0
clang -c ../../vkfast.c C:/RedGpuSDK/redgpu.c C:/RedGpuSDK/redgpu_2.c C:/RedGpuSDK/redgpu_32.c && clang++ main.cpp *.o
exit
#endif

#include "../../vkfast.h"
#define VKFAST_EXAMPLES_COMMON_INCLUDE_GLM
#include "../Common/vkfast_examples_common.h"
using namespace glm;

int some_texture_channels_count = 3; // NOTE(Constantine): Hardcoded.
int some_texture_w = 0;
int some_texture_h = 0;
unsigned char * some_texture = NULL;

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

  vec2 fetch = weights.x * uv0 + weights.y * uv1 + weights.z * uv2;

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

  return vec3(r / 255.f, g / 255.f, b / 255.f);
}

#if defined(_WIN32)
  #define FileMap(X_utf16, X_utf8, Y, Z, W) red32FileMap((const short unsigned int *)(X_utf16), Y, Z, W)
#elif defined(__linux__) && !defined(__ANDROID__)
  #define FileMap(X_utf16, X_utf8, Y, Z, W) red32FileMap((const short unsigned int *)(X_utf8), Y, Z, W)
#else
  #error Unsupported OS for now
#endif

int main() {
#if defined(__MINGW32__)
  SetProcessDPIAware();
#elif defined(_WIN32)
  SetProcessDpiAwareness(PROCESS_PER_MONITOR_DPI_AWARE);
#endif

#if defined(_MSC_VER) && defined(_DEBUG)
  _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

  #define window_w 1920
  #define window_h 1080

  int windowMonitorArea[4] = {0};
  vfGetMainMonitorAreaRectangle(windowMonitorArea, FF, LL);
  REDGPU_2_EXPECTFL(windowMonitorArea[2] == window_w);
  REDGPU_2_EXPECTFL(windowMonitorArea[3] == window_h);

  gpu_handle_context_t ctx = vfContextInit(1, NULL, FF, LL);
  vfWindowFullscreen(ctx, NULL, "[vkFast] RT Triangle Textured CPU", window_w, window_h, 0, RED_PRESENT_VSYNC_MODE_ON, FF, LL);

  const unsigned array65536[2] = {65536, 65536};

  gpu_thread_t gpu_thread = NULL;
  vfGpuThreadCreate(ctx, 1, &gpu_thread, NULL, FF, LL);

  struct Pixels {
    unsigned char pixels[window_h][window_w][4];
  };
  // To free
  struct Pixels * pix = (struct Pixels *)red32MemoryCalloc(sizeof(struct Pixels));
  REDGPU_2_EXPECTFL(pix != NULL);

  void * texture_fd = NULL;
  void * texture_mh = NULL;
  void * texture_data = NULL;
  // To close
  FileMap(L"texture.ppm", "texture.ppm", &texture_fd, &texture_mh, &texture_data);
  REDGPU_2_EXPECTFL(texture_fd != NULL || !"If you're running the example from Visual Studio, copy the 'texture.ppm' texture to this example's vs2019/ folder.");
  REDGPU_2_EXPECTFL(texture_mh != NULL);
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

    time += 0.2f; // NOTE(Constantine): Speeded up.

    gpu_thread_t gpu_threads[2] = {gpu_thread, 0};
    vfDrawPixels(ctx, pix->pixels, NULL, 2, gpu_threads, array65536, FF, LL);
  }
  
  vfAllQueuesWaitIdle(ctx, FF, LL);

  vfGpuThreadDestroy(ctx, gpu_thread);

  REDGPU_2_EXPECTFL(0 == red32FileUnmap(texture_fd, texture_mh, texture_data));
  texture_fd = NULL;
  texture_mh = NULL;
  texture_data = NULL;

  red32MemoryFree(pix);
  pix = NULL;

  vfContextDeinit(ctx, FF, LL);
  vfExit(0);
}

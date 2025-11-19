#if 0
clang main.c ../../vkfast.c C:/RedGpuSDK/redgpu.c C:/RedGpuSDK/redgpu_2.c C:/RedGpuSDK/redgpu_32.c
exit
#endif

#include "../../vkfast.h"

#include <stdio.h> // For printf

#include <shellscalingapi.h>   // For SetProcessDpiAwareness
#pragma comment(lib, "shcore") // For SetProcessDpiAwareness

#define countof(x) (sizeof(x) / sizeof((x)[0]))

#define FF __FILE__
#define LL __LINE__

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
  vfWindowFullscreen(ctx, NULL, "[vkFast] Mandelbrot set CPU", window_w, window_h, 0, FF, LL);

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
    // Draw pixels:
    for (int iy = 0; iy < window_h; iy += 1) {
      for (int ix = 0; ix < window_w; ix += 1) {
        int   W = window_w;
        int   H = window_h;
        int   X = (int)(ix);
        int   Y = (int)(iy);
        float U = (float)(X) / (W-1);
        float V = (float)(Y) / (H-1);

        float cx = (U - 0.5f) * 3.f * (16.f / 9.f);
        float cy = (V - 0.5f) * 3.f;
        int i    = 0;
        int iter = 100;
        float zx = cx;
        float zy = cy;
        for (i = 0; i < iter; i += 1) {
          float x = (zx * zx - zy * zy) + cx;
          float y = (zy * zx + zx * zy) + cy;
          if ((x * x + y * y) > 4.f) {
            break;
          }
          zx = x;
          zy = y;
        }

        pixels[iy * window_w * 4 + ix * 4 + 0] = ((i == iter ? 0.f : (float)(i)) / 50.f) * 255.f;
        pixels[iy * window_w * 4 + ix * 4 + 1] = 0;
        pixels[iy * window_w * 4 + ix * 4 + 2] = 0;
        pixels[iy * window_w * 4 + ix * 4 + 3] = 255;
      }
    }
    vfDrawPixels(ctx, pix->pixels, NULL, FF, LL);
    vfAsyncDrawWaitToFinish(ctx, FF, LL);
  }
  
  red32MemoryFree(pix);
  pix = NULL;

  vfContextDeinit(ctx, FF, LL);
  vfExit(0);
}

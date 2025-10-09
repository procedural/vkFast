#include "../../vkfast.h"
#include "../../vkfast_ids.h"

#include <stdio.h> // For printf

#include <shellscalingapi.h>   // For SetProcessDpiAwareness
#pragma comment(lib, "shcore") // For SetProcessDpiAwareness

#ifdef _WIN32
#define GLFW_INCLUDE_NONE
#define GLFW_EXPOSE_NATIVE_WIN32
#include "glfw-3.4.bin.WIN64/include/GLFW/glfw3.h" // https://github.com/glfw/glfw/releases/download/3.4/glfw-3.4.bin.WIN64.zip
#include "glfw-3.4.bin.WIN64/include/GLFW/glfw3native.h" // https://github.com/glfw/glfw/releases/download/3.4/glfw-3.4.bin.WIN64.zip
#pragma comment(lib, "../glfw-3.4.bin.WIN64/lib-vc2019/glfw3_mt.lib") // https://github.com/glfw/glfw/releases/download/3.4/glfw-3.4.bin.WIN64.zip
#endif

#define countof(x) (sizeof(x) / sizeof((x)[0]))

#define FF __FILE__
#define LL __LINE__

int main() {
  SetProcessDpiAwareness(PROCESS_PER_MONITOR_DPI_AWARE);

  glfwInit();
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
  GLFWwindow * window1 = glfwCreateWindow(700, 700, "[vkFast] GLFW Two Windows Drawing: Window One", NULL, NULL);
  GLFWwindow * window2 = glfwCreateWindow(500, 500, "[vkFast] GLFW Two Windows Drawing: Window Two", NULL, NULL);
  void * window1_handle = (void *)glfwGetWin32Window(window1);
  void * window2_handle = (void *)glfwGetWin32Window(window2);

  gpu_handle_context_t ctx1 = vfContextInit(1, NULL, FF, LL);
  
  // Donate raw context from ctx to ctx2.
  vf_handle_context_t vfctx2 = {0};
  vfctx2.doNotDestroyRawContext = 1; // Since we donate raw context from ctx1.
  vfctx2.doNotFreeHandle        = 1; // Since we created vfctx2 on the stack.
  vfctx2.context                = vfContextGetRaw(ctx1, FF, LL);
  // Custom initialization of ctx2.
  gpu_internal_memory_allocation_sizes_t ctx2allocsizes = {0};
  ctx2allocsizes.bytes_count_for_memory_storages_type_gpu_only     = 0;
  ctx2allocsizes.bytes_count_for_memory_storages_type_cpu_upload   = (128 * 1024 * 1024);
  ctx2allocsizes.bytes_count_for_memory_storages_type_cpu_readback = 0;
  gpu_context_optional_parameters_t ctx2params = {0};
  ctx2params.internal_memory_allocation_sizes             = &ctx2allocsizes;
  ctx2params.optional_pointer_to_custom_vf_handle_context = (void *)&vfctx2;
  gpu_handle_context_t ctx2 = vfContextInit(0, &ctx2params, FF, LL);

  vfWindowFullscreen(ctx1, window1_handle, "[vkFast] GLFW Two Windows Drawing: Window One", 700, 700, 0, FF, LL);
  vfWindowFullscreen(ctx2, window2_handle, "[vkFast] GLFW Two Windows Drawing: Window Two", 500, 500, 1, FF, LL);

  while (glfwWindowShouldClose(window1) == 0 && glfwWindowShouldClose(window2) == 0) {
    glfwPollEvents();

    GLFWwindow * windows[2] = {window1, window2};
    gpu_handle_context_t ctxs[2] = {ctx1, ctx2};

    for (int i = 0; i < 2; i += 1) {
      gpu_handle_context_t ctx = ctxs[i];

      int window_w = 0;
      int window_h = 0;
      vfWindowGetSize(ctx, &window_w, &window_h);

      // To free
      unsigned char * pixels = (unsigned char *)red32MemoryCalloc(4 * window_h * window_w);
      REDGPU_2_EXPECTFL(pixels != NULL);

      double mouse_x = 0;
      double mouse_y = 0;
      glfwGetCursorPos(windows[i], &mouse_x, &mouse_y);

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
      for (int y = 0; y < window_h; y += 1) {
        for (int x = 0; x < window_w; x += 1) {
          // NOTE(Constantine):
          // The automatic DPI scaling is disabled with SetProcessDpiAwareness(PROCESS_PER_MONITOR_DPI_AWARE), but
          // with DPI scaling, 1920x1080 screen resolution with a desktop scaling of, say, 1.25 is equal to 1536x864 (draw coords: 1535x863).
          for (int i = -2; i < 2; i += 1) {
            if (y == 0+i) {
              pixels[y * window_w * 4 + x * 4 + 0] = 255;
              pixels[y * window_w * 4 + x * 4 + 1] = 0;
              pixels[y * window_w * 4 + x * 4 + 2] = 0;
              pixels[y * window_w * 4 + x * 4 + 3] = 255;
            } else if (x == 0+i) {
              pixels[y * window_w * 4 + x * 4 + 0] = 0;
              pixels[y * window_w * 4 + x * 4 + 1] = 255;
              pixels[y * window_w * 4 + x * 4 + 2] = 0;
              pixels[y * window_w * 4 + x * 4 + 3] = 255;
            } else if (y == mouse_y+i) {
              pixels[y * window_w * 4 + x * 4 + 0] = 0;
              pixels[y * window_w * 4 + x * 4 + 1] = 0;
              pixels[y * window_w * 4 + x * 4 + 2] = 255;
              pixels[y * window_w * 4 + x * 4 + 3] = 255;
            } else if (x == mouse_x+i) {
              pixels[y * window_w * 4 + x * 4 + 0] = 255;
              pixels[y * window_w * 4 + x * 4 + 1] = 255;
              pixels[y * window_w * 4 + x * 4 + 2] = 255;
              pixels[y * window_w * 4 + x * 4 + 3] = 255;
            }
          }
        }
      }

      vfDrawPixels(ctx, pixels, FF, LL);

      red32MemoryFree(pixels);
      pixels = NULL;
    }
  }

  vfContextDeinit(ctx2, FF, LL);
  vfContextDeinit(ctx1, FF, LL);

  vfExit(0);
}

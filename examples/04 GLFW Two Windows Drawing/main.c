//\\rc rawbuild begin gcc-linux-64-bit
//\\rc rawbuild require debug,release,release-fast
//\\rc rawbuild require glfw3,sdl3
//\\rc rawbuild `gcc`
//\\rc rawbuild debug ` -g -O0`
//\\rc rawbuild release,release-fast ` -O2`
//\\rc rawbuild sdl3 ` -DVKFAST_EXAMPLE_04_ENABLE_GLFW3_TO_SDL3`
//\\rc rawbuild ` main.c ../../vkfast.c /home/linuxbrew/RedGpuSDK/redgpu.c /home/linuxbrew/RedGpuSDK/redgpu_2.c /home/linuxbrew/RedGpuSDK/redgpu_32.c -I/home/linuxbrew/.linuxbrew/include/ -I/home/linuxbrew/.linuxbrew/Cellar/xorgproto/2025.1/include/ -I/var/home/linuxbrew/.linuxbrew/Cellar/libxcb/1.17.0/include/`
//\\rc rawbuild glfw3 ` /home/linuxbrew/.linuxbrew/Cellar/glfw/3.4/lib/libglfw3.a`
//\\rc rawbuild sdl3 ` /home/linuxbrew/.linuxbrew/lib/libSDL3.so`
//\\rc rawbuild ` /home/linuxbrew/.linuxbrew/lib/libX11.so /home/linuxbrew/.linuxbrew/lib/libvulkan.so -lm`
//\\rc rawbuild end

//\\rc rawbuild begin clang-windows-64-bit
//\\rc rawbuild require debug,release,release-fast
//\\rc rawbuild `clang`
//\\rc rawbuild debug ` -g -O0`
//\\rc rawbuild release,release-fast ` -O2`
//\\rc rawbuild ` main.c ../../vkfast.c C:/RedGpuSDK/redgpu.c C:/RedGpuSDK/redgpu_2.c C:/RedGpuSDK/redgpu_32.c ../Common/glfw-3.4.bin.WIN64/lib-mingw-w64/libglfw3.a -lgdi32`
//\\rc rawbuild end

#include "../../vkfast.h"
#include "../../vkfast_ids.h"
#ifndef VKFAST_EXAMPLE_04_ENABLE_GLFW3_TO_SDL3
#define VKFAST_EXAMPLES_COMMON_INCLUDE_GLFW3
#endif
#include "../Common/vkfast_examples_common.h"
#ifdef VKFAST_EXAMPLE_04_ENABLE_GLFW3_TO_SDL3
#include "../../extra/GLFW3 to SDL3/vkfast_extra_glfw3_to_sdl3.h"
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

  glfwInit();
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
  GLFWwindow * window1 = glfwCreateWindow(700, 700, "[vkFast] GLFW Two Windows Drawing: Window One", NULL, NULL);
  GLFWwindow * window2 = glfwCreateWindow(500, 500, "[vkFast] GLFW Two Windows Drawing: Window Two", NULL, NULL);
#if defined(_WIN32)
  void * window1_handle = (void *)glfwGetWin32Window(window1);
  void * window2_handle = (void *)glfwGetWin32Window(window2);
#elif defined(__linux__) && !defined(__ANDROID__)
  // NOTE(Constantine): this struct's layout is defined in redgpu_32.c file of REDGPU 2 SDK.
  struct X11WindowData {
    Display * display;
    Window    window;
    Atom      wmDeleteMessage;
  };
  struct X11WindowData window1Data = {0};
  struct X11WindowData window2Data = {0};
  #ifdef VKFAST_EXTRA_INCLUDED_GLFW3_TO_SDL3
  window1Data.display = glfwGetX11Display(window1);
  window2Data.display = glfwGetX11Display(window2);
  #else
  window1Data.display = glfwGetX11Display();
  window2Data.display = glfwGetX11Display();
  #endif
  window1Data.window = glfwGetX11Window(window1);
  window2Data.window = glfwGetX11Window(window2);
  window1Data.wmDeleteMessage = 0;
  window2Data.wmDeleteMessage = 0;
  REDGPU_2_EXPECTFL(window1Data.display != NULL || !"On Wayland, you need to run the app like this: XDG_SESSION_TYPE=x11 ./a.out");
  REDGPU_2_EXPECTFL(window1Data.window  != 0    || !"On Wayland, you need to run the app like this: XDG_SESSION_TYPE=x11 ./a.out");
  REDGPU_2_EXPECTFL(window2Data.display != NULL || !"On Wayland, you need to run the app like this: XDG_SESSION_TYPE=x11 ./a.out");
  REDGPU_2_EXPECTFL(window2Data.window  != 0    || !"On Wayland, you need to run the app like this: XDG_SESSION_TYPE=x11 ./a.out");
  void * window1_handle = &window1Data;
  void * window2_handle = &window2Data;
#endif

  gpu_handle_context_t ctx1 = vfContextInit(1, NULL, FF, LL);
  
  // Donate raw context from ctx to ctx2.
  vf_handle_context_t vfctx2 = {0};
  vfctx2.doNotDestroyRawContext = 1; // Since we donate raw context from ctx1.
  vfctx2.doNotFreeHandle        = 1; // Since we created vfctx2 on the stack.
  vfctx2.context                = vfContextGetRaw(ctx1, FF, LL);
  // Custom initialization of ctx2.
  gpu_internal_memory_allocation_sizes_t ctx2allocsizes = {0};
  ctx2allocsizes.bytes_count_for_memory_storages_type_gpu_only         = 0;
  ctx2allocsizes.bytes_count_for_memory_storages_type_cpu_upload       = 0;
  ctx2allocsizes.bytes_count_for_memory_storages_type_cpu_readback     = 0;
  ctx2allocsizes.bytes_count_for_memory_present_pixels_type_cpu_upload = VKFAST_DEFAULT_MEMORY_ALLOCATION_SIZE_PRESENT_PIXELS_CPU_UPLOAD_288MB;
  gpu_context_optional_parameters_t ctx2params = {0};
  ctx2params.internal_memory_allocation_sizes             = &ctx2allocsizes;
  ctx2params.optional_pointer_to_custom_vf_handle_context = (void *)&vfctx2;
  gpu_handle_context_t ctx2 = vfContextInit(0, &ctx2params, FF, LL);

  const unsigned array65536[2] = {65536, 65536};

  gpu_thread_t gpu_thread_ctx1 = NULL;
  gpu_thread_t gpu_thread_ctx2 = NULL;
  vfGpuThreadCreate(ctx1, 1, &gpu_thread_ctx1, NULL, FF, LL);
  vfGpuThreadCreate(ctx2, 1, &gpu_thread_ctx2, NULL, FF, LL);

  vfWindowFullscreen(ctx1, window1_handle, "[vkFast] GLFW Two Windows Drawing: Window One", 700, 700, 0, RED_PRESENT_VSYNC_MODE_ON, FF, LL);
  vfWindowFullscreen(ctx2, window2_handle, "[vkFast] GLFW Two Windows Drawing: Window Two", 500, 500, 0, RED_PRESENT_VSYNC_MODE_ON, FF, LL);

  while (glfwWindowShouldClose(window1) == 0 && glfwWindowShouldClose(window2) == 0) {
    #ifdef VKFAST_EXTRA_INCLUDED_GLFW3_TO_SDL3
    GLFWwindow * glfwwindows[2] = {window1, window2};
    glfwPollEvents(2, glfwwindows);
    #else
    glfwPollEvents();
    #endif

    GLFWwindow * windows[2] = {window1, window2};
    gpu_handle_context_t ctxs[2] = {ctx1, ctx2};

    for (int i = 0; i < 2; i += 1) {
      GLFWwindow * window = windows[i];
      gpu_handle_context_t ctx = ctxs[i];

      int os_window_w = 0;
      int os_window_h = 0;
      glfwGetWindowSize(window, &os_window_w, &os_window_h);

      if (vfWindowIsMinimized(ctx) || os_window_w == 0 || os_window_h == 0) {
        continue;
      }

      int window_w = 0;
      int window_h = 0;
      vfWindowGetSize(ctx, &window_w, &window_h);

      // To free
      unsigned char * pixels = (unsigned char *)red32MemoryCalloc(4 * window_h * window_w);
      REDGPU_2_EXPECTFL(pixels != NULL);

      double mouse_x = 0;
      double mouse_y = 0;
      glfwGetCursorPos(window, &mouse_x, &mouse_y);

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
          if (y == 0-2 || y == 0-1 || y == 0+0 || y == 0+1 || y == 0+2) {
            pixels[y * window_w * 4 + x * 4 + 0] = 255;
            pixels[y * window_w * 4 + x * 4 + 1] = 0;
            pixels[y * window_w * 4 + x * 4 + 2] = 0;
            pixels[y * window_w * 4 + x * 4 + 3] = 255;
          } else if (x == 0-2 || x == 0-1 || x == 0+0 || x == 0+1 || x == 0+2) {
            pixels[y * window_w * 4 + x * 4 + 0] = 0;
            pixels[y * window_w * 4 + x * 4 + 1] = 255;
            pixels[y * window_w * 4 + x * 4 + 2] = 0;
            pixels[y * window_w * 4 + x * 4 + 3] = 255;
          } else if (y == mouse_y-2 || y == mouse_y-1 || y == mouse_y+0 || y == mouse_y+1 || y == mouse_y+2) {
            pixels[y * window_w * 4 + x * 4 + 0] = 0;
            pixels[y * window_w * 4 + x * 4 + 1] = 0;
            pixels[y * window_w * 4 + x * 4 + 2] = 255;
            pixels[y * window_w * 4 + x * 4 + 3] = 255;
          } else if (x == mouse_x-2 || x == mouse_x-1 || x == mouse_x+0 || x == mouse_x+1 || x == mouse_x+2) {
            pixels[y * window_w * 4 + x * 4 + 0] = 255;
            pixels[y * window_w * 4 + x * 4 + 1] = 255;
            pixels[y * window_w * 4 + x * 4 + 2] = 255;
            pixels[y * window_w * 4 + x * 4 + 3] = 255;
          }
        }
      }

      gpu_thread_t gpu_threads[2] = {i == 0 ? gpu_thread_ctx1 : gpu_thread_ctx2, 0};
      vfDrawPixels(ctx, pixels, NULL, 2, gpu_threads, array65536, FF, LL);

      red32MemoryFree(pixels);
      pixels = NULL;
    }
  }

  vfAllQueuesWaitIdle(ctx1, FF, LL);
  vfAllQueuesWaitIdle(ctx2, FF, LL);

  vfGpuThreadDestroy(ctx1, gpu_thread_ctx1);
  vfGpuThreadDestroy(ctx2, gpu_thread_ctx2);

  vfContextDeinit(ctx2, FF, LL);
  vfContextDeinit(ctx1, FF, LL);

  #ifdef VKFAST_EXTRA_INCLUDED_GLFW3_TO_SDL3
  glfwTerminateWindow(window1);
  glfwTerminateWindow(window2);
  #endif

  glfwTerminate();
}

#if 0
# Build and run commands:
source /opt/intel/oneapi/setvars.sh intel64
icx -c ../../../vkfast.c "../../../extra/CPU GPU Array/vkfast_extra_cpu_gpu_array.c" ../../../extra/REII/vkfast_extra_reii.c /home/linuxbrew/RedGpuSDK/redgpu.c /home/linuxbrew/RedGpuSDK/redgpu_2.c /home/linuxbrew/RedGpuSDK/redgpu_32.c -I/home/linuxbrew/.linuxbrew/include/ -I/home/linuxbrew/.linuxbrew/Cellar/xorgproto/2025.1/include/ -I/var/home/linuxbrew/.linuxbrew/Cellar/libxcb/1.17.0/include/
icpx -fsycl -fsycl-targets=spir64 -Xclang -fsycl-allow-func-ptr main.cpp *.o -I/home/linuxbrew/.linuxbrew/include/ -I/home/linuxbrew/.linuxbrew/Cellar/xorgproto/2025.1/include/ -I/var/home/linuxbrew/.linuxbrew/Cellar/libxcb/1.17.0/include/ /home/linuxbrew/.linuxbrew/Cellar/glfw/3.5.1/lib/libglfw3.a /home/linuxbrew/.linuxbrew/lib/libX11.so /home/linuxbrew/.linuxbrew/lib/libvulkan.so -lm -o a.out
XDG_SESSION_TYPE=x11 ./a.out
#endif

#include <iostream>
#include <vector>
#include <limits>

// NOTE(Constantine)(Sep 18, 2026): X11/X.h defines None, so we include sycl.hpp before vkFast.
#include <sycl/sycl.hpp>

#include "../../../vkfast.h"
#include "../../../extra/REII/vkfast_extra_reii.h"
#include "../../../extra/vkFast Extensions/ReBAR/vkfast_ext_rebar.h"
#define VKFAST_EXAMPLES_COMMON_INCLUDE_GLFW3
#include "../../../examples/Common/vkfast_examples_common.h"

// Variable to capture scroll wheel deltas across frames
double scrollDeltaY = 0.0;

// Callback function to catch scroll wheel inputs
void mouseCallback(GLFWwindow * window, double xoffset, double yoffset) {
  scrollDeltaY += yoffset; // Accumulate scroll input
}

int main() {
  #define WIDTH 700
  #define HEIGHT 700

  glfwInit();
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  glfwWindowHint(GLFW_RESIZABLE, 0);
  GLFWwindow * window = glfwCreateWindow(WIDTH, HEIGHT, "[vkFast] Sycl Mandelbrot REII", 0, 0);
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

  glfwSetScrollCallback(window, mouseCallback);

  gpu_handle_context_t ctx = vfContextInitNoDefaultAllocs(1, NULL, FF, LL);
  vfWindowFullscreen(ctx, window_handle, "[vkFast] Sycl Mandelbrot REII", WIDTH, HEIGHT, 0, RED_PRESENT_VSYNC_MODE_ON, FF, LL);

  const unsigned array65536[2] = {65536, 65536};

  gpu_thread_t gpu_thread = NULL;
  vfGpuThreadCreate(ctx, 1, &gpu_thread, NULL, FF, LL);

  struct Pixels {
    unsigned char pixels[HEIGHT][WIDTH][4];
  };
  VfeReBARMallocShared pixelsHandles = {};
  volatile struct Pixels * pix = (volatile struct Pixels *)vfeReBARMallocShared(ctx, sizeof(struct Pixels), &pixelsHandles);

  sycl::property_list sycl_queue_properties{sycl::property::queue::enable_profiling()};
  sycl::queue sycl_queue(sycl::gpu_selector_v, sycl_queue_properties);

  // Create an output frame buffer using USM shared allocation
  uint8_t * pixels = (uint8_t *)sycl::malloc_shared(WIDTH * HEIGHT * 4, sycl_queue);

  // View offsets for panning
  float offsetX = 0.0f;
  float offsetY = 0.0f;

  // Mouse state tracking
  double lastX = 0.0;
  double lastY = 0.0;
  int wasPressed = 0;

  float zoom = 3.0f;

  while (glfwWindowShouldClose(window) == 0) {
    glfwPollEvents();

    int os_window_w = 0;
    int os_window_h = 0;
    glfwGetWindowSize(window, &os_window_w, &os_window_h);

    if (vfWindowIsMinimized(ctx) || os_window_w == 0 || os_window_h == 0) {
      continue;
    }

    // Calculate Zoom Scale from Scroll Wheel
    if (scrollDeltaY != 0.0) {
      if (scrollDeltaY > 0.0) {
        zoom *= 0.9f; // Zoom In (shrink the coordinate space)
      } else {
        zoom *= 1.1f; // Zoom Out (expand the coordinate space)
      }
      scrollDeltaY = 0.0; // Reset scroll delta for the next frame
    }

    // Check if the Left Mouse Button is currently held down
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
      double currentX = 0;
      double currentY = 0;
      glfwGetCursorPos(window, &currentX, &currentY);

      if (wasPressed) {
        // Calculate how far the mouse moved since the last frame
        double deltaX = currentX - lastX;
        double deltaY = currentY - lastY;

        // Convert pixel delta into Mandelbrot coordinate scaling
        offsetX -= (float)(deltaX / WIDTH)  * zoom;
        offsetY -= (float)(deltaY / HEIGHT) * zoom;
      }

      // Save current positions for the next frame's comparison
      lastX = currentX;
      lastY = currentY;
      wasPressed = 1; // Mark that dragging is active
    } else {
      wasPressed = 0; // Reset state when button is released
    }

    sycl_queue.submit([&](sycl::handler& sycl_cgh) {
      sycl_cgh.parallel_for(sycl::range<2>(HEIGHT, WIDTH), [=](sycl::id<2> sycl_id) {
        int iy = sycl_id[0];
        int ix = sycl_id[1];

        int   W = WIDTH;
        int   H = HEIGHT;
        int   X = (int)(ix);
        int   Y = (int)(iy);
        float U = (float)(X) / (W-1);
        float V = (float)(Y) / (H-1);

        float cx = (U - 0.5f) * zoom + offsetX; // * (16.f / 9.f);
        float cy = (V - 0.5f) * zoom + offsetY;
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

        pixels[iy * WIDTH * 4 + ix * 4 + 0/*Blue*/]  = ((i == iter ? 0.f : (float)(i)) / 50.f) * 255.f;
        pixels[iy * WIDTH * 4 + ix * 4 + 1/*Green*/] = 0;
        pixels[iy * WIDTH * 4 + ix * 4 + 2/*Red*/]   = 0;
        pixels[iy * WIDTH * 4 + ix * 4 + 3/*Alpha*/] = 255;
      });
    });

    // Wait for execution to finish
    sycl_queue.wait();

    // Copy pixels and draw
    memcpy((void *)&pix->pixels[0][0][0], pixels, WIDTH * HEIGHT * 4);

    gpu_thread_t gpu_threads[2] = {gpu_thread, 0};
    vfAsyncDrawPixelsRaw(ctx, &pixelsHandles.storageRaw, NULL, 2, gpu_threads, array65536, FF, LL);
  }

  vfAllQueuesWaitIdle(ctx, FF, LL);

  // Cleanup
  sycl::free(pixels, sycl_queue);

  vfGpuThreadDestroy(ctx, gpu_thread);
  vfeReBARFreeShared(ctx, &pixelsHandles);
  vfContextDeinit(ctx, FF, LL);
  glfwTerminate();

  vfExit(0);
}

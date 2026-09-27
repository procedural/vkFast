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
#define VKFAST_EXAMPLES_COMMON_INCLUDE_GLM
#include "../../../examples/Common/vkfast_examples_common.h"

using namespace glm;

vec4 circle(vec2 p, vec2 center, float radius)
{
  // Renders a smooth red circle on a transparent background
  return mix(vec4(1,1,1,0), vec4(1,0,0,1), smoothstep(radius + 0.005f, radius - 0.005f, length(p - center)));
}

vec4 scene(vec2 uv, float t)
{
  // Bouncing animation logic
  return circle(uv, vec2(0, sin(t * 16.0) * (sin(t) * 0.5 + 0.5) * 0.5), 0.2);
}

void mainImage(vec4 & fragColor, vec2 fragCoord, vec2 iResolution, vec2 iMouse, float iTime, float iFrame) {
  // Normalize coordinates for the full screen
  vec2 uv = fragCoord.xy() / iResolution.xy();
  uv = uv * 2.0f - vec2(1.0f);
  uv.x *= iResolution.x / iResolution.y; // Correct aspect ratio

  vec4 col = scene(uv, iTime);

  fragColor = col;
}

#define WINDOW_WIDTH  384
#define WINDOW_HEIGHT 384

#include <thread>
#include <chrono>
void limitFPS(double targetFPS, std::chrono::steady_clock::time_point frameStartTime) {
  // 1. Convert target FPS into a precise chrono duration
  double targetDurationSecs = 1.0 / targetFPS;
  auto targetDuration = std::chrono::duration<double>(targetDurationSecs);

  // 2. Establish our precise target time point
  // We mix GLFW time with the stable steady_clock for scheduling
  auto targetEndPoint = frameStartTime + std::chrono::duration_cast<std::chrono::steady_clock::duration>(targetDuration);

  // 3. Coarse Sleep Phase (Heavy Power Saving)
  // Sleep until 2 milliseconds before our actual deadline
  using namespace std::literals::chrono_literals;
  auto coarseWaitLimit = targetEndPoint - 2ms;
  if (std::chrono::steady_clock::now() < coarseWaitLimit) {
    std::this_thread::sleep_until(coarseWaitLimit);
  }

  // 4. Fine-Grained Yield Phase (Eco-Friendly Waiting)
  // Instead of a tight CPU-melting loop, we yield control to other OS threads
  while (std::chrono::steady_clock::now() < targetEndPoint) {
    std::this_thread::yield();
  }
}

int main() {
  glfwInit();
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
  GLFWwindow * window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "[vkFast] Vsync Off Motion Blur", 0, 0);
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
  vfWindowFullscreen(ctx, window_handle, "[vkFast] Vsync Off Motion Blur", WINDOW_WIDTH, WINDOW_HEIGHT, 0, RED_PRESENT_VSYNC_MODE_OFF, FF, LL);

  const unsigned array65536[2] = {65536, 65536};

  gpu_thread_t gpu_thread = NULL;
  vfGpuThreadCreate(ctx, 1, &gpu_thread, NULL, FF, LL);

  struct Pixels {
    unsigned char pixels[WINDOW_HEIGHT][WINDOW_WIDTH][4];
  };

  struct PixelsSamples {
    float pixels[WINDOW_HEIGHT][WINDOW_WIDTH][4];
  };

  sycl::property_list sycl_queue_properties{sycl::property::queue::enable_profiling()};
  sycl::queue sycl_queue(sycl::gpu_selector_v, sycl_queue_properties);

  std::cout << "Running Sycl on device: " << sycl_queue.get_device().get_info<sycl::info::device::name>() << std::endl;

  // Create an output frame buffer using USM shared allocation
  struct Pixels * pixels = (struct Pixels *)sycl::malloc_shared(sizeof(struct Pixels), sycl_queue);
  struct PixelsSamples * pixelsSamples = (struct PixelsSamples *)sycl::malloc_shared(sizeof(struct PixelsSamples), sycl_queue);

  VfeReBARMallocShared pixelsHandles = {};
  volatile struct Pixels * pix = (volatile struct Pixels *)vfeReBARMallocShared(ctx, sizeof(struct Pixels), &pixelsHandles);

  // Mouse state tracking
  double lastX = 0.0;
  double lastY = 0.0;
  int wasPressed = 0;

  vec2    iResolution = {WINDOW_WIDTH, WINDOW_HEIGHT};
  vec2    iMouse      = {0, 0};
  float   iTime       = 0;
  int64_t iFrame      = 0;

  const int motionBlurSamples = 32;

  float fps_limiter_target_fps = 60 * motionBlurSamples;
  std::chrono::steady_clock::time_point fps_limiter_frame_start_time = std::chrono::steady_clock::now();

  while (glfwWindowShouldClose(window) == 0) {
    glfwPollEvents();

    fps_limiter_frame_start_time = std::chrono::steady_clock::now();

    // Check if the Left Mouse Button is currently held down
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
      double currentX = 0;
      double currentY = 0;
      glfwGetCursorPos(window, &currentX, &currentY);

      if (wasPressed) {
        // Calculate how far the mouse moved since the last frame
        double deltaX = currentX - lastX;
        double deltaY = currentY - lastY;

        // Convert pixel delta into coordinate scaling
        iMouse.x -= (float)(deltaX);
        iMouse.y -= (float)(deltaY);
      }

      // Save current positions for the next frame's comparison
      lastX = currentX;
      lastY = currentY;
      wasPressed = 1; // Mark that dragging is active
    } else {
      wasPressed = 0; // Reset state when button is released
    }

    auto syclQueueSubmitEvent =sycl_queue.submit([&](sycl::handler& sycl_cgh) {
      sycl_cgh.parallel_for(sycl::range<2>(WINDOW_HEIGHT, WINDOW_WIDTH), [=](sycl::id<2> sycl_id) {
        int y = sycl_id[0];
        int x = sycl_id[1];

        float xf = (float)x;
        float yf = (float)y;

        vec4 color = {};
        vec2 fragCoord = {xf + 0.5f, (WINDOW_HEIGHT-yf) + 0.5f}; // https://registry.khronos.org/OpenGL-Refpages/gl4/html/gl_FragCoord.xhtml

        mainImage(color, fragCoord, iResolution, iMouse, iTime, (float)iFrame);

        pixelsSamples->pixels[y][x][0] += color.r;
        pixelsSamples->pixels[y][x][1] += color.g;
        pixelsSamples->pixels[y][x][2] += color.b;
        pixelsSamples->pixels[y][x][3] += color.a;
      });
    });

    // Wait for execution to finish
    sycl_queue.wait();

    if (0) {
      auto end   = syclQueueSubmitEvent.get_profiling_info<sycl::info::event_profiling::command_end>();
      auto start = syclQueueSubmitEvent.get_profiling_info<sycl::info::event_profiling::command_start>();

      std::cout << "Sycl queue submit elapsed time: " << (end - start) / 1000.0 << " microseconds.\n";
    }

    if (iFrame % motionBlurSamples == 0) {
      // Average accumulated samples
      #pragma omp parallel for
      for (int y = 0; y < WINDOW_HEIGHT; y += 1) {
        #pragma omp parallel for
        for (int x = 0; x < WINDOW_WIDTH; x += 1) {
          unsigned char r = (unsigned char)((pixelsSamples->pixels[y][x][0] / (float)(motionBlurSamples)) * 255.0f);
          unsigned char g = (unsigned char)((pixelsSamples->pixels[y][x][1] / (float)(motionBlurSamples)) * 255.0f);
          unsigned char b = (unsigned char)((pixelsSamples->pixels[y][x][2] / (float)(motionBlurSamples)) * 255.0f);
          unsigned char a = (unsigned char)((pixelsSamples->pixels[y][x][3] / (float)(motionBlurSamples)) * 255.0f);

          // NOTE(Constantine): pixels are in BGRA order.
          pixels->pixels[y][x][0] = b;
          pixels->pixels[y][x][1] = g;
          pixels->pixels[y][x][2] = r;
          pixels->pixels[y][x][3] = a;
        }
      }

      // Copy pixels and draw
      memcpy((void *)&pix->pixels[0][0][0], (void *)&pixels->pixels[0][0][0], WINDOW_WIDTH * WINDOW_HEIGHT * 4);

      gpu_thread_t gpu_threads[2] = {gpu_thread, 0};
      vfAsyncDrawPixelsRaw(ctx, &pixelsHandles.storageRaw, NULL, 2, gpu_threads, array65536, FF, LL);

      // Clear previous samples
      {
        #pragma omp parallel for
        for (int y = 0; y < WINDOW_HEIGHT; y += 1) {
          #pragma omp parallel for
          for (int x = 0; x < WINDOW_WIDTH; x += 1) {
            pixelsSamples->pixels[y][x][0] = 0;
            pixelsSamples->pixels[y][x][1] = 0;
            pixelsSamples->pixels[y][x][2] = 0;
            pixelsSamples->pixels[y][x][3] = 0;
          }
        }
      }
    }

    glfwSwapBuffers(window);

    iTime  += 0.0015f;
    iFrame += 1;

    if (1) {
      limitFPS(fps_limiter_target_fps, fps_limiter_frame_start_time);
    }
  }

  vfAllQueuesWaitIdle(ctx, FF, LL);

  // Cleanup
  sycl::free(pixels, sycl_queue);
  sycl::free(pixelsSamples, sycl_queue);

  vfeReBARFreeShared(ctx, &pixelsHandles);

  vfGpuThreadDestroy(ctx, gpu_thread);

  vfContextDeinit(ctx, FF, LL);
  glfwTerminate();
}

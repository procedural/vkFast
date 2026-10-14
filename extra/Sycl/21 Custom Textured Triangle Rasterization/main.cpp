#if 0
# Build and run commands:
source /opt/intel/oneapi/setvars.sh intel64
icx -c ../../../vkfast.c "../../../extra/CPU GPU Array/vkfast_extra_cpu_gpu_array.c" ../../../extra/REII/vkfast_extra_reii.c /home/linuxbrew/RedGpuSDK/redgpu.c /home/linuxbrew/RedGpuSDK/redgpu_2.c /home/linuxbrew/RedGpuSDK/redgpu_32.c -I/home/linuxbrew/.linuxbrew/include/ -I/home/linuxbrew/.linuxbrew/Cellar/xorgproto/2025.1/include/ -I/var/home/linuxbrew/.linuxbrew/Cellar/libxcb/1.17.0/include/
icpx -fsycl -fsycl-targets=spir64 -Xclang -fsycl-allow-func-ptr main.cpp *.o -I/home/linuxbrew/.linuxbrew/include/ -I/home/linuxbrew/.linuxbrew/Cellar/xorgproto/2025.1/include/ -I/var/home/linuxbrew/.linuxbrew/Cellar/libxcb/1.17.0/include/ /home/linuxbrew/.linuxbrew/Cellar/glfw/3.5.1/lib/libglfw3.a /home/linuxbrew/.linuxbrew/lib/libX11.so /home/linuxbrew/.linuxbrew/lib/libvulkan.so -lm -o a.out
XDG_SESSION_TYPE=x11 ./a.out
#endif

// NOTE(Constantine)(Sep 18, 2026): X11/X.h defines None, so we include sycl.hpp before vkFast.
#include <sycl/sycl.hpp>

#include "../../../vkfast.h"
#define VKFAST_EXAMPLES_COMMON_INCLUDE_GLM
#include "../../../examples/Common/vkfast_examples_common.h"

#include <time.h> // For struct timespec

using namespace glm;
typedef unsigned uint;

#define WINDOW_WIDTH  1920
#define WINDOW_HEIGHT 1080

#define TEXTURE0_SIZE_WIDTH  1024
#define TEXTURE0_SIZE_HEIGHT 1024

struct Pixels {
  unsigned char pixels[WINDOW_HEIGHT][WINDOW_WIDTH][4];
};

struct Texture {
  float pixels[TEXTURE0_SIZE_HEIGHT][TEXTURE0_SIZE_WIDTH][4];
};

void imageStore(struct Pixels * screen, ivec2 coord, vec4 color) {
  screen->pixels[coord.y][coord.x][0] = (unsigned char)(sqrt(color.b) * 255.0f);
  screen->pixels[coord.y][coord.x][1] = (unsigned char)(sqrt(color.g) * 255.0f);
  screen->pixels[coord.y][coord.x][2] = (unsigned char)(sqrt(color.r) * 255.0f);
  screen->pixels[coord.y][coord.x][3] = (unsigned char)(sqrt(color.a) * 255.0f);
}

vec4 texelFetch(struct Texture * texture, ivec2 coord, int _) {
  return vec4(
    texture->pixels[coord.y][coord.x][0],
    texture->pixels[coord.y][coord.x][1],
    texture->pixels[coord.y][coord.x][2],
    texture->pixels[coord.y][coord.x][3]
  );
}

#define SWIZ(X) X()

#include "shader.glsl"

#define STB_IMAGE_IMPLEMENTATION
#include "../../../extra/stb_image 2017/stb_image.h"

#ifdef __linux__
#define fopen_s(pFile, filename, mode) (*(pFile) = fopen((filename), (mode)))
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

  gpu_handle_context_t ctx = vfContextInit(1, NULL, FF, LL);
  vfWindowFullscreen(ctx, NULL, "[vkFast] Custom Textured Triangle Rasterization Test", WINDOW_WIDTH, WINDOW_HEIGHT, 0, RED_PRESENT_VSYNC_MODE_ON, FF, LL);

  const unsigned array65536[2] = {65536, 65536};

  gpu_thread_t gpu_thread = NULL;
  vfGpuThreadCreate(ctx, 1, &gpu_thread, NULL, FF, LL);

  sycl::property_list sycl_queue_properties{sycl::property::queue::enable_profiling()};
  sycl::queue sycl_queue(sycl::gpu_selector_v, sycl_queue_properties);

  struct Pixels  * screen   = (struct Pixels *)sycl::malloc_shared(sizeof(struct Pixels), sycl_queue);
  struct Texture * texture0 = (struct Texture *)sycl::malloc_shared(sizeof(struct Texture), sycl_queue);

  // Generate gradient texture0
  for (int y = 0; y < TEXTURE0_SIZE_HEIGHT; y++) {
    for (int x = 0; x < TEXTURE0_SIZE_WIDTH; x++) {
      float r = (float)x / (float)TEXTURE0_SIZE_WIDTH;
      float g = (float)y / (float)TEXTURE0_SIZE_HEIGHT;
      float b = 0.5f;

      texture0->pixels[y][x][0] = r;
      texture0->pixels[y][x][1] = g;
      texture0->pixels[y][x][2] = b;
      texture0->pixels[y][x][3] = 1.0f;
    }
  }

  // Load a texture file to texture0
  if (1) {
    FILE * image_file_fd = NULL;
    fopen_s(&image_file_fd, "texture.png",  "rb");
    REDGPU_2_EXPECTFL(image_file_fd != NULL || !"You need to have a 'texture.png' file with an alpha channel in this example's folder.");
    int image_w = 0;
    int image_h = 0;
    int image_channels_count = 0;
    void * image = stbi_load_from_file(image_file_fd, &image_w, &image_h, &image_channels_count, STBI_rgb_alpha);
    REDGPU_2_EXPECTFL(image != NULL);
    REDGPU_2_EXPECTFL(image_channels_count == 4);

    struct ImageRgba {
      unsigned char pixels[TEXTURE0_SIZE_HEIGHT][TEXTURE0_SIZE_WIDTH][4];
    };

    struct ImageRgba * image_rgba = (struct ImageRgba *)image;

    for (int y = 0; y < TEXTURE0_SIZE_HEIGHT; y++) {
      for (int x = 0; x < TEXTURE0_SIZE_WIDTH; x++) {
        float r = (float)image_rgba->pixels[y][x][0] / 255.f;
        float g = (float)image_rgba->pixels[y][x][1] / 255.f;
        float b = (float)image_rgba->pixels[y][x][2] / 255.f;

        // Gamma correction
        r *= r;
        g *= g;
        b *= b;

        texture0->pixels[y][x][0] = r;
        texture0->pixels[y][x][1] = g;
        texture0->pixels[y][x][2] = b;
        texture0->pixels[y][x][3] = 1.0f;
      }
    }
  }

  vec2  iResolution = {WINDOW_WIDTH, WINDOW_HEIGHT};
  vec2  iMouse      = {0, 0};
  float iTime       = 0;
  float iFrame      = 0;

  struct timespec start, end;

  while (vfWindowLoop(ctx)) {
    clock_gettime(CLOCK_MONOTONIC, &start);

    iTime  += 0.01f;
    iFrame += 1;

    auto syclQueueSubmitEvent = sycl_queue.submit([&](sycl::handler& sycl_cgh) {
      sycl_cgh.parallel_for(sycl::range<2>(WINDOW_HEIGHT, WINDOW_WIDTH), [=](sycl::id<2> sycl_id) {
        int y = sycl_id[0];
        int x = sycl_id[1];

        float xf = (float)x;
        float yf = (float)y;
        vec2 fragCoord = {xf + 0.5f, (WINDOW_HEIGHT-yf) + 0.5f}; // https://registry.khronos.org/OpenGL-Refpages/gl4/html/gl_FragCoord.xhtml

        mainImage(uvec3(x, y, 0), screen, texture0, fragCoord, iResolution, iMouse, iTime, iFrame);
      });
    });

    sycl_queue.wait();

    clock_gettime(CLOCK_MONOTONIC, &end);

    if (1) {
      auto end   = syclQueueSubmitEvent.get_profiling_info<sycl::info::event_profiling::command_end>();
      auto start = syclQueueSubmitEvent.get_profiling_info<sycl::info::event_profiling::command_start>();

      std::cout << "Sycl queue submit elapsed time: " << (end - start) << " nanoseconds.\n";
    }

    gpu_thread_t gpu_threads[2] = {gpu_thread, 0};
    vfDrawPixels(ctx, screen, NULL, 2, gpu_threads, array65536, FF, LL);

    long long nanoseconds = (end.tv_sec - start.tv_sec) * 1000000000LL + (end.tv_nsec - start.tv_nsec);
    double microseconds = (double)nanoseconds / 1000.0;
    double milliseconds = (double)nanoseconds / 1000000.0;
    double fps = 1000.0 / milliseconds;
    printf("Triangle render time: %f ms (%f fps)\n", milliseconds, fps);
  }

  vfAllQueuesWaitIdle(ctx, FF, LL);

  // Cleanup
  sycl::free(screen, sycl_queue);
  sycl::free(texture0, sycl_queue);

  vfGpuThreadDestroy(ctx, gpu_thread);

  vfContextDeinit(ctx, FF, LL);
}

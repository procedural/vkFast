#define GLM_FORCE_SWIZZLE

#include "../../vkfast.h"
#define VKFAST_EXAMPLES_COMMON_INCLUDE_GLM
#include "../Common/vkfast_examples_common.h"

#include <time.h> // For struct timespec

using namespace glm;
typedef unsigned uint;

#define WINDOW_WIDTH  1920
#define WINDOW_HEIGHT 1080

#define TEXTURE0_SIZE_WIDTH  512
#define TEXTURE0_SIZE_HEIGHT 512

unsigned char screen[WINDOW_HEIGHT][WINDOW_WIDTH][4] = {};
float texture0[TEXTURE0_SIZE_WIDTH][TEXTURE0_SIZE_HEIGHT][4] = {};

float iTime = 0.0f;

uvec2 iResolution           = {WINDOW_WIDTH, WINDOW_HEIGHT};
ivec2 iChannelResolution[1] = {{TEXTURE0_SIZE_WIDTH, TEXTURE0_SIZE_HEIGHT}};

void imageStore(void * _, ivec2 coord, vec4 color) {
  screen[coord.y][coord.x][0] = (unsigned char)(sqrt(color.b) * 255.0f);
  screen[coord.y][coord.x][1] = (unsigned char)(sqrt(color.g) * 255.0f);
  screen[coord.y][coord.x][2] = (unsigned char)(sqrt(color.r) * 255.0f);
  screen[coord.y][coord.x][3] = (unsigned char)(sqrt(color.a) * 255.0f);
}

vec4 texelFetch(void * _, ivec2 coord, int __) {
  return vec4(
    texture0[coord.y][coord.x][0],
    texture0[coord.y][coord.x][1],
    texture0[coord.y][coord.x][2],
    texture0[coord.y][coord.x][3]
  );
}

#define SWIZ(X) X()

#include "shader.glsl"

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

  // Generate gradient texture0
  for (int y = 0; y < TEXTURE0_SIZE_HEIGHT; y++) {
    for (int x = 0; x < TEXTURE0_SIZE_WIDTH; x++) {
      float r = (float)x / (float)TEXTURE0_SIZE_WIDTH;
      float g = (float)y / (float)TEXTURE0_SIZE_HEIGHT;
      float b = 0.5f;

      texture0[y][x][0] = r;
      texture0[y][x][1] = g;
      texture0[y][x][2] = b;
      texture0[y][x][3] = 1.0f;
    }
  }

  struct timespec start, end;

  while (vfWindowLoop(ctx)) {
    clock_gettime(CLOCK_MONOTONIC, &start);

    // NOTE(Constantine): Enable "Project properties -> C/C++ -> Language -> Open MP Support"
    #pragma omp parallel for
    for (int y = 0; y < WINDOW_HEIGHT; y += 1) {
      #pragma omp parallel for
      for (int x = 0; x < WINDOW_WIDTH; x += 1) {
        float xf = (float)x;
        float yf = (float)y;
        vec2 fragCoord = {xf + 0.5f, (WINDOW_HEIGHT-yf) + 0.5f}; // https://registry.khronos.org/OpenGL-Refpages/gl4/html/gl_FragCoord.xhtml

        mainImage(uvec3(x, y, 0), fragCoord);
      }
    }

    clock_gettime(CLOCK_MONOTONIC, &end);

    gpu_thread_t gpu_threads[2] = {gpu_thread, 0};
    vfDrawPixels(ctx, screen, NULL, 2, gpu_threads, array65536, FF, LL);

    iTime += 0.1f;

    long long nanoseconds = (end.tv_sec - start.tv_sec) * 1000000000LL + (end.tv_nsec - start.tv_nsec);
    double microseconds = (double)nanoseconds / 1000.0;
    double milliseconds = (double)nanoseconds / 1000000.0;
    double fps = 1000.0 / milliseconds;
    printf("Triangle render time: %f ms (%f fps)\n", milliseconds, fps);
  }

  vfAllQueuesWaitIdle(ctx, FF, LL);

  vfGpuThreadDestroy(ctx, gpu_thread);

  vfContextDeinit(ctx, FF, LL);
}

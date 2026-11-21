#if 0
set -x

if [[ "$ARC_GLFW3" == "" ]] && [[ "$ARC_SDL3" == "" ]]; then
  ARC_GLFW3='1'
fi

if [[ "$ARC_GLFW3" == "1" ]]; then
  ARC_GLFW3_LIB='/home/linuxbrew/.linuxbrew/Cellar/glfw/3.5.1/lib/libglfw3.a'
fi

if [[ "$ARC_SDL3" == "1" ]]; then
  ARC_SDL3_DEF='-DVKFAST_EXAMPLE_01_ENABLE_GLFW3_TO_SDL3'
  ARC_SDL3_LIB='/home/linuxbrew/.linuxbrew/lib/libSDL3.so'
fi

gcc $@ $ARC_SDL3_DEF main.c ../../vkfast.c /home/linuxbrew/RedGpuSDK/redgpu.c /home/linuxbrew/RedGpuSDK/redgpu_2.c /home/linuxbrew/RedGpuSDK/redgpu_32.c -I/home/linuxbrew/.linuxbrew/include/ -I/home/linuxbrew/.linuxbrew/Cellar/xorgproto/2025.1/include/ -I/var/home/linuxbrew/.linuxbrew/Cellar/libxcb/1.17.0/include/ $ARC_GLFW3_LIB $ARC_SDL3_LIB /home/linuxbrew/.linuxbrew/lib/libX11.so /home/linuxbrew/.linuxbrew/lib/libvulkan.so -lm

exit
#endif

// NOTE(Constantine):
// Useful Termux links:
// https://github.com/brunodev85/winlator
// https://github.com/The412Banner/AIO-Graphics-Test

#include "../../vkfast.h"
#include "../../extra/vkFast Extensions/ReBAR/vkfast_ext_rebar.h"
#if !defined(VKFAST_EXAMPLE_01_ENABLE_GLFW3_TO_SDL3)
  #define VKFAST_EXAMPLES_COMMON_INCLUDE_GLFW3
#endif
#include "../Common/vkfast_examples_common.h"
#if defined(VKFAST_EXAMPLE_01_ENABLE_GLFW3_TO_SDL3)
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
  GLFWwindow * window = glfwCreateWindow(700, 700, "[vkFast] GLFW Test", NULL, NULL);
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
  #ifdef VKFAST_EXTRA_INCLUDED_GLFW3_TO_SDL3
  windowData.display = glfwGetX11Display(window);
  #else
  windowData.display = glfwGetX11Display();
  #endif
  windowData.window = glfwGetX11Window(window);
  windowData.wmDeleteMessage = 0;
  REDGPU_2_EXPECTFL(windowData.display != NULL || !"On Wayland, you need to run the app like this: XDG_SESSION_TYPE=x11 ./a.out");
  REDGPU_2_EXPECTFL(windowData.window  != 0    || !"On Wayland, you need to run the app like this: XDG_SESSION_TYPE=x11 ./a.out");
  void * window_handle = &windowData;
#endif

  gpu_handle_context_t ctx = vfContextInit(1, NULL, FF, LL);
  //REDGPU_2_EXPECTFL(vfeReBARGetMaxMallocSharedSize(ctx) >= (4ULL * 1024*1024*1024 - 1));
  vfWindowFullscreen(ctx, window_handle, "[vkFast] GLFW Test", 700, 700, 0, RED_PRESENT_VSYNC_MODE_ON, FF, LL);

  const unsigned array65536[2] = {65536, 65536};

  gpu_thread_t gpu_thread = NULL;
  vfGpuThreadCreate(ctx, 1, &gpu_thread, NULL, FF, LL);

  struct float4 {
    float x, y, z, w;
  };

  VfeReBARMallocShared storageInputSharedDataHandles = {0};
  struct float4 * storage_input = (struct float4 *)vfeReBARMallocShared(ctx, 2 * sizeof(struct float4), &storageInputSharedDataHandles);

  storage_input[0].x = 4;
  storage_input[0].y = 8;
  storage_input[0].z = 15;
  storage_input[0].w = 16;

  storage_input[1].x = 16;
  storage_input[1].y = 23;
  storage_input[1].z = 42;
  storage_input[1].w = 108;

  VfeReBARMallocShared storageOutputSharedDataHandles = {0};
  struct float4 * storage_output = (struct float4 *)vfeReBARMallocShared(ctx, 1 * sizeof(struct float4), &storageOutputSharedDataHandles);

  #include "add.cs.h"
  gpu_program_info_t cs_info = {0};
  cs_info.program_binary_bytes_count = sizeof(g_main);
  cs_info.program_binary             = g_main;
  uint64_t cs = vfProgramCreateFromBinaryCompute(ctx, &cs_info, FF, LL);

  RedStructDeclarationMember slots[2] = {0};
  slots[0].slot            = 0;
  slots[0].type            = RED_STRUCT_MEMBER_TYPE_ARRAY_RO_RW;
  slots[0].count           = 1;
  slots[0].visibleToStages = RED_VISIBLE_TO_STAGE_BITFLAG_COMPUTE;

  slots[1].slot            = 1;
  slots[1].type            = RED_STRUCT_MEMBER_TYPE_ARRAY_RO_RW;
  slots[1].count           = 1;
  slots[1].visibleToStages = RED_VISIBLE_TO_STAGE_BITFLAG_COMPUTE;
  gpu_program_pipeline_info_t pp_info = {0};
  pp_info.variables_slot        = 2;
  pp_info.variables_bytes_count = 1 * 4*sizeof(float);
  pp_info.struct_members_count  = countof(slots);
  pp_info.struct_members        = slots;
  uint64_t pp = vfProgramPipelineCreateCompute(ctx, cs, &pp_info, NULL, FF, LL);

  uint64_t batch = 0;

  while (glfwWindowShouldClose(window) == 0) {
    #ifdef VKFAST_EXTRA_INCLUDED_GLFW3_TO_SDL3
    glfwPollEvents(1, &window);
    #else
    glfwPollEvents();
    #endif

    int os_window_w = 0;
    int os_window_h = 0;
    glfwGetWindowSize(window, &os_window_w, &os_window_h);

    if (vfWindowIsMinimized(ctx) || os_window_w == 0 || os_window_h == 0) {
      continue;
    }

    gpu_batch_info_t bindings_info = {0};
    bindings_info.max_new_bindings_sets_count = 1;
    bindings_info.max_storage_binds_count     = 2;
    batch = vfBatchBegin(ctx, batch, &bindings_info, NULL, FF, LL);
    vfBatchBindProgramPipelineCompute(ctx, batch, pp, FF, LL);
    vfBatchBindNewBindingsSet(ctx, batch, countof(slots), slots, FF, LL);
    vfBatchBindStorageRaw(ctx, batch, 0, 1, &storageInputSharedDataHandles.storageRaw, FF, LL);
    vfBatchBindStorageRaw(ctx, batch, 1, 1, &storageOutputSharedDataHandles.storageRaw, FF, LL);
    vfBatchBindNewBindingsEnd(ctx, batch, FF, LL);
    float salt[4] = {0};
    salt[0] = 0;
    salt[1] = -1;
    salt[2] = -7;
    salt[3] = 6;
    vfBatchBindVariablesCopy(ctx, batch, 0, sizeof(salt), salt, FF, LL);
    vfBatchCompute(ctx, batch, 1, 1, 1, FF, LL);
    vfBatchBarrierCpuReadback(ctx, batch, FF, LL);
    vfBatchEnd(ctx, batch, FF, LL);

    RedHandleCalls batchRaw = vfBatchGetRawHandle(ctx, batch, FF, LL);
    uint64_t wait = vfAsyncBatchExecuteRaw(ctx, 1, &batchRaw, 1, &gpu_thread, array65536, FF, LL);
    vfAsyncWaitToFinish(ctx, wait, FF, LL);

    // NOTE(Constantine): Expected result: 20 30 50 130 (20 31 57 124 + salt)"
    printf("Result: %f %f %f %f\n",
      storage_output[0].x,
      storage_output[0].y,
      storage_output[0].z,
      storage_output[0].w
    );
    REDGPU_2_EXPECTFL(storage_output[0].x == 20);
    REDGPU_2_EXPECTFL(storage_output[0].y == 30);
    REDGPU_2_EXPECTFL(storage_output[0].z == 50);
    REDGPU_2_EXPECTFL(storage_output[0].w == 130);

    // Now let's draw pixels

    int window_w = 0;
    int window_h = 0;
    vfWindowGetSize(ctx, &window_w, &window_h);

    // To free
    VfeReBARMallocShared pixelsHandles = {0};
    unsigned char * pixels = (unsigned char *)vfeReBARMallocShared(ctx, 4 * window_h * window_w, &pixelsHandles);
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
        // NOTE(Constantine):
        // The automatic DPI scaling is disabled with SetProcessDpiAwareness(PROCESS_PER_MONITOR_DPI_AWARE), but
        // with DPI scaling, 1920x1080 screen resolution with a desktop scaling of, say, 1.25 is equal to 1536x864 (draw coords: 1535x863).
        if (y == 0) {
          pixels[y * window_w * 4 + x * 4 + 0] = 255;
          pixels[y * window_w * 4 + x * 4 + 1] = 0;
          pixels[y * window_w * 4 + x * 4 + 2] = 0;
          pixels[y * window_w * 4 + x * 4 + 3] = 255;
        } else if (x == 0) {
          pixels[y * window_w * 4 + x * 4 + 0] = 0;
          pixels[y * window_w * 4 + x * 4 + 1] = 255;
          pixels[y * window_w * 4 + x * 4 + 2] = 0;
          pixels[y * window_w * 4 + x * 4 + 3] = 255;
        } else if (y == mouse_y) {
          pixels[y * window_w * 4 + x * 4 + 0] = 0;
          pixels[y * window_w * 4 + x * 4 + 1] = 0;
          pixels[y * window_w * 4 + x * 4 + 2] = 255;
          pixels[y * window_w * 4 + x * 4 + 3] = 255;
        } else if (x == mouse_x) {
          pixels[y * window_w * 4 + x * 4 + 0] = 255;
          pixels[y * window_w * 4 + x * 4 + 1] = 255;
          pixels[y * window_w * 4 + x * 4 + 2] = 255;
          pixels[y * window_w * 4 + x * 4 + 3] = 255;
        }
      }
    }

    gpu_thread_t gpu_threads[2] = {gpu_thread, 0};
    vfAsyncDrawPixelsRaw(ctx, &pixelsHandles.storageRaw, NULL, 2, gpu_threads, array65536, FF, LL);

    vfeReBARFreeShared(ctx, &pixelsHandles);
    pixels = NULL;
    pixelsHandles = (VfeReBARMallocShared){0};
  }

  vfAllQueuesWaitIdle(ctx, FF, LL);

  vfGpuThreadDestroy(ctx, gpu_thread);

  vfeReBARFreeShared(ctx, &storageOutputSharedDataHandles);
  vfeReBARFreeShared(ctx, &storageInputSharedDataHandles);

  uint64_t ids[] = {
    cs,
    pp,
    batch,
  };
  vfIdDestroy(countof(ids), ids, FF, LL);
  vfContextDeinit(ctx, FF, LL);
  #ifdef VKFAST_EXTRA_INCLUDED_GLFW3_TO_SDL3
  glfwTerminateWindow(window);
  #endif
  glfwTerminate();
  #if defined(VKFAST_INCLUDE_TERMUX_PATHS)
  system("pause");
  #endif
}

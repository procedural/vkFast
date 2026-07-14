//\\rc rawbuild begin gcc-linux-64-bit
//\\rc rawbuild require debug,release,release-fast
//\\rc rawbuild require glfw3,sdl3
//\\rc rawbuild `gcc`
//\\rc rawbuild debug ` -g -O0`
//\\rc rawbuild release,release-fast ` -O2`
//\\rc rawbuild sdl3 ` -DVKFAST_EXAMPLE_01_ENABLE_GLFW3_TO_SDL3`
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

//\\rc rawbuild begin mingw-clang-termux-64-bit
//\\rc rawbuild `x86_64-w64-mingw32-clang -DVKFAST_INCLUDE_TERMUX_PATHS main.c ../../vkfast.c /data/data/com.termux/files/home/RedGpuSDK/redgpu.c /data/data/com.termux/files/home/RedGpuSDK/redgpu_2.c /data/data/com.termux/files/home/RedGpuSDK/redgpu_32.c ../Common/glfw-3.4.bin.WIN64/lib-mingw-w64/libglfw3.a -luser32 -lshell32 -lgdi32`
//\\rc rawbuild end

#include "../../vkfast.h"
#ifndef VKFAST_EXAMPLE_01_ENABLE_GLFW3_TO_SDL3
#define VKFAST_EXAMPLES_COMMON_INCLUDE_GLFW3
#endif
#include "../Common/vkfast_examples_common.h"
#ifdef VKFAST_EXAMPLE_01_ENABLE_GLFW3_TO_SDL3
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
  vfWindowFullscreen(ctx, window_handle, "[vkFast] GLFW Test", 700, 700, 0, RED_PRESENT_VSYNC_MODE_ON, FF, LL);

  const unsigned array65536[2] = {65536, 65536};

  gpu_thread_t gpu_thread = NULL;
  vfGpuThreadCreate(ctx, 1, &gpu_thread, NULL, FF, LL);

  gpu_storage_info_t storage_info = {0};
  storage_info.storage_type = GPU_STORAGE_TYPE_CPU_UPLOAD;
  storage_info.bytes_count  = 2 * 4*sizeof(float);
  gpu_storage_t storage_input_cpu = {0};
  vfStorageCreate(ctx, &storage_info, &storage_input_cpu, FF, LL);
  storage_info.storage_type = GPU_STORAGE_TYPE_GPU_ONLY;
  gpu_storage_t storage_input_gpu = {0};
  vfStorageCreate(ctx, &storage_info, &storage_input_gpu, FF, LL);
  
  storage_input_cpu.as_vec4[0].x = 4;
  storage_input_cpu.as_vec4[0].y = 8;
  storage_input_cpu.as_vec4[0].z = 15;
  storage_input_cpu.as_vec4[0].w = 16;
  
  storage_input_cpu.as_vec4[1].x = 16;
  storage_input_cpu.as_vec4[1].y = 23;
  storage_input_cpu.as_vec4[1].z = 42;
  storage_input_cpu.as_vec4[1].w = 108;
    
  uint64_t copy = vfBatchBegin(ctx, 0, NULL, NULL, FF, LL);
  vfBatchStorageCopyFromCpuToGpu(ctx, copy, storage_input_cpu.id, storage_input_gpu.id, FF, LL);
  vfBatchEnd(ctx, copy, FF, LL);
  RedHandleCalls copyRaw = vfBatchGetRawHandle(ctx, copy, FF, LL);
  uint64_t async = vfAsyncBatchExecuteRaw(ctx, 1, &copyRaw, 1, &gpu_thread, array65536, FF, LL);
  vfAsyncWaitToFinish(ctx, async, FF, LL);

  gpu_storage_info_t storage_output_info = {0};
  storage_output_info.storage_type = GPU_STORAGE_TYPE_CPU_READBACK;
  storage_output_info.bytes_count  = 1 * 4*sizeof(float);
  gpu_storage_t storage_output_cpu = {0};
  vfStorageCreate(ctx, &storage_output_info, &storage_output_cpu, FF, LL);
  storage_output_info.storage_type = GPU_STORAGE_TYPE_GPU_ONLY;
  gpu_storage_t storage_output_gpu = {0};
  vfStorageCreate(ctx, &storage_output_info, &storage_output_gpu, FF, LL);

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
  gpu_program_pipeline_compute_info_t pp_info = {0};
  pp_info.compute_program       = cs;
  pp_info.variables_slot        = 2;
  pp_info.variables_bytes_count = 1 * 4*sizeof(float);
  pp_info.struct_members_count  = countof(slots);
  pp_info.struct_members        = slots;
  uint64_t pp = vfProgramPipelineCreateCompute(ctx, &pp_info, FF, LL);
  
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
    vfBatchBindStorageSingle(ctx, batch, 0, storage_input_gpu.id, FF, LL);
    vfBatchBindStorageSingle(ctx, batch, 1, storage_output_gpu.id, FF, LL);
    vfBatchBindNewBindingsEnd(ctx, batch, FF, LL);
    float salt[4] = {0};
    salt[0] = 0;
    salt[1] = -1;
    salt[2] = -7;
    salt[3] = 6;
    vfBatchBindVariablesCopy(ctx, batch, 0, sizeof(salt), salt, FF, LL);
    vfBatchCompute(ctx, batch, 1, 1, 1, FF, LL);
    vfBatchBarrierMemory(ctx, batch, FF, LL);
    vfBatchStorageCopyFromGpuToCpu(ctx, batch, storage_output_gpu.id, storage_output_cpu.id, FF, LL);
    vfBatchBarrierCpuReadback(ctx, batch, FF, LL);
    vfBatchEnd(ctx, batch, FF, LL);

    RedHandleCalls batchRaw = vfBatchGetRawHandle(ctx, batch, FF, LL);
    uint64_t wait = vfAsyncBatchExecuteRaw(ctx, 1, &batchRaw, 1, &gpu_thread, array65536, FF, LL);
    vfAsyncWaitToFinish(ctx, wait, FF, LL);

    // NOTE(Constantine): Expected result: 20 30 50 130 (20 31 57 124 + salt)"
    printf("Result: %f %f %f %f\n",
      storage_output_cpu.as_vec4[0].x,
      storage_output_cpu.as_vec4[0].y,
      storage_output_cpu.as_vec4[0].z,
      storage_output_cpu.as_vec4[0].w
    );
    REDGPU_2_EXPECTFL(storage_output_cpu.as_vec4[0].x == 20);
    REDGPU_2_EXPECTFL(storage_output_cpu.as_vec4[0].y == 30);
    REDGPU_2_EXPECTFL(storage_output_cpu.as_vec4[0].z == 50);
    REDGPU_2_EXPECTFL(storage_output_cpu.as_vec4[0].w == 130);

    // Now let's draw pixels

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
    vfDrawPixels(ctx, pixels, NULL, 2, gpu_threads, array65536, FF, LL);

    red32MemoryFree(pixels);
    pixels = NULL;
  }

  vfAllQueuesWaitIdle(ctx, FF, LL);

  vfGpuThreadDestroy(ctx, gpu_thread);
  uint64_t ids[] = {
    storage_input_cpu.id,
    storage_input_gpu.id,
    copy,
    storage_output_cpu.id,
    storage_output_gpu.id,
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
}

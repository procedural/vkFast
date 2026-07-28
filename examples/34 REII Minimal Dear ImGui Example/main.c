//\\rc rawbuild begin gcc-linux-64-bit
//\\rc rawbuild require-config debug,release,release-fast
//\\rc rawbuild `g++ -shared -fPIC -fvisibility=hidden`
//\\rc rawbuild debug ` -g -O0`
//\\rc rawbuild release,release-fast ` -O2`
//\\rc rawbuild ` "../../extra/Dear ImGui 2016/imgui_megafile.cpp" -o libimgui.so`
//\\rc rawbuild next_command
//\\rc rawbuild `gcc`
//\\rc rawbuild debug ` -g -O0`
//\\rc rawbuild release,release-fast ` -O2`
//\\rc rawbuild ` main.c ../Common/font_droid_sans_mono.c ../../vkfast.c ../../extra/Banzai/vkfast_extra_banzai.c ../../extra/Banzai/vkfast_extra_banzai_pointer.c "../../extra/CPU GPU Array/vkfast_extra_cpu_gpu_array.c" ../../extra/REII/vkfast_extra_reii.c /home/linuxbrew/RedGpuSDK/redgpu.c /home/linuxbrew/RedGpuSDK/redgpu_2.c /home/linuxbrew/RedGpuSDK/redgpu_32.c -I/home/linuxbrew/.linuxbrew/include/ -I/home/linuxbrew/.linuxbrew/Cellar/xorgproto/2025.1/include/ -I/var/home/linuxbrew/.linuxbrew/Cellar/libxcb/1.17.0/include/ /home/linuxbrew/.linuxbrew/Cellar/glfw/3.4/lib/libglfw3.a /home/linuxbrew/.linuxbrew/lib/libX11.so /home/linuxbrew/.linuxbrew/lib/libvulkan.so libimgui.so -lm`
//\\rc rawbuild end

#include "../../vkfast_ex.h"
#include "../../extra/Banzai/vkfast_extra_banzai_pointer.h"
#include "../../extra/REII/vkfast_extra_reii.h"
#define VKFAST_EXAMPLES_COMMON_INCLUDE_GLFW3
#define VKFAST_EXAMPLES_COMMON_INCLUDE_EXTRA_BANZAI
#include "../Common/vkfast_examples_common.h"
// NOTE(Constantine): Dear ImGui 2016 needs GLFW.
#include "../../extra/Dear ImGui 2016/imgui_reii.h"

extern size_t        gFontDroidSansMonoFontGetBytesCount();
extern unsigned char gFontDroidSansMonoFont[];

int main() {
#if defined(__MINGW32__)
  SetProcessDPIAware();
#elif defined(_WIN32)
  SetProcessDpiAwareness(PROCESS_PER_MONITOR_DPI_AWARE);
#endif

#if defined(_MSC_VER) && defined(_DEBUG)
  _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

  int window_w = 700;
  int window_h = 700;

  glfwInit();
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  GLFWwindow * window = glfwCreateWindow(window_w, window_h, "[vkFast] REII Minimal Dear ImGui Example", 0, 0);
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

  gpu_internal_memory_allocation_sizes_t memory_allocation_sizes = {0};
  memory_allocation_sizes.bytes_count_for_memory_storages_type_gpu_only         = (1024/*mb*/ * 1024 * 1024) - 64; // NOTE(Constantine)(Mar 20, 2026): '- 64' added for Intel iGPUs which can allocate not 1073741824, but 1073741820 max, lol.
  memory_allocation_sizes.bytes_count_for_memory_storages_type_cpu_upload       = (512/*mb*/ * 1024 * 1024);
  memory_allocation_sizes.bytes_count_for_memory_storages_type_cpu_readback     = (512/*mb*/ * 1024 * 1024);
  memory_allocation_sizes.bytes_count_for_memory_present_pixels_type_cpu_upload = 0;
  gpu_context_optional_parameters_t optional_parameters = {0};
  optional_parameters.internal_memory_allocation_sizes = &memory_allocation_sizes;

  const int doDoubleGammaCorrection = 0;
  // NOTE(Constantine): You can also define REDGPU_COMPILE_SWITCH_DEBUG to see extra errors.
  gpu_handle_context_t ctx = vfContextInit(1, &optional_parameters, FF, LL);
  vfWindowFullscreen(ctx, window_handle, "[vkFast] REII Minimal Dear ImGui Example", window_w, window_h, 0, RED_PRESENT_VSYNC_MODE_ON, FF, LL);

  const unsigned array65536[2] = {65536, 65536};

  gpu_thread_t gpu_thread = NULL;
  vfGpuThreadCreate(ctx, 1, &gpu_thread, NULL, FF, LL);

  gpu_storage_t storage_gpu_only     = {0};
  gpu_storage_t storage_cpu_upload   = {0};
  gpu_storage_t storage_cpu_readback = {0};
  vfeBanzaiStoragesCreate(ctx, &storage_gpu_only, &storage_cpu_upload, &storage_cpu_readback, FF, LL);

  uint64_t storage_gpu_only_mem_offset = 0;
  uint64_t storage_cpu_upload_mem_offset = 0;
  uint64_t storage_cpu_readback_mem_offset = 0;

  ReiiHandleTextureMemory outputDSTexMemory = {0};
  reiiCreateTextureMemory(ctx, GPU_EXTRA_REII_TEXTURE_TYPE_OUTPUT_DEPTH_STENCIL_MSAA, (288/*mb*/ * 1024 * 1024), &outputDSTexMemory);
  ReiiHandleTexture houtputdstex = {0};
  ReiiHandleTexture * outputdstex = &houtputdstex;
  reiiCreateTextureFromTextureMemory(ctx, &outputDSTexMemory, REII_TEXTURE_BINDING_2D, &houtputdstex);
  reiiTextureSetStateMsaa(ctx, REII_TEXTURE_BINDING_2D, outputdstex, 4);
  reiiTextureSetStateMipmap(ctx, REII_TEXTURE_BINDING_2D, outputdstex, 0);
  reiiTextureSetStateMipmapLevelsCount(ctx, REII_TEXTURE_BINDING_2D, outputdstex, 1);
  reiiTextureDefineAndCopyFromCpu(ctx, REII_TEXTURE_BINDING_2D, outputdstex, 0, REII_TEXTURE_TEXEL_FORMAT_DS, window_w, window_h, REII_TEXTURE_TEXEL_FORMAT_DS, REII_TEXTURE_TEXEL_TYPE_FLOAT, 4, NULL, 1, &gpu_thread, array65536);

  ReiiHandleTextureMemory outputTexMemory = {0};
  reiiCreateTextureMemory(ctx, GPU_EXTRA_REII_TEXTURE_TYPE_OUTPUT_COLOR, (288/*mb*/ * 1024 * 1024), &outputTexMemory);
  ReiiHandleTexture houtputtex = {0};
  ReiiHandleTexture * outputtex = &houtputtex;
  reiiCreateTextureFromTextureMemory(ctx, &outputTexMemory, REII_TEXTURE_BINDING_2D, &houtputtex);
  reiiTextureSetStateMipmap(ctx, REII_TEXTURE_BINDING_2D, outputtex, 0);
  reiiTextureSetStateMipmapLevelsCount(ctx, REII_TEXTURE_BINDING_2D, outputtex, 1);
  reiiTextureDefineAndCopyFromCpu(ctx, REII_TEXTURE_BINDING_2D, outputtex, 0, REII_TEXTURE_TEXEL_FORMAT_RGBA, window_w, window_h, REII_TEXTURE_TEXEL_FORMAT_RGBA, REII_TEXTURE_TEXEL_TYPE_U8, 4, NULL, 1, &gpu_thread, array65536);

  ReiiHandleTextureMemory outputMSTexMemory = {0};
  reiiCreateTextureMemory(ctx, GPU_EXTRA_REII_TEXTURE_TYPE_OUTPUT_COLOR_MSAA, (288/*mb*/ * 1024 * 1024), &outputMSTexMemory);
  ReiiHandleTexture houtputmstex = {0};
  ReiiHandleTexture * outputmstex = &houtputmstex;
  reiiCreateTextureFromTextureMemory(ctx, &outputMSTexMemory, REII_TEXTURE_BINDING_2D, &houtputmstex);
  reiiTextureSetStateMsaa(ctx, REII_TEXTURE_BINDING_2D, outputmstex, 4);
  reiiTextureSetStateMipmap(ctx, REII_TEXTURE_BINDING_2D, outputmstex, 0);
  reiiTextureSetStateMipmapLevelsCount(ctx, REII_TEXTURE_BINDING_2D, outputmstex, 1);
  reiiTextureDefineAndCopyFromCpu(ctx, REII_TEXTURE_BINDING_2D, outputmstex, 0, REII_TEXTURE_TEXEL_FORMAT_RGBA, window_w, window_h, REII_TEXTURE_TEXEL_FORMAT_RGBA, REII_TEXTURE_TEXEL_TYPE_U8, 4, NULL, 1, &gpu_thread, array65536);

  // Dear ImGui 2016
  ReiiHandleTextureMemory imgui_fontAtlasMemory = {0};
  reiiCreateTextureMemory(ctx, GPU_EXTRA_REII_TEXTURE_TYPE_GENERAL, (64/*mb*/ * 1024 * 1024), &imgui_fontAtlasMemory);

  ReiiCpuScratchBuffer imgui_fontAtlas_upload_scratch_buffer = OffsetAllocateCpuScratchBuffer(
    128/*mb*/ * 1024 * 1024,
    &storage_cpu_upload, &storage_cpu_upload_mem_offset,
    FF, LL
  );

  gpu_extra_cpu_gpu_array imgui_dynamicMeshPosition = OffsetAllocateCpuGpuArrayWithTale64BytesAlign(
    32/*mb*/ * 1024 * 1024,
    &storage_cpu_upload, &storage_cpu_upload_mem_offset,
    &storage_gpu_only,   &storage_gpu_only_mem_offset,
    FF, LL
  );

  gpu_extra_cpu_gpu_array imgui_dynamicMeshColor = OffsetAllocateCpuGpuArrayWithTale64BytesAlign(
    32/*mb*/ * 1024 * 1024,
    &storage_cpu_upload, &storage_cpu_upload_mem_offset,
    &storage_gpu_only,   &storage_gpu_only_mem_offset,
    FF, LL
  );

  Red2Output imgui_mutableOutputsArray[100] = {0}; // NOTE(Constantine): Maybe need more?
  imguiInit(
    window, // GLFWwindow * window
    ctx, // gpu_handle_context_t context
    imgui_fontAtlasMemory, // ReiiHandleTextureMemory fontAtlasMemory
    imgui_fontAtlas_upload_scratch_buffer, // ReiiCpuScratchBuffer fontAtlasScratchBuffer
    100, // uint64_t maxNewBindingsSetsCount
    imgui_dynamicMeshPosition, // gpu_extra_cpu_gpu_array dynamicMeshPosition
    imgui_dynamicMeshColor, // gpu_extra_cpu_gpu_array dynamicMeshColor
    100, // uint64_t mutableOutputsArrayMaxCapacity
    imgui_mutableOutputsArray, // Red2Output * mutableOutputsArray
    outputtex, // ReiiHandleTexture * outputTexture
    gpu_thread, // gpu_thread_t gpuThread
    0, // unsigned optionalQueueFamilyIndex
    NULL // RedHandleQueue optionalQueue
  );

  ImguiIO    * io    = (ImguiIO *)igGetIO();
  ImguiStyle * style = (ImguiStyle *)igGetStyle();

  style->scrollbarRounding = 0;
  style->windowRounding    = 0;
  style->frameRounding     = 0;

  float fontPixelSize = 22;
  size_t fontDataBytesCount = gFontDroidSansMonoFontGetBytesCount();
  char * fontData = (char *)malloc(fontDataBytesCount);
  REDGPU_2_EXPECTFL(fontData != NULL);
  memcpy(fontData, gFontDroidSansMonoFont, fontDataBytesCount);
  struct ImFont * imfont = ImFontAtlas_AddFontFromMemoryTTF(io->fonts, fontData, fontDataBytesCount, fontPixelSize, NULL, ImFontAtlas_GetGlyphRangesCyrillic(io->fonts));
  // free(fontData); // NOTE(Constantine): Commented out intentionally, "ownership of font_data is transfered by Dear ImGui by default".
  imguiInvalidateFontTexture();
  imguiCreateFontTexture();
  REDGPU_2_EXPECTFL(ImFontAtlas_GetFontsCount(io->fonts) == 2);
  ImFontAtlas_SetFontAsDefault(io->fonts, 1);

  uint64_t batch = 0;
  ReiiHandleCommandList hlist = {0};
  ReiiHandleCommandList * list = &hlist;
  Red2Output mutable_outputs_array[3]  = {0};
  list->mutable_outputs_array.items    = mutable_outputs_array;
  list->mutable_outputs_array.capacity = countof(mutable_outputs_array);

  ReiiGammaCorrectColorTextureToTheInversePowerOf2StaticState gammaCorrectionStaticState = {0};

  int previous_window_w = window_w;
  int previous_window_h = window_h;

  while (glfwWindowShouldClose(window) == 0) {
    glfwPollEvents();

    int os_window_w = 0;
    int os_window_h = 0;
    glfwGetWindowSize(window, &os_window_w, &os_window_h);

    if (vfWindowIsMinimized(ctx) || os_window_w == 0 || os_window_h == 0) {
      continue;
    }

    {
      // Font resizing with Left Ctrl + '-' and Left Ctrl + '+' logic.
      ReiiBool32 fontPixelSizeChanged = 0;
      if (igIsKeyDown(GLFW_KEY_LEFT_CONTROL) && igIsKeyPressed(GLFW_KEY_EQUAL, 0)) {
        fontPixelSizeChanged = 1;
        fontPixelSize += 2;
        if (fontPixelSize > 100) { fontPixelSize = 100; }
      }
      if (igIsKeyDown(GLFW_KEY_LEFT_CONTROL) && igIsKeyPressed(GLFW_KEY_MINUS, 0)) {
        fontPixelSizeChanged = 1;
        fontPixelSize -= 2;
        if (fontPixelSize < 10) { fontPixelSize = 10; }
      }
      if (fontPixelSizeChanged == 1) {
        fontPixelSizeChanged = 0;
        ImFontAtlas_Clear(io->fonts);
        imguiInvalidateFontTexture();
        ImFontAtlas_AddFontDefault(io->fonts, NULL);
        fontData = (char *)malloc(fontDataBytesCount);
        REDGPU_2_EXPECTFL(fontData != NULL);
        memcpy(fontData, gFontDroidSansMonoFont, fontDataBytesCount);
        imfont = ImFontAtlas_AddFontFromMemoryTTF(io->fonts, fontData, fontDataBytesCount, fontPixelSize, NULL, ImFontAtlas_GetGlyphRangesCyrillic(io->fonts));
        imguiCreateFontTexture();
        REDGPU_2_EXPECTFL(ImFontAtlas_GetFontsCount(io->fonts) == 2);
        ImFontAtlas_SetFontAsDefault(io->fonts, 1);
      }
    }

    imguiSetProcessInputsState(1);
    imguiNewFrame();

    {
      vfWindowGetSize(ctx, &window_w, &window_h);

      if (window_w != previous_window_w || window_h != previous_window_h) {
        // Recreate output textures then.

        vfAllQueuesWaitIdle(ctx, FF, LL);

        vfGpuThreadDestroy(ctx, gpu_thread);
        gpu_thread = NULL;
        vfGpuThreadCreate(ctx, 1, &gpu_thread, NULL, FF, LL);

        reiiDestroyEx(ctx, GPU_EXTRA_REII_DESTROY_TYPE_TEXTURE, outputmstex);
        reiiDestroyEx(ctx, GPU_EXTRA_REII_DESTROY_TYPE_TEXTURE, outputtex);
        reiiDestroyEx(ctx, GPU_EXTRA_REII_DESTROY_TYPE_TEXTURE, outputdstex);

        reiiResetTextureMemory(ctx, &outputMSTexMemory);
        reiiResetTextureMemory(ctx, &outputTexMemory);
        reiiResetTextureMemory(ctx, &outputDSTexMemory);

        reiiCreateTextureFromTextureMemory(ctx, &outputDSTexMemory, REII_TEXTURE_BINDING_2D, &houtputdstex);
        reiiTextureSetStateMsaa(ctx, REII_TEXTURE_BINDING_2D, outputdstex, 4);
        reiiTextureSetStateMipmap(ctx, REII_TEXTURE_BINDING_2D, outputdstex, 0);
        reiiTextureSetStateMipmapLevelsCount(ctx, REII_TEXTURE_BINDING_2D, outputdstex, 1);
        reiiTextureDefineAndCopyFromCpu(ctx, REII_TEXTURE_BINDING_2D, outputdstex, 0, REII_TEXTURE_TEXEL_FORMAT_DS, window_w, window_h, REII_TEXTURE_TEXEL_FORMAT_DS, REII_TEXTURE_TEXEL_TYPE_FLOAT, 4, NULL, 1, &gpu_thread, array65536);

        reiiCreateTextureFromTextureMemory(ctx, &outputTexMemory, REII_TEXTURE_BINDING_2D, &houtputtex);
        reiiTextureSetStateMipmap(ctx, REII_TEXTURE_BINDING_2D, outputtex, 0);
        reiiTextureSetStateMipmapLevelsCount(ctx, REII_TEXTURE_BINDING_2D, outputtex, 1);
        reiiTextureDefineAndCopyFromCpu(ctx, REII_TEXTURE_BINDING_2D, outputtex, 0, REII_TEXTURE_TEXEL_FORMAT_RGBA, window_w, window_h, REII_TEXTURE_TEXEL_FORMAT_RGBA, REII_TEXTURE_TEXEL_TYPE_U8, 4, NULL, 1, &gpu_thread, array65536);

        reiiCreateTextureFromTextureMemory(ctx, &outputMSTexMemory, REII_TEXTURE_BINDING_2D, &houtputmstex);
        reiiTextureSetStateMsaa(ctx, REII_TEXTURE_BINDING_2D, outputmstex, 4);
        reiiTextureSetStateMipmap(ctx, REII_TEXTURE_BINDING_2D, outputmstex, 0);
        reiiTextureSetStateMipmapLevelsCount(ctx, REII_TEXTURE_BINDING_2D, outputmstex, 1);
        reiiTextureDefineAndCopyFromCpu(ctx, REII_TEXTURE_BINDING_2D, outputmstex, 0, REII_TEXTURE_TEXEL_FORMAT_RGBA, window_w, window_h, REII_TEXTURE_TEXEL_FORMAT_RGBA, REII_TEXTURE_TEXEL_TYPE_U8, 4, NULL, 1, &gpu_thread, array65536);
      }

      previous_window_w = window_w;
      previous_window_h = window_h;
    }

    static bool showTestWindow = 1;
    igShowTestWindow(&showTestWindow);
#if 0
    static bool styleOnce = 1;
    if (styleOnce == 1) {
      styleOnce = 0;

      // Flat UI by yorick.penninks: https://color.adobe.com/Flat-UI-color-theme-2469224/
      static ImVec3 color_for_text = {236 / 255.f, 240 / 255.f, 241 / 255.f};
      static ImVec3 color_for_head = { 41 / 255.f, 128 / 255.f, 185 / 255.f};
      static ImVec3 color_for_area = { 57 / 255.f,  79 / 255.f, 105 / 255.f};
      static ImVec3 color_for_body = { 44 / 255.f,  62 / 255.f,  80 / 255.f};
      static ImVec3 color_for_pops = { 33 / 255.f,  46 / 255.f,  60 / 255.f};

      // Mint Y Dark:
      //static struct ImVec3 color_for_text = {211 / 255.f, 211 / 255.f, 211 / 255.f};
      //static struct ImVec3 color_for_head = { 95 / 255.f, 142 / 255.f,  85 / 255.f};
      //static struct ImVec3 color_for_area = { 47 / 255.f,  47 / 255.f,  47 / 255.f};
      //static struct ImVec3 color_for_body = { 64 / 255.f,  64 / 255.f,  64 / 255.f};
      //static struct ImVec3 color_for_pops = { 30 / 255.f,  30 / 255.f,  30 / 255.f};

      // Arc Theme:
      //static struct ImVec3 color_for_text = {211 / 255.f, 218 / 255.f, 227 / 255.f};
      //static struct ImVec3 color_for_head = { 64 / 255.f, 132 / 255.f, 214 / 255.f};
      //static struct ImVec3 color_for_area = { 47 / 255.f,  52 / 255.f,  63 / 255.f};
      //static struct ImVec3 color_for_body = { 56 / 255.f,  60 / 255.f,  74 / 255.f};
      //static struct ImVec3 color_for_pops = { 28 / 255.f,  30 / 255.f,  37 / 255.f};

      igColorEdit3("Text", &color_for_text.x);
      igColorEdit3("Head", &color_for_head.x);
      igColorEdit3("Area", &color_for_area.x);
      igColorEdit3("Body", &color_for_body.x);
      igColorEdit3("Pops", &color_for_pops.x);

      imguiEasyTheming(color_for_text, color_for_head, color_for_area, color_for_body, color_for_pops);

      {
        ImguiStyle * style = (ImguiStyle *)igGetStyle();
        for (int i = 0; i < IMGUI_COLOR_COUNT; i += 1) {
          float x = style->colors[i].x;
          float y = style->colors[i].y;
          float z = style->colors[i].z;
          float w = style->colors[i].w;
          style->colors[i] = (ImVec4){x*x, y*y, z*z, w};
        }
      }
    }
#endif

    gpu_batch_info_t bindings_info = {0};
    bindings_info.max_new_bindings_sets_count = 1;
    bindings_info.max_storage_binds_count     = 0;
    bindings_info.max_texture_rw_binds_count  = 1;
    bindings_info.max_texture_ro_binds_count  = 0;
    bindings_info.max_sampler_binds_count     = 0;
    batch = vfBatchBegin(ctx, batch, &bindings_info, NULL, FF, LL);
    list->batch_id = batch;
    reiiCommandListReset(ctx, list);
    reiiCommandSetViewportEx(ctx, list, 0, 0, window_w, window_h, 0, 1);
    reiiCommandSetScissor(ctx, list, 0, 0, window_w, window_h);
    reiiCommandClearTexture(ctx, list, outputdstex, outputmstex, outputmstex->texture, REII_CLEAR_DEPTH_BIT | REII_CLEAR_COLOR_BIT, 0.f, 0, 0.f,0.f,0.f,1.f);
    reiiCommandResolveMsaaColorTexture(ctx, list, outputmstex, outputtex);
    vfBatchEnd(ctx, batch, FF, LL);
    {
      RedHandleCalls batchRaw = vfBatchGetRawHandle(ctx, batch, FF, LL);
      uint64_t wait = vfAsyncBatchExecuteRaw(ctx, 1, &batchRaw, 1, &gpu_thread, array65536, FF, LL);
      vfAsyncWaitToFinish(ctx, wait, FF, LL);
    }

    igRender();

    batch = vfBatchBegin(ctx, batch, &bindings_info, NULL, FF, LL);
    list->batch_id = batch;
    reiiCommandGammaCorrectColorTextureToTheInversePowerOf2(ctx, list, outputtex, doDoubleGammaCorrection, 1, &gammaCorrectionStaticState);
    vfBatchEnd(ctx, batch, FF, LL);
    {
      RedHandleCalls batchRaw = vfBatchGetRawHandle(ctx, batch, FF, LL);
      uint64_t wait = vfAsyncBatchExecuteRaw(ctx, 1, &batchRaw, 1, &gpu_thread, array65536, FF, LL);
      vfAsyncWaitToFinish(ctx, wait, FF, LL);
    }

    gpu_thread_t gpu_threads[2] = {gpu_thread, 0};
    vfAsyncDrawImageRaw(ctx, outputtex->image.handle, NULL, 2, gpu_threads, array65536, FF, LL);
  }

  vfAllQueuesWaitIdle(ctx, FF, LL);

  vfGpuThreadDestroy(ctx, gpu_thread);

  imguiDeinit();
  reiiDestroyEx(ctx, GPU_EXTRA_REII_DESTROY_TYPE_TEXTURE_MEMORY, &imgui_fontAtlasMemory);

  vfIdDestroy(1, &gammaCorrectionStaticState.programPipeline, FF, LL);
  vfIdDestroy(1, &gammaCorrectionStaticState.programCompute, FF, LL);
  reiiDestroyEx(ctx, GPU_EXTRA_REII_DESTROY_TYPE_COMMAND_LIST, list);
  reiiDestroyEx(ctx, GPU_EXTRA_REII_DESTROY_TYPE_TEXTURE, outputmstex);
  reiiDestroyEx(ctx, GPU_EXTRA_REII_DESTROY_TYPE_TEXTURE, outputtex);
  reiiDestroyEx(ctx, GPU_EXTRA_REII_DESTROY_TYPE_TEXTURE, outputdstex);
  reiiDestroyEx(ctx, GPU_EXTRA_REII_DESTROY_TYPE_TEXTURE_MEMORY, &outputMSTexMemory);
  reiiDestroyEx(ctx, GPU_EXTRA_REII_DESTROY_TYPE_TEXTURE_MEMORY, &outputTexMemory);
  reiiDestroyEx(ctx, GPU_EXTRA_REII_DESTROY_TYPE_TEXTURE_MEMORY, &outputDSTexMemory);
  uint64_t ids[] = {
    batch,
    storage_gpu_only.id,
    storage_cpu_upload.id,
    storage_cpu_readback.id,
  };
  vfIdDestroy(countof(ids), ids, FF, LL);
  vfContextDeinit(ctx, FF, LL);
  glfwTerminate();
}

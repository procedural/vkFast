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

typedef struct SudokuCell {
  int anti[9];
  int posi[9];
  int immu[9];
} SudokuCell;

void Sudoku9x9CellSetPosiImmutable(SudokuCell * cel,
  int _00, int _01, int _02, int _03, int _04, int _05, int _06, int _07, int _08,
  int _09, int _10, int _11, int _12, int _13, int _14, int _15, int _16, int _17,
  int _18, int _19, int _20, int _21, int _22, int _23, int _24, int _25, int _26,
  int _27, int _28, int _29, int _30, int _31, int _32, int _33, int _34, int _35,
  int _36, int _37, int _38, int _39, int _40, int _41, int _42, int _43, int _44,
  int _45, int _46, int _47, int _48, int _49, int _50, int _51, int _52, int _53,
  int _54, int _55, int _56, int _57, int _58, int _59, int _60, int _61, int _62,
  int _63, int _64, int _65, int _66, int _67, int _68, int _69, int _70, int _71,
  int _72, int _73, int _74, int _75, int _76, int _77, int _78, int _79, int _80
)
{
  int posi[81] = {
    _00, _01, _02, _03, _04, _05, _06, _07, _08,
    _09, _10, _11, _12, _13, _14, _15, _16, _17,
    _18, _19, _20, _21, _22, _23, _24, _25, _26,
    _27, _28, _29, _30, _31, _32, _33, _34, _35,
    _36, _37, _38, _39, _40, _41, _42, _43, _44,
    _45, _46, _47, _48, _49, _50, _51, _52, _53,
    _54, _55, _56, _57, _58, _59, _60, _61, _62,
    _63, _64, _65, _66, _67, _68, _69, _70, _71,
    _72, _73, _74, _75, _76, _77, _78, _79, _80
  };
  for (int i = 0; i < 81; i += 1) {
    if (posi[i] == 0) {
      continue;
    }
    cel[i].posi[posi[i]-1] = 1;
    cel[i].immu[posi[i]-1] = 1;
  }
}

void Sudoku9x9ClearAntiCell(SudokuCell * cel, int cellIndex) {
  for (int j = 0; j < 9; j += 1) {
    cel[cellIndex].anti[j] = 0;
  }
}

void Sudoku9x9ClearAllAntiCells(SudokuCell * cel) {
  for (int i = 0; i < 81; i += 1) {
    for (int j = 0; j < 9; j += 1) {
      cel[i].anti[j] = 0;
    }
  }
}

void Sudoku9x9SelectColumnRow(bool * sel, int centerIndex) {
  for (int i = 0; i < 81; i += 1) { // Clear all sel
    sel[i] = 0;
  }

  const int centerRow = centerIndex / 9;
  const int centerCol = centerIndex % 9;

  // Select
  for (int i = 0; i < 81; i += 1) {
    int row = i / 9;
    int col = i % 9;

    if (row == centerRow || col == centerCol) {
      sel[i] = 1;
    }
  }
}

void Sudoku9x9FillAntiCenterCellWithAvailablePosiNumbers(SudokuCell * cel, int centerIndex) {
  // First, set all the anti numbers to 1, then set some or all of them to 0 as matches in row/column/quadrant found.
  for (int j = 0; j < 9; j += 1) {
    cel[centerIndex].anti[j] = 1;
  }

  const int centerRow = centerIndex / 9;
  const int centerCol = centerIndex % 9;
  const int centerQua = (centerRow / 3) * 3 + (centerCol / 3);

  // Exclude row matches
  for (int i = 0; i < 81; i += 1) {
    int row = i / 9;
    int col = i % 9;
    int qua = (row / 3) * 3 + (col / 3);

    if (row == centerRow) { // If row matches our center cell row
      for (int j = 0; j < 9; j += 1) { // For all posi bools
        if (cel[i].posi[j] == 1) { // If posi bool is set to 1
          cel[centerIndex].anti[j] = 0; // Set anti bool to 0
          break;
        }
      }
    }
  }

  // Exclude column matches
  for (int i = 0; i < 81; i += 1) {
    int row = i / 9;
    int col = i % 9;
    int qua = (row / 3) * 3 + (col / 3);

    if (col == centerCol) { // If col matches our center cell col
      for (int j = 0; j < 9; j += 1) { // For all posi bools
        if (cel[i].posi[j] == 1) { // If posi bool is set to 1
          cel[centerIndex].anti[j] = 0; // Set anti bool to 0
          break;
        }
      }
    }
  }

  // Exclude quadrant matches
  for (int i = 0; i < 81; i += 1) {
    int row = i / 9;
    int col = i % 9;
    int qua = (row / 3) * 3 + (col / 3);

    if (qua == centerQua) { // If qua matches our center cell qua
      for (int j = 0; j < 9; j += 1) { // For all posi bools
        if (cel[i].posi[j] == 1) { // If posi bool is set to 1
          cel[centerIndex].anti[j] = 0; // Set anti bool to 0
          break;
        }
      }
    }
  }
}

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
  GLFWwindow * window = glfwCreateWindow(window_w, window_h, "[vkFast] REII Anti Sudoku", 0, 0);
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
  vfWindowFullscreen(ctx, window_handle, "[vkFast] REII Anti Sudoku", window_w, window_h, 0, RED_PRESENT_VSYNC_MODE_ON, FF, LL);

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

  SudokuCell cel[81] = {};

  // Hardcoded
  Sudoku9x9CellSetPosiImmutable(cel,
    8,1,0,0,0,0,6,3,0,
    0,0,5,8,0,0,0,9,0,
    0,9,0,0,5,0,0,0,8,
    0,0,6,4,0,1,0,8,0,
    0,0,9,0,0,0,3,0,0,
    0,2,0,6,0,9,7,0,0,
    3,0,0,0,2,0,0,4,0,
    0,4,0,0,0,6,5,0,0,
    0,5,2,0,0,0,0,6,7
  );

  bool sel[81]     = {};
  int  centerIndex = 0;
  Sudoku9x9SelectColumnRow(sel, centerIndex);

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

    static bool showAntiSudokuParamsWindow = 1;
    ImVec2 paramsDefaultSize = {550, 680};
    igSetNextWindowSize(paramsDefaultSize, (1 << 2)/*ImGuiSetCond_FirstUseEver*/);
    if (!igBegin("Parameters", &showAntiSudokuParamsWindow, 0))
    {
      // Early out if the window is collapsed, as an optimization.
      igEnd();
    } else {
      static int setNum = 0;
      igInputInt("Set number", &setNum, 1, 1, 0);
      ImVec2 buttonSize = {};
      if (igButton("Toggle number in anti center cell", buttonSize)) {
        if (setNum >= 1 && setNum <= 9) {
          cel[centerIndex].anti[setNum-1] ^= 1;
        }
      }
      if (igButton("Set number in posi center cell", buttonSize)) {
        Sudoku9x9ClearAllAntiCells(cel);

        if (setNum == 0) { // Clear posi numbers, if not immutable
          int hasImmu = 0;
          for (int i = 0; i < 9; i += 1) {
            if (cel[centerIndex].immu[i] == 1) {
              hasImmu = 1;
              break;
            }
          }
          if (hasImmu == 0) {
            for (int i = 0; i < 9; i += 1) { // Clear posi numbers
              cel[centerIndex].posi[i] = 0;
            }
          }
        }
        if (setNum >= 1 && setNum <= 9) { // Set posi number, if not immutable
          int hasImmu = 0;
          for (int i = 0; i < 9; i += 1) {
            if (cel[centerIndex].immu[i] == 1) {
              hasImmu = 1;
              break;
            }
          }
          if (hasImmu == 0) {
            for (int i = 0; i < 9; i += 1) { // Clear posi numbers
              cel[centerIndex].posi[i] = 0;
            }
            cel[centerIndex].posi[setNum-1] = 1;
          }
        }
      }
      if (igButton("Clear all anti cells", buttonSize)) {
        Sudoku9x9ClearAllAntiCells(cel);
      }
      if (igButton("Fill anti center cell with available posi numbers", buttonSize)) {
        Sudoku9x9ClearAntiCell(cel, centerIndex);
        Sudoku9x9FillAntiCenterCellWithAvailablePosiNumbers(cel, centerIndex);
      }
      if (igButton("Fill all anti cells with available posi numbers", buttonSize)) {
        Sudoku9x9ClearAllAntiCells(cel);

        for (int i = 0; i < 81; i += 1) {
          int hasPosi = 0;
          for (int j = 0; j < 9; j += 1) {
            if (cel[i].posi[j] == 1) {
              hasPosi = 1;
              break;
            }
          }
          if (hasPosi == 0) {
            Sudoku9x9FillAntiCenterCellWithAvailablePosiNumbers(cel, i);
          }
        }
      }

      igEnd();
    }
    static bool showAntiSudokuWindow = 1;
    ImVec2 defaultSize = {550, 680};
    igSetNextWindowSize(defaultSize, (1 << 2)/*ImGuiSetCond_FirstUseEver*/);
    if (!igBegin("Anti Sudoku Field", &showAntiSudokuWindow, 0))
    {
      // Early out if the window is collapsed, as an optimization.
      igEnd();
    } else {

      for (int i = 0; i < 81; i += 1) {
        igPushIdInt(i);

        ImVec4 col = {1, 0, 0, 1};
        //igPushStyleColor(0/*ImGuiCol_Text*/, col);

        ImVec2 size = {140,140};
        char buf[256] = {};
        sprintf(buf, "Anti:\n%c %c %c %c\n%c %c %c %c %c\nPosi:\n%c%c %c%c %c%c %c%c\n%c%c %c%c %c%c %c%c %c%c\n",
                                  cel[i].anti[0]?'1':' ',
                                  cel[i].anti[1]?'2':' ',
                                  cel[i].anti[2]?'3':' ',
                                  cel[i].anti[3]?'4':' ',
                                  cel[i].anti[4]?'5':' ',
                                  cel[i].anti[5]?'6':' ',
                                  cel[i].anti[6]?'7':' ',
                                  cel[i].anti[7]?'8':' ',
                                  cel[i].anti[8]?'9':' ',
          cel[i].immu[0]?'_':' ', cel[i].posi[0]?'1':' ',
          cel[i].immu[1]?'_':' ', cel[i].posi[1]?'2':' ',
          cel[i].immu[2]?'_':' ', cel[i].posi[2]?'3':' ',
          cel[i].immu[3]?'_':' ', cel[i].posi[3]?'4':' ',
          cel[i].immu[4]?'_':' ', cel[i].posi[4]?'5':' ',
          cel[i].immu[5]?'_':' ', cel[i].posi[5]?'6':' ',
          cel[i].immu[6]?'_':' ', cel[i].posi[6]?'7':' ',
          cel[i].immu[7]?'_':' ', cel[i].posi[7]?'8':' ',
          cel[i].immu[8]?'_':' ', cel[i].posi[8]?'9':' '
        );
        bool setCenterIndex = 0;
        if (igSelectableEx(buf, &sel[i], 0, size)) {
          setCenterIndex = 1;
          Sudoku9x9SelectColumnRow(sel, i);
        }
        if ((i % 9) < 8) igSameLine(0, -1);

        //igPopStyleColor(1);

        igPopId();

        if (setCenterIndex == 1) {
          centerIndex = i;
        }
      }

      igEnd();
    }

    #if 0
    static bool showAntiSudokuParamsWindow = 1;
    ImVec2 paramsDefaultSize = {550, 680};
    igSetNextWindowSize(paramsDefaultSize, (1 << 2)/*ImGuiSetCond_FirstUseEver*/);
    if (!igBegin("Parameters", &showAntiSudokuParamsWindow, 0))
    {
      // Early out if the window is collapsed, as an optimization.
      igEnd();
    } else {
      static int setNum = 0;
      igInputInt("Set number", &setNum, 1, 1, 0);
      ImVec2 buttonSize = {};
      if (igButton("Toggle number in anti center cell", buttonSize)) {
        if (setNum >= 1 && setNum <= 9) {
          cel[centerIndex].anti[setNum-1] ^= 1;
        }
      }
      if (igButton("Set number in posi center cell", buttonSize)) {
        Sudoku9x9ClearAllAntiCells(cel);

        if (setNum == 0) { // Clear posi numbers, if not immutable
          int hasImmu = 0;
          for (int i = 0; i < 9; i += 1) {
            if (cel[centerIndex].immu[i] == 1) {
              hasImmu = 1;
              break;
            }
          }
          if (hasImmu == 0) {
            for (int i = 0; i < 9; i += 1) { // Clear posi numbers
              cel[centerIndex].posi[i] = 0;
            }
          }
        }
        if (setNum >= 1 && setNum <= 9) { // Set posi number, if not immutable
          int hasImmu = 0;
          for (int i = 0; i < 9; i += 1) {
            if (cel[centerIndex].immu[i] == 1) {
              hasImmu = 1;
              break;
            }
          }
          if (hasImmu == 0) {
            for (int i = 0; i < 9; i += 1) { // Clear posi numbers
              cel[centerIndex].posi[i] = 0;
            }
            cel[centerIndex].posi[setNum-1] = 1;
          }
        }
      }
      if (igButton("Clear all anti cells", buttonSize)) {
        Sudoku9x9ClearAllAntiCells(cel);
      }
      if (igButton("Fill anti center cell with available posi numbers", buttonSize)) {
        Sudoku9x9FillAntiCenterCellWithAvailablePosiNumbers(cel, centerIndex);
      }

      igEnd();
    }
    static bool showAntiSudokuWindow = 1;
    ImVec2 defaultSize = {550, 680};
    igSetNextWindowSize(defaultSize, (1 << 2)/*ImGuiSetCond_FirstUseEver*/);
    if (!igBegin("Anti Sudoku Field", &showAntiSudokuWindow, 0))
    {
      // Early out if the window is collapsed, as an optimization.
      igEnd();
    } else {

      for (int i = 0; i < 81; i += 1) {
        igPushIdInt(i);

        ImVec4 col = {1, 0, 0, 1};
        //igPushStyleColor(0/*ImGuiCol_Text*/, col);

        ImVec2 size = {140,140};
        char buf[256] = {};
        sprintf(buf, "Anti:\n%c %c %c %c\n%c %c %c %c %c\nPosi:\n%c%c %c%c %c%c %c%c\n%c%c %c%c %c%c %c%c %c%c\n",
                                  cel[i].anti[0]?'1':' ',
                                  cel[i].anti[1]?'2':' ',
                                  cel[i].anti[2]?'3':' ',
                                  cel[i].anti[3]?'4':' ',
                                  cel[i].anti[4]?'5':' ',
                                  cel[i].anti[5]?'6':' ',
                                  cel[i].anti[6]?'7':' ',
                                  cel[i].anti[7]?'8':' ',
                                  cel[i].anti[8]?'9':' ',
          cel[i].immu[0]?'_':' ', cel[i].posi[0]?'1':' ',
          cel[i].immu[1]?'_':' ', cel[i].posi[1]?'2':' ',
          cel[i].immu[2]?'_':' ', cel[i].posi[2]?'3':' ',
          cel[i].immu[3]?'_':' ', cel[i].posi[3]?'4':' ',
          cel[i].immu[4]?'_':' ', cel[i].posi[4]?'5':' ',
          cel[i].immu[5]?'_':' ', cel[i].posi[5]?'6':' ',
          cel[i].immu[6]?'_':' ', cel[i].posi[6]?'7':' ',
          cel[i].immu[7]?'_':' ', cel[i].posi[7]?'8':' ',
          cel[i].immu[8]?'_':' ', cel[i].posi[8]?'9':' '
        );
        bool setCenterIndex = 0;
        if (igSelectableEx(buf, &sel[i], 0, size)) {
          setCenterIndex = 1;
          Sudoku9x9SelectColumnRow(sel, i);
        }
        if ((i % 9) < 8) igSameLine(0, -1);

        //igPopStyleColor(1);

        igPopId();

        if (setCenterIndex == 1) {
          centerIndex = i;
        }
      }

      igEnd();
    }
    #endif

    #if 0
    static bool showAntiSudokuWindow = 1;
    ImVec2 defaultSize = {550, 680};
    igSetNextWindowSize(defaultSize, (1 << 2)/*ImGuiSetCond_FirstUseEver*/);
    if (!igBegin("Anti Sudoku Field", &showAntiSudokuWindow, 0))
    {
      // Early out if the window is collapsed, as an optimization.
      igEnd();
    } else {

      for (int i = 0; i < 81; i += 1) {
        igPushIdInt(i);

        ImVec4 col = {1, 0, 0, 1};
        //igPushStyleColor(0/*ImGuiCol_Text*/, col);

        ImVec2 size = {140,140};
        char buf[256] = {};
        sprintf(buf, "Anti:\n%c %c %c %c\n%c %c %c %c %c\nPosi:\n%c%c %c%c %c%c %c%c\n%c%c %c%c %c%c %c%c %c%c\n",
                                  cel[i].anti[0]?'1':' ',
                                  cel[i].anti[1]?'2':' ',
                                  cel[i].anti[2]?'3':' ',
                                  cel[i].anti[3]?'4':' ',
                                  cel[i].anti[4]?'5':' ',
                                  cel[i].anti[5]?'6':' ',
                                  cel[i].anti[6]?'7':' ',
                                  cel[i].anti[7]?'8':' ',
                                  cel[i].anti[8]?'9':' ',
          cel[i].immu[0]?'_':' ', cel[i].posi[0]?'1':' ',
          cel[i].immu[1]?'_':' ', cel[i].posi[1]?'2':' ',
          cel[i].immu[2]?'_':' ', cel[i].posi[2]?'3':' ',
          cel[i].immu[3]?'_':' ', cel[i].posi[3]?'4':' ',
          cel[i].immu[4]?'_':' ', cel[i].posi[4]?'5':' ',
          cel[i].immu[5]?'_':' ', cel[i].posi[5]?'6':' ',
          cel[i].immu[6]?'_':' ', cel[i].posi[6]?'7':' ',
          cel[i].immu[7]?'_':' ', cel[i].posi[7]?'8':' ',
          cel[i].immu[8]?'_':' ', cel[i].posi[8]?'9':' '
        );
        if (igSelectableEx(buf, &sel[i], 0, size)) {
          centerIndex = i;
          Sudoku9x9SelectColumnRow(sel, i);
        }
        if ((i % 9) < 8) igSameLine(0, -1);

        //igPopStyleColor(1);

        igPopId();
      }

      igEnd();
    }
    #endif

    #if 0
    static bool showAntiSudokuWindow = 1;
    ImVec2 defaultSize = {550, 680};
    igSetNextWindowSize(defaultSize, (1 << 2)/*ImGuiSetCond_FirstUseEver*/);
    if (!igBegin("Анти Судоку", &showAntiSudokuWindow, 0))
    {
      // Early out if the window is collapsed, as an optimization.
      igEnd();
    } else {

      static bool sel[81] = {};
      for (int i = 0; i < 81; i += 1) {
        igPushIdInt(i);

        ImVec4 col = {1, 0, 0, 1};
        //igPushStyleColor(0/*ImGuiCol_Text*/, col);

        ImVec2 size = {140,140};
        char buf[256] = {};
        sprintf(buf, "Анти:\n%c %c %c %c %c\n%c %c %c %c\nПози:\n%c%c %c%c %c%c %c%c %c%c\n%c%c %c%c %c%c %c%c\n",
                                  cel[i].anti[0]?'1':' ',
                                  cel[i].anti[1]?'2':' ',
                                  cel[i].anti[2]?'3':' ',
                                  cel[i].anti[3]?'4':' ',
                                  cel[i].anti[4]?'5':' ',
                                  cel[i].anti[5]?'6':' ',
                                  cel[i].anti[6]?'7':' ',
                                  cel[i].anti[7]?'8':' ',
                                  cel[i].anti[8]?'9':' ',
          cel[i].immu[0]?'_':' ', cel[i].posi[0]?'1':' ',
          cel[i].immu[1]?'_':' ', cel[i].posi[1]?'2':' ',
          cel[i].immu[2]?'_':' ', cel[i].posi[2]?'3':' ',
          cel[i].immu[3]?'_':' ', cel[i].posi[3]?'4':' ',
          cel[i].immu[4]?'_':' ', cel[i].posi[4]?'5':' ',
          cel[i].immu[5]?'_':' ', cel[i].posi[5]?'6':' ',
          cel[i].immu[6]?'_':' ', cel[i].posi[6]?'7':' ',
          cel[i].immu[7]?'_':' ', cel[i].posi[7]?'8':' ',
          cel[i].immu[8]?'_':' ', cel[i].posi[8]?'9':' '
        );
        if (igSelectableEx(buf, &sel[i], 0, size)) {
          for (int j = 0; j < 81; j += 1) { // Clear all
            sel[j] = 0;
          }
          sel[i] ^= 1;
        }
        if ((i % 9) < 8) igSameLine(0, -1);

        //igPopStyleColor(1);

        igPopId();
      }

      igEnd();
    }
    #endif

    #if 0
    static bool showAntiSudokuWindow = 1;
    ImVec2 defaultSize = {550, 680};
    igSetNextWindowSize(defaultSize, (1 << 2)/*ImGuiSetCond_FirstUseEver*/);
    if (!igBegin("Анти Судоку", &showAntiSudokuWindow, 0))
    {
      // Early out if the window is collapsed, as an optimization.
      igEnd();
    } else {

      static bool sel[81] = {};
      for (int i = 0; i < 81; i += 1) {
        igPushIdInt(i);

        ImVec4 col = {1, 0, 0, 1};
        igPushStyleColor(0/*ImGuiCol_Text*/, col);

        ImVec2 size = {140,140};
        if (igSelectableEx("Анти:\n1 2 3 4 5\n6 7 8 9\nПози:\n1 2 3 4 5\n6 7 8 9\n", &sel[i], 0, size)) {
          for (int j = 0; j < 81; j += 1) { // Clear all
            sel[j] = 0;
          }
          sel[i] ^= 1;
        }
        if ((i % 9) < 8) igSameLine(0, -1);

        igPopStyleColor(1);

        igPopId();
      }

      igEnd();
    }
    #endif

    #if 0
    static bool showAntiSudokuWindow = 1;
    ImVec2 defaultSize = {550, 680};
    igSetNextWindowSize(defaultSize, (1 << 2)/*ImGuiSetCond_FirstUseEver*/);
    if (!igBegin("Anti Sudoku", &showAntiSudokuWindow, 0))
    {
      // Early out if the window is collapsed, as an optimization.
      igEnd();
    } else {
      if (igTreeNode("Sudoku Grid")) {
        static bool sel[9] = {};
        for (int i = 0; i < 9; i += 1) {
          igPushIdInt(i);
          ImVec2 size = {140,140};
          if (igSelectableEx("Anti:\n1 2 3 4 5\n6 7 8 9\nPosi:\n1 2 3 4 5\n6 7 8 9\n", &sel[i], 0, size)) {
            for (int j = 0; j < 9; j += 1) { // Clear all
              sel[j] = 0;
            }
            sel[i] ^= 1;
          }
          if ((i % 3) < 2) igSameLine(0, -1);
          igPopId();
        }
        igTreePop();
      }

      igEnd();
    }
    #endif

    #if 0
    static bool showAntiSudokuWindow = 1;
    ImVec2 defaultSize = {550, 680};
    igSetNextWindowSize(defaultSize, (1 << 2)/*ImGuiSetCond_FirstUseEver*/);
    if (!igBegin("Anti Sudoku", &showAntiSudokuWindow, 0))
    {
      // Early out if the window is collapsed, as an optimization.
      igEnd();
    } else {
      if (igTreeNode("Sudoku Grid")) {
        static bool sel[16] = {True, False, False, False, False, True, False, False, False, False, True, False, False, False, False, True};
        for (int i = 0; i < 16; i += 1) {
          igPushIdInt(i);
          ImVec2 size = {140,140};
          if (igSelectableEx("Anti:\n1 2 3 4 5\n6 7 8 9\nPosi:\n1 2 3 4 5\n6 7 8 9\n", &sel[i], 0, size)) {
            printf("sel[%d] was: %d\n", i, sel[i]);
            int x = i % 4, y = i / 4;
            if (x > 0) sel[i - 1] ^= 1;
            if (x < 3) sel[i + 1] ^= 1;
            if (y > 0) sel[i - 4] ^= 1;
            if (y < 3) sel[i + 4] ^= 1;
          }
          if ((i % 4) < 3) igSameLine(0, -1);
          igPopId();
        }
        igTreePop();
      }

      igEnd();
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

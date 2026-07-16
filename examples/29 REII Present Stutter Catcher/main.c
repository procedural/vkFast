//\\rc rawbuild begin gcc-linux-64-bit
//\\rc rawbuild require debug,release,release-fast
//\\rc rawbuild require glfw3,sdl3
//\\rc rawbuild `g++ -shared -fPIC -fvisibility=hidden`
//\\rc rawbuild debug ` -g -O0`
//\\rc rawbuild release,release-fast ` -O2`
//\\rc rawbuild ` "../../extra/Dear ImGui 2016/imgui_megafile.cpp" -o libimgui.so`
//\\rc rawbuild next_command
//\\rc rawbuild `gcc`
//\\rc rawbuild debug ` -g -O0`
//\\rc rawbuild release,release-fast ` -O2`
//\\rc rawbuild sdl3 ` -DVKFAST_EXAMPLE_29_ENABLE_GLFW3_TO_SDL3`
//\\rc rawbuild ` main.c ../Common/font_droid_sans_mono.c ../../vkfast.c ../../extra/Banzai/vkfast_extra_banzai.c ../../extra/Banzai/vkfast_extra_banzai_pointer.c "../../extra/CPU GPU Array/vkfast_extra_cpu_gpu_array.c" ../../extra/REII/vkfast_extra_reii.c /home/linuxbrew/RedGpuSDK/redgpu.c /home/linuxbrew/RedGpuSDK/redgpu_2.c /home/linuxbrew/RedGpuSDK/redgpu_32.c -I/home/linuxbrew/.linuxbrew/include/ -I/home/linuxbrew/.linuxbrew/Cellar/xorgproto/2025.1/include/ -I/var/home/linuxbrew/.linuxbrew/Cellar/libxcb/1.17.0/include/`
//\\rc rawbuild glfw3 ` /home/linuxbrew/.linuxbrew/Cellar/glfw/3.4/lib/libglfw3.a`
//\\rc rawbuild sdl3 ` /home/linuxbrew/.linuxbrew/lib/libSDL3.so`
//\\rc rawbuild ` /home/linuxbrew/.linuxbrew/lib/libX11.so /home/linuxbrew/.linuxbrew/lib/libvulkan.so libimgui.so -lm`
//\\rc rawbuild end

// /home/linuxbrew/.linuxbrew/lib/libSDL3.so

//\\rc rawbuild begin clang-windows-64-bit
//\\rc rawbuild require debug,release,release-fast
//\\rc rawbuild `clang++ -shared -fvisibility=hidden`
//\\rc rawbuild debug ` -g -O0`
//\\rc rawbuild release,release-fast ` -O2`
//\\rc rawbuild ` "../../extra/Dear ImGui 2016/imgui_megafile.cpp" -o libimgui.so`
//\\rc rawbuild next_command
//\\rc rawbuild `clang`
//\\rc rawbuild debug ` -g -O0`
//\\rc rawbuild release,release-fast ` -O2`
//\\rc rawbuild ` main.c ../Common/font_droid_sans_mono.c ../../vkfast.c C:/RedGpuSDK/redgpu.c C:/RedGpuSDK/redgpu_2.c C:/RedGpuSDK/redgpu_32.c ../../extra/Banzai/vkfast_extra_banzai.c ../../extra/Banzai/vkfast_extra_banzai_pointer.c "../../extra/CPU GPU Array/vkfast_extra_cpu_gpu_array.c" ../../extra/REII/vkfast_extra_reii.c libimgui.so ../Common/glfw-3.4.bin.WIN64/lib-mingw-w64/libglfw3.a -lgdi32`
//\\rc rawbuild end

//\\rc rawbuild begin mingw-clang-termux-64-bit
//\\rc rawbuild `x86_64-w64-mingw32-clang++ -DVKFAST_INCLUDE_TERMUX_PATHS -shared -static-libgcc -static-libstdc++ -fPIC -fvisibility=hidden "../../extra/Dear ImGui 2016/imgui_megafile.cpp" -o imgui.dll`
//\\rc rawbuild next_command
//\\rc rawbuild `x86_64-w64-mingw32-clang -DVKFAST_INCLUDE_TERMUX_PATHS main.c ../../vkfast.c /data/data/com.termux/files/home/RedGpuSDK/redgpu.c /data/data/com.termux/files/home/RedGpuSDK/redgpu_2.c /data/data/com.termux/files/home/RedGpuSDK/redgpu_32.c ../../extra/Banzai/vkfast_extra_banzai.c ../../extra/Banzai/vkfast_extra_banzai_pointer.c "../../extra/CPU GPU Array/vkfast_extra_cpu_gpu_array.c" ../../extra/REII/vkfast_extra_reii.c ../Common/font_droid_sans_mono.c ../Common/glfw-3.4.bin.WIN64/lib-mingw-w64/libglfw3.a -luser32 -lshell32 -lgdi32 imgui.dll`
//\\rc rawbuild next_command
//\\rc rawbuild `cp a.exe /storage/emulated/0/Download/`
//\\rc rawbuild next_command
//\\rc rawbuild `cp imgui.dll /storage/emulated/0/Download/`
//\\rc rawbuild next_command
//\\rc rawbuild `cp ../../extra/Dear\ ImGui\ 2016/NotoSans.ttf /storage/emulated/0/Download/`
//\\rc rawbuild end

#include "../../vkfast_ex.h"
#include "../../vkfast_ids.h"
#include "../../extra/Banzai/vkfast_extra_banzai_pointer.h"
#include "../../extra/REII/vkfast_extra_reii.h"
#ifndef VKFAST_EXAMPLE_29_ENABLE_GLFW3_TO_SDL3
#define VKFAST_EXAMPLES_COMMON_INCLUDE_GLFW3
#endif
#define VKFAST_EXAMPLES_COMMON_INCLUDE_EXTRA_BANZAI
#include "../Common/vkfast_examples_common.h"
#ifdef VKFAST_EXAMPLE_29_ENABLE_GLFW3_TO_SDL3
#include "../../extra/GLFW3 to SDL3/vkfast_extra_glfw3_to_sdl3.h"
#endif
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

#if defined(_WIN32)
  LARGE_INTEGER frequency = {0};
  REDGPU_2_EXPECTFL(QueryPerformanceFrequency(&frequency) == TRUE); // Query the frequency (ticks per second)
#endif

  int window_w = 1000;
  int window_h = 1000;

  glfwInit();
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  GLFWwindow * window = glfwCreateWindow(window_w, window_h, "[vkFast] REII Present Stutter Catcher", 0, 0);
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
  vfWindowFullscreen(ctx, window_handle, "[vkFast] REII Present Stutter Catcher", window_w, window_h, 0, RED_PRESENT_VSYNC_MODE_ON, FF, LL);

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

  gpu_extra_cpu_gpu_array mesh_vertex_array = OffsetAllocateCpuGpuArrayWithTale64BytesAlign(
    64/*mb*/ * 1024 * 1024,
    &storage_cpu_upload, &storage_cpu_upload_mem_offset,
    &storage_gpu_only,   &storage_gpu_only_mem_offset,
    FF, LL
  );
  gpu_extra_cpu_gpu_array instance_color_array = OffsetAllocateCpuGpuArrayWithTale64BytesAlign(
    64/*mb*/ * 1024 * 1024,
    &storage_cpu_upload, &storage_cpu_upload_mem_offset,
    &storage_gpu_only,   &storage_gpu_only_mem_offset,
    FF, LL
  );
  gpu_extra_cpu_gpu_array instance_position_array = OffsetAllocateCpuGpuArrayWithTale64BytesAlign(
    64/*mb*/ * 1024 * 1024,
    &storage_cpu_upload, &storage_cpu_upload_mem_offset,
    &storage_gpu_only,   &storage_gpu_only_mem_offset,
    FF, LL
  );

  #include "mesh.vs.h"
  #include "mesh.fs.h"
  gpu_program_info_t vp = {0};
  vp.program_binary_bytes_count = sizeof(g_main_vs);
  vp.program_binary             = g_main_vs;
  gpu_program_info_t fp = {0};
  fp.program_binary_bytes_count = sizeof(g_main_fs);
  fp.program_binary             = g_main_fs;
  char * vp_string = NULL;
  char * fp_string = NULL;

  RedStructDeclarationMember slots[3] = {0};
  slots[0].slot            = 0;
  slots[0].type            = RED_STRUCT_MEMBER_TYPE_ARRAY_RO_RW;
  slots[0].count           = 1;
  slots[0].visibleToStages = RED_VISIBLE_TO_STAGE_BITFLAG_VERTEX;
  slots[1].slot            = 1;
  slots[1].type            = RED_STRUCT_MEMBER_TYPE_ARRAY_RO_RW;
  slots[1].count           = 1;
  slots[1].visibleToStages = RED_VISIBLE_TO_STAGE_BITFLAG_VERTEX;
  slots[2].slot            = 2;
  slots[2].type            = RED_STRUCT_MEMBER_TYPE_ARRAY_RO_RW;
  slots[2].count           = 1;
  slots[2].visibleToStages = RED_VISIBLE_TO_STAGE_BITFLAG_VERTEX;
  gpu_extra_reii_mesh_state_compile_info_t mesh_state_compile_info = {0};
  mesh_state_compile_info.state_multisample_count     = RED_MULTISAMPLE_COUNT_BITFLAG_4;
  mesh_state_compile_info.output_depth_stencil_enable = 1;
  mesh_state_compile_info.output_depth_stencil_format = RED_FORMAT_DEPTH_32_FLOAT;
  mesh_state_compile_info.output_color_format         = RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1;
  mesh_state_compile_info.variables_slot              = 3;
  mesh_state_compile_info.variables_bytes_count       = 2 * sizeof(ReiiVec4);
  mesh_state_compile_info.struct_members_count        = countof(slots);
  mesh_state_compile_info.struct_members              = slots;
  ReiiMeshState mesh_state                                  = {0};
  mesh_state.compileInfo                                    = mesh_state_compile_info;
  mesh_state.programVertex                                  = vp;
  mesh_state.programFragment                                = fp;
  mesh_state.codeVertex                                     = vp_string;
  mesh_state.codeFragment                                   = fp_string;
  mesh_state.rasterizationDepthClampEnable                  = 0;
  mesh_state.rasterizationCullMode                          = REII_CULL_MODE_NONE;
  mesh_state.rasterizationFrontFace                         = REII_FRONT_FACE_COUNTER_CLOCKWISE;
  mesh_state.rasterizationDepthBiasEnable                   = 0;
  mesh_state.rasterizationDepthBiasConstantFactor           = 0;
  mesh_state.rasterizationDepthBiasSlopeFactor              = 0;
  mesh_state.multisampleEnable                              = 1;
  mesh_state.multisampleAlphaToCoverageEnable               = 0;
  mesh_state.multisampleAlphaToOneEnable                    = 0;
  mesh_state.depthTestEnable                                = 1;
  mesh_state.depthTestDepthWriteEnable                      = 1;
  mesh_state.depthTestDepthCompareOp                        = REII_COMPARE_OP_GREATER_OR_EQUAL;
  mesh_state.stencilTestEnable                              = 0;
  mesh_state.stencilTestFrontStencilTestFailOp              = REII_STENCIL_OP_KEEP;
  mesh_state.stencilTestFrontStencilTestPassDepthTestPassOp = REII_STENCIL_OP_KEEP;
  mesh_state.stencilTestFrontStencilTestPassDepthTestFailOp = REII_STENCIL_OP_KEEP;
  mesh_state.stencilTestFrontCompareOp                      = REII_COMPARE_OP_NEVER;
  mesh_state.stencilTestBackStencilTestFailOp               = REII_STENCIL_OP_KEEP;
  mesh_state.stencilTestBackStencilTestPassDepthTestPassOp  = REII_STENCIL_OP_KEEP;
  mesh_state.stencilTestBackStencilTestPassDepthTestFailOp  = REII_STENCIL_OP_KEEP;
  mesh_state.stencilTestBackCompareOp                       = REII_COMPARE_OP_NEVER;
  mesh_state.stencilTestFrontAndBackCompareMask             = 0;
  mesh_state.stencilTestFrontAndBackWriteMask               = 0;
  mesh_state.stencilTestFrontAndBackReference               = 0;
  mesh_state.blendLogicOpEnable                             = 0;
  mesh_state.blendLogicOp                                   = REII_LOGIC_OP_CLEAR;
  mesh_state.blendConstants[0]                              = 0;
  mesh_state.blendConstants[1]                              = 0;
  mesh_state.blendConstants[2]                              = 0;
  mesh_state.blendConstants[3]                              = 0;
  mesh_state.outputColorWriteEnableR                        = 1;
  mesh_state.outputColorWriteEnableG                        = 1;
  mesh_state.outputColorWriteEnableB                        = 1;
  mesh_state.outputColorWriteEnableA                        = 1;
  mesh_state.outputColorBlendEnable                         = 0;
  mesh_state.outputColorBlendColorFactorSource              = REII_BLEND_FACTOR_ZERO;
  mesh_state.outputColorBlendColorFactorTarget              = REII_BLEND_FACTOR_ZERO;
  mesh_state.outputColorBlendColorOp                        = REII_BLEND_OP_ADD;
  mesh_state.outputColorBlendAlphaFactorSource              = REII_BLEND_FACTOR_ZERO;
  mesh_state.outputColorBlendAlphaFactorTarget              = REII_BLEND_FACTOR_ZERO;
  mesh_state.outputColorBlendAlphaOp                        = REII_BLEND_OP_ADD;
  reiiMeshStateCompile(ctx, &mesh_state);

  ReiiHandleTextureMemory outputDSTexMemory = {0};
  reiiCreateTextureMemory(ctx, GPU_EXTRA_REII_TEXTURE_TYPE_OUTPUT_DEPTH_STENCIL_MSAA, (288/*mb*/ * 1024 * 1024), &outputDSTexMemory);
  ReiiHandleTexture houtputdstex = {0};
  ReiiHandleTexture * outputdstex = &houtputdstex;
  reiiCreateTextureFromTextureMemory(ctx, &outputDSTexMemory, REII_TEXTURE_BINDING_2D, &houtputdstex);
  reiiTextureSetStateMsaa(ctx, REII_TEXTURE_BINDING_2D, outputdstex, RED_MULTISAMPLE_COUNT_BITFLAG_4);
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
  reiiTextureSetStateMsaa(ctx, REII_TEXTURE_BINDING_2D, outputmstex, RED_MULTISAMPLE_COUNT_BITFLAG_4);
  reiiTextureSetStateMipmap(ctx, REII_TEXTURE_BINDING_2D, outputmstex, 0);
  reiiTextureSetStateMipmapLevelsCount(ctx, REII_TEXTURE_BINDING_2D, outputmstex, 1);
  reiiTextureDefineAndCopyFromCpu(ctx, REII_TEXTURE_BINDING_2D, outputmstex, 0, REII_TEXTURE_TEXEL_FORMAT_RGBA, window_w, window_h, REII_TEXTURE_TEXEL_FORMAT_RGBA, REII_TEXTURE_TEXEL_TYPE_U8, 4, NULL, 1, &gpu_thread, array65536);

  float mesh_vertices[] = {
    #include "../../extra/3D Mesh Suzanne Head/3d_mesh_vertices_suzanne_head.h"
  };

  ReiiHandleUnorderedArray hmesh = {0};
  ReiiHandleUnorderedArray * mesh = &hmesh;
  mesh->position = mesh_vertex_array;
  reiiCreateUnorderedArray(ctx, mesh);
  reiiUnorderedArraySet(ctx, mesh);
  for (int i = 0, mesh_vertices_count = countof(mesh_vertices) / 3; i < mesh_vertices_count; i += 1) {
    reiiUnorderedArrayPosition(ctx, mesh, mesh_vertices[i * 3 + 0], mesh_vertices[i * 3 + 1], mesh_vertices[i * 3 + 2], 1);
  }
  reiiUnorderedArrayEnd(ctx, mesh, 1, &gpu_thread, array65536);

  const int instanceCountX = 15;
  const int instanceCountY = 15;
  const int instanceCountZ = 15;
  RandomInit();

  ReiiHandleUnorderedArray hinstanceColors = {0};
  ReiiHandleUnorderedArray * instanceColors = &hinstanceColors;
  instanceColors->texcoord[0] = instance_color_array;
  reiiCreateUnorderedArray(ctx, instanceColors);
  reiiUnorderedArraySet(ctx, instanceColors);
  for (int k = 0; k < instanceCountZ; k += 1) {
    for (int j = 0; j < instanceCountY; j += 1) {
      for (int i = 0; i < instanceCountX; i += 1) {
        reiiUnorderedArrayTexcoord(ctx, instanceColors, 0, RandomRange(0.15f, 0.45f), RandomRange(0.15f, 0.45f), RandomRange(0.25f, 1.f), 1);
      }
    }
  }
  reiiUnorderedArrayEnd(ctx, instanceColors, 1, &gpu_thread, array65536);

  ReiiHandleUnorderedArray hinstancePositions = {0};
  ReiiHandleUnorderedArray * instancePositions = &hinstancePositions;
  instancePositions->texcoord[0] = instance_position_array;
  reiiCreateUnorderedArray(ctx, instancePositions);
  reiiUnorderedArraySet(ctx, instancePositions);
  for (int k = 0; k < instanceCountZ; k += 1) {
    for (int j = 0; j < instanceCountY; j += 1) {
      for (int i = 0; i < instanceCountX; i += 1) {
        reiiUnorderedArrayTexcoord(ctx, instancePositions, 0, i * 3.f, j * 3.f, k * 3.f, RandomRange(0.05f, 0.45f));
      }
    }
  }
  reiiUnorderedArrayEnd(ctx, instancePositions, 1, &gpu_thread, array65536);

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
  list->dynamic_mesh_position          = mesh_vertex_array;

  ReiiGammaCorrectColorTextureToTheInversePowerOf2StaticState gammaCorrectionStaticState = {0};

  ReiiVec4   camera_pos  = {0, 0, 0};
  ReiiVec4   camera_quat = {0, 0, 0, 1};
  ReiiBool32 camera_is_enabled = 0;

  camera_pos.x = (instanceCountX * 3.f) / 2.f;
  camera_pos.y = (instanceCountY * 3.f) / 2.f;
  camera_pos.z = -25.f;

  if (camera_is_enabled == 1) {
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  }

  #ifdef VKFAST_EXTRA_INCLUDED_GLFW3_TO_SDL3
  glfwPollEvents(1, &window);
  #else
  glfwPollEvents();
  #endif
  double mouse_x = 0;
  double mouse_y = 0;
  glfwGetCursorPos(window, &mouse_x, &mouse_y);
  double mouse_x_prev = mouse_x;
  double mouse_y_prev = mouse_y;
  int    mouse_right_mouse_button_state_prev = 0;

  int previous_window_w = window_w;
  int previous_window_h = window_h;

  unsigned frame = 0;

  #define MILLISECONDS_ARRAY_MAX_CAPTURE_FRAMES_COUNT 600
  int   milliseconds_arrayIndex = 0;
  float milliseconds_maxTime = 0;
  float milliseconds[MILLISECONDS_ARRAY_MAX_CAPTURE_FRAMES_COUNT] = {0};
  int   milliseconds_isCapturing = 1;
  int   camera_animation_is_enabled = 1;
  int   milliseconds_clearOnWrap = 1;

  #if !defined(VKFAST_EXTRA_INCLUDED_GLFW3_TO_SDL3)
  // NOTE(Constantine):
  // https://github.com/glfw/glfw/issues/1308
  // You also may be interested in the power-efficient C++ version of this FPS limiter:
  // https://github.com/procedural/vkFast/commit/3dc58b8de91d64122859675c03e507ec1653e3f9
  int    fps_limiter_enabled    = 0;
  double fps_limiter_lasttime   = glfwGetTime();
  float  fps_limiter_target_fps = 60;
  #endif

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

    #if defined(_WIN32)
    LARGE_INTEGER t_start = {0};
    QueryPerformanceCounter(&t_start);
    #endif
    #if defined(__linux__) && !defined(__ANDROID__)
    struct timespec t_start;
    clock_gettime(CLOCK_REALTIME, &t_start);
    #endif

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

    imguiSetProcessInputsState(!camera_is_enabled);
    imguiNewFrame();

    int doPresentRebuild = 0;
    if (glfwGetKey(window, GLFW_KEY_Z)) {
      const char * window_title = "[vkFast] REII Present Stutter Catcher (VSync Off)";
      doPresentRebuild = vfWindowFullscreen(ctx, window_handle, window_title, window_w, window_h, 0, RED_PRESENT_VSYNC_MODE_OFF, FF, LL);
      glfwSetWindowTitle(window, window_title);
    }
    if (glfwGetKey(window, GLFW_KEY_X)) {
      const char * window_title = "[vkFast] REII Present Stutter Catcher (VSync On)";
      doPresentRebuild = vfWindowFullscreen(ctx, window_handle, window_title, window_w, window_h, 0, RED_PRESENT_VSYNC_MODE_ON, FF, LL);
      glfwSetWindowTitle(window, window_title);
    }

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
        reiiTextureSetStateMsaa(ctx, REII_TEXTURE_BINDING_2D, outputdstex, RED_MULTISAMPLE_COUNT_BITFLAG_4);
        reiiTextureSetStateMipmap(ctx, REII_TEXTURE_BINDING_2D, outputdstex, 0);
        reiiTextureSetStateMipmapLevelsCount(ctx, REII_TEXTURE_BINDING_2D, outputdstex, 1);
        reiiTextureDefineAndCopyFromCpu(ctx, REII_TEXTURE_BINDING_2D, outputdstex, 0, REII_TEXTURE_TEXEL_FORMAT_DS, window_w, window_h, REII_TEXTURE_TEXEL_FORMAT_DS, REII_TEXTURE_TEXEL_TYPE_FLOAT, 4, NULL, 1, &gpu_thread, array65536);

        reiiCreateTextureFromTextureMemory(ctx, &outputTexMemory, REII_TEXTURE_BINDING_2D, &houtputtex);
        reiiTextureSetStateMipmap(ctx, REII_TEXTURE_BINDING_2D, outputtex, 0);
        reiiTextureSetStateMipmapLevelsCount(ctx, REII_TEXTURE_BINDING_2D, outputtex, 1);
        reiiTextureDefineAndCopyFromCpu(ctx, REII_TEXTURE_BINDING_2D, outputtex, 0, REII_TEXTURE_TEXEL_FORMAT_RGBA, window_w, window_h, REII_TEXTURE_TEXEL_FORMAT_RGBA, REII_TEXTURE_TEXEL_TYPE_U8, 4, NULL, 1, &gpu_thread, array65536);

        reiiCreateTextureFromTextureMemory(ctx, &outputMSTexMemory, REII_TEXTURE_BINDING_2D, &houtputmstex);
        reiiTextureSetStateMsaa(ctx, REII_TEXTURE_BINDING_2D, outputmstex, RED_MULTISAMPLE_COUNT_BITFLAG_4);
        reiiTextureSetStateMipmap(ctx, REII_TEXTURE_BINDING_2D, outputmstex, 0);
        reiiTextureSetStateMipmapLevelsCount(ctx, REII_TEXTURE_BINDING_2D, outputmstex, 1);
        reiiTextureDefineAndCopyFromCpu(ctx, REII_TEXTURE_BINDING_2D, outputmstex, 0, REII_TEXTURE_TEXEL_FORMAT_RGBA, window_w, window_h, REII_TEXTURE_TEXEL_FORMAT_RGBA, REII_TEXTURE_TEXEL_TYPE_U8, 4, NULL, 1, &gpu_thread, array65536);
      }

      previous_window_w = window_w;
      previous_window_h = window_h;
    }

    glfwGetCursorPos(window, &mouse_x, &mouse_y);

    int mouse_right_mouse_button_state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2);

    // NOTE(Constantine):
    // Camera quaternion rotation and translation.
    const float mouse_move_sensitivity = 0.0035f;
    const float camera_move_speed      = 0.5f;
    if (mouse_right_mouse_button_state == GLFW_PRESS && mouse_right_mouse_button_state != mouse_right_mouse_button_state_prev) {
      camera_animation_is_enabled = 0;
      camera_is_enabled = !camera_is_enabled;
      glfwSetInputMode(window, GLFW_CURSOR, camera_is_enabled == 1 ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
    } else if (camera_is_enabled == 1) {
      float mouse_move_x = (float)(mouse_x - mouse_x_prev) * mouse_move_sensitivity;
      float mouse_move_y = (float)(mouse_y - mouse_y_prev) * mouse_move_sensitivity;

      float key_f = glfwGetKey(window, GLFW_KEY_W);
      float key_b = glfwGetKey(window, GLFW_KEY_S);

      float key_r = glfwGetKey(window, GLFW_KEY_D);
      float key_l = glfwGetKey(window, GLFW_KEY_A);

      float key_u = glfwGetKey(window, GLFW_KEY_E);
      float key_d = glfwGetKey(window, GLFW_KEY_Q);

      float rot_x[4];
      float rot_y[4];
  
      float axis_x[3] = {1, 0, 0};
      float axis_y[3] = {0, 1, 0};
      quatFromAxisAngle(rot_y, axis_y, mouse_move_x);
      quatFromAxisAngle(rot_x, axis_x, mouse_move_y);
  
      quatMul(&camera_quat.x, &camera_quat.x, rot_x);
      quatMul(&camera_quat.x, rot_y, &camera_quat.x);

      float side_vec[3] = {1, 0, 0};
      float   up_vec[3] = {0, 1, 0};
      float  dir_vec[3] = {0, 0, 1};
      quatRotateVec3Fast(side_vec, side_vec, &camera_quat.x);
      quatRotateVec3Fast(  up_vec,   up_vec, &camera_quat.x);
      quatRotateVec3Fast( dir_vec,  dir_vec, &camera_quat.x);

      vec3Mulf(side_vec, side_vec, key_r - key_l);
      vec3Mulf(  up_vec,   up_vec, key_u - key_d);
      vec3Mulf( dir_vec,  dir_vec, key_f - key_b);
  
      float move_vec_normalized[3] = {0, 0, 0};

      vec3Add(move_vec_normalized, move_vec_normalized, side_vec);
      vec3Add(move_vec_normalized, move_vec_normalized,   up_vec);
      vec3Add(move_vec_normalized, move_vec_normalized,  dir_vec);

      float move_vec_len = sqrtf(
        move_vec_normalized[0] * move_vec_normalized[0] +
        move_vec_normalized[1] * move_vec_normalized[1] +
        move_vec_normalized[2] * move_vec_normalized[2]
      );
      if (move_vec_len != 0) {
        move_vec_normalized[0] /= move_vec_len;
        move_vec_normalized[1] /= move_vec_len;
        move_vec_normalized[2] /= move_vec_len;
      }

      vec3Mulf(move_vec_normalized, move_vec_normalized, camera_move_speed);

      vec3Add(&camera_pos.x, &camera_pos.x, move_vec_normalized);
    }

    static float disabled_camera_ping_pong = 0.45f;
    if (camera_animation_is_enabled == 1) {
      camera_pos.x += disabled_camera_ping_pong;
      if (camera_pos.x < -5.f || camera_pos.x > ((instanceCountX * 3.f) + 5.f)) {
        disabled_camera_ping_pong *= -1.f;
      }
    }

    // Present Stutter Catcher UI
    {
      enum ImGuiCol_
      {
          ImGuiCol_Text,
          ImGuiCol_TextDisabled,
          ImGuiCol_WindowBg,              // Background of normal windows
          ImGuiCol_ChildWindowBg,         // Background of child windows
          ImGuiCol_PopupBg,               // Background of popups, menus, tooltips windows
          ImGuiCol_Border,
          ImGuiCol_BorderShadow,
          ImGuiCol_FrameBg,               // Background of checkbox, radio button, plot, slider, text input
          ImGuiCol_FrameBgHovered,
          ImGuiCol_FrameBgActive,
          ImGuiCol_TitleBg,
          ImGuiCol_TitleBgCollapsed,
          ImGuiCol_TitleBgActive,
          ImGuiCol_MenuBarBg,
          ImGuiCol_ScrollbarBg,
          ImGuiCol_ScrollbarGrab,
          ImGuiCol_ScrollbarGrabHovered,
          ImGuiCol_ScrollbarGrabActive,
          ImGuiCol_ComboBg,
          ImGuiCol_CheckMark,
          ImGuiCol_SliderGrab,
          ImGuiCol_SliderGrabActive,
          ImGuiCol_Button,
          ImGuiCol_ButtonHovered,
          ImGuiCol_ButtonActive,
          ImGuiCol_Header,
          ImGuiCol_HeaderHovered,
          ImGuiCol_HeaderActive,
          ImGuiCol_Column,
          ImGuiCol_ColumnHovered,
          ImGuiCol_ColumnActive,
          ImGuiCol_ResizeGrip,
          ImGuiCol_ResizeGripHovered,
          ImGuiCol_ResizeGripActive,
          ImGuiCol_CloseButton,
          ImGuiCol_CloseButtonHovered,
          ImGuiCol_CloseButtonActive,
          ImGuiCol_PlotLines,
          ImGuiCol_PlotLinesHovered,
          ImGuiCol_PlotHistogram,
          ImGuiCol_PlotHistogramHovered,
          ImGuiCol_TextSelectedBg,
          ImGuiCol_ModalWindowDarkening,  // darken entire screen when a modal window is active
          ImGuiCol_COUNT
      };

      vf_handle_context_t * context = (vf_handle_context_t *)ctx;
      igText("GPU name: %s", context->gpuInfo->gpuName);
      igCheckbox("Capture frame times", (bool *)&milliseconds_isCapturing);
      if (igCheckbox("Camera animation", (bool *)&camera_animation_is_enabled)) {
        if (camera_animation_is_enabled == 1) {
          if (camera_pos.x < -5.f) {
            camera_pos.x = -5.f;
          }
          if (camera_pos.x > ((instanceCountX * 3.f) + 5.f)) {
            camera_pos.x = (instanceCountX * 3.f) + 5.f;
          }
        }
      }
      igCheckbox("Clear all inserted frames on insert index wrap", (bool *)&milliseconds_clearOnWrap);
      igText("Next frame insert index: %d", milliseconds_arrayIndex);
      if (igButton("Clear all inserted frames", REDGPU_32_STRUCT(ImVec2, 0, 0))) {
        milliseconds_maxTime = 0;
        for (int i = 0; i < MILLISECONDS_ARRAY_MAX_CAPTURE_FRAMES_COUNT; i += 1) {
          milliseconds[i] = 0;
        }
      }
      style->colors[ImGuiCol_PlotHistogramHovered] = REDGPU_32_STRUCT(ImVec4, 1.f, 0.f, 0.f, 1.f);
      ImVec2 graph_size = {0, 80};
      igPlotHistogram("Frame times", milliseconds, MILLISECONDS_ARRAY_MAX_CAPTURE_FRAMES_COUNT, 0, NULL, 0, milliseconds_maxTime, graph_size, 4);

      #if !defined(VKFAST_EXTRA_INCLUDED_GLFW3_TO_SDL3)
      igCheckbox("Enable FPS limiter", (bool *)&fps_limiter_enabled);
      if (igDragFloat("FPS limiter target FPS", &fps_limiter_target_fps, 1, 0, INT_MAX, 0, 1)) {
        if (fps_limiter_target_fps < 15) { fps_limiter_target_fps = 15; }
      }
      #endif
      igText("To disable VSync, press Z key.");
      igText("To enable  VSync, press X key.");
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
    bindings_info.max_new_bindings_sets_count = 2;
    bindings_info.max_storage_binds_count     = 3;
    bindings_info.max_texture_rw_binds_count  = 1;
    batch = vfBatchBegin(ctx, batch, &bindings_info, NULL, FF, LL);
    list->batch_id = batch;
    reiiCommandListReset(ctx, list);
    reiiCommandSetViewportEx(ctx, list, 0, 0, window_w, window_h, 0, 1);
    reiiCommandSetScissor(ctx, list, 0, 0, window_w, window_h);
    reiiCommandClearTexture(ctx, list, outputdstex, outputmstex, outputmstex->texture, REII_CLEAR_DEPTH_BIT | REII_CLEAR_COLOR_BIT, 0.f, 0, 0.f,0.f,0.05f,1.f);
    reiiCommandMeshSetState(ctx, list, &mesh_state, NULL);
    reiiCommandBindNewBindingsSet(ctx, list, countof(slots), slots);
    reiiCommandBindStorageRaw(ctx, list, 0, 1, &mesh->position.gpu);
    reiiCommandBindStorageRaw(ctx, list, 1, 1, &instanceColors->texcoord[0].gpu);
    reiiCommandBindStorageRaw(ctx, list, 2, 1, &instancePositions->texcoord[0].gpu);
    reiiCommandBindNewBindingsEnd(ctx, list);
    reiiCommandBindVariablesCopy(ctx, list, 0 * sizeof(ReiiVec4), 1 * sizeof(ReiiVec4), &camera_pos);
    reiiCommandBindVariablesCopy(ctx, list, 1 * sizeof(ReiiVec4), 1 * sizeof(ReiiVec4), &camera_quat);
    reiiCommandRenderTargetSet(ctx, list, outputdstex, outputmstex, outputmstex->texture);
    reiiCommandUnorderedArrayDrawInstanced(ctx, list, mesh, instanceCountX * instanceCountY * instanceCountZ);
    reiiCommandRenderTargetEnd(ctx, list);
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

    mouse_x_prev = mouse_x;
    mouse_y_prev = mouse_y;
    mouse_right_mouse_button_state_prev = mouse_right_mouse_button_state;

    frame += 1;

    #if !defined(VKFAST_EXTRA_INCLUDED_GLFW3_TO_SDL3)
    if (fps_limiter_enabled == 1) {
      while (glfwGetTime() < fps_limiter_lasttime + 1.0f / fps_limiter_target_fps) {
        #if defined(_WIN32)
        YieldProcessor();
        #elif defined(__linux__) && !defined(__ANDROID__)
        __builtin_ia32_pause();
        #else
        #error Unsupported OS for now
        #endif
      }
      fps_limiter_lasttime += 1.0f / fps_limiter_target_fps;
    } else {
      fps_limiter_lasttime = glfwGetTime();
    }
    #endif

    #if defined(_WIN32)
    LARGE_INTEGER t_end = {0};
    QueryPerformanceCounter(&t_end);
    #endif
    #if defined(__linux__) && !defined(__ANDROID__)
    struct timespec t_end;
    clock_gettime(CLOCK_REALTIME, &t_end);
    #endif

    if (milliseconds_isCapturing == 1) {
      #if defined(_WIN32)
      LONGLONG elapsedTicks = t_end.QuadPart - t_start.QuadPart;
      LONGLONG nanoseconds = (elapsedTicks * 1000000000LL) / frequency.QuadPart;
      #endif
      #if defined(__linux__) && !defined(__ANDROID__)
      long seconds = 0;

      long t_start_ns = 0;
      seconds = t_start.tv_sec;
      {
        long seconds_as_milliseconds = seconds                 * 1000;
        long seconds_as_microseconds = seconds_as_milliseconds * 1000;
        long seconds_as_nanoseconds  = seconds_as_microseconds * 1000;
        t_start_ns = seconds_as_nanoseconds;
      }
      t_start_ns += t_start.tv_nsec;

      long t_end_ns = 0;
      seconds = t_end.tv_sec;
      {
        long seconds_as_milliseconds = seconds                 * 1000;
        long seconds_as_microseconds = seconds_as_milliseconds * 1000;
        long seconds_as_nanoseconds  = seconds_as_microseconds * 1000;
        t_end_ns = seconds_as_nanoseconds;
      }
      t_end_ns += t_end.tv_nsec;

      long nanoseconds = t_end_ns - t_start_ns;
      #endif
      double milliseconds_fp = (double)(nanoseconds) / 1000000.0;
      //printf("Elapsed milliseconds: %f\n", milliseconds_fp);

      {
        if (frame > 1 && milliseconds_maxTime < (float)milliseconds_fp) {
          milliseconds_maxTime = (float)milliseconds_fp;
        }
        milliseconds[milliseconds_arrayIndex] = (float)milliseconds_fp;
        milliseconds_arrayIndex += 1;
        milliseconds_arrayIndex %= MILLISECONDS_ARRAY_MAX_CAPTURE_FRAMES_COUNT;

        if (milliseconds_clearOnWrap == 1 && milliseconds_arrayIndex == 0) {
          milliseconds_maxTime = 0;
          for (int i = 0; i < MILLISECONDS_ARRAY_MAX_CAPTURE_FRAMES_COUNT; i += 1) {
            milliseconds[i] = 0;
          }
        }
      }
    }
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
  reiiDestroyEx(ctx, GPU_EXTRA_REII_DESTROY_TYPE_MESH_STATE, &mesh_state);
  uint64_t ids[] = {
    batch,
    storage_gpu_only.id,
    storage_cpu_upload.id,
    storage_cpu_readback.id,
  };
  vfIdDestroy(countof(ids), ids, FF, LL);
  vfContextDeinit(ctx, FF, LL);
  #ifdef VKFAST_EXTRA_INCLUDED_GLFW3_TO_SDL3
  glfwTerminateWindow(window);
  #endif
  glfwTerminate();
}

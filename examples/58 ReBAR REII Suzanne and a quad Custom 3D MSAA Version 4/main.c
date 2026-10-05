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
//\\rc rawbuild ` main.c ../../vkfast.c ../../extra/Banzai/vkfast_extra_banzai.c ../../extra/Banzai/vkfast_extra_banzai_pointer.c "../../extra/CPU GPU Array/vkfast_extra_cpu_gpu_array.c" ../../extra/REII/vkfast_extra_reii.c /home/linuxbrew/RedGpuSDK/redgpu.c /home/linuxbrew/RedGpuSDK/redgpu_2.c /home/linuxbrew/RedGpuSDK/redgpu_32.c -I/home/linuxbrew/.linuxbrew/include/ -I/home/linuxbrew/.linuxbrew/Cellar/xorgproto/2025.1/include/ -I/var/home/linuxbrew/.linuxbrew/Cellar/libxcb/1.17.0/include/ /home/linuxbrew/.linuxbrew/Cellar/glfw/3.5.1/lib/libglfw3.a /home/linuxbrew/.linuxbrew/lib/libX11.so /home/linuxbrew/.linuxbrew/lib/libvulkan.so libimgui.so -lm`
//\\rc rawbuild end

#define VKFAST_EXAMPLES_COMMON_INCLUDE_GLFW3
#define VKFAST_EXAMPLES_COMMON_INCLUDE_EXTRA_BANZAI
#include "../Common/vkfast_examples_common.h"

#include "../../extra/vkFast Extensions/I dont care/idc_vkfast_next.h"

typedef ReiiVec4 float4;

typedef struct float2 {
  float x, y;
} float2;

#include "shared_data.h"

#ifdef __linux__
#define fopen_s(pFile, filename, mode) (*(pFile) = fopen((filename), (mode)))
#endif

int main() {
  gpu_program_pipeline_info_t * ppi = NULL;

  gpu_program_pipeline_info_t ppiMeshState        = {0};
  RedStructDeclarationMember  ppiMeshStateSlots[] = {
    {
      /*slot*/  0,
      /*type*/  RED_STRUCT_MEMBER_TYPE_ARRAY_RO_RW,
      /*count*/ 1,
      /*visib*/ RED_VISIBLE_TO_STAGE_BITFLAG_VERTEX | RED_VISIBLE_TO_STAGE_BITFLAG_FRAGMENT,
      /*sampl*/ 0,
    },
    {
      /*slot*/  1,
      /*type*/  RED_STRUCT_MEMBER_TYPE_TEXTURE_RO,
      /*count*/ 1,
      /*visib*/ RED_VISIBLE_TO_STAGE_BITFLAG_FRAGMENT,
      /*sampl*/ 0,
    },
  };
  ppi                        = &ppiMeshState;
  ppi->struct_members_count  = countof(ppiMeshStateSlots);
  ppi->struct_members        = ppiMeshStateSlots;
  ppi->variables_slot        = 2;
  ppi->variables_bytes_count = sizeof(struct Variables);

  gpu_program_pipeline_info_t ppiCustomMsaaResolve = {0};
  RedStructDeclarationMember  ppiCustomMsaaResolveSlots[] = {
    {
      /*slot*/  0,
      /*type*/  RED_STRUCT_MEMBER_TYPE_ARRAY_RO_RW,
      /*count*/ 1,
      /*visib*/ RED_VISIBLE_TO_STAGE_BITFLAG_COMPUTE,
      /*sampl*/ 0,
    },
    {
      /*slot*/  1,
      /*type*/  RED_STRUCT_MEMBER_TYPE_TEXTURE_RW,
      /*count*/ 1,
      /*visib*/ RED_VISIBLE_TO_STAGE_BITFLAG_COMPUTE,
      /*sampl*/ 0,
    },
  };
  ppi                        = &ppiCustomMsaaResolve;
  ppi->struct_members_count  = countof(ppiCustomMsaaResolveSlots);
  ppi->struct_members        = ppiCustomMsaaResolveSlots;
  ppi->variables_slot        = 2;
  ppi->variables_bytes_count = sizeof(struct Variables);

  int window_w = 700; // NOTE(Constantine): For this example, the texture sizes, including the one in shared_data.h, are hardcoded to this initial window size of 700x700.
  int window_h = 700;

  #define APP_AND_FOLDER_NAME     "58 ReBAR REII Suzanne and a quad Custom 3D MSAA Version 4"
  #ifdef _WIN32
  #define APP_AND_FOLDER_NAME_WS L"58 ReBAR REII Suzanne and a quad Custom 3D MSAA Version 4"
  #endif

  glfwInit();
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  GLFWwindow * window = glfwCreateWindow(window_w, window_h, "[vkFast] " APP_AND_FOLDER_NAME, 0, 0);
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

  // NOTE(Constantine): You can also define REDGPU_COMPILE_SWITCH_DEBUG to see extra errors.
  gpu_handle_context_t ctx = vfContextInit(1, NULL, FF, LL);
  vfWindowFullscreen(ctx, window_handle, "[vkFast] " APP_AND_FOLDER_NAME, window_w, window_h, 0, RED_PRESENT_VSYNC_MODE_ON, FF, LL);

  const unsigned array65536[2] = {65536, 65536};

  gpu_thread_t gpu_thread = NULL;
  vfGpuThreadCreate(ctx, 1, &gpu_thread, NULL, FF, LL);

  ReiiMeshState mesh_state                                  = {0};
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
  mesh_state.codeVertex                                     = NULL;
  mesh_state.codeFragment                                   = NULL;
  mesh_state.extension                                      = NULL;
  mesh_state.programVertex.program_binary_bytes_count       = 0;
  mesh_state.programVertex.program_binary                   = NULL;
  mesh_state.programFragment.program_binary_bytes_count     = 0;
  mesh_state.programFragment.program_binary                 = NULL;
  mesh_state.compileInfo.state_multisample_count            = RED_MULTISAMPLE_COUNT_BITFLAG_1;
  mesh_state.compileInfo.output_depth_stencil_enable        = 1;
  mesh_state.compileInfo.output_depth_stencil_format        = RED_FORMAT_DEPTH_32_FLOAT;
  mesh_state.compileInfo.output_color_format                = RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1;
  mesh_state.programPipelineInfo                            = ppiMeshState;
  mesh_state.programPipelineInfoSamplersCount               = 1;

#ifdef _WIN32
  #define MY_DXC_PATH        "C:/dxc/dxc.exe"
  #define MY_VKFAST_PATH     "C:/vkfast/"
  #define MY_VKFAST_PATH_WS L"C:/vkfast/"
#else
  #define MY_DXC_PATH        "/opt/dxc"
  #define MY_VKFAST_PATH     "/home/constantine/Desktop/vkfast/"
#endif

  mesh_state.compileCommandVS =
    MY_DXC_PATH
    " \"" MY_VKFAST_PATH "examples/" APP_AND_FOLDER_NAME "/mesh.hlsl\""
    " -DVS -T vs_6_0 -Fo"
    " \"" MY_VKFAST_PATH "examples/" APP_AND_FOLDER_NAME "/mesh.vs.spv\""
    " -spirv";
  mesh_state.compileCommandFS =
    MY_DXC_PATH
    " \"" MY_VKFAST_PATH "examples/" APP_AND_FOLDER_NAME "/mesh.hlsl\""
    " -DFS -T ps_6_0 -Fo"
    " \"" MY_VKFAST_PATH "examples/" APP_AND_FOLDER_NAME "/mesh.fs.spv\""
    " -spirv";
#ifdef _WIN32
  mesh_state.compiledSpvFilepathVS = MY_VKFAST_PATH_WS L"examples/" APP_AND_FOLDER_NAME_WS "/mesh.vs.spv";
  mesh_state.compiledSpvFilepathFS = MY_VKFAST_PATH_WS L"examples/" APP_AND_FOLDER_NAME_WS "/mesh.fs.spv";
#else
  mesh_state.compiledSpvFilepathVS = MY_VKFAST_PATH "examples/" APP_AND_FOLDER_NAME "/mesh.vs.spv";
  mesh_state.compiledSpvFilepathFS = MY_VKFAST_PATH "examples/" APP_AND_FOLDER_NAME "/mesh.fs.spv";
#endif

  ReiiMeshState quad_mesh_state = mesh_state;
  quad_mesh_state.compileCommandVS =
    MY_DXC_PATH
    " \"" MY_VKFAST_PATH "examples/" APP_AND_FOLDER_NAME "/quad_mesh.hlsl\""
    " -DVS -T vs_6_0 -Fo"
    " \"" MY_VKFAST_PATH "examples/" APP_AND_FOLDER_NAME "/quad_mesh.vs.spv\""
    " -spirv";
  quad_mesh_state.compileCommandFS =
    MY_DXC_PATH
    " \"" MY_VKFAST_PATH "examples/" APP_AND_FOLDER_NAME "/quad_mesh.hlsl\""
    " -DFS -T ps_6_0 -Fo"
    " \"" MY_VKFAST_PATH "examples/" APP_AND_FOLDER_NAME "/quad_mesh.fs.spv\""
    " -spirv";
#ifdef _WIN32
  quad_mesh_state.compiledSpvFilepathVS = MY_VKFAST_PATH_WS L"examples/" APP_AND_FOLDER_NAME_WS "/quad_mesh.vs.spv";
  quad_mesh_state.compiledSpvFilepathFS = MY_VKFAST_PATH_WS L"examples/" APP_AND_FOLDER_NAME_WS "/quad_mesh.fs.spv";
#else
  quad_mesh_state.compiledSpvFilepathVS = MY_VKFAST_PATH "examples/" APP_AND_FOLDER_NAME "/quad_mesh.vs.spv";
  quad_mesh_state.compiledSpvFilepathFS = MY_VKFAST_PATH "examples/" APP_AND_FOLDER_NAME "/quad_mesh.fs.spv";
#endif

  reiiMeshStateCompileEx(ctx, &mesh_state);
  reiiMeshStateCompileEx(ctx, &quad_mesh_state);

  ReiiHandleTextureMemory outputDSTexMemory = {0};
  reiiCreateTextureMemory(ctx, GPU_EXTRA_REII_TEXTURE_TYPE_OUTPUT_DEPTH_STENCIL, (288/*mb*/ * 1024 * 1024), &outputDSTexMemory);
  ReiiHandleTexture houtputdstex = {0};
  ReiiHandleTexture * outputdstex = &houtputdstex;
  reiiCreateTextureFromTextureMemory(ctx, &outputDSTexMemory, REII_TEXTURE_BINDING_2D, &houtputdstex);
  reiiTextureSetStateMipmap(ctx, REII_TEXTURE_BINDING_2D, outputdstex, 0);
  reiiTextureSetStateMipmapLevelsCount(ctx, REII_TEXTURE_BINDING_2D, outputdstex, 1);
  reiiTextureDefineAndCopyFromCpu(ctx, REII_TEXTURE_BINDING_2D, outputdstex, 0, REII_TEXTURE_TEXEL_FORMAT_DS, window_w, window_h, REII_TEXTURE_TEXEL_FORMAT_DS, REII_TEXTURE_TEXEL_TYPE_FLOAT, 4, NULL, 1, &gpu_thread, array65536);

  ReiiHandleTextureMemory outputColTexMemory = {0};
  reiiCreateTextureMemory(ctx, GPU_EXTRA_REII_TEXTURE_TYPE_OUTPUT_COLOR, (288/*mb*/ * 1024 * 1024), &outputColTexMemory);
  ReiiHandleTexture houtputcoltex = {0};
  ReiiHandleTexture * outputcoltex = &houtputcoltex;
  reiiCreateTextureFromTextureMemory(ctx, &outputColTexMemory, REII_TEXTURE_BINDING_2D, &houtputcoltex);
  reiiTextureSetStateMipmap(ctx, REII_TEXTURE_BINDING_2D, outputcoltex, 0);
  reiiTextureSetStateMipmapLevelsCount(ctx, REII_TEXTURE_BINDING_2D, outputcoltex, 1);
  reiiTextureDefineAndCopyFromCpu(ctx, REII_TEXTURE_BINDING_2D, outputcoltex, 0, REII_TEXTURE_TEXEL_FORMAT_RGBA, window_w, window_h, REII_TEXTURE_TEXEL_FORMAT_RGBA, REII_TEXTURE_TEXEL_TYPE_U8, 4, NULL, 1, &gpu_thread, array65536);

  void * idcimgui = idcImguiInit(ctx, window, &gpu_thread, outputcoltex);

  // Create scratch memory for Suzanne texture uploads
  VfeReBARMallocShared scratchMemoryHandles = {0};
  void * scratchMemory = vfeReBARMallocShared(ctx, 64/*mb*/ * 1024 * 1024, &scratchMemoryHandles);
  ReiiCpuScratchBuffer scratchBuffer = {0};
  scratchBuffer.cpu_scratch_buffer_ptr = scratchMemory;
  scratchBuffer.cpu_scratch_buffer     = scratchMemoryHandles.storageRaw;

  // Create and load a sampler and a texture for Suzanne
  RedHandleSampler sampler = NULL;
  ReiiHandleTextureMemory texturesMemory = {0};
  ReiiHandleTexture htextureHandle = {0};
  ReiiHandleTexture * textureHandle = &htextureHandle;
  {
    sampler = reiiCreateSampler(ctx, NULL, REII_SAMPLER_FILTERING_LINEAR, REII_SAMPLER_FILTERING_LINEAR, REII_SAMPLER_BEHAVIOR_OUTSIDE_TEXTURE_COORDINATE_REPEAT, REII_SAMPLER_BEHAVIOR_OUTSIDE_TEXTURE_COORDINATE_REPEAT, 1);

    reiiCreateTextureMemory(ctx, GPU_EXTRA_REII_TEXTURE_TYPE_GENERAL, (64/*mb*/ * 1024 * 1024), &texturesMemory);

    FILE * image_file_fd = NULL;
    fopen_s(&image_file_fd, "texture.png",  "rb");
    REDGPU_2_EXPECTFL(image_file_fd != NULL || !"You need to have a 'texture.png' file with an alpha channel in this example's folder.");
    int image_w = 0;
    int image_h = 0;
    int image_channels_count = 0;
    void * image = stbi_load_from_file(image_file_fd, &image_w, &image_h, &image_channels_count, STBI_rgb_alpha);
    REDGPU_2_EXPECTFL(image != NULL);
    REDGPU_2_EXPECTFL(image_channels_count == 4);

    reiiCreateTextureFromTextureMemory(ctx, &texturesMemory, REII_TEXTURE_BINDING_2D, &htextureHandle);
    reiiTextureSetStateMipmap(ctx, REII_TEXTURE_BINDING_2D, textureHandle, 0);
    reiiTextureSetStateMipmapLevelsCount(ctx, REII_TEXTURE_BINDING_2D, textureHandle, 1);
    memcpy(scratchBuffer.cpu_scratch_buffer_ptr, image, image_w * image_h * image_channels_count);
    reiiTextureDefineAndCopyFromCpu(ctx, REII_TEXTURE_BINDING_2D, textureHandle,
      0,
      REII_TEXTURE_TEXEL_FORMAT_RGBA,
      image_w,
      image_h,
      REII_TEXTURE_TEXEL_FORMAT_RGBA,
      REII_TEXTURE_TEXEL_TYPE_U8,
      4,
      &scratchBuffer,
      1, &gpu_thread, array65536
    );

    stbi_image_free(image);
    image = NULL;
    fclose(image_file_fd);
    image_file_fd = NULL;
  }

  float suzanne_head_mesh_vertices[] = {
    #include "../../extra/3D Mesh Suzanne Head/3d_mesh_vertices_suzanne_head.h"
  };

  VfeReBARMallocShared sharedDataHandles = {0};
  volatile struct SharedData * sharedData = (volatile struct SharedData *)vfeReBARMallocShared(ctx, sizeof(struct SharedData), &sharedDataHandles);

  // Set Suzanne Head mesh data
  for (int i = 0; i < 2904; i += 1) {
    const float scale = 0.5f;
    sharedData[0].meshSuzanneHeadVertexPos[i].x = suzanne_head_mesh_vertices[i * 3 + 0] * scale;
    sharedData[0].meshSuzanneHeadVertexPos[i].y = suzanne_head_mesh_vertices[i * 3 + 1] * scale;
    sharedData[0].meshSuzanneHeadVertexPos[i].z = suzanne_head_mesh_vertices[i * 3 + 2] * scale;
    sharedData[0].meshSuzanneHeadVertexPos[i].w = 1;

    sharedData[0].meshSuzanneHeadVertexCol[i].x = i * 0.00025f;
    sharedData[0].meshSuzanneHeadVertexCol[i].y = 0;
    sharedData[0].meshSuzanneHeadVertexCol[i].z = 0.1f;
    sharedData[0].meshSuzanneHeadVertexCol[i].w = 1;
  }

  // Set Quad mesh data
  {
    // Vertex positions

    float pos_scale = 5;
    float pos_y     = -1;

    sharedData[0].meshQuadVertexPos[0].x =-pos_scale;
    sharedData[0].meshQuadVertexPos[0].y = pos_y;
    sharedData[0].meshQuadVertexPos[0].z =-pos_scale;
    sharedData[0].meshQuadVertexPos[0].w = 1;

    sharedData[0].meshQuadVertexPos[1].x = pos_scale;
    sharedData[0].meshQuadVertexPos[1].y = pos_y;
    sharedData[0].meshQuadVertexPos[1].z =-pos_scale;
    sharedData[0].meshQuadVertexPos[1].w = 1;

    sharedData[0].meshQuadVertexPos[2].x = pos_scale;
    sharedData[0].meshQuadVertexPos[2].y = pos_y;
    sharedData[0].meshQuadVertexPos[2].z = pos_scale;
    sharedData[0].meshQuadVertexPos[2].w = 1;

    sharedData[0].meshQuadVertexPos[3].x =-pos_scale;
    sharedData[0].meshQuadVertexPos[3].y = pos_y;
    sharedData[0].meshQuadVertexPos[3].z =-pos_scale;
    sharedData[0].meshQuadVertexPos[3].w = 1;

    sharedData[0].meshQuadVertexPos[4].x = pos_scale;
    sharedData[0].meshQuadVertexPos[4].y = pos_y;
    sharedData[0].meshQuadVertexPos[4].z = pos_scale;
    sharedData[0].meshQuadVertexPos[4].w = 1;

    sharedData[0].meshQuadVertexPos[5].x =-pos_scale;
    sharedData[0].meshQuadVertexPos[5].y = pos_y;
    sharedData[0].meshQuadVertexPos[5].z = pos_scale;
    sharedData[0].meshQuadVertexPos[5].w = 1;

    // Vertex UVs

    sharedData[0].meshQuadVertexUVs[0].x = 0;
    sharedData[0].meshQuadVertexUVs[0].y = 1;

    sharedData[0].meshQuadVertexUVs[1].x = 1;
    sharedData[0].meshQuadVertexUVs[1].y = 1;

    sharedData[0].meshQuadVertexUVs[2].x = 1;
    sharedData[0].meshQuadVertexUVs[2].y = 0;

    sharedData[0].meshQuadVertexUVs[3].x = 0;
    sharedData[0].meshQuadVertexUVs[3].y = 1;

    sharedData[0].meshQuadVertexUVs[4].x = 1;
    sharedData[0].meshQuadVertexUVs[4].y = 0;

    sharedData[0].meshQuadVertexUVs[5].x = 0;
    sharedData[0].meshQuadVertexUVs[5].y = 0;
  }

  // NOTE(Constantine): Custom 3D MSAA
  // Based on 2D MSAA positions from "19.2.4 Specification of Sample Positions":
  // https://microsoft.github.io/DirectX-Specs/d3d/archive/D3D11_3_FunctionalSpec.htm#19.2.4%20Specification%20of%20Sample%20Positions
  float msaa_offsets_table_x1_x[1] = {0};
  float msaa_offsets_table_x1_y[1] = {0};
  float msaa_offsets_table_x1_z[1] = {0};
  float msaa_offsets_table_x2_x[2] = { 4, -4};
  float msaa_offsets_table_x2_y[2] = { 4, -4};
  float msaa_offsets_table_x2_z[2] = {-4,  4};
  float msaa_offsets_table_x4_x[4] = {-2,  6, -6,  2};
  float msaa_offsets_table_x4_y[4] = {-6, -2,  2,  6};
  float msaa_offsets_table_x4_z[4] = { 2, -6,  6, -2};
  float msaa_offsets_table_x8_x[8] = { 1, -1,  5, -3, -5, -7,  3,  7};
  float msaa_offsets_table_x8_y[8] = {-3,  3,  1, -5,  5, -1,  7, -7};
  float msaa_offsets_table_x8_z[8] = { 5, -5, -3,  1,  7,  3, -7, -1};
  float msaa_offsets_table_x16_x[16] = { 1, -1, -3,  4, -5,  2,  5,  3, -2,  0, -4, -6, -8,  7,  6, -7};
  float msaa_offsets_table_x16_y[16] = { 1, -3,  2, -1, -2,  5,  3, -5,  6, -7, -6,  4,  0, -4,  7, -8};
  float msaa_offsets_table_x16_z[16] = {-5,  4,  1, -3,  6, -2, -7,  2,  0,  5,  7, -4, -8,  3, -6, -1};
  // Converting to a -0.5 to 0.5 range:
  for (int i = 0; i < 2;  i += 1) { msaa_offsets_table_x2_x[i]  /= 16.f; }
  for (int i = 0; i < 2;  i += 1) { msaa_offsets_table_x2_y[i]  /= 16.f; }
  for (int i = 0; i < 2;  i += 1) { msaa_offsets_table_x2_z[i]  /= 16.f; }
  for (int i = 0; i < 4;  i += 1) { msaa_offsets_table_x4_x[i]  /= 16.f; }
  for (int i = 0; i < 4;  i += 1) { msaa_offsets_table_x4_y[i]  /= 16.f; }
  for (int i = 0; i < 4;  i += 1) { msaa_offsets_table_x4_z[i]  /= 16.f; }
  for (int i = 0; i < 8;  i += 1) { msaa_offsets_table_x8_x[i]  /= 16.f; }
  for (int i = 0; i < 8;  i += 1) { msaa_offsets_table_x8_y[i]  /= 16.f; }
  for (int i = 0; i < 8;  i += 1) { msaa_offsets_table_x8_z[i]  /= 16.f; }
  for (int i = 0; i < 16; i += 1) { msaa_offsets_table_x16_x[i] /= 16.f; }
  for (int i = 0; i < 16; i += 1) { msaa_offsets_table_x16_y[i] /= 16.f; }
  for (int i = 0; i < 16; i += 1) { msaa_offsets_table_x16_z[i] /= 16.f; }

  int msaaSamplesCount = 16;

  float * msaa_samples_offset_table_x = NULL;
  float * msaa_samples_offset_table_y = NULL;
  float * msaa_samples_offset_table_z = NULL;

  if (msaaSamplesCount == 1) {
    msaa_samples_offset_table_x = &msaa_offsets_table_x1_x[0];
    msaa_samples_offset_table_y = &msaa_offsets_table_x1_y[0];
    msaa_samples_offset_table_z = &msaa_offsets_table_x1_z[0];
  } else if (msaaSamplesCount == 2) {
    msaa_samples_offset_table_x = &msaa_offsets_table_x2_x[0];
    msaa_samples_offset_table_y = &msaa_offsets_table_x2_y[0];
    msaa_samples_offset_table_z = &msaa_offsets_table_x2_z[0];
  } else if (msaaSamplesCount == 4) {
    msaa_samples_offset_table_x = &msaa_offsets_table_x4_x[0];
    msaa_samples_offset_table_y = &msaa_offsets_table_x4_y[0];
    msaa_samples_offset_table_z = &msaa_offsets_table_x4_z[0];
  } else if (msaaSamplesCount == 8) {
    msaa_samples_offset_table_x = &msaa_offsets_table_x8_x[0];
    msaa_samples_offset_table_y = &msaa_offsets_table_x8_y[0];
    msaa_samples_offset_table_z = &msaa_offsets_table_x8_z[0];
  } else if (msaaSamplesCount == 16) {
    msaa_samples_offset_table_x = &msaa_offsets_table_x16_x[0];
    msaa_samples_offset_table_y = &msaa_offsets_table_x16_y[0];
    msaa_samples_offset_table_z = &msaa_offsets_table_x16_z[0];
  } else {
    REDGPU_2_EXPECTFL(!"Error: unsupported msaaSamplesCount value (the only supported values are 1, 2, 4, 8 and 16).");
  }

  for (int i = 0; i < msaaSamplesCount; i += 1) {
    sharedData[0].msaaSamplesOffsetTableX[i] = msaa_samples_offset_table_x[i];
    sharedData[0].msaaSamplesOffsetTableY[i] = msaa_samples_offset_table_y[i];
    sharedData[0].msaaSamplesOffsetTableZ[i] = msaa_samples_offset_table_z[i];
  }

  #include "custom_msaa_resolve.cs.h"
  gpu_program_info_t custom_msaa_resolve_cs_info = {0};
  custom_msaa_resolve_cs_info.program_binary_bytes_count = sizeof(custom_msaa_resolve_cs_code);
  custom_msaa_resolve_cs_info.program_binary             = custom_msaa_resolve_cs_code;
  uint64_t custom_msaa_resolve_cs = vfProgramCreateFromBinaryCompute(ctx, &custom_msaa_resolve_cs_info, FF, LL);
  uint64_t custom_msaa_resolve_pp = vfProgramPipelineCreateCompute(ctx, custom_msaa_resolve_cs, &ppiCustomMsaaResolve, NULL, FF, LL);

  uint64_t batch = 0;
  ReiiHandleCommandList hlist = {0};
  ReiiHandleCommandList * list = &hlist;
  Red2Output mutable_outputs_array[32] = {0};
  list->mutable_outputs_array.items    = mutable_outputs_array;
  list->mutable_outputs_array.capacity = countof(mutable_outputs_array);

  struct Variables variables = {0};

  variables.cameraPos.x = 0;
  variables.cameraPos.y = 0;
  variables.cameraPos.z = -2.f;
  variables.cameraPos.w = 0;

  variables.cameraRotQuaternion.x = 0;
  variables.cameraRotQuaternion.y = 0;
  variables.cameraRotQuaternion.z = 0;
  variables.cameraRotQuaternion.w = 1;

  variables.msaaSamplesCount = msaaSamplesCount; // NOTE(Constantine): Passing MSAA samples count to the custom MSAA resolve compute shader.
  variables.msaaResolve = 0;
  variables.msaaCurrentSample = 0;
  variables.msaaCameraOffsetMultiplier = 0;

  ReiiBool32 camera_is_enabled = 1;
  if (camera_is_enabled == 1) {
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  }

  glfwPollEvents();
  double mouse_x = 0;
  double mouse_y = 0;
  glfwGetCursorPos(window, &mouse_x, &mouse_y);
  double mouse_x_prev = mouse_x;
  double mouse_y_prev = mouse_y;
  int    mouse_right_mouse_button_state_prev = 0;

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
      vfWindowGetSize(ctx, &window_w, &window_h);

      if (window_w != previous_window_w || window_h != previous_window_h) {
        // Recreate output textures then.

        vfAllQueuesWaitIdle(ctx, FF, LL);

        vfGpuThreadDestroy(ctx, gpu_thread);
        gpu_thread = NULL;
        vfGpuThreadCreate(ctx, 1, &gpu_thread, NULL, FF, LL);

        reiiDestroyEx(ctx, GPU_EXTRA_REII_DESTROY_TYPE_TEXTURE, outputcoltex);
        reiiDestroyEx(ctx, GPU_EXTRA_REII_DESTROY_TYPE_TEXTURE, outputdstex);

        reiiResetTextureMemory(ctx, &outputColTexMemory);
        reiiResetTextureMemory(ctx, &outputDSTexMemory);

        reiiCreateTextureFromTextureMemory(ctx, &outputDSTexMemory, REII_TEXTURE_BINDING_2D, &houtputdstex);
        reiiTextureSetStateMipmap(ctx, REII_TEXTURE_BINDING_2D, outputdstex, 0);
        reiiTextureSetStateMipmapLevelsCount(ctx, REII_TEXTURE_BINDING_2D, outputdstex, 1);
        reiiTextureDefineAndCopyFromCpu(ctx, REII_TEXTURE_BINDING_2D, outputdstex, 0, REII_TEXTURE_TEXEL_FORMAT_DS, window_w, window_h, REII_TEXTURE_TEXEL_FORMAT_DS, REII_TEXTURE_TEXEL_TYPE_FLOAT, 4, NULL, 1, &gpu_thread, array65536);

        reiiCreateTextureFromTextureMemory(ctx, &outputColTexMemory, REII_TEXTURE_BINDING_2D, &houtputcoltex);
        reiiTextureSetStateMipmap(ctx, REII_TEXTURE_BINDING_2D, outputcoltex, 0);
        reiiTextureSetStateMipmapLevelsCount(ctx, REII_TEXTURE_BINDING_2D, outputcoltex, 1);
        reiiTextureDefineAndCopyFromCpu(ctx, REII_TEXTURE_BINDING_2D, outputcoltex, 0, REII_TEXTURE_TEXEL_FORMAT_RGBA, window_w, window_h, REII_TEXTURE_TEXEL_FORMAT_RGBA, REII_TEXTURE_TEXEL_TYPE_U8, 4, NULL, 1, &gpu_thread, array65536);
      }

      previous_window_w = window_w;
      previous_window_h = window_h;
    }

    glfwGetCursorPos(window, &mouse_x, &mouse_y);

    int mouse_right_mouse_button_state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2);

    // NOTE(Constantine):
    // Camera quaternion rotation and translation.
    const float mouse_move_sensitivity = 0.0035f;
    const float camera_move_speed      = 0.01f;
    if (mouse_right_mouse_button_state == GLFW_PRESS && mouse_right_mouse_button_state != mouse_right_mouse_button_state_prev) {
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
  
      quatMul(&variables.cameraRotQuaternion.x, &variables.cameraRotQuaternion.x, rot_x);
      quatMul(&variables.cameraRotQuaternion.x, rot_y, &variables.cameraRotQuaternion.x);

      float side_vec[3] = {1, 0, 0};
      float   up_vec[3] = {0, 1, 0};
      float  dir_vec[3] = {0, 0, 1};
      quatRotateVec3Fast(side_vec, side_vec, &variables.cameraRotQuaternion.x);
      quatRotateVec3Fast(  up_vec,   up_vec, &variables.cameraRotQuaternion.x);
      quatRotateVec3Fast( dir_vec,  dir_vec, &variables.cameraRotQuaternion.x);

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

      vec3Add(&variables.cameraPos.x, &variables.cameraPos.x, move_vec_normalized);
    }

    if (igSliderInt("msaa_samples_count", &msaaSamplesCount, 1, 16, NULL)) {
      if (msaaSamplesCount == 3) {
        msaaSamplesCount = 4;
      } else if (msaaSamplesCount == 5 || msaaSamplesCount == 6 || msaaSamplesCount == 7) {
        msaaSamplesCount = 8;
      } else if (msaaSamplesCount > 8) {
        msaaSamplesCount = 16;
      }

      variables.msaaSamplesCount = msaaSamplesCount;

      if (msaaSamplesCount == 1) {
        msaa_samples_offset_table_x = &msaa_offsets_table_x1_x[0];
        msaa_samples_offset_table_y = &msaa_offsets_table_x1_y[0];
        msaa_samples_offset_table_z = &msaa_offsets_table_x1_z[0];
      } else if (msaaSamplesCount == 2) {
        msaa_samples_offset_table_x = &msaa_offsets_table_x2_x[0];
        msaa_samples_offset_table_y = &msaa_offsets_table_x2_y[0];
        msaa_samples_offset_table_z = &msaa_offsets_table_x2_z[0];
      } else if (msaaSamplesCount == 4) {
        msaa_samples_offset_table_x = &msaa_offsets_table_x4_x[0];
        msaa_samples_offset_table_y = &msaa_offsets_table_x4_y[0];
        msaa_samples_offset_table_z = &msaa_offsets_table_x4_z[0];
      } else if (msaaSamplesCount == 8) {
        msaa_samples_offset_table_x = &msaa_offsets_table_x8_x[0];
        msaa_samples_offset_table_y = &msaa_offsets_table_x8_y[0];
        msaa_samples_offset_table_z = &msaa_offsets_table_x8_z[0];
      } else if (msaaSamplesCount == 16) {
        msaa_samples_offset_table_x = &msaa_offsets_table_x16_x[0];
        msaa_samples_offset_table_y = &msaa_offsets_table_x16_y[0];
        msaa_samples_offset_table_z = &msaa_offsets_table_x16_z[0];
      } else {
        REDGPU_2_EXPECTFL(!"Error: unsupported msaaSamplesCount value (the only supported values are 1, 2, 4, 8 and 16).");
      }

      for (int i = 0; i < msaaSamplesCount; i += 1) {
        sharedData[0].msaaSamplesOffsetTableX[i] = msaa_samples_offset_table_x[i];
        sharedData[0].msaaSamplesOffsetTableY[i] = msaa_samples_offset_table_y[i];
        sharedData[0].msaaSamplesOffsetTableZ[i] = msaa_samples_offset_table_z[i];
      }
    }

    static float msaa_camera_offset_multiplier = 0.003f;
    igSliderFloat("msaa_camera_offset_multiplier", &msaa_camera_offset_multiplier, 0.0f, 1.0f, NULL, 1);

    float msaaCameraOffsetMultiplier = msaa_camera_offset_multiplier;
    static bool msaa_enable = 1;
    igCheckbox("msaa_enable", &msaa_enable);
    if (msaa_enable == 0) {
      msaaCameraOffsetMultiplier = 0;
    }
    variables.msaaCameraOffsetMultiplier = msaaCameraOffsetMultiplier;

    static bool showTestWindow = 1;
    igShowTestWindow(&showTestWindow);

    // Clear the accumulation buffer on each new frame here.
    // TODO(Constantine): Move this code to compute shader.
    #pragma omp parallel for
    for (int y = 0; y < 700; y += 1) { // NOTE(Constantine): Resolution is hardcoded.
      #pragma omp parallel for
      for (int x = 0; x < 700; x += 1) { // NOTE(Constantine): Resolution is hardcoded.
        sharedData[0].renderTargetFloat4[y][x].x = 0;
        sharedData[0].renderTargetFloat4[y][x].y = 0;
        sharedData[0].renderTargetFloat4[y][x].z = 0;
        sharedData[0].renderTargetFloat4[y][x].w = 0;
      }
    }

    gpu_batch_info_t bindings_info = {0};
    bindings_info.max_new_bindings_sets_count = 2;
    bindings_info.max_storage_binds_count     = 2;
    bindings_info.max_texture_ro_binds_count  = 1;
    bindings_info.max_texture_rw_binds_count  = 1;
    bindings_info.max_sampler_binds_count     = 1;
    batch = vfBatchBegin(ctx, batch, &bindings_info, NULL, FF, LL);
    list->batch_id = batch;
    reiiCommandListReset(ctx, list);
    reiiCommandSetViewportEx(ctx, list, 0, 0, window_w, window_h, 0, 1);
    reiiCommandSetScissor(ctx, list, 0, 0, window_w, window_h);

    reiiCommandMeshSetState(ctx, list, &mesh_state, NULL);
    reiiCommandBindSamplers(ctx, list, 1, &sampler);
    reiiCommandBindNewBindingsSet(ctx, list, ppiMeshState.struct_members_count, ppiMeshState.struct_members);
    reiiCommandBindStorageRaw(ctx, list, 0, 1, &sharedDataHandles.storageRaw);
    RedStructMemberTexture texture_slot = {0};
    texture_slot.texture = textureHandle->texture;
    texture_slot.setTo1  = 1;
    reiiCommandBindTextureRO(ctx, list, 1, 1, &texture_slot);
    reiiCommandBindNewBindingsEnd(ctx, list);

    vfBatchBindProgramPipelineCompute(ctx, batch, custom_msaa_resolve_pp, FF, LL);
    vfBatchBindNewBindingsSet(ctx, batch, ppiCustomMsaaResolve.struct_members_count, ppiCustomMsaaResolve.struct_members, FF, LL);
    vfBatchBindStorageRaw(ctx, batch, 0, 1, &sharedDataHandles.storageRaw, FF, LL);
    texture_slot.texture = outputcoltex->texture;
    texture_slot.setTo1  = 1;
    vfBatchBindTextureRWEx(ctx, batch, 1, 1, &texture_slot, FF, LL);
    vfBatchBindNewBindingsEnd(ctx, batch, FF, LL);

    variables.msaaResolve = 0;
    vfBatchBindVariablesCopy(ctx, batch, 0, sizeof(variables), &variables, FF, LL);

    for (int i = 0; i < msaaSamplesCount; i += 1) {
      float clearR = sqrt(0.f);
      float clearG = sqrt(0.f);
      float clearB = sqrt(0.05f);
      float clearA = 1.f;
      reiiCommandClearTexture(ctx, list, outputdstex, outputcoltex, outputcoltex->texture, REII_CLEAR_DEPTH_BIT | REII_CLEAR_COLOR_BIT, 0.f, 0, clearB,clearG,clearR,clearA);
      reiiCommandRenderTargetSet(ctx, list, outputdstex, outputcoltex, outputcoltex->texture);

      struct Variables v = variables;
      v.msaaCurrentSample = i;
      reiiCommandBindVariablesCopy(ctx, list, 0, sizeof(v), &v);

      reiiCommandMeshSetState(ctx, list, &mesh_state, NULL);
      reiiCommandMeshLaunchThreads(ctx, list, 0, 2904, 0, 1);
      reiiCommandMeshSetState(ctx, list, &quad_mesh_state, NULL);
      reiiCommandMeshLaunchThreads(ctx, list, 0, 6, 0, 1);

      reiiCommandRenderTargetEnd(ctx, list);

      vfBatchBarrierMemory(ctx, batch, FF, LL);

      // Accumulate
      vfBatchComputeLaunchThreads(ctx, batch, 700,700,0, 8,8,1, FF, LL);

      vfBatchBarrierMemory(ctx, batch, FF, LL);
    }

    // Resolve
    variables.msaaResolve = 1;
    vfBatchBindVariablesCopy(ctx, batch, 0, sizeof(variables), &variables, FF, LL);
    vfBatchComputeLaunchThreads(ctx, batch, 700,700,0, 8,8,1, FF, LL);

    vfBatchEnd(ctx, batch, FF, LL);

    RedHandleCalls batchRaw = vfBatchGetRawHandle(ctx, batch, FF, LL);
    uint64_t wait = vfAsyncBatchExecuteRaw(ctx, 1, &batchRaw, 1, &gpu_thread, array65536, FF, LL);
    vfAsyncWaitToFinish(ctx, wait, FF, LL);

    idcImguiDraw(idcimgui, camera_is_enabled == 0);

    gpu_thread_t gpu_threads[2] = {gpu_thread, 0};
    vfAsyncDrawImageRaw(ctx, outputcoltex->image.handle, NULL, 2, gpu_threads, array65536, FF, LL);

    mouse_x_prev = mouse_x;
    mouse_y_prev = mouse_y;
    mouse_right_mouse_button_state_prev = mouse_right_mouse_button_state;
  }

  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
  vfAllQueuesWaitIdle(ctx, FF, LL);

  idcImguiDeinit(idcimgui);
  vfGpuThreadDestroy(ctx, gpu_thread);
  reiiDestroyEx(ctx, GPU_EXTRA_REII_DESTROY_TYPE_SAMPLER, sampler);
  reiiDestroyEx(ctx, GPU_EXTRA_REII_DESTROY_TYPE_TEXTURE, textureHandle);
  reiiDestroyEx(ctx, GPU_EXTRA_REII_DESTROY_TYPE_TEXTURE_MEMORY, &texturesMemory);
  reiiDestroyEx(ctx, GPU_EXTRA_REII_DESTROY_TYPE_COMMAND_LIST, list);
  reiiDestroyEx(ctx, GPU_EXTRA_REII_DESTROY_TYPE_TEXTURE, outputcoltex);
  reiiDestroyEx(ctx, GPU_EXTRA_REII_DESTROY_TYPE_TEXTURE, outputdstex);
  reiiDestroyEx(ctx, GPU_EXTRA_REII_DESTROY_TYPE_TEXTURE_MEMORY, &outputColTexMemory);
  reiiDestroyEx(ctx, GPU_EXTRA_REII_DESTROY_TYPE_TEXTURE_MEMORY, &outputDSTexMemory);
  reiiDestroyEx(ctx, GPU_EXTRA_REII_DESTROY_TYPE_MESH_STATE, &mesh_state);
  reiiDestroyEx(ctx, GPU_EXTRA_REII_DESTROY_TYPE_MESH_STATE, &quad_mesh_state);
  uint64_t ids[] = {
    batch,
    custom_msaa_resolve_pp,
    custom_msaa_resolve_cs,
  };
  vfIdDestroy(countof(ids), ids, FF, LL);
  vfeReBARFreeShared(ctx, &scratchMemoryHandles);
  vfeReBARFreeShared(ctx, &sharedDataHandles);
  vfContextDeinit(ctx, FF, LL);
  glfwTerminate();
}

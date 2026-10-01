//\\rc rawbuild begin gcc-linux-64-bit
//\\rc rawbuild require-config debug,release,release-fast
//\\rc rawbuild `gcc`
//\\rc rawbuild debug ` -g -O0`
//\\rc rawbuild release,release-fast ` -O2`
//\\rc rawbuild ` main.c ../../vkfast.c "../../extra/CPU GPU Array/vkfast_extra_cpu_gpu_array.c" ../../extra/REII/vkfast_extra_reii.c /home/linuxbrew/RedGpuSDK/redgpu.c /home/linuxbrew/RedGpuSDK/redgpu_2.c /home/linuxbrew/RedGpuSDK/redgpu_32.c -I/home/linuxbrew/.linuxbrew/include/ -I/home/linuxbrew/.linuxbrew/Cellar/xorgproto/2025.1/include/ -I/var/home/linuxbrew/.linuxbrew/Cellar/libxcb/1.17.0/include/ /home/linuxbrew/.linuxbrew/Cellar/glfw/3.5.1/lib/libglfw3.a /home/linuxbrew/.linuxbrew/lib/libX11.so /home/linuxbrew/.linuxbrew/lib/libvulkan.so -lm`
//\\rc rawbuild end

#include "../../vkfast.h"
#include "../../extra/REII/vkfast_extra_reii.h"
#include "../../extra/vkFast Extensions/ReBAR/vkfast_ext_rebar.h"
#define STB_IMAGE_IMPLEMENTATION
#include "../../extra/stb_image 2017/stb_image.h"
#define VKFAST_EXAMPLES_COMMON_INCLUDE_GLFW3
#include "../Common/vkfast_examples_common.h"

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
      /*visib*/ RED_VISIBLE_TO_STAGE_BITFLAG_VERTEX,
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

  int window_w = 700;
  int window_h = 700;

  glfwInit();
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  GLFWwindow * window = glfwCreateWindow(window_w, window_h, "[vkFast] ReBAR REII Suzanne and a quad", 0, 0);
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
  gpu_handle_context_t ctx = vfContextInitNoDefaultAllocs(1, NULL, FF, LL);
  vfWindowFullscreen(ctx, window_handle, "[vkFast] ReBAR REII Suzanne and a quad", window_w, window_h, 0, RED_PRESENT_VSYNC_MODE_ON, FF, LL);

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
  mesh_state.compileCommandVS =
  "/opt/dxc"
  " \"/home/constantine/Desktop/vkfast/examples/54 ReBAR REII Suzanne and a quad/mesh.hlsl\""
  " -DVS -T vs_6_0 -Fo"
  " \"/home/constantine/Desktop/vkfast/examples/54 ReBAR REII Suzanne and a quad/mesh.vs.spv\""
  " -spirv";
  mesh_state.compileCommandFS =
  "/opt/dxc"
  " \"/home/constantine/Desktop/vkfast/examples/54 ReBAR REII Suzanne and a quad/mesh.hlsl\""
  " -DFS -T ps_6_0 -Fo"
  " \"/home/constantine/Desktop/vkfast/examples/54 ReBAR REII Suzanne and a quad/mesh.fs.spv\""
  " -spirv";
  mesh_state.compiledSpvFilepathVS = "/home/constantine/Desktop/vkfast/examples/54 ReBAR REII Suzanne and a quad/mesh.vs.spv";
  mesh_state.compiledSpvFilepathFS = "/home/constantine/Desktop/vkfast/examples/54 ReBAR REII Suzanne and a quad/mesh.fs.spv";

  ReiiMeshState quad_mesh_state = mesh_state;
  quad_mesh_state.compileCommandVS =
  "/opt/dxc"
  " \"/home/constantine/Desktop/vkfast/examples/54 ReBAR REII Suzanne and a quad/quad_mesh.hlsl\""
  " -DVS -T vs_6_0 -Fo"
  " \"/home/constantine/Desktop/vkfast/examples/54 ReBAR REII Suzanne and a quad/quad_mesh.vs.spv\""
  " -spirv";
  quad_mesh_state.compileCommandFS =
  "/opt/dxc"
  " \"/home/constantine/Desktop/vkfast/examples/54 ReBAR REII Suzanne and a quad/quad_mesh.hlsl\""
  " -DFS -T ps_6_0 -Fo"
  " \"/home/constantine/Desktop/vkfast/examples/54 ReBAR REII Suzanne and a quad/quad_mesh.fs.spv\""
  " -spirv";
  quad_mesh_state.compiledSpvFilepathVS = "/home/constantine/Desktop/vkfast/examples/54 ReBAR REII Suzanne and a quad/quad_mesh.vs.spv";
  quad_mesh_state.compiledSpvFilepathFS = "/home/constantine/Desktop/vkfast/examples/54 ReBAR REII Suzanne and a quad/quad_mesh.fs.spv";

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

  // Create scratch memory for Suzanne texture uploads
  VfeReBARMallocShared scratchMemoryHandles = {};
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

  VfeReBARMallocShared sharedDataHandles = {};
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

    float pos_scale = 50;
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
    sharedData[0].meshQuadVertexUVs[0].y = 0;

    sharedData[0].meshQuadVertexUVs[1].x = 1;
    sharedData[0].meshQuadVertexUVs[1].y = 0;

    sharedData[0].meshQuadVertexUVs[2].x = 1;
    sharedData[0].meshQuadVertexUVs[2].y = 1;

    sharedData[0].meshQuadVertexUVs[3].x = 0;
    sharedData[0].meshQuadVertexUVs[3].y = 0;

    sharedData[0].meshQuadVertexUVs[4].x = 1;
    sharedData[0].meshQuadVertexUVs[4].y = 1;

    sharedData[0].meshQuadVertexUVs[5].x = 0;
    sharedData[0].meshQuadVertexUVs[5].y = 1;
  }

  uint64_t batch = 0;
  ReiiHandleCommandList hlist = {0};
  ReiiHandleCommandList * list = &hlist;
  Red2Output mutable_outputs_array[2]  = {0};
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

    gpu_batch_info_t bindings_info = {0};
    bindings_info.max_new_bindings_sets_count = 1;
    bindings_info.max_storage_binds_count     = 1;
    bindings_info.max_texture_ro_binds_count  = 1;
    bindings_info.max_sampler_binds_count     = 1;
    batch = vfBatchBegin(ctx, batch, &bindings_info, NULL, FF, LL);
    list->batch_id = batch;
    reiiCommandListReset(ctx, list);
    reiiCommandSetViewportEx(ctx, list, 0, 0, window_w, window_h, 0, 1);
    reiiCommandSetScissor(ctx, list, 0, 0, window_w, window_h);
    float clearR = sqrt(0.f);
    float clearG = sqrt(0.f);
    float clearB = sqrt(0.05f);
    float clearA = 1.f;
    reiiCommandClearTexture(ctx, list, outputdstex, outputcoltex, outputcoltex->texture, REII_CLEAR_DEPTH_BIT | REII_CLEAR_COLOR_BIT, 0.f, 0, clearB,clearG,clearR,clearA);
    reiiCommandMeshSetState(ctx, list, &mesh_state, NULL);
    reiiCommandBindSamplers(ctx, list, 1, &sampler);
    reiiCommandBindNewBindingsSet(ctx, list, ppiMeshState.struct_members_count, ppiMeshState.struct_members);
    reiiCommandBindStorageRaw(ctx, list, 0, 1, &sharedDataHandles.storageRaw);
    RedStructMemberTexture texture_slot = {0};
    texture_slot.texture = textureHandle->texture;
    texture_slot.setTo1  = 1;
    reiiCommandBindTextureRO(ctx, list, 1, 1, &texture_slot);
    reiiCommandBindNewBindingsEnd(ctx, list);
    reiiCommandBindVariablesCopy(ctx, list, 0, sizeof(variables), &variables);
    reiiCommandRenderTargetSet(ctx, list, outputdstex, outputcoltex, outputcoltex->texture);
    reiiCommandUnorderedArrayDrawInstancedEx(ctx, list, 0, 2904, 0, 1);
    reiiCommandMeshSetState(ctx, list, &quad_mesh_state, NULL);
    reiiCommandUnorderedArrayDrawInstancedEx(ctx, list, 0, 6, 0, 1);
    reiiCommandRenderTargetEnd(ctx, list);
    vfBatchEnd(ctx, batch, FF, LL);

    RedHandleCalls batchRaw = vfBatchGetRawHandle(ctx, batch, FF, LL);
    uint64_t wait = vfAsyncBatchExecuteRaw(ctx, 1, &batchRaw, 1, &gpu_thread, array65536, FF, LL);
    vfAsyncWaitToFinish(ctx, wait, FF, LL);

    gpu_thread_t gpu_threads[2] = {gpu_thread, 0};
    vfAsyncDrawImageRaw(ctx, outputcoltex->image.handle, NULL, 2, gpu_threads, array65536, FF, LL);

    mouse_x_prev = mouse_x;
    mouse_y_prev = mouse_y;
    mouse_right_mouse_button_state_prev = mouse_right_mouse_button_state;
  }

  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
  vfAllQueuesWaitIdle(ctx, FF, LL);

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
  };
  vfIdDestroy(countof(ids), ids, FF, LL);
  vfeReBARFreeShared(ctx, &scratchMemoryHandles);
  vfeReBARFreeShared(ctx, &sharedDataHandles);
  vfContextDeinit(ctx, FF, LL);
  glfwTerminate();
}

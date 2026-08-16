#define _USE_MATH_DEFINES // For M_PI

#include "../../vkfast.h"
#include "../../extra/Banzai/vkfast_extra_banzai_pointer.h"
#include "../../extra/REII/vkfast_extra_reii.h"
#define VKFAST_EXAMPLES_COMMON_INCLUDE_GLFW3
#define VKFAST_EXAMPLES_COMMON_INCLUDE_EXTRA_BANZAI
#include "../Common/vkfast_examples_common.h"

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
  GLFWwindow * window = glfwCreateWindow(window_w, window_h, "[vkFast] REII BSI 1.0 Text Test Non3D", 0, 0);
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
  vfWindowFullscreen(ctx, window_handle, "[vkFast] REII BSI 1.0 Text Test Non3D", window_w, window_h, 0, RED_PRESENT_VSYNC_MODE_ON, FF, LL);

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

  gpu_extra_cpu_gpu_array pos_array = OffsetAllocateCpuGpuArrayWithTale64BytesAlign(
    64/*mb*/ * 1024 * 1024,
    &storage_cpu_upload, &storage_cpu_upload_mem_offset,
    &storage_gpu_only,   &storage_gpu_only_mem_offset,
    FF, LL
  );
  gpu_extra_cpu_gpu_array col_array = OffsetAllocateCpuGpuArrayWithTale64BytesAlign(
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

  RedStructDeclarationMember slots[2] = {0};
  slots[0].slot            = 0;
  slots[0].type            = RED_STRUCT_MEMBER_TYPE_ARRAY_RO_RW;
  slots[0].count           = 1;
  slots[0].visibleToStages = RED_VISIBLE_TO_STAGE_BITFLAG_VERTEX;
  slots[1].slot            = 1;
  slots[1].type            = RED_STRUCT_MEMBER_TYPE_ARRAY_RO_RW;
  slots[1].count           = 1;
  slots[1].visibleToStages = RED_VISIBLE_TO_STAGE_BITFLAG_VERTEX;
  gpu_extra_reii_mesh_state_compile_info_t mesh_state_compile_info = {0};
  mesh_state_compile_info.state_multisample_count     = RED_MULTISAMPLE_COUNT_BITFLAG_4;
  mesh_state_compile_info.output_depth_stencil_enable = 1;
  mesh_state_compile_info.output_depth_stencil_format = RED_FORMAT_DEPTH_32_FLOAT;
  mesh_state_compile_info.output_color_format         = RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1;
  mesh_state_compile_info.variables_slot              = 2;
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

  #include "mesh_text_non3d.vs.h"
  #include "mesh_text_non3d.fs.h"
  gpu_program_info_t vp_ascii_monospace = {0};
  vp_ascii_monospace.program_binary_bytes_count = sizeof(g_main);
  vp_ascii_monospace.program_binary             = g_main;
  gpu_program_info_t fp_ascii_monospace = {0};
  fp_ascii_monospace.program_binary_bytes_count = sizeof(g_main_text_non3d_fs);
  fp_ascii_monospace.program_binary             = g_main_text_non3d_fs;
  char * vp_ascii_monospace_string = NULL;
  char * fp_ascii_monospace_string = NULL;

  RedStructDeclarationMember slots_ascii_monospace[2] = {0};
  slots_ascii_monospace[0].slot            = 0;
  slots_ascii_monospace[0].type            = RED_STRUCT_MEMBER_TYPE_ARRAY_RO_RW;
  slots_ascii_monospace[0].count           = 1;
  slots_ascii_monospace[0].visibleToStages = RED_VISIBLE_TO_STAGE_BITFLAG_VERTEX;

  slots_ascii_monospace[1].slot            = 1;
  slots_ascii_monospace[1].type            = RED_STRUCT_MEMBER_TYPE_ARRAY_RO_RW;
  slots_ascii_monospace[1].count           = 1;
  slots_ascii_monospace[1].visibleToStages = RED_VISIBLE_TO_STAGE_BITFLAG_VERTEX;
  gpu_extra_reii_mesh_state_compile_info_t mesh_state_compile_info_ascii_monospace = {0};
  mesh_state_compile_info_ascii_monospace.state_multisample_count     = RED_MULTISAMPLE_COUNT_BITFLAG_4;
  mesh_state_compile_info_ascii_monospace.output_depth_stencil_enable = 1;
  mesh_state_compile_info_ascii_monospace.output_depth_stencil_format = RED_FORMAT_DEPTH_32_FLOAT;
  mesh_state_compile_info_ascii_monospace.output_color_format         = RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1;
  mesh_state_compile_info_ascii_monospace.variables_slot              = 2;
  mesh_state_compile_info_ascii_monospace.variables_bytes_count       = 0;
  mesh_state_compile_info_ascii_monospace.struct_members_count        = countof(slots_ascii_monospace);
  mesh_state_compile_info_ascii_monospace.struct_members              = slots_ascii_monospace;
  ReiiMeshState mesh_state_ascii_monospace                                  = {0};
  mesh_state_ascii_monospace.compileInfo                                    = mesh_state_compile_info_ascii_monospace;
  mesh_state_ascii_monospace.programVertex                                  = vp_ascii_monospace;
  mesh_state_ascii_monospace.programFragment                                = fp_ascii_monospace;
  mesh_state_ascii_monospace.codeVertex                                     = vp_ascii_monospace_string;
  mesh_state_ascii_monospace.codeFragment                                   = fp_ascii_monospace_string;
  mesh_state_ascii_monospace.rasterizationDepthClampEnable                  = 0;
  mesh_state_ascii_monospace.rasterizationCullMode                          = REII_CULL_MODE_NONE;
  mesh_state_ascii_monospace.rasterizationFrontFace                         = REII_FRONT_FACE_COUNTER_CLOCKWISE;
  mesh_state_ascii_monospace.rasterizationDepthBiasEnable                   = 0;
  mesh_state_ascii_monospace.rasterizationDepthBiasConstantFactor           = 0;
  mesh_state_ascii_monospace.rasterizationDepthBiasSlopeFactor              = 0;
  mesh_state_ascii_monospace.multisampleEnable                              = 1;
  mesh_state_ascii_monospace.multisampleAlphaToCoverageEnable               = 0;
  mesh_state_ascii_monospace.multisampleAlphaToOneEnable                    = 0;
  mesh_state_ascii_monospace.depthTestEnable                                = 0; // NOTE(Constantine): No need for depth test for non-3D text.
  mesh_state_ascii_monospace.depthTestDepthWriteEnable                      = 0; // NOTE(Constantine): No need for depth test for non-3D text.
  mesh_state_ascii_monospace.depthTestDepthCompareOp                        = REII_COMPARE_OP_GREATER_OR_EQUAL;
  mesh_state_ascii_monospace.stencilTestEnable                              = 0;
  mesh_state_ascii_monospace.stencilTestFrontStencilTestFailOp              = REII_STENCIL_OP_KEEP;
  mesh_state_ascii_monospace.stencilTestFrontStencilTestPassDepthTestPassOp = REII_STENCIL_OP_KEEP;
  mesh_state_ascii_monospace.stencilTestFrontStencilTestPassDepthTestFailOp = REII_STENCIL_OP_KEEP;
  mesh_state_ascii_monospace.stencilTestFrontCompareOp                      = REII_COMPARE_OP_NEVER;
  mesh_state_ascii_monospace.stencilTestBackStencilTestFailOp               = REII_STENCIL_OP_KEEP;
  mesh_state_ascii_monospace.stencilTestBackStencilTestPassDepthTestPassOp  = REII_STENCIL_OP_KEEP;
  mesh_state_ascii_monospace.stencilTestBackStencilTestPassDepthTestFailOp  = REII_STENCIL_OP_KEEP;
  mesh_state_ascii_monospace.stencilTestBackCompareOp                       = REII_COMPARE_OP_NEVER;
  mesh_state_ascii_monospace.stencilTestFrontAndBackCompareMask             = 0;
  mesh_state_ascii_monospace.stencilTestFrontAndBackWriteMask               = 0;
  mesh_state_ascii_monospace.stencilTestFrontAndBackReference               = 0;
  mesh_state_ascii_monospace.blendLogicOpEnable                             = 0;
  mesh_state_ascii_monospace.blendLogicOp                                   = REII_LOGIC_OP_CLEAR;
  mesh_state_ascii_monospace.blendConstants[0]                              = 0;
  mesh_state_ascii_monospace.blendConstants[1]                              = 0;
  mesh_state_ascii_monospace.blendConstants[2]                              = 0;
  mesh_state_ascii_monospace.blendConstants[3]                              = 0;
  mesh_state_ascii_monospace.outputColorWriteEnableR                        = 1;
  mesh_state_ascii_monospace.outputColorWriteEnableG                        = 1;
  mesh_state_ascii_monospace.outputColorWriteEnableB                        = 1;
  mesh_state_ascii_monospace.outputColorWriteEnableA                        = 1;
  mesh_state_ascii_monospace.outputColorBlendEnable                         = 0;
  mesh_state_ascii_monospace.outputColorBlendColorFactorSource              = REII_BLEND_FACTOR_ZERO;
  mesh_state_ascii_monospace.outputColorBlendColorFactorTarget              = REII_BLEND_FACTOR_ZERO;
  mesh_state_ascii_monospace.outputColorBlendColorOp                        = REII_BLEND_OP_ADD;
  mesh_state_ascii_monospace.outputColorBlendAlphaFactorSource              = REII_BLEND_FACTOR_ZERO;
  mesh_state_ascii_monospace.outputColorBlendAlphaFactorTarget              = REII_BLEND_FACTOR_ZERO;
  mesh_state_ascii_monospace.outputColorBlendAlphaOp                        = REII_BLEND_OP_ADD;
  reiiMeshStateCompile(ctx, &mesh_state_ascii_monospace);

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

  float suzanne_head_vertices[] = {
    #include "../../extra/3D Mesh Suzanne Head/3d_mesh_vertices_suzanne_head.h"
  };

  struct int3   { int   x, y, z; };
  struct float2 { float x, y; };
  struct float4 { float x, y, z, w; };

  enum mesh_ascii_monospace_enums {
    #include "../../extra/3D Various Mesh Headers/ascii monospace/mesh_enums.h"
    BLENDER_MESH_ENUMS_COUNT
  };
  struct int3 mesh_ascii_monospace_indices[] = {
    #include "../../extra/3D Various Mesh Headers/ascii monospace/mesh_indices.h"
  };
  struct float4 mesh_ascii_monospace_normals[] = {
    #include "../../extra/3D Various Mesh Headers/ascii monospace/mesh_normals.h"
  };
  struct float2 mesh_ascii_monospace_uvs[] = {
    #include "../../extra/3D Various Mesh Headers/ascii monospace/mesh_uvs.h"
  };
  struct float4 mesh_ascii_monospace_vertices[] = {
    #include "../../extra/3D Various Mesh Headers/ascii monospace/mesh_vertices.h"
  };
  uint64_t mesh_ascii_monospace_submesh_tri_begin[] = {
    #include "../../extra/3D Various Mesh Headers/ascii monospace/submesh_tri_begin.h"
  };
  uint64_t mesh_ascii_monospace_submesh_tri_end[] = {
    #include "../../extra/3D Various Mesh Headers/ascii monospace/submesh_tri_end.h"
  };
  struct float4 mesh_ascii_monospace_submesh_xform_scale[] = {
    #include "../../extra/3D Various Mesh Headers/ascii monospace/submesh_xform_scale.h"
  };
  struct float4 mesh_ascii_monospace_submesh_xform_rotation_quaternion[] = {
    #include "../../extra/3D Various Mesh Headers/ascii monospace/submesh_xform_rotation_quaternion.h"
  };
  struct float4 mesh_ascii_monospace_submesh_xform_translation[] = {
    #include "../../extra/3D Various Mesh Headers/ascii monospace/submesh_xform_translation.h"
  };

  gpu_extra_cpu_gpu_array mesh_ascii_monospace_vertex_array = OffsetAllocateCpuGpuArrayWithTale64BytesAlign(
    157104, // NOTE(Constantine): Hardcoded for the current ascii monospace mesh.
    &storage_cpu_upload, &storage_cpu_upload_mem_offset,
    &storage_gpu_only,   &storage_gpu_only_mem_offset,
    FF, LL
  );

  gpu_extra_cpu_gpu_array mesh_ascii_monospace_instances_position_array = OffsetAllocateCpuGpuArrayWithTale64BytesAlign(
    3 * sizeof(ReiiVec4), // NOTE(Constantine): Hardcoded for the current example.
    &storage_cpu_upload, &storage_cpu_upload_mem_offset,
    &storage_gpu_only,   &storage_gpu_only_mem_offset,
    FF, LL
  );

  ReiiHandleUnorderedArray hmesh_ascii_monospace = {0};
  ReiiHandleUnorderedArray * mesh_ascii_monospace = &hmesh_ascii_monospace;
  mesh_ascii_monospace->position = mesh_ascii_monospace_vertex_array;
  reiiCreateUnorderedArray(ctx, mesh_ascii_monospace);
  reiiUnorderedArraySet(ctx, mesh_ascii_monospace);
  for (int j = 0; j < BLENDER_MESH_ENUMS_COUNT; j += 1) {
    for (int i = mesh_ascii_monospace_submesh_tri_begin[j]; i < mesh_ascii_monospace_submesh_tri_end[j]; i += 1) {
      int v0i = mesh_ascii_monospace_indices[i].x;
      int v1i = mesh_ascii_monospace_indices[i].y;
      int v2i = mesh_ascii_monospace_indices[i].z;

      struct float4 v0 = mesh_ascii_monospace_vertices[v0i];
      struct float4 v1 = mesh_ascii_monospace_vertices[v1i];
      struct float4 v2 = mesh_ascii_monospace_vertices[v2i];

      // Scale

      v0.x *= mesh_ascii_monospace_submesh_xform_scale[j].x;
      v0.y *= mesh_ascii_monospace_submesh_xform_scale[j].y;
      v0.z *= mesh_ascii_monospace_submesh_xform_scale[j].z;

      v1.x *= mesh_ascii_monospace_submesh_xform_scale[j].x;
      v1.y *= mesh_ascii_monospace_submesh_xform_scale[j].y;
      v1.z *= mesh_ascii_monospace_submesh_xform_scale[j].z;

      v2.x *= mesh_ascii_monospace_submesh_xform_scale[j].x;
      v2.y *= mesh_ascii_monospace_submesh_xform_scale[j].y;
      v2.z *= mesh_ascii_monospace_submesh_xform_scale[j].z;

      // Rotate

      quatRotateVec3Fast(&v0.x, &v0.x, &mesh_ascii_monospace_submesh_xform_rotation_quaternion[j].x);
      quatRotateVec3Fast(&v1.x, &v1.x, &mesh_ascii_monospace_submesh_xform_rotation_quaternion[j].x);
      quatRotateVec3Fast(&v2.x, &v2.x, &mesh_ascii_monospace_submesh_xform_rotation_quaternion[j].x);

      // Translate

      vec3Add(&v0.x, &mesh_ascii_monospace_submesh_xform_translation[j].x, &v0.x);
      vec3Add(&v1.x, &mesh_ascii_monospace_submesh_xform_translation[j].x, &v1.x);
      vec3Add(&v2.x, &mesh_ascii_monospace_submesh_xform_translation[j].x, &v2.x);

      // Rotate everything from Blender coordinates to Vulkan coordinates

      //struct float4 q = {0, 0, 0, 1};
      //float axis[3] = {1, 0, 0};
      //quatFromAxisAngle(&q.x, axis, -90.f * (M_PI/180.f));
      //quatRotateVec3Fast(&v0.x, &v0.x, &q.x);
      //quatRotateVec3Fast(&v1.x, &v1.x, &q.x);
      //quatRotateVec3Fast(&v2.x, &v2.x, &q.x);

      // Mirror everything from Blender coordinates to Vulkan coordinates

      //v0.x *= -1.f;
      //v1.x *= -1.f;
      //v2.x *= -1.f;

      // Set vertex final position

      reiiUnorderedArrayPosition(ctx, mesh_ascii_monospace, v0.x, v0.y, v0.z, 1);
      reiiUnorderedArrayPosition(ctx, mesh_ascii_monospace, v1.x, v1.y, v1.z, 1);
      reiiUnorderedArrayPosition(ctx, mesh_ascii_monospace, v2.x, v2.y, v2.z, 1);
    }
  }
  reiiUnorderedArrayEnd(ctx, mesh_ascii_monospace, 1, &gpu_thread, array65536);

  uint64_t batch = 0;
  ReiiHandleCommandList hlist = {0};
  ReiiHandleCommandList * list = &hlist;
  Red2Output mutable_outputs_array[4]  = {0};
  list->mutable_outputs_array.items    = mutable_outputs_array;
  list->mutable_outputs_array.capacity = countof(mutable_outputs_array);
  list->dynamic_mesh_position          = pos_array;
  list->dynamic_mesh_color             = col_array;

  ReiiGammaCorrectColorTextureToTheInversePowerOf2StaticState gammaCorrectionStaticState = {0};

  ReiiVec4   camera_pos  = {0, 0, -2.f, 0};
  ReiiVec4   camera_quat = {0, 0, 0, 1};
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

    glfwGetCursorPos(window, &mouse_x, &mouse_y);

    int mouse_right_mouse_button_state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2);

    // NOTE(Constantine):
    // Camera quaternion rotation and translation.
    const float mouse_move_sensitivity = 0.0035f;
    const float camera_move_speed      = 0.025f;
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

    gpu_batch_info_t bindings_info = {0};
    bindings_info.max_new_bindings_sets_count = 3;
    bindings_info.max_storage_binds_count     = 5;
    bindings_info.max_texture_rw_binds_count  = 1;
    batch = vfBatchBegin(ctx, batch, &bindings_info, NULL, FF, LL);
    list->batch_id = batch;
    reiiCommandListReset(ctx, list);
    reiiCommandSetViewportEx(ctx, list, 0, 0, window_w, window_h, 0, 1);
    reiiCommandSetScissor(ctx, list, 0, 0, window_w, window_h);
    reiiCommandClearTexture(ctx, list, outputdstex, outputmstex, outputmstex->texture, REII_CLEAR_DEPTH_BIT | REII_CLEAR_COLOR_BIT, 0.f, 0, 0.f,0.f,0.05f,1.f);
    reiiCommandMeshSetState(ctx, list, &mesh_state, NULL);
    reiiCommandBindNewBindingsSet(ctx, list, countof(slots), slots);
    {
      gpu_extra_cpu_gpu_array dynamicMeshPositionOffsetted = list->dynamic_mesh_position;
      gpu_extra_cpu_gpu_array dynamicMeshColorOffsetted    = list->dynamic_mesh_color;
      vfeCpuGpuArrayOffset(&dynamicMeshPositionOffsetted, list->dynamicMeshPositionVec4Offset * sizeof(ReiiVec4));
      vfeCpuGpuArrayOffset(&dynamicMeshColorOffsetted, list->dynamicMeshColorVec4Offset * sizeof(ReiiVec4));
      reiiCommandBindStorageRaw(ctx, list, 0, 1, &dynamicMeshPositionOffsetted.gpu);
      reiiCommandBindStorageRaw(ctx, list, 1, 1, &dynamicMeshColorOffsetted.gpu);
    }
    reiiCommandBindNewBindingsEnd(ctx, list);
    reiiCommandBindVariablesCopy(ctx, list, 0 * sizeof(ReiiVec4), 1 * sizeof(ReiiVec4), &camera_pos);
    reiiCommandBindVariablesCopy(ctx, list, 1 * sizeof(ReiiVec4), 1 * sizeof(ReiiVec4), &camera_quat);
    reiiCommandMeshSet(ctx, list);
    // Draw colored triangle
    {
      reiiCommandMeshColor(ctx, list, 1, 0, 0, 1);
      reiiCommandMeshPosition(ctx, list, 1,1,0, 1);
      reiiCommandMeshColor(ctx, list, 0, 1, 0, 1);
      reiiCommandMeshPosition(ctx, list, 0,2,0, 1);
      reiiCommandMeshColor(ctx, list, 0, 0, 1, 1);
      reiiCommandMeshPosition(ctx, list, 0,1,1, 1);
    }
    // Draw suzanne head
    for (int i = 0, suzanne_head_vertices_count = countof(suzanne_head_vertices) / 3; i < suzanne_head_vertices_count; i += 1) {
      float scale = 0.5f;
      reiiCommandMeshColor(ctx, list, i * 0.00025f, 0, 0.1f, 1);
      reiiCommandMeshPosition(ctx, list,
        suzanne_head_vertices[i * 3 + 0] * scale,
        suzanne_head_vertices[i * 3 + 1] * scale,
        suzanne_head_vertices[i * 3 + 2] * scale,
        1
      );
    }
    reiiCommandMeshEndWithTale64BytesAlign(ctx, list, outputdstex, outputmstex, outputmstex->texture);
    reiiCommandMeshSetState(ctx, list, &mesh_state_ascii_monospace, NULL);
    reiiCommandBindNewBindingsSet(ctx, list, countof(slots_ascii_monospace), slots_ascii_monospace);
    reiiCommandBindStorageRaw(ctx, list, 0, 1, &mesh_ascii_monospace->position.gpu);
    reiiCommandBindStorageRaw(ctx, list, 1, 1, &mesh_ascii_monospace_instances_position_array.gpu);
    reiiCommandBindNewBindingsEnd(ctx, list);
    reiiCommandRenderTargetSet(ctx, list, outputdstex, outputmstex, outputmstex->texture);
    reiiCommandUnorderedArrayDrawInstanced(ctx, list, mesh_ascii_monospace, 3);
    reiiCommandRenderTargetEnd(ctx, list);
    reiiCommandResolveMsaaColorTexture(ctx, list, outputmstex, outputtex);
    vfBatchBarrierMemory(ctx, batch, FF, LL);
    reiiCommandGammaCorrectColorTextureToTheInversePowerOf2(ctx, list, outputtex, doDoubleGammaCorrection, 1, &gammaCorrectionStaticState);
    vfBatchEnd(ctx, batch, FF, LL);

    RedHandleCalls batchRaw = vfBatchGetRawHandle(ctx, batch, FF, LL);
    uint64_t wait = vfAsyncBatchExecuteRaw(ctx, 1, &batchRaw, 1, &gpu_thread, array65536, FF, LL);
    vfAsyncWaitToFinish(ctx, wait, FF, LL);

    gpu_thread_t gpu_threads[2] = {gpu_thread, 0};
    vfAsyncDrawImageRaw(ctx, outputtex->image.handle, NULL, 2, gpu_threads, array65536, FF, LL);

    mouse_x_prev = mouse_x;
    mouse_y_prev = mouse_y;
    mouse_right_mouse_button_state_prev = mouse_right_mouse_button_state;
  }

  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

  vfAllQueuesWaitIdle(ctx, FF, LL);

  vfGpuThreadDestroy(ctx, gpu_thread);
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
  reiiDestroyEx(ctx, GPU_EXTRA_REII_DESTROY_TYPE_MESH_STATE, &mesh_state_ascii_monospace);
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

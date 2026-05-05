#if 0
clang main.c ../../vkfast.c C:/RedGpuSDK/redgpu.c C:/RedGpuSDK/redgpu_2.c C:/RedGpuSDK/redgpu_32.c ../../extra/Banzai/vkfast_extra_banzai.c ../../extra/Banzai/vkfast_extra_banzai_pointer.c "../../extra/CPU GPU Array/vkfast_extra_cpu_gpu_array.c" ../../extra/REII/vkfast_extra_reii.c
exit
#endif

#include "../../vkfast.h"
#include "../../extra/Banzai/vkfast_extra_banzai_pointer.h"
#include "../../extra/REII/vkfast_extra_reii.h"
#define PAR_SHAPES_IMPLEMENTATION
#include "../../extra/par_shapes/par_shapes.h"
#define VKFAST_EXAMPLES_COMMON_INCLUDE_GLFW3
#define VKFAST_EXAMPLES_COMMON_INCLUDE_EXTRA_BANZAI
#include "../Common/vkfast_examples_common.h"

int main() {
#ifdef __MINGW32__
  SetProcessDPIAware();
#else
  SetProcessDpiAwareness(PROCESS_PER_MONITOR_DPI_AWARE);
#endif

#if defined(_MSC_VER) && defined(_DEBUG)
  _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

  int window_w = 700;
  int window_h = 700;

  glfwInit();
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  GLFWwindow * window = glfwCreateWindow(window_w, window_h, "[vkFast] REII Immediate 3D Meshes", 0, 0);
  void * window_handle = (void *)glfwGetWin32Window(window);

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
  vfWindowFullscreen(ctx, window_handle, "[vkFast] REII Immediate 3D Meshes", window_w, window_h, 0, FF, LL);

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

  ReiiHandleTextureMemory outputDSTexMemory = {0};
  reiiCreateTextureMemory(ctx, GPU_EXTRA_REII_TEXTURE_TYPE_OUTPUT_DEPTH_STENCIL_MSAA, (288/*mb*/ * 1024 * 1024), &outputDSTexMemory);
  ReiiHandleTexture houtputdstex = {0};
  ReiiHandleTexture * outputdstex = &houtputdstex;
  reiiCreateTextureFromTextureMemory(ctx, &outputDSTexMemory, REII_TEXTURE_BINDING_2D, &houtputdstex);
  reiiTextureSetStateMsaa(ctx, REII_TEXTURE_BINDING_2D, outputdstex, 4);
  reiiTextureSetStateMipmap(ctx, REII_TEXTURE_BINDING_2D, outputdstex, 0);
  reiiTextureSetStateMipmapLevelsCount(ctx, REII_TEXTURE_BINDING_2D, outputdstex, 1);
  reiiTextureDefineAndCopyFromCpu(ctx, REII_TEXTURE_BINDING_2D, outputdstex, 0, REII_TEXTURE_TEXEL_FORMAT_DS, window_w, window_h, REII_TEXTURE_TEXEL_FORMAT_DS, REII_TEXTURE_TEXEL_TYPE_FLOAT, 4, NULL);

  ReiiHandleTextureMemory outputTexMemory = {0};
  reiiCreateTextureMemory(ctx, GPU_EXTRA_REII_TEXTURE_TYPE_OUTPUT_COLOR, (288/*mb*/ * 1024 * 1024), &outputTexMemory);
  ReiiHandleTexture houtputtex = {0};
  ReiiHandleTexture * outputtex = &houtputtex;
  reiiCreateTextureFromTextureMemory(ctx, &outputTexMemory, REII_TEXTURE_BINDING_2D, &houtputtex);
  reiiTextureSetStateMipmap(ctx, REII_TEXTURE_BINDING_2D, outputtex, 0);
  reiiTextureSetStateMipmapLevelsCount(ctx, REII_TEXTURE_BINDING_2D, outputtex, 1);
  reiiTextureDefineAndCopyFromCpu(ctx, REII_TEXTURE_BINDING_2D, outputtex, 0, REII_TEXTURE_TEXEL_FORMAT_RGBA, window_w, window_h, REII_TEXTURE_TEXEL_FORMAT_RGBA, REII_TEXTURE_TEXEL_TYPE_U8, 4, NULL);

  ReiiHandleTextureMemory outputMSTexMemory = {0};
  reiiCreateTextureMemory(ctx, GPU_EXTRA_REII_TEXTURE_TYPE_OUTPUT_COLOR_MSAA, (288/*mb*/ * 1024 * 1024), &outputMSTexMemory);
  ReiiHandleTexture houtputmstex = {0};
  ReiiHandleTexture * outputmstex = &houtputmstex;
  reiiCreateTextureFromTextureMemory(ctx, &outputMSTexMemory, REII_TEXTURE_BINDING_2D, &houtputmstex);
  reiiTextureSetStateMsaa(ctx, REII_TEXTURE_BINDING_2D, outputmstex, 4);
  reiiTextureSetStateMipmap(ctx, REII_TEXTURE_BINDING_2D, outputmstex, 0);
  reiiTextureSetStateMipmapLevelsCount(ctx, REII_TEXTURE_BINDING_2D, outputmstex, 1);
  reiiTextureDefineAndCopyFromCpu(ctx, REII_TEXTURE_BINDING_2D, outputmstex, 0, REII_TEXTURE_TEXEL_FORMAT_RGBA, window_w, window_h, REII_TEXTURE_TEXEL_FORMAT_RGBA, REII_TEXTURE_TEXEL_TYPE_U8, 4, NULL);

  float mesh_vertices[] = {
    #include "../../extra/3D Mesh Suzanne Head/3d_mesh_vertices_suzanne_head.h"
  };

  par_shapes_mesh * sphere = par_shapes_create_subdivided_sphere(3);
  REDGPU_2_EXPECTFL(sphere != NULL);

  par_shapes_mesh * cube = par_shapes_create_cube();
  par_shapes_rotate(cube, PAR_PI / 5.0, (float[]){0, 1, 0});

  par_shapes_mesh * dodecahedron = par_shapes_create_dodecahedron();
  par_shapes_unweld(dodecahedron, true);
  par_shapes_compute_normals(dodecahedron);

  // A par scene:

  // Tessellate an open-ended cylinder with 30 slices and 3 stacks.
  par_shapes_mesh * open_cylinder = par_shapes_create_cylinder(30, 3);
  par_shapes_rotate(open_cylinder, -PAR_PI / 2.0, (float[]) {1, 0, 0});
  par_shapes_translate(open_cylinder, 0, 0, 3);

  // Create a disk-shaped cylinder cap with 30 slices.
  par_shapes_mesh * disk_cylinder = par_shapes_create_disk(1, 30, (float[]){0, 1, 3}, (float[]){0, 1, 0});

  // Instantiate a dome shape.
  par_shapes_mesh * dome = par_shapes_create_hemisphere(10, 10);
  par_shapes_scale(dome, 0.2, 0.2, 0.2);
  par_shapes_translate(dome, 0, 1, 3);

  // Create a rectangular backdrop.
  par_shapes_mesh * rect = par_shapes_create_plane(3, 3);
  par_shapes_translate(rect, -0.5, 0, 1);
  par_shapes_scale(rect, 4, 1.5, 1);

  // Place a submerged donut into the scene.
  par_shapes_mesh * donut = par_shapes_create_torus(30, 40, 0.1);
  par_shapes_scale(donut, 2, 2, 2);
  par_shapes_translate(donut, 0, 0, 3);

  uint64_t batch = 0;
  ReiiHandleCommandList hlist = {0};
  ReiiHandleCommandList * list = &hlist;
  Red2Output mutable_outputs_array[3]  = {0};
  list->mutable_outputs_array.items    = mutable_outputs_array;
  list->mutable_outputs_array.capacity = countof(mutable_outputs_array);
  list->dynamic_mesh_position          = pos_array;
  list->dynamic_mesh_color             = col_array;

  ReiiGammaCorrectColorTextureToTheInversePowerOf2StaticState gammaCorrectionStaticState = {0};

  ReiiVec4   camera_pos  = {0, 0, -2.f};
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
        reiiTextureDefineAndCopyFromCpu(ctx, REII_TEXTURE_BINDING_2D, outputdstex, 0, REII_TEXTURE_TEXEL_FORMAT_DS, window_w, window_h, REII_TEXTURE_TEXEL_FORMAT_DS, REII_TEXTURE_TEXEL_TYPE_FLOAT, 4, NULL);

        reiiCreateTextureFromTextureMemory(ctx, &outputTexMemory, REII_TEXTURE_BINDING_2D, &houtputtex);
        reiiTextureSetStateMipmap(ctx, REII_TEXTURE_BINDING_2D, outputtex, 0);
        reiiTextureSetStateMipmapLevelsCount(ctx, REII_TEXTURE_BINDING_2D, outputtex, 1);
        reiiTextureDefineAndCopyFromCpu(ctx, REII_TEXTURE_BINDING_2D, outputtex, 0, REII_TEXTURE_TEXEL_FORMAT_RGBA, window_w, window_h, REII_TEXTURE_TEXEL_FORMAT_RGBA, REII_TEXTURE_TEXEL_TYPE_U8, 4, NULL);

        reiiCreateTextureFromTextureMemory(ctx, &outputMSTexMemory, REII_TEXTURE_BINDING_2D, &houtputmstex);
        reiiTextureSetStateMsaa(ctx, REII_TEXTURE_BINDING_2D, outputmstex, 4);
        reiiTextureSetStateMipmap(ctx, REII_TEXTURE_BINDING_2D, outputmstex, 0);
        reiiTextureSetStateMipmapLevelsCount(ctx, REII_TEXTURE_BINDING_2D, outputmstex, 1);
        reiiTextureDefineAndCopyFromCpu(ctx, REII_TEXTURE_BINDING_2D, outputmstex, 0, REII_TEXTURE_TEXEL_FORMAT_RGBA, window_w, window_h, REII_TEXTURE_TEXEL_FORMAT_RGBA, REII_TEXTURE_TEXEL_TYPE_U8, 4, NULL);
      }

      previous_window_w = window_w;
      previous_window_h = window_h;
    }

    glfwGetCursorPos(window, &mouse_x, &mouse_y);

    int mouse_right_mouse_button_state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2);

    // NOTE(Constantine):
    // Camera quaternion rotation and translation.
    const float mouse_move_sensitivity = 0.0035f;
    const float camera_move_speed      = 0.1f;
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
    bindings_info.max_new_bindings_sets_count = 2;
    bindings_info.max_storage_binds_count     = 2;
    bindings_info.max_texture_rw_binds_count  = 1;
    batch = vfBatchBegin(ctx, batch, &bindings_info, NULL, FF, LL);
    list->batch_id = batch;
    reiiCommandListReset(ctx, list);
    reiiCommandSetViewportEx(ctx, list, 0, 0, window_w, window_h, 0, 1);
    reiiCommandSetScissor(ctx, list, 0, 0, window_w, window_h);
    reiiCommandClearTexture(ctx, list, outputdstex, outputmstex, outputmstex->texture, REII_CLEAR_DEPTH_BIT | REII_CLEAR_COLOR_BIT, 0.f, 0, 0.f,0.f,0.05f,1.f);
    reiiCommandMeshSetState(ctx, list, &mesh_state, NULL);
    reiiCommandBindNewBindingsSet(ctx, list, countof(slots), slots);
    reiiCommandBindStorageRaw(ctx, list, 0, 1, &pos_array.gpu);
    reiiCommandBindStorageRaw(ctx, list, 1, 1, &col_array.gpu);
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
    for (int i = 0, mesh_vertices_count = countof(mesh_vertices) / 3; i < mesh_vertices_count; i += 1) {
      float scale = 0.5f;
      reiiCommandMeshColor(ctx, list, i * 0.00025f, 0, 0.1f, 1);
      reiiCommandMeshPosition(ctx, list,
        mesh_vertices[i * 3 + 0] * scale,
        mesh_vertices[i * 3 + 1] * scale,
        mesh_vertices[i * 3 + 2] * scale,
        1
      );
    }
    // Draw sphere
    {
      const par_shapes_mesh * par_mesh = sphere;

      float scale = 1.f;
      float offset_x = 3.f;
      float offset_y = 0.f;
      float offset_z = 0.f;

      for (int i = 0; i < par_mesh->ntriangles; i += 1) {
        const uint64_t tri_vert0_idx = par_mesh->triangles[i * 3 + 0];
        const uint64_t tri_vert1_idx = par_mesh->triangles[i * 3 + 1];
        const uint64_t tri_vert2_idx = par_mesh->triangles[i * 3 + 2];

        reiiCommandMeshColor(ctx, list, i * 0.00025f, 0, 0.1f, 1);
        reiiCommandMeshPosition(ctx, list,
          par_mesh->points[tri_vert0_idx * 3 + 0] * scale + offset_x,
          par_mesh->points[tri_vert0_idx * 3 + 1] * scale + offset_y,
          par_mesh->points[tri_vert0_idx * 3 + 2] * scale + offset_z,
          1
        );
        reiiCommandMeshColor(ctx, list, i * 0.00025f, 0, 0.1f, 1);
        reiiCommandMeshPosition(ctx, list,
          par_mesh->points[tri_vert1_idx * 3 + 0] * scale + offset_x,
          par_mesh->points[tri_vert1_idx * 3 + 1] * scale + offset_y,
          par_mesh->points[tri_vert1_idx * 3 + 2] * scale + offset_z,
          1
        );
        reiiCommandMeshColor(ctx, list, i * 0.00025f, 0, 0.1f, 1);
        reiiCommandMeshPosition(ctx, list,
          par_mesh->points[tri_vert2_idx * 3 + 0] * scale + offset_x,
          par_mesh->points[tri_vert2_idx * 3 + 1] * scale + offset_y,
          par_mesh->points[tri_vert2_idx * 3 + 2] * scale + offset_z,
          1
        );
      }
    }
    // Draw rotated cube
    {
      const par_shapes_mesh * par_mesh = cube;

      float scale = 1.f;
      float offset_x = -3.f;
      float offset_y = 0.f;
      float offset_z = 0.f;

      for (int i = 0; i < par_mesh->ntriangles; i += 1) {
        const uint64_t tri_vert0_idx = par_mesh->triangles[i * 3 + 0];
        const uint64_t tri_vert1_idx = par_mesh->triangles[i * 3 + 1];
        const uint64_t tri_vert2_idx = par_mesh->triangles[i * 3 + 2];

        reiiCommandMeshColor(ctx, list, i * 0.0025f, 0, 0.1f, 1);
        reiiCommandMeshPosition(ctx, list,
          par_mesh->points[tri_vert0_idx * 3 + 0] * scale + offset_x,
          par_mesh->points[tri_vert0_idx * 3 + 1] * scale + offset_y,
          par_mesh->points[tri_vert0_idx * 3 + 2] * scale + offset_z,
          1
        );
        reiiCommandMeshColor(ctx, list, i * 0.0025f, 0, 0.1f, 1);
        reiiCommandMeshPosition(ctx, list,
          par_mesh->points[tri_vert1_idx * 3 + 0] * scale + offset_x,
          par_mesh->points[tri_vert1_idx * 3 + 1] * scale + offset_y,
          par_mesh->points[tri_vert1_idx * 3 + 2] * scale + offset_z,
          1
        );
        reiiCommandMeshColor(ctx, list, i * 0.0025f, 0, 0.1f, 1);
        reiiCommandMeshPosition(ctx, list,
          par_mesh->points[tri_vert2_idx * 3 + 0] * scale + offset_x,
          par_mesh->points[tri_vert2_idx * 3 + 1] * scale + offset_y,
          par_mesh->points[tri_vert2_idx * 3 + 2] * scale + offset_z,
          1
        );
      }
    }
    // Draw dodecahedron
    {
      const par_shapes_mesh * par_mesh = dodecahedron;

      float scale = 1.f;
      float offset_x = -6.f;
      float offset_y = 0.f;
      float offset_z = 0.f;

      for (int i = 0; i < par_mesh->ntriangles; i += 1) {
        const uint64_t tri_vert0_idx = par_mesh->triangles[i * 3 + 0];
        const uint64_t tri_vert1_idx = par_mesh->triangles[i * 3 + 1];
        const uint64_t tri_vert2_idx = par_mesh->triangles[i * 3 + 2];

        reiiCommandMeshColor(ctx, list, i * 0.0025f, 0, 0.1f, 1);
        reiiCommandMeshPosition(ctx, list,
          par_mesh->points[tri_vert0_idx * 3 + 0] * scale + offset_x,
          par_mesh->points[tri_vert0_idx * 3 + 1] * scale + offset_y,
          par_mesh->points[tri_vert0_idx * 3 + 2] * scale + offset_z,
          1
        );
        reiiCommandMeshColor(ctx, list, i * 0.0025f, 0, 0.1f, 1);
        reiiCommandMeshPosition(ctx, list,
          par_mesh->points[tri_vert1_idx * 3 + 0] * scale + offset_x,
          par_mesh->points[tri_vert1_idx * 3 + 1] * scale + offset_y,
          par_mesh->points[tri_vert1_idx * 3 + 2] * scale + offset_z,
          1
        );
        reiiCommandMeshColor(ctx, list, i * 0.0025f, 0, 0.1f, 1);
        reiiCommandMeshPosition(ctx, list,
          par_mesh->points[tri_vert2_idx * 3 + 0] * scale + offset_x,
          par_mesh->points[tri_vert2_idx * 3 + 1] * scale + offset_y,
          par_mesh->points[tri_vert2_idx * 3 + 2] * scale + offset_z,
          1
        );
      }
    }
    // Draw a scene of par meshes
    {
      const par_shapes_mesh * par_meshes[] = {
        open_cylinder,
        disk_cylinder,
        dome,
        rect,
        donut
      };

      float scale = 1.f;
      float offset_x = 6.f;
      float offset_y = 0.f;
      float offset_z = 0.f;

      for (int j = 0; j < countof(par_meshes); j += 1) {
        const par_shapes_mesh * par_mesh = par_meshes[j];

        for (int i = 0; i < par_mesh->ntriangles; i += 1) {
          const uint64_t tri_vert0_idx = par_mesh->triangles[i * 3 + 0];
          const uint64_t tri_vert1_idx = par_mesh->triangles[i * 3 + 1];
          const uint64_t tri_vert2_idx = par_mesh->triangles[i * 3 + 2];

          reiiCommandMeshColor(ctx, list, i * 0.0025f, 0, 0.1f, 1);
          reiiCommandMeshPosition(ctx, list,
            par_mesh->points[tri_vert0_idx * 3 + 0] * scale + offset_x,
            par_mesh->points[tri_vert0_idx * 3 + 1] * scale + offset_y,
            par_mesh->points[tri_vert0_idx * 3 + 2] * scale + offset_z,
            1
          );
          reiiCommandMeshColor(ctx, list, i * 0.0025f, 0, 0.1f, 1);
          reiiCommandMeshPosition(ctx, list,
            par_mesh->points[tri_vert1_idx * 3 + 0] * scale + offset_x,
            par_mesh->points[tri_vert1_idx * 3 + 1] * scale + offset_y,
            par_mesh->points[tri_vert1_idx * 3 + 2] * scale + offset_z,
            1
          );
          reiiCommandMeshColor(ctx, list, i * 0.0025f, 0, 0.1f, 1);
          reiiCommandMeshPosition(ctx, list,
            par_mesh->points[tri_vert2_idx * 3 + 0] * scale + offset_x,
            par_mesh->points[tri_vert2_idx * 3 + 1] * scale + offset_y,
            par_mesh->points[tri_vert2_idx * 3 + 2] * scale + offset_z,
            1
          );
        }
      }
    }
    reiiCommandMeshEndWithTale64BytesAlign(ctx, list, outputdstex, outputmstex, outputmstex->texture);
    reiiCommandResolveMsaaColorTexture(ctx, list, outputmstex, outputtex);
    reiiCommandGammaCorrectColorTextureToTheInversePowerOf2(ctx, list, outputtex, doDoubleGammaCorrection, 1, &gammaCorrectionStaticState);
    vfBatchEnd(ctx, batch, FF, LL);

    uint64_t wait = vfAsyncBatchExecute(ctx, 1, &batch, FF, LL);
    vfAsyncWaitToFinish(ctx, wait, FF, LL);
    vfAsyncDrawImageRaw(ctx, outputtex->image.handle, NULL, FF, LL);
    vfAsyncDrawWaitToFinish(ctx, FF, LL);

    mouse_x_prev = mouse_x;
    mouse_y_prev = mouse_y;
    mouse_right_mouse_button_state_prev = mouse_right_mouse_button_state;
  }

  par_shapes_free_mesh(sphere);
  par_shapes_free_mesh(cube);
  par_shapes_free_mesh(dodecahedron);
  par_shapes_free_mesh(open_cylinder);
  par_shapes_free_mesh(disk_cylinder);
  par_shapes_free_mesh(dome);
  par_shapes_free_mesh(rect);
  par_shapes_free_mesh(donut);
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
  glfwTerminate();
}

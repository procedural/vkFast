#if 0
clang main.c ../../vkfast.c C:/RedGpuSDK/redgpu.c C:/RedGpuSDK/redgpu_2.c C:/RedGpuSDK/redgpu_32.c ../../extra/Banzai/vkfast_extra_banzai.c ../../extra/Banzai/vkfast_extra_banzai_pointer.c "../../extra/CPU GPU Array/vkfast_extra_cpu_gpu_array.c" ../../extra/REII/vkfast_extra_reii.c
exit
#endif

#include "../../vkfast.h"
#include "../../extra/Banzai/vkfast_extra_banzai_pointer.h"
#include "../../extra/REII/vkfast_extra_reii.h"

#include <stdio.h> // For printf
#include <math.h>  // For sin, cos

#include <shellscalingapi.h>   // For SetProcessDpiAwareness
#pragma comment(lib, "shcore") // For SetProcessDpiAwareness

#ifdef _WIN32
#define GLFW_INCLUDE_NONE
#define GLFW_EXPOSE_NATIVE_WIN32
#include "glfw-3.4.bin.WIN64/include/GLFW/glfw3.h" // https://github.com/glfw/glfw/releases/download/3.4/glfw-3.4.bin.WIN64.zip
#include "glfw-3.4.bin.WIN64/include/GLFW/glfw3native.h" // https://github.com/glfw/glfw/releases/download/3.4/glfw-3.4.bin.WIN64.zip
#pragma comment(lib, "../glfw-3.4.bin.WIN64/lib-vc2019/glfw3_mt.lib") // https://github.com/glfw/glfw/releases/download/3.4/glfw-3.4.bin.WIN64.zip
#pragma comment(lib, "User32.lib")
#pragma comment(lib, "Shell32.lib")
#pragma comment(lib, "Gdi32.lib")
#endif

#define countof(x) (sizeof(x) / sizeof((x)[0]))

#define FF __FILE__
#define LL __LINE__

#if defined(_MSC_VER) && defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif

void RandomInit() {
  srand(time(NULL));
}

float RandomRange(float min, float max) {
  float n = rand() / (float)RAND_MAX;
  return min + n * (max - min);
}

void vec3Mulf(float * out, const float * v, float f) {
  out[0] = v[0] * f;
  out[1] = v[1] * f;
  out[2] = v[2] * f;
}

void vec3Add(float * out, const float * v1, const float * v2) {
  out[0] = v1[0] + v2[0];
  out[1] = v1[1] + v2[1];
  out[2] = v1[2] + v2[2];
}

void vec3Cross(float * out, const float * v1, const float * v2) {
  float v1x = v1[0];
  float v1y = v1[1];
  float v1z = v1[2];

  float v2x = v2[0];
  float v2y = v2[1];
  float v2z = v2[2];

  out[0] = (v1y * v2z) - (v2y * v1z);
  out[1] = (v1z * v2x) - (v2z * v1x);
  out[2] = (v1x * v2y) - (v2x * v1y);
}

void quatFromAxisAngle(float * out, const float * axis, float angle_rad) {
  float s = (float)sin(angle_rad / 2.f);
  float c = (float)cos(angle_rad / 2.f);

  out[0] = axis[0] * s;
  out[1] = axis[1] * s;
  out[2] = axis[2] * s;
  out[3] = c;
}

void quatMul(float * out, const float * q1, const float * q2) {
  float q1x = q1[0];
  float q1y = q1[1];
  float q1z = q1[2];
  float q1w = q1[3];

  float q2x = q2[0];
  float q2y = q2[1];
  float q2z = q2[2];
  float q2w = q2[3];

  out[0] = q1x * q2w + q2x * q1w + (q1y * q2z - q2y * q1z);
  out[1] = q1y * q2w + q2y * q1w + (q1z * q2x - q2z * q1x);
  out[2] = q1z * q2w + q2z * q1w + (q1x * q2y - q2x * q1y);
  out[3] = q1w * q2w - (q1x * q2x + q1y * q2y + q1z * q2z);
}

void quatRotateVec3(float * out, const float * v, const float * q) {
  float v4[4] = { v[0],  v[1],  v[2], 0};
  float nq[4] = {-q[0], -q[1], -q[2], q[3]};

  float t1[4];
  quatMul(t1, q, v4);
  float t2[4];
  quatMul(t2, t1, nq);
  
  out[0] = t2[0];
  out[1] = t2[1];
  out[2] = t2[2];
}

void quatRotateVec3Fast(float * out, const float * v, const float * q) {
  // (cross(q.xyz, cross(q.xyz, v) + (v * q.w)) * 2.f) + v
  float vc[3] = {v[0], v[1], v[2]};
  vec3Cross(out, q, vc);
  float t1[3];
  vec3Mulf(t1, vc, q[3]);
  vec3Add(out, out, t1);
  vec3Cross(out, q, out);
  vec3Mulf(out, out, 2.f);
  vec3Add(out, out, vc);
}

static gpu_extra_cpu_gpu_array OffsetAllocateCpuGpuArray(uint64_t bytesCountToAllocate, gpu_storage_t * storage_cpu, uint64_t * storage_cpu_offset, gpu_storage_t * storage_gpu, uint64_t * storage_gpu_offset, const char * optionalFile, int optionalLine) {
  gpu_extra_banzai_pointer_t cpu_pointer = {0};
  gpu_extra_banzai_pointer_t gpu_pointer = {0};
  vfeBanzaiGetPointer(storage_cpu, storage_cpu_offset[0], &cpu_pointer, optionalFile, optionalLine);
  vfeBanzaiGetPointer(storage_gpu, storage_gpu_offset[0], &gpu_pointer, optionalFile, optionalLine);
  storage_cpu_offset[0] += bytesCountToAllocate;
  storage_gpu_offset[0] += bytesCountToAllocate;

  RedStructMemberArray cpu_array = {0};
  RedStructMemberArray gpu_array = {0};
  vfeBanzaiPointerGetRawLimited(&cpu_pointer, bytesCountToAllocate, &cpu_array, optionalFile, optionalLine);
  vfeBanzaiPointerGetRawLimited(&gpu_pointer, bytesCountToAllocate, &gpu_array, optionalFile, optionalLine);
  gpu_extra_cpu_gpu_array cpu_gpu_array = {0};
  cpu_gpu_array.cpu_ptr = cpu_pointer.mapped_void_ptr;
  cpu_gpu_array.cpu     = cpu_array;
  cpu_gpu_array.gpu     = gpu_array;

  return cpu_gpu_array;
}

int main() {
#ifdef __MINGW32__
  SetProcessDPIAware();
#else
  SetProcessDpiAwareness(PROCESS_PER_MONITOR_DPI_AWARE);
#endif

#if defined(_MSC_VER) && defined(_DEBUG)
  _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

  LARGE_INTEGER frequency = {0};
  REDGPU_2_EXPECTFL(QueryPerformanceFrequency(&frequency) == TRUE); // Query the frequency (ticks per second)

  int window_w = 1000;
  int window_h = 1000;

  glfwInit();
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  GLFWwindow * window = glfwCreateWindow(window_w, window_h, "[vkFast] REII Instancing", 0, 0);
  void * window_handle = (void *)glfwGetWin32Window(window);

  gpu_internal_memory_allocation_sizes_t memory_allocation_sizes = {0};
  memory_allocation_sizes.bytes_count_for_memory_storages_type_gpu_only         = (1024/*mb*/ * 1024 * 1024) - 64; // NOTE(Constantine)(Mar 20, 2026): '- 64' added for Intel iGPUs which can allocate not 1073741824, but 1073741820 max, lol.
  memory_allocation_sizes.bytes_count_for_memory_storages_type_cpu_upload       = (512/*mb*/ * 1024 * 1024);
  memory_allocation_sizes.bytes_count_for_memory_storages_type_cpu_readback     = (512/*mb*/ * 1024 * 1024);
  memory_allocation_sizes.bytes_count_for_memory_present_pixels_type_cpu_upload = 0;
  gpu_context_optional_parameters_t optional_parameters = {0};
  optional_parameters.internal_memory_allocation_sizes = &memory_allocation_sizes;

  // NOTE(Constantine): You can also define REDGPU_COMPILE_SWITCH_DEBUG to see extra errors.
  gpu_handle_context_t ctx = vfContextInit(1, &optional_parameters, FF, LL);
  vfWindowFullscreen(ctx, window_handle, "[vkFast] REII Instancing", window_w, window_h, 0, FF, LL);

  gpu_storage_t storage_gpu_only     = {0};
  gpu_storage_t storage_cpu_upload   = {0};
  gpu_storage_t storage_cpu_readback = {0};
  vfeBanzaiStoragesCreate(ctx, &storage_gpu_only, &storage_cpu_upload, &storage_cpu_readback, FF, LL);

  uint64_t storage_gpu_only_mem_offset = 0;
  uint64_t storage_cpu_upload_mem_offset = 0;
  uint64_t storage_cpu_readback_mem_offset = 0;

  gpu_extra_banzai_pointer_t pixels_gpu_only = {0};
  vfeBanzaiGetPointer(&storage_gpu_only, storage_gpu_only_mem_offset, &pixels_gpu_only, FF, LL);
  storage_gpu_only_mem_offset += (288/*mb*/ * 1024 * 1024);

  gpu_extra_cpu_gpu_array mesh_vertex_array = OffsetAllocateCpuGpuArray(
    64/*mb*/ * 1024 * 1024,
    &storage_cpu_upload, &storage_cpu_upload_mem_offset,
    &storage_gpu_only,   &storage_gpu_only_mem_offset,
    FF, LL
  );
  gpu_extra_cpu_gpu_array instance_color_array = OffsetAllocateCpuGpuArray(
    64/*mb*/ * 1024 * 1024,
    &storage_cpu_upload, &storage_cpu_upload_mem_offset,
    &storage_gpu_only,   &storage_gpu_only_mem_offset,
    FF, LL
  );
  gpu_extra_cpu_gpu_array instance_position_array = OffsetAllocateCpuGpuArray(
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

  ReiiMeshState mesh_state                                  = {0};
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
  gpu_extra_reii_mesh_state_compile_info_t mesh_state_to_compile = {0};
  mesh_state_to_compile.state                                  = &mesh_state;
  mesh_state_to_compile.state_multisample_count                = RED_MULTISAMPLE_COUNT_BITFLAG_4;
  mesh_state_to_compile.output_depth_stencil_enable            = 1;
  mesh_state_to_compile.output_depth_stencil_format            = RED_FORMAT_DEPTH_32_FLOAT;
  mesh_state_to_compile.output_color_format                    = RED_FORMAT_PRESENT_BGRA_8_8_8_8_UINT_TO_FLOAT_0_1;
  mesh_state_to_compile.variables_slot                         = 3;
  mesh_state_to_compile.variables_bytes_count                  = 2 * sizeof(ReiiVec4);
  mesh_state_to_compile.struct_members_count                   = countof(slots);
  mesh_state_to_compile.struct_members                         = slots;
  reiiMeshStateCompile(ctx, &mesh_state_to_compile);

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

  ReiiHandleStaticArray hmesh = {0};
  ReiiHandleStaticArray * mesh = &hmesh;
  mesh->position = mesh_vertex_array;
  reiiCreateStaticArray(ctx, mesh);
  reiiStaticArraySet(ctx, mesh);
  for (int i = 0, mesh_vertices_count = countof(mesh_vertices) / 3; i < mesh_vertices_count; i += 1) {
    reiiStaticArrayPosition(ctx, mesh, mesh_vertices[i * 3 + 0], mesh_vertices[i * 3 + 1], mesh_vertices[i * 3 + 2], 1);
  }
  reiiStaticArrayEnd(ctx, mesh);

  const int instanceCountX = 25;
  const int instanceCountY = 25;
  const int instanceCountZ = 25;
  RandomInit();

  ReiiHandleStaticArray hinstanceColors = {0};
  ReiiHandleStaticArray * instanceColors = &hinstanceColors;
  instanceColors->color = instance_color_array;
  reiiCreateStaticArray(ctx, instanceColors);
  reiiStaticArraySet(ctx, instanceColors);
  for (int k = 0; k < instanceCountZ; k += 1) {
    for (int j = 0; j < instanceCountY; j += 1) {
      for (int i = 0; i < instanceCountX; i += 1) {
        reiiStaticArrayColor(ctx, instanceColors, RandomRange(0.15f, 0.45f), RandomRange(0.15f, 0.45f), RandomRange(0.25f, 1.f), 1);
      }
    }
  }
  reiiStaticArrayEnd(ctx, instanceColors);

  ReiiHandleStaticArray hinstancePositions = {0};
  ReiiHandleStaticArray * instancePositions = &hinstancePositions;
  instancePositions->position = instance_position_array;
  reiiCreateStaticArray(ctx, instancePositions);
  reiiStaticArraySet(ctx, instancePositions);
  for (int k = 0; k < instanceCountZ; k += 1) {
    for (int j = 0; j < instanceCountY; j += 1) {
      for (int i = 0; i < instanceCountX; i += 1) {
        reiiStaticArrayPosition(ctx, instancePositions, i * 3.f, j * 3.f, k * 3.f, RandomRange(0.05f, 0.45f));
      }
    }
  }
  reiiStaticArrayEnd(ctx, instancePositions);

  uint64_t batch = 0;
  ReiiHandleCommandList hlist = {0};
  ReiiHandleCommandList * list = &hlist;
  Red2Output mutable_outputs_array[3]  = {0};
  list->mutable_outputs_array.items    = mutable_outputs_array;
  list->mutable_outputs_array.capacity = countof(mutable_outputs_array);

  ReiiVec4   camera_pos  = {0, 0, -2.f};
  ReiiVec4   camera_quat = {0, 0, 0, 1};
  ReiiBool32 camera_is_enabled = 1;

  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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

    LARGE_INTEGER t_start = {0};
    QueryPerformanceCounter(&t_start);

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
    const float camera_move_speed      = 0.5f;
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
    bindings_info.max_new_bindings_sets_count = 1;
    bindings_info.max_storage_binds_count     = 3;
    batch = vfBatchBegin(ctx, batch, &bindings_info, NULL, FF, LL);
    list->batch_id = batch;
    reiiCommandListReset(ctx, list);
    reiiCommandSetViewportEx(ctx, list, 0, 0, window_w, window_h, 0, 1);
    reiiCommandSetScissor(ctx, list, 0, 0, window_w, window_h);
    reiiCommandClearTexture(ctx, list, outputdstex, outputmstex, outputmstex->texture, REII_CLEAR_DEPTH_BIT | REII_CLEAR_COLOR_BIT, 0.f, 0, 0.f,0.f,0.05f,1.f);
    reiiCommandMeshSetState(ctx, list, &mesh_state, NULL);
    reiiCommandBindNewBindingsSet(ctx, list, countof(slots), slots);
    reiiCommandBindStorageRaw(ctx, list, 0, 1, &mesh->position.gpu);
    reiiCommandBindStorageRaw(ctx, list, 1, 1, &instanceColors->color.gpu);
    reiiCommandBindStorageRaw(ctx, list, 2, 1, &instancePositions->position.gpu);
    reiiCommandBindNewBindingsEnd(ctx, list);
    reiiCommandBindVariablesCopy(ctx, list, 0 * sizeof(ReiiVec4), 1 * sizeof(ReiiVec4), &camera_pos);
    reiiCommandBindVariablesCopy(ctx, list, 1 * sizeof(ReiiVec4), 1 * sizeof(ReiiVec4), &camera_quat);
    reiiCommandRenderTargetSet(ctx, list, outputdstex, outputmstex, outputmstex->texture);
    reiiCommandStaticArrayDrawInstanced(ctx, list, mesh, instanceCountX * instanceCountY * instanceCountZ);
    reiiCommandRenderTargetEnd(ctx, list);
    RedStructMemberArray raw_pixels = {0};
    vfeBanzaiPointerGetRaw(&pixels_gpu_only, &raw_pixels, FF, LL);
    reiiCommandResolveMsaaColorTexture(ctx, list, outputmstex, outputtex);
    reiiCommandCopyFromColorTextureToStorageRaw(ctx, list, outputtex, &raw_pixels);
    vfBatchEnd(ctx, batch, FF, LL);

    uint64_t wait = vfAsyncBatchExecute(ctx, 1, &batch, FF, LL);
    vfAsyncWaitToFinish(ctx, wait, FF, LL);
    vfAsyncDrawPixelsRaw(ctx, &raw_pixels, NULL, FF, LL);
    vfAsyncDrawWaitToFinish(ctx, FF, LL);

    mouse_x_prev = mouse_x;
    mouse_y_prev = mouse_y;
    mouse_right_mouse_button_state_prev = mouse_right_mouse_button_state;

    LARGE_INTEGER t_end = {0};
    QueryPerformanceCounter(&t_end);

    {
      LONGLONG elapsedTicks = t_end.QuadPart - t_start.QuadPart;
      LONGLONG nanoseconds = (elapsedTicks * 1000000000LL) / frequency.QuadPart;
      double milliseconds_fp = (double)(nanoseconds) / 1000000.0;
      //printf("Elapsed milliseconds: %f\n", milliseconds_fp);
    }
  }

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

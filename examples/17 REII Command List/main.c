#if 0
clang main.c ../../vkfast.c C:/RedGpuSDK/redgpu.c C:/RedGpuSDK/redgpu_2.c C:/RedGpuSDK/redgpu_32.c ../../extra/Banzai/vkfast_extra_banzai.c ../../extra/Banzai/vkfast_extra_banzai_pointer.c ../../extra/REII/vkfast_extra_reii.c
exit
#endif

#include "../../vkfast.h"
#include "../../extra/Banzai/vkfast_extra_banzai_pointer.h"
#include "../../extra/REII/vkfast_extra_reii.h"

#include <stdio.h> // For printf
#include <math.h>  // For sin, cos

#include <shellscalingapi.h>   // For SetProcessDpiAwareness
#pragma comment(lib, "shcore") // For SetProcessDpiAwareness

#define countof(x) (sizeof(x) / sizeof((x)[0]))

#define FF __FILE__
#define LL __LINE__

#if defined(_MSC_VER) && defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif

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

  #define window_w 1920
  #define window_h 1080

  int windowMonitorArea[4] = {0};
  vfGetMainMonitorAreaRectangle(windowMonitorArea, FF, LL);
  REDGPU_2_EXPECTFL(windowMonitorArea[2] == window_w);
  REDGPU_2_EXPECTFL(windowMonitorArea[3] == window_h);

  gpu_internal_memory_allocation_sizes_t memory_allocation_sizes = {0};
  memory_allocation_sizes.bytes_count_for_memory_storages_type_gpu_only         = (1024/*mb*/ * 1024 * 1024);
  memory_allocation_sizes.bytes_count_for_memory_storages_type_cpu_upload       = (512/*mb*/ * 1024 * 1024);
  memory_allocation_sizes.bytes_count_for_memory_storages_type_cpu_readback     = (512/*mb*/ * 1024 * 1024);
  memory_allocation_sizes.bytes_count_for_memory_present_pixels_type_cpu_upload = 0;
  gpu_context_optional_parameters_t optional_parameters = {0};
  optional_parameters.internal_memory_allocation_sizes = &memory_allocation_sizes;

  gpu_handle_context_t ctx = vfContextInit(1, &optional_parameters, FF, LL);
  vfWindowFullscreen(ctx, NULL, "[vkFast] REII Command List", window_w, window_h, 0, FF, LL);

  gpu_storage_t storage_gpu_only     = {0};
  gpu_storage_t storage_cpu_upload   = {0};
  gpu_storage_t storage_cpu_readback = {0};
  vfeBanzaiStoragesCreate(ctx, &storage_gpu_only, &storage_cpu_upload, &storage_cpu_readback, FF, LL);

  uint64_t storage_gpu_only_mem_offset = 0;
  uint64_t storage_cpu_upload_mem_offset = 0;
  uint64_t storage_cpu_readback_mem_offset = 0;

  uint64_t                   pixels_gpu_only_bytes_count = (288/*mb*/ * 1024 * 1024);
  gpu_extra_banzai_pointer_t pixels_gpu_only             = {0};
  vfeBanzaiGetPointer(&storage_gpu_only, storage_gpu_only_mem_offset, &pixels_gpu_only, FF, LL);
  storage_gpu_only_mem_offset += pixels_gpu_only_bytes_count;

  uint64_t                   dynamic_mesh_pos_bytes_count = (64/*mb*/ * 1024 * 1024);
  gpu_extra_banzai_pointer_t dynamic_mesh_pos_cpu_upload  = {0};
  gpu_extra_banzai_pointer_t dynamic_mesh_pos_gpu_only    = {0};
  vfeBanzaiGetPointer(&storage_cpu_upload, storage_cpu_upload_mem_offset, &dynamic_mesh_pos_cpu_upload, FF, LL);
  storage_cpu_upload_mem_offset += dynamic_mesh_pos_bytes_count;
  vfeBanzaiGetPointer(&storage_gpu_only, storage_gpu_only_mem_offset, &dynamic_mesh_pos_gpu_only, FF, LL);
  storage_gpu_only_mem_offset += dynamic_mesh_pos_bytes_count;

  uint64_t                   dynamic_mesh_col_bytes_count = (64/*mb*/ * 1024 * 1024);
  gpu_extra_banzai_pointer_t dynamic_mesh_col_cpu_upload  = {0};
  gpu_extra_banzai_pointer_t dynamic_mesh_col_gpu_only    = {0};
  vfeBanzaiGetPointer(&storage_cpu_upload, storage_cpu_upload_mem_offset, &dynamic_mesh_col_cpu_upload, FF, LL);
  storage_cpu_upload_mem_offset += dynamic_mesh_col_bytes_count;
  vfeBanzaiGetPointer(&storage_gpu_only, storage_gpu_only_mem_offset, &dynamic_mesh_col_gpu_only, FF, LL);
  storage_gpu_only_mem_offset += dynamic_mesh_col_bytes_count;

  RedStructMemberArray raw_pos_cpu_upload = {0};
  vfeBanzaiPointerGetRawLimited(&dynamic_mesh_pos_cpu_upload, dynamic_mesh_pos_bytes_count, &raw_pos_cpu_upload, FF, LL);
  RedStructMemberArray raw_pos_gpu_only = {0};
  vfeBanzaiPointerGetRawLimited(&dynamic_mesh_pos_gpu_only, dynamic_mesh_pos_bytes_count, &raw_pos_gpu_only, FF, LL);
  ReiiCpuGpuArray pos_array = {0};
  pos_array.cpu_ptr = dynamic_mesh_pos_cpu_upload.mapped_void_ptr;
  pos_array.cpu     = raw_pos_cpu_upload;
  pos_array.gpu     = raw_pos_gpu_only;

  RedStructMemberArray raw_col_cpu_upload = {0};
  vfeBanzaiPointerGetRawLimited(&dynamic_mesh_col_cpu_upload, dynamic_mesh_col_bytes_count, &raw_col_cpu_upload, FF, LL);
  RedStructMemberArray raw_col_gpu_only = {0};
  vfeBanzaiPointerGetRawLimited(&dynamic_mesh_col_gpu_only, dynamic_mesh_col_bytes_count, &raw_col_gpu_only, FF, LL);
  ReiiCpuGpuArray col_array = {0};
  col_array.cpu_ptr = dynamic_mesh_col_cpu_upload.mapped_void_ptr;
  col_array.cpu     = raw_col_cpu_upload;
  col_array.gpu     = raw_col_gpu_only;

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
  mesh_state.multisampleEnable                              = 0;
  mesh_state.multisampleAlphaToCoverageEnable               = 0;
  mesh_state.multisampleAlphaToOneEnable                    = 0;
  mesh_state.depthTestEnable                                = 0;
  mesh_state.depthTestDepthWriteEnable                      = 0;
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
  RedStructDeclarationMember slots[2] = {0};
  slots[0].slot            = 0;
  slots[0].type            = RED_STRUCT_MEMBER_TYPE_ARRAY_RO_RW;
  slots[0].count           = 1;
  slots[0].visibleToStages = RED_VISIBLE_TO_STAGE_BITFLAG_VERTEX;
  slots[1].slot            = 1;
  slots[1].type            = RED_STRUCT_MEMBER_TYPE_ARRAY_RO_RW;
  slots[1].count           = 1;
  slots[1].visibleToStages = RED_VISIBLE_TO_STAGE_BITFLAG_VERTEX;
  gpu_extra_reii_mesh_state_compile_info_t mesh_state_to_compile = {0};
  mesh_state_to_compile.state                                  = &mesh_state;
  mesh_state_to_compile.state_multisample_count                = RED_MULTISAMPLE_COUNT_BITFLAG_1;
  mesh_state_to_compile.output_depth_stencil_enable            = 0;
  mesh_state_to_compile.output_depth_stencil_format            = RED_FORMAT_DEPTH_32_FLOAT;
  mesh_state_to_compile.output_color_format                    = RED_FORMAT_PRESENT_BGRA_8_8_8_8_UINT_TO_FLOAT_0_1;
  mesh_state_to_compile.variables_slot                         = 2;
  mesh_state_to_compile.variables_bytes_count                  = 0;
  mesh_state_to_compile.struct_members_count                   = countof(slots);
  mesh_state_to_compile.struct_members                         = slots;
  reiiMeshStateCompile(ctx, &mesh_state_to_compile);

  ReiiHandleTextureMemory outputDSTexMemory = {0};
  reiiCreateTextureMemory(ctx, GPU_EXTRA_REII_TEXTURE_TYPE_OUTPUT_DEPTH_STENCIL, (288/*mb*/ * 1024 * 1024), &outputDSTexMemory);
  ReiiHandleTexture houtputdstex = {0};
  ReiiHandleTexture * outputdstex = &houtputdstex;
  reiiCreateTextureFromTextureMemory(ctx, &outputDSTexMemory, REII_TEXTURE_BINDING_2D, &houtputdstex);
  reiiTextureSetStateMipmap(ctx, REII_TEXTURE_BINDING_2D, outputdstex, 0);
  reiiTextureSetStateMipmapLevelsCount(ctx, REII_TEXTURE_BINDING_2D, outputdstex, 1);
  reiiTextureSetStateSampler(ctx, REII_TEXTURE_BINDING_2D, outputdstex, REII_SAMPLER_FILTERING_LINEAR, REII_SAMPLER_FILTERING_LINEAR, REII_SAMPLER_BEHAVIOR_OUTSIDE_TEXTURE_COORDINATE_REPEAT, REII_SAMPLER_BEHAVIOR_OUTSIDE_TEXTURE_COORDINATE_REPEAT, 1);
  reiiTextureDefineAndCopyFromCpu(ctx, REII_TEXTURE_BINDING_2D, outputdstex, 0, REII_TEXTURE_TEXEL_FORMAT_DS, window_w, window_h, REII_TEXTURE_TEXEL_FORMAT_DS, REII_TEXTURE_TEXEL_TYPE_FLOAT, 4, NULL);

  ReiiHandleTextureMemory outputTexMemory = {0};
  reiiCreateTextureMemory(ctx, GPU_EXTRA_REII_TEXTURE_TYPE_OUTPUT_COLOR, (288/*mb*/ * 1024 * 1024), &outputTexMemory);
  ReiiHandleTexture houtputtex = {0};
  ReiiHandleTexture * outputtex = &houtputtex;
  reiiCreateTextureFromTextureMemory(ctx, &outputTexMemory, REII_TEXTURE_BINDING_2D, &houtputtex);
  reiiTextureSetStateMipmap(ctx, REII_TEXTURE_BINDING_2D, outputtex, 0);
  reiiTextureSetStateMipmapLevelsCount(ctx, REII_TEXTURE_BINDING_2D, outputtex, 1);
  reiiTextureSetStateSampler(ctx, REII_TEXTURE_BINDING_2D, outputtex, REII_SAMPLER_FILTERING_LINEAR, REII_SAMPLER_FILTERING_LINEAR, REII_SAMPLER_BEHAVIOR_OUTSIDE_TEXTURE_COORDINATE_REPEAT, REII_SAMPLER_BEHAVIOR_OUTSIDE_TEXTURE_COORDINATE_REPEAT, 1);
  reiiTextureDefineAndCopyFromCpu(ctx, REII_TEXTURE_BINDING_2D, outputtex, 0, REII_TEXTURE_TEXEL_FORMAT_RGBA, window_w, window_h, REII_TEXTURE_TEXEL_FORMAT_RGBA, REII_TEXTURE_TEXEL_TYPE_U8, 4, NULL);

  float mesh_vertices[] = {
    #include "../../extra/3D Mesh Suzanne Head/3d_mesh_vertices_suzanne_head.h"
  };

  uint64_t batch = 0;
  ReiiHandleCommandList hlist = {0};
  ReiiHandleCommandList * list = &hlist;
  Red2Output mutable_outputs_array[1]  = {0};
  list->mutable_outputs_array.items    = mutable_outputs_array;
  list->mutable_outputs_array.capacity = _countof(mutable_outputs_array);
  list->dynamic_mesh_position          = pos_array;
  list->dynamic_mesh_color             = col_array;

  while (vfWindowLoop(ctx)) {
    LARGE_INTEGER t_start = {0};
    QueryPerformanceCounter(&t_start);

    gpu_batch_info_t bindings_info = {0};
    bindings_info.max_new_bindings_sets_count = 1;
    bindings_info.max_storage_binds_count     = 2;
    batch = vfBatchBegin(ctx, batch, &bindings_info, NULL, FF, LL);
    list->batch_id = batch;
    reiiCommandListReset(ctx, list);
    reiiCommandSetViewportExt(ctx, list, 0, 0, window_w, window_h, 0, 1);
    reiiCommandSetScissor(ctx, list, 0, 0, window_w, window_h);
    reiiCommandClearTexture(ctx, list, outputdstex, outputtex, outputtex->texture, REII_CLEAR_DEPTH_BIT | REII_CLEAR_COLOR_BIT, 0.f, 0, 0.f,0.f,0.05f,1.f);
    reiiCommandMeshSetState(ctx, list, &mesh_state, NULL);
    reiiCommandBindNewBindingsSet(ctx, list, countof(slots), slots);
    reiiCommandBindStorageRaw(ctx, list, 0, 1, &pos_array.gpu);
    reiiCommandBindStorageRaw(ctx, list, 1, 1, &col_array.gpu);
    reiiCommandBindNewBindingsEnd(ctx, list);
    reiiCommandMeshSet(ctx, list);
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
    reiiCommandMeshEndExt(ctx, list, outputdstex, outputtex, outputtex->texture);
    vfBatchEnd(ctx, batch, FF, LL);

    uint64_t wait = vfAsyncBatchExecute(ctx, 1, &batch, FF, LL);
    vfAsyncWaitToFinish(ctx, wait, FF, LL);
    RedStructMemberArray raw_pixels = {0};
    vfeBanzaiPointerGetRaw(&pixels_gpu_only, &raw_pixels, FF, LL);
    vfAsyncDrawPixelsRaw(ctx, &raw_pixels, NULL, FF, LL);
    vfAsyncDrawWaitToFinish(ctx, FF, LL);

    LARGE_INTEGER t_end = {0};
    QueryPerformanceCounter(&t_end);

    {
      LONGLONG elapsedTicks = t_end.QuadPart - t_start.QuadPart;
      LONGLONG nanoseconds = (elapsedTicks * 1000000000LL) / frequency.QuadPart;
      double milliseconds_fp = (double)(nanoseconds) / 1000000.0;
      printf("Elapsed milliseconds: %f\n", milliseconds_fp);
    }
  }

  reiiDestroyCommandList(ctx, list);
  uint64_t ids[] = {
    storage_gpu_only.id,
    storage_cpu_upload.id,
    storage_cpu_readback.id,
  };
  vfIdDestroy(countof(ids), ids, FF, LL);
  vfContextDeinit(ctx, FF, LL);
  vfExit(0);
}

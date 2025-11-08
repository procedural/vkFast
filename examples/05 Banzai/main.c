// gcc main.c ../../vkfast.c C:/RedGpuSDK/redgpu.c C:/RedGpuSDK/redgpu_2.c C:/RedGpuSDK/redgpu_32.c ../../extra/Banzai/vkfast_extra_banzai.c ../../extra/Banzai/vkfast_extra_banzai_pointer.c

#include "../../vkfast.h"
#include "../../extra/Banzai/vkfast_extra_banzai_pointer.h"

#include <stdio.h> // For printf

#define countof(x) (sizeof(x) / sizeof((x)[0]))

#define FF __FILE__
#define LL __LINE__

int main() {
  gpu_handle_context_t ctx = vfContextInit(1, NULL, FF, LL);

  gpu_storage_t storage_gpu_only     = {0};
  gpu_storage_t storage_cpu_upload   = {0};
  gpu_storage_t storage_cpu_readback = {0};

  vfeBanzaiStoragesCreate(ctx, &storage_gpu_only, &storage_cpu_upload, &storage_cpu_readback, FF, LL);
  
  gpu_extra_banzai_pointer_t storage_input_cpu = {0};
  gpu_extra_banzai_pointer_t storage_input_gpu = {0};
  vfeBanzaiGetPointer(&storage_cpu_upload, 0, &storage_input_cpu, FF, LL);
  vfeBanzaiGetPointer(&storage_gpu_only,   0, &storage_input_gpu, FF, LL);

  gpu_extra_banzai_pointer_t storage_output_cpu = {0};
  gpu_extra_banzai_pointer_t storage_output_gpu = {0};
  vfeBanzaiGetPointer(&storage_cpu_readback, 0,                   &storage_output_cpu, FF, LL);
  vfeBanzaiGetPointer(&storage_gpu_only,     2 * 4*sizeof(float), &storage_output_gpu, FF, LL);

  storage_input_cpu.as_vec4[0].x = 4;
  storage_input_cpu.as_vec4[0].y = 8;
  storage_input_cpu.as_vec4[0].z = 15;
  storage_input_cpu.as_vec4[0].w = 16;
  
  storage_input_cpu.as_vec4[1].x = 16;
  storage_input_cpu.as_vec4[1].y = 23;
  storage_input_cpu.as_vec4[1].z = 42;
  storage_input_cpu.as_vec4[1].w = 108;
    
  uint64_t copy = vfBatchBegin(ctx, 0, NULL, NULL, FF, LL);
  vfeBanzaiBatchPointerCopyFromCpuToGpu(ctx, copy, &storage_input_cpu, &storage_input_gpu, 2 * 4*sizeof(float), FF, LL);
  vfBatchEnd(ctx, copy, FF, LL);
  uint64_t async = vfAsyncBatchExecute(ctx, 1, &copy, FF, LL);
  vfAsyncWaitToFinish(ctx, async, FF, LL);

  #include "add.cs.h"
  gpu_program_info_t cs_info = {0};
  cs_info.program_binary_bytes_count = sizeof(g_main);
  cs_info.program_binary             = g_main;
  uint64_t cs = vfProgramCreateFromBinaryCompute(ctx, &cs_info, FF, LL);

  struct Variables {
    unsigned ptr_offset_storage_input_gpu;
    unsigned ptr_offset_storage_output_gpu;
    unsigned _0;
    unsigned _1;
    float salt_x;
    float salt_y;
    float salt_z;
    float salt_w;
  };

  RedStructDeclarationMember slots[1] = {0};
  slots[0].slot            = 0;
  slots[0].type            = RED_STRUCT_MEMBER_TYPE_ARRAY_RO_RW;
  slots[0].count           = 1;
  slots[0].visibleToStages = RED_VISIBLE_TO_STAGE_BITFLAG_COMPUTE;
  gpu_program_pipeline_compute_info_t pp_info = {0};
  pp_info.compute_program       = cs;
  pp_info.variables_slot        = 2;
  pp_info.variables_bytes_count = sizeof(struct Variables);
  pp_info.struct_members_count  = countof(slots);
  pp_info.struct_members        = slots;
  uint64_t pp = vfProgramPipelineCreateCompute(ctx, &pp_info, FF, LL);
  
  uint64_t batch = 0;

  {
    gpu_batch_info_t bindings_info = {0};
    bindings_info.max_new_bindings_sets_count = 1;
    bindings_info.max_storage_binds_count     = 1;
    batch = vfBatchBegin(ctx, batch, &bindings_info, NULL, FF, LL);
    vfBatchBindProgramPipelineCompute(ctx, batch, pp, FF, LL);
    vfBatchBindNewBindingsSet(ctx, batch, countof(slots), slots, FF, LL);
    vfBatchBindStorage(ctx, batch, 0, 1, &storage_gpu_only.id, FF, LL);
    vfBatchBindNewBindingsEnd(ctx, batch, FF, LL);
    struct Variables variables = {0};
    variables.ptr_offset_storage_input_gpu  = (unsigned)(uint64_t)storage_input_gpu.bytes_first;
    variables.ptr_offset_storage_output_gpu = (unsigned)(uint64_t)storage_output_gpu.bytes_first;
    variables.salt_x = 0;
    variables.salt_y = -1;
    variables.salt_z = -7;
    variables.salt_w = 6;
    vfBatchBindVariablesCopy(ctx, batch, sizeof(variables), &variables, FF, LL);
    vfBatchCompute(ctx, batch, 1, 1, 1, FF, LL);
    vfBatchBarrierMemory(ctx, batch, FF, LL);
    vfeBanzaiBatchPointerCopyFromGpuToCpu(ctx, batch, &storage_output_gpu, &storage_output_cpu, 1 * 4*sizeof(float), FF, LL);
    vfBatchBarrierCpuReadback(ctx, batch, FF, LL);
    vfBatchEnd(ctx, batch, FF, LL);

    uint64_t wait = vfAsyncBatchExecute(ctx, 1, &batch, FF, LL);
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
  }
  
  uint64_t ids[] = {
    storage_gpu_only.id,
    storage_cpu_upload.id,
    storage_cpu_readback.id,
    copy,
    cs,
    pp,
    batch,
  };
  vfIdDestroy(countof(ids), ids, FF, LL);
  vfContextDeinit(ctx, FF, LL);
}

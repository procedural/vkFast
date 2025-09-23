#include "../../vkfast.h"

#include <stdio.h> // For printf

#define countof(x) (sizeof(x) / sizeof((x)[0]))

#define FF __FILE__
#define LL __LINE__

int main() {
  const int window_w = 1920;
  const int window_h = 1080;

  vfContextInit(1, NULL, FF, LL);
  vfWindowFullscreen(NULL, "[vkFast] Hello Compute", window_w, window_h, FF, LL);

  gpu_storage_info_t storage_info = {0};
  storage_info.storage_type = GPU_STORAGE_TYPE_CPU_UPLOAD;
  storage_info.bytes_count  = 2 * 4*sizeof(float);
  gpu_storage_t storage_input_cpu = {0};
  vfStorageCreate(&storage_info, &storage_input_cpu, FF, LL);
  storage_info.storage_type = GPU_STORAGE_TYPE_GPU_ONLY;
  gpu_storage_t storage_input_gpu = {0};
  vfStorageCreate(&storage_info, &storage_input_gpu, FF, LL);
  
  storage_input_cpu.as_vec4[0].x = 4;
  storage_input_cpu.as_vec4[0].y = 8;
  storage_input_cpu.as_vec4[0].z = 15;
  storage_input_cpu.as_vec4[0].w = 16;
  
  storage_input_cpu.as_vec4[1].x = 16;
  storage_input_cpu.as_vec4[1].y = 23;
  storage_input_cpu.as_vec4[1].z = 42;
  storage_input_cpu.as_vec4[1].w = 108;
    
  uint64_t copy = vfBatchBegin(0, NULL, NULL, FF, LL);
  vfBatchStorageCopyFromCpuToGpu(copy, storage_input_cpu.id, storage_input_gpu.id, FF, LL);
  vfBatchEnd(copy, FF, LL);
  uint64_t async = vfAsyncBatchExecute(1, &copy, 0, FF, LL);
  vfAsyncWaitToFinish(async, FF, LL);

  gpu_storage_info_t storage_output_info = {0};
  storage_output_info.storage_type = GPU_STORAGE_TYPE_CPU_READBACK;
  storage_output_info.bytes_count  = 1 * 4*sizeof(float);
  gpu_storage_t storage_output_cpu = {0};
  vfStorageCreate(&storage_output_info, &storage_output_cpu, FF, LL);
  storage_output_info.storage_type = GPU_STORAGE_TYPE_GPU_ONLY;
  gpu_storage_t storage_output_gpu = {0};
  vfStorageCreate(&storage_output_info, &storage_output_gpu, FF, LL);

  #include "add.cs.h"
  gpu_program_info_t cs_info = {0};
  cs_info.program_binary_bytes_count = sizeof(g_main);
  cs_info.program_binary             = g_main;
  uint64_t cs = vfProgramCreateFromBinaryCompute(&cs_info, FF, LL);

  RedStructDeclarationMember slot0 = {0};
  slot0.slot            = 0;
  slot0.type            = RED_STRUCT_MEMBER_TYPE_ARRAY_RO_RW;
  slot0.count           = 1;
  slot0.visibleToStages = RED_VISIBLE_TO_STAGE_BITFLAG_COMPUTE;
  RedStructDeclarationMember slot1 = {0};
  slot1.slot            = 1;
  slot1.type            = RED_STRUCT_MEMBER_TYPE_ARRAY_RO_RW;
  slot1.count           = 1;
  slot1.visibleToStages = RED_VISIBLE_TO_STAGE_BITFLAG_COMPUTE;
  const RedStructDeclarationMember slots[] = {slot0, slot1};
  gpu_program_pipeline_compute_info_t pp_info = {0};
  pp_info.compute_program       = cs;
  pp_info.variables_slot        = 2;
  pp_info.variables_bytes_count = 1 * 4*sizeof(float);
  pp_info.struct_members_count  = countof(slots);
  pp_info.struct_members        = slots;
  uint64_t pp = vfProgramPipelineCreateCompute(&pp_info, FF, LL);
  
  uint64_t batch = 0;

  while (vfWindowLoop()) {
    gpu_batch_info_t bindings_info = {0};
    bindings_info.max_new_bindings_sets_count = 1;
    bindings_info.max_storage_binds_count     = 2;
    batch = vfBatchBegin(batch, &bindings_info, NULL, FF, LL);
    vfBatchBindProgramPipelineCompute(batch, pp, FF, LL);
    vfBatchBindNewBindingsSet(batch, countof(slots), slots, FF, LL);
    vfBatchBindStorage(batch, 0, 1, &storage_input_gpu.id, FF, LL);
    vfBatchBindStorage(batch, 1, 1, &storage_output_gpu.id, FF, LL);
    vfBatchBindNewBindingsEnd(batch, FF, LL);
    float salt[4] = {0};
    salt[0] = 0;
    salt[1] = -1;
    salt[2] = -7;
    salt[3] = 6;
    vfBatchBindVariablesCopy(batch, sizeof(salt), salt, FF, LL);
    vfBatchCompute(batch, 1, 1, 1, FF, LL);
    vfBatchBarrierMemory(batch, FF, LL);
    vfBatchStorageCopyFromGpuToCpu(batch, storage_output_gpu.id, storage_output_cpu.id, FF, LL);
    vfBatchBarrierCpuReadback(batch, FF, LL);
    vfBatchEnd(batch, FF, LL);

    uint64_t wait = vfAsyncBatchExecute(1, &batch, 1, FF, LL);
    vfAsyncWaitToFinish(wait, FF, LL);

    // NOTE(Constantine): Expected result: 20 30 50 130 (20 31 57 124 + salt)"
    printf("Result: %f %f %f %f\n",
      storage_output_cpu.as_vec4[0].x,
      storage_output_cpu.as_vec4[0].y,
      storage_output_cpu.as_vec4[0].z,
      storage_output_cpu.as_vec4[0].w
    );
  }
  
  uint64_t ids[] = {
    storage_input_cpu.id,
    storage_input_gpu.id,
    copy,
    storage_output_cpu.id,
    storage_output_gpu.id,
    cs,
    pp,
    batch,
  };
  vfIdDestroy(countof(ids), ids, FF, LL);
  vfContextDeinit(FF, LL);
  vfExit(0);
}

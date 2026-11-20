#if 0
set -x

gcc $@ main.c ../../vkfast.c /home/linuxbrew/RedGpuSDK/redgpu.c /home/linuxbrew/RedGpuSDK/redgpu_2.c /home/linuxbrew/RedGpuSDK/redgpu_32.c -I/var/home/linuxbrew/.linuxbrew/Cellar/libxcb/1.17.0/include/ /home/linuxbrew/.linuxbrew/lib/libX11.so /home/linuxbrew/.linuxbrew/lib/libvulkan.so -lm

exit
#endif

#include "../../vkfast.h"
#include "../../extra/vkFast Extensions/ReBAR/vkfast_ext_rebar.h"
#include "../Common/vkfast_examples_common.h"

int main() {
#if defined(_MSC_VER) && defined(_DEBUG)
  _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

  gpu_handle_context_t ctx = vfContextInit(1, NULL, FF, LL);

  const unsigned array65536[2] = {65536, 65536};

  gpu_thread_t gpu_thread = NULL;
  vfGpuThreadCreate(ctx, 1, &gpu_thread, NULL, FF, LL);

  struct float4 {
    float x, y, z, w;
  };

  VfeReBARMallocShared storageInputSharedDataHandles = {0};
  struct float4 * storage_input = (struct float4 *)vfeReBARMallocShared(ctx, 2 * sizeof(struct float4), &storageInputSharedDataHandles);

  storage_input[0].x = 4;
  storage_input[0].y = 8;
  storage_input[0].z = 15;
  storage_input[0].w = 16;

  storage_input[1].x = 16;
  storage_input[1].y = 23;
  storage_input[1].z = 42;
  storage_input[1].w = 108;

  VfeReBARMallocShared storageOutputSharedDataHandles = {0};
  struct float4 * storage_output = (struct float4 *)vfeReBARMallocShared(ctx, 1 * sizeof(struct float4), &storageOutputSharedDataHandles);

  #include "add.cs.h"
  gpu_program_info_t cs_info = {0};
  cs_info.program_binary_bytes_count = sizeof(g_main);
  cs_info.program_binary             = g_main;
  uint64_t cs = vfProgramCreateFromBinaryCompute(ctx, &cs_info, FF, LL);

  RedStructDeclarationMember slots[2] = {0};
  slots[0].slot            = 0;
  slots[0].type            = RED_STRUCT_MEMBER_TYPE_ARRAY_RO_RW;
  slots[0].count           = 1;
  slots[0].visibleToStages = RED_VISIBLE_TO_STAGE_BITFLAG_COMPUTE;

  slots[1].slot            = 1;
  slots[1].type            = RED_STRUCT_MEMBER_TYPE_ARRAY_RO_RW;
  slots[1].count           = 1;
  slots[1].visibleToStages = RED_VISIBLE_TO_STAGE_BITFLAG_COMPUTE;
  gpu_program_pipeline_info_t pp_info = {0};
  pp_info.variables_slot        = 2;
  pp_info.variables_bytes_count = 1 * 4*sizeof(float);
  pp_info.struct_members_count  = countof(slots);
  pp_info.struct_members        = slots;
  uint64_t pp = vfProgramPipelineCreateCompute(ctx, cs, &pp_info, NULL, FF, LL);

  uint64_t batch = 0;

  {
    gpu_batch_info_t bindings_info = {0};
    bindings_info.max_new_bindings_sets_count = 1;
    bindings_info.max_storage_binds_count     = 2;
    batch = vfBatchBegin(ctx, batch, &bindings_info, NULL, FF, LL);
    vfBatchBindProgramPipelineCompute(ctx, batch, pp, FF, LL);
    vfBatchBindNewBindingsSet(ctx, batch, countof(slots), slots, FF, LL);
    vfBatchBindStorageRaw(ctx, batch, 0, 1, &storageInputSharedDataHandles.storageRaw, FF, LL);
    vfBatchBindStorageRaw(ctx, batch, 1, 1, &storageOutputSharedDataHandles.storageRaw, FF, LL);
    vfBatchBindNewBindingsEnd(ctx, batch, FF, LL);
    float salt[4] = {0};
    salt[0] = 0;
    salt[1] = -1;
    salt[2] = -7;
    salt[3] = 6;
    vfBatchBindVariablesCopy(ctx, batch, 0, sizeof(salt), salt, FF, LL);
    vfBatchCompute(ctx, batch, 1, 1, 1, FF, LL);
    vfBatchBarrierCpuReadback(ctx, batch, FF, LL);
    vfBatchEnd(ctx, batch, FF, LL);

    RedHandleCalls batchRaw = vfBatchGetRawHandle(ctx, batch, FF, LL);
    uint64_t wait = vfAsyncBatchExecuteRaw(ctx, 1, &batchRaw, 1, &gpu_thread, array65536, FF, LL);
    vfAsyncWaitToFinish(ctx, wait, FF, LL);

    // NOTE(Constantine): Expected result: 20 30 50 130 (20 31 57 124 + salt)"
    printf("Result: %f %f %f %f\n",
      storage_output[0].x,
      storage_output[0].y,
      storage_output[0].z,
      storage_output[0].w
    );
    REDGPU_2_EXPECTFL(storage_output[0].x == 20);
    REDGPU_2_EXPECTFL(storage_output[0].y == 30);
    REDGPU_2_EXPECTFL(storage_output[0].z == 50);
    REDGPU_2_EXPECTFL(storage_output[0].w == 130);
  }
  
  vfAllQueuesWaitIdle(ctx, FF, LL);

  vfGpuThreadDestroy(ctx, gpu_thread);
  vfeReBARFreeShared(ctx, &storageOutputSharedDataHandles);
  vfeReBARFreeShared(ctx, &storageInputSharedDataHandles);
  uint64_t ids[] = {
    cs,
    pp,
    batch,
  };
  vfIdDestroy(countof(ids), ids, FF, LL);
  vfContextDeinit(ctx, FF, LL);

  #if defined(VKFAST_INCLUDE_TERMUX_PATHS)
  system("pause");
  #endif
}

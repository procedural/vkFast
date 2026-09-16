//\\rc rawbuild begin gcc-linux-64-bit
//\\rc rawbuild require-config debug,release,release-fast
//\\rc rawbuild `gcc`
//\\rc rawbuild debug ` -g -O0`
//\\rc rawbuild release,release-fast ` -O2`
//\\rc rawbuild ` main.c ../../vkfast.c /home/linuxbrew/RedGpuSDK/redgpu.c /home/linuxbrew/RedGpuSDK/redgpu_2.c /home/linuxbrew/RedGpuSDK/redgpu_32.c -I/home/linuxbrew/.linuxbrew/include/ -I/home/linuxbrew/.linuxbrew/Cellar/xorgproto/2025.1/include/ -I/var/home/linuxbrew/.linuxbrew/Cellar/libxcb/1.17.0/include/ /home/linuxbrew/.linuxbrew/lib/libX11.so /home/linuxbrew/.linuxbrew/lib/libvulkan.so -lm`
//\\rc rawbuild end

#include "../../vkfast.h"
#include "../../extra/vkFast Extensions/ReBAR/vkfast_ext_rebar.h"
#include "../Common/vkfast_examples_common.h"

typedef ReiiVec4 float4;

#include "shared_data.h"

int main() {
  gpu_handle_context_t ctx = vfContextInitNoDefaultAllocs(1, NULL, FF, LL);
  REDGPU_2_EXPECTFL(vfeReBARGetMaxMallocSharedSize(ctx) >= (4ULL * 1024*1024*1024 - 1));

  const unsigned array65536[2] = {65536, 65536};

  gpu_thread_t gpu_thread = NULL;
  vfGpuThreadCreate(ctx, 1, &gpu_thread, NULL, FF, LL);

  VfeReBARMallocShared sharedDataHandles = {};
  volatile struct SharedData * sharedData = (volatile struct SharedData *)vfeReBARMallocShared(ctx, sizeof(struct SharedData), &sharedDataHandles);
  
  sharedData[0].input[0].x = 4;
  sharedData[0].input[0].y = 8;
  sharedData[0].input[0].z = 15;
  sharedData[0].input[0].w = 16;
  
  sharedData[0].input[1].x = 16;
  sharedData[0].input[1].y = 23;
  sharedData[0].input[1].z = 42;
  sharedData[0].input[1].w = 108;

  #include "add.cs.h"
  gpu_program_info_t cs_info = {0};
  cs_info.program_binary_bytes_count = sizeof(g_main);
  cs_info.program_binary             = g_main;
  uint64_t cs = vfProgramCreateFromBinaryCompute(ctx, &cs_info, FF, LL);

  RedStructDeclarationMember slots[1] = {0};
  slots[0].slot            = 0;
  slots[0].type            = RED_STRUCT_MEMBER_TYPE_ARRAY_RO_RW;
  slots[0].count           = 1;
  slots[0].visibleToStages = RED_VISIBLE_TO_STAGE_BITFLAG_COMPUTE;
  gpu_program_pipeline_info_t pp_info = {0};
  pp_info.variables_slot        = 1;
  pp_info.variables_bytes_count = 1 * 4*sizeof(float);
  pp_info.struct_members_count  = countof(slots);
  pp_info.struct_members        = slots;
  uint64_t pp = vfProgramPipelineCreateCompute(ctx, cs, &pp_info, NULL, FF, LL);
  
  uint64_t batch = 0;

  {
    gpu_batch_info_t bindings_info = {0};
    bindings_info.max_new_bindings_sets_count = 1;
    bindings_info.max_storage_binds_count     = 1;
    batch = vfBatchBegin(ctx, batch, &bindings_info, NULL, FF, LL);
    vfBatchBindProgramPipelineCompute(ctx, batch, pp, FF, LL);
    vfBatchBindNewBindingsSet(ctx, batch, countof(slots), slots, FF, LL);
    vfBatchBindStorageRaw(ctx, batch, 0, 1, &sharedDataHandles.storageRaw, FF, LL);
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
      sharedData[0].output.x,
      sharedData[0].output.y,
      sharedData[0].output.z,
      sharedData[0].output.w
    );
    REDGPU_2_EXPECTFL(sharedData[0].output.x == 20);
    REDGPU_2_EXPECTFL(sharedData[0].output.y == 30);
    REDGPU_2_EXPECTFL(sharedData[0].output.z == 50);
    REDGPU_2_EXPECTFL(sharedData[0].output.w == 130);
  }
  
  vfAllQueuesWaitIdle(ctx, FF, LL);

  vfGpuThreadDestroy(ctx, gpu_thread);
  uint64_t ids[] = {
    cs,
    pp,
    batch,
  };
  vfIdDestroy(countof(ids), ids, FF, LL);

  vfeReBARFreeShared(ctx, &sharedDataHandles);
  sharedData = NULL;

  vfContextDeinit(ctx, FF, LL);
}

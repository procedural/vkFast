#include "../../vkfast.h"

#include <stdio.h> // For printf

#include <shellscalingapi.h>   // For SetProcessDpiAwareness
#pragma comment(lib, "shcore") // For SetProcessDpiAwareness

#define countof(x) (sizeof(x) / sizeof((x)[0]))

#define FF __FILE__
#define LL __LINE__

int main() {
  SetProcessDpiAwareness(PROCESS_PER_MONITOR_DPI_AWARE);

  #define window_w 1920
  #define window_h 1080

  gpu_handle_context_t ctx = vfContextInit(1, NULL, FF, LL);
  vfWindowFullscreen(ctx, NULL, "[vkFast] Hello Compute", window_w, window_h, FF, LL);

  gpu_storage_info_t storage_info = {0};
  storage_info.storage_type = GPU_STORAGE_TYPE_CPU_UPLOAD;
  storage_info.bytes_count  = 2 * 4*sizeof(float);
  gpu_storage_t storage_input_cpu = {0};
  vfStorageCreate(ctx, &storage_info, &storage_input_cpu, FF, LL);
  storage_info.storage_type = GPU_STORAGE_TYPE_GPU_ONLY;
  gpu_storage_t storage_input_gpu = {0};
  vfStorageCreate(ctx, &storage_info, &storage_input_gpu, FF, LL);
  
  storage_input_cpu.as_vec4[0].x = 4;
  storage_input_cpu.as_vec4[0].y = 8;
  storage_input_cpu.as_vec4[0].z = 15;
  storage_input_cpu.as_vec4[0].w = 16;
  
  storage_input_cpu.as_vec4[1].x = 16;
  storage_input_cpu.as_vec4[1].y = 23;
  storage_input_cpu.as_vec4[1].z = 42;
  storage_input_cpu.as_vec4[1].w = 108;
    
  uint64_t copy = vfBatchBegin(ctx, 0, NULL, NULL, FF, LL);
  vfBatchStorageCopyFromCpuToGpu(ctx, copy, storage_input_cpu.id, storage_input_gpu.id, FF, LL);
  vfBatchEnd(ctx, copy, FF, LL);
  uint64_t async = vfAsyncBatchExecute(ctx, 1, &copy, FF, LL);
  vfAsyncWaitToFinish(ctx, async, FF, LL);

  gpu_storage_info_t storage_output_info = {0};
  storage_output_info.storage_type = GPU_STORAGE_TYPE_CPU_READBACK;
  storage_output_info.bytes_count  = 1 * 4*sizeof(float);
  gpu_storage_t storage_output_cpu = {0};
  vfStorageCreate(ctx, &storage_output_info, &storage_output_cpu, FF, LL);
  storage_output_info.storage_type = GPU_STORAGE_TYPE_GPU_ONLY;
  gpu_storage_t storage_output_gpu = {0};
  vfStorageCreate(ctx, &storage_output_info, &storage_output_gpu, FF, LL);

  #include "add.cs.h"
  gpu_program_info_t cs_info = {0};
  cs_info.program_binary_bytes_count = sizeof(g_main);
  cs_info.program_binary             = g_main;
  uint64_t cs = vfProgramCreateFromBinaryCompute(ctx, &cs_info, FF, LL);

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
  uint64_t pp = vfProgramPipelineCreateCompute(ctx, &pp_info, FF, LL);
  
  uint64_t batch = 0;

  struct Pixels {
    unsigned char pixels[window_h][window_w][4];
  };
  // To free
  struct Pixels * pix = (struct Pixels *)red32MemoryCalloc(sizeof(struct Pixels));
  REDGPU_2_EXPECTFL(pix != NULL);

  while (vfWindowLoop(ctx)) {
    gpu_batch_info_t bindings_info = {0};
    bindings_info.max_new_bindings_sets_count = 1;
    bindings_info.max_storage_binds_count     = 2;
    batch = vfBatchBegin(ctx, batch, &bindings_info, NULL, FF, LL);
    vfBatchBindProgramPipelineCompute(ctx, batch, pp, FF, LL);
    vfBatchBindNewBindingsSet(ctx, batch, countof(slots), slots, FF, LL);
    vfBatchBindStorage(ctx, batch, 0, 1, &storage_input_gpu.id, FF, LL);
    vfBatchBindStorage(ctx, batch, 1, 1, &storage_output_gpu.id, FF, LL);
    vfBatchBindNewBindingsEnd(ctx, batch, FF, LL);
    float salt[4] = {0};
    salt[0] = 0;
    salt[1] = -1;
    salt[2] = -7;
    salt[3] = 6;
    vfBatchBindVariablesCopy(ctx, batch, sizeof(salt), salt, FF, LL);
    vfBatchCompute(ctx, batch, 1, 1, 1, FF, LL);
    vfBatchBarrierMemory(ctx, batch, FF, LL);
    vfBatchStorageCopyFromGpuToCpu(ctx, batch, storage_output_gpu.id, storage_output_cpu.id, FF, LL);
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

    unsigned char * pixels = &pix->pixels[0][0][0];
    // Clear pixels:
    for (int y = 0; y < window_h; y += 1) {
      for (int x = 0; x < window_w; x += 1) {
        pixels[y * window_w * 4 + x * 4 + 0] = 0;
        pixels[y * window_w * 4 + x * 4 + 1] = 0;
        pixels[y * window_w * 4 + x * 4 + 2] = 0;
        pixels[y * window_w * 4 + x * 4 + 3] = 0;
      }
    }
    // Draw pixels:
    for (int y = 0; y < window_h; y += 1) {
      for (int x = 0; x < window_w; x += 1) {
        // NOTE(Constantine):
        // The automatic DPI scaling is disabled with SetProcessDpiAwareness(PROCESS_PER_MONITOR_DPI_AWARE), but
        // with DPI scaling, 1920x1080 screen resolution with a desktop scaling of, say, 1.25 is equal to 1536x864 (draw coords: 1535x863).
        if (y == 0) {
          pixels[y * window_w * 4 + x * 4 + 0] = 255;
          pixels[y * window_w * 4 + x * 4 + 1] = 0;
          pixels[y * window_w * 4 + x * 4 + 2] = 0;
          pixels[y * window_w * 4 + x * 4 + 3] = 255;
        } else if (x == 0) {
          pixels[y * window_w * 4 + x * 4 + 0] = 0;
          pixels[y * window_w * 4 + x * 4 + 1] = 255;
          pixels[y * window_w * 4 + x * 4 + 2] = 0;
          pixels[y * window_w * 4 + x * 4 + 3] = 255;
        } else if (y == window_h-1) {
          pixels[y * window_w * 4 + x * 4 + 0] = 0;
          pixels[y * window_w * 4 + x * 4 + 1] = 0;
          pixels[y * window_w * 4 + x * 4 + 2] = 255;
          pixels[y * window_w * 4 + x * 4 + 3] = 255;
        } else if (x == window_w-1) {
          pixels[y * window_w * 4 + x * 4 + 0] = 255;
          pixels[y * window_w * 4 + x * 4 + 1] = 255;
          pixels[y * window_w * 4 + x * 4 + 2] = 255;
          pixels[y * window_w * 4 + x * 4 + 3] = 255;
        }
      }
    }
    vfDrawPixels(ctx, pix->pixels, FF, LL);
  }
  
  red32MemoryFree(pix);
  pix = NULL;

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
  vfIdDestroy(ctx, countof(ids), ids, FF, LL);
  vfContextDeinit(ctx, FF, LL);
  vfExit(0);
}

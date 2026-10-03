#pragma once

#include "../../../vkfast.h"
#include "../../../vkfast_ex.h"
#include "../../../vkfast_ids.h"

#include "../../../extra/Dear ImGui 2016/imgui_reii.h"
#include "../../../extra/Banzai/vkfast_extra_banzai_pointer.h"

#ifndef IDC_IMGUI_API_NON_STATIC
#define IDC_IMGUI_API_PRE  static
#define IDC_IMGUI_API_POST
#define IDC_IMGUI_IMPLEMENTATION
#endif

IDC_IMGUI_API_PRE void * IDC_IMGUI_API_POST idcImguiInit   (gpu_handle_context_t context, GLFWwindow * glfw_window, gpu_thread_t * gpu_thread, ReiiHandleTexture * output_texture);
IDC_IMGUI_API_PRE void   IDC_IMGUI_API_POST idcImguiDraw   (void * idcimgui, int draw);
IDC_IMGUI_API_PRE void   IDC_IMGUI_API_POST idcImguiDeinit (void * idcimgui);

#ifdef IDC_IMGUI_IMPLEMENTATION

typedef struct IdcImguiState {
  gpu_handle_context_t    context;
  ImguiIO *               io;
  ImguiStyle *            style;
  gpu_storage_t           storage_gpu_only;
  gpu_storage_t           storage_cpu_upload;
  gpu_storage_t           storage_cpu_readback;
  ReiiHandleTextureMemory imgui_fontAtlasMemory;
  ReiiCpuScratchBuffer    imgui_fontAtlas_upload_scratch_buffer;
  gpu_extra_cpu_gpu_array imgui_dynamicMeshPosition;
  gpu_extra_cpu_gpu_array imgui_dynamicMeshColor;
  Red2Output              imgui_mutableOutputsArray[100]; // NOTE(Constantine): Increase for more if needed.
} IdcImguiState;

IDC_IMGUI_API_PRE void * IDC_IMGUI_API_POST idcImguiInit(gpu_handle_context_t context, GLFWwindow * glfw_window, gpu_thread_t * gpu_thread, ReiiHandleTexture * output_texture) {
  IdcImguiState * idcimgui = (IdcImguiState *)calloc(1, sizeof(IdcImguiState));
  REDGPU_2_EXPECTFL(idcimgui != NULL);

  gpu_storage_t storage_gpu_only     = {0};
  gpu_storage_t storage_cpu_upload   = {0};
  gpu_storage_t storage_cpu_readback = {0};
  vfeBanzaiStoragesCreate(context, &storage_gpu_only, &storage_cpu_upload, &storage_cpu_readback, FF, LL);

  uint64_t storage_gpu_only_mem_offset = 0;
  uint64_t storage_cpu_upload_mem_offset = 0;
  uint64_t storage_cpu_readback_mem_offset = 0;

  ReiiHandleTextureMemory imgui_fontAtlasMemory = {0};
  reiiCreateTextureMemory(context, GPU_EXTRA_REII_TEXTURE_TYPE_GENERAL, (64/*mb*/ * 1024 * 1024), &imgui_fontAtlasMemory);

  ReiiCpuScratchBuffer imgui_fontAtlas_upload_scratch_buffer = OffsetAllocateCpuScratchBuffer(
    128/*mb*/ * 1024 * 1024, // NOTE(Constantine): Increase for more if needed.
    &storage_cpu_upload, &storage_cpu_upload_mem_offset,
    FF, LL
  );

  gpu_extra_cpu_gpu_array imgui_dynamicMeshPosition = OffsetAllocateCpuGpuArrayWithTale64BytesAlign(
    32/*mb*/ * 1024 * 1024, // NOTE(Constantine): Increase for more if needed.
    &storage_cpu_upload, &storage_cpu_upload_mem_offset,
    &storage_gpu_only,   &storage_gpu_only_mem_offset,
    FF, LL
  );

  gpu_extra_cpu_gpu_array imgui_dynamicMeshColor = OffsetAllocateCpuGpuArrayWithTale64BytesAlign(
    32/*mb*/ * 1024 * 1024, // NOTE(Constantine): Increase for more if needed.
    &storage_cpu_upload, &storage_cpu_upload_mem_offset,
    &storage_gpu_only,   &storage_gpu_only_mem_offset,
    FF, LL
  );

  const int imgui_maxNewBindingsSetsCount  = 100; // NOTE(Constantine): Increase for more if needed.
  const int imgui_mutableOutputsArrayCount = 100; // NOTE(Constantine): Increase for more if needed.

  imguiInit(
    glfw_window, // GLFWwindow * window
    context, // gpu_handle_context_t context
    imgui_fontAtlasMemory, // ReiiHandleTextureMemory fontAtlasMemory
    imgui_fontAtlas_upload_scratch_buffer, // ReiiCpuScratchBuffer fontAtlasScratchBuffer
    imgui_maxNewBindingsSetsCount, // uint64_t maxNewBindingsSetsCount
    imgui_dynamicMeshPosition, // gpu_extra_cpu_gpu_array dynamicMeshPosition
    imgui_dynamicMeshColor, // gpu_extra_cpu_gp&gpu_threadu_array dynamicMeshColor
    imgui_mutableOutputsArrayCount, // uint64_t mutableOutputsArrayMaxCapacity
    idcimgui->imgui_mutableOutputsArray, // Red2Output * mutableOutputsArray
    output_texture, // ReiiHandleTexture * outputTexture
    gpu_thread, // gpu_thread_t * gpuThread
    0, // unsigned optionalQueueFamilyIndex
    NULL // RedHandleQueue optionalQueue
  );

  idcimgui->context = context;
  idcimgui->io      = (ImguiIO *)igGetIO();
  idcimgui->style   = (ImguiStyle *)igGetStyle();

  idcimgui->storage_gpu_only     = storage_gpu_only;
  idcimgui->storage_cpu_upload   = storage_cpu_upload;
  idcimgui->storage_cpu_readback = storage_cpu_readback;

  idcimgui->imgui_fontAtlasMemory                 = imgui_fontAtlasMemory;
  idcimgui->imgui_fontAtlas_upload_scratch_buffer = imgui_fontAtlas_upload_scratch_buffer;
  idcimgui->imgui_dynamicMeshPosition             = imgui_dynamicMeshPosition;
  idcimgui->imgui_dynamicMeshColor                = imgui_dynamicMeshColor;

  idcimgui->style->scrollbarRounding = 0;
  idcimgui->style->windowRounding    = 0;
  idcimgui->style->frameRounding     = 0;

  imguiSetProcessInputsState(1);
  imguiNewFrame();

  return idcimgui;
}

IDC_IMGUI_API_PRE void IDC_IMGUI_API_POST idcImguiDraw(void * idcimgui, int draw) {
  imguiSetProcessInputsState(draw);
  if (draw) {
    igRender();
    imguiNewFrame();
  }
}

IDC_IMGUI_API_PRE void IDC_IMGUI_API_POST idcImguiDeinit(void * idcimgui) {
  IdcImguiState * icimgui = (IdcImguiState *)idcimgui;

  imguiDeinit();
  reiiDestroyEx(icimgui->context, GPU_EXTRA_REII_DESTROY_TYPE_TEXTURE_MEMORY, &icimgui->imgui_fontAtlasMemory);
  uint64_t ids[] = {
    icimgui->storage_gpu_only.id,
    icimgui->storage_cpu_upload.id,
    icimgui->storage_cpu_readback.id,
  };
  vfIdDestroy(countof(ids), ids, FF, LL);

  free(icimgui);
}

#endif // IDC_IMGUI_IMPLEMENTATION

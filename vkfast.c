#include "vkfast.h"

#ifndef __cplusplus
#define REDGPU_DISABLE_NAMED_PARAMETERS
#endif
#include "C:/RedGpuSDK/misc/np/np.h"
#include "C:/RedGpuSDK/misc/np/np_redgpu.h"
#include "C:/RedGpuSDK/misc/np/np_redgpu_wsi.h"
#include "C:/RedGpuSDK/misc/np/np_redgpu_2.h"

const int TODO = 1;

typedef struct vkfast_global_state {
  int                isDebugMode;

  int                screenWidth;
  int                screenHeight;

  void *             windowHandle;
  int                windowMsaaSamples;
  
  RedContext         context;
  int                gpuIndex;
  const RedGpuInfo * gpuInfo;

  RedHandleGpu       gpu;
  unsigned           mainQueueFamilyIndex;
  RedHandleQueue     mainQueue;

  unsigned           specificMemoryTypesCpuVisibleCount;
  unsigned           specificMemoryTypesCpuVisible[32];
  unsigned           specificMemoryTypesReadbackCount;
  unsigned           specificMemoryTypesReadback[32];
} vkfast_global_state;

vkfast_global_state g_vkfast;

void red2Crash(const char * error, const char * functionName, RedHandleGpu optionalGpuHandle, const char * optionalFile, int optionalLine) {
  struct StringArray {
    char * items;
    size_t count;
    size_t capacity;
    size_t alignment;
  };

  struct StringArray str = {0};

  char * optionalLineStr = (char *)red32MemoryCalloc(4096);
  red32IntToChars(optionalLine, optionalLineStr);

  REDGPU_32_DYNAMIC_ARRAY_STRING_JOIN(str, "[REDGPU 2][Crash][");
  REDGPU_32_DYNAMIC_ARRAY_STRING_JOIN(str, optionalFile);
  REDGPU_32_DYNAMIC_ARRAY_STRING_JOIN(str, ":");
  REDGPU_32_DYNAMIC_ARRAY_STRING_JOIN(str, optionalLineStr);
  REDGPU_32_DYNAMIC_ARRAY_STRING_JOIN(str, "][");
  REDGPU_32_DYNAMIC_ARRAY_STRING_JOIN(str, functionName);
  REDGPU_32_DYNAMIC_ARRAY_STRING_JOIN(str, "] ");
  REDGPU_32_DYNAMIC_ARRAY_STRING_JOIN(str, error);
  REDGPU_32_DYNAMIC_ARRAY_STRING_JOIN(str, "\n");

  red32ConsolePrintError(str.items);

  red32MemoryFree(optionalLineStr);
  REDGPU_32_DYNAMIC_ARRAY_FREE(str);

  red32Exit(1);
}

void vfWindowFullscreen(void * window_handle, int enable_debug_mode, int screen_width, int screen_height, const char * window_title, int msaa_samples, const char * optionalFile, int optionalLine) {
  if (enable_debug_mode) {
    red32OutputDebugString ("[vkFast] In case of an error, email me (Constantine) at: iamvfx@gmail.com" "\n");
    red32ConsolePrint      ("[vkFast] In case of an error, email me (Constantine) at: iamvfx@gmail.com" "\n");
  }

  REDGPU_2_EXPECT(TODO && msaa_samples == 1);

  vkfast_global_state defaults = {0};
  g_vkfast = defaults;

  g_vkfast.isDebugMode = enable_debug_mode;
  g_vkfast.screenWidth = screen_width;
  g_vkfast.screenHeight = screen_height;
  g_vkfast.windowHandle = window_handle;
  g_vkfast.windowMsaaSamples = msaa_samples;

  if (g_vkfast.windowHandle == NULL) {
    g_vkfast.windowHandle = red32WindowCreate(window_title);
  }

  np(redCreateContext,
    "malloc", red32MemoryCalloc,
    "free", red32MemoryFree,
    "optionalMallocTagged", NULL,
    "optionalFreeTagged", NULL,
    "debugCallback", NULL,
    "sdkVersion", RED_SDK_VERSION_1_0_135,
    "sdkExtensionsCount", 0,
    "sdkExtensions", NULL,
    "optionalProgramName", NULL,
    "optionalProgramVersion", 0,
    "optionalEngineName", NULL,
    "optionalEngineVersion", 0,
    "optionalSettings", NULL,
    "outContext", &g_vkfast.context,
    "outStatuses", NULL,
    "optionalFile", __FILE__,
    "optionalLine", __LINE__,
    "optionalUserData", NULL
  );

  REDGPU_2_EXPECT(g_vkfast.context != NULL);
  REDGPU_2_EXPECT(g_vkfast.context->gpusCount > 0);

  g_vkfast.gpuInfo = &g_vkfast.context->gpus[g_vkfast.gpuIndex];

  REDGPU_2_EXPECT(g_vkfast.gpuInfo->queuesCount > 0);

  if (g_vkfast.isDebugMode) {
    red32OutputDebugString("[vkFast] Your GPU name: ");
    red32OutputDebugString(g_vkfast.gpuInfo->gpuName);
    red32OutputDebugString("\n");

    red32ConsolePrint("[vkFast] Your GPU name: ");
    red32ConsolePrint(g_vkfast.gpuInfo->gpuName);
    red32ConsolePrint("\n");
  }

  if (g_vkfast.gpuInfo->gpuVendorId == 4318/*NVIDIA*/) {
    np(red2ExpectMinimumGuarantees,
      "gpuInfo", g_vkfast.gpuInfo,
      "optionalFile", __FILE__,
      "optionalLine", __LINE__
    );
  } else if (g_vkfast.gpuInfo->gpuVendorId == 32902/*Intel UHD Graphics 730*/) {
    np(red2ExpectMinimumGuaranteesIntelUHDGraphics730,
      "gpuInfo", g_vkfast.gpuInfo,
      "optionalFile", __FILE__,
      "optionalLine", __LINE__
    );
  } else {
    REDGPU_2_EXPECT(!"Unsupported by vkFast GPU, recompile your program with vfWindow1920x1080()::enable_debug_mode parameter enabled and email me your GPU name please: iamvfx@gmail.com");
  }

  np(red2ExpectAllMemoryToBeCoherent,
    "gpuInfo", g_vkfast.gpuInfo,
    "optionalFile", __FILE__,
    "optionalLine", __LINE__
  );
  
  np(red2ExpectMinimumImageFormatsLimitsAndFeatures, 
    "gpuInfo", g_vkfast.gpuInfo,
    "optionalFile", __FILE__,
    "optionalLine", __LINE__
  );

  g_vkfast.gpu                  = g_vkfast.gpuInfo->gpu;
  g_vkfast.mainQueueFamilyIndex = g_vkfast.gpuInfo->queuesFamilyIndex[0];
  g_vkfast.mainQueue            = g_vkfast.gpuInfo->queues[0];

  unsigned specificMemoryTypeCPUVisible = -1;
  unsigned specificMemoryTypeReadback   = -1;
  if (g_vkfast.gpuInfo->gpuVendorId == 4318/*NVIDIA*/) { // Tested on RTX 2060 and Windows 10.
    unsigned      memoryTypesCount = 0;
    RedMemoryType memoryTypes[32]  = {0};
    unsigned      memoryHeapsCount = 0;
    RedMemoryHeap memoryHeaps[32]  = {0};

    memoryTypesCount = 6;
    memoryHeapsCount = 3;

    memoryTypes[0].memoryHeapIndex = 1;
    memoryTypes[0].isGpuVram       = 0;
    memoryTypes[0].isCpuMappable   = 0;
    memoryTypes[0].isCpuCoherent   = 0;
    memoryTypes[0].isCpuCached     = 0;

    memoryTypes[1].memoryHeapIndex = 0;
    memoryTypes[1].isGpuVram       = 1;
    memoryTypes[1].isCpuMappable   = 0;
    memoryTypes[1].isCpuCoherent   = 0;
    memoryTypes[1].isCpuCached     = 0;

    memoryTypes[2].memoryHeapIndex = 0;
    memoryTypes[2].isGpuVram       = 1;
    memoryTypes[2].isCpuMappable   = 0;
    memoryTypes[2].isCpuCoherent   = 0;
    memoryTypes[2].isCpuCached     = 0;

    memoryTypes[3].memoryHeapIndex = 1;
    memoryTypes[3].isGpuVram       = 0;
    memoryTypes[3].isCpuMappable   = 1;
    memoryTypes[3].isCpuCoherent   = 1;
    memoryTypes[3].isCpuCached     = 0;

    memoryTypes[4].memoryHeapIndex = 1;
    memoryTypes[4].isGpuVram       = 0;
    memoryTypes[4].isCpuMappable   = 1;
    memoryTypes[4].isCpuCoherent   = 1;
    memoryTypes[4].isCpuCached     = 1;

    memoryTypes[5].memoryHeapIndex = 2;
    memoryTypes[5].isGpuVram       = 1;
    memoryTypes[5].isCpuMappable   = 1;
    memoryTypes[5].isCpuCoherent   = 1;
    memoryTypes[5].isCpuCached     = 0;

    memoryHeaps[0].memoryBytesCount = 6000000000;
    memoryHeaps[0].isGpuVram        = 1;

    memoryHeaps[1].memoryBytesCount = 2000000000;
    memoryHeaps[1].isGpuVram        = 0;

    memoryHeaps[2].memoryBytesCount = 200000000;
    memoryHeaps[2].isGpuVram        = 1;

    np(red2ExpectMemoryTypes,
      "gpuInfo", g_vkfast.gpuInfo,
      "expectedMemoryHeapsCount", memoryHeapsCount,
      "expectedMemoryHeaps", memoryHeaps,
      "expectedMemoryTypesCount", memoryTypesCount,
      "expectedMemoryTypes", memoryTypes,
      "optionalFile", __FILE__,
      "optionalLine", __LINE__
    );

    g_vkfast.specificMemoryTypesCpuVisibleCount = 1;
    g_vkfast.specificMemoryTypesReadbackCount   = 1;

    g_vkfast.specificMemoryTypesCpuVisible[0] = 3;
    g_vkfast.specificMemoryTypesReadback[0]   = 4; // The cpu cached one
  } else if (g_vkfast.gpuInfo->gpuVendorId == 32902/*Intel UHD Graphics 730*/) {
    unsigned      memoryTypesCount = 0;
    RedMemoryType memoryTypes[32]  = {0};
    unsigned      memoryHeapsCount = 0;
    RedMemoryHeap memoryHeaps[32]  = {0};

    memoryTypesCount = 3;
    memoryHeapsCount = 1;

    memoryTypes[0].memoryHeapIndex = 0;
    memoryTypes[0].isGpuVram       = 1;
    memoryTypes[0].isCpuMappable   = 0;
    memoryTypes[0].isCpuCoherent   = 0;
    memoryTypes[0].isCpuCached     = 0;

    memoryTypes[1].memoryHeapIndex = 0;
    memoryTypes[1].isGpuVram       = 1;
    memoryTypes[1].isCpuMappable   = 1;
    memoryTypes[1].isCpuCoherent   = 1;
    memoryTypes[1].isCpuCached     = 0;

    memoryTypes[2].memoryHeapIndex = 0;
    memoryTypes[2].isGpuVram       = 1;
    memoryTypes[2].isCpuMappable   = 1;
    memoryTypes[2].isCpuCoherent   = 1;
    memoryTypes[2].isCpuCached     = 1;

    memoryHeaps[0].memoryBytesCount = 2000000000;
    memoryHeaps[0].isGpuVram        = 1;

    np(red2ExpectMemoryTypes,
      "gpuInfo", g_vkfast.gpuInfo,
      "expectedMemoryHeapsCount", memoryHeapsCount,
      "expectedMemoryHeaps", memoryHeaps,
      "expectedMemoryTypesCount", memoryTypesCount,
      "expectedMemoryTypes", memoryTypes,
      "optionalFile", __FILE__,
      "optionalLine", __LINE__
    );

    g_vkfast.specificMemoryTypesCpuVisibleCount = 1;
    g_vkfast.specificMemoryTypesReadbackCount   = 1;

    g_vkfast.specificMemoryTypesCpuVisible[0] = 1;
    g_vkfast.specificMemoryTypesReadback[0]   = 2; // The cpu cached one
  } else {
    REDGPU_2_EXPECT(!"Unsupported by vkFast GPU, recompile your program with vfWindow1920x1080()::enable_debug_mode parameter enabled and email me your GPU name please: iamvfx@gmail.com");
  }
}

int vfWindowLoop() {
  return red32WindowLoop();
}

void vfExit(int exit_code) {
  red32WindowDestroy(g_vkfast.windowHandle);
  red32Exit(exit_code);
}

gpu_storage_t vfStorageCreateFromStruct(gpu_storage_info_t storage, const char * optionalFile, int optionalLine) {
  gpu_storage_t out = {0};

  return out;
}

gpu_storage_t vfStorageCreateFromStructCpuReadback(gpu_storage_info_t storage, const char * optional_file, int optional_line) {
  gpu_storage_t out = {0};

  return out;
}

uint64_t vfStorageCreateFromStructGpuOnly(gpu_storage_info_t storage, const char * optional_file, int optional_line) {
  uint64_t out = -1;

  return out;
}

uint64_t vfTextureCreateFromStruct(gpu_texture_info_t texture, const char * optionalFile, int optionalLine) {
  uint64_t out = -1;

  return out;
}

uint64_t vfSamplerCreateFromStruct(gpu_sampler_info_t sampler, const char * optionalFile, int optionalLine) {
  uint64_t out = -1;

  return out;
}

uint64_t vfTextureCreateFromBmp(int width, int height, int generate_mip_levels, int texture_count, const char ** texture_paths, const char * optionalFile, int optionalLine) {
  uint64_t out = -1;

  return out;
}

uint64_t vfCubemapCreateFromBmp(int width, int height, int generate_mip_levels, int texture_count, const char ** pos_x_texture_paths, const char ** neg_x_texture_paths, const char ** pos_y_texture_paths, const char ** neg_y_texture_paths, const char ** pos_z_texture_paths, const char ** neg_z_texture_paths, const char * optionalFile, int optionalLine) {
  uint64_t out = -1;

  return out;
}

uint64_t vfProgramCreateFromFileVertProgram(const char * shader_filepath, const char * optionalFile, int optionalLine) {
  uint64_t out = -1;

  return out;
}

uint64_t vfProgramCreateFromFileFragProgram(const char * shader_filepath, const char * optionalFile, int optionalLine) {
  uint64_t out = -1;

  return out;
}

uint64_t vfProgramCreateFromStringVertProgram(const char * shader_string, const char * optionalFile, int optionalLine) {
  uint64_t out = -1;

  return out;
}

uint64_t vfProgramCreateFromStringFragProgram(const char * shader_string, const char * optionalFile, int optionalLine) {
  uint64_t out = -1;

  return out;
}

uint64_t vfProgramPipelineCreate(uint64_t vert_program, uint64_t frag_program, const char * optionalFile, int optionalLine) {
  uint64_t out = -1;

  return out;
}

uint64_t vfBatchBegin(const char * optionalFile, int optionalLine) {
  uint64_t out = -1;

  return out;
}

void vfBatchStorageCopyFromCpuToGpu(uint64_t batch_id, uint64_t storage_id, const char * optional_file, int optional_line) {
}

void vfBatchStorageCopyFromGpuToCpu(uint64_t batch_id, uint64_t storage_id, const char * optional_file, int optional_line) {
}

void vfBatchTexturePixelsCopyFromCpuToGpu(uint64_t batch_id, uint64_t texture_id, int texture_layer, int mip_level, int x, int y, int width, int height, uint64_t copy_source_storage_id, const char * optional_file, int optional_line) {
}

void vfBatchTexturePixelsCopyFromGpuToCpu(uint64_t batch_id, uint64_t texture_id, int texture_layer, int mip_level, int x, int y, int width, int height, uint64_t copy_target_storage_id, const char * optional_file, int optional_line) {
}

void vfBatchBindStorage(uint64_t batch_id, int storage_ids_count, const uint64_t * storage_ids, const char * optionalFile, int optionalLine) {
}

void vfBatchBindTexture(uint64_t batch_id, int texture_ids_count, const uint64_t * texture_ids, const char * optionalFile, int optionalLine) {
}

void vfBatchBindCubemap(uint64_t batch_id, int texture_ids_count, const uint64_t * texture_ids, const char * optionalFile, int optionalLine) {
}

void vfBatchBindSampler(uint64_t batch_id, int sampler_ids_count, const uint64_t * sampler_ids, const char * optionalFile, int optionalLine) {
}

void vfBatchBindRWTexture(uint64_t batch_id, int texture_ids_count, const uint64_t * texture_ids, const char * optional_file, int optional_line) {
}

void vfBatchBindProgramPipeline(uint64_t batch_id, uint64_t program_pipeline_id, const char * optionalFile, int optionalLine) {
}

void vfBatchClear(uint64_t batch_id, const char * optionalFile, int optionalLine) {
}

void vfBatchDraw(uint64_t batch_id, uint64_t gpu_cmd_count, const gpu_cmd_t * gpu_cmd, const char * optionalFile, int optionalLine) {
}

void vfBatchFire(uint64_t batch_id, uint64_t count, const char * optionalFile, int optionalLine) {
}

void vfBatchEnd(uint64_t batch_id, const char * optionalFile, int optionalLine) {
}

void vfBatchExecute(uint64_t batch_ids_count, const uint64_t * batch_ids, const char * optionalFile, int optionalLine) {
}

uint64_t vfAsyncBatchExecute(uint64_t batch_ids_count, const uint64_t * batch_ids, const char * optional_file, int optional_line) {
}

void vfAsyncWaitToFinish(uint64_t async_id) {
}
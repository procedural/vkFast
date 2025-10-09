#ifdef _WIN32
#define GPU_API_PRE __declspec(dllexport)
#define GPU_API_POST
#endif

#include "vkfast.h"
#include "vkfast_ids.h"

#ifndef __cplusplus
#define REDGPU_DISABLE_NAMED_PARAMETERS
#endif
#include "C:/RedGpuSDK/misc/np/np.h"
#include "C:/RedGpuSDK/misc/np/np_redgpu.h"
#include "C:/RedGpuSDK/misc/np/np_redgpu_2.h"
#include "C:/RedGpuSDK/misc/np/np_redgpu_wsi.h"

#ifdef _WIN32
#include <Windows.h>
#endif

static void vfInternalPrint(const char * string) {
  red32OutputDebugString(string);
  red32ConsolePrint(string);
}

void red2Crash(const char * error, const char * functionName, RedHandleGpu optionalGpuHandle, const char * optionalFile, int optionalLine) {
  struct StringArray {
    char * items;
    size_t count;
    size_t capacity;
    size_t alignment;
  };

  // To free
  struct StringArray str = {0};

  // To free
  char * optionalLineStr = (char *)red32MemoryCalloc(4096);
  red32IntToChars(optionalLine, optionalLineStr);

  REDGPU_32_DYNAMIC_ARRAY_STRING_JOIN(str, "[vkFast][Crash][");
  REDGPU_32_DYNAMIC_ARRAY_STRING_JOIN(str, optionalFile);
  REDGPU_32_DYNAMIC_ARRAY_STRING_JOIN(str, ":");
  REDGPU_32_DYNAMIC_ARRAY_STRING_JOIN(str, optionalLineStr);
  REDGPU_32_DYNAMIC_ARRAY_STRING_JOIN(str, "][");
  REDGPU_32_DYNAMIC_ARRAY_STRING_JOIN(str, functionName);
  REDGPU_32_DYNAMIC_ARRAY_STRING_JOIN(str, "] ");
  REDGPU_32_DYNAMIC_ARRAY_STRING_JOIN(str, error);
  REDGPU_32_DYNAMIC_ARRAY_STRING_JOIN(str, "\n");

  vfInternalPrint(str.items);
  MessageBoxA(NULL, str.items, "[vkFast][Crash]", MB_OK);

  red32MemoryFree(optionalLineStr);
  REDGPU_32_DYNAMIC_ARRAY_FREE(str);

  red32Exit(1);
}

static RedBool32 vfRedGpuDebugCallback(RedDebugCallbackSeverity severity, RedDebugCallbackTypeBitflags types, const RedDebugCallbackData * data, RedContext context) {
  if (0 == strcmp(data->messageIdName, "VUID-VkDebugUtilsMessengerCallbackDataEXT-flags-zerobitmask")) {
    return 0;
  }
  if (0 == strcmp(data->messageIdName, "RED_PROCEDURE_ID_redQueuePresent") &&
      0 == strcmp(data->message,       "RED_STATUS_ERROR_PRESENT_IS_OUT_OF_DATE"))
  {
    return 0;
  }
  if (0 == strcmp(data->messageIdName, "RED_PROCEDURE_ID_redPresentGetImageIndex") &&
      0 == strcmp(data->message,       "RED_STATUS_ERROR_PRESENT_IS_OUT_OF_DATE"))
  {
    return 0;
  }
  vfInternalPrint("[vkFast][Debug callback] ");
  vfInternalPrint(data->message);
  MessageBoxA(NULL, data->message, "[vkFast][Debug callback]", MB_OK);
  red32Exit(1);
  return 0;
}

GPU_API_PRE gpu_handle_context_t GPU_API_POST vfContextInit(int enable_debug_mode, const gpu_context_optional_parameters_t * optional_parameters, const char * optionalFile, int optionalLine) {
  if (enable_debug_mode) {
    vfInternalPrint("[vkFast][Debug] In case of an error, email me (Constantine) at: iamvfx@gmail.com" "\n");
  }

  void * optional_pointer_to_custom_vf_handle_context = 0;

  if (optional_parameters != NULL) {
    optional_pointer_to_custom_vf_handle_context = optional_parameters->optional_pointer_to_custom_vf_handle_context;
  }

  vf_handle_context_t * vkfast = (vf_handle_context_t *)optional_pointer_to_custom_vf_handle_context;
  if (vkfast == NULL) {
    // To free
    vkfast = (vf_handle_context_t *)red32MemoryCalloc(sizeof(vf_handle_context_t));
    REDGPU_2_EXPECT(vkfast != NULL);
  }

  uint64_t internalMemoryAllocationSizeGpuVramArrays = VKFAST_DEFAULT_MEMORY_ALLOCATION_SIZE_GPU_ONLY_512MB;
  uint64_t internalMemoryAllocationSizeCpuVisible    = VKFAST_DEFAULT_MEMORY_ALLOCATION_SIZE_CPU_UPLOAD_512MB;
  uint64_t internalMemoryAllocationSizeCpuReadback   = VKFAST_DEFAULT_MEMORY_ALLOCATION_SIZE_CPU_READBACK_512MB;
  if (optional_parameters != NULL) {
    if (optional_parameters->internal_memory_allocation_sizes != NULL) {
      internalMemoryAllocationSizeGpuVramArrays = optional_parameters->internal_memory_allocation_sizes->bytes_count_for_memory_storages_type_gpu_only;
      internalMemoryAllocationSizeCpuVisible    = optional_parameters->internal_memory_allocation_sizes->bytes_count_for_memory_storages_type_cpu_upload;
      internalMemoryAllocationSizeCpuReadback   = optional_parameters->internal_memory_allocation_sizes->bytes_count_for_memory_storages_type_cpu_readback;
    }
  }

  RedContext context = vkfast->context;
  if (context == NULL) {
    #ifdef _WIN32
    unsigned extensions[] = {
      RED_SDK_EXTENSION_WSI_WIN32
    };
    #endif
    np(redCreateContext,
      "malloc", red32MemoryCalloc,
      "free", red32MemoryFree,
      "optionalMallocTagged", NULL,
      "optionalFreeTagged", NULL,
      "debugCallback", enable_debug_mode == 1 ? vfRedGpuDebugCallback : NULL,
      "sdkVersion", RED_SDK_VERSION_1_0_135,
      "sdkExtensionsCount", sizeof(extensions) / sizeof(extensions[0]),
      "sdkExtensions", extensions,
      "optionalProgramName", NULL,
      "optionalProgramVersion", 0,
      "optionalEngineName", NULL,
      "optionalEngineVersion", 0,
      "optionalSettings", NULL,
      "outContext", &context,
      "outStatuses", NULL,
      "optionalFile", optionalFile,
      "optionalLine", optionalLine,
      "optionalUserData", NULL
    );
  }

  REDGPU_2_EXPECT(context != NULL);
  REDGPU_2_EXPECT(context->gpusCount > 0);

  const RedGpuInfo * gpuInfo = &context->gpus[vkfast->gpuIndex]; // NOTE(Constantine): Picking the first available GPU by default.

  if (enable_debug_mode == 1) {
    vfInternalPrint("[vkFast][Debug] Your GPU name: ");
    vfInternalPrint(gpuInfo->gpuName);
    vfInternalPrint("\n");
    vfInternalPrint("[vkFast][Debug] For extra debug information, recompile redgpu.c as redgpu.cpp with REDGPU_COMPILE_SWITCH 3" "\n");
  }

  REDGPU_2_EXPECT(gpuInfo->queuesCount > 0);

  if (enable_debug_mode == 1) {
    if (gpuInfo->supportsMemoryGetBudget == 1) {
      RedMemoryBudget memoryBudget = {0};
      memoryBudget.setTo1000237000 = 1000237000;
      memoryBudget.setTo0          = 0;
      np(redMemoryGetBudget,
        "context", context,
        "gpu", gpuInfo->gpu,
        "outMemoryBudget", &memoryBudget,
        "outStatuses", NULL,
        "optionalFile", optionalFile,
        "optionalLine", optionalLine,
        "optionalUserData", NULL
      );

      // To free
      char * numberString = (char *)red32MemoryCalloc(4096);

      vfInternalPrint("[vkFast][Debug] gpuInfo->memoryHeaps current process heap budget:" "\n");
      for (unsigned i = 0; i < gpuInfo->memoryHeapsCount; i += 1) {
        vfInternalPrint("[vkFast][Debug]   [");
        red32Uint64ToChars(i, numberString);
        vfInternalPrint(numberString);
        vfInternalPrint("]: ");
        red32Uint64ToChars(memoryBudget.memoryHeapsBudget[i], numberString);
        vfInternalPrint(numberString);
        vfInternalPrint("\n");
      }

      vfInternalPrint("[vkFast][Debug] gpuInfo->memoryHeaps current process estimated heap usage:" "\n");
      for (unsigned i = 0; i < gpuInfo->memoryHeapsCount; i += 1) {
        vfInternalPrint("[vkFast][Debug]   [");
        red32Uint64ToChars(i, numberString);
        vfInternalPrint(numberString);
        vfInternalPrint("]: ");
        red32Uint64ToChars(memoryBudget.memoryHeapsUsage[i], numberString);
        vfInternalPrint(numberString);
        vfInternalPrint("\n");
      }

      red32MemoryFree(numberString);
    }
  }

  if (gpuInfo->gpuVendorId == 4318/*NVIDIA*/) {
    np(red2ExpectMinimumGuarantees,
      "gpuInfo", gpuInfo,
      "optionalFile", optionalFile,
      "optionalLine", optionalLine
    );
  } else if (gpuInfo->gpuVendorId == 32902/*Intel UHD Graphics 730*/) {
    np(red2ExpectMinimumGuaranteesIntelUHDGraphics730,
      "gpuInfo", gpuInfo,
      "optionalFile", optionalFile,
      "optionalLine", optionalLine
    );
  } else {
    REDGPU_2_EXPECT(!"Unsupported by vkFast GPU, recompile your program with vfContextInit()::enable_debug_mode parameter enabled and email me your GPU name please: iamvfx@gmail.com");
  }

  np(red2ExpectAllMemoryToBeCoherent,
    "gpuInfo", gpuInfo,
    "optionalFile", optionalFile,
    "optionalLine", optionalLine
  );
  
  np(red2ExpectMinimumImageFormatsLimitsAndFeatures, 
    "gpuInfo", gpuInfo,
    "optionalFile", optionalFile,
    "optionalLine", optionalLine
  );

  RedHandleGpu   gpu                  = gpuInfo->gpu;
  unsigned       mainQueueFamilyIndex = gpuInfo->queuesFamilyIndex[vkfast->mainQueueIndex];
  RedHandleQueue mainQueue            = gpuInfo->queues[vkfast->mainQueueIndex];            // NOTE(Constantine): Picking the first available GPU queue by default.

  unsigned specificMemoryTypesGpuVram     = -1;
  unsigned specificMemoryTypesCpuUpload   = -1;
  unsigned specificMemoryTypesCpuReadback = -1;
  {
    if (gpuInfo->gpuVendorId == 4318/*NVIDIA*/) { // NOTE(Constantine): Tested on RTX 2060 and Windows 10.
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
        "gpuInfo", gpuInfo,
        "expectedMemoryHeapsCount", memoryHeapsCount,
        "expectedMemoryHeaps", memoryHeaps,
        "expectedMemoryTypesCount", memoryTypesCount,
        "expectedMemoryTypes", memoryTypes,
        "optionalFile", optionalFile,
        "optionalLine", optionalLine
      );

      specificMemoryTypesGpuVram     = 1;
      specificMemoryTypesCpuUpload   = 3;
      specificMemoryTypesCpuReadback = 4; // NOTE(Constantine): The cpu cached one.

    } else if (gpuInfo->gpuVendorId == 32902/*Intel UHD Graphics*/) { // NOTE(Constantine): Tested on Intel UHD Graphics 730 and Windows 10.
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
        "gpuInfo", gpuInfo,
        "expectedMemoryHeapsCount", memoryHeapsCount,
        "expectedMemoryHeaps", memoryHeaps,
        "expectedMemoryTypesCount", memoryTypesCount,
        "expectedMemoryTypes", memoryTypes,
        "optionalFile", optionalFile,
        "optionalLine", optionalLine
      );

      specificMemoryTypesGpuVram     = 0;
      specificMemoryTypesCpuUpload   = 1;
      specificMemoryTypesCpuReadback = 2; // NOTE(Constantine): The cpu cached one.

    } else {
      REDGPU_2_EXPECTWG(!"Unsupported by vkFast GPU, recompile your program with vfContextInit()::enable_debug_mode parameter enabled and email me your GPU name please: iamvfx@gmail.com");
    }
  }

  Red2Memory memoryGpuVramForArrays_memory      = {0};
  Red2Array  memoryGpuVramForArrays_array       = {0};
  Red2Array  memoryCpuUpload_memory_and_array   = {0};
  void *     memoryCpuUpload_mapped_void_ptr    = NULL;
  Red2Array  memoryCpuReadback_memory_and_array = {0};
  void *     memoryCpuReadback_mapped_void_ptr  = NULL;
  if (internalMemoryAllocationSizeGpuVramArrays > 0) {
    np(red2MemoryAllocate,
      "context", context,
      "gpu", gpu,
      "handleName", "vkFast_memoryGpuVramForArrays_memory",
      "bytesCount", internalMemoryAllocationSizeGpuVramArrays,
      "memoryTypeIndex", specificMemoryTypesGpuVram,
      "memoryBitflags", 0,
      "outMemory", &memoryGpuVramForArrays_memory,
      "outStatuses", NULL,
      "optionalFile", optionalFile,
      "optionalLine", optionalLine,
      "optionalUserData", NULL
    );
    Red2Memory * memorys[1] = {&memoryGpuVramForArrays_memory};
    np(red2CreateArray,
      "context", context,
      "gpu", gpu,
      "handleName", "vkFast_memoryGpuVramForArrays_array",
      "type", RED_ARRAY_TYPE_ARRAY_RW,
      "bytesCount", internalMemoryAllocationSizeGpuVramArrays,
      "structuredBufferElementBytesCount", 0,
      "restrictToAccess", 0,
      "initialQueueFamilyIndex", -1,
      "maxAllowedOverallocationBytesCount", 0,
      "dedicate", 0,
      "mappable", 0,
      "dedicateOrMappableMemoryTypeIndex", 0,
      "dedicateOrMappableMemoryBitflags", 0,
      "suballocateFromMemoryOnFirstMatchPointersCount", 1,
      "suballocateFromMemoryOnFirstMatchPointers", memorys,
      "outArray", &memoryGpuVramForArrays_array,
      "outStatuses", NULL,
      "optionalFile", optionalFile,
      "optionalLine", optionalLine,
      "optionalUserData", NULL
    );
  }
  if (internalMemoryAllocationSizeCpuVisible > 0) {
    np(red2CreateArray,
      "context", context,
      "gpu", gpu,
      "handleName", "vkFast_memoryCpuUpload_memory_and_array",
      "type", RED_ARRAY_TYPE_ARRAY_RO,
      "bytesCount", internalMemoryAllocationSizeCpuVisible,
      "structuredBufferElementBytesCount", 0,
      "restrictToAccess", RED_ACCESS_BITFLAG_COPY_R,
      "initialQueueFamilyIndex", -1,
      "maxAllowedOverallocationBytesCount", 0, // NOTE(Constantine): Intel UHD Graphics 730 on Windows 10 aligns CPU visible allocations to 64 bytes.
      "dedicate", 0,
      "mappable", 1,
      "dedicateOrMappableMemoryTypeIndex", specificMemoryTypesCpuUpload,
      "dedicateOrMappableMemoryBitflags", 0,
      "suballocateFromMemoryOnFirstMatchPointersCount", 0,
      "suballocateFromMemoryOnFirstMatchPointers", NULL,
      "outArray", &memoryCpuUpload_memory_and_array,
      "outStatuses", NULL,
      "optionalFile", optionalFile,
      "optionalLine", optionalLine,
      "optionalUserData", NULL
    );
    np(redMemoryMap,
      "context", context,
      "gpu", gpu,
      "mappableMemory", memoryCpuUpload_memory_and_array.handleAllocatedDedicatedOrMappableMemoryOrPickedMemory,
      "mappableMemoryBytesFirst", 0,
      "mappableMemoryBytesCount", memoryCpuUpload_memory_and_array.array.memoryBytesCount,
      "outVolatilePointer", &memoryCpuUpload_mapped_void_ptr,
      "outStatuses", NULL,
      "optionalFile", optionalFile,
      "optionalLine", optionalLine,
      "optionalUserData", NULL
    );
    REDGPU_2_EXPECTWG(memoryCpuUpload_mapped_void_ptr != NULL);
    REDGPU_2_EXPECTWG(0 == REDGPU_2_BYTES_TO_NEXT_ALIGNMENT_BOUNDARY((uint64_t)memoryCpuUpload_mapped_void_ptr, gpuInfo->minMemoryAllocateBytesAlignment)); // NOTE(Constantine): Start address is guaranteed to be aligned.
  }
  if (internalMemoryAllocationSizeCpuReadback > 0) {
    np(red2CreateArray,
      "context", context,
      "gpu", gpu,
      "handleName", "vkFast_memoryCpuReadback_memory_and_array",
      "type", RED_ARRAY_TYPE_ARRAY_RW,
      "bytesCount", internalMemoryAllocationSizeCpuReadback,
      "structuredBufferElementBytesCount", 0,
      "restrictToAccess", RED_ACCESS_BITFLAG_CPU_RW,
      "initialQueueFamilyIndex", -1,
      "maxAllowedOverallocationBytesCount", 0, // NOTE(Constantine): Intel UHD Graphics 730 on Windows 10 aligns CPU visible allocations to 64 bytes.
      "dedicate", 0,
      "mappable", 1,
      "dedicateOrMappableMemoryTypeIndex", specificMemoryTypesCpuReadback,
      "dedicateOrMappableMemoryBitflags", 0,
      "suballocateFromMemoryOnFirstMatchPointersCount", 0,
      "suballocateFromMemoryOnFirstMatchPointers", NULL,
      "outArray", &memoryCpuReadback_memory_and_array,
      "outStatuses", NULL,
      "optionalFile", optionalFile,
      "optionalLine", optionalLine,
      "optionalUserData", NULL
    );
    np(redMemoryMap,
      "context", context,
      "gpu", gpu,
      "mappableMemory", memoryCpuReadback_memory_and_array.handleAllocatedDedicatedOrMappableMemoryOrPickedMemory,
      "mappableMemoryBytesFirst", 0,
      "mappableMemoryBytesCount", memoryCpuReadback_memory_and_array.array.memoryBytesCount,
      "outVolatilePointer", &memoryCpuReadback_mapped_void_ptr,
      "outStatuses", NULL,
      "optionalFile", optionalFile,
      "optionalLine", optionalLine,
      "optionalUserData", NULL
    );
    REDGPU_2_EXPECTWG(memoryCpuReadback_mapped_void_ptr != NULL);
    REDGPU_2_EXPECTWG(0 == REDGPU_2_BYTES_TO_NEXT_ALIGNMENT_BOUNDARY((uint64_t)memoryCpuReadback_mapped_void_ptr, gpuInfo->minMemoryAllocateBytesAlignment)); // NOTE(Constantine): Start address is guaranteed to be aligned.
  }

  // Filling
  vf_handle_context_t;
  vkfast->doNotDestroyRawContext;
  vkfast->doNotFreeHandle;
  vkfast->isDebugMode = enable_debug_mode;
  vkfast->context = context;
  vkfast->gpuInfo = gpuInfo;
  vkfast->gpuIndex;
  vkfast->gpu = gpu;
  vkfast->mainQueueIndex;
  vkfast->mainQueueFamilyIndex = mainQueueFamilyIndex;
  vkfast->mainQueue = mainQueue;
  vkfast->specificMemoryTypesGpuVram = specificMemoryTypesGpuVram;
  vkfast->specificMemoryTypesCpuUpload = specificMemoryTypesCpuUpload;
  vkfast->specificMemoryTypesCpuReadback = specificMemoryTypesCpuReadback;
  vkfast->memoryGpuVramForArrays_memory = memoryGpuVramForArrays_memory;
  vkfast->memoryGpuVramForArrays_array = memoryGpuVramForArrays_array;
  vkfast->memoryGpuVramForArrays_memory_suballocations_offset = 0;
  vkfast->memoryCpuUpload_memory_and_array = memoryCpuUpload_memory_and_array;
  vkfast->memoryCpuUpload_mapped_void_ptr_original = memoryCpuUpload_mapped_void_ptr;
  vkfast->memoryCpuUpload_mapped_void_ptr_offset = memoryCpuUpload_mapped_void_ptr;
  vkfast->memoryCpuUpload_memory_suballocations_offset = 0;
  vkfast->memoryCpuReadback_memory_and_array = memoryCpuReadback_memory_and_array;
  vkfast->memoryCpuReadback_mapped_void_ptr_original = memoryCpuReadback_mapped_void_ptr;
  vkfast->memoryCpuReadback_mapped_void_ptr_offset = memoryCpuReadback_mapped_void_ptr;
  vkfast->memoryCpuReadback_memory_suballocations_offset = 0;
  vkfast->windowHandle = NULL;
  vkfast->screenWidth = 0;
  vkfast->screenHeight = 0;
  vkfast->presentQueueIndex = 0;
  vkfast->surface = NULL;
  vkfast->present = NULL;
  vkfast->presentImages[0] = NULL;
  vkfast->presentImages[1] = NULL;
  vkfast->presentImages[2] = NULL;
  vkfast->presentGpuSignal = NULL;
  vkfast->presentCopyCalls = REDGPU_32_STRUCT(RedCalls, 0);
  vkfast->presentPixelsStorageCpuUpload = REDGPU_32_STRUCT(gpu_storage_t, 0);

  return (gpu_handle_context_t)(void *)vkfast;
}

GPU_API_PRE void GPU_API_POST vfIdDestroy(uint64_t ids_count, const uint64_t * ids, const char * optionalFile, int optionalLine) {
  for (uint64_t i = 0; i < ids_count; i += 1) {
    vf_handle_t * handle = (vf_handle_t *)(void *)ids[i];
    if (handle == NULL) {
      continue;
    }
    if (handle->handle_id == VF_HANDLE_ID_INVALID) {
      continue;
    }

    if (handle->handle_id == VF_HANDLE_ID_GPU_CODE) {
      np(red2DestroyHandle,
        "context", handle->vkfast->context,
        "gpu", handle->vkfast->gpu,
        "handleType", RED_HANDLE_TYPE_GPU_CODE,
        "handle", handle->gpuCode.gpuCode,
        "optionalHandle2", NULL,
        "optionalFile", optionalFile,
        "optionalLine", optionalLine,
        "optionalUserData", NULL
      );
      continue;
    }

    if (handle->handle_id == VF_HANDLE_ID_PROCEDURE) {
      np(red2DestroyHandle,
        "context", handle->vkfast->context,
        "gpu", handle->vkfast->gpu,
        "handleType", RED_HANDLE_TYPE_PROCEDURE_PARAMETERS,
        "handle", handle->procedure.procedureParameters,
        "optionalHandle2", NULL,
        "optionalFile", optionalFile,
        "optionalLine", optionalLine,
        "optionalUserData", NULL
      );
      np(red2DestroyHandle,
        "context", handle->vkfast->context,
        "gpu", handle->vkfast->gpu,
        "handleType", RED_HANDLE_TYPE_PROCEDURE,
        "handle", handle->procedure.procedure,
        "optionalHandle2", NULL,
        "optionalFile", optionalFile,
        "optionalLine", optionalLine,
        "optionalUserData", NULL
      );
      continue;
    }

    if (handle->handle_id == VF_HANDLE_ID_BATCH) {
      np(red2DestroyHandle,
        "context", handle->vkfast->context,
        "gpu", handle->vkfast->gpu,
        "handleType", RED_HANDLE_TYPE_STRUCTS_MEMORY,
        "handle", handle->batch.structsMemory,
        "optionalHandle2", NULL,
        "optionalFile", optionalFile,
        "optionalLine", optionalLine,
        "optionalUserData", NULL
      );
      np(red2DestroyHandle,
        "context", handle->vkfast->context,
        "gpu", handle->vkfast->gpu,
        "handleType", RED_HANDLE_TYPE_CALLS,
        "handle", handle->batch.calls.handle,
        "optionalHandle2", handle->batch.calls.memory,
        "optionalFile", optionalFile,
        "optionalLine", optionalLine,
        "optionalUserData", NULL
      );
      continue;
    }
  }

  for (uint64_t i = 0; i < ids_count; i += 1) {
    vf_handle_t * handle = (vf_handle_t *)(void *)ids[i];
    red32MemoryFree(handle); // NOTE(Constantine): Internally, all handles must be allocated, except for async_id.
  }
}

GPU_API_PRE void GPU_API_POST vfContextDeinit(gpu_handle_context_t context, const char * optionalFile, int optionalLine) {
  vf_handle_context_t * vkfast = (vf_handle_context_t *)(void *)context;

  vfAsyncDrawWaitToFinish(context, optionalFile, optionalLine);

  // NOTE(Constantine): WSI.
  {
    vfIdDestroy(1, &vkfast->presentPixelsStorageCpuUpload.id, optionalFile, optionalLine);

    np(red2DestroyHandle,
      "context", vkfast->context,
      "gpu", vkfast->gpu,
      "handleType", RED_HANDLE_TYPE_CALLS,
      "handle", vkfast->presentCopyCalls.handle,
      "optionalHandle2", vkfast->presentCopyCalls.memory,
      "optionalFile", optionalFile,
      "optionalLine", optionalLine,
      "optionalUserData", NULL
    );

    np(red2DestroyHandle,
      "context", vkfast->context,
      "gpu", vkfast->gpu,
      "handleType", RED_HANDLE_TYPE_GPU_SIGNAL,
      "handle", vkfast->presentGpuSignal,
      "optionalHandle2", NULL,
      "optionalFile", optionalFile,
      "optionalLine", optionalLine,
      "optionalUserData", NULL
    );

    np(red2DestroyHandle,
      "context", vkfast->context,
      "gpu", vkfast->gpu,
      "handleType", RED_HANDLE_TYPE_PRESENT,
      "handle", vkfast->present,
      "optionalHandle2", NULL,
      "optionalFile", optionalFile,
      "optionalLine", optionalLine,
      "optionalUserData", NULL
    );

    np(red2DestroyHandle,
      "context", vkfast->context,
      "gpu", vkfast->gpu,
      "handleType", RED_HANDLE_TYPE_SURFACE,
      "handle", vkfast->surface,
      "optionalHandle2", NULL,
      "optionalFile", optionalFile,
      "optionalLine", optionalLine,
      "optionalUserData", NULL
    );
  }

  np(red2DestroyHandle,
    "context", vkfast->context,
    "gpu", vkfast->gpu,
    "handleType", RED_HANDLE_TYPE_ARRAY,
    "handle", vkfast->memoryGpuVramForArrays_array.array.handle,
    "optionalHandle2", NULL,
    "optionalFile", optionalFile,
    "optionalLine", optionalLine,
    "optionalUserData", NULL
  );
  np(red2DestroyHandle,
    "context", vkfast->context,
    "gpu", vkfast->gpu,
    "handleType", RED_HANDLE_TYPE_MEMORY,
    "handle", vkfast->memoryGpuVramForArrays_memory.handle,
    "optionalHandle2", NULL,
    "optionalFile", optionalFile,
    "optionalLine", optionalLine,
    "optionalUserData", NULL
  );

  if (vkfast->memoryCpuUpload_memory_and_array.handleAllocatedDedicatedOrMappableMemoryOrPickedMemory != NULL) {
    np(redMemoryUnmap,
      "context", vkfast->context,
      "gpu", vkfast->gpu,
      "mappableMemory", vkfast->memoryCpuUpload_memory_and_array.handleAllocatedDedicatedOrMappableMemoryOrPickedMemory,
      "optionalFile", optionalFile,
      "optionalLine", optionalLine,
      "optionalUserData", NULL
    );
  }
  np(red2DestroyHandle,
    "context", vkfast->context,
    "gpu", vkfast->gpu,
    "handleType", RED_HANDLE_TYPE_ARRAY,
    "handle", vkfast->memoryCpuUpload_memory_and_array.array.handle,
    "optionalHandle2", NULL,
    "optionalFile", optionalFile,
    "optionalLine", optionalLine,
    "optionalUserData", NULL
  );
  np(red2DestroyHandle,
    "context", vkfast->context,
    "gpu", vkfast->gpu,
    "handleType", RED_HANDLE_TYPE_MEMORY,
    "handle", vkfast->memoryCpuUpload_memory_and_array.handleAllocatedDedicatedOrMappableMemoryOrPickedMemory,
    "optionalHandle2", NULL,
    "optionalFile", optionalFile,
    "optionalLine", optionalLine,
    "optionalUserData", NULL
  );

  if (vkfast->memoryCpuReadback_memory_and_array.handleAllocatedDedicatedOrMappableMemoryOrPickedMemory != NULL) {
    np(redMemoryUnmap,
      "context", vkfast->context,
      "gpu", vkfast->gpu,
      "mappableMemory", vkfast->memoryCpuReadback_memory_and_array.handleAllocatedDedicatedOrMappableMemoryOrPickedMemory,
      "optionalFile", optionalFile,
      "optionalLine", optionalLine,
      "optionalUserData", NULL
    );
  }
  np(red2DestroyHandle,
    "context", vkfast->context,
    "gpu", vkfast->gpu,
    "handleType", RED_HANDLE_TYPE_ARRAY,
    "handle", vkfast->memoryCpuReadback_memory_and_array.array.handle,
    "optionalHandle2", NULL,
    "optionalFile", optionalFile,
    "optionalLine", optionalLine,
    "optionalUserData", NULL
  );
  np(red2DestroyHandle,
    "context", vkfast->context,
    "gpu", vkfast->gpu,
    "handleType", RED_HANDLE_TYPE_MEMORY,
    "handle", vkfast->memoryCpuReadback_memory_and_array.handleAllocatedDedicatedOrMappableMemoryOrPickedMemory,
    "optionalHandle2", NULL,
    "optionalFile", optionalFile,
    "optionalLine", optionalLine,
    "optionalUserData", NULL
  );

  if (vkfast->doNotDestroyRawContext == 0) {
    np(redDestroyContext,
      "context", vkfast->context,
      "optionalFile", optionalFile,
      "optionalLine", optionalLine,
      "optionalUserData", NULL
    );
  }

  if (vkfast->doNotFreeHandle == 0) {
    red32MemoryFree(vkfast);
  }
}

GPU_API_PRE RedContext GPU_API_POST vfContextGetRaw(gpu_handle_context_t context, const char * optionalFile, int optionalLine) {
  vf_handle_context_t * vkfast = (vf_handle_context_t *)(void *)context;

  REDGPU_2_EXPECT(vkfast != NULL);

  return vkfast->context;
}

GPU_API_PRE void GPU_API_POST vfContextResetAndInvalidateAllStorages(gpu_handle_context_t context, const char * optionalFile, int optionalLine) {
  vf_handle_context_t * vkfast = (vf_handle_context_t *)(void *)context;

  vkfast->memoryGpuVramForArrays_memory_suballocations_offset = 0;
  vkfast->memoryCpuUpload_mapped_void_ptr_offset = vkfast->memoryCpuUpload_mapped_void_ptr_original;
  vkfast->memoryCpuUpload_memory_suballocations_offset = 0;
  vkfast->memoryCpuReadback_mapped_void_ptr_offset = vkfast->memoryCpuReadback_mapped_void_ptr_original;
  vkfast->memoryCpuReadback_memory_suballocations_offset = 0;
}

GPU_API_PRE void GPU_API_POST vfGetMainMonitorAreaRectangle(int * out4ints, const char * optionalFile, int optionalLine) {
  // https://learn.microsoft.com/ru-ru/windows/win32/api/winuser/nf-winuser-monitorfrompoint
  POINT point = {0};
  HMONITOR hmonitor = MonitorFromPoint(point, MONITOR_DEFAULTTONEAREST);

  // https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-getmonitorinfoa
  MONITORINFO monitorInfo = {0};
  monitorInfo.cbSize = sizeof(MONITORINFO);
  REDGPU_2_EXPECT(GetMonitorInfoA(hmonitor, &monitorInfo));

  out4ints[0] = monitorInfo.rcMonitor.left;
  out4ints[1] = monitorInfo.rcMonitor.top;
  out4ints[2] = monitorInfo.rcMonitor.right;
  out4ints[3] = monitorInfo.rcMonitor.bottom;
}

static int vfInternalRebuildPresent(gpu_handle_context_t context, const char * optionalFile, int optionalLine) {
  vf_handle_context_t * vkfast = (vf_handle_context_t *)(void *)context;

  RedHandleGpu gpu = vkfast->gpu;

  vfAsyncDrawWaitToFinish(context, optionalFile, optionalLine);

  RedHandlePresent present = NULL;
  RedHandleImage presentImages[3] = {0};
  RedHandleGpuSignal presentGpuSignal = NULL;
  gpu_storage_t presentPixelsStorageCpuUpload = {0};

  if (vkfast->surface == NULL) {
    #ifdef _WIN32
    np(redCreateSurfaceWin32,
      "context", vkfast->context,
      "gpu", vkfast->gpu,
      "handleName", "vkFast_surface_win32",
      "win32Hinstance", GetModuleHandle(NULL),
      "win32Hwnd", vkfast->windowHandle,
      "outSurface", &vkfast->surface,
      "outStatuses", NULL,
      "optionalFile", optionalFile,
      "optionalLine", optionalLine,
      "optionalUserData", NULL
    );
    #endif
    REDGPU_2_EXPECTWG(vkfast->surface != NULL);
  }

  RedQueueFamilyIndexGetSupportsPresentOnSurface queueFamilyIndexSupportsPresentOnSurface = {0};
  queueFamilyIndexSupportsPresentOnSurface.surface                                     = vkfast->surface;
  queueFamilyIndexSupportsPresentOnSurface.outQueueFamilyIndexSupportsPresentOnSurface = 0;
  np(redQueueFamilyIndexGetSupportsPresent,
    "context", vkfast->context,
    "gpu", vkfast->gpu,
    "queueFamilyIndex", vkfast->gpuInfo->queuesFamilyIndex[vkfast->presentQueueIndex],
    "supportsPresentOnWin32", NULL,
    "supportsPresentOnXlib", NULL,
    "supportsPresentOnXcb", NULL,
    "supportsPresentOnSurface", &queueFamilyIndexSupportsPresentOnSurface,
    "outStatuses", NULL,
    "optionalFile", optionalFile,
    "optionalLine", optionalLine,
    "optionalUserData", NULL
  );
  REDGPU_2_EXPECTWG(queueFamilyIndexSupportsPresentOnSurface.outQueueFamilyIndexSupportsPresentOnSurface == 1);

  RedSurfaceCurrentPropertiesAndPresentLimits surfaceCurrentPropertiesAndPresentLimits = {0};
  np(redSurfaceGetCurrentPropertiesAndPresentLimits,
    "context", vkfast->context,
    "gpu", vkfast->gpu,
    "surface", vkfast->surface,
    "outSurfaceCurrentPropertiesAndPresentLimits", &surfaceCurrentPropertiesAndPresentLimits,
    "outStatuses", NULL,
    "optionalFile", optionalFile,
    "optionalLine", optionalLine,
    "optionalUserData", NULL
  );
  // printf("[vkFast][DEBUG PRINTF] surfaceCurrentPropertiesAndPresentLimits.currentSurfaceWidth/Height: %d, %d\n", surfaceCurrentPropertiesAndPresentLimits.currentSurfaceWidth, surfaceCurrentPropertiesAndPresentLimits.currentSurfaceHeight);
  if (surfaceCurrentPropertiesAndPresentLimits.currentSurfaceWidth == 0 && surfaceCurrentPropertiesAndPresentLimits.currentSurfaceHeight == 0) {
    return 1; // NOTE(Constantine): The window is minimized then. Tested on Windows 10.
  }
  if (surfaceCurrentPropertiesAndPresentLimits.currentSurfaceWidth != -1 && surfaceCurrentPropertiesAndPresentLimits.currentSurfaceHeight != -1) {
    vkfast->screenWidth  = surfaceCurrentPropertiesAndPresentLimits.currentSurfaceWidth;
    vkfast->screenHeight = surfaceCurrentPropertiesAndPresentLimits.currentSurfaceHeight;
  }

  // NOTE(Constantine): WSI destroy before the rebuild.
  {
    if (vkfast->presentPixelsStorageCpuUpload.id != 0) {
      vfIdDestroy(1, &vkfast->presentPixelsStorageCpuUpload.id, optionalFile, optionalLine);
      vkfast->presentPixelsStorageCpuUpload = REDGPU_32_STRUCT(gpu_storage_t, 0);
    }

    if (vkfast->presentCopyCalls.handle != NULL) {
      np(red2DestroyHandle,
        "context", vkfast->context,
        "gpu", vkfast->gpu,
        "handleType", RED_HANDLE_TYPE_CALLS,
        "handle", vkfast->presentCopyCalls.handle,
        "optionalHandle2", vkfast->presentCopyCalls.memory,
        "optionalFile", optionalFile,
        "optionalLine", optionalLine,
        "optionalUserData", NULL
      );
      vkfast->presentCopyCalls = REDGPU_32_STRUCT(RedCalls, 0);
    }

    if (vkfast->presentGpuSignal != NULL) {
      np(red2DestroyHandle,
        "context", vkfast->context,
        "gpu", vkfast->gpu,
        "handleType", RED_HANDLE_TYPE_GPU_SIGNAL,
        "handle", vkfast->presentGpuSignal,
        "optionalHandle2", NULL,
        "optionalFile", optionalFile,
        "optionalLine", optionalLine,
        "optionalUserData", NULL
      );
      vkfast->presentGpuSignal = NULL;
    }

    if (vkfast->present != NULL) {
      np(red2DestroyHandle,
        "context", vkfast->context,
        "gpu", vkfast->gpu,
        "handleType", RED_HANDLE_TYPE_PRESENT,
        "handle", vkfast->present,
        "optionalHandle2", NULL,
        "optionalFile", optionalFile,
        "optionalLine", optionalLine,
        "optionalUserData", NULL
      );
      vkfast->present = NULL;
    }
  }

  np(redCreatePresent,
    "context", vkfast->context,
    "gpu", vkfast->gpu,
    "queue", vkfast->gpuInfo->queues[vkfast->presentQueueIndex],
    "handleName", "vkFast_present",
    "surface", vkfast->surface,
    "imagesCount", 3,
    "imagesWidth", vkfast->screenWidth,
    "imagesHeight", vkfast->screenHeight,
    "imagesLayersCount", 1,
    "imagesRestrictToAccess", RED_ACCESS_BITFLAG_COPY_W,
    "transform", RED_SURFACE_TRANSFORM_BITFLAG_IDENTITY,
    "compositeAlpha", RED_SURFACE_COMPOSITE_ALPHA_BITFLAG_OPAQUE,
    "vsyncMode", RED_PRESENT_VSYNC_MODE_ON,
    "clipped", 0,
    "discardAfterPresent", 1, // NOTE(Constantine): Optimization.
    "oldPresent", NULL,
    "outPresent", &present,
    "outImages", presentImages,
    "outTextures", NULL,
    "outStatuses", NULL,
    "optionalFile", optionalFile,
    "optionalLine", optionalLine,
    "optionalUserData", NULL
  );
  REDGPU_2_EXPECTWG(present != NULL);

  np(redCreateGpuSignal,
    "context", vkfast->context,
    "gpu", vkfast->gpu,
    "handleName", "vkFast_present_gpuSignal",
    "outGpuSignal", &presentGpuSignal,
    "outStatuses", NULL,
    "optionalFile", optionalFile,
    "optionalLine", optionalLine,
    "optionalUserData", NULL
  );
  REDGPU_2_EXPECTWG(presentGpuSignal != NULL);

  RedCalls presentCopyCalls = {0};
  np(redCreateCalls,
    "context", vkfast->context,
    "gpu", vkfast->gpu,
    "handleName", "vkFast_present_copyCalls",
    "queueFamilyIndex", vkfast->gpuInfo->queuesFamilyIndex[vkfast->presentQueueIndex],
    "outCalls", &presentCopyCalls,
    "outStatuses", NULL,
    "optionalFile", optionalFile,
    "optionalLine", optionalLine,
    "optionalUserData", NULL
  );
  REDGPU_2_EXPECTWG(presentCopyCalls.handle != NULL);
  REDGPU_2_EXPECTWG(presentCopyCalls.memory != NULL);

  gpu_storage_info_t storage_info = {0};
  storage_info.storage_type = GPU_STORAGE_TYPE_CPU_UPLOAD;
  storage_info.bytes_count = sizeof(unsigned char) * 4 * vkfast->screenHeight * vkfast->screenWidth;
  vfStorageCreate(context, &storage_info, &presentPixelsStorageCpuUpload, optionalFile, optionalLine);

  vkfast->present = present;
  vkfast->presentImages[0] = presentImages[0];
  vkfast->presentImages[1] = presentImages[1];
  vkfast->presentImages[2] = presentImages[2];
  vkfast->presentGpuSignal = presentGpuSignal;
  vkfast->presentCopyCalls = presentCopyCalls;
  vkfast->presentPixelsStorageCpuUpload = presentPixelsStorageCpuUpload;

  return 0;
}

GPU_API_PRE void GPU_API_POST vfWindowFullscreen(gpu_handle_context_t context, void * optional_external_window_handle, const char * window_title, int screen_width, int screen_height, unsigned draw_queue_index, const char * optionalFile, int optionalLine) {
  vf_handle_context_t * vkfast = (vf_handle_context_t *)(void *)context;
  
  RedHandleGpu gpu = vkfast->gpu;

  REDGPU_2_EXPECTWG(draw_queue_index < vkfast->gpuInfo->queuesCount);

  void * window_handle = optional_external_window_handle;
  if (window_handle == NULL) {
    window_handle = red32WindowCreate(window_title);
  }
  REDGPU_2_EXPECTWG(window_handle != NULL);

  vkfast->windowHandle = window_handle;
  vkfast->screenWidth = screen_width;
  vkfast->screenHeight = screen_height;
  vkfast->presentQueueIndex = draw_queue_index;

  vfInternalRebuildPresent(context, optionalFile, optionalLine);
}

GPU_API_PRE int GPU_API_POST vfWindowLoop(gpu_handle_context_t context) {
  return red32WindowLoop();
}

GPU_API_PRE void GPU_API_POST vfWindowGetSize(gpu_handle_context_t context, int * out_window_width, int * out_window_height) {
  vf_handle_context_t * vkfast = (vf_handle_context_t *)(void *)context;

  if (out_window_width  != NULL) { out_window_width[0]  = vkfast->screenWidth;  }
  if (out_window_height != NULL) { out_window_height[0] = vkfast->screenHeight; }
}

GPU_API_PRE void GPU_API_POST vfExit(int exit_code) {
  red32Exit(exit_code);
}

GPU_API_PRE void GPU_API_POST vfStorageCreate(gpu_handle_context_t context, const gpu_storage_info_t * storage_info, gpu_storage_t * out_storage, const char * optionalFile, int optionalLine) {
  vf_handle_context_t * vkfast = (vf_handle_context_t *)(void *)context;

  RedHandleGpu gpu = vkfast->gpu;

  REDGPU_2_EXPECTWG(storage_info->storage_type != GPU_STORAGE_TYPE_NONE);

  uint64_t             alignment         = 0;
  RedStructMemberArray arrayRangeInfo    = {0};
  void *               mappedVoidPointer = NULL;
  {
    // NOTE(Constantine): Storage range mapping.

    if (storage_info->storage_type == GPU_STORAGE_TYPE_GPU_ONLY) {
      
      alignment = vkfast->gpuInfo->minArrayRORWStructMemberRangeBytesAlignment;

      // NOTE(Constantine): Aligns start address.
      vkfast->memoryGpuVramForArrays_memory_suballocations_offset += REDGPU_2_BYTES_TO_NEXT_ALIGNMENT_BOUNDARY(vkfast->memoryGpuVramForArrays_memory_suballocations_offset, alignment);

      arrayRangeInfo.array                = vkfast->memoryGpuVramForArrays_array.array.handle;
      arrayRangeInfo.arrayRangeBytesFirst = vkfast->memoryGpuVramForArrays_memory_suballocations_offset;
      arrayRangeInfo.arrayRangeBytesCount = storage_info->bytes_count + REDGPU_2_BYTES_TO_NEXT_ALIGNMENT_BOUNDARY(storage_info->bytes_count, alignment);
      vkfast->memoryGpuVramForArrays_memory_suballocations_offset += arrayRangeInfo.arrayRangeBytesCount;

      REDGPU_2_EXPECTWG(vkfast->memoryGpuVramForArrays_memory_suballocations_offset <= vkfast->memoryGpuVramForArrays_array.array.memoryBytesCount);

    } else if (storage_info->storage_type == GPU_STORAGE_TYPE_CPU_UPLOAD) {
    
      alignment = vkfast->gpuInfo->minMemoryAllocateBytesAlignment; // NOTE(Constantine): Can't be placed into a struct, so picking only one alignment.

      // NOTE(Constantine): Aligns start address.
      vkfast->memoryCpuUpload_memory_suballocations_offset += REDGPU_2_BYTES_TO_NEXT_ALIGNMENT_BOUNDARY(vkfast->memoryCpuUpload_memory_suballocations_offset, alignment);

      arrayRangeInfo.array                = vkfast->memoryCpuUpload_memory_and_array.array.handle;
      arrayRangeInfo.arrayRangeBytesFirst = vkfast->memoryCpuUpload_memory_suballocations_offset;
      arrayRangeInfo.arrayRangeBytesCount = storage_info->bytes_count + REDGPU_2_BYTES_TO_NEXT_ALIGNMENT_BOUNDARY(storage_info->bytes_count, alignment);
      vkfast->memoryCpuUpload_memory_suballocations_offset += arrayRangeInfo.arrayRangeBytesCount;

      REDGPU_2_EXPECTWG(vkfast->memoryCpuUpload_memory_suballocations_offset <= vkfast->memoryCpuUpload_memory_and_array.array.memoryBytesCount);

    } else if (storage_info->storage_type == GPU_STORAGE_TYPE_CPU_READBACK) {
    
      alignment = vkfast->gpuInfo->minMemoryAllocateBytesAlignment; // NOTE(Constantine): Can't be placed into a struct, so picking only one alignment.

      // NOTE(Constantine): Aligns start address.
      vkfast->memoryCpuReadback_memory_suballocations_offset += REDGPU_2_BYTES_TO_NEXT_ALIGNMENT_BOUNDARY(vkfast->memoryCpuReadback_memory_suballocations_offset, alignment);

      arrayRangeInfo.array                = vkfast->memoryCpuReadback_memory_and_array.array.handle;
      arrayRangeInfo.arrayRangeBytesFirst = vkfast->memoryCpuReadback_memory_suballocations_offset;
      arrayRangeInfo.arrayRangeBytesCount = storage_info->bytes_count + REDGPU_2_BYTES_TO_NEXT_ALIGNMENT_BOUNDARY(storage_info->bytes_count, alignment);
      vkfast->memoryCpuReadback_memory_suballocations_offset += arrayRangeInfo.arrayRangeBytesCount;

      REDGPU_2_EXPECTWG(vkfast->memoryCpuReadback_memory_suballocations_offset <= vkfast->memoryCpuReadback_memory_and_array.array.memoryBytesCount);

    } else {
      REDGPU_2_EXPECT(!"[vkFast Internal][" __FUNCTION__ "] Unreachable enum value.");
    }

    // NOTE(Constantine): Pointer mapping.

    if (storage_info->storage_type == GPU_STORAGE_TYPE_CPU_UPLOAD) {
    
      mappedVoidPointer = vkfast->memoryCpuUpload_mapped_void_ptr_offset; // NOTE(Constantine): Start address is guaranteed to be aligned.
      
      uint8_t * ptr = (uint8_t *)vkfast->memoryCpuUpload_mapped_void_ptr_offset;
      ptr += arrayRangeInfo.arrayRangeBytesCount + REDGPU_2_BYTES_TO_NEXT_ALIGNMENT_BOUNDARY(arrayRangeInfo.arrayRangeBytesCount, alignment);
      vkfast->memoryCpuUpload_mapped_void_ptr_offset = (void *)ptr;

    } else if (storage_info->storage_type == GPU_STORAGE_TYPE_CPU_READBACK) {
    
      mappedVoidPointer = vkfast->memoryCpuReadback_mapped_void_ptr_offset; // NOTE(Constantine): Start address is guaranteed to be aligned.
      
      uint8_t * ptr = (uint8_t *)vkfast->memoryCpuReadback_mapped_void_ptr_offset;
      ptr += arrayRangeInfo.arrayRangeBytesCount + REDGPU_2_BYTES_TO_NEXT_ALIGNMENT_BOUNDARY(arrayRangeInfo.arrayRangeBytesCount, alignment);
      vkfast->memoryCpuReadback_mapped_void_ptr_offset = (void *)ptr;

    }
  }

  // To free
  vf_handle_t * handle = (vf_handle_t *)red32MemoryCalloc(sizeof(vf_handle_t));
  REDGPU_2_EXPECTWG(handle != NULL);

  // Filling
  vf_handle_t;
  vf_handle_storage_t;
  handle->vkfast                 = vkfast;
  handle->handle_id              = VF_HANDLE_ID_STORAGE;
  handle->storage.info           = storage_info[0];
  handle->storage.arrayRangeInfo = arrayRangeInfo;

  // Filling
  gpu_storage_t;
  out_storage->id              = (uint64_t)(void *)handle;
  out_storage->info            = storage_info[0];
  out_storage->alignment       = alignment;
  out_storage->mapped_void_ptr = mappedVoidPointer;
}

GPU_API_PRE void GPU_API_POST vfStorageGetRaw(gpu_handle_context_t context, uint64_t storage_id, RedStructMemberArray * out_storage_raw, const char * optionalFile, int optionalLine) {
  vf_handle_t * storage = (vf_handle_t *)(void *)storage_id;
  vf_handle_context_t * vkfast = storage->vkfast;
  RedHandleGpu gpu = vkfast->gpu;
  REDGPU_2_EXPECTWG(storage->handle_id == VF_HANDLE_ID_STORAGE);

  out_storage_raw[0] = storage->storage.arrayRangeInfo;
}

GPU_API_PRE uint64_t GPU_API_POST vfProgramCreateFromBinaryCompute(gpu_handle_context_t context, const gpu_program_info_t * program_info, const char * optionalFile, int optionalLine) {
  vf_handle_context_t * vkfast = (vf_handle_context_t *)(void *)context;

  RedHandleGpu gpu = vkfast->gpu;

  // To destroy
  RedHandleGpuCode gpuCode = NULL;
  np(redCreateGpuCode,
    "context", vkfast->context,
    "gpu", vkfast->gpu,
    "handleName", program_info->optional_debug_name,
    "irBytesCount", program_info->program_binary_bytes_count,
    "ir", (const void *)program_info->program_binary,
    "outGpuCode", &gpuCode,
    "outStatuses", NULL,
    "optionalFile", optionalFile,
    "optionalLine", optionalLine,
    "optionalUserData", NULL
  );

  // To free
  vf_handle_t * handle = (vf_handle_t *)red32MemoryCalloc(sizeof(vf_handle_t));
  REDGPU_2_EXPECTWG(handle != NULL);

  vf_handle_t;
  vf_handle_gpu_code_t;
  handle->vkfast              = vkfast;
  handle->handle_id           = VF_HANDLE_ID_GPU_CODE;
  handle->gpuCode.info        = program_info[0];
  handle->gpuCode.gpuCodeType = VF_GPU_CODE_TYPE_COMPUTE;
  handle->gpuCode.gpuCode     = gpuCode;

  return (uint64_t)(void *)handle;
}

GPU_API_PRE uint64_t GPU_API_POST vfProgramPipelineCreateCompute(gpu_handle_context_t context, const gpu_program_pipeline_compute_info_t * program_pipeline_compute_info, const char * optionalFile, int optionalLine) {
  vf_handle_context_t * vkfast = (vf_handle_context_t *)(void *)context;
  
  RedHandleGpu gpu = vkfast->gpu;

  vf_handle_t * gpuCodeCompute = (vf_handle_t *)(void *)program_pipeline_compute_info->compute_program;
  REDGPU_2_EXPECTWG(gpuCodeCompute->handle_id == VF_HANDLE_ID_GPU_CODE);
  REDGPU_2_EXPECTWG(gpuCodeCompute->gpuCode.gpuCodeType == VF_GPU_CODE_TYPE_COMPUTE);

  if (program_pipeline_compute_info->variables_bytes_count > 0) {
    for (unsigned i = 0; i < program_pipeline_compute_info->struct_members_count; i += 1) {
      REDGPU_2_EXPECTWG(program_pipeline_compute_info->variables_slot != program_pipeline_compute_info->struct_members[i].slot);
    }
  }

  Red2ProcedureParametersDeclaration parameters = {0};
  parameters.variablesSlot            = program_pipeline_compute_info->variables_slot;
  parameters.variablesVisibleToStages = program_pipeline_compute_info->variables_bytes_count == 0 ? 0 : RED_VISIBLE_TO_STAGE_BITFLAG_COMPUTE;
  parameters.variablesBytesCount      = program_pipeline_compute_info->variables_bytes_count;
  parameters.structsDeclarationsCount = program_pipeline_compute_info->struct_members_count == 0 ? 0 : 1;
  parameters.structsDeclarations[0].structDeclarationMembersCount        = program_pipeline_compute_info->struct_members_count;
  parameters.structsDeclarations[0].structDeclarationMembers             = program_pipeline_compute_info->struct_members;
  parameters.structsDeclarations[0].structDeclarationMembersArrayROCount = 0;
  parameters.structsDeclarations[0].structDeclarationMembersArrayRO      = NULL;

  // To destroy
  RedHandleProcedureParameters procedureParameters = NULL;
  np(red2CreateProcedureParameters,
    "context", vkfast->context,
    "gpu", vkfast->gpu,
    "handleName", program_pipeline_compute_info->optional_debug_name,
    "procedureParametersDeclaration", &parameters,
    "outProcedureParameters", &procedureParameters,
    "outStatuses", NULL,
    "optionalFile", optionalFile,
    "optionalLine", optionalLine,
    "optionalUserData", NULL
  );

  // To destroy
  RedHandleProcedure procedure = NULL;
  np(redCreateProcedureCompute,
    "context", vkfast->context,
    "gpu", vkfast->gpu,
    "handleName", program_pipeline_compute_info->optional_debug_name,
    "procedureCache", NULL,
    "procedureParameters", procedureParameters,
    "gpuCodeMainProcedureName", "main",
    "gpuCode", gpuCodeCompute->gpuCode.gpuCode,
    "outProcedure", &procedure,
    "outStatuses", NULL,
    "optionalFile", optionalFile,
    "optionalLine", optionalLine,
    "optionalUserData", NULL
  );

  // To free
  vf_handle_t * handle = (vf_handle_t *)red32MemoryCalloc(sizeof(vf_handle_t));
  REDGPU_2_EXPECTWG(handle != NULL);

  // Filling
  vf_handle_t;
  vf_handle_procedure_t;
  handle->vkfast                        = vkfast;
  handle->handle_id                     = VF_HANDLE_ID_PROCEDURE;
  handle->procedure.infoCompute         = program_pipeline_compute_info[0];
  handle->procedure.procedureType       = VF_PROCEDURE_TYPE_COMPUTE;
  handle->procedure.procedureParameters = procedureParameters;
  handle->procedure.procedure           = procedure;

  return (uint64_t)(void *)handle;
}

GPU_API_PRE uint64_t GPU_API_POST vfBatchBegin(gpu_handle_context_t context, uint64_t existing_batch_id, const gpu_batch_info_t * batch_info, const char * optional_debug_name, const char * optionalFile, int optionalLine) {
  vf_handle_t * handle = (vf_handle_t *)(void *)existing_batch_id;
  
  vf_handle_context_t * vkfast = (vf_handle_context_t *)(void *)context;

  RedHandleGpu gpu = vkfast->gpu;

  if (handle == NULL) {
    // To destroy
    RedCalls calls = {0};
    np(redCreateCallsReusable,
      "context", vkfast->context,
      "gpu", vkfast->gpu,
      "handleName", optional_debug_name,
      "queueFamilyIndex", vkfast->mainQueueFamilyIndex,
      "outCalls", &calls,
      "outStatuses", NULL,
      "optionalFile", optionalFile,
      "optionalLine", optionalLine,
      "optionalUserData", NULL
    );
  
    RedCallProceduresAndAddresses addresses = {0};
    np(redGetCallProceduresAndAddresses,
      "context", vkfast->context,
      "gpu", vkfast->gpu,
      "outCallProceduresAndAddresses", &addresses,
      "outStatuses", NULL,
      "optionalFile", optionalFile,
      "optionalLine", optionalLine,
      "optionalUserData", NULL
    );

    // To destroy
    RedHandleStructsMemory structsMemory = 0;
    if (batch_info != NULL) {
      if (batch_info->max_new_bindings_sets_count > 0) {
        np(redStructsMemoryAllocate,
          "context", vkfast->context,
          "gpu", vkfast->gpu,
          "handleName", optional_debug_name,
          "maxStructsCount", batch_info->max_new_bindings_sets_count,
          "maxStructsMembersOfTypeArrayROConstantCount", 0,
          "maxStructsMembersOfTypeArrayROOrArrayRWCount", batch_info->max_storage_binds_count,
          "maxStructsMembersOfTypeTextureROCount", 0,
          "maxStructsMembersOfTypeTextureRWCount", 0,
          "outStructsMemory", &structsMemory,
          "outStatuses", NULL,
          "optionalFile", optionalFile,
          "optionalLine", optionalLine,
          "optionalUserData", NULL
        );
      }
    }

    // To free
    handle = (vf_handle_t *)red32MemoryCalloc(sizeof(vf_handle_t));
    REDGPU_2_EXPECTWG(handle != NULL);

    // Filling
    vf_handle_t;
    vf_handle_batch_t;
    handle->vkfast              = vkfast;
    handle->handle_id           = VF_HANDLE_ID_BATCH;
    handle->batch.calls         = calls;
    handle->batch.addresses     = addresses;
    handle->batch.structsMemory = structsMemory;
    handle->batch.currentStruct = REDGPU_32_STRUCT(Red2Struct, 0);
    handle->batch.currentProcedureParameters = NULL;
  }

  np(redCallsSet,
    "context", vkfast->context,
    "gpu", vkfast->gpu,
    "calls", handle->batch.calls.handle,
    "callsMemory", handle->batch.calls.memory,
    "callsReusable", handle->batch.calls.reusable,
    "outStatuses", NULL,
    "optionalFile", optionalFile,
    "optionalLine", optionalLine,
    "optionalUserData", NULL
  );

  if (handle->batch.structsMemory != NULL) {
    np(redStructsMemoryReset,
      "context", vkfast->context,
      "gpu", vkfast->gpu,
      "structsMemory", handle->batch.structsMemory,
      "outStatuses", NULL,
      "optionalFile", optionalFile,
      "optionalLine", optionalLine,
      "optionalUserData", NULL
    );

    // NOTE(Constantine): Oh God, I hope this call doesn't copy descriptors from structsMemory like redCallSetProcedureParametersStructs() :D
    np(redCallSetStructsMemory,
      "address", handle->batch.addresses.redCallSetStructsMemory,
      "calls", handle->batch.calls.handle,
      "structsMemory", handle->batch.structsMemory,
      "structsMemorySamplers", NULL
    );
  }

  return (uint64_t)(void *)handle;
}

GPU_API_PRE void GPU_API_POST vfBatchStorageCopyFromCpuToGpu(gpu_handle_context_t context, uint64_t batch_id, uint64_t from_cpu_storage_id, uint64_t to_gpu_storage_id, const char * optionalFile, int optionalLine) {
  vf_handle_t * batch = (vf_handle_t *)(void *)batch_id;
  vf_handle_context_t * vkfast = batch->vkfast;
  RedHandleGpu gpu = vkfast->gpu;
  REDGPU_2_EXPECTWG(batch->handle_id == VF_HANDLE_ID_BATCH);
  
  vf_handle_t * from_cpu_storage = (vf_handle_t *)(void *)from_cpu_storage_id;
  REDGPU_2_EXPECTWG(from_cpu_storage->handle_id == VF_HANDLE_ID_STORAGE);

  vf_handle_t * to_gpu_storage = (vf_handle_t *)(void *)to_gpu_storage_id;
  REDGPU_2_EXPECTWG(to_gpu_storage->handle_id == VF_HANDLE_ID_STORAGE);

  RedCopyArrayRange range = {0};
  range.arrayRBytesFirst  = from_cpu_storage->storage.arrayRangeInfo.arrayRangeBytesFirst;
  range.arrayWBytesFirst  = to_gpu_storage->storage.arrayRangeInfo.arrayRangeBytesFirst;
  range.bytesCount        = from_cpu_storage->storage.info.bytes_count;
  npfp(redCallCopyArrayToArray, batch->batch.addresses.redCallCopyArrayToArray,
    "calls", batch->batch.calls.handle,
    "arrayR", from_cpu_storage->storage.arrayRangeInfo.array,
    "arrayW", to_gpu_storage->storage.arrayRangeInfo.array,
    "rangesCount", 1,
    "ranges", &range
  );
}

GPU_API_PRE void GPU_API_POST vfBatchStorageCopyFromGpuToCpu(gpu_handle_context_t context, uint64_t batch_id, uint64_t from_gpu_storage_id, uint64_t to_cpu_storage_id, const char * optionalFile, int optionalLine) {
  vf_handle_t * batch = (vf_handle_t *)(void *)batch_id;
  vf_handle_context_t * vkfast = batch->vkfast;
  RedHandleGpu gpu = vkfast->gpu;
  REDGPU_2_EXPECTWG(batch->handle_id == VF_HANDLE_ID_BATCH);
  
  vf_handle_t * from_gpu_storage = (vf_handle_t *)(void *)from_gpu_storage_id;
  REDGPU_2_EXPECTWG(from_gpu_storage->handle_id == VF_HANDLE_ID_STORAGE);

  vf_handle_t * to_cpu_storage = (vf_handle_t *)(void *)to_cpu_storage_id;
  REDGPU_2_EXPECTWG(to_cpu_storage->handle_id == VF_HANDLE_ID_STORAGE);

  RedCopyArrayRange range = {0};
  range.arrayRBytesFirst  = from_gpu_storage->storage.arrayRangeInfo.arrayRangeBytesFirst;
  range.arrayWBytesFirst  = to_cpu_storage->storage.arrayRangeInfo.arrayRangeBytesFirst;
  range.bytesCount        = from_gpu_storage->storage.info.bytes_count;
  npfp(redCallCopyArrayToArray, batch->batch.addresses.redCallCopyArrayToArray,
    "calls", batch->batch.calls.handle,
    "arrayR", from_gpu_storage->storage.arrayRangeInfo.array,
    "arrayW", to_cpu_storage->storage.arrayRangeInfo.array,
    "rangesCount", 1,
    "ranges", &range
  );
}

GPU_API_PRE void GPU_API_POST vfBatchStorageCopyRaw(gpu_handle_context_t context, uint64_t batch_id, RedHandleArray from_storage_raw, RedHandleArray to_storage_raw, const RedCopyArrayRange * range, const char * optionalFile, int optionalLine) {
  vf_handle_t * batch = (vf_handle_t *)(void *)batch_id;
  vf_handle_context_t * vkfast = batch->vkfast;
  RedHandleGpu gpu = vkfast->gpu;
  REDGPU_2_EXPECTWG(batch->handle_id == VF_HANDLE_ID_BATCH);

  npfp(redCallCopyArrayToArray, batch->batch.addresses.redCallCopyArrayToArray,
    "calls", batch->batch.calls.handle,
    "arrayR", from_storage_raw,
    "arrayW", to_storage_raw,
    "rangesCount", 1,
    "ranges", range
  );
}

GPU_API_PRE void GPU_API_POST vfBatchBindProgramPipelineCompute(gpu_handle_context_t context, uint64_t batch_id, uint64_t program_pipeline_compute_id, const char * optionalFile, int optionalLine) {
  vf_handle_t * batch = (vf_handle_t *)(void *)batch_id;
  vf_handle_context_t * vkfast = batch->vkfast;
  RedHandleGpu gpu = vkfast->gpu;
  REDGPU_2_EXPECTWG(batch->handle_id == VF_HANDLE_ID_BATCH);

  vf_handle_t * program_pipeline_compute = (vf_handle_t *)(void *)program_pipeline_compute_id;
  REDGPU_2_EXPECTWG(program_pipeline_compute->handle_id == VF_HANDLE_ID_PROCEDURE);
  REDGPU_2_EXPECTWG(program_pipeline_compute->procedure.procedureType == VF_PROCEDURE_TYPE_COMPUTE);

  npfp(redCallSetProcedure, batch->batch.addresses.redCallSetProcedure,
    "calls", batch->batch.calls.handle,
    "procedureType", RED_PROCEDURE_TYPE_COMPUTE,
    "procedure", program_pipeline_compute->procedure.procedure
  );

  batch->batch.currentProcedureParameters = program_pipeline_compute->procedure.procedureParameters;
}

GPU_API_PRE void GPU_API_POST vfBatchBindNewBindingsSet(gpu_handle_context_t context, uint64_t batch_id, int slots_count, const RedStructDeclarationMember * slots, const char * optionalFile, int optionalLine) {
  vf_handle_t * batch = (vf_handle_t *)(void *)batch_id;
  vf_handle_context_t * vkfast = batch->vkfast;
  RedHandleGpu gpu = vkfast->gpu;
  REDGPU_2_EXPECTWG(batch->handle_id == VF_HANDLE_ID_BATCH);

  REDGPU_2_EXPECTWG(batch->batch.structsMemory != NULL || !"vfBatchBegin()::batch_bindings_info was likely set to NULL?");
  REDGPU_2_EXPECTWG(batch->batch.currentProcedureParameters != NULL || !"Was vfBatchBindProgramPipelineCompute() ever called previously?");

  Red2Struct structure = {0};
  np(red2StructsMemorySuballocateStruct,
    "context", vkfast->context,
    "gpu", vkfast->gpu,
    "handleName", NULL,
    "structsMemory", batch->batch.structsMemory,
    "structDeclarationMembersCount", slots_count,
    "structDeclarationMembers", slots,
    "structDeclarationMembersArrayROCount", 0,
    "structDeclarationMembersArrayRO", NULL,
    "outStruct", &structure,
    "outStatuses", NULL,
    "optionalFile", optionalFile,
    "optionalLine", optionalLine,
    "optionalUserData", NULL
  );
  batch->batch.currentStruct = structure;

  np(redCallSetProcedureParameters,
    "address", batch->batch.addresses.redCallSetProcedureParameters,
    "calls", batch->batch.calls.handle,
    "procedureType", RED_PROCEDURE_TYPE_COMPUTE,
    "procedureParameters", batch->batch.currentProcedureParameters
  );
}

GPU_API_PRE void GPU_API_POST vfBatchBindStorage(gpu_handle_context_t context, uint64_t batch_id, int slot, int storage_ids_count, const uint64_t * storage_ids, const char * optionalFile, int optionalLine) {
  vf_handle_t * batch = (vf_handle_t *)(void *)batch_id;
  vf_handle_context_t * vkfast = batch->vkfast;
  RedHandleGpu gpu = vkfast->gpu;
  REDGPU_2_EXPECTWG(batch->handle_id == VF_HANDLE_ID_BATCH);

  REDGPU_2_EXPECTWG(batch->batch.currentStruct.handle != NULL || !"Was vfBatchBindNewBindingsSet() ever called previously?");

  // To free
  RedStructMemberArray * arrays = (RedStructMemberArray *)red32MemoryCalloc(storage_ids_count * sizeof(RedStructMemberArray));
  REDGPU_2_EXPECTWG(arrays != NULL);

  for (int i = 0; i < storage_ids_count; i += 1) {
    vf_handle_t * storage = (vf_handle_t *)(void *)storage_ids[i];
    REDGPU_2_EXPECTWG(storage->handle_id == VF_HANDLE_ID_STORAGE);

    arrays[i] = storage->storage.arrayRangeInfo;
  }

  RedStructMember member = {0};
  member.setTo35   = 35;
  member.setTo0    = 0;
  member.structure = batch->batch.currentStruct.handle;
  member.slot      = slot;
  member.first     = 0;
  member.count     = storage_ids_count;
  member.type      = RED_STRUCT_MEMBER_TYPE_ARRAY_RO_RW;
  member.textures  = NULL;
  member.arrays    = arrays;
  member.setTo00   = 0;
  np(redStructsSet,
    "context", vkfast->context,
    "gpu", vkfast->gpu,
    "structsMembersCount", 1,
    "structsMembers", &member,
    "optionalFile", optionalFile,
    "optionalLine", optionalLine,
    "optionalUserData", NULL
  );

  red32MemoryFree(arrays);
  arrays = NULL;
}

GPU_API_PRE void GPU_API_POST vfBatchBindStorageRaw(gpu_handle_context_t context, uint64_t batch_id, int slot, int storage_raw_count, const RedStructMemberArray * storage_raw, const char * optionalFile, int optionalLine) {
  vf_handle_t * batch = (vf_handle_t *)(void *)batch_id;
  vf_handle_context_t * vkfast = batch->vkfast;
  RedHandleGpu gpu = vkfast->gpu;
  REDGPU_2_EXPECTWG(batch->handle_id == VF_HANDLE_ID_BATCH);

  REDGPU_2_EXPECTWG(batch->batch.currentStruct.handle != NULL || !"Was vfBatchBindNewBindingsSet() ever called previously?");

  RedStructMember member = {0};
  member.setTo35   = 35;
  member.setTo0    = 0;
  member.structure = batch->batch.currentStruct.handle;
  member.slot      = slot;
  member.first     = 0;
  member.count     = storage_raw_count;
  member.type      = RED_STRUCT_MEMBER_TYPE_ARRAY_RO_RW;
  member.textures  = NULL;
  member.arrays    = storage_raw;
  member.setTo00   = 0;
  np(redStructsSet,
    "context", vkfast->context,
    "gpu", vkfast->gpu,
    "structsMembersCount", 1,
    "structsMembers", &member,
    "optionalFile", optionalFile,
    "optionalLine", optionalLine,
    "optionalUserData", NULL
  );
}

GPU_API_PRE void GPU_API_POST vfBatchBindNewBindingsEnd(gpu_handle_context_t context, uint64_t batch_id, const char * optionalFile, int optionalLine) {
  vf_handle_t * batch = (vf_handle_t *)(void *)batch_id;
  vf_handle_context_t * vkfast = batch->vkfast;
  RedHandleGpu gpu = vkfast->gpu;
  REDGPU_2_EXPECTWG(batch->handle_id == VF_HANDLE_ID_BATCH);

  npfp(redCallSetProcedureParametersStructs, batch->batch.addresses.redCallSetProcedureParametersStructs,
    "calls", batch->batch.calls.handle,
    "procedureType", RED_PROCEDURE_TYPE_COMPUTE,
    "procedureParameters", batch->batch.currentProcedureParameters,
    "procedureParametersDeclarationStructsDeclarationsFirst", 0,
    "structsCount", 1, // NOTE(Constantine): Only one struct for now.
    "structs", &batch->batch.currentStruct.handle,
    "setTo0", 0,
    "setTo00", 0
  );

  np(red2DestroyHandle,
    "context", vkfast->context,
    "gpu", vkfast->gpu,
    "handleType", RED_HANDLE_TYPE_STRUCT_DECLARATION,
    "handle", batch->batch.currentStruct.handleDeclaration,
    "optionalHandle2", NULL,
    "optionalFile", optionalFile,
    "optionalLine", optionalLine,
    "optionalUserData", NULL
  );
  batch->batch.currentStruct.handleDeclaration = NULL;
  batch->batch.currentStruct.handle = NULL;
}

GPU_API_PRE void GPU_API_POST vfBatchBindVariablesCopy(gpu_handle_context_t context, uint64_t batch_id, unsigned variables_bytes_count, const void * variables, const char * optionalFile, int optionalLine) {
  vf_handle_t * batch = (vf_handle_t *)(void *)batch_id;
  vf_handle_context_t * vkfast = batch->vkfast;
  RedHandleGpu gpu = vkfast->gpu;
  REDGPU_2_EXPECTWG(batch->handle_id == VF_HANDLE_ID_BATCH);

  REDGPU_2_EXPECTWG(batch->batch.currentProcedureParameters != NULL || !"Was vfBatchBindProgramPipelineCompute() ever called previously?");

  npfp(redCallSetProcedureParametersVariables, batch->batch.addresses.redCallSetProcedureParametersVariables,
    "calls", batch->batch.calls.handle,
    "procedureParameters", batch->batch.currentProcedureParameters,
    "visibleToStages", RED_VISIBLE_TO_STAGE_BITFLAG_COMPUTE,
    "variablesBytesFirst", 0,
    "dataBytesCount", variables_bytes_count,
    "data", variables
  );
}

GPU_API_PRE void GPU_API_POST vfBatchCompute(gpu_handle_context_t context, uint64_t batch_id, unsigned workgroups_count_x, unsigned workgroups_count_y, unsigned workgroups_count_z, const char * optionalFile, int optionalLine) {
  vf_handle_t * batch = (vf_handle_t *)(void *)batch_id;
  vf_handle_context_t * vkfast = batch->vkfast;
  RedHandleGpu gpu = vkfast->gpu;
  REDGPU_2_EXPECTWG(batch->handle_id == VF_HANDLE_ID_BATCH);

  npfp(redCallProcedureCompute, batch->batch.addresses.redCallProcedureCompute,
    "calls", batch->batch.calls.handle,
    "workgroupsCountX", workgroups_count_x,
    "workgroupsCountY", workgroups_count_y,
    "workgroupsCountZ", workgroups_count_z
  );
}

GPU_API_PRE void GPU_API_POST vfBatchBarrierMemory(gpu_handle_context_t context, uint64_t batch_id, const char * optionalFile, int optionalLine) {
  vf_handle_t * batch = (vf_handle_t *)(void *)batch_id;
  vf_handle_context_t * vkfast = batch->vkfast;
  RedHandleGpu gpu = vkfast->gpu;
  REDGPU_2_EXPECTWG(batch->handle_id == VF_HANDLE_ID_BATCH);

  np(red2CallGlobalOrderBarrier,
    "address", batch->batch.addresses.redCallUsageAliasOrderBarrier,
    "calls", batch->batch.calls.handle
  );
}

GPU_API_PRE void GPU_API_POST vfBatchBarrierCpuReadback(gpu_handle_context_t context, uint64_t batch_id, const char * optionalFile, int optionalLine) {
  vf_handle_t * batch = (vf_handle_t *)(void *)batch_id;
  vf_handle_context_t * vkfast = batch->vkfast;
  RedHandleGpu gpu = vkfast->gpu;
  REDGPU_2_EXPECTWG(batch->handle_id == VF_HANDLE_ID_BATCH);

  np(red2CallGlobalReadbackBarrier,
    "address", batch->batch.addresses.redCallUsageAliasOrderBarrier,
    "calls", batch->batch.calls.handle
  );
}

GPU_API_PRE void GPU_API_POST vfBatchEnd(gpu_handle_context_t context, uint64_t batch_id, const char * optionalFile, int optionalLine) {
  vf_handle_t * batch = (vf_handle_t *)(void *)batch_id;
  vf_handle_context_t * vkfast = batch->vkfast;
  RedHandleGpu gpu = vkfast->gpu;
  REDGPU_2_EXPECTWG(batch->handle_id == VF_HANDLE_ID_BATCH);

  np(redCallsEnd,
    "context", vkfast->context,
    "gpu", vkfast->gpu,
    "calls", batch->batch.calls.handle,
    "callsMemory", batch->batch.calls.memory,
    "outStatuses", NULL,
    "optionalFile", optionalFile,
    "optionalLine", optionalLine,
    "optionalUserData", NULL
  );

  batch->batch.currentStruct.handle = NULL;
  batch->batch.currentStruct.handleDeclaration = NULL;
  batch->batch.currentProcedureParameters = NULL;
}

GPU_API_PRE void GPU_API_POST vfBatchGetRaw(gpu_handle_context_t context, uint64_t batch_id, RedCalls * out_batch_raw, const char * optionalFile, int optionalLine) {
  vf_handle_t * batch = (vf_handle_t *)(void *)batch_id;
  vf_handle_context_t * vkfast = batch->vkfast;
  RedHandleGpu gpu = vkfast->gpu;
  REDGPU_2_EXPECTWG(batch->handle_id == VF_HANDLE_ID_BATCH);

  out_batch_raw[0] = batch->batch.calls;
}

GPU_API_PRE uint64_t GPU_API_POST vfAsyncBatchExecute(gpu_handle_context_t context, uint64_t batch_ids_count, const uint64_t * batch_ids, const char * optionalFile, int optionalLine) {
  if (batch_ids_count == 0) {
    return 0;
  }

  vf_handle_context_t * vkfast = (vf_handle_context_t *)(void *)context;

  RedHandleGpu gpu = vkfast->gpu;

  // To free
  RedHandleCalls * calls = (RedHandleCalls *)red32MemoryCalloc(batch_ids_count * sizeof(RedHandleCalls));
  REDGPU_2_EXPECTWG(calls != NULL);

  for (int i = 0; i < batch_ids_count; i += 1) {
    vf_handle_t * batch = (vf_handle_t *)(void *)batch_ids[i];
    REDGPU_2_EXPECTWG(batch->handle_id == VF_HANDLE_ID_BATCH);

    calls[i] = batch->batch.calls.handle;
  }

  // To destroy
  RedHandleCpuSignal cpuSignal = NULL;
  np(redCreateCpuSignal,
    "context", vkfast->context,
    "gpu", vkfast->gpu,
    "handleName", NULL,
    "createSignaled", 0,
    "outCpuSignal", &cpuSignal,
    "outStatuses", NULL,
    "optionalFile", optionalFile,
    "optionalLine", optionalLine,
    "optionalUserData", NULL
  );

  RedGpuTimeline timelines[1] = {0};
  timelines[0].setTo4                            = 4;
  timelines[0].setTo0                            = 0;
  timelines[0].waitForAndUnsignalGpuSignalsCount = 0;
  timelines[0].waitForAndUnsignalGpuSignals      = NULL;
  timelines[0].setTo65536                        = NULL;
  timelines[0].callsCount                        = batch_ids_count;
  timelines[0].calls                             = calls;
  timelines[0].signalGpuSignalsCount             = 0;
  timelines[0].signalGpuSignals                  = NULL;
  np(redQueueSubmit,
    "context", vkfast->context,
    "gpu", vkfast->gpu,
    "queue", vkfast->mainQueue,
    "timelinesCount", 1,
    "timelines", timelines,
    "signalCpuSignal", cpuSignal,
    "outStatuses", NULL,
    "optionalFile", optionalFile,
    "optionalLine", optionalLine,
    "optionalUserData", NULL
  );

  red32MemoryFree(calls);
  calls = NULL;

  return (uint64_t)(void *)cpuSignal;
}

GPU_API_PRE void GPU_API_POST vfAsyncWaitToFinish(gpu_handle_context_t context, uint64_t async_id, const char * optionalFile, int optionalLine) {
  if (async_id == 0) {
    return;
  }

  vf_handle_context_t * vkfast = (vf_handle_context_t *)(void *)context;

  RedHandleCpuSignal cpuSignal = (RedHandleCpuSignal)(void *)async_id;

  np(redCpuSignalWait,
    "context", vkfast->context,
    "gpu", vkfast->gpu,
    "cpuSignalsCount", 1,
    "cpuSignals", &cpuSignal,
    "waitAll", 1,
    "outStatuses", NULL,
    "optionalFile", optionalFile,
    "optionalLine", optionalLine,
    "optionalUserData", NULL
  );

  np(red2DestroyHandle,
    "context", vkfast->context,
    "gpu", vkfast->gpu,
    "handleType", RED_HANDLE_TYPE_CPU_SIGNAL,
    "handle", cpuSignal,
    "optionalHandle2", NULL,
    "optionalFile", optionalFile,
    "optionalLine", optionalLine,
    "optionalUserData", NULL
  );
}

static int vfInternalAsyncDrawPixels(gpu_handle_context_t context, uint64_t pixels_storage_id, const void * copy_pixels, const char * optionalFile, int optionalLine) {
  vf_handle_context_t * vkfast = (vf_handle_context_t *)(void *)context;

  RedHandleGpu gpu = vkfast->gpu;

  vf_handle_t * pixels_storage = (vf_handle_t *)(void *)pixels_storage_id;
  REDGPU_2_EXPECTWG(pixels_storage->handle_id == VF_HANDLE_ID_STORAGE);

  unsigned presentImageIndex = 0;
  RedStatuses presentGetImageIndexStatuses = {0};
  np(redPresentGetImageIndex,
    "context", vkfast->context,
    "gpu", vkfast->gpu,
    "present", vkfast->present,
    "signalCpuSignal", NULL,
    "signalGpuSignal", vkfast->presentGpuSignal,
    "outImageIndex", &presentImageIndex,
    "outStatuses", &presentGetImageIndexStatuses,
    "optionalFile", optionalFile,
    "optionalLine", optionalLine,
    "optionalUserData", NULL
  );
  REDGPU_2_EXPECTWG(presentGetImageIndexStatuses.status == RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(presentGetImageIndexStatuses.statusError == RED_STATUS_SUCCESS || presentGetImageIndexStatuses.statusError == RED_STATUS_ERROR_PRESENT_IS_OUT_OF_DATE);
  if (presentGetImageIndexStatuses.statusError == RED_STATUS_ERROR_PRESENT_IS_OUT_OF_DATE) {
    return vfInternalRebuildPresent(context, optionalFile, optionalLine);
  }

  if (copy_pixels != NULL) {
    // NOTE(Constantine): The reason we copy pixels here is because vkfast->screenWidth/Height were updated in a potential vfInternalRebuildPresent call above.
    red32MemoryCopy(vkfast->presentPixelsStorageCpuUpload.mapped_void_ptr, copy_pixels, sizeof(unsigned char) * 4 * vkfast->screenHeight * vkfast->screenWidth);
  }

  {
    RedCallProceduresAndAddresses addresses = {0};
    np(redGetCallProceduresAndAddresses,
      "context", vkfast->context,
      "gpu", vkfast->gpu,
      "outCallProceduresAndAddresses", &addresses,
      "outStatuses", NULL,
      "optionalFile", optionalFile,
      "optionalLine", optionalLine,
      "optionalUserData", NULL
    );

    np(redCallsSet,
      "context", vkfast->context,
      "gpu", vkfast->gpu,
      "calls", vkfast->presentCopyCalls.handle,
      "callsMemory", vkfast->presentCopyCalls.memory,
      "callsReusable", vkfast->presentCopyCalls.reusable,
      "outStatuses", NULL,
      "optionalFile", optionalFile,
      "optionalLine", optionalLine,
      "optionalUserData", NULL
    );

    {
      RedUsageImage imageUsage = {0};
      imageUsage.barrierSplit           = RED_BARRIER_SPLIT_NONE;
      imageUsage.oldAccessStages        = 0;
      imageUsage.newAccessStages        = RED_ACCESS_STAGE_BITFLAG_COPY;
      imageUsage.oldAccess              = 0;
      imageUsage.newAccess              = RED_ACCESS_BITFLAG_COPY_W;
      imageUsage.oldState               = RED_STATE_UNUSABLE; // NOTE(Constantine): Optimization.
      imageUsage.newState               = RED_STATE_USABLE;
      imageUsage.queueFamilyIndexSource = -1;
      imageUsage.queueFamilyIndexTarget = -1;
      imageUsage.image                  = vkfast->presentImages[presentImageIndex];
      imageUsage.imageAllParts          = RED_IMAGE_PART_BITFLAG_COLOR;
      imageUsage.imageLevelsFirst       = 0;
      imageUsage.imageLevelsCount       = -1;
      imageUsage.imageLayersFirst       = 0;
      imageUsage.imageLayersCount       = -1;
      np(redCallUsageAliasOrderBarrier,
        "address", addresses.redCallUsageAliasOrderBarrier,
        "calls", vkfast->presentCopyCalls.handle,
        "context", vkfast->context,
        "arrayUsagesCount", 0,
        "arrayUsages", NULL,
        "imageUsagesCount", 1,
        "imageUsages", &imageUsage,
        "aliasesCount", 0,
        "aliases", NULL,
        "ordersCount", 0,
        "orders", NULL,
        "dependencyByRegion", 0
      );
    }

    {
      RedCopyArrayImageRange copy = {0};
      copy.arrayBytesFirst               = pixels_storage->storage.arrayRangeInfo.arrayRangeBytesFirst;
      copy.arrayTexelsCountToNextRow     = vkfast->screenWidth;
      copy.arrayTexelsCountToNextLayerOr3DDepthSliceDividedByTexelsCountToNextRow = 0;
      copy.imageParts.allParts           = RED_IMAGE_PART_BITFLAG_COLOR;
      copy.imageParts.level              = 0;
      copy.imageParts.layersFirst        = 0;
      copy.imageParts.layersCount        = 1;
      copy.imageOffset.texelX            = 0;
      copy.imageOffset.texelY            = 0;
      copy.imageOffset.texelZ            = 0;
      copy.imageExtent.texelsCountWidth  = vkfast->screenWidth;
      copy.imageExtent.texelsCountHeight = vkfast->screenHeight;
      copy.imageExtent.texelsCountDepth  = 1;
      npfp(redCallCopyArrayToImage, addresses.redCallCopyArrayToImage,
        "calls", vkfast->presentCopyCalls.handle,
        "arrayR", pixels_storage->storage.arrayRangeInfo.array,
        "imageW", vkfast->presentImages[presentImageIndex],
        "setTo1", 1,
        "rangesCount", 1,
        "ranges", &copy
      );
    }

    np(red2CallGlobalOrderBarrier,
      "address", addresses.redCallUsageAliasOrderBarrier,
      "calls", vkfast->presentCopyCalls.handle
    );

    {
      RedUsageImage imageUsage = {0};
      imageUsage.barrierSplit           = RED_BARRIER_SPLIT_NONE;
      imageUsage.oldAccessStages        = RED_ACCESS_STAGE_BITFLAG_COPY;
      imageUsage.newAccessStages        = 0;
      imageUsage.oldAccess              = RED_ACCESS_BITFLAG_COPY_W;
      imageUsage.newAccess              = 0;
      imageUsage.oldState               = RED_STATE_USABLE;
      imageUsage.newState               = RED_STATE_PRESENT;
      imageUsage.queueFamilyIndexSource = -1;
      imageUsage.queueFamilyIndexTarget = -1;
      imageUsage.image                  = vkfast->presentImages[presentImageIndex];
      imageUsage.imageAllParts          = RED_IMAGE_PART_BITFLAG_COLOR;
      imageUsage.imageLevelsFirst       = 0;
      imageUsage.imageLevelsCount       = -1;
      imageUsage.imageLayersFirst       = 0;
      imageUsage.imageLayersCount       = -1;
      np(redCallUsageAliasOrderBarrier,
        "address", addresses.redCallUsageAliasOrderBarrier,
        "calls", vkfast->presentCopyCalls.handle,
        "context", vkfast->context,
        "arrayUsagesCount", 0,
        "arrayUsages", NULL,
        "imageUsagesCount", 1,
        "imageUsages", &imageUsage,
        "aliasesCount", 0,
        "aliases", NULL,
        "ordersCount", 0,
        "orders", NULL,
        "dependencyByRegion", 0
      );
    }

    np(redCallsEnd,
      "context", vkfast->context,
      "gpu", vkfast->gpu,
      "calls", vkfast->presentCopyCalls.handle,
      "callsMemory", vkfast->presentCopyCalls.memory,
      "outStatuses", NULL,
      "optionalFile", optionalFile,
      "optionalLine", optionalLine,
      "optionalUserData", NULL
    );
  }

  {
    unsigned arrayOf65536[1] = {65536};
    RedGpuTimeline timelines[1] = {0};
    timelines[0].setTo4                            = 4;
    timelines[0].setTo0                            = 0;
    timelines[0].waitForAndUnsignalGpuSignalsCount = 1;
    timelines[0].waitForAndUnsignalGpuSignals      = &vkfast->presentGpuSignal;
    timelines[0].setTo65536                        = arrayOf65536;
    timelines[0].callsCount                        = 1;
    timelines[0].calls                             = &vkfast->presentCopyCalls.handle;
    timelines[0].signalGpuSignalsCount             = 1;
    timelines[0].signalGpuSignals                  = &vkfast->presentGpuSignal;
    np(redQueueSubmit,
      "context", vkfast->context,
      "gpu", vkfast->gpu,
      "queue", vkfast->gpuInfo->queues[vkfast->presentQueueIndex],
      "timelinesCount", 1,
      "timelines", timelines,
      "signalCpuSignal", NULL,
      "outStatuses", NULL,
      "optionalFile", optionalFile,
      "optionalLine", optionalLine,
      "optionalUserData", NULL
    );
  }

  RedStatus queuePresentStatus = RED_STATUS_SUCCESS;
  RedStatuses queuePresentStatuses = {0};
  np(redQueuePresent,
    "context", vkfast->context,
    "gpu", vkfast->gpu,
    "queue", vkfast->gpuInfo->queues[vkfast->presentQueueIndex],
    "waitForAndUnsignalGpuSignalsCount", 1,
    "waitForAndUnsignalGpuSignals", &vkfast->presentGpuSignal,
    "presentsCount", 1,
    "presents", &vkfast->present,
    "presentsImageIndex", &presentImageIndex,
    "outPresentsStatus", &queuePresentStatus,
    "outStatuses", &queuePresentStatuses,
    "optionalFile", optionalFile,
    "optionalLine", optionalLine,
    "optionalUserData", NULL
  );
  REDGPU_2_EXPECTWG(queuePresentStatus == RED_STATUS_SUCCESS || queuePresentStatus == RED_STATUS_ERROR_PRESENT_IS_OUT_OF_DATE);
  REDGPU_2_EXPECTWG(queuePresentStatuses.status == RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(queuePresentStatuses.statusError == RED_STATUS_SUCCESS || queuePresentStatuses.statusError == RED_STATUS_ERROR_PRESENT_IS_OUT_OF_DATE);
  if (queuePresentStatus == RED_STATUS_ERROR_PRESENT_IS_OUT_OF_DATE || queuePresentStatuses.statusError == RED_STATUS_ERROR_PRESENT_IS_OUT_OF_DATE) {
    return vfInternalRebuildPresent(context, optionalFile, optionalLine);
  }

  return 0;
}

GPU_API_PRE int GPU_API_POST vfDrawPixels(gpu_handle_context_t context, const void * pixels, const char * optionalFile, int optionalLine) {
  vf_handle_context_t * vkfast = (vf_handle_context_t *)(void *)context;

  int isWindowMinimized = vfInternalAsyncDrawPixels(context, vkfast->presentPixelsStorageCpuUpload.id, pixels, optionalFile, optionalLine);
  vfAsyncDrawWaitToFinish(context, optionalFile, optionalLine);

  return isWindowMinimized;
}

GPU_API_PRE int GPU_API_POST vfAsyncDrawPixels(gpu_handle_context_t context, uint64_t pixels_storage_id, const char * optionalFile, int optionalLine) {
  return vfInternalAsyncDrawPixels(context, pixels_storage_id, NULL, optionalFile, optionalLine);
}

GPU_API_PRE void GPU_API_POST vfAsyncDrawWaitToFinish(gpu_handle_context_t context, const char * optionalFile, int optionalLine) {
  vf_handle_context_t * vkfast = (vf_handle_context_t *)(void *)context;

  RedHandleGpu gpu = vkfast->gpu;

  RedStatus queuePresentStatus = RED_STATUS_SUCCESS;
  RedStatuses queuePresentStatuses = {0};
  np(redQueuePresent,
    "context", vkfast->context,
    "gpu", vkfast->gpu,
    "queue", vkfast->gpuInfo->queues[vkfast->presentQueueIndex],
    "waitForAndUnsignalGpuSignalsCount", 0,
    "waitForAndUnsignalGpuSignals", NULL,
    "presentsCount", 0,
    "presents", NULL,
    "presentsImageIndex", NULL,
    "outPresentsStatus", &queuePresentStatus,
    "outStatuses", &queuePresentStatuses,
    "optionalFile", optionalFile,
    "optionalLine", optionalLine,
    "optionalUserData", NULL
  );
  REDGPU_2_EXPECTWG(queuePresentStatus == RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(queuePresentStatuses.status == RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(queuePresentStatuses.statusError == RED_STATUS_SUCCESS);
}

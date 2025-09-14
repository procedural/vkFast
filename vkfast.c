#ifdef _WIN32
#define GPU_API_PRE __declspec(dllexport)
#define GPU_API_POST
#endif

#include "vkfast.h"

#ifndef __cplusplus
#define REDGPU_DISABLE_NAMED_PARAMETERS
#endif
#include "C:/RedGpuSDK/misc/np/np.h"
#include "C:/RedGpuSDK/misc/np/np_redgpu.h"
#include "C:/RedGpuSDK/misc/np/np_redgpu_wsi.h"
#include "C:/RedGpuSDK/misc/np/np_redgpu_2.h"

#define VKFAST_INTERNAL_DEFAULT_MEMORY_ALLOCATION_SIZE_GPU_VRAM_ARRAYS_512MB (512 * 1024 * 1024)
#define VKFAST_INTERNAL_DEFAULT_MEMORY_ALLOCATION_SIZE_GPU_VRAM_IMAGES_512MB (512 * 1024 * 1024)
#define VKFAST_INTERNAL_DEFAULT_MEMORY_ALLOCATION_SIZE_CPU_VISIBLE_512MB     (512 * 1024 * 1024)
#define VKFAST_INTERNAL_DEFAULT_MEMORY_ALLOCATION_SIZE_CPU_READBACK_512MB    (512 * 1024 * 1024)

typedef struct vkfast_global_state_context_t {
  int                isDebugMode;

  void *             windowHandle;
  int                screenWidth;
  int                screenHeight;
  int                msaaSamples;
  
  RedContext         context;
  const RedGpuInfo * gpuInfo;

  RedHandleGpu       gpu;
  unsigned           mainQueueFamilyIndex;
  RedHandleQueue     mainQueue;

  unsigned           specificMemoryTypesGpuVram;
  unsigned           specificMemoryTypesCpuUpload;
  unsigned           specificMemoryTypesCpuReadback;

  // Memory

  Red2Memory         memoryGpuVramForArrays_memory;
  Red2Array          memoryGpuVramForArrays_array;
  uint64_t           memoryGpuVramForArrays_memory_suballocations_offset;
  
  Red2Memory         memoryGpuVramForImages_memory;
  uint64_t           memoryGpuVramForImages_memory_suballocations_offset;

  Red2Array          memoryCpuUpload_memory_and_array;
  void *             memoryCpuUpload_mapped_void_ptr;
  uint64_t           memoryCpuUpload_memory_suballocations_offset;

  Red2Array          memoryCpuReadback_memory_and_array;
  void *             memoryCpuReadback_mapped_void_ptr;
  uint64_t           memoryCpuReadback_memory_suballocations_offset;
} vkfast_global_state_context_t;

vkfast_global_state_context_t * g_vkfast; // NOTE(Constantine): Every time vkfast_global_state_context is accessed in vf* functions, that is not thread-safe.

typedef enum vkfast_handle_id_t {
  VF_HANDLE_ID_INVALID = 0,
} vkfast_handle_id_t;

typedef struct vkfast_handle_t {
  vkfast_handle_id_t vkfast_handle_id;
  union {
  };
} vkfast_handle_t;

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

static RedBool32 vfRedGpuDebugCallback(RedDebugCallbackSeverity severity, RedDebugCallbackTypeBitflags types, const RedDebugCallbackData * data, RedContext context) {
  vfInternalPrint(data->message);
  red32Exit(1);
  return 0;
}

GPU_API_PRE void GPU_API_POST vfContextInit(int enable_debug_mode, const gpu_context_optional_parameters_t * optional_parameters, const char * optionalFile, int optionalLine) {
  if (enable_debug_mode) {
    vfInternalPrint("[vkFast][Debug] In case of an error, email me (Constantine) at: iamvfx@gmail.com" "\n");
  }

  g_vkfast = (vkfast_global_state_context_t *)red32MemoryCalloc(sizeof(vkfast_global_state_context_t));
  REDGPU_2_EXPECT(g_vkfast != NULL);

  uint64_t internalMemoryAllocationSizeGpuVramArrays = VKFAST_INTERNAL_DEFAULT_MEMORY_ALLOCATION_SIZE_GPU_VRAM_ARRAYS_512MB;
  uint64_t internalMemoryAllocationSizeGpuVramImages = VKFAST_INTERNAL_DEFAULT_MEMORY_ALLOCATION_SIZE_GPU_VRAM_IMAGES_512MB;
  uint64_t internalMemoryAllocationSizeCpuVisible    = VKFAST_INTERNAL_DEFAULT_MEMORY_ALLOCATION_SIZE_CPU_VISIBLE_512MB;
  uint64_t internalMemoryAllocationSizeCpuReadback   = VKFAST_INTERNAL_DEFAULT_MEMORY_ALLOCATION_SIZE_CPU_READBACK_512MB;
  if (optional_parameters != NULL) {
    if (optional_parameters->internal_memory_allocation_sizes != NULL) {
      internalMemoryAllocationSizeGpuVramArrays = optional_parameters->internal_memory_allocation_sizes->bytesCountForMemoryGpuVramArrays;
      internalMemoryAllocationSizeGpuVramImages = optional_parameters->internal_memory_allocation_sizes->bytesCountForMemoryGpuVramImages;
      internalMemoryAllocationSizeCpuVisible    = optional_parameters->internal_memory_allocation_sizes->bytesCountForMemoryCpuVisible;
      internalMemoryAllocationSizeCpuReadback   = optional_parameters->internal_memory_allocation_sizes->bytesCountForMemoryCpuReadback;
    }
  }

  RedContext context = NULL;
  np(redCreateContext,
    "malloc", red32MemoryCalloc,
    "free", red32MemoryFree,
    "optionalMallocTagged", NULL,
    "optionalFreeTagged", NULL,
    "debugCallback", enable_debug_mode == 1 ? vfRedGpuDebugCallback : NULL,
    "sdkVersion", RED_SDK_VERSION_1_0_135,
    "sdkExtensionsCount", 0,
    "sdkExtensions", NULL,
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

  REDGPU_2_EXPECT(context != NULL);
  REDGPU_2_EXPECT(context->gpusCount > 0);

  const RedGpuInfo * gpuInfo = &context->gpus[0]; // NOTE(Constantine): Always picking the first available GPU.

  if (enable_debug_mode == 1) {
    vfInternalPrint("[vkFast][Debug] Your GPU name: ");
    vfInternalPrint(gpuInfo->gpuName);
    vfInternalPrint("\n");
    vfInternalPrint("[vkFast][Debug] For extra debug information, recompile redgpu.c with REDGPU_COMPILE_SWITCH 3" "\n");
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
    REDGPU_2_EXPECT(!"Unsupported by vkFast GPU, recompile your program with vfWindowFullscreen()::enable_debug_mode parameter enabled and email me your GPU name please: iamvfx@gmail.com");
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
  unsigned       mainQueueFamilyIndex = gpuInfo->queuesFamilyIndex[0];
  RedHandleQueue mainQueue            = gpuInfo->queues[0];

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

    } else if (gpuInfo->gpuVendorId == 32902/*Intel UHD Graphics 730*/) {
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
      REDGPU_2_EXPECTWG(!"Unsupported by vkFast GPU, recompile your program with vfWindowFullscreen()::enable_debug_mode parameter enabled and email me your GPU name please: iamvfx@gmail.com");
    }
  }

  Red2Memory memoryGpuVramForArrays_memory      = {0};
  Red2Array  memoryGpuVramForArrays_array       = {0};
  Red2Memory memoryGpuVramForImages_memory      = {0};
  Red2Array  memoryCpuUpload_memory_and_array   = {0};
  void *     memoryCpuUpload_mapped_void_ptr    = NULL;
  Red2Array  memoryCpuReadback_memory_and_array = {0};
  void *     memoryCpuReadback_mapped_void_ptr  = NULL;
  {
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
    Red2Memory * memorys[] = {&memoryGpuVramForArrays_memory};
    np(red2CreateArray,
      "context", context,
      "gpu", gpu,
      "handleName", "vkFast_memoryGpuVramForArrays_array",
      "type", RED_ARRAY_TYPE_ARRAY_RW,
      "bytesCount", internalMemoryAllocationSizeGpuVramArrays,
      "structuredBufferElementBytesCount", 0,
      "initialAccess", 0,
      "initialQueueFamilyIndex", -1,
      "maxAllowedOverallocationBytesCount", 0,
      "dedicate", 0,
      "mappable", 0,
      "dedicateOrMappableMemoryTypeIndex", 0,
      "dedicateOrMappableMemoryBitflags", 0,
      "suballocateFromMemoryOnFirstMatchPointersCount", _countof(memorys),
      "suballocateFromMemoryOnFirstMatchPointers", memorys,
      "outArray", &memoryGpuVramForArrays_array,
      "outStatuses", NULL,
      "optionalFile", optionalFile,
      "optionalLine", optionalLine,
      "optionalUserData", NULL
    );
  }
  {
    np(red2MemoryAllocate,
      "context", context,
      "gpu", gpu,
      "handleName", "vkFast_memoryGpuVramForImages_memory",
      "bytesCount", internalMemoryAllocationSizeGpuVramImages,
      "memoryTypeIndex", specificMemoryTypesGpuVram,
      "memoryBitflags", 0,
      "outMemory", &memoryGpuVramForImages_memory,
      "outStatuses", NULL,
      "optionalFile", optionalFile,
      "optionalLine", optionalLine,
      "optionalUserData", NULL
    );
  }
  {
    np(red2CreateArray,
      "context", context,
      "gpu", gpu,
      "handleName", "vkFast_memoryCpuUpload_memory_and_array",
      "type", RED_ARRAY_TYPE_ARRAY_RO,
      "bytesCount", internalMemoryAllocationSizeCpuVisible,
      "structuredBufferElementBytesCount", 0,
      "initialAccess", RED_ACCESS_BITFLAG_COPY_R,
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
  }
  {
    np(red2CreateArray,
      "context", context,
      "gpu", gpu,
      "handleName", "vkFast_memoryCpuReadback_memory_and_array",
      "type", RED_ARRAY_TYPE_ARRAY_RW,
      "bytesCount", internalMemoryAllocationSizeCpuReadback,
      "structuredBufferElementBytesCount", 0,
      "initialAccess", RED_ACCESS_BITFLAG_COPY_W,
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
  }

  // Filling
  vkfast_global_state_context_t;
  g_vkfast->isDebugMode = enable_debug_mode;
  g_vkfast->windowHandle = NULL;
  g_vkfast->screenWidth = 0;
  g_vkfast->screenHeight = 0;
  g_vkfast->msaaSamples = 0;
  g_vkfast->context = context;
  g_vkfast->gpuInfo = gpuInfo;
  g_vkfast->gpu = gpu;
  g_vkfast->mainQueueFamilyIndex = mainQueueFamilyIndex;
  g_vkfast->mainQueue = mainQueue;
  g_vkfast->specificMemoryTypesGpuVram = specificMemoryTypesGpuVram;
  g_vkfast->specificMemoryTypesCpuUpload = specificMemoryTypesCpuUpload;
  g_vkfast->specificMemoryTypesCpuReadback = specificMemoryTypesCpuReadback;
  g_vkfast->memoryGpuVramForArrays_memory = memoryGpuVramForArrays_memory;
  g_vkfast->memoryGpuVramForArrays_array = memoryGpuVramForArrays_array;
  g_vkfast->memoryGpuVramForArrays_memory_suballocations_offset = 0;
  g_vkfast->memoryGpuVramForImages_memory = memoryGpuVramForImages_memory;
  g_vkfast->memoryGpuVramForImages_memory_suballocations_offset = 0;
  g_vkfast->memoryCpuUpload_memory_and_array = memoryCpuUpload_memory_and_array;
  g_vkfast->memoryCpuUpload_mapped_void_ptr = memoryCpuUpload_mapped_void_ptr;
  g_vkfast->memoryCpuUpload_memory_suballocations_offset = 0;
  g_vkfast->memoryCpuReadback_memory_and_array = memoryCpuReadback_memory_and_array;
  g_vkfast->memoryCpuReadback_mapped_void_ptr = memoryCpuReadback_mapped_void_ptr;
  g_vkfast->memoryCpuReadback_memory_suballocations_offset = 0;
}

GPU_API_PRE void GPU_API_POST vfContextDeinit(uint64_t ids_count, const uint64_t * ids, const char * optionalFile, int optionalLine) {
  for (uint64_t i = 0; i < ids_count; i += 1) {
    vkfast_handle_t * handle = (vkfast_handle_t *)(void *)ids[i];
    if (handle->vkfast_handle_id == VF_HANDLE_ID_INVALID) {
      continue;
    }
  }

  np(red2DestroyHandle,
    "context", g_vkfast->context,
    "gpu", g_vkfast->gpu,
    "handleType", RED_HANDLE_TYPE_ARRAY,
    "handle", g_vkfast->memoryGpuVramForArrays_array.array.handle,
    "optionalHandle2", NULL,
    "optionalFile", optionalFile,
    "optionalLine", optionalLine,
    "optionalUserData", NULL
  );
  np(red2DestroyHandle,
    "context", g_vkfast->context,
    "gpu", g_vkfast->gpu,
    "handleType", RED_HANDLE_TYPE_MEMORY,
    "handle", g_vkfast->memoryGpuVramForArrays_memory.handle,
    "optionalHandle2", NULL,
    "optionalFile", optionalFile,
    "optionalLine", optionalLine,
    "optionalUserData", NULL
  );
  
  np(red2DestroyHandle,
    "context", g_vkfast->context,
    "gpu", g_vkfast->gpu,
    "handleType", RED_HANDLE_TYPE_MEMORY,
    "handle", g_vkfast->memoryGpuVramForImages_memory.handle,
    "optionalHandle2", NULL,
    "optionalFile", optionalFile,
    "optionalLine", optionalLine,
    "optionalUserData", NULL
  );

  np(redMemoryUnmap,
    "context", g_vkfast->context,
    "gpu", g_vkfast->gpu,
    "mappableMemory", g_vkfast->memoryCpuUpload_memory_and_array.handleAllocatedDedicatedOrMappableMemoryOrPickedMemory,
    "optionalFile", optionalFile,
    "optionalLine", optionalLine,
    "optionalUserData", NULL
  );
  np(red2DestroyHandle,
    "context", g_vkfast->context,
    "gpu", g_vkfast->gpu,
    "handleType", RED_HANDLE_TYPE_ARRAY,
    "handle", g_vkfast->memoryCpuUpload_memory_and_array.array.handle,
    "optionalHandle2", NULL,
    "optionalFile", optionalFile,
    "optionalLine", optionalLine,
    "optionalUserData", NULL
  );
  np(red2DestroyHandle,
    "context", g_vkfast->context,
    "gpu", g_vkfast->gpu,
    "handleType", RED_HANDLE_TYPE_MEMORY,
    "handle", g_vkfast->memoryCpuUpload_memory_and_array.handleAllocatedDedicatedOrMappableMemoryOrPickedMemory,
    "optionalHandle2", NULL,
    "optionalFile", optionalFile,
    "optionalLine", optionalLine,
    "optionalUserData", NULL
  );

  np(redMemoryUnmap,
    "context", g_vkfast->context,
    "gpu", g_vkfast->gpu,
    "mappableMemory", g_vkfast->memoryCpuReadback_memory_and_array.handleAllocatedDedicatedOrMappableMemoryOrPickedMemory,
    "optionalFile", optionalFile,
    "optionalLine", optionalLine,
    "optionalUserData", NULL
  );
  np(red2DestroyHandle,
    "context", g_vkfast->context,
    "gpu", g_vkfast->gpu,
    "handleType", RED_HANDLE_TYPE_ARRAY,
    "handle", g_vkfast->memoryCpuReadback_memory_and_array.array.handle,
    "optionalHandle2", NULL,
    "optionalFile", optionalFile,
    "optionalLine", optionalLine,
    "optionalUserData", NULL
  );
  np(red2DestroyHandle,
    "context", g_vkfast->context,
    "gpu", g_vkfast->gpu,
    "handleType", RED_HANDLE_TYPE_MEMORY,
    "handle", g_vkfast->memoryCpuReadback_memory_and_array.handleAllocatedDedicatedOrMappableMemoryOrPickedMemory,
    "optionalHandle2", NULL,
    "optionalFile", optionalFile,
    "optionalLine", optionalLine,
    "optionalUserData", NULL
  );

  np(redDestroyContext,
    "context", g_vkfast->context,
    "optionalFile", optionalFile,
    "optionalLine", optionalLine,
    "optionalUserData", NULL
  );

  for (uint64_t i = 0; i < ids_count; i += 1) {
    vkfast_handle_t * handle = (vkfast_handle_t *)(void *)ids[i];
    red32MemoryFree(handle);
  }
}

GPU_API_PRE void GPU_API_POST vfWindowFullscreen(void * optional_existing_window_handle, const char * window_title, int screen_width, int screen_height, int msaa_samples, const char * optionalFile, int optionalLine) {
  REDGPU_2_EXPECT(msaa_samples == 1 || !"TODO");

  void * window_handle = optional_existing_window_handle;
  if (window_handle == NULL) {
    window_handle = red32WindowCreate(window_title);
  }
  g_vkfast->windowHandle = window_handle;
  g_vkfast->screenWidth = screen_width;
  g_vkfast->screenHeight = screen_height;
  g_vkfast->msaaSamples = msaa_samples;

  // TODO: if already have redgpu window resources, destroy them and create them again here.
}

GPU_API_PRE int GPU_API_POST vfWindowLoop() {
  return red32WindowLoop();
}

GPU_API_PRE void GPU_API_POST vfExit(int exit_code) {
  red32Exit(exit_code);
}

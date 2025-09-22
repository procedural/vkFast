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

#ifndef _countof
#define _countof(x) (sizeof(x) / sizeof((x)[0]))
#endif

typedef struct vf_global_state_context_t {
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
} vf_global_state_context_t;

vf_global_state_context_t * g_vkfast; // NOTE(Constantine): Every time vf_global_state_context_t values are modified in a vf* function, that function is not thread-safe.

typedef struct vf_handle_storage_t {
  gpu_storage_info_t   info;
  RedStructMemberArray arrayRangeInfo; // NOTE(Constantine): Kept for GPU copy calls.
} vf_handle_storage_t;

typedef struct vf_handle_texture_t {
  gpu_texture_info_t info;
  Red2Image          image;   // NOTE(Constantine): Kept to be destroyed.
  RedHandleTexture   texture; // NOTE(Constantine): Kept to be destroyed.
} vf_handle_texture_t;

typedef struct vf_handle_sampler_t {
  gpu_sampler_info_t info;
  RedHandleSampler   sampler; // NOTE(Constantine): Kept to be destroyed.
} vf_handle_sampler_t;

typedef enum vf_gpu_code_type_t {
  VF_GPU_CODE_TYPE_INVALID  = 0,
  VF_GPU_CODE_TYPE_VERTEX   = 1,
  VF_GPU_CODE_TYPE_FRAGMENT = 2,
  VF_GPU_CODE_TYPE_COMPUTE  = 3,
} vf_gpu_code_type_t;

typedef struct vf_handle_gpu_code_t {
  gpu_program_info_t info;        // NOTE(Constantine): Program binary is a stale pointer, do not use.
  vf_gpu_code_type_t gpuCodeType;
  RedHandleGpuCode   gpuCode;     // NOTE(Constantine): Kept to be destroyed.
} vf_handle_gpu_code_t;

typedef enum vf_procedure_type_t {
  VF_PROCEDURE_TYPE_INVALID = 0,
  VF_PROCEDURE_TYPE_DRAW    = 1,
  VF_PROCEDURE_TYPE_COMPUTE = 2,
} vf_procedure_type_t;

typedef struct vf_handle_procedure_t {
  union {
    gpu_program_pipeline_info_t      infoGraphics;
    gpu_program_pipeline_comp_info_t infoCompute;
  };
  vf_procedure_type_t          procedureType;
  RedHandleProcedureParameters procedureParameters;
  RedHandleProcedure           procedure;
} vf_handle_procedure_t;

typedef struct vf_handle_batch_t {
  RedCalls                      calls;
  RedCallProceduresAndAddresses addresses;
  RedHandleStructsMemory        structsMemory;
  Red2Struct                    currentStruct;              // NOTE(Constantine): Only one struct for now.
  RedHandleProcedureParameters  currentProcedureParameters; // NOTE(Constantine): Same parameters for both graphics and compute procedures.
} vf_handle_batch_t;

typedef enum vf_handle_id_t {
  VF_HANDLE_ID_INVALID   = 0,
  VF_HANDLE_ID_STORAGE   = 1,
  VF_HANDLE_ID_TEXTURE   = 2,
  VF_HANDLE_ID_SAMPLER   = 3,
  VF_HANDLE_ID_GPU_CODE  = 4,
  VF_HANDLE_ID_PROCEDURE = 5,
  VF_HANDLE_ID_BATCH     = 6,
} vf_handle_id_t;

typedef struct vf_handle_t {
  vf_handle_id_t handle_id;
  union {
    vf_handle_storage_t   storage;
    vf_handle_texture_t   texture;
    vf_handle_sampler_t   sampler;
    vf_handle_gpu_code_t  gpuCode;
    vf_handle_procedure_t procedure;
    vf_handle_batch_t     batch;
  };
} vf_handle_t;

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

  red32ConsolePrintError(str.items);

  red32MemoryFree(optionalLineStr);
  REDGPU_32_DYNAMIC_ARRAY_FREE(str);

  red32Exit(1);
}

static RedBool32 vfRedGpuDebugCallback(RedDebugCallbackSeverity severity, RedDebugCallbackTypeBitflags types, const RedDebugCallbackData * data, RedContext context) {
  vfInternalPrint("[vkFast][Debug callback] ");
  vfInternalPrint(data->message);
  red32Exit(1);
  return 0;
}

GPU_API_PRE void GPU_API_POST vfContextInit(int enable_debug_mode, const gpu_context_optional_parameters_t * optional_parameters, const char * optionalFile, int optionalLine) {
  if (enable_debug_mode) {
    vfInternalPrint("[vkFast][Debug] In case of an error, email me (Constantine) at: iamvfx@gmail.com" "\n");
  }

  // To free
  g_vkfast = (vf_global_state_context_t *)red32MemoryCalloc(sizeof(vf_global_state_context_t));
  REDGPU_2_EXPECT(g_vkfast != NULL);

  uint64_t internalMemoryAllocationSizeGpuVramArrays = VKFAST_INTERNAL_DEFAULT_MEMORY_ALLOCATION_SIZE_GPU_VRAM_ARRAYS_512MB;
  uint64_t internalMemoryAllocationSizeGpuVramImages = VKFAST_INTERNAL_DEFAULT_MEMORY_ALLOCATION_SIZE_GPU_VRAM_IMAGES_512MB;
  uint64_t internalMemoryAllocationSizeCpuVisible    = VKFAST_INTERNAL_DEFAULT_MEMORY_ALLOCATION_SIZE_CPU_VISIBLE_512MB;
  uint64_t internalMemoryAllocationSizeCpuReadback   = VKFAST_INTERNAL_DEFAULT_MEMORY_ALLOCATION_SIZE_CPU_READBACK_512MB;
  if (optional_parameters != NULL) {
    if (optional_parameters->internal_memory_allocation_sizes != NULL) {
      internalMemoryAllocationSizeGpuVramArrays = optional_parameters->internal_memory_allocation_sizes->bytes_count_for_memory_gpu_vram_arrays;
      internalMemoryAllocationSizeGpuVramImages = optional_parameters->internal_memory_allocation_sizes->bytes_count_for_memory_gpu_vram_images;
      internalMemoryAllocationSizeCpuVisible    = optional_parameters->internal_memory_allocation_sizes->bytes_count_for_memory_cpu_visible;
      internalMemoryAllocationSizeCpuReadback   = optional_parameters->internal_memory_allocation_sizes->bytes_count_for_memory_cpu_readback;
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
      REDGPU_2_EXPECTWG(!"Unsupported by vkFast GPU, recompile your program with vfContextInit()::enable_debug_mode parameter enabled and email me your GPU name please: iamvfx@gmail.com");
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
      "restrictToAccess", 0,
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
  {
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
  vf_global_state_context_t;
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
    vf_handle_t * handle = (vf_handle_t *)(void *)ids[i];
    if (handle->handle_id == VF_HANDLE_ID_INVALID) {
      continue;
    }

    if (handle->handle_id == VF_HANDLE_ID_TEXTURE) {
      np(red2DestroyHandle,
        "context", g_vkfast->context,
        "gpu", g_vkfast->gpu,
        "handleType", RED_HANDLE_TYPE_TEXTURE,
        "handle", handle->texture.texture,
        "optionalHandle2", NULL,
        "optionalFile", optionalFile,
        "optionalLine", optionalLine,
        "optionalUserData", NULL
      );
      np(red2DestroyHandle,
        "context", g_vkfast->context,
        "gpu", g_vkfast->gpu,
        "handleType", RED_HANDLE_TYPE_IMAGE,
        "handle", handle->texture.image.image.handle,
        "optionalHandle2", NULL,
        "optionalFile", optionalFile,
        "optionalLine", optionalLine,
        "optionalUserData", NULL
      );
      continue;
    }

    if (handle->handle_id == VF_HANDLE_ID_SAMPLER) {
      np(red2DestroyHandle,
        "context", g_vkfast->context,
        "gpu", g_vkfast->gpu,
        "handleType", RED_HANDLE_TYPE_SAMPLER,
        "handle", handle->sampler.sampler,
        "optionalHandle2", NULL,
        "optionalFile", optionalFile,
        "optionalLine", optionalLine,
        "optionalUserData", NULL
      );
      continue;
    }

    if (handle->handle_id == VF_HANDLE_ID_GPU_CODE) {
      np(red2DestroyHandle,
        "context", g_vkfast->context,
        "gpu", g_vkfast->gpu,
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
        "context", g_vkfast->context,
        "gpu", g_vkfast->gpu,
        "handleType", RED_HANDLE_TYPE_PROCEDURE_PARAMETERS,
        "handle", handle->procedure.procedureParameters,
        "optionalHandle2", NULL,
        "optionalFile", optionalFile,
        "optionalLine", optionalLine,
        "optionalUserData", NULL
      );
      np(red2DestroyHandle,
        "context", g_vkfast->context,
        "gpu", g_vkfast->gpu,
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
        "context", g_vkfast->context,
        "gpu", g_vkfast->gpu,
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
    vf_handle_t * handle = (vf_handle_t *)(void *)ids[i];
    red32MemoryFree(handle); // NOTE(Constantine): Internally, all handles must be allocated.
  }

  red32MemoryFree(g_vkfast);
  g_vkfast = NULL;
}

GPU_API_PRE void GPU_API_POST vfWindowFullscreen(void * optional_existing_window_handle, const char * window_title, int screen_width, int screen_height, int msaa_samples, const char * optionalFile, int optionalLine) {
  REDGPU_2_EXPECT(msaa_samples == 1 || !"[vkFast][TODO] MSAA > 1.");

  void * window_handle = optional_existing_window_handle;
  if (window_handle == NULL) {
    window_handle = red32WindowCreate(window_title);
  }

  // TODO(Constantine): If already have redgpu window resources, destroy them and create them again here.

  g_vkfast->windowHandle = window_handle;
  g_vkfast->screenWidth = screen_width;
  g_vkfast->screenHeight = screen_height;
  g_vkfast->msaaSamples = msaa_samples;
}

GPU_API_PRE int GPU_API_POST vfWindowLoop() {
  return red32WindowLoop();
}

GPU_API_PRE void GPU_API_POST vfExit(int exit_code) {
  red32Exit(exit_code);
}

GPU_API_PRE void GPU_API_POST vfStorageCreateFromStruct(const gpu_storage_info_t * storage_info, gpu_storage_t * out_storage, const char * optionalFile, int optionalLine) {
  // NOTE(Constantine):
  // This procedure is not thread-safe since it modifies global g_vkfast->*_memory_suballocations_offset values.
  // This procedure doesn't do anything CPU heavy, it just calculates some pointer offsets, so it's cheap to call it
  // from one thread only or with an external mutex lock.

  RedHandleGpu gpu = g_vkfast->gpu;

  REDGPU_2_EXPECTWG(storage_info->storage_type != GPU_STORAGE_TYPE_NONE);

  uint64_t             alignment         = 0;
  RedStructMemberArray arrayRangeInfo    = {0};
  void *               mappedVoidPointer = NULL;
  {
    // NOTE(Constantine): Storage range mapping.

    if (storage_info->storage_type == GPU_STORAGE_TYPE_GPU_ONLY) {
      
      alignment = g_vkfast->gpuInfo->minArrayRORWStructMemberRangeBytesAlignment;

      // NOTE(Constantine): Aligns start address.
      g_vkfast->memoryGpuVramForArrays_memory_suballocations_offset += REDGPU_2_BYTES_TO_NEXT_ALIGNMENT_BOUNDARY(g_vkfast->memoryGpuVramForArrays_memory_suballocations_offset, alignment);

      arrayRangeInfo.array                = g_vkfast->memoryGpuVramForArrays_array.array.handle;
      arrayRangeInfo.arrayRangeBytesFirst = g_vkfast->memoryGpuVramForArrays_memory_suballocations_offset;
      arrayRangeInfo.arrayRangeBytesCount = storage_info->bytes_count + REDGPU_2_BYTES_TO_NEXT_ALIGNMENT_BOUNDARY(storage_info->bytes_count, alignment);
      g_vkfast->memoryGpuVramForArrays_memory_suballocations_offset += arrayRangeInfo.arrayRangeBytesCount;

      REDGPU_2_EXPECTWG(g_vkfast->memoryGpuVramForArrays_memory_suballocations_offset <= g_vkfast->memoryGpuVramForArrays_array.array.memoryBytesCount);

    } else if (storage_info->storage_type == GPU_STORAGE_TYPE_CPU_UPLOAD) {
    
      alignment = g_vkfast->gpuInfo->minMemoryAllocateBytesAlignment; // NOTE(Constantine): Can't be placed into a struct, so picking only one alignment.

      // NOTE(Constantine): Aligns start address.
      g_vkfast->memoryCpuUpload_memory_suballocations_offset += REDGPU_2_BYTES_TO_NEXT_ALIGNMENT_BOUNDARY(g_vkfast->memoryCpuUpload_memory_suballocations_offset, alignment);

      arrayRangeInfo.array                = g_vkfast->memoryCpuUpload_memory_and_array.array.handle;
      arrayRangeInfo.arrayRangeBytesFirst = g_vkfast->memoryCpuUpload_memory_suballocations_offset;
      arrayRangeInfo.arrayRangeBytesCount = storage_info->bytes_count + REDGPU_2_BYTES_TO_NEXT_ALIGNMENT_BOUNDARY(storage_info->bytes_count, alignment);
      g_vkfast->memoryCpuUpload_memory_suballocations_offset += arrayRangeInfo.arrayRangeBytesCount;

      REDGPU_2_EXPECTWG(g_vkfast->memoryCpuUpload_memory_suballocations_offset <= g_vkfast->memoryCpuUpload_memory_and_array.array.memoryBytesCount);

    } else if (storage_info->storage_type == GPU_STORAGE_TYPE_CPU_READBACK) {
    
      alignment = g_vkfast->gpuInfo->minMemoryAllocateBytesAlignment; // NOTE(Constantine): Can't be placed into a struct, so picking only one alignment.

      // NOTE(Constantine): Aligns start address.
      g_vkfast->memoryCpuReadback_memory_suballocations_offset += REDGPU_2_BYTES_TO_NEXT_ALIGNMENT_BOUNDARY(g_vkfast->memoryCpuReadback_memory_suballocations_offset, alignment);

      arrayRangeInfo.array                = g_vkfast->memoryCpuReadback_memory_and_array.array.handle;
      arrayRangeInfo.arrayRangeBytesFirst = g_vkfast->memoryCpuReadback_memory_suballocations_offset;
      arrayRangeInfo.arrayRangeBytesCount = storage_info->bytes_count + REDGPU_2_BYTES_TO_NEXT_ALIGNMENT_BOUNDARY(storage_info->bytes_count, alignment);
      g_vkfast->memoryCpuReadback_memory_suballocations_offset += arrayRangeInfo.arrayRangeBytesCount;

      REDGPU_2_EXPECTWG(g_vkfast->memoryCpuReadback_memory_suballocations_offset <= g_vkfast->memoryCpuReadback_memory_and_array.array.memoryBytesCount);

    } else {
      REDGPU_2_EXPECT(!"[vkFast Internal][" __FUNCTION__ "] Unreachable enum value.");
    }

    // NOTE(Constantine): Pointer mapping.

    if (storage_info->storage_type == GPU_STORAGE_TYPE_CPU_UPLOAD) {
    
      mappedVoidPointer = g_vkfast->memoryCpuUpload_mapped_void_ptr; // NOTE(Constantine): Start address is guaranteed to be aligned.
      
      uint8_t * ptr = (uint8_t *)g_vkfast->memoryCpuUpload_mapped_void_ptr;
      ptr += arrayRangeInfo.arrayRangeBytesCount + REDGPU_2_BYTES_TO_NEXT_ALIGNMENT_BOUNDARY(arrayRangeInfo.arrayRangeBytesCount, alignment);
      g_vkfast->memoryCpuUpload_mapped_void_ptr = (void *)ptr;

    } else if (storage_info->storage_type == GPU_STORAGE_TYPE_CPU_READBACK) {
    
      mappedVoidPointer = g_vkfast->memoryCpuReadback_mapped_void_ptr; // NOTE(Constantine): Start address is guaranteed to be aligned.
      
      uint8_t * ptr = (uint8_t *)g_vkfast->memoryCpuReadback_mapped_void_ptr;
      ptr += arrayRangeInfo.arrayRangeBytesCount + REDGPU_2_BYTES_TO_NEXT_ALIGNMENT_BOUNDARY(arrayRangeInfo.arrayRangeBytesCount, alignment);
      g_vkfast->memoryCpuReadback_mapped_void_ptr = (void *)ptr;

    }
  }

  // To free
  vf_handle_t * handle = (vf_handle_t *)red32MemoryCalloc(sizeof(vf_handle_t));
  REDGPU_2_EXPECTWG(handle != NULL);

  // Filling
  vf_handle_t;
  vf_handle_storage_t;
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

GPU_API_PRE uint64_t GPU_API_POST vfTextureCreateFromStruct(const gpu_texture_info_t * texture_info, const char * optionalFile, int optionalLine) {
  RedHandleGpu gpu = g_vkfast->gpu;

  REDGPU_2_EXPECTWG(texture_info->count > 0);
  REDGPU_2_EXPECTWG(texture_info->mipmap_levels_count > 0);

  Red2Image image = {0};
  Red2Memory * memorys[] = {&g_vkfast->memoryGpuVramForImages_memory};
  np(red2CreateImage,
    "context", g_vkfast->context,
    "gpu", g_vkfast->gpu,
    "handleName", texture_info->optional_debug_name,
    "dimensions", RED_IMAGE_DIMENSIONS_2D,
    "format", texture_info->format,
    "width", texture_info->w,
    "height", texture_info->h,
    "depth", 1,
    "levelsCount", texture_info->mipmap_levels_count,
    "layersCount", texture_info->count,
    "multisampleCount", RED_MULTISAMPLE_COUNT_BITFLAG_1,
    "restrictToAccess", 0,
    "initialQueueFamilyIndex", g_vkfast->mainQueueFamilyIndex,
    "dedicate", 0,
    "dedicateMemoryTypeIndex", 0,
    "dedicateMemoryBitflags", 0,
    "suballocateFromMemoryOnFirstMatchPointersCount", _countof(memorys),
    "suballocateFromMemoryOnFirstMatchPointers", memorys,
    "outImage", &image,
    "outStatuses", NULL,
    "optionalFile", optionalFile,
    "optionalLine", optionalLine,
    "optionalUserData", NULL
  );

  RedHandleTexture texture = NULL;
  np(redCreateTexture,
    "context", g_vkfast->context,
    "gpu", g_vkfast->gpu,
    "handleName", texture_info->optional_debug_name,
    "image", image.image.handle,
    "parts", RED_IMAGE_PART_BITFLAG_COLOR,
    "dimensions", RED_TEXTURE_DIMENSIONS_2D_LAYERED,
    "format", texture_info->format,
    "levelsFirst", 0,
    "levelsCount", texture_info->mipmap_levels_count,
    "layersFirst", 0,
    "layersCount", texture_info->count,
    "restrictToAccess", 0,
    "outTexture", &texture,
    "outStatuses", NULL,
    "optionalFile", optionalFile,
    "optionalLine", optionalLine,
    "optionalUserData", NULL
  );

  vf_handle_t * handle = (vf_handle_t *)red32MemoryCalloc(sizeof(vf_handle_t));
  REDGPU_2_EXPECTWG(handle != NULL);

  // Filling
  vf_handle_t;
  vf_handle_texture_t;
  handle->handle_id       = VF_HANDLE_ID_TEXTURE;
  handle->texture.info    = texture_info[0];
  handle->texture.image   = image;
  handle->texture.texture = texture;

  return (uint64_t)(void *)handle;
}

GPU_API_PRE uint64_t GPU_API_POST vfSamplerCreateFromStruct(const gpu_sampler_info_t * sampler_info, const char * optionalFile, int optionalLine) {
  RedHandleGpu gpu = g_vkfast->gpu;

  RedHandleSampler sampler = NULL;
  np(redCreateSampler,
    "context", g_vkfast->context,
    "gpu", g_vkfast->gpu,
    "handleName", sampler_info->optional_debug_name,
    "filteringMag", sampler_info->filteringMag,
    "filteringMin", sampler_info->filteringMin,
    "filteringMip", sampler_info->filteringMip,
    "behaviorOutsideTextureCoordinateU", sampler_info->behaviorOutsideTextureCoordinateU,
    "behaviorOutsideTextureCoordinateV", sampler_info->behaviorOutsideTextureCoordinateV,
    "behaviorOutsideTextureCoordinateW", sampler_info->behaviorOutsideTextureCoordinateW,
    "mipLodBias", sampler_info->mipLodBias,
    "enableAnisotropy", sampler_info->enableAnisotropy,
    "maxAnisotropy", sampler_info->maxAnisotropy,
    "enableCompare", sampler_info->enableCompare,
    "compareOp", sampler_info->compareOp,
    "minLod", sampler_info->minLod,
    "maxLod", sampler_info->maxLod,
    "outSampler", &sampler,
    "outStatuses", NULL,
    "optionalFile", optionalFile,
    "optionalLine", optionalLine,
    "optionalUserData", NULL
  );

  vf_handle_t * handle = (vf_handle_t *)red32MemoryCalloc(sizeof(vf_handle_t));
  REDGPU_2_EXPECTWG(handle != NULL);

  // Filling
  vf_handle_t;
  vf_handle_sampler_t;
  handle->handle_id       = VF_HANDLE_ID_SAMPLER;
  handle->sampler.info    = sampler_info[0];
  handle->sampler.sampler = sampler;

  return (uint64_t)(void *)handle;
}

GPU_API_PRE uint64_t GPU_API_POST vfTextureCreateFromBmp(int width, int height, int generate_mip_levels, int texture_count, const char ** texture_paths, const char * optionalFile, int optionalLine) {
  REDGPU_2_EXPECT(!"TODO"); return -1;
}

GPU_API_PRE uint64_t GPU_API_POST vfCubemapCreateFromBmp(int width, int height, int generate_mip_levels, int texture_count, const char ** pos_x_texture_paths, const char ** neg_x_texture_paths, const char ** pos_y_texture_paths, const char ** neg_y_texture_paths, const char ** pos_z_texture_paths, const char ** neg_z_texture_paths, const char * optionalFile, int optionalLine) {
  REDGPU_2_EXPECT(!"TODO"); return -1;
}

GPU_API_PRE uint64_t GPU_API_POST vfProgramCreateFromBinaryFileVertProgram(const char * shader_binary_filepath, const char * optionalFile, int optionalLine) {
  REDGPU_2_EXPECT(!"TODO"); return -1;
}

GPU_API_PRE uint64_t GPU_API_POST vfProgramCreateFromBinaryFileFragProgram(const char * shader_binary_filepath, const char * optionalFile, int optionalLine) {
  REDGPU_2_EXPECT(!"TODO"); return -1;
}

GPU_API_PRE uint64_t GPU_API_POST vfProgramCreateFromBinaryFileCompProgram(const char * shader_binary_filepath, const char * optionalFile, int optionalLine) {
  REDGPU_2_EXPECT(!"TODO"); return -1;
}

GPU_API_PRE uint64_t GPU_API_POST vfProgramCreateFromBinaryVertProgram(const gpu_program_info_t * program_info, const char * optionalFile, int optionalLine) {
  RedHandleGpu gpu = g_vkfast->gpu;

  RedHandleGpuCode gpuCode = NULL;
  np(redCreateGpuCode,
    "context", g_vkfast->context,
    "gpu", g_vkfast->gpu,
    "handleName", program_info->optional_debug_name,
    "irBytesCount", program_info->program_binary_bytes_count,
    "ir", (const void *)program_info->program_binary,
    "outGpuCode", &gpuCode,
    "outStatuses", NULL,
    "optionalFile", optionalFile,
    "optionalLine", optionalLine,
    "optionalUserData", NULL
  );

  vf_handle_t * handle = (vf_handle_t *)red32MemoryCalloc(sizeof(vf_handle_t));
  REDGPU_2_EXPECTWG(handle != NULL);

  // Filling
  vf_handle_t;
  vf_handle_gpu_code_t;
  handle->handle_id           = VF_HANDLE_ID_GPU_CODE;
  handle->gpuCode.info        = program_info[0];
  handle->gpuCode.gpuCodeType = VF_GPU_CODE_TYPE_VERTEX;
  handle->gpuCode.gpuCode     = gpuCode;

  return (uint64_t)(void *)handle;
}

GPU_API_PRE uint64_t GPU_API_POST vfProgramCreateFromBinaryFragProgram(const gpu_program_info_t * program_info, const char * optionalFile, int optionalLine) {
  RedHandleGpu gpu = g_vkfast->gpu;

  RedHandleGpuCode gpuCode = NULL;
  np(redCreateGpuCode,
    "context", g_vkfast->context,
    "gpu", g_vkfast->gpu,
    "handleName", program_info->optional_debug_name,
    "irBytesCount", program_info->program_binary_bytes_count,
    "ir", (const void *)program_info->program_binary,
    "outGpuCode", &gpuCode,
    "outStatuses", NULL,
    "optionalFile", optionalFile,
    "optionalLine", optionalLine,
    "optionalUserData", NULL
  );

  vf_handle_t * handle = (vf_handle_t *)red32MemoryCalloc(sizeof(vf_handle_t));
  REDGPU_2_EXPECTWG(handle != NULL);

  // Filling
  vf_handle_t;
  vf_handle_gpu_code_t;
  handle->handle_id           = VF_HANDLE_ID_GPU_CODE;
  handle->gpuCode.info        = program_info[0];
  handle->gpuCode.gpuCodeType = VF_GPU_CODE_TYPE_FRAGMENT;
  handle->gpuCode.gpuCode     = gpuCode;

  return (uint64_t)(void *)handle;
}

GPU_API_PRE uint64_t GPU_API_POST vfProgramCreateFromBinaryCompProgram(const gpu_program_info_t * program_info, const char * optionalFile, int optionalLine) {
  RedHandleGpu gpu = g_vkfast->gpu;

  RedHandleGpuCode gpuCode = NULL;
  np(redCreateGpuCode,
    "context", g_vkfast->context,
    "gpu", g_vkfast->gpu,
    "handleName", program_info->optional_debug_name,
    "irBytesCount", program_info->program_binary_bytes_count,
    "ir", (const void *)program_info->program_binary,
    "outGpuCode", &gpuCode,
    "outStatuses", NULL,
    "optionalFile", optionalFile,
    "optionalLine", optionalLine,
    "optionalUserData", NULL
  );

  vf_handle_t * handle = (vf_handle_t *)red32MemoryCalloc(sizeof(vf_handle_t));
  REDGPU_2_EXPECTWG(handle != NULL);

  vf_handle_t;
  vf_handle_gpu_code_t;
  handle->handle_id           = VF_HANDLE_ID_GPU_CODE;
  handle->gpuCode.info        = program_info[0];
  handle->gpuCode.gpuCodeType = VF_GPU_CODE_TYPE_COMPUTE;
  handle->gpuCode.gpuCode     = gpuCode;

  return (uint64_t)(void *)handle;
}

GPU_API_PRE uint64_t GPU_API_POST vfProgramPipelineCreate(const gpu_program_pipeline_info_t * program_pipeline_info, const char * optionalFile, int optionalLine) {
  RedHandleGpu gpu = g_vkfast->gpu;

  vf_handle_t * gpuCodeVert = (vf_handle_t *)(void *)program_pipeline_info->vert_program;
  vf_handle_t * gpuCodeFrag = (vf_handle_t *)(void *)program_pipeline_info->frag_program;
  REDGPU_2_EXPECTWG(gpuCodeVert->handle_id == VF_HANDLE_ID_GPU_CODE);
  REDGPU_2_EXPECTWG(gpuCodeFrag->handle_id == VF_HANDLE_ID_GPU_CODE);
  REDGPU_2_EXPECTWG(gpuCodeVert->gpuCode.gpuCodeType == VF_GPU_CODE_TYPE_VERTEX);
  REDGPU_2_EXPECTWG(gpuCodeFrag->gpuCode.gpuCodeType == VF_GPU_CODE_TYPE_FRAGMENT);

  RedHandleProcedureParameters procedureParameters = NULL;
  np(red2CreateProcedureParameters,
    "context", g_vkfast->context,
    "gpu", g_vkfast->gpu,
    "handleName", program_pipeline_info->optional_debug_name,
    "procedureParametersDeclaration", &program_pipeline_info->parameters,
    "outProcedureParameters", &procedureParameters,
    "outStatuses", NULL,
    "optionalFile", optionalFile,
    "optionalLine", optionalLine,
    "optionalUserData", NULL
  );

  RedOutputDeclarationMembers outputDeclarationMembers = {0};
  outputDeclarationMembers.depthStencilEnable                        = 1;
  outputDeclarationMembers.depthStencilFormat                        = RED_FORMAT_DEPTH_32_FLOAT;
  outputDeclarationMembers.depthStencilMultisampleCount              = RED_MULTISAMPLE_COUNT_BITFLAG_1;
  outputDeclarationMembers.depthStencilDepthSetProcedureOutputOp     = RED_SET_PROCEDURE_OUTPUT_OP_PRESERVE;
  outputDeclarationMembers.depthStencilDepthEndProcedureOutputOp     = RED_END_PROCEDURE_OUTPUT_OP_PRESERVE;
  outputDeclarationMembers.depthStencilStencilSetProcedureOutputOp   = RED_SET_PROCEDURE_OUTPUT_OP_PRESERVE;
  outputDeclarationMembers.depthStencilStencilEndProcedureOutputOp   = RED_END_PROCEDURE_OUTPUT_OP_PRESERVE;
  outputDeclarationMembers.depthStencilSharesMemoryWithAnotherMember = 0;
  outputDeclarationMembers.colorsCount                               = 1;
  outputDeclarationMembers.colorsFormat[0]                           = RED_FORMAT_PRESENT_BGRA_8_8_8_8_UINT_TO_FLOAT_0_1;
  outputDeclarationMembers.colorsFormat[1]                           = RED_FORMAT_UNDEFINED;
  outputDeclarationMembers.colorsFormat[2]                           = RED_FORMAT_UNDEFINED;
  outputDeclarationMembers.colorsFormat[3]                           = RED_FORMAT_UNDEFINED;
  outputDeclarationMembers.colorsFormat[4]                           = RED_FORMAT_UNDEFINED;
  outputDeclarationMembers.colorsFormat[5]                           = RED_FORMAT_UNDEFINED;
  outputDeclarationMembers.colorsFormat[6]                           = RED_FORMAT_UNDEFINED;
  outputDeclarationMembers.colorsFormat[7]                           = RED_FORMAT_UNDEFINED;
  for (int i = 0; i < 8; i += 1) {
    outputDeclarationMembers.colorsMultisampleCount[i]               = RED_MULTISAMPLE_COUNT_BITFLAG_1;
    outputDeclarationMembers.colorsSetProcedureOutputOp[i]           = RED_SET_PROCEDURE_OUTPUT_OP_PRESERVE;
    outputDeclarationMembers.colorsEndProcedureOutputOp[i]           = RED_END_PROCEDURE_OUTPUT_OP_PRESERVE;
    outputDeclarationMembers.colorsSharesMemoryWithAnotherMember[i]  = 0;
  }
  RedHandleProcedure procedure = NULL;
  np(red2CreateProcedure,
    "context", g_vkfast->context,
    "gpu", g_vkfast->gpu,
    "handleName", program_pipeline_info->optional_debug_name,
    "procedureCache", NULL,
    "outputDeclarationMembers", &outputDeclarationMembers,
    "outputDeclarationMembersResolveSources", NULL, // TODO: MSAA
    "dependencyByRegion", 0,
    "dependencyByRegionAllowUsageAliasOrderBarriers", 0,
    "procedureParameters", procedureParameters,
    "gpuCodeVertexMainProcedureName", "main",
    "gpuCodeVertex", gpuCodeVert->gpuCode.gpuCode,
    "gpuCodeFragmentMainProcedureName", "main",
    "gpuCodeFragment", gpuCodeFrag->gpuCode.gpuCode,
    "state", &program_pipeline_info->state,
    "stateExtension", NULL,
    "deriveBase", 0,
    "deriveFrom", NULL,
    "outProcedure", &procedure,
    "outStatuses", NULL,
    "optionalFile", optionalFile,
    "optionalLine", optionalLine,
    "optionalUserData", NULL
  );

  vf_handle_t * handle = (vf_handle_t *)red32MemoryCalloc(sizeof(vf_handle_t));
  REDGPU_2_EXPECTWG(handle != NULL);

  // Filling
  vf_handle_t;
  vf_handle_procedure_t;
  handle->handle_id                     = VF_HANDLE_ID_PROCEDURE;
  handle->procedure.infoGraphics        = program_pipeline_info[0];
  handle->procedure.procedureType       = VF_PROCEDURE_TYPE_DRAW;
  handle->procedure.procedureParameters = procedureParameters;
  handle->procedure.procedure           = procedure;

  return (uint64_t)(void *)handle;
}

GPU_API_PRE uint64_t GPU_API_POST vfProgramPipelineCreateCompute(const gpu_program_pipeline_comp_info_t * program_pipeline_comp_info, const char * optionalFile, int optionalLine) {
  RedHandleGpu gpu = g_vkfast->gpu;

  vf_handle_t * gpuCodeComp = (vf_handle_t *)(void *)program_pipeline_comp_info->comp_program;
  REDGPU_2_EXPECTWG(gpuCodeComp->handle_id == VF_HANDLE_ID_GPU_CODE);
  REDGPU_2_EXPECTWG(gpuCodeComp->gpuCode.gpuCodeType == VF_GPU_CODE_TYPE_COMPUTE);

  RedHandleProcedureParameters procedureParameters = NULL;
  np(red2CreateProcedureParameters,
    "context", g_vkfast->context,
    "gpu", g_vkfast->gpu,
    "handleName", program_pipeline_comp_info->optional_debug_name,
    "procedureParametersDeclaration", &program_pipeline_comp_info->parameters,
    "outProcedureParameters", &procedureParameters,
    "outStatuses", NULL,
    "optionalFile", optionalFile,
    "optionalLine", optionalLine,
    "optionalUserData", NULL
  );

  RedHandleProcedure procedure = NULL;
  np(redCreateProcedureCompute,
    "context", g_vkfast->context,
    "gpu", g_vkfast->gpu,
    "handleName", program_pipeline_comp_info->optional_debug_name,
    "procedureCache", NULL,
    "procedureParameters", procedureParameters,
    "gpuCodeMainProcedureName", "main",
    "gpuCode", gpuCodeComp->gpuCode.gpuCode,
    "outProcedure", &procedure,
    "outStatuses", NULL,
    "optionalFile", optionalFile,
    "optionalLine", optionalLine,
    "optionalUserData", NULL
  );

  vf_handle_t * handle = (vf_handle_t *)red32MemoryCalloc(sizeof(vf_handle_t));
  REDGPU_2_EXPECTWG(handle != NULL);

  // Filling
  vf_handle_t;
  vf_handle_procedure_t;
  handle->handle_id                     = VF_HANDLE_ID_PROCEDURE;
  handle->procedure.infoCompute         = program_pipeline_comp_info[0];
  handle->procedure.procedureType       = VF_PROCEDURE_TYPE_COMPUTE;
  handle->procedure.procedureParameters = procedureParameters;
  handle->procedure.procedure           = procedure;

  return (uint64_t)(void *)handle;
}

GPU_API_PRE uint64_t GPU_API_POST vfBatchBegin(const gpu_batch_bindings_info_t * batch_bindings_info, const char * optional_debug_name, const char * optionalFile, int optionalLine) {
  RedHandleGpu gpu = g_vkfast->gpu;

  RedCalls calls = {0};
  np(redCreateCallsReusable,
    "context", g_vkfast->context,
    "gpu", g_vkfast->gpu,
    "handleName", optional_debug_name,
    "queueFamilyIndex", g_vkfast->mainQueueFamilyIndex,
    "outCalls", &calls,
    "outStatuses", NULL,
    "optionalFile", optionalFile,
    "optionalLine", optionalLine,
    "optionalUserData", NULL
  );
  
  RedCallProceduresAndAddresses addresses = {0};
  np(redGetCallProceduresAndAddresses,
    "context", g_vkfast->context,
    "gpu", g_vkfast->gpu,
    "outCallProceduresAndAddresses", &addresses,
    "outStatuses", NULL,
    "optionalFile", optionalFile,
    "optionalLine", optionalLine,
    "optionalUserData", NULL
  );

  RedHandleStructsMemory structsMemory = 0;
  if (batch_bindings_info != NULL) {
    np(redStructsMemoryAllocate,
      "context", g_vkfast->context,
      "gpu", g_vkfast->gpu,
      "handleName", optional_debug_name,
      "maxStructsCount", batch_bindings_info->maxNewBindingsSetsCount,
      "maxStructsMembersOfTypeArrayROConstantCount", batch_bindings_info->maxArrayROConstantCount,
      "maxStructsMembersOfTypeArrayROOrArrayRWCount", batch_bindings_info->maxArrayRORWCount,
      "maxStructsMembersOfTypeTextureROCount", batch_bindings_info->maxTextureROCount,
      "maxStructsMembersOfTypeTextureRWCount", batch_bindings_info->maxTextureRWCount,
      "outStructsMemory", &structsMemory,
      "outStatuses", NULL,
      "optionalFile", optionalFile,
      "optionalLine", optionalLine,
      "optionalUserData", NULL
    );
  }

  np(redCallsSet,
    "context", g_vkfast->context,
    "gpu", g_vkfast->gpu,
    "calls", calls.handle,
    "callsMemory", calls.memory,
    "callsReusable", calls.reusable,
    "outStatuses", NULL,
    "optionalFile", optionalFile,
    "optionalLine", optionalLine,
    "optionalUserData", NULL
  );

  // NOTE(Constantine): Oh God, I hope this call doesn't copy descriptors from structsMemory like redCallSetProcedureParametersStructs() :D
  np(redCallSetStructsMemory,
    "address", addresses.redCallSetStructsMemory,
    "calls", calls.handle,
    "structsMemory", structsMemory,
    "structsMemorySamplers", NULL // TODO: Separate structs memory for samplers.
  );

  vf_handle_t * handle = (vf_handle_t *)red32MemoryCalloc(sizeof(vf_handle_t));
  REDGPU_2_EXPECTWG(handle != NULL);

  // Filling
  vf_handle_t;
  vf_handle_batch_t;
  handle->handle_id           = VF_HANDLE_ID_BATCH;
  handle->batch.calls         = calls;
  handle->batch.addresses     = addresses;
  handle->batch.structsMemory = structsMemory;
  handle->batch.currentStruct = REDGPU_32_STRUCT(Red2Struct, 0);
  handle->batch.currentProcedureParameters = NULL;

  return (uint64_t)(void *)handle;
}

GPU_API_PRE void GPU_API_POST vfBatchStorageCopyFromCpuToGpu(uint64_t batch_id, uint64_t from_cpu_storage_id, uint64_t to_gpu_storage_id, const char * optionalFile, int optionalLine) {
  RedHandleGpu gpu = g_vkfast->gpu;

  vf_handle_t * batch = (vf_handle_t *)(void *)batch_id;
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

GPU_API_PRE void GPU_API_POST vfBatchStorageCopyFromGpuToCpu(uint64_t batch_id, uint64_t from_gpu_storage_id, uint64_t to_cpu_storage_id, const char * optionalFile, int optionalLine) {
  RedHandleGpu gpu = g_vkfast->gpu;

  vf_handle_t * batch = (vf_handle_t *)(void *)batch_id;
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

GPU_API_PRE void GPU_API_POST vfBatchBindProgramPipelineCompute(uint64_t batch_id, uint64_t program_pipeline_compute_id, const char * optionalFile, int optionalLine) {
  RedHandleGpu gpu = g_vkfast->gpu;

  vf_handle_t * batch = (vf_handle_t *)(void *)batch_id;
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

GPU_API_PRE void GPU_API_POST vfBatchBindNewBindingsSet(uint64_t batch_id, int slots_count, const RedStructDeclarationMember * slots, const char * optionalFile, int optionalLine) {
  RedHandleGpu gpu = g_vkfast->gpu;

  vf_handle_t * batch = (vf_handle_t *)(void *)batch_id;
  REDGPU_2_EXPECTWG(batch->handle_id == VF_HANDLE_ID_BATCH);

  if (batch->batch.currentStruct.handleDeclaration != NULL) {
    np(red2DestroyHandle,
      "context", g_vkfast->context,
      "gpu", g_vkfast->gpu,
      "handleType", RED_HANDLE_TYPE_STRUCT_DECLARATION,
      "handle", batch->batch.currentStruct.handleDeclaration,
      "optionalHandle2", NULL,
      "optionalFile", optionalFile,
      "optionalLine", optionalLine,
      "optionalUserData", NULL
    );
    batch->batch.currentStruct.handleDeclaration = NULL;
  }
  batch->batch.currentStruct.handle = NULL;

  REDGPU_2_EXPECTWG(batch->batch.structsMemory != NULL || !"vfBatchBegin()::batch_bindings_info was likely set to NULL?");

  Red2Struct structure = {0};
  np(red2StructsMemorySuballocateStruct,
    "context", g_vkfast->context,
    "gpu", g_vkfast->gpu,
    "handleName", NULL,
    "structsMemory", batch->batch.structsMemory,
    "structDeclarationMembersCount", slots_count,
    "structDeclarationMembers", slots,
    "structDeclarationMembersArrayROCount", 0, // TODO: Array ROs.
    "structDeclarationMembersArrayRO", NULL,   // TODO: Array ROs.
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
    "procedureType", RED_PROCEDURE_TYPE_DRAW,
    "procedureParameters", batch->batch.currentProcedureParameters
  );
  np(redCallSetProcedureParameters,
    "address", batch->batch.addresses.redCallSetProcedureParameters,
    "calls", batch->batch.calls.handle,
    "procedureType", RED_PROCEDURE_TYPE_COMPUTE,
    "procedureParameters", batch->batch.currentProcedureParameters
  );
}

GPU_API_PRE void GPU_API_POST vfBatchBindStorage(uint64_t batch_id, int slot, int storage_ids_count, const uint64_t * storage_ids, const char * optionalFile, int optionalLine) {
  RedHandleGpu gpu = g_vkfast->gpu;

  vf_handle_t * batch = (vf_handle_t *)(void *)batch_id;
  REDGPU_2_EXPECTWG(batch->handle_id == VF_HANDLE_ID_BATCH);

  // To free
  RedStructMemberArray * arrays = (RedStructMemberArray *)red32MemoryCalloc(storage_ids_count * sizeof(RedStructMemberArray));
  REDGPU_2_EXPECTWG(arrays != NULL);

  for (int i = 0; i < storage_ids_count; i += 1) {
    vf_handle_t * storage = (vf_handle_t *)(void *)storage_ids[i];
    REDGPU_2_EXPECTWG(storage->handle_id == VF_HANDLE_ID_STORAGE);

    arrays[i] = storage->storage.arrayRangeInfo;
  }

  REDGPU_2_EXPECTWG(batch->batch.currentStruct.handle != NULL || !"Was vfBatchBindNewBindingsSet() called previously?");

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
    "context", g_vkfast->context,
    "gpu", g_vkfast->gpu,
    "structsMembersCount", 1,
    "structsMembers", &member,
    "optionalFile", optionalFile,
    "optionalLine", optionalLine,
    "optionalUserData", NULL
  );

  red32MemoryFree(arrays);
  arrays = NULL;
}

GPU_API_PRE void GPU_API_POST vfBatchBindNewBindingsEnd(uint64_t batch_id, const char * optionalFile, int optionalLine) {
  RedHandleGpu gpu = g_vkfast->gpu;

  vf_handle_t * batch = (vf_handle_t *)(void *)batch_id;
  REDGPU_2_EXPECTWG(batch->handle_id == VF_HANDLE_ID_BATCH);

  npfp(redCallSetProcedureParametersStructs, batch->batch.addresses.redCallSetProcedureParametersStructs,
    "calls", batch->batch.calls.handle,
    "procedureType", RED_PROCEDURE_TYPE_COMPUTE, // TODO(Constantine): Should handle graphics procedures too.
    "procedureParameters", batch->batch.currentProcedureParameters,
    "procedureParametersDeclarationStructsDeclarationsFirst", 0,
    "structsCount", 1, // TODO(Constantine): Multiple structs, not just one?
    "structs", &batch->batch.currentStruct.handle,
    "setTo0", 0,
    "setTo00", 0
  );
}

GPU_API_PRE void GPU_API_POST vfBatchCompute(uint64_t batch_id, unsigned workgroups_count_x, unsigned workgroups_count_y, unsigned workgroups_count_z, const char * optionalFile, int optionalLine) {
  RedHandleGpu gpu = g_vkfast->gpu;

  vf_handle_t * batch = (vf_handle_t *)(void *)batch_id;
  REDGPU_2_EXPECTWG(batch->handle_id == VF_HANDLE_ID_BATCH);

  npfp(redCallProcedureCompute, batch->batch.addresses.redCallProcedureCompute,
    "calls", batch->batch.calls.handle,
    "workgroupsCountX", workgroups_count_x,
    "workgroupsCountY", workgroups_count_y,
    "workgroupsCountZ", workgroups_count_z
  );
}

GPU_API_PRE void GPU_API_POST vfBatchMemoryBarrier(uint64_t batch_id, const char * optionalFile, int optionalLine) {
  RedHandleGpu gpu = g_vkfast->gpu;

  vf_handle_t * batch = (vf_handle_t *)(void *)batch_id;
  REDGPU_2_EXPECTWG(batch->handle_id == VF_HANDLE_ID_BATCH);

  np(red2CallGlobalOrderBarrier,
    "address", batch->batch.addresses.redCallUsageAliasOrderBarrier,
    "calls", batch->batch.calls.handle
  );
}

GPU_API_PRE void GPU_API_POST vfBatchCpuReadbackBarrier(uint64_t batch_id, const char * optionalFile, int optionalLine) {
  RedHandleGpu gpu = g_vkfast->gpu;

  vf_handle_t * batch = (vf_handle_t *)(void *)batch_id;
  REDGPU_2_EXPECTWG(batch->handle_id == VF_HANDLE_ID_BATCH);

  np(red2CallGlobalReadbackBarrier,
    "address", batch->batch.addresses.redCallUsageAliasOrderBarrier,
    "calls", batch->batch.calls.handle
  );
}

GPU_API_PRE void GPU_API_POST vfBatchEnd(uint64_t batch_id, const char * optionalFile, int optionalLine) {
  RedHandleGpu gpu = g_vkfast->gpu;

  vf_handle_t * batch = (vf_handle_t *)(void *)batch_id;
  REDGPU_2_EXPECTWG(batch->handle_id == VF_HANDLE_ID_BATCH);

  np(redCallsEnd,
    "context", g_vkfast->context,
    "gpu", g_vkfast->gpu,
    "calls", batch->batch.calls.handle,
    "callsMemory", batch->batch.calls.memory,
    "outStatuses", NULL,
    "optionalFile", optionalFile,
    "optionalLine", optionalLine,
    "optionalUserData", NULL
  );

  if (batch->batch.currentStruct.handleDeclaration != NULL) {
    np(red2DestroyHandle,
      "context", g_vkfast->context,
      "gpu", g_vkfast->gpu,
      "handleType", RED_HANDLE_TYPE_STRUCT_DECLARATION,
      "handle", batch->batch.currentStruct.handleDeclaration,
      "optionalHandle2", NULL,
      "optionalFile", optionalFile,
      "optionalLine", optionalLine,
      "optionalUserData", NULL
    );
    batch->batch.currentStruct.handleDeclaration = NULL;
  }
  batch->batch.currentStruct.handle = NULL;
  batch->batch.currentProcedureParameters = NULL;
}

GPU_API_PRE uint64_t GPU_API_POST vfAsyncBatchExecute(uint64_t batch_ids_count, const uint64_t * batch_ids, const char * optionalFile, int optionalLine) {
  RedHandleGpu gpu = g_vkfast->gpu;

  // To free
  RedHandleCalls * calls = (RedHandleCalls *)red32MemoryCalloc(batch_ids_count * sizeof(RedHandleCalls));
  REDGPU_2_EXPECTWG(calls != NULL);

  for (int i = 0; i < batch_ids_count; i += 1) {
    vf_handle_t * batch = (vf_handle_t *)(void *)batch_ids[i];
    REDGPU_2_EXPECTWG(batch->handle_id == VF_HANDLE_ID_BATCH);

    calls[i] = batch->batch.calls.handle;
  }

  RedHandleCpuSignal cpuSignal = NULL;
  np(redCreateCpuSignal,
    "context", g_vkfast->context,
    "gpu", g_vkfast->gpu,
    "handleName", NULL,
    "createSignaled", 0,
    "outCpuSignal", &cpuSignal,
    "outStatuses", NULL,
    "optionalFile", optionalFile,
    "optionalLine", optionalLine,
    "optionalUserData", NULL
  );

  RedGpuTimeline timeline = {0};
  timeline.setTo4                            = 4;
  timeline.setTo0                            = 0;
  timeline.waitForAndUnsignalGpuSignalsCount = 0;
  timeline.waitForAndUnsignalGpuSignals      = NULL;
  timeline.setTo65536                        = NULL;
  timeline.callsCount                        = batch_ids_count;
  timeline.calls                             = calls;
  timeline.signalGpuSignalsCount             = 0;
  timeline.signalGpuSignals                  = NULL;
  np(redQueueSubmit,
    "context", g_vkfast->context,
    "gpu", g_vkfast->gpu,
    "queue", g_vkfast->mainQueue,
    "timelinesCount", 1,
    "timelines", &timeline,
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

GPU_API_PRE void GPU_API_POST vfAsyncWaitToFinish(uint64_t async_id, const char * optionalFile, int optionalLine) {
  RedHandleCpuSignal cpuSignal = (RedHandleCpuSignal)(void *)async_id;

  np(redCpuSignalWait,
    "context", g_vkfast->context,
    "gpu", g_vkfast->gpu,
    "cpuSignalsCount", 1,
    "cpuSignals", &cpuSignal,
    "waitAll", 1,
    "outStatuses", NULL,
    "optionalFile", optionalFile,
    "optionalLine", optionalLine,
    "optionalUserData", NULL
  );

  np(red2DestroyHandle,
    "context", g_vkfast->context,
    "gpu", g_vkfast->gpu,
    "handleType", RED_HANDLE_TYPE_CPU_SIGNAL,
    "handle", cpuSignal,
    "optionalHandle2", NULL,
    "optionalFile", optionalFile,
    "optionalLine", optionalLine,
    "optionalUserData", NULL
  );
}

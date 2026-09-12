#pragma once

#include "../../../vkfast.h"
#include "../../../vkfast_ex.h"
#include "../../../vkfast_ids.h"

#ifndef __cplusplus
  #ifndef REDGPU_DISABLE_NAMED_PARAMETERS
  #define REDGPU_DISABLE_NAMED_PARAMETERS
  #endif
#endif
#if defined(_WIN32)
  #if defined(VKFAST_INCLUDE_TERMUX_PATHS)
    #include "/data/data/com.termux/files/home/RedGpuSDK/misc/np/np.h"
    #include "/data/data/com.termux/files/home/RedGpuSDK/misc/np/np_redgpu.h"
    #include "/data/data/com.termux/files/home/RedGpuSDK/misc/np/np_redgpu_2.h"
    #include "/data/data/com.termux/files/home/RedGpuSDK/misc/np/np_redgpu_wsi.h"
    #include "/data/data/com.termux/files/home/RedGpuSDK/redgpu_context_from_vk.h"
  #elif defined(VKFAST_INCLUDE_LINUX_PATHS)
    #include "/home/linuxbrew/RedGpuSDK/misc/np/np.h"
    #include "/home/linuxbrew/RedGpuSDK/misc/np/np_redgpu.h"
    #include "/home/linuxbrew/RedGpuSDK/misc/np/np_redgpu_2.h"
    #include "/home/linuxbrew/RedGpuSDK/misc/np/np_redgpu_wsi.h"
    #include "/home/linuxbrew/RedGpuSDK/redgpu_context_from_vk.h"
  #else
    #include "C:/RedGpuSDK/misc/np/np.h"
    #include "C:/RedGpuSDK/misc/np/np_redgpu.h"
    #include "C:/RedGpuSDK/misc/np/np_redgpu_2.h"
    #include "C:/RedGpuSDK/misc/np/np_redgpu_wsi.h"
    #include "C:/RedGpuSDK/redgpu_context_from_vk.h"
  #endif
#elif defined(__linux__) && !defined(__ANDROID__)
  #include "/home/linuxbrew/RedGpuSDK/misc/np/np.h"
  #include "/home/linuxbrew/RedGpuSDK/misc/np/np_redgpu.h"
  #include "/home/linuxbrew/RedGpuSDK/misc/np/np_redgpu_2.h"
  #include "/home/linuxbrew/RedGpuSDK/misc/np/np_redgpu_wsi.h"
  #include "/home/linuxbrew/RedGpuSDK/redgpu_context_from_vk.h"
#else
  #error Unsupported OS for now
#endif

typedef struct VfeReBARMallocShared {
  RedArray        array;
  RedHandleMemory memory;
} VfeReBARMallocShared;

#ifndef VFE_REBAR_API_NON_STATIC
  #define VFE_REBAR_API_PRE  static
  #define VFE_REBAR_API_POST
#endif

VFE_REBAR_API_PRE void     VFE_REBAR_API_POST vfeReBARGetMemoryBudget        (gpu_handle_context_t context, RedMemoryBudget * outMemoryBudget);
VFE_REBAR_API_PRE uint64_t VFE_REBAR_API_POST vfeReBARGetMaxMallocSharedSize (gpu_handle_context_t context);
VFE_REBAR_API_PRE void *   VFE_REBAR_API_POST vfeReBARMallocShared           (gpu_handle_context_t context, uint64_t bytesCount, VfeReBARMallocShared * outArray);
VFE_REBAR_API_PRE void     VFE_REBAR_API_POST vfeReBARFreeShared             (gpu_handle_context_t context, VfeReBARMallocShared * array);

#ifndef VFE_REBAR_API_NON_STATIC
  #define VFE_REBAR_IMPLEMENTATION
#endif
#ifdef VFE_REBAR_IMPLEMENTATION

static void vfeReBARInternalFillMemoryTypeIsSupportedArray(unsigned resourceMemoryTypesSupported, unsigned char * outMemoryTypeIsSupportedArrayOf32) {
  outMemoryTypeIsSupportedArrayOf32[0]  = (resourceMemoryTypesSupported & REDGPU_B32(0000,0000,0000,0000,0000,0000,0000,0001)) == 0 ? 0 : 1;
  outMemoryTypeIsSupportedArrayOf32[1]  = (resourceMemoryTypesSupported & REDGPU_B32(0000,0000,0000,0000,0000,0000,0000,0010)) == 0 ? 0 : 1;
  outMemoryTypeIsSupportedArrayOf32[2]  = (resourceMemoryTypesSupported & REDGPU_B32(0000,0000,0000,0000,0000,0000,0000,0100)) == 0 ? 0 : 1;
  outMemoryTypeIsSupportedArrayOf32[3]  = (resourceMemoryTypesSupported & REDGPU_B32(0000,0000,0000,0000,0000,0000,0000,1000)) == 0 ? 0 : 1;
  outMemoryTypeIsSupportedArrayOf32[4]  = (resourceMemoryTypesSupported & REDGPU_B32(0000,0000,0000,0000,0000,0000,0001,0000)) == 0 ? 0 : 1;
  outMemoryTypeIsSupportedArrayOf32[5]  = (resourceMemoryTypesSupported & REDGPU_B32(0000,0000,0000,0000,0000,0000,0010,0000)) == 0 ? 0 : 1;
  outMemoryTypeIsSupportedArrayOf32[6]  = (resourceMemoryTypesSupported & REDGPU_B32(0000,0000,0000,0000,0000,0000,0100,0000)) == 0 ? 0 : 1;
  outMemoryTypeIsSupportedArrayOf32[7]  = (resourceMemoryTypesSupported & REDGPU_B32(0000,0000,0000,0000,0000,0000,1000,0000)) == 0 ? 0 : 1;
  outMemoryTypeIsSupportedArrayOf32[8]  = (resourceMemoryTypesSupported & REDGPU_B32(0000,0000,0000,0000,0000,0001,0000,0000)) == 0 ? 0 : 1;
  outMemoryTypeIsSupportedArrayOf32[9]  = (resourceMemoryTypesSupported & REDGPU_B32(0000,0000,0000,0000,0000,0010,0000,0000)) == 0 ? 0 : 1;
  outMemoryTypeIsSupportedArrayOf32[10] = (resourceMemoryTypesSupported & REDGPU_B32(0000,0000,0000,0000,0000,0100,0000,0000)) == 0 ? 0 : 1;
  outMemoryTypeIsSupportedArrayOf32[11] = (resourceMemoryTypesSupported & REDGPU_B32(0000,0000,0000,0000,0000,1000,0000,0000)) == 0 ? 0 : 1;
  outMemoryTypeIsSupportedArrayOf32[12] = (resourceMemoryTypesSupported & REDGPU_B32(0000,0000,0000,0000,0001,0000,0000,0000)) == 0 ? 0 : 1;
  outMemoryTypeIsSupportedArrayOf32[13] = (resourceMemoryTypesSupported & REDGPU_B32(0000,0000,0000,0000,0010,0000,0000,0000)) == 0 ? 0 : 1;
  outMemoryTypeIsSupportedArrayOf32[14] = (resourceMemoryTypesSupported & REDGPU_B32(0000,0000,0000,0000,0100,0000,0000,0000)) == 0 ? 0 : 1;
  outMemoryTypeIsSupportedArrayOf32[15] = (resourceMemoryTypesSupported & REDGPU_B32(0000,0000,0000,0000,1000,0000,0000,0000)) == 0 ? 0 : 1;
  outMemoryTypeIsSupportedArrayOf32[16] = (resourceMemoryTypesSupported & REDGPU_B32(0000,0000,0000,0001,0000,0000,0000,0000)) == 0 ? 0 : 1;
  outMemoryTypeIsSupportedArrayOf32[17] = (resourceMemoryTypesSupported & REDGPU_B32(0000,0000,0000,0010,0000,0000,0000,0000)) == 0 ? 0 : 1;
  outMemoryTypeIsSupportedArrayOf32[18] = (resourceMemoryTypesSupported & REDGPU_B32(0000,0000,0000,0100,0000,0000,0000,0000)) == 0 ? 0 : 1;
  outMemoryTypeIsSupportedArrayOf32[19] = (resourceMemoryTypesSupported & REDGPU_B32(0000,0000,0000,1000,0000,0000,0000,0000)) == 0 ? 0 : 1;
  outMemoryTypeIsSupportedArrayOf32[20] = (resourceMemoryTypesSupported & REDGPU_B32(0000,0000,0001,0000,0000,0000,0000,0000)) == 0 ? 0 : 1;
  outMemoryTypeIsSupportedArrayOf32[21] = (resourceMemoryTypesSupported & REDGPU_B32(0000,0000,0010,0000,0000,0000,0000,0000)) == 0 ? 0 : 1;
  outMemoryTypeIsSupportedArrayOf32[22] = (resourceMemoryTypesSupported & REDGPU_B32(0000,0000,0100,0000,0000,0000,0000,0000)) == 0 ? 0 : 1;
  outMemoryTypeIsSupportedArrayOf32[23] = (resourceMemoryTypesSupported & REDGPU_B32(0000,0000,1000,0000,0000,0000,0000,0000)) == 0 ? 0 : 1;
  outMemoryTypeIsSupportedArrayOf32[24] = (resourceMemoryTypesSupported & REDGPU_B32(0000,0001,0000,0000,0000,0000,0000,0000)) == 0 ? 0 : 1;
  outMemoryTypeIsSupportedArrayOf32[25] = (resourceMemoryTypesSupported & REDGPU_B32(0000,0010,0000,0000,0000,0000,0000,0000)) == 0 ? 0 : 1;
  outMemoryTypeIsSupportedArrayOf32[26] = (resourceMemoryTypesSupported & REDGPU_B32(0000,0100,0000,0000,0000,0000,0000,0000)) == 0 ? 0 : 1;
  outMemoryTypeIsSupportedArrayOf32[27] = (resourceMemoryTypesSupported & REDGPU_B32(0000,1000,0000,0000,0000,0000,0000,0000)) == 0 ? 0 : 1;
  outMemoryTypeIsSupportedArrayOf32[28] = (resourceMemoryTypesSupported & REDGPU_B32(0001,0000,0000,0000,0000,0000,0000,0000)) == 0 ? 0 : 1;
  outMemoryTypeIsSupportedArrayOf32[29] = (resourceMemoryTypesSupported & REDGPU_B32(0010,0000,0000,0000,0000,0000,0000,0000)) == 0 ? 0 : 1;
  outMemoryTypeIsSupportedArrayOf32[30] = (resourceMemoryTypesSupported & REDGPU_B32(0100,0000,0000,0000,0000,0000,0000,0000)) == 0 ? 0 : 1;
  outMemoryTypeIsSupportedArrayOf32[31] = (resourceMemoryTypesSupported & REDGPU_B32(1000,0000,0000,0000,0000,0000,0000,0000)) == 0 ? 0 : 1;
}

static unsigned vfeReBARInternalPickSpecificMemoryTypeReBARdGpu(const RedGpuInfo * gpuInfo, const RedArray * array, uint64_t minAllowedBytesCount) {
  unsigned memoryTypesSupported = array->memoryTypesSupported;

  unsigned char arrayMemoryTypeIsSupported[32] = {0};
  vfeReBARInternalFillMemoryTypeIsSupportedArray(memoryTypesSupported, arrayMemoryTypeIsSupported);

  for (unsigned i = 0; i < gpuInfo->memoryTypesCount; i += 1) {
    const RedMemoryType * type = &gpuInfo->memoryTypes[i];
    if (arrayMemoryTypeIsSupported[i] == 1 &&
      type->isCpuMappable == 1 &&
      type->isCpuCoherent == 1 &&
      type->isCpuCached   == 0 &&
      gpuInfo->memoryHeaps[type->memoryHeapIndex].isGpuVram == 1 &&
      gpuInfo->memoryHeaps[type->memoryHeapIndex].memoryBytesCount > 0 &&
      gpuInfo->memoryHeaps[type->memoryHeapIndex].memoryBytesCount >= minAllowedBytesCount)
    {
      return i;
    }
  }

  return -1;
}

static unsigned vfeReBARInternalPickSpecificMemoryTypeReBARiGpu(const RedGpuInfo * gpuInfo, const RedArray * array, uint64_t minAllowedBytesCount) {
  unsigned memoryTypesSupported = array->memoryTypesSupported;

  unsigned char arrayMemoryTypeIsSupported[32] = {0};
  vfeReBARInternalFillMemoryTypeIsSupportedArray(memoryTypesSupported, arrayMemoryTypeIsSupported);

  for (unsigned i = 0; i < gpuInfo->memoryTypesCount; i += 1) {
    const RedMemoryType * type = &gpuInfo->memoryTypes[i];
    if (arrayMemoryTypeIsSupported[i] == 1 &&
      type->isCpuMappable == 1 &&
      type->isCpuCoherent == 1 &&
      type->isCpuCached   == 0 &&
      gpuInfo->memoryHeaps[type->memoryHeapIndex].memoryBytesCount > 0 &&
      gpuInfo->memoryHeaps[type->memoryHeapIndex].memoryBytesCount >= minAllowedBytesCount)
    {
      return i;
    }
  }
  for (unsigned i = 0; i < gpuInfo->memoryTypesCount; i += 1) {
    const RedMemoryType * type = &gpuInfo->memoryTypes[i];
    if (arrayMemoryTypeIsSupported[i] == 1 &&
      type->isCpuMappable == 1 &&
      type->isCpuCoherent == 1 &&
      gpuInfo->memoryHeaps[type->memoryHeapIndex].memoryBytesCount > 0 &&
      gpuInfo->memoryHeaps[type->memoryHeapIndex].memoryBytesCount >= minAllowedBytesCount)
    {
      return i;
    }
  }

  return -1;
}

VFE_REBAR_API_PRE void VFE_REBAR_API_POST vfeReBARGetMemoryBudget(gpu_handle_context_t context, RedMemoryBudget * outMemoryBudget) {
  const char * optionalFile = NULL;
  int optionalLine = 0;

  vf_handle_context_t * vkfast = (vf_handle_context_t *)(void *)context;

  RedMemoryBudget defaults = {0};
  defaults.setTo1000237000 = 1000237000;
  defaults.setTo0          = 0;
  outMemoryBudget[0] = defaults;

  if (vkfast->gpuInfo->supportsMemoryGetBudget == 1) {
    np(redMemoryGetBudget,
      "context", vkfast->context,
      "gpu", vkfast->gpu,
      "outMemoryBudget", outMemoryBudget,
      "outStatuses", NULL,
      "optionalFile", optionalFile,
      "optionalLine", optionalLine,
      "optionalUserData", NULL
    );
  }
}

VFE_REBAR_API_PRE uint64_t VFE_REBAR_API_POST vfeReBARGetMaxMallocSharedSize(gpu_handle_context_t context) {
  vf_handle_context_t * vkfast = (vf_handle_context_t *)(void *)context;
  return (uint64_t)vkfast->gpuInfo->maxArrayRORWStructMemberRangeBytesCount;
}

VFE_REBAR_API_PRE void * VFE_REBAR_API_POST vfeReBARMallocShared(gpu_handle_context_t context, uint64_t bytesCount, VfeReBARMallocShared * outArray) {
  const char * optionalFile = NULL;
  int optionalLine = 0;

  vf_handle_context_t * vkfast = (vf_handle_context_t *)(void *)context;

  RedHandleGpu gpu = vkfast->gpu;

  RedArray array = {0};
  {
    uint64_t maxAllowedOverallocationBytesCount = 0;

    np(redCreateArray,
      "context", vkfast->context,
      "gpu", vkfast->gpu,
      "handleName", NULL,
      "type", RED_ARRAY_TYPE_ARRAY_RW,
      "bytesCount", bytesCount,
      "structuredBufferElementBytesCount", 0,
      "restrictToAccess", 0,
      "initialQueueFamilyIndex", vkfast->gpuInfo->queuesCount > 1 ? -1 : (unsigned)vkfast->gpuInfo->queuesFamilyIndex[vkfast->mainQueueFamilyIndex],
      "dedicate", 0,
      "outArray", &array,
      "outStatuses", NULL,
      "optionalFile", optionalFile,
      "optionalLine", optionalLine,
      "optionalUserData", NULL
    );
    REDGPU_2_EXPECTWG(array.handle != NULL);
    size_t bytesToNextAlignmentBoundary = REDGPU_2_BYTES_TO_NEXT_ALIGNMENT_BOUNDARY(array.memoryBytesCount, array.memoryBytesAlignment);
    REDGPU_2_EXPECTWG(bytesToNextAlignmentBoundary == 0);
    REDGPU_2_EXPECTWG((array.memoryBytesCount - bytesCount) <= maxAllowedOverallocationBytesCount);
  }

  const uint64_t minAllowedBytesCount = 1ULL * 1024*1024*1024; // NOTE(Constantine): Requiring 1 GB of VRAM minimum, to excluse dGPUs with 256 MB ReBARs only, like RTX 20 series and older.
  unsigned pickedMemoryTypeReBAR = -1;
  if (vkfast->gpuInfo->gpuType == RED_GPU_TYPE_DISCRETE) {
    pickedMemoryTypeReBAR = vfeReBARInternalPickSpecificMemoryTypeReBARdGpu(vkfast->gpuInfo, &array, minAllowedBytesCount);
  } else {
    pickedMemoryTypeReBAR = vfeReBARInternalPickSpecificMemoryTypeReBARiGpu(vkfast->gpuInfo, &array, minAllowedBytesCount);
  }
  REDGPU_2_EXPECTWG(!"Memory type pick fail" || (pickedMemoryTypeReBAR != -1));

  RedHandleMemory memory = NULL;
  np(redMemoryAllocate,
    "context", vkfast->context,
    "gpu", vkfast->gpu,
    "handleName", NULL,
    "bytesCount", array.memoryBytesCount,
    "memoryTypeIndex", pickedMemoryTypeReBAR,
    "dedicateToArray", NULL,
    "dedicateToImage", NULL,
    "memoryBitflags", 0,
    "outMemory", &memory,
    "outStatuses", NULL,
    "optionalFile", optionalFile,
    "optionalLine", optionalLine,
    "optionalUserData", NULL
  );
  REDGPU_2_EXPECTWG(memory != NULL);

  RedMemoryArray memoryArray = {0};
  memoryArray.setTo1000157000  = 1000157000;
  memoryArray.setTo0           = 0;
  memoryArray.array            = array.handle;
  memoryArray.memory           = memory;
  memoryArray.memoryBytesFirst = 0;
  RedStatuses opstatuses = {0};
  np(redMemorySet,
    "context", vkfast->context,
    "gpu", vkfast->gpu,
    "memoryArraysCount", 1,
    "memoryArrays", &memoryArray,
    "memoryImagesCount", 0,
    "memoryImages", NULL,
    "outStatuses", &opstatuses,
    "optionalFile", optionalFile,
    "optionalLine", optionalLine,
    "optionalUserData", NULL
  );
  REDGPU_2_EXPECTWG(opstatuses.statusError == RED_STATUS_SUCCESS);

  void * volatilePointerReBAR = NULL;
  np(redMemoryMap,
    "context", vkfast->context,
    "gpu", vkfast->gpu,
    "mappableMemory", memory,
    "mappableMemoryBytesFirst", 0,
    "mappableMemoryBytesCount", bytesCount,
    "outVolatilePointer", &volatilePointerReBAR,
    "outStatuses", NULL,
    "optionalFile", optionalFile,
    "optionalLine", optionalLine,
    "optionalUserData", NULL
  );
  REDGPU_2_EXPECTWG(volatilePointerReBAR != NULL);
  REDGPU_2_EXPECTWG(!"Start address is not aligned" || (0 == REDGPU_2_BYTES_TO_NEXT_ALIGNMENT_BOUNDARY((uint64_t)volatilePointerReBAR, vkfast->gpuInfo->minMemoryAllocateBytesAlignment)));

  REDGPU_2_EXPECTWG(outArray != NULL);
  // Filling
  outArray->array  = array;
  outArray->memory = memory;

  return volatilePointerReBAR;
}

VFE_REBAR_API_PRE void VFE_REBAR_API_POST vfeReBARFreeShared(gpu_handle_context_t context, VfeReBARMallocShared * array) {
  const char * optionalFile = NULL;
  int optionalLine = 0;

  vf_handle_context_t * vkfast = (vf_handle_context_t *)(void *)context;

  if (array->memory != NULL) {
    np(redMemoryUnmap,
      "context", vkfast->context,
      "gpu", vkfast->gpu,
      "mappableMemory", array->memory,
      "optionalFile", optionalFile,
      "optionalLine", optionalLine,
      "optionalUserData", NULL
    );
  }
  np(red2DestroyHandle,
    "context", vkfast->context,
    "gpu", vkfast->gpu,
    "handleType", RED_HANDLE_TYPE_ARRAY,
    "handle", array->array.handle,
    "optionalHandle2", NULL,
    "optionalFile", optionalFile,
    "optionalLine", optionalLine,
    "optionalUserData", NULL
  );
  np(red2DestroyHandle,
    "context", vkfast->context,
    "gpu", vkfast->gpu,
    "handleType", RED_HANDLE_TYPE_MEMORY,
    "handle", array->memory,
    "optionalHandle2", NULL,
    "optionalFile", optionalFile,
    "optionalLine", optionalLine,
    "optionalUserData", NULL
  );
}

#endif // VFE_REBAR_IMPLEMENTATION

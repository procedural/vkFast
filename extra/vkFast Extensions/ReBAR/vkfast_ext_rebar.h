#pragma once

#include "../../../vkfast.h"
#include "../../../vkfast_ex.h"
#include "../../../vkfast_ids.h"

#if defined(_WIN32)
  #if defined(VKFAST_INCLUDE_TERMUX_PATHS)
    #include "/data/data/com.termux/files/home/RedGpuSDK/redgpu_context_from_vk.h"
  #elif defined(VKFAST_INCLUDE_LINUX_PATHS)
    #include "/home/linuxbrew/RedGpuSDK/redgpu_context_from_vk.h"
  #else
    #include "C:/RedGpuSDK/redgpu_context_from_vk.h"
  #endif
#elif defined(__linux__) && !defined(__ANDROID__)
  #include "/home/linuxbrew/RedGpuSDK/redgpu_context_from_vk.h"
#else
  #error Unsupported OS for now
#endif

typedef struct VfeReBARMallocShared {
  RedArray             array;
  RedHandleMemory      memory;
  RedStructMemberArray storageRaw;
} VfeReBARMallocShared;

#if !defined(VFE_REBAR_API_NON_STATIC) && !defined(ALL_API_NON_STATIC)
#define VFE_REBAR_API_PRE  static
#define VFE_REBAR_API_POST
#define VFE_REBAR_IMPLEMENTATION
#endif

npf(VFE_REBAR_API_PRE void     VFE_REBAR_API_POST, vfeReBARGetMemoryBudget,        gpu_handle_context_t,context, RedMemoryBudget *,outMemoryBudget);
npf(VFE_REBAR_API_PRE uint64_t VFE_REBAR_API_POST, vfeReBARGetMaxMallocSharedSize, gpu_handle_context_t,context);
npf(VFE_REBAR_API_PRE void *   VFE_REBAR_API_POST, vfeReBARMallocShared,           gpu_handle_context_t,context, uint64_t,bytesCount, VfeReBARMallocShared *,outArray);
npf(VFE_REBAR_API_PRE void     VFE_REBAR_API_POST, vfeReBARFreeShared,             gpu_handle_context_t,context, VfeReBARMallocShared *,array);

#ifdef VFE_REBAR_IMPLEMENTATION

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
      context, vkfast->context,
      gpu, vkfast->gpu,
      outMemoryBudget, outMemoryBudget,
      outStatuses, NULL,
      optionalFile, optionalFile,
      optionalLine, optionalLine,
      optionalUserData, NULL
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

  REDGPU_2_EXPECTWG(!"vfeReBARMallocShared() expects input bytesCount > 0" || bytesCount > 0);

  uint64_t alignedBytesCount = bytesCount + REDGPU_2_BYTES_TO_NEXT_ALIGNMENT_BOUNDARY(bytesCount, 64); // NOTE(Constantine)(Sep 13, 2026): Intel GPUs prefer 64-byte aligned memory allocations.
  bytesCount = alignedBytesCount;

  RedArray array = {0};
  {
    uint64_t maxAllowedOverallocationBytesCount = 0;

    np(redCreateArray,
      context, vkfast->context,
      gpu, vkfast->gpu,
      handleName, NULL,
      type, RED_ARRAY_TYPE_ARRAY_RW,
      bytesCount, bytesCount,
      structuredBufferElementBytesCount, 0,
      restrictToAccess, 0,
      initialQueueFamilyIndex, vkfast->gpuInfo->queuesCount > 1 ? -1 : (unsigned)vkfast->gpuInfo->queuesFamilyIndex[vkfast->mainQueueFamilyIndex],
      dedicate, 0,
      outArray, &array,
      outStatuses, NULL,
      optionalFile, optionalFile,
      optionalLine, optionalLine,
      optionalUserData, NULL
    );
    REDGPU_2_EXPECTWG(array.handle != NULL);
    size_t bytesToNextAlignmentBoundary = REDGPU_2_BYTES_TO_NEXT_ALIGNMENT_BOUNDARY(array.memoryBytesCount, array.memoryBytesAlignment);
    REDGPU_2_EXPECTWG(bytesToNextAlignmentBoundary == 0);
    REDGPU_2_EXPECTWG((array.memoryBytesCount - bytesCount) <= maxAllowedOverallocationBytesCount);
  }

  const uint64_t minAllowedBytesCount = 200ULL * 1024*1024; // NOTE(Constantine): Requiring 200 MB of ReBAR VRAM minimum.
  unsigned pickedMemoryTypeReBAR = -1;
  if (vkfast->gpuInfo->gpuType == RED_GPU_TYPE_DISCRETE) {
    pickedMemoryTypeReBAR = red2MemoryTypeIndexPickFirstReBARdGpu(vkfast->gpuInfo, array.memoryTypesSupported, minAllowedBytesCount);
  } else {
    pickedMemoryTypeReBAR = red2MemoryTypeIndexPickFirstReBARiGpu(vkfast->gpuInfo, array.memoryTypesSupported, minAllowedBytesCount);
  }
  REDGPU_2_EXPECTWG(!"Memory type pick fail" || (pickedMemoryTypeReBAR != -1));

  RedHandleMemory memory = NULL;
  np(redMemoryAllocate,
    context, vkfast->context,
    gpu, vkfast->gpu,
    handleName, NULL,
    bytesCount, array.memoryBytesCount,
    memoryTypeIndex, pickedMemoryTypeReBAR,
    dedicateToArray, NULL,
    dedicateToImage, NULL,
    memoryBitflags, 0,
    outMemory, &memory,
    outStatuses, NULL,
    optionalFile, optionalFile,
    optionalLine, optionalLine,
    optionalUserData, NULL
  );
  REDGPU_2_EXPECTWG(memory != NULL);

  RedMemoryArray memoryArray = {0};
  memoryArray.setTo1000157000  = 1000157000;
  memoryArray.setTo0           = 0;
  memoryArray.array            = array.handle;
  memoryArray.memory           = memory;
  memoryArray.memoryBytesFirst = 0;
  RedStatuses opstatuses = {(RedStatus)0};
  np(redMemorySet,
    context, vkfast->context,
    gpu, vkfast->gpu,
    memoryArraysCount, 1,
    memoryArrays, &memoryArray,
    memoryImagesCount, 0,
    memoryImages, NULL,
    outStatuses, &opstatuses,
    optionalFile, optionalFile,
    optionalLine, optionalLine,
    optionalUserData, NULL
  );
  REDGPU_2_EXPECTWG(opstatuses.statusError == RED_STATUS_SUCCESS);

  void * volatilePointerReBAR = NULL;
  np(redMemoryMap,
    context, vkfast->context,
    gpu, vkfast->gpu,
    mappableMemory, memory,
    mappableMemoryBytesFirst, 0,
    mappableMemoryBytesCount, bytesCount,
    outVolatilePointer, &volatilePointerReBAR,
    outStatuses, NULL,
    optionalFile, optionalFile,
    optionalLine, optionalLine,
    optionalUserData, NULL
  );
  REDGPU_2_EXPECTWG(volatilePointerReBAR != NULL);

  RedStructMemberArray storageRaw = {0};
  storageRaw.array                = array.handle;
  storageRaw.arrayRangeBytesFirst = 0;
  storageRaw.arrayRangeBytesCount = bytesCount;

  REDGPU_2_EXPECTWG(outArray != NULL);
  // Filling
  // VfeReBARMallocShared;
  outArray->array      = array;
  outArray->memory     = memory;
  outArray->storageRaw = storageRaw;

  return volatilePointerReBAR;
}

VFE_REBAR_API_PRE void VFE_REBAR_API_POST vfeReBARFreeShared(gpu_handle_context_t context, VfeReBARMallocShared * array) {
  const char * optionalFile = NULL;
  int optionalLine = 0;

  vf_handle_context_t * vkfast = (vf_handle_context_t *)(void *)context;

  if (array->memory != NULL) {
    np(redMemoryUnmap,
      context, vkfast->context,
      gpu, vkfast->gpu,
      mappableMemory, array->memory,
      optionalFile, optionalFile,
      optionalLine, optionalLine,
      optionalUserData, NULL
    );
  }
  np(red2DestroyHandle,
    context, vkfast->context,
    gpu, vkfast->gpu,
    handleType, RED_HANDLE_TYPE_ARRAY,
    handle, array->array.handle,
    optionalHandle2, NULL,
    optionalFile, optionalFile,
    optionalLine, optionalLine,
    optionalUserData, NULL
  );
  np(red2DestroyHandle,
    context, vkfast->context,
    gpu, vkfast->gpu,
    handleType, RED_HANDLE_TYPE_MEMORY,
    handle, array->memory,
    optionalHandle2, NULL,
    optionalFile, optionalFile,
    optionalLine, optionalLine,
    optionalUserData, NULL
  );
}

#endif // VFE_REBAR_IMPLEMENTATION

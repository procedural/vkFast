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

#ifndef VFE_REBAR_API_NON_STATIC
  #define VFE_REBAR_API_PRE  static
  #define VFE_REBAR_API_POST
#endif

VFE_REBAR_API_PRE void   VFE_REBAR_API_POST vfeReBARGetMemoryBudget              (gpu_handle_context_t context, RedMemoryBudget * outMemoryBudget);
VFE_REBAR_API_PRE void * VFE_REBAR_API_POST vfeReBARMallocSharedB580ReBARHeap0v1 (gpu_handle_context_t context, uint64_t bytesCount, Red2Array * outArray); // v1 from Sep 10, 2026
VFE_REBAR_API_PRE void * VFE_REBAR_API_POST vfeReBARMallocSharedB580ReBARHeap1v1 (gpu_handle_context_t context, uint64_t bytesCount, Red2Array * outArray); // v1 from Sep 10, 2026
VFE_REBAR_API_PRE void   VFE_REBAR_API_POST vfeReBARFreeSharedB580ReBARHeap      (gpu_handle_context_t context, Red2Array * array);

#ifndef VFE_REBAR_API_NON_STATIC
  #define VFE_REBAR_IMPLEMENTATION
#endif
#ifdef VFE_REBAR_IMPLEMENTATION

VFE_REBAR_API_PRE void VFE_REBAR_API_POST vfeReBARGetMemoryBudget(gpu_handle_context_t context, RedMemoryBudget * outMemoryBudget) {
  const char * optionalFile = NULL;
  int optionalLine = 0;

  vf_handle_context_t * vkfast = (vf_handle_context_t *)(void *)context;

  RedMemoryBudget defaults = {};
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

VFE_REBAR_API_PRE void VFE_REBAR_API_POST vfeReBARFreeSharedB580ReBARHeap(gpu_handle_context_t context, Red2Array * array) {
  const char * optionalFile = NULL;
  int optionalLine = 0;

  vf_handle_context_t * vkfast = (vf_handle_context_t *)(void *)context;

  if (array->handleAllocatedDedicatedOrMappableMemoryOrPickedMemory != NULL) {
    np(redMemoryUnmap,
       "context", vkfast->context,
       "gpu", vkfast->gpu,
       "mappableMemory", array->handleAllocatedDedicatedOrMappableMemoryOrPickedMemory,
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
     "handle", array->handleAllocatedDedicatedOrMappableMemoryOrPickedMemory,
     "optionalHandle2", NULL,
     "optionalFile", optionalFile,
     "optionalLine", optionalLine,
     "optionalUserData", NULL
  );
}

static void * vfeReBARInternalMallocSharedB580ReBARHeapv1(vf_handle_context_t * vkfast, uint64_t bytesCount, unsigned memoryTypeIndexReBAR, Red2Array * outArray) {
  const char * optionalFile = NULL;
  int optionalLine = 0;

  RedHandleGpu gpu = vkfast->gpu;

  np(red2CreateArray,
     "context", vkfast->context,
     "gpu", vkfast->gpu,
     "handleName", NULL, // TODO(Constantine): Pass optional debug name info.
     "type", RED_ARRAY_TYPE_ARRAY_RW,
     "bytesCount", bytesCount,
     "structuredBufferElementBytesCount", 0,
     "restrictToAccess", 0,
     "initialQueueFamilyIndex", vkfast->gpuInfo->queuesCount > 1 ? -1 : (unsigned)vkfast->gpuInfo->queuesFamilyIndex[vkfast->mainQueueFamilyIndex],
     "maxAllowedOverallocationBytesCount", 0, // NOTE(Constantine): Intel's classic 64 bytes alignment?
     "dedicate", 0,
     "mappable", 1,
     "dedicateOrMappableMemoryTypeIndex", memoryTypeIndexReBAR,
     "dedicateOrMappableMemoryBitflags", 0,
     "suballocateFromMemoryOnFirstMatchPointersCount", 0,
     "suballocateFromMemoryOnFirstMatchPointers", NULL,
     "outArray", outArray,
     "outStatuses", NULL,
     "optionalFile", optionalFile,
     "optionalLine", optionalLine,
     "optionalUserData", NULL
  );
  REDGPU_2_EXPECTWG(outArray->array.handle != NULL);
  REDGPU_2_EXPECTWG(outArray->handleAllocatedDedicatedOrMappableMemoryOrPickedMemory != NULL);
  void * volatilePointer = NULL;
  np(redMemoryMap,
     "context", vkfast->context,
     "gpu", vkfast->gpu,
     "mappableMemory", outArray->handleAllocatedDedicatedOrMappableMemoryOrPickedMemory,
     "mappableMemoryBytesFirst", 0,
     "mappableMemoryBytesCount", outArray->array.memoryBytesCount,
     "outVolatilePointer", &volatilePointer,
     "outStatuses", NULL,
     "optionalFile", optionalFile,
     "optionalLine", optionalLine,
     "optionalUserData", NULL
  );
  REDGPU_2_EXPECTWG(volatilePointer != NULL);
  REDGPU_2_EXPECTWG(0 == REDGPU_2_BYTES_TO_NEXT_ALIGNMENT_BOUNDARY((uint64_t)volatilePointer, vkfast->gpuInfo->minMemoryAllocateBytesAlignment)); // NOTE(Constantine): Start address is guaranteed to be aligned.
  return volatilePointer;
}

VFE_REBAR_API_PRE void * VFE_REBAR_API_POST vfeReBARMallocSharedB580ReBARHeap0v1(gpu_handle_context_t context, uint64_t bytesCount, Red2Array * outArray) {
  const char * optionalFile = NULL;
  int optionalLine = 0;

  vf_handle_context_t * vkfast = (vf_handle_context_t *)(void *)context;

  RedHandleGpu gpu = vkfast->gpu;

  REDGPU_2_EXPECTWG(vkfast->gpuInfo->gpuVendorId == 0x8086/*Intel*/);
  REDGPU_2_EXPECTWG(vkfast->gpuInfo->gpuDeviceId == 0xE20B/*Intel Arc B580*/);
  REDGPU_2_EXPECTWG(vkfast->gpuInfo->maxArrayRORWStructMemberRangeBytesCount >= 4294967295 || !"As of Sep 10, 2026, Intel Arc B580 on Linux allows to bind for shaders >= 4 GB at once (-1 byte at the end, lol)");
  REDGPU_2_EXPECTWG(bytesCount <= vkfast->gpuInfo->maxArrayRORWStructMemberRangeBytesCount);

  unsigned      memoryTypesCount = 0;
  RedMemoryType memoryTypes[32]  = {0};
  unsigned      memoryHeapsCount = 0;
  RedMemoryHeap memoryHeaps[32]  = {0};

  memoryTypesCount = 7;
  memoryHeapsCount = 2;

  memoryTypes[0].memoryHeapIndex = 0;
  memoryTypes[0].isGpuVram       = 1;
  memoryTypes[0].isCpuMappable   = 0;
  memoryTypes[0].isCpuCoherent   = 0;
  memoryTypes[0].isCpuCached     = 0;

  memoryTypes[1].memoryHeapIndex = 0;
  memoryTypes[1].isGpuVram       = 1;
  memoryTypes[1].isCpuMappable   = 0;
  memoryTypes[1].isCpuCoherent   = 0;
  memoryTypes[1].isCpuCached     = 0;

  memoryTypes[2].memoryHeapIndex = 1;
  memoryTypes[2].isGpuVram       = 0;
  memoryTypes[2].isCpuMappable   = 1;
  memoryTypes[2].isCpuCoherent   = 1;
  memoryTypes[2].isCpuCached     = 1;

  memoryTypes[3].memoryHeapIndex = 0;
  memoryTypes[3].isGpuVram       = 1;
  memoryTypes[3].isCpuMappable   = 1;
  memoryTypes[3].isCpuCoherent   = 1;
  memoryTypes[3].isCpuCached     = 0;

  memoryTypes[4].memoryHeapIndex = 0;
  memoryTypes[4].isGpuVram       = 1;
  memoryTypes[4].isCpuMappable   = 0;
  memoryTypes[4].isCpuCoherent   = 0;
  memoryTypes[4].isCpuCached     = 0;

  memoryTypes[5].memoryHeapIndex = 1;
  memoryTypes[5].isGpuVram       = 0;
  memoryTypes[5].isCpuMappable   = 1;
  memoryTypes[5].isCpuCoherent   = 1;
  memoryTypes[5].isCpuCached     = 1;

  memoryTypes[6].memoryHeapIndex = 0;
  memoryTypes[6].isGpuVram       = 1;
  memoryTypes[6].isCpuMappable   = 1;
  memoryTypes[6].isCpuCoherent   = 1;
  memoryTypes[6].isCpuCached     = 0;

  memoryHeaps[0].memoryBytesCount = 12000000000;
  memoryHeaps[0].isGpuVram        = 1;

  memoryHeaps[1].memoryBytesCount = 2000000000;
  memoryHeaps[1].isGpuVram        = 0;

  np(red2ExpectMemoryTypes,
     "gpuInfo", vkfast->gpuInfo,
     "expectedMemoryHeapsCount", memoryHeapsCount,
     "expectedMemoryHeaps", memoryHeaps,
     "expectedMemoryTypesCount", memoryTypesCount,
     "expectedMemoryTypes", memoryTypes,
     "optionalFile", optionalFile,
     "optionalLine", optionalLine
  );

  unsigned memoryTypeIndexReBAR = 3;

  return vfeReBARInternalMallocSharedB580ReBARHeapv1(vkfast, bytesCount, memoryTypeIndexReBAR, outArray);
}

VFE_REBAR_API_PRE void * VFE_REBAR_API_POST vfeReBARMallocSharedB580ReBARHeap1v1(gpu_handle_context_t context, uint64_t bytesCount, Red2Array * outArray) {
  const char * optionalFile = NULL;
  int optionalLine = 0;

  vf_handle_context_t * vkfast = (vf_handle_context_t *)(void *)context;

  RedHandleGpu gpu = vkfast->gpu;

  REDGPU_2_EXPECTWG(vkfast->gpuInfo->gpuVendorId == 0x8086/*Intel*/);
  REDGPU_2_EXPECTWG(vkfast->gpuInfo->gpuDeviceId == 0xE20B/*Intel Arc B580*/);
  REDGPU_2_EXPECTWG(vkfast->gpuInfo->maxArrayRORWStructMemberRangeBytesCount >= 4294967295 || !"As of Sep 10, 2026, Intel Arc B580 on Linux allows to bind for shaders >= 4 GB at once (-1 byte at the end, lol)");
  REDGPU_2_EXPECTWG(bytesCount <= vkfast->gpuInfo->maxArrayRORWStructMemberRangeBytesCount);

  unsigned      memoryTypesCount = 0;
  RedMemoryType memoryTypes[32]  = {0};
  unsigned      memoryHeapsCount = 0;
  RedMemoryHeap memoryHeaps[32]  = {0};

  memoryTypesCount = 7;
  memoryHeapsCount = 2;

  memoryTypes[0].memoryHeapIndex = 0;
  memoryTypes[0].isGpuVram       = 1;
  memoryTypes[0].isCpuMappable   = 0;
  memoryTypes[0].isCpuCoherent   = 0;
  memoryTypes[0].isCpuCached     = 0;

  memoryTypes[1].memoryHeapIndex = 0;
  memoryTypes[1].isGpuVram       = 1;
  memoryTypes[1].isCpuMappable   = 0;
  memoryTypes[1].isCpuCoherent   = 0;
  memoryTypes[1].isCpuCached     = 0;

  memoryTypes[2].memoryHeapIndex = 1;
  memoryTypes[2].isGpuVram       = 0;
  memoryTypes[2].isCpuMappable   = 1;
  memoryTypes[2].isCpuCoherent   = 1;
  memoryTypes[2].isCpuCached     = 1;

  memoryTypes[3].memoryHeapIndex = 0;
  memoryTypes[3].isGpuVram       = 1;
  memoryTypes[3].isCpuMappable   = 1;
  memoryTypes[3].isCpuCoherent   = 1;
  memoryTypes[3].isCpuCached     = 0;

  memoryTypes[4].memoryHeapIndex = 0;
  memoryTypes[4].isGpuVram       = 1;
  memoryTypes[4].isCpuMappable   = 0;
  memoryTypes[4].isCpuCoherent   = 0;
  memoryTypes[4].isCpuCached     = 0;

  memoryTypes[5].memoryHeapIndex = 1;
  memoryTypes[5].isGpuVram       = 0;
  memoryTypes[5].isCpuMappable   = 1;
  memoryTypes[5].isCpuCoherent   = 1;
  memoryTypes[5].isCpuCached     = 1;

  memoryTypes[6].memoryHeapIndex = 0;
  memoryTypes[6].isGpuVram       = 1;
  memoryTypes[6].isCpuMappable   = 1;
  memoryTypes[6].isCpuCoherent   = 1;
  memoryTypes[6].isCpuCached     = 0;

  memoryHeaps[0].memoryBytesCount = 12000000000;
  memoryHeaps[0].isGpuVram        = 1;

  memoryHeaps[1].memoryBytesCount = 2000000000;
  memoryHeaps[1].isGpuVram        = 0;

  np(red2ExpectMemoryTypes,
     "gpuInfo", vkfast->gpuInfo,
     "expectedMemoryHeapsCount", memoryHeapsCount,
     "expectedMemoryHeaps", memoryHeaps,
     "expectedMemoryTypesCount", memoryTypesCount,
     "expectedMemoryTypes", memoryTypes,
     "optionalFile", optionalFile,
     "optionalLine", optionalLine
  );

  unsigned memoryTypeIndexReBAR = 6;

  return vfeReBARInternalMallocSharedB580ReBARHeapv1(vkfast, bytesCount, memoryTypeIndexReBAR, outArray);
}

#endif // VFE_REBAR_IMPLEMENTATION

#ifndef __cplusplus
#define REDGPU_DISABLE_NAMED_PARAMETERS
#endif

#include "C:/RedGpuSDK/redgpu_2.h"
#include "C:/RedGpuSDK/redgpu_32.h"

#include "C:/RedGpuSDK/misc/np/np.h"
#include "C:/RedGpuSDK/misc/np/np_redgpu.h"
#include "C:/RedGpuSDK/misc/np/np_redgpu_2.h"

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

int main() {
  RedContext context = NULL;
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
    "outContext", &context,
    "outStatuses", NULL,
    "optionalFile", __FILE__,
    "optionalLine", __LINE__,
    "optionalUserData", NULL
  );

  REDGPU_2_EXPECTFL(context != NULL);
  REDGPU_2_EXPECTFL(context->gpusCount > 0);

  const RedGpuInfo * gpuInfo = &context->gpus[0];

  if (gpuInfo->gpuVendorId == 4318/*NVIDIA*/) {
    np(red2ExpectMinimumGuarantees,
      "gpuInfo", gpuInfo,
      "optionalFile", __FILE__,
      "optionalLine", __LINE__
    );
  } else if (gpuInfo->gpuVendorId == 32902/*IntelUHDGraphics*/) {
    np(red2ExpectMinimumGuaranteesIntelUHDGraphics730,
      "gpuInfo", gpuInfo,
      "optionalFile", __FILE__,
      "optionalLine", __LINE__
    );
  } else if (gpuInfo->gpuVendorId == 4098/*AMDRadeon*/) {
    np(red2ExpectMinimumGuaranteesIntelUHDGraphics730,
      "gpuInfo", gpuInfo,
      "optionalFile", __FILE__,
      "optionalLine", __LINE__
    );
  } else {
    int unsupported_by_this_example_code_gpu_vendor = 0;
    REDGPU_2_EXPECTFL(unsupported_by_this_example_code_gpu_vendor);
  }
  np(red2ExpectAllMemoryToBeCoherent,
    "gpuInfo", gpuInfo,
    "optionalFile", __FILE__,
    "optionalLine", __LINE__
  );
  np(red2ExpectMinimumImageFormatsLimitsAndFeatures, 
    "gpuInfo", gpuInfo,
    "optionalFile", __FILE__,
    "optionalLine", __LINE__
  );

  RedHandleGpu   gpu                  = gpuInfo->gpu;
  const unsigned mainQueueFamilyIndex = gpuInfo->queuesFamilyIndex[0];
  RedHandleQueue mainQueue            = gpuInfo->queues[0];

  unsigned specificMemoryTypeCPUVisibleVRAM = 0;
  unsigned specificMemoryTypeReadback       = 0;
  if (gpuInfo->gpuVendorId == 4318/*NVIDIA*/) { // Tested on RTX 2060 and Windows 10.
    unsigned      memoryTypesCount = 0;
    RedMemoryType memoryTypes[32]  = {0};
    unsigned      memoryHeapsCount = 0;
    RedMemoryHeap memoryHeaps[16]  = {0};

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
      "optionalFile", __FILE__,
      "optionalLine", __LINE__
    );

    specificMemoryTypeCPUVisibleVRAM = 5;
    specificMemoryTypeReadback       = 4; // The cpu cached one
  } else if (gpuInfo->gpuVendorId == 32902/*IntelUHDGraphics*/) {
    unsigned      memoryTypesCount = 0;
    RedMemoryType memoryTypes[32]  = {0};
    unsigned      memoryHeapsCount = 0;
    RedMemoryHeap memoryHeaps[16]  = {0};

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
      "optionalFile", __FILE__,
      "optionalLine", __LINE__
    );

    specificMemoryTypeCPUVisibleVRAM = 1;
    specificMemoryTypeReadback       = 2; // The cpu cached one
  } else if (gpuInfo->gpuVendorId == 4098/*AMDRadeon*/) {
    unsigned      memoryTypesCount = 0;
    RedMemoryType memoryTypes[32]  = {0};
    unsigned      memoryHeapsCount = 0;
    RedMemoryHeap memoryHeaps[32]  = {0};

    memoryTypesCount = 4;
    memoryHeapsCount = 3;

    memoryTypes[0].memoryHeapIndex = 0;
    memoryTypes[0].isGpuVram       = 1;
    memoryTypes[0].isCpuMappable   = 0;
    memoryTypes[0].isCpuCoherent   = 0;
    memoryTypes[0].isCpuCached     = 0;

    memoryTypes[1].memoryHeapIndex = 1;
    memoryTypes[1].isGpuVram       = 0;
    memoryTypes[1].isCpuMappable   = 1;
    memoryTypes[1].isCpuCoherent   = 1;
    memoryTypes[1].isCpuCached     = 0;

    memoryTypes[2].memoryHeapIndex = 2;
    memoryTypes[2].isGpuVram       = 1;
    memoryTypes[2].isCpuMappable   = 1;
    memoryTypes[2].isCpuCoherent   = 1;
    memoryTypes[2].isCpuCached     = 0;

    memoryTypes[3].memoryHeapIndex = 1;
    memoryTypes[3].isGpuVram       = 0;
    memoryTypes[3].isCpuMappable   = 1;
    memoryTypes[3].isCpuCoherent   = 1;
    memoryTypes[3].isCpuCached     = 1;

    memoryHeaps[0].memoryBytesCount = 1800000000;
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
      "optionalFile", __FILE__,
      "optionalLine", __LINE__
    );

    specificMemoryTypeCPUVisibleVRAM = 2;
    specificMemoryTypeReadback       = 3; // NOTE(Constantine): The cpu cached one.
  } else {
    int unsupported_by_this_example_code_gpu_vendor = 0;
    REDGPU_2_EXPECTFL(unsupported_by_this_example_code_gpu_vendor);
  }

  struct HandlesToDestroy {
    RedHandleType destroyHandleType;
    void *        destroyHandle;
    void *        destroyHandle2;
  };

  struct HandlesToDestroyArray {
    struct HandlesToDestroy * items;
    size_t                    count;
    size_t                    capacity;
    size_t                    alignment;
  };
  
  struct HandlesToDestroyArray handlesToDestroy = {0};

  struct float4 {
    float x, y, z, w;
  };

  Red2Array array0 = {0};
  Red2Array array1 = {0};
  Red2Array array2 = {0};

  np(red2CreateArray,
    "context", context,
    "gpu", gpu,
    "handleName", "array0",
    "type", RED_ARRAY_TYPE_ARRAY_RW,
    "bytesCount", sizeof(struct float4),
    "structuredBufferElementBytesCount", sizeof(unsigned),
    "restrictToAccess", 0,
    "initialQueueFamilyIndex", mainQueueFamilyIndex,
    "maxAllowedOverallocationBytesCount", REDGPU_2_BYTES_TO_NEXT_ALIGNMENT_BOUNDARY(sizeof(struct float4), 64), // Intel UHD Graphics 730 on Windows 10 aligns to 64 bytes
    "dedicate", 0,
    "mappable", 1,
    "dedicateOrMappableMemoryTypeIndex", specificMemoryTypeCPUVisibleVRAM,
    "dedicateOrMappableMemoryBitflags", 0,
    "suballocateFromMemoryOnFirstMatchPointersCount", 0,
    "suballocateFromMemoryOnFirstMatchPointers", NULL,
    "outArray", &array0,
    "outStatuses", NULL,
    "optionalFile", __FILE__,
    "optionalLine", __LINE__,
    "optionalUserData", NULL
  );
  REDGPU_32_DYNAMIC_ARRAY_APPEND(handlesToDestroy, REDGPU_32_STRUCT(HandlesToDestroy, RED_HANDLE_TYPE_ARRAY,  array0.array.handle));
  REDGPU_32_DYNAMIC_ARRAY_APPEND(handlesToDestroy, REDGPU_32_STRUCT(HandlesToDestroy, RED_HANDLE_TYPE_MEMORY, array0.handleAllocatedDedicatedOrMappableMemoryOrPickedMemory));

  np(red2CreateArray,
    "context", context,
    "gpu", gpu,
    "handleName", "array1",
    "type", RED_ARRAY_TYPE_ARRAY_RW,
    "bytesCount", sizeof(struct float4),
    "structuredBufferElementBytesCount", sizeof(unsigned),
    "restrictToAccess", 0,
    "initialQueueFamilyIndex", mainQueueFamilyIndex,
    "maxAllowedOverallocationBytesCount", REDGPU_2_BYTES_TO_NEXT_ALIGNMENT_BOUNDARY(sizeof(struct float4), 64), // Intel UHD Graphics 730 on Windows 10 aligns to 64 bytes
    "dedicate", 0,
    "mappable", 1,
    "dedicateOrMappableMemoryTypeIndex", specificMemoryTypeCPUVisibleVRAM,
    "dedicateOrMappableMemoryBitflags", 0,
    "suballocateFromMemoryOnFirstMatchPointersCount", 0,
    "suballocateFromMemoryOnFirstMatchPointers", NULL,
    "outArray", &array1,
    "outStatuses", NULL,
    "optionalFile", __FILE__,
    "optionalLine", __LINE__,
    "optionalUserData", NULL
  );
  REDGPU_32_DYNAMIC_ARRAY_APPEND(handlesToDestroy, REDGPU_32_STRUCT(HandlesToDestroy, RED_HANDLE_TYPE_ARRAY,  array1.array.handle));
  REDGPU_32_DYNAMIC_ARRAY_APPEND(handlesToDestroy, REDGPU_32_STRUCT(HandlesToDestroy, RED_HANDLE_TYPE_MEMORY, array1.handleAllocatedDedicatedOrMappableMemoryOrPickedMemory));

  np(red2CreateArray,
    "context", context,
    "gpu", gpu,
    "handleName", "array2",
    "type", RED_ARRAY_TYPE_ARRAY_RW,
    "bytesCount", sizeof(struct float4),
    "structuredBufferElementBytesCount", sizeof(unsigned),
    "restrictToAccess", 0,
    "initialQueueFamilyIndex", mainQueueFamilyIndex,
    "maxAllowedOverallocationBytesCount", REDGPU_2_BYTES_TO_NEXT_ALIGNMENT_BOUNDARY(sizeof(struct float4), 64), // Intel UHD Graphics 730 on Windows 10 aligns to 64 bytes
    "dedicate", 0,
    "mappable", 1,
    "dedicateOrMappableMemoryTypeIndex", specificMemoryTypeReadback,
    "dedicateOrMappableMemoryBitflags", 0,
    "suballocateFromMemoryOnFirstMatchPointersCount", 0,
    "suballocateFromMemoryOnFirstMatchPointers", NULL,
    "outArray", &array2,
    "outStatuses", NULL,
    "optionalFile", __FILE__,
    "optionalLine", __LINE__,
    "optionalUserData", NULL
  );
  REDGPU_32_DYNAMIC_ARRAY_APPEND(handlesToDestroy, REDGPU_32_STRUCT(HandlesToDestroy, RED_HANDLE_TYPE_ARRAY,  array2.array.handle));
  REDGPU_32_DYNAMIC_ARRAY_APPEND(handlesToDestroy, REDGPU_32_STRUCT(HandlesToDestroy, RED_HANDLE_TYPE_MEMORY, array2.handleAllocatedDedicatedOrMappableMemoryOrPickedMemory));

  volatile struct float4 * array0p = NULL;
  volatile struct float4 * array1p = NULL;
  volatile struct float4 * array2p = NULL;

  {
    np(redMemoryMap,
      "context", context,
      "gpu", gpu,
      "mappableMemory", array0.handleAllocatedDedicatedOrMappableMemoryOrPickedMemory,
      "mappableMemoryBytesFirst", 0,
      "mappableMemoryBytesCount", sizeof(struct float4),
      "outVolatilePointer", (void **)&array0p,
      "outStatuses", NULL,
      "optionalFile", __FILE__,
      "optionalLine", __LINE__,
      "optionalUserData", NULL
    );

    array0p[0].x = 4;
    array0p[0].y = 8;
    array0p[0].z = 15;
    array0p[0].w = 16;

    np(redMemoryUnmap,
      "context", context,
      "gpu", gpu,
      "mappableMemory", array0.handleAllocatedDedicatedOrMappableMemoryOrPickedMemory,
      "optionalFile", __FILE__,
      "optionalLine", __LINE__,
      "optionalUserData", NULL
    );

    array0p = NULL;
  }

  {
    np(redMemoryMap,
      "context", context,
      "gpu", gpu,
      "mappableMemory", array1.handleAllocatedDedicatedOrMappableMemoryOrPickedMemory,
      "mappableMemoryBytesFirst", 0,
      "mappableMemoryBytesCount", sizeof(struct float4),
      "outVolatilePointer", (void **)&array1p,
      "outStatuses", NULL,
      "optionalFile", __FILE__,
      "optionalLine", __LINE__,
      "optionalUserData", NULL
    );

    array1p[0].x = 16;
    array1p[0].y = 23;
    array1p[0].z = 42;
    array1p[0].w = 108;

    np(redMemoryUnmap,
      "context", context,
      "gpu", gpu,
      "mappableMemory", array1.handleAllocatedDedicatedOrMappableMemoryOrPickedMemory,
      "optionalFile", __FILE__,
      "optionalLine", __LINE__,
      "optionalUserData", NULL
    );

    array1p = NULL;
  }

  RedHandleStructsMemory structsMemory = NULL;
  np(redStructsMemoryAllocate,
    "context", context,
    "gpu", gpu,
    "handleName", "structsMemory",
    "maxStructsCount", 1,
    "maxStructsMembersOfTypeArrayROConstantCount", 0,
    "maxStructsMembersOfTypeArrayROOrArrayRWCount", 3,
    "maxStructsMembersOfTypeTextureROCount", 0,
    "maxStructsMembersOfTypeTextureRWCount", 0,
    "outStructsMemory", &structsMemory,
    "outStatuses", NULL,
    "optionalFile", __FILE__,
    "optionalLine", __LINE__,
    "optionalUserData", NULL
  );
  REDGPU_32_DYNAMIC_ARRAY_APPEND(handlesToDestroy, REDGPU_32_STRUCT(HandlesToDestroy, RED_HANDLE_TYPE_STRUCTS_MEMORY, structsMemory));

  RedStructDeclarationMember structDeclarationMembers[3] = {0};

  structDeclarationMembers[0].slot            = 0;
  structDeclarationMembers[0].type            = RED_STRUCT_MEMBER_TYPE_ARRAY_RO_RW;
  structDeclarationMembers[0].count           = 1;
  structDeclarationMembers[0].visibleToStages = RED_VISIBLE_TO_STAGE_BITFLAG_COMPUTE;
  structDeclarationMembers[0].inlineSampler   = NULL;

  structDeclarationMembers[1].slot            = 1;
  structDeclarationMembers[1].type            = RED_STRUCT_MEMBER_TYPE_ARRAY_RO_RW;
  structDeclarationMembers[1].count           = 1;
  structDeclarationMembers[1].visibleToStages = RED_VISIBLE_TO_STAGE_BITFLAG_COMPUTE;
  structDeclarationMembers[1].inlineSampler   = NULL;

  structDeclarationMembers[2].slot            = 2;
  structDeclarationMembers[2].type            = RED_STRUCT_MEMBER_TYPE_ARRAY_RO_RW;
  structDeclarationMembers[2].count           = 1;
  structDeclarationMembers[2].visibleToStages = RED_VISIBLE_TO_STAGE_BITFLAG_COMPUTE;
  structDeclarationMembers[2].inlineSampler   = NULL;

  Red2Struct structure = {0};
  np(red2StructsMemorySuballocateStruct,
    "context", context,
    "gpu", gpu,
    "handleName", "struct",
    "structsMemory", structsMemory,
    "structDeclarationMembersCount", sizeof(structDeclarationMembers) / sizeof(structDeclarationMembers[0]),
    "structDeclarationMembers", structDeclarationMembers,
    "structDeclarationMembersArrayROCount", 0,
    "structDeclarationMembersArrayRO", NULL,
    "outStruct", &structure,
    "outStatuses", NULL,
    "optionalFile", __FILE__,
    "optionalLine", __LINE__,
    "optionalUserData", NULL
  );
  REDGPU_32_DYNAMIC_ARRAY_APPEND(handlesToDestroy, REDGPU_32_STRUCT(HandlesToDestroy, RED_HANDLE_TYPE_STRUCT_DECLARATION, structure.handleDeclaration));

  {
    RedStructMember      structMembers[3]      = {0};
    RedStructMemberArray structMembersArray[3] = {0};

    structMembersArray[0].array                = array0.array.handle;
    structMembersArray[0].arrayRangeBytesFirst = 0;
    structMembersArray[0].arrayRangeBytesCount = array0.array.memoryBytesCount;
    structMembers[0].setTo35   = 35;
    structMembers[0].setTo0    = 0;
    structMembers[0].structure = structure.handle;
    structMembers[0].slot      = 0;
    structMembers[0].first     = 0;
    structMembers[0].count     = 1;
    structMembers[0].type      = RED_STRUCT_MEMBER_TYPE_ARRAY_RO_RW;
    structMembers[0].textures  = NULL;
    structMembers[0].arrays    = &structMembersArray[0];
    structMembers[0].setTo00   = 0;

    structMembersArray[1].array                = array1.array.handle;
    structMembersArray[1].arrayRangeBytesFirst = 0;
    structMembersArray[1].arrayRangeBytesCount = array1.array.memoryBytesCount;
    structMembers[1].setTo35   = 35;
    structMembers[1].setTo0    = 0;
    structMembers[1].structure = structure.handle;
    structMembers[1].slot      = 1;
    structMembers[1].first     = 0;
    structMembers[1].count     = 1;
    structMembers[1].type      = RED_STRUCT_MEMBER_TYPE_ARRAY_RO_RW;
    structMembers[1].textures  = NULL;
    structMembers[1].arrays    = &structMembersArray[1];
    structMembers[1].setTo00   = 0;

    structMembersArray[2].array                = array2.array.handle;
    structMembersArray[2].arrayRangeBytesFirst = 0;
    structMembersArray[2].arrayRangeBytesCount = array2.array.memoryBytesCount;
    structMembers[2].setTo35   = 35;
    structMembers[2].setTo0    = 0;
    structMembers[2].structure = structure.handle;
    structMembers[2].slot      = 2;
    structMembers[2].first     = 0;
    structMembers[2].count     = 1;
    structMembers[2].type      = RED_STRUCT_MEMBER_TYPE_ARRAY_RO_RW;
    structMembers[2].textures  = NULL;
    structMembers[2].arrays    = &structMembersArray[2];
    structMembers[2].setTo00   = 0;

    np(redStructsSet,
      "context", context,
      "gpu", gpu,
      "structsMembersCount", sizeof(structMembers) / sizeof(structMembers[0]),
      "structsMembers", structMembers,
      "optionalFile", __FILE__,
      "optionalLine", __LINE__,
      "optionalUserData", NULL
    );
  }

  Red2ProcedureParametersAndDeclarations procedureParametersAdd = {0};
  {
    Red2ProcedureParametersDeclaration procedureParametersDeclaration = {0};
    procedureParametersDeclaration.variablesSlot            = 0;
    procedureParametersDeclaration.variablesVisibleToStages = 0;
    procedureParametersDeclaration.variablesBytesCount      = 0;
    procedureParametersDeclaration.structsDeclarationsCount = 1;
    procedureParametersDeclaration.structsDeclarations[0].structDeclarationMembersCount        = sizeof(structDeclarationMembers) / sizeof(structDeclarationMembers[0]);
    procedureParametersDeclaration.structsDeclarations[0].structDeclarationMembers             = structDeclarationMembers;
    procedureParametersDeclaration.structsDeclarations[0].structDeclarationMembersArrayROCount = 0;
    procedureParametersDeclaration.structsDeclarations[0].structDeclarationMembersArrayRO      = NULL;
    procedureParametersDeclaration.handlesDeclaration       = NULL;
    np(red2CreateProcedureParameters,
      "context", context,
      "gpu", gpu,
      "handleName", "procedureParameters",
      "procedureParametersDeclaration", &procedureParametersDeclaration,
      "outProcedureParametersAndDeclarations", &procedureParametersAdd,
      "outStatuses", NULL,
      "optionalFile", __FILE__,
      "optionalLine", __LINE__,
      "optionalUserData", NULL
    );
  }
  REDGPU_32_DYNAMIC_ARRAY_APPEND(handlesToDestroy, REDGPU_32_STRUCT(HandlesToDestroy, RED_HANDLE_TYPE_PROCEDURE_PARAMETERS, procedureParametersAdd.procedureParameters));
  for (int i = 0; i < 7; i += 1) {
    REDGPU_32_DYNAMIC_ARRAY_APPEND(handlesToDestroy, REDGPU_32_STRUCT(HandlesToDestroy, RED_HANDLE_TYPE_STRUCT_DECLARATION, procedureParametersAdd.structsDeclarations[i]));
  }
  REDGPU_32_DYNAMIC_ARRAY_APPEND(handlesToDestroy, REDGPU_32_STRUCT(HandlesToDestroy, RED_HANDLE_TYPE_STRUCT_DECLARATION, procedureParametersAdd.handlesDeclaration));

  RedHandleGpuCode gpuCodeAdd = NULL;
  {
    #include "add.cs.h"
    np(redCreateGpuCode,
      "context", context,
      "gpu", gpu,
      "handleName", "gpuCodeAdd",
      "irBytesCount", sizeof(g_main),
      "ir", g_main,
      "outGpuCode", &gpuCodeAdd,
      "outStatuses", NULL,
      "optionalFile", __FILE__,
      "optionalLine", __LINE__,
      "optionalUserData", NULL
    );
  }
  REDGPU_32_DYNAMIC_ARRAY_APPEND(handlesToDestroy, REDGPU_32_STRUCT(HandlesToDestroy, RED_HANDLE_TYPE_GPU_CODE, gpuCodeAdd));

  RedHandleProcedure procedureAdd = NULL;
  np(redCreateProcedureCompute,
    "context", context,
    "gpu", gpu,
    "handleName", "procedureAdd",
    "procedureCache", NULL,
    "procedureParameters", procedureParametersAdd.procedureParameters,
    "gpuCodeMainProcedureName", "main",
    "gpuCode", gpuCodeAdd,
    "outProcedure", &procedureAdd,
    "outStatuses", NULL,
    "optionalFile", __FILE__,
    "optionalLine", __LINE__,
    "optionalUserData", NULL
  );
  REDGPU_32_DYNAMIC_ARRAY_APPEND(handlesToDestroy, REDGPU_32_STRUCT(HandlesToDestroy, RED_HANDLE_TYPE_PROCEDURE, procedureAdd));

  Red2Calls calls = {0};
  Red2Output mutableOutputs[16] = {0};
  // Filling
  Red2CallsMutableOutputsArray;
  calls.mutableOutputsArray.items     = &mutableOutputs[0];
  calls.mutableOutputsArray.count     = 0;
  calls.mutableOutputsArray.capacity  = 16;
  calls.mutableOutputsArray.alignment = 1;
  np(redCreateCallsReusable,
    "context", context,
    "gpu", gpu,
    "handleName", "calls",
    "queueFamilyIndex", mainQueueFamilyIndex,
    "outCalls", &calls.calls,
    "outStatuses", NULL,
    "optionalFile", __FILE__,
    "optionalLine", __LINE__,
    "optionalUserData", NULL
  );
  REDGPU_32_DYNAMIC_ARRAY_APPEND(handlesToDestroy, REDGPU_32_STRUCT(HandlesToDestroy, RED_HANDLE_TYPE_CALLS, calls.calls.handle, calls.calls.memory));

  RedCallProceduresAndAddresses callpa = {0};
  np(redGetCallProceduresAndAddresses,
    "context", context,
    "gpu", gpu,
    "outCallProceduresAndAddresses", &callpa,
    "outStatuses", NULL,
    "optionalFile", __FILE__,
    "optionalLine", __LINE__,
    "optionalUserData", NULL
  );

  np(red2CallsSet,
    "context", context,
    "gpu", gpu,
    "calls", calls.calls.handle,
    "callsMemory", calls.calls.memory,
    "callsReusable", calls.calls.reusable,
    "mutableOutputsArray", &calls.mutableOutputsArray,
    "outStatuses", NULL,
    "optionalFile", __FILE__,
    "optionalLine", __LINE__,
    "optionalUserData", NULL
  );
  np(redCallSetStructsMemory,
    "address", callpa.redCallSetStructsMemory,
    "calls", calls.calls.handle,
    "structsMemory", structsMemory,
    "structsMemorySamplers", 0
  );
  np(redCallSetProcedureParameters,
    "address", callpa.redCallSetProcedureParameters,
    "calls", calls.calls.handle,
    "procedureType", RED_PROCEDURE_TYPE_COMPUTE,
    "procedureParameters", procedureParametersAdd.procedureParameters
  );
  npfp(redCallSetProcedureParametersStructs, callpa.redCallSetProcedureParametersStructs,
    "calls", calls.calls.handle,
    "procedureType", RED_PROCEDURE_TYPE_COMPUTE,
    "procedureParameters", procedureParametersAdd.procedureParameters,
    "procedureParametersDeclarationStructsDeclarationsFirst", 0,
    "structsCount", 1,
    "structs", &structure.handle,
    "setTo0", 0,
    "setTo00", 0
  );
  npfp(redCallSetProcedure, callpa.redCallSetProcedure,
    "calls", calls.calls.handle,
    "procedureType", RED_PROCEDURE_TYPE_COMPUTE,
    "procedure", procedureAdd
  );
  npfp(redCallProcedureCompute, callpa.redCallProcedureCompute,
    "calls", calls.calls.handle,
    "workgroupsCountX", 1,
    "workgroupsCountY", 1,
    "workgroupsCountZ", 1
  );
  np(red2CallGlobalReadbackBarrier,
    "address", callpa.redCallUsageAliasOrderBarrier,
    "calls", calls.calls.handle
  );
  np(redCallsEnd,
    "context", context,
    "gpu", gpu,
    "calls", calls.calls.handle,
    "callsMemory", calls.calls.memory,
    "outStatuses", NULL,
    "optionalFile", __FILE__,
    "optionalLine", __LINE__,
    "optionalUserData", NULL
  );

  RedHandleCpuSignal cpuSignal = 0;
  np(redCreateCpuSignal,
    "context", context,
    "gpu", gpu,
    "handleName", "cpuSignal",
    "createSignaled", 0,
    "outCpuSignal", &cpuSignal,
    "outStatuses", NULL,
    "optionalFile", __FILE__,
    "optionalLine", __LINE__,
    "optionalUserData", NULL
  );
  REDGPU_32_DYNAMIC_ARRAY_APPEND(handlesToDestroy, REDGPU_32_STRUCT(HandlesToDestroy, RED_HANDLE_TYPE_CPU_SIGNAL, cpuSignal));

  RedGpuTimeline timeline = {0};
  timeline.setTo4                            = 4;
  timeline.setTo0                            = 0;
  timeline.waitForAndUnsignalGpuSignalsCount = 0;
  timeline.waitForAndUnsignalGpuSignals      = 0;
  timeline.setTo65536                        = 0;
  timeline.callsCount                        = 1;
  timeline.calls                             = &calls.calls.handle;
  timeline.signalGpuSignalsCount             = 0;
  timeline.signalGpuSignals                  = 0;
  np(redQueueSubmit,
    "context", context,
    "gpu", gpu,
    "queue", mainQueue,
    "timelinesCount", 1,
    "timelines", &timeline,
    "signalCpuSignal", cpuSignal,
    "outStatuses", NULL,
    "optionalFile", __FILE__,
    "optionalLine", __LINE__,
    "optionalUserData", NULL
  );
  np(redCpuSignalWait,
    "context", context,
    "gpu", gpu,
    "cpuSignalsCount", 1,
    "cpuSignals", &cpuSignal,
    "waitAll", 1,
    "outStatuses", NULL,
    "optionalFile", __FILE__,
    "optionalLine", __LINE__,
    "optionalUserData", NULL
  );
  np(redCpuSignalUnsignal,
    "context", context,
    "gpu", gpu,
    "cpuSignalsCount", 1,
    "cpuSignals", &cpuSignal,
    "outStatuses", NULL,
    "optionalFile", __FILE__,
    "optionalLine", __LINE__,
    "optionalUserData", NULL
  );

  struct StringArray {
    char * items;
    size_t count;
    size_t capacity;
    size_t alignment;
  };
  struct StringArray printString = {0};
  {
    np(redMemoryMap,
      "context", context,
      "gpu", gpu,
      "mappableMemory", array2.handleAllocatedDedicatedOrMappableMemoryOrPickedMemory,
      "mappableMemoryBytesFirst", 0,
      "mappableMemoryBytesCount", sizeof(struct float4),
      "outVolatilePointer", (void **)&array2p,
      "outStatuses", NULL,
      "optionalFile", __FILE__,
      "optionalLine", __LINE__,
      "optionalUserData", NULL
    );

    char strX[512] = {0};
    char strY[512] = {0};
    char strZ[512] = {0};
    char strW[512] = {0};
    red32FloatToChars(array2p[0].x, strX);
    red32FloatToChars(array2p[0].y, strY);
    red32FloatToChars(array2p[0].z, strZ);
    red32FloatToChars(array2p[0].w, strW);

    // NOTE(Constantine): "array2p: 20 31 57 124"
    REDGPU_32_DYNAMIC_ARRAY_STRING_JOIN(printString, "array2p: ");
    REDGPU_32_DYNAMIC_ARRAY_STRING_JOIN(printString, strX);
    REDGPU_32_DYNAMIC_ARRAY_STRING_JOIN(printString, " ");
    REDGPU_32_DYNAMIC_ARRAY_STRING_JOIN(printString, strY);
    REDGPU_32_DYNAMIC_ARRAY_STRING_JOIN(printString, " ");
    REDGPU_32_DYNAMIC_ARRAY_STRING_JOIN(printString, strZ);
    REDGPU_32_DYNAMIC_ARRAY_STRING_JOIN(printString, " ");
    REDGPU_32_DYNAMIC_ARRAY_STRING_JOIN(printString, strW);
    REDGPU_32_DYNAMIC_ARRAY_STRING_JOIN(printString, "\n");
    red32ConsolePrint(printString.items);

    np(redMemoryUnmap,
      "context", context,
      "gpu", gpu,
      "mappableMemory", array2.handleAllocatedDedicatedOrMappableMemoryOrPickedMemory,
      "optionalFile", __FILE__,
      "optionalLine", __LINE__,
      "optionalUserData", NULL
    );

    array2p = NULL;
  }

  for (int64_t i = handlesToDestroy.count-1; i >= 0; i -= 1) {
    red2DestroyHandle(context, gpu, handlesToDestroy.items[i].destroyHandleType, handlesToDestroy.items[i].destroyHandle, handlesToDestroy.items[i].destroyHandle2, __FILE__, __LINE__, NULL);
  }
  redDestroyContext(context, __FILE__, __LINE__, NULL);

  REDGPU_32_DYNAMIC_ARRAY_FREE(printString);
  REDGPU_32_DYNAMIC_ARRAY_FREE(handlesToDestroy);
}

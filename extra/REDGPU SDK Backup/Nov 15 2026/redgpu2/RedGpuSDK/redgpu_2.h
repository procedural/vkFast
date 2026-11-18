#pragma once

#include "redgpu.h"

#ifdef __cplusplus
extern "C" {
#endif

// Expected minimum guarantees

#define REDGPU_2_EXPECTED_maxMemoryAllocateCount_4096                        (4096)
#define REDGPU_2_EXPECTED_minMemoryAllocateBytesAlignment_4096               (4096)
#define REDGPU_2_EXPECTED_maxMemoryAllocateBytesCount_1073741824             (1073741824)
#define REDGPU_2_EXPECTED_minMemoryNonCoherentBlockBytesCount_256            (256)
#define REDGPU_2_EXPECTED_maxCreateSamplerCount_4000                         (4000)
#define REDGPU_2_EXPECTED_minMemoryPageSeparationArrayImageBytesCount_65536  (65536)
#define REDGPU_2_EXPECTED_minArrayROCStructMemberRangeBytesAlignment_256     (256)
#define REDGPU_2_EXPECTED_maxArrayROCStructMemberRangeBytesCount_65536       (65536)
#define REDGPU_2_EXPECTED_minArrayRORWStructMemberRangeBytesAlignment_64     (64)
#define REDGPU_2_EXPECTED_maxArrayRORWStructMemberRangeBytesCount_1073741820 (1073741820)
#define REDGPU_2_EXPECTED_maxImageDimensions1D_16384                         (16384)
#define REDGPU_2_EXPECTED_maxImageDimensions2D_16384                         (16384)
#define REDGPU_2_EXPECTED_maxImageDimensions3D_2048                          (2048)
#define REDGPU_2_EXPECTED_maxImageDimensionsCube_16384                       (16384)
#define REDGPU_2_EXPECTED_maxImageLayersCount_2048                           (2048)
#define REDGPU_2_EXPECTED_maxSamplerMipLodBias_15                            (15)
#define REDGPU_2_EXPECTED_maxSamplerAnisotropy_16                            (16)
#define REDGPU_2_EXPECTED_precisionBitsSamplerFilteringMagMin_8              (8)
#define REDGPU_2_EXPECTED_precisionBitsSamplerFilteringMip_8                 (8)
#define REDGPU_2_EXPECTED_maxOutputWidth_16384                               (16384)
#define REDGPU_2_EXPECTED_maxOutputHeight_16384                              (16384)
#define REDGPU_2_EXPECTED_maxOutputColorsCount_8                             (8)
#define REDGPU_2_EXPECTED_maxStructSlotsCount_1024                           (1024)
#define REDGPU_2_EXPECTED_maxStructPerStageArrayROCsCount_12                 (12)
#define REDGPU_2_EXPECTED_maxStructPerStageArrayRORWsCount_524288            (524288)
#define REDGPU_2_EXPECTED_maxStructPerStageSamplersCount_524288              (524288)
#define REDGPU_2_EXPECTED_maxStructPerStageTextureROsCount_524288            (524288)
#define REDGPU_2_EXPECTED_maxStructPerStageTextureRWsCount_524288            (524288)
#define REDGPU_2_EXPECTED_maxStructPerStageAllResourcesCount_1572864         (1572864)
#define REDGPU_2_EXPECTED_maxStructArrayROCsCount_72                         (72)
#define REDGPU_2_EXPECTED_maxStructArrayRORWsCount_524288                    (524288)
#define REDGPU_2_EXPECTED_maxStructSamplersCount_524288                      (524288)
#define REDGPU_2_EXPECTED_maxStructTextureROsCount_524288                    (524288)
#define REDGPU_2_EXPECTED_maxStructTextureRWsCount_524288                    (524288)
#define REDGPU_2_EXPECTED_maxProcedureParametersVariablesBytesCount_128      (128)
#define REDGPU_2_EXPECTED_maxViewportDimensionsX_16384                       (16384)
#define REDGPU_2_EXPECTED_maxViewportDimensionsY_16384                       (16384)
#define REDGPU_2_EXPECTED_minViewportBoundsRange_minus_32768                 (-32768)
#define REDGPU_2_EXPECTED_maxViewportBoundsRange_32767                       (32767)
#define REDGPU_2_EXPECTED_precisionBitsViewportCoordinates_8                 (8)
#define REDGPU_2_EXPECTED_precisionBitsViewportBounds_8                      (8)
#define REDGPU_2_EXPECTED_maxPerStageClipDistances_8                         (8)
#define REDGPU_2_EXPECTED_maxPerStageCullDistances_8                         (8)
#define REDGPU_2_EXPECTED_maxPerStageClipAndCullDistances_8                  (8)
#define REDGPU_2_EXPECTED_maxVertexOutputLocationsCount_128                  (128)
#define REDGPU_2_EXPECTED_maxFragmentInputLocationsCount_112                 (112)
#define REDGPU_2_EXPECTED_maxFragmentOutputColorsCount_8                     (8)
#define REDGPU_2_EXPECTED_maxFragmentOutputColorsCountBlendDualSource_1      (1)
#define REDGPU_2_EXPECTED_maxFragmentOutputResourcesCount_16                 (16)
#define REDGPU_2_EXPECTED_maxComputeSharedMemoryBytesCount_16384             (16384)
#define REDGPU_2_EXPECTED_maxComputeWorkgroupsCountX_65535                   (65535)
#define REDGPU_2_EXPECTED_maxComputeWorkgroupsCountY_65535                   (65535)
#define REDGPU_2_EXPECTED_maxComputeWorkgroupsCountZ_65535                   (65535)
#define REDGPU_2_EXPECTED_maxComputeWorkgroupInvocationsCount_1024           (1024)
#define REDGPU_2_EXPECTED_maxComputeWorkgroupDimensionsX_1024                (1024)
#define REDGPU_2_EXPECTED_maxComputeWorkgroupDimensionsY_1024                (1024)
#define REDGPU_2_EXPECTED_maxComputeWorkgroupDimensionsZ_64                  (64)
#define REDGPU_2_EXPECTED_minImageSampleImageFetchOffset_minus_8             (-8)
#define REDGPU_2_EXPECTED_maxImageSampleImageFetchOffset_7                   (7)
#define REDGPU_2_EXPECTED_minImageGatherOffset_minus_32                      (-32)
#define REDGPU_2_EXPECTED_maxImageGatherOffset_31                            (31)
#define REDGPU_2_EXPECTED_optimalCopyArrayImageRangeArrayBytesFirstBytesAlignment_128           (128)
#define REDGPU_2_EXPECTED_optimalCopyArrayImageRangeArrayTexelsCountToNextRowBytesAlignment_128 (128)

// red2StructsMemorySuballocateStruct

typedef struct Red2Struct {
  RedHandleStruct            handle;
  RedHandleStructDeclaration handleDeclaration; // Kept alive for redStructsSet
} Red2Struct;

// red2CreateMemoryForArray

typedef struct Red2Array {
  RedArray        array;
  RedHandleMemory memory;
} Red2Array;

// red2CreateMemoryForImage

typedef struct Red2Image {
  RedImage        image;
  RedHandleMemory memory;
} Red2Image;

// red2CreateProcedureParameters

typedef struct Red2ProcedureParametersDeclarationStruct {
  unsigned                                  structDeclarationMembersCount;
  const RedStructDeclarationMember *        structDeclarationMembers;
  unsigned                                  structDeclarationMembersArrayROCount;
  const RedStructDeclarationMemberArrayRO * structDeclarationMembersArrayRO;
} Red2ProcedureParametersDeclarationStruct;

typedef struct Red2ProcedureParametersDeclaration {
  unsigned                                   variablesSlot;
  RedVisibleToStageBitflags                  variablesVisibleToStages;
  unsigned                                   variablesBytesCount;
  unsigned                                   structsDeclarationsCount; // Max: 7
  Red2ProcedureParametersDeclarationStruct   structsDeclarations[7];
  Red2ProcedureParametersDeclarationStruct * handlesDeclaration;
} Red2ProcedureParametersDeclaration;

typedef struct Red2ProcedureParametersAndDeclarations {
  RedHandleProcedureParameters procedureParameters;
  RedHandleStructDeclaration   structsDeclarations[7];
  RedHandleStructDeclaration   handlesDeclaration;
} Red2ProcedureParametersAndDeclarations;

// redCreateCalls
// redCreateCallsReusable

typedef struct Red2Output {
  RedHandleOutput            handle;
  RedHandleOutputDeclaration handleDeclaration;
} Red2Output;

typedef struct Red2CallsMutableOutputsArray {
  Red2Output * items;
  size_t       count;
  size_t       capacity;
  size_t       alignment;
} Red2CallsMutableOutputsArray;

typedef struct Red2Calls {
  RedCalls                     calls;
  Red2CallsMutableOutputsArray mutableOutputsArray; // Provided by the user
} Red2Calls;

// red2CallUsageAliasOrderBarrier

typedef struct Red2UsageArrayTempCallStruct {
  unsigned _0;
  size_t   _1;
  unsigned _2;
  unsigned _3;
  unsigned _4;
  unsigned _5;
  uint64_t _6;
  uint64_t _7;
  uint64_t _8;
} Red2UsageArrayTempCallStruct;

typedef struct Red2UsageImageTempCallStruct {
  unsigned _0;
  size_t   _1;
  unsigned _2;
  unsigned _3;
  unsigned _4;
  unsigned _5;
  unsigned _6;
  unsigned _7;
  uint64_t _8;
  struct {
    unsigned _10;
    unsigned _11;
    unsigned _12;
    unsigned _13;
    unsigned _14;
  } _9;
} Red2UsageImageTempCallStruct;

#ifndef REDGPU_2_USER_DECLSPEC
#define REDGPU_2_USER_DECLSPEC
#endif

#ifndef REDGPU_2_USER_API
#define REDGPU_2_USER_API
#endif

#ifndef REDGPU_2_DECLSPEC
#define REDGPU_2_DECLSPEC
#endif

#ifndef REDGPU_2_API
#define REDGPU_2_API
#endif

#define REDGPU_2_BYTES_TO_NEXT_ALIGNMENT_BOUNDARY(CURRENT_BYTES, ALIGNMENT) ( ((ALIGNMENT) - (CURRENT_BYTES) % ((ALIGNMENT) ? (ALIGNMENT) : 1)) % ((ALIGNMENT) ? (ALIGNMENT) : 1) )

#define REDGPU_2_EXPECTFL(CONDITION) if (!(CONDITION)) { red2Crash(#CONDITION, __FUNCTION__, 0, __FILE__, __LINE__); }
#define REDGPU_2_EXPECT(CONDITION)   if (!(CONDITION)) { red2Crash(#CONDITION, __FUNCTION__, 0, optionalFile, optionalLine); }
#define REDGPU_2_EXPECTWG(CONDITION) if (!(CONDITION)) { red2Crash(#CONDITION, __FUNCTION__, gpu, optionalFile, optionalLine); }

// red2Crash is expected to be defined and exposed by the user.
REDGPU_2_USER_DECLSPEC void REDGPU_2_USER_API red2Crash (const char * error, const char * functionName, RedHandleGpu optionalGpuHandle, const char * optionalFile, int optionalLine);

// Expect

npf(REDGPU_2_DECLSPEC void REDGPU_2_API, red2ExpectMinimumGuarantees,                    const RedGpuInfo *,gpuInfo, const char *,optionalFile, int,optionalLine);
npf(REDGPU_2_DECLSPEC void REDGPU_2_API, red2ExpectMinimumGuaranteesIntelUHDGraphics730, const RedGpuInfo *,gpuInfo, const char *,optionalFile, int,optionalLine);
npf(REDGPU_2_DECLSPEC void REDGPU_2_API, red2ExpectAllMemoryToBeCoherent,                const RedGpuInfo *,gpuInfo, const char *,optionalFile, int,optionalLine);
npf(REDGPU_2_DECLSPEC void REDGPU_2_API, red2ExpectMinimumImageFormatsLimitsAndFeatures, const RedGpuInfo *,gpuInfo, const char *,optionalFile, int,optionalLine);
npf(REDGPU_2_DECLSPEC void REDGPU_2_API, red2ExpectMemoryTypes,                          const RedGpuInfo *,gpuInfo, unsigned,expectedMemoryHeapsCount, const RedMemoryHeap *,expectedMemoryHeaps, unsigned,expectedMemoryTypesCount, const RedMemoryType *,expectedMemoryTypes, const char *,optionalFile, int,optionalLine);

// Memory

npf(REDGPU_2_DECLSPEC unsigned REDGPU_2_API, red2MemoryTypeIndexPickFirstVram,           const RedGpuInfo *,gpuInfo, unsigned,memoryTypesSupported);
npf(REDGPU_2_DECLSPEC unsigned REDGPU_2_API, red2MemoryTypeIndexPickFirstUpload,         const RedGpuInfo *,gpuInfo, unsigned,memoryTypesSupported);
npf(REDGPU_2_DECLSPEC unsigned REDGPU_2_API, red2MemoryTypeIndexPickFirstReadback,       const RedGpuInfo *,gpuInfo, unsigned,memoryTypesSupported);
npf(REDGPU_2_DECLSPEC unsigned REDGPU_2_API, red2MemoryTypeIndexPickFirstReBARdGpu,      const RedGpuInfo *,gpuInfo, unsigned,memoryTypesSupported, uint64_t,minAllowedBytesCount);
npf(REDGPU_2_DECLSPEC unsigned REDGPU_2_API, red2MemoryTypeIndexPickFirstReBARiGpu,      const RedGpuInfo *,gpuInfo, unsigned,memoryTypesSupported, uint64_t,minAllowedBytesCount);

// Struct

npf(REDGPU_2_DECLSPEC void REDGPU_2_API, red2StructsMemorySuballocateStruct, RedContext,context, RedHandleGpu,gpu, const char *,handleName, RedHandleStructsMemory,structsMemory, unsigned,structDeclarationMembersCount, const RedStructDeclarationMember *,structDeclarationMembers, unsigned,structDeclarationMembersArrayROCount, const RedStructDeclarationMemberArrayRO *,structDeclarationMembersArrayRO, Red2Struct *,outStruct, RedStatuses *,outStatuses, const char *,optionalFile, int,optionalLine, void *,optionalUserData);

// Create

npf(REDGPU_2_DECLSPEC void REDGPU_2_API, red2CreateMemoryForArray,           RedContext,context, RedHandleGpu,gpu, const char *,handleName, unsigned,memoryTypeIndex, RedBool32,dedicate, Red2Array *,outArray, RedStatuses *,outStatuses, const char *,optionalFile, int,optionalLine, void *,optionalUserData);
npf(REDGPU_2_DECLSPEC void REDGPU_2_API, red2CreateMemoryForImage,           RedContext,context, RedHandleGpu,gpu, const char *,handleName, unsigned,memoryTypeIndex, RedBool32,dedicate, Red2Image *,outImage, RedStatuses *,outStatuses, const char *,optionalFile, int,optionalLine, void *,optionalUserData);
npf(REDGPU_2_DECLSPEC void REDGPU_2_API, red2CreateProcedureParameters,      RedContext,context, RedHandleGpu,gpu, const char *,handleName, const Red2ProcedureParametersDeclaration *,procedureParametersDeclaration, Red2ProcedureParametersAndDeclarations *,outProcedureParametersAndDeclarations, RedStatuses *,outStatuses, const char *,optionalFile, int,optionalLine, void *,optionalUserData);
npf(REDGPU_2_DECLSPEC void REDGPU_2_API, red2CreateProcedure,                RedContext,context, RedHandleGpu,gpu, const char *,handleName, RedHandleProcedureCache,procedureCache, const RedOutputDeclarationMembers *,outputDeclarationMembers, const RedOutputDeclarationMembersResolveSources *,outputDeclarationMembersResolveSources, RedBool32,dependencyByRegion, RedBool32,dependencyByRegionAllowUsageAliasOrderBarriers, RedHandleProcedureParameters,procedureParameters, const char *,gpuCodeVertexMainProcedureName, RedHandleGpuCode,gpuCodeVertex, const char *,gpuCodeFragmentMainProcedureName, RedHandleGpuCode,gpuCodeFragment, const RedProcedureState *,state, const void *,stateExtension, RedBool32,deriveBase, RedHandleProcedure,deriveFrom, RedHandleProcedure *,outProcedure, RedStatuses *,outStatuses, const char *,optionalFile, int,optionalLine, void *,optionalUserData);

// Destroy

npf(REDGPU_2_DECLSPEC void REDGPU_2_API, red2DestroyHandle,                  RedContext,context, RedHandleGpu,gpu, RedHandleType,handleType, void *,handle, void *,optionalHandle2, const char *,optionalFile, int,optionalLine, void *,optionalUserData);

// Calls

npf(REDGPU_2_DECLSPEC void REDGPU_2_API, red2CallsSet,                       RedContext,context, RedHandleGpu,gpu, RedHandleCalls,calls, RedHandleCallsMemory,callsMemory, RedBool32,callsReusable, Red2CallsMutableOutputsArray *,mutableOutputsArray, RedStatuses *,outStatuses, const char *,optionalFile, int,optionalLine, void *,optionalUserData);
    REDGPU_DECLSPEC   void REDGPU_API    redCallsEnd                        (RedContext context, RedHandleGpu gpu, RedHandleCalls calls, RedHandleCallsMemory callsMemory, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData);

// Calls recording

npf(REDGPU_2_DECLSPEC void REDGPU_2_API, red2CallSetProcedureOutput,         RedTypeProcedureAddressCallSetProcedureOutput,    address, RedHandleCalls,calls, RedContext,context, RedHandleGpu,gpu, Red2CallsMutableOutputsArray *,mutableOutputsArray, const RedOutputDeclarationMembers *,outputDeclarationMembers, const RedOutputDeclarationMembersResolveSources *,outputDeclarationMembersResolveSources, RedBool32,dependencyByRegion, RedBool32,dependencyByRegionAllowUsageAliasOrderBarriers, const RedOutputMembers *,outputMembers, const RedOutputMembersResolveTargets *,outputMembersResolveTargets, unsigned,width, unsigned,height, float,depthClearValue, unsigned,stencilClearValue, const RedColorsClearValuesFloat *,colorsClearValuesFloat, const RedColorsClearValuesSint *,colorsClearValuesSint, const RedColorsClearValuesUint *,colorsClearValuesUint, RedStatuses *,outStatuses, const char *,optionalFile, int,optionalLine, void *,optionalUserData);
    REDGPU_DECLSPEC   void REDGPU_API    redCallEndProcedureOutput          (RedTypeProcedureAddressCallEndProcedureOutput     address, RedHandleCalls calls);
npf(REDGPU_2_DECLSPEC void REDGPU_2_API, red2CallGlobalOrderBarrier,         RedTypeProcedureAddressCallUsageAliasOrderBarrier,address, RedHandleCalls,calls);
npf(REDGPU_2_DECLSPEC void REDGPU_2_API, red2CallGlobalReadbackBarrier,      RedTypeProcedureAddressCallUsageAliasOrderBarrier,address, RedHandleCalls,calls);
npf(REDGPU_2_DECLSPEC void REDGPU_2_API, red2CallUsageAliasOrderBarrier,     RedTypeProcedureAddressCallUsageAliasOrderBarrier,address, RedHandleCalls,calls, RedContext,context, unsigned,arrayUsagesCount, const RedUsageArray *,arrayUsages, Red2UsageArrayTempCallStruct *,arrayTempCallStructs, unsigned,imageUsagesCount, const RedUsageImage *,imageUsages, Red2UsageImageTempCallStruct *,imageTempCallStructs, unsigned,aliasesCount, const RedAlias *,aliases, unsigned,ordersCount, const RedOrder *,orders, RedBool32,dependencyByRegion);

#ifdef __cplusplus
}
#endif

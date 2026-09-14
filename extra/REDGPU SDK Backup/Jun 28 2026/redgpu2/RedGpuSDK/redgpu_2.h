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

// red2MemoryAllocate

typedef struct Red2Memory {
  RedHandleMemory handle;
  uint64_t        currentSuballocateOffset;
  uint64_t        bytesCount;
  unsigned        gpuIndex;
  unsigned        memoryTypeIndex;
} Red2Memory;

// red2StructsMemorySuballocateStruct

typedef struct Red2Struct {
  RedHandleStruct            handle;
  RedHandleStructDeclaration handleDeclaration; // Kept alive for redStructsSet
} Red2Struct;

// red2CreateArray

typedef struct Red2Array {
  RedArray        array;
  RedHandleMemory handleAllocatedDedicatedOrMappableMemoryOrPickedMemory;
} Red2Array;

// red2CreateImage

typedef struct Red2Image {
  RedImage        image;
  RedHandleMemory handleAllocatedDedicatedMemoryOrPickedMemory;
} Red2Image;

// redCreateTexture

#define RED2_RESTRICT_TO_ACCESS_TEXTURE_RO                RED_ACCESS_BITFLAG_STRUCT_RESOURCE_NON_FRAGMENT_STAGE_R | RED_ACCESS_BITFLAG_STRUCT_RESOURCE_FRAGMENT_STAGE_R
#define RED2_RESTRICT_TO_ACCESS_TEXTURE_RW                RED_ACCESS_BITFLAG_STRUCT_RESOURCE_W
#define RED2_RESTRICT_TO_ACCESS_TEXTURE_OUTPUT_DEPTH_RO   RED_ACCESS_BITFLAG_OUTPUT_DEPTH_R
#define RED2_RESTRICT_TO_ACCESS_TEXTURE_OUTPUT_DEPTH_RW   RED_ACCESS_BITFLAG_OUTPUT_DEPTH_RW
#define RED2_RESTRICT_TO_ACCESS_TEXTURE_OUTPUT_STENCIL_RO RED_ACCESS_BITFLAG_OUTPUT_STENCIL_R
#define RED2_RESTRICT_TO_ACCESS_TEXTURE_OUTPUT_STENCIL_RW RED_ACCESS_BITFLAG_OUTPUT_STENCIL_RW
#define RED2_RESTRICT_TO_ACCESS_TEXTURE_OUTPUT_COLOR_W    RED_ACCESS_BITFLAG_OUTPUT_COLOR_W

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
REDGPU_2_USER_DECLSPEC void REDGPU_2_USER_API red2Crash(const char * error, const char * functionName, RedHandleGpu optionalGpuHandle, const char * optionalFile, int optionalLine);

// Expect

REDGPU_2_DECLSPEC void REDGPU_2_API red2ExpectMinimumGuarantees                    (const RedGpuInfo * gpuInfo, const char * optionalFile, int optionalLine);
REDGPU_2_DECLSPEC void REDGPU_2_API red2ExpectMinimumGuaranteesIntelUHDGraphics730 (const RedGpuInfo * gpuInfo, const char * optionalFile, int optionalLine);
REDGPU_2_DECLSPEC void REDGPU_2_API red2ExpectAllMemoryToBeCoherent                (const RedGpuInfo * gpuInfo, const char * optionalFile, int optionalLine);
REDGPU_2_DECLSPEC void REDGPU_2_API red2ExpectMinimumImageFormatsLimitsAndFeatures (const RedGpuInfo * gpuInfo, const char * optionalFile, int optionalLine);
REDGPU_2_DECLSPEC void REDGPU_2_API red2ExpectMemoryTypes                          (const RedGpuInfo * gpuInfo, unsigned expectedMemoryHeapsCount, const RedMemoryHeap * expectedMemoryHeaps, unsigned expectedMemoryTypesCount, const RedMemoryType * expectedMemoryTypes, const char * optionalFile, int optionalLine);

// Memory

REDGPU_DECLSPEC   void REDGPU_API   redMemoryGetBudget                 (RedContext context, RedHandleGpu gpu, RedMemoryBudget * outMemoryBudget, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData);
REDGPU_2_DECLSPEC void REDGPU_2_API red2MemoryAllocate                 (RedContext context, RedHandleGpu gpu, const char * handleName, uint64_t bytesCount, unsigned memoryTypeIndex, unsigned memoryBitflags, Red2Memory * outMemory, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData);
REDGPU_DECLSPEC   void REDGPU_API   redMemoryMap                       (RedContext context, RedHandleGpu gpu, RedHandleMemory mappableMemory, uint64_t mappableMemoryBytesFirst, uint64_t mappableMemoryBytesCount, void ** outVolatilePointer, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData);
REDGPU_DECLSPEC   void REDGPU_API   redMemoryUnmap                     (RedContext context, RedHandleGpu gpu, RedHandleMemory mappableMemory, const char * optionalFile, int optionalLine, void * optionalUserData);
REDGPU_DECLSPEC   void REDGPU_API   redMemoryNonCoherentFlush          (RedContext context, RedHandleGpu gpu, unsigned mappableMemoryRangesCount, const RedMappableMemoryRange * mappableMemoryRanges, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData);
REDGPU_DECLSPEC   void REDGPU_API   redMemoryNonCoherentInvalidate     (RedContext context, RedHandleGpu gpu, unsigned mappableMemoryRangesCount, const RedMappableMemoryRange * mappableMemoryRanges, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData);

// Struct

REDGPU_DECLSPEC   void REDGPU_API   redStructsMemoryAllocate           (RedContext context, RedHandleGpu gpu, const char * handleName, unsigned maxStructsCount, unsigned maxStructsMembersOfTypeArrayROConstantCount, unsigned maxStructsMembersOfTypeArrayROOrArrayRWCount, unsigned maxStructsMembersOfTypeTextureROCount, unsigned maxStructsMembersOfTypeTextureRWCount, RedHandleStructsMemory * outStructsMemory, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData);
REDGPU_DECLSPEC   void REDGPU_API   redStructsMemoryAllocateSamplersWithInlineSamplers (RedContext context, RedHandleGpu gpu, const char * handleName, unsigned maxStructsCount, unsigned maxStructsMembersOfTypeSamplerCount, unsigned maxStructsMembersOfTypeInlineSamplerCount, RedHandleStructsMemory * outStructsMemory, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData);
REDGPU_2_DECLSPEC void REDGPU_2_API red2StructsMemorySuballocateStruct (RedContext context, RedHandleGpu gpu, const char * handleName, RedHandleStructsMemory structsMemory, unsigned structDeclarationMembersCount, const RedStructDeclarationMember * structDeclarationMembers, unsigned structDeclarationMembersArrayROCount, const RedStructDeclarationMemberArrayRO * structDeclarationMembersArrayRO, Red2Struct * outStruct, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData);
REDGPU_DECLSPEC   void REDGPU_API   redStructsMemoryReset              (RedContext context, RedHandleGpu gpu, RedHandleStructsMemory structsMemory, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData);
REDGPU_DECLSPEC   void REDGPU_API   redStructsSet                      (RedContext context, RedHandleGpu gpu, unsigned structsMembersCount, const RedStructMember * structsMembers, const char * optionalFile, int optionalLine, void * optionalUserData);

// Create

REDGPU_DECLSPEC   void REDGPU_API   redCreateContext                   (RedTypeProcedureMalloc malloc, RedTypeProcedureFree free, RedTypeProcedureMallocTagged optionalMallocTagged, RedTypeProcedureFreeTagged optionalFreeTagged, RedTypeProcedureDebugCallback debugCallback, RedSdkVersion sdkVersion, unsigned sdkExtensionsCount, const unsigned * sdkExtensions, const char * optionalProgramName, unsigned optionalProgramVersion, const char * optionalEngineName, unsigned optionalEngineVersion, const void * optionalSettings, RedContext * outContext, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData);
REDGPU_2_DECLSPEC void REDGPU_2_API red2CreateArray                    (RedContext context, RedHandleGpu gpu, const char * handleName, RedArrayType type, uint64_t bytesCount, uint64_t structuredBufferElementBytesCount, RedAccessBitflags restrictToAccess, unsigned initialQueueFamilyIndex, uint64_t maxAllowedOverallocationBytesCount, RedBool32 dedicate, RedBool32 mappable, unsigned dedicateOrMappableMemoryTypeIndex, unsigned dedicateOrMappableMemoryBitflags, unsigned suballocateFromMemoryOnFirstMatchPointersCount, Red2Memory ** suballocateFromMemoryOnFirstMatchPointers, Red2Array * outArray, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData);
REDGPU_2_DECLSPEC void REDGPU_2_API red2CreateImage                    (RedContext context, RedHandleGpu gpu, const char * handleName, RedImageDimensions dimensions, RedFormat format, unsigned width, unsigned height, unsigned depth, unsigned levelsCount, unsigned layersCount, RedMultisampleCountBitflag multisampleCount, RedAccessBitflags restrictToAccess, unsigned initialQueueFamilyIndex, RedBool32 dedicate, unsigned dedicateMemoryTypeIndex, unsigned dedicateMemoryBitflags, unsigned suballocateFromMemoryOnFirstMatchPointersCount, Red2Memory ** suballocateFromMemoryOnFirstMatchPointers, Red2Image * outImage, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData);
REDGPU_DECLSPEC   void REDGPU_API   redCreateSampler                   (RedContext context, RedHandleGpu gpu, const char * handleName, RedSamplerFiltering filteringMag, RedSamplerFiltering filteringMin, RedSamplerFilteringMip filteringMip, RedSamplerBehaviorOutsideTextureCoordinate behaviorOutsideTextureCoordinateU, RedSamplerBehaviorOutsideTextureCoordinate behaviorOutsideTextureCoordinateV, RedSamplerBehaviorOutsideTextureCoordinate behaviorOutsideTextureCoordinateW, float mipLodBias, RedBool32 enableAnisotropy, float maxAnisotropy, RedBool32 enableCompare, RedCompareOp compareOp, float minLod, float maxLod, RedHandleSampler * outSampler, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData);
REDGPU_DECLSPEC   void REDGPU_API   redCreateTexture                   (RedContext context, RedHandleGpu gpu, const char * handleName, RedHandleImage image, RedImagePartBitflags parts, RedTextureDimensions dimensions, RedFormat format, unsigned levelsFirst, unsigned levelsCount, unsigned layersFirst, unsigned layersCount, RedAccessBitflags restrictToAccess, RedHandleTexture * outTexture, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData);
REDGPU_DECLSPEC   void REDGPU_API   redCreateGpuCode                   (RedContext context, RedHandleGpu gpu, const char * handleName, uint64_t irBytesCount, const void * ir, RedHandleGpuCode * outGpuCode, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData);
REDGPU_2_DECLSPEC void REDGPU_2_API red2CreateProcedureParameters      (RedContext context, RedHandleGpu gpu, const char * handleName, const Red2ProcedureParametersDeclaration * procedureParametersDeclaration, Red2ProcedureParametersAndDeclarations * outProcedureParametersAndDeclarations, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData);
REDGPU_DECLSPEC   void REDGPU_API   redCreateProcedureCache            (RedContext context, RedHandleGpu gpu, const char * handleName, uint64_t fromBlobBytesCount, const void * fromBlob, RedHandleProcedureCache * outProcedureCache, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData);
REDGPU_2_DECLSPEC void REDGPU_2_API red2CreateProcedure                (RedContext context, RedHandleGpu gpu, const char * handleName, RedHandleProcedureCache procedureCache, const RedOutputDeclarationMembers * outputDeclarationMembers, const RedOutputDeclarationMembersResolveSources * outputDeclarationMembersResolveSources, RedBool32 dependencyByRegion, RedBool32 dependencyByRegionAllowUsageAliasOrderBarriers, RedHandleProcedureParameters procedureParameters, const char * gpuCodeVertexMainProcedureName, RedHandleGpuCode gpuCodeVertex, const char * gpuCodeFragmentMainProcedureName, RedHandleGpuCode gpuCodeFragment, const RedProcedureState * state, const void * stateExtension, RedBool32 deriveBase, RedHandleProcedure deriveFrom, RedHandleProcedure * outProcedure, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData);
REDGPU_DECLSPEC   void REDGPU_API   redCreateProcedureCompute          (RedContext context, RedHandleGpu gpu, const char * handleName, RedHandleProcedureCache procedureCache, RedHandleProcedureParameters procedureParameters, const char * gpuCodeMainProcedureName, RedHandleGpuCode gpuCode, RedHandleProcedure * outProcedure, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData);
REDGPU_DECLSPEC   void REDGPU_API   redCreateCpuSignal                 (RedContext context, RedHandleGpu gpu, const char * handleName, RedBool32 createSignaled, RedHandleCpuSignal * outCpuSignal, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData);
REDGPU_DECLSPEC   void REDGPU_API   redCreateGpuSignal                 (RedContext context, RedHandleGpu gpu, const char * handleName, RedHandleGpuSignal * outGpuSignal, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData);
REDGPU_DECLSPEC   void REDGPU_API   redCreateGpuToCpuSignal            (RedContext context, RedHandleGpu gpu, const char * handleName, RedHandleGpuToCpuSignal * outGpuToCpuSignal, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData);
REDGPU_DECLSPEC   void REDGPU_API   redCreateCalls                     (RedContext context, RedHandleGpu gpu, const char * handleName, unsigned queueFamilyIndex, RedCalls * outCalls, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData);
REDGPU_DECLSPEC   void REDGPU_API   redCreateCallsReusable             (RedContext context, RedHandleGpu gpu, const char * handleName, unsigned queueFamilyIndex, RedCalls * outCalls, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData);

// Destroy

REDGPU_DECLSPEC   void REDGPU_API   redDestroyContext                  (RedContext context, const char * optionalFile, int optionalLine, void * optionalUserData);
REDGPU_2_DECLSPEC void REDGPU_2_API red2DestroyHandle                  (RedContext context, RedHandleGpu gpu, RedHandleType handleType, void * handle, void * optionalHandle2, const char * optionalFile, int optionalLine, void * optionalUserData);

// Procedure cache

REDGPU_DECLSPEC   void REDGPU_API   redProcedureCacheGetBlob           (RedContext context, RedHandleGpu gpu, RedHandleProcedureCache procedureCache, uint64_t * outBlobBytesCount, void * outBlob, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData);
REDGPU_DECLSPEC   void REDGPU_API   redProcedureCacheMergeCaches       (RedContext context, RedHandleGpu gpu, unsigned sourceProcedureCachesCount, const RedHandleProcedureCache * sourceProcedureCaches, RedHandleProcedureCache targetProcedureCache, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData);

// CPU signal

REDGPU_DECLSPEC   void REDGPU_API   redCpuSignalGetStatus              (RedContext context, RedHandleGpu gpu, RedHandleCpuSignal cpuSignal, RedStatus * outStatus, const char * optionalFile, int optionalLine, void * optionalUserData);
REDGPU_DECLSPEC   void REDGPU_API   redCpuSignalWait                   (RedContext context, RedHandleGpu gpu, unsigned cpuSignalsCount, const RedHandleCpuSignal * cpuSignals, RedBool32 waitAll, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData);
REDGPU_DECLSPEC   void REDGPU_API   redCpuSignalUnsignal               (RedContext context, RedHandleGpu gpu, unsigned cpuSignalsCount, const RedHandleCpuSignal * cpuSignals, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData);

// GPU to CPU signal

REDGPU_DECLSPEC   void REDGPU_API   redGpuToCpuSignalGetStatus         (RedContext context, RedHandleGpu gpu, RedHandleGpuToCpuSignal gpuToCpuSignal, RedStatus * outStatus, const char * optionalFile, int optionalLine, void * optionalUserData);
REDGPU_DECLSPEC   void REDGPU_API   redGpuToCpuSignalUnsignal          (RedContext context, RedHandleGpu gpu, RedHandleGpuToCpuSignal gpuToCpuSignal, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData);

// Calls

REDGPU_2_DECLSPEC void REDGPU_2_API red2CallsSet                       (RedContext context, RedHandleGpu gpu, RedHandleCalls calls, RedHandleCallsMemory callsMemory, RedBool32 callsReusable, Red2CallsMutableOutputsArray * mutableOutputsArray, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData);
REDGPU_DECLSPEC   void REDGPU_API   redCallsEnd                        (RedContext context, RedHandleGpu gpu, RedHandleCalls calls, RedHandleCallsMemory callsMemory, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData);

// Calls recording

REDGPU_DECLSPEC   void REDGPU_API   redGetCallProceduresAndAddresses   (RedContext context, RedHandleGpu gpu, RedCallProceduresAndAddresses * outCallProceduresAndAddresses, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData);
REDGPU_DECLSPEC   void REDGPU_API   redCallSetDynamicViewport          (RedTypeProcedureAddressCallSetDynamicViewport     address, RedHandleCalls calls, float x, float y, float width, float height, float depthMin, float depthMax);
REDGPU_DECLSPEC   void REDGPU_API   redCallSetDynamicScissor           (RedTypeProcedureAddressCallSetDynamicScissor      address, RedHandleCalls calls, int x, int y, unsigned width, unsigned height);
REDGPU_DECLSPEC   void REDGPU_API   redCallSetStructsMemory            (RedTypeProcedureAddressCallSetStructsMemory       address, RedHandleCalls calls, RedHandleStructsMemory structsMemory, RedHandleStructsMemory structsMemorySamplers);
REDGPU_DECLSPEC   void REDGPU_API   redCallSetProcedureParameters      (RedTypeProcedureAddressCallSetProcedureParameters address, RedHandleCalls calls, RedProcedureType procedureType, RedHandleProcedureParameters procedureParameters);
REDGPU_2_DECLSPEC void REDGPU_2_API red2CallSetProcedureOutput         (RedTypeProcedureAddressCallSetProcedureOutput     address, RedHandleCalls calls, RedContext context, RedHandleGpu gpu, Red2CallsMutableOutputsArray * mutableOutputsArray, const RedOutputDeclarationMembers * outputDeclarationMembers, const RedOutputDeclarationMembersResolveSources * outputDeclarationMembersResolveSources, RedBool32 dependencyByRegion, RedBool32 dependencyByRegionAllowUsageAliasOrderBarriers, const RedOutputMembers * outputMembers, const RedOutputMembersResolveTargets * outputMembersResolveTargets, unsigned width, unsigned height, float depthClearValue, unsigned stencilClearValue, const RedColorsClearValuesFloat * colorsClearValuesFloat, const RedColorsClearValuesSint * colorsClearValuesSint, const RedColorsClearValuesUint * colorsClearValuesUint, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData);
REDGPU_DECLSPEC   void REDGPU_API   redCallEndProcedureOutput          (RedTypeProcedureAddressCallEndProcedureOutput     address, RedHandleCalls calls);
REDGPU_2_DECLSPEC void REDGPU_2_API red2CallGlobalOrderBarrier         (RedTypeProcedureAddressCallUsageAliasOrderBarrier address, RedHandleCalls calls);
REDGPU_2_DECLSPEC void REDGPU_2_API red2CallGlobalReadbackBarrier      (RedTypeProcedureAddressCallUsageAliasOrderBarrier address, RedHandleCalls calls);
REDGPU_2_DECLSPEC void REDGPU_2_API red2CallUsageAliasOrderBarrier     (RedTypeProcedureAddressCallUsageAliasOrderBarrier address, RedHandleCalls calls, RedContext context, unsigned arrayUsagesCount, const RedUsageArray * arrayUsages, Red2UsageArrayTempCallStruct * arrayTempCallStructs, unsigned imageUsagesCount, const RedUsageImage * imageUsages, Red2UsageImageTempCallStruct * imageTempCallStructs, unsigned aliasesCount, const RedAlias * aliases, unsigned ordersCount, const RedOrder * orders, RedBool32 dependencyByRegion);
REDGPU_DECLSPEC   void REDGPU_API   redCallMark                        (RedTypeProcedureAddressCallMark                   address, RedHandleCalls calls, const char * mark);
REDGPU_DECLSPEC   void REDGPU_API   redCallMarkSet                     (RedTypeProcedureAddressCallMarkSet                address, RedHandleCalls calls, const char * mark);
REDGPU_DECLSPEC   void REDGPU_API   redCallMarkEnd                     (RedTypeProcedureAddressCallMarkEnd                address, RedHandleCalls calls);

// Queue

REDGPU_DECLSPEC   void REDGPU_API   redQueueSubmit                     (RedContext context, RedHandleGpu gpu, RedHandleQueue queue, unsigned timelinesCount, const RedGpuTimeline * timelines, RedHandleCpuSignal signalCpuSignal, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData);

// Mark

REDGPU_DECLSPEC   void REDGPU_API   redMark                            (const char * mark, const char * optionalFile, int optionalLine, void * optionalUserData);
REDGPU_DECLSPEC   void REDGPU_API   redMarkSet                         (const char * mark, const char * optionalFile, int optionalLine, void * optionalUserData);
REDGPU_DECLSPEC   void REDGPU_API   redMarkEnd                         (const char * optionalFile, int optionalLine, void * optionalUserData);

#ifdef __cplusplus
}
#endif

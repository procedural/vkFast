#ifdef _WIN32
#define REDGPU_OS_WINDOWS
#endif
#if defined(__linux__) && !defined(__ANDROID__)
#define REDGPU_OS_LINUX
#endif
#ifdef __ANDROID__
#define REDGPU_OS_ANDROID
#endif

#ifdef REDGPU_OS_WINDOWS
#define REDGPU_2_DECLSPEC __declspec(dllexport)
#define REDGPU_2_API
#endif
#ifdef REDGPU_OS_LINUX
#define REDGPU_2_DECLSPEC __attribute__((visibility("default")))
#define REDGPU_2_API
#endif

#include "redgpu_2.h"
#include "redgpu_wsi.h"

#ifdef REDGPU_OS_WINDOWS
typedef void * HINSTANCE;
typedef void * HWND;
#endif
#ifdef REDGPU_OS_LINUX
#include <X11/Xlib.h>
#include <xcb/xcb.h>
#endif

#ifdef REDGPU_OS_WINDOWS
#define VK_USE_PLATFORM_WIN32_KHR
#endif
#ifdef REDGPU_OS_LINUX
#define VK_USE_PLATFORM_XLIB_KHR
#define VK_USE_PLATFORM_XCB_KHR
#endif
#include "redgpu_vk.h"

#ifndef __cplusplus
#define REDGPU_DISABLE_NAMED_PARAMETERS
#endif

#include "misc/np/np.h"
#include "misc/np/np_redgpu.h"

static unsigned red2InternalGetGpuIndex(RedContext context, RedHandleGpu gpu) {
  unsigned gpuIndex = (unsigned)-1;
  for (unsigned i = 0; i < context->gpusCount; i += 1) {
    if (context->gpus[i].gpu == gpu) {
      gpuIndex = i;
      break;
    }
  }
  return gpuIndex;
}

REDGPU_2_DECLSPEC void REDGPU_2_API red2MemoryAllocate(RedContext context, RedHandleGpu gpu, const char * handleName, uint64_t bytesCount, unsigned memoryTypeIndex, unsigned memoryBitflags, Red2Memory * outMemory, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData) {
  unsigned gpuIndex = red2InternalGetGpuIndex(context, gpu);
  REDGPU_2_EXPECTWG(gpuIndex != (unsigned)-1);

  REDGPU_2_EXPECTWG(context->gpus[gpuIndex].memoryTypes[memoryTypeIndex].isCpuMappable == 0 || !"Create mappable memory with red2CreateArray");

  RedHandleMemory handle = NULL;
  np(redMemoryAllocate,
    "context", context,
    "gpu", gpu,
    "handleName", handleName,
    "bytesCount", bytesCount,
    "memoryTypeIndex", memoryTypeIndex,
    "dedicateToArray", 0,
    "dedicateToImage", 0,
    "memoryBitflags", memoryBitflags,
    "outMemory", &handle,
    "outStatuses", outStatuses,
    "optionalFile", optionalFile,
    "optionalLine", optionalLine,
    "optionalUserData", optionalUserData
  );
  // Filling
  Red2Memory;
  outMemory->handle                   = handle;
  outMemory->currentSuballocateOffset = 0;
  outMemory->bytesCount               = bytesCount;
  outMemory->gpuIndex                 = gpuIndex;
  outMemory->memoryTypeIndex          = memoryTypeIndex;
}

REDGPU_2_DECLSPEC void REDGPU_2_API red2StructsMemorySuballocateStruct(RedContext context, RedHandleGpu gpu, const char * handleName, RedHandleStructsMemory structsMemory, unsigned structDeclarationMembersCount, const RedStructDeclarationMember * structDeclarationMembers, unsigned structDeclarationMembersArrayROCount, const RedStructDeclarationMemberArrayRO * structDeclarationMembersArrayRO, Red2Struct * outStruct, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData) {
  RedHandleStructDeclaration handleDeclaration = NULL;
  np(redCreateStructDeclaration,
    "context", context,
    "gpu", gpu,
    "handleName", handleName,
    "structDeclarationMembersCount", structDeclarationMembersCount,
    "structDeclarationMembers", structDeclarationMembers,
    "structDeclarationMembersArrayROCount", structDeclarationMembersArrayROCount,
    "structDeclarationMembersArrayRO", structDeclarationMembersArrayRO,
    "procedureParametersHandlesDeclaration", 0,
    "outStructDeclaration", &handleDeclaration,
    "outStatuses", outStatuses,
    "optionalFile", optionalFile,
    "optionalLine", optionalLine,
    "optionalUserData", optionalUserData
  );
  RedHandleStruct handle = NULL;
  np(redStructsMemorySuballocateStructs,
    "context", context,
    "gpu", gpu,
    "handleNames", &handleName,
    "structsMemory", structsMemory,
    "structsDeclarationsCount", 1,
    "structsDeclarations", &handleDeclaration,
    "outStructs", &handle,
    "outStatuses", outStatuses,
    "optionalFile", optionalFile,
    "optionalLine", optionalLine,
    "optionalUserData", optionalUserData
  );
  // Filling
  Red2Struct;
  outStruct->handle            = handle;
  outStruct->handleDeclaration = handleDeclaration;
}

REDGPU_2_DECLSPEC void REDGPU_2_API red2ExpectMinimumGuarantees(const RedGpuInfo * gpuInfo, const char * optionalFile, int optionalLine) {
  RedHandleGpu gpu = gpuInfo->gpu;
  REDGPU_2_EXPECTWG(gpuInfo->maxMemoryAllocateCount                      >= REDGPU_2_EXPECTED_maxMemoryAllocateCount_4096); // maxMemoryAllocationCount
  REDGPU_2_EXPECTWG(gpuInfo->minMemoryAllocateBytesAlignment             <= REDGPU_2_EXPECTED_minMemoryAllocateBytesAlignment_4096); // minMemoryMapAlignment; RX 550 and Arc A770 and Arc B580 on Linux
  REDGPU_2_EXPECTWG(gpuInfo->maxMemoryAllocateBytesCount                 >= REDGPU_2_EXPECTED_maxMemoryAllocateBytesCount_1073741824); // maxMemoryAllocationSize; Redmi Note 8 on Android 11; Not used: RX 550 and RX 9070 XT on Windows is 2147483648
  REDGPU_2_EXPECTWG(gpuInfo->minMemoryNonCoherentBlockBytesCount         <= REDGPU_2_EXPECTED_minMemoryNonCoherentBlockBytesCount_256); // nonCoherentAtomSize
  REDGPU_2_EXPECTWG(gpuInfo->maxCreateSamplerCount                       >= REDGPU_2_EXPECTED_maxCreateSamplerCount_4000); // maxSamplerAllocationCount
  REDGPU_2_EXPECTWG(gpuInfo->minMemoryPageSeparationArrayImageBytesCount <= REDGPU_2_EXPECTED_minMemoryPageSeparationArrayImageBytesCount_65536); // bufferImageGranularity; D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT (65536)
  REDGPU_2_EXPECTWG(gpuInfo->minArrayROCStructMemberRangeBytesAlignment  <= REDGPU_2_EXPECTED_minArrayROCStructMemberRangeBytesAlignment_256); // minUniformBufferOffsetAlignment; D3D12_CONSTANT_BUFFER_DATA_PLACEMENT_ALIGNMENT (256)
  REDGPU_2_EXPECTWG(gpuInfo->maxArrayROCStructMemberRangeBytesCount      >= REDGPU_2_EXPECTED_maxArrayROCStructMemberRangeBytesCount_65536); // maxUniformBufferRange; D3D12_REQ_CONSTANT_BUFFER_ELEMENT_COUNT (4096) * 16
  REDGPU_2_EXPECTWG(gpuInfo->minArrayRORWStructMemberRangeBytesAlignment <= REDGPU_2_EXPECTED_minArrayRORWStructMemberRangeBytesAlignment_64); // minStorageBufferOffsetAlignment; Arc A770 and Arc B580 on Windows, Redmi Note 8 on Android 11; Not used: D3D12_RAW_UAV_SRV_BYTE_ALIGNMENT (16)
  REDGPU_2_EXPECTWG(gpuInfo->maxArrayRORWStructMemberRangeBytesCount     >= REDGPU_2_EXPECTED_maxArrayRORWStructMemberRangeBytesCount_1073741820); // maxStorageBufferRange; Arc A770 and Arc B580 on Windows
  REDGPU_2_EXPECTWG(gpuInfo->maxArrayIndexUint32Value                    == 0xFFFFFFFF); // maxDrawIndexedIndexValue
  REDGPU_2_EXPECTWG(gpuInfo->maxImageDimensions1D                        >= REDGPU_2_EXPECTED_maxImageDimensions1D_16384); // maxImageDimension1D; D3D12_REQ_TEXTURE1D_U_DIMENSION (16384)
  REDGPU_2_EXPECTWG(gpuInfo->maxImageDimensions2D                        >= REDGPU_2_EXPECTED_maxImageDimensions2D_16384); // maxImageDimension2D; D3D12_REQ_TEXTURE2D_U_OR_V_DIMENSION (16384)
  REDGPU_2_EXPECTWG(gpuInfo->maxImageDimensions3D                        >= REDGPU_2_EXPECTED_maxImageDimensions3D_2048); // maxImageDimension3D; D3D12_REQ_TEXTURE3D_U_V_OR_W_DIMENSION (2048)
  REDGPU_2_EXPECTWG(gpuInfo->maxImageDimensionsCube                      >= REDGPU_2_EXPECTED_maxImageDimensionsCube_16384); // maxImageDimensionCube; D3D12_REQ_TEXTURECUBE_DIMENSION (16384)
  REDGPU_2_EXPECTWG(gpuInfo->maxImageLayersCount                         >= REDGPU_2_EXPECTED_maxImageLayersCount_2048); // maxImageArrayLayers; D3D12_REQ_TEXTURE2D_ARRAY_AXIS_DIMENSION (2048)
  REDGPU_2_EXPECTWG(gpuInfo->maxSamplerMipLodBias                        >= REDGPU_2_EXPECTED_maxSamplerMipLodBias_15); // maxSamplerLodBias; RTX 2060; Not used: D3D12_MIP_LOD_BIAS_MAX (15.99f)
  REDGPU_2_EXPECTWG(gpuInfo->maxSamplerAnisotropy                        >= REDGPU_2_EXPECTED_maxSamplerAnisotropy_16); // maxSamplerAnisotropy; D3D12_MAX_MAXANISOTROPY (16)
  REDGPU_2_EXPECTWG(gpuInfo->precisionBitsSamplerFilteringMagMin         >= REDGPU_2_EXPECTED_precisionBitsSamplerFilteringMagMin_8); // subTexelPrecisionBits; D3D12_SUBTEXEL_FRACTIONAL_BIT_COUNT (8)
  REDGPU_2_EXPECTWG(gpuInfo->precisionBitsSamplerFilteringMip            >= REDGPU_2_EXPECTED_precisionBitsSamplerFilteringMip_8); // mipmapPrecisionBits; D3D12_MIP_LOD_FRACTIONAL_BIT_COUNT (8)
  REDGPU_2_EXPECTWG(gpuInfo->maxOutputWidth                              >= REDGPU_2_EXPECTED_maxOutputWidth_16384); // maxFramebufferWidth; D3D12_REQ_TEXTURE2D_U_OR_V_DIMENSION (16384)
  REDGPU_2_EXPECTWG(gpuInfo->maxOutputHeight                             >= REDGPU_2_EXPECTED_maxOutputHeight_16384); // maxFramebufferHeight; D3D12_REQ_TEXTURE2D_U_OR_V_DIMENSION (16384)
  REDGPU_2_EXPECTWG(gpuInfo->maxOutputColorsCount                        >= REDGPU_2_EXPECTED_maxOutputColorsCount_8); // maxColorAttachments; D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT (8)
  REDGPU_2_EXPECTWG(gpuInfo->supportedMultisampleCountsForOutputDepth    >= (RED_MULTISAMPLE_COUNT_BITFLAG_1 | RED_MULTISAMPLE_COUNT_BITFLAG_4));
  REDGPU_2_EXPECTWG(gpuInfo->supportedMultisampleCountsForOutputStencil  >= (RED_MULTISAMPLE_COUNT_BITFLAG_1 | RED_MULTISAMPLE_COUNT_BITFLAG_4));
  REDGPU_2_EXPECTWG(gpuInfo->supportedMultisampleCountsForOutputColor    >= (RED_MULTISAMPLE_COUNT_BITFLAG_1 | RED_MULTISAMPLE_COUNT_BITFLAG_4));
  REDGPU_2_EXPECTWG(gpuInfo->supportedMultisampleCountsForEmptyOutput    >= (RED_MULTISAMPLE_COUNT_BITFLAG_1 | RED_MULTISAMPLE_COUNT_BITFLAG_4));
  REDGPU_2_EXPECTWG(gpuInfo->maxStructSlotsCount                         >= REDGPU_2_EXPECTED_maxStructSlotsCount_1024); // maxPerSetDescriptors; Arc A770 and Arc B580 on Linux
  REDGPU_2_EXPECTWG(gpuInfo->maxStructPerStageArrayROCsCount             >= REDGPU_2_EXPECTED_maxStructPerStageArrayROCsCount_12); // maxPerStageDescriptorUniformBuffers; Not used: Tegra X1 on Linux is 15
  REDGPU_2_EXPECTWG(gpuInfo->maxStructPerStageArrayRORWsCount            >= REDGPU_2_EXPECTED_maxStructPerStageArrayRORWsCount_524288); // maxPerStageDescriptorStorageBuffers; Redmi Note 8 on Android 11; Not used: Tegra X1 on Linux and RTX 2060 on both Windows and Linux is 1048576
  REDGPU_2_EXPECTWG(gpuInfo->maxStructPerStageSamplersCount              >= REDGPU_2_EXPECTED_maxStructPerStageSamplersCount_524288); // maxPerStageDescriptorSamplers; Redmi Note 8 on Android 11; Not used: Tegra X1 on Linux and RTX 2060 on both Windows and Linux is 1048576
  REDGPU_2_EXPECTWG(gpuInfo->maxStructPerStageTextureROsCount            >= REDGPU_2_EXPECTED_maxStructPerStageTextureROsCount_524288); // maxPerStageDescriptorSampledImages; Redmi Note 8 on Android 11; Not used: Tegra X1 on Linux and RTX 2060 on both Windows and Linux is 1048576
  REDGPU_2_EXPECTWG(gpuInfo->maxStructPerStageTextureRWsCount            >= REDGPU_2_EXPECTED_maxStructPerStageTextureRWsCount_524288); // maxPerStageDescriptorStorageImages; Redmi Note 8 on Android 11; Not used: Tegra X1 on Linux and RTX 2060 on both Windows and Linux is 1048576
  REDGPU_2_EXPECTWG(gpuInfo->maxStructPerStageAllResourcesCount          >= REDGPU_2_EXPECTED_maxStructPerStageAllResourcesCount_1572864); // maxPerStageResources; Redmi Note 8 on Android 11; Not used: RX 550 and RX 9070 XT on Linux is 8388606
  REDGPU_2_EXPECTWG(gpuInfo->maxStructArrayROCsCount                     >= REDGPU_2_EXPECTED_maxStructArrayROCsCount_72); // maxDescriptorSetUniformBuffers; Not used: Tegra X1 on Linux is 90
  REDGPU_2_EXPECTWG(gpuInfo->maxStructArrayRORWsCount                    >= REDGPU_2_EXPECTED_maxStructArrayRORWsCount_524288); // maxDescriptorSetStorageBuffers; Redmi Note 8 on Android 11; Not used: Tegra X1 on Linux and RTX 2060 on both Windows and Linux is 1048576
  REDGPU_2_EXPECTWG(gpuInfo->maxStructSamplersCount                      >= REDGPU_2_EXPECTED_maxStructSamplersCount_524288); // maxDescriptorSetSamplers; Redmi Note 8 on Android 11; Not used: Tegra X1 on Linux and RTX 2060 on both Windows and Linux is 1048576
  REDGPU_2_EXPECTWG(gpuInfo->maxStructTextureROsCount                    >= REDGPU_2_EXPECTED_maxStructTextureROsCount_524288); // maxDescriptorSetSampledImages; Redmi Note 8 on Android 11; Not used: Tegra X1 on Linux and RTX 2060 on both Windows and Linux is 1048576
  REDGPU_2_EXPECTWG(gpuInfo->maxStructTextureRWsCount                    >= REDGPU_2_EXPECTED_maxStructTextureRWsCount_524288); // maxDescriptorSetStorageImages; Redmi Note 8 on Android 11; Not used: Tegra X1 on Linux and RTX 2060 on both Windows and Linux is 1048576
  REDGPU_2_EXPECTWG(gpuInfo->maxProcedureParametersVariablesBytesCount   >= REDGPU_2_EXPECTED_maxProcedureParametersVariablesBytesCount_128); // maxPushConstantsSize; Arc A770 on Linux, RX 550 on Windows, Redmi Note 8 on Android 11
  REDGPU_2_EXPECTWG(gpuInfo->maxViewportDimensions[0]                    >= REDGPU_2_EXPECTED_maxViewportDimensionsX_16384); // maxViewportDimensions[0]; D3D12_REQ_TEXTURE2D_U_OR_V_DIMENSION (16384)
  REDGPU_2_EXPECTWG(gpuInfo->maxViewportDimensions[1]                    >= REDGPU_2_EXPECTED_maxViewportDimensionsY_16384); // maxViewportDimensions[1]; D3D12_REQ_TEXTURE2D_U_OR_V_DIMENSION (16384)
  REDGPU_2_EXPECTWG(gpuInfo->minViewportBoundsRange                      <= REDGPU_2_EXPECTED_minViewportBoundsRange_minus_32768); // viewportBoundsRange[0]; D3D12_VIEWPORT_BOUNDS_MIN (-32768)
  REDGPU_2_EXPECTWG(gpuInfo->maxViewportBoundsRange                      >= REDGPU_2_EXPECTED_maxViewportBoundsRange_32767); // viewportBoundsRange[1]; D3D12_VIEWPORT_BOUNDS_MAX (32767)
  REDGPU_2_EXPECTWG(gpuInfo->precisionBitsViewportCoordinates            >= REDGPU_2_EXPECTED_precisionBitsViewportCoordinates_8); // subPixelPrecisionBits; D3D12_SUBPIXEL_FRACTIONAL_BIT_COUNT (8)
  REDGPU_2_EXPECTWG(gpuInfo->precisionBitsViewportBounds                 >= REDGPU_2_EXPECTED_precisionBitsViewportBounds_8); // viewportSubPixelBits; D3D12_SUBPIXEL_FRACTIONAL_BIT_COUNT (8)
  REDGPU_2_EXPECTWG(gpuInfo->maxPerStageClipDistances                    >= REDGPU_2_EXPECTED_maxPerStageClipDistances_8); // maxClipDistances; D3D12_CLIP_OR_CULL_DISTANCE_COUNT (8)
  REDGPU_2_EXPECTWG(gpuInfo->maxPerStageCullDistances                    >= REDGPU_2_EXPECTED_maxPerStageCullDistances_8); // maxCullDistances; D3D12_CLIP_OR_CULL_DISTANCE_COUNT (8)
  REDGPU_2_EXPECTWG(gpuInfo->maxPerStageClipAndCullDistances             >= REDGPU_2_EXPECTED_maxPerStageClipAndCullDistances_8); // maxCombinedClipAndCullDistances; D3D12_CLIP_OR_CULL_DISTANCE_COUNT (8)
  REDGPU_2_EXPECTWG(gpuInfo->maxVertexOutputLocationsCount               >= REDGPU_2_EXPECTED_maxVertexOutputLocationsCount_128); // maxVertexOutputComponents; D3D12_VS_INPUT_REGISTER_COUNT (32) * D3D12_VS_INPUT_REGISTER_COMPONENTS (4)
  REDGPU_2_EXPECTWG(gpuInfo->maxFragmentInputLocationsCount              >= REDGPU_2_EXPECTED_maxFragmentInputLocationsCount_112); // maxFragmentInputComponents; Redmi Note 8 on Android 11; Not used: Arc A770 and Arc B580 on Linux is 116; Not used: D3D12_PS_INPUT_REGISTER_COUNT (32) * D3D12_PS_INPUT_REGISTER_COMPONENTS (4)
  REDGPU_2_EXPECTWG(gpuInfo->maxFragmentOutputColorsCount                >= REDGPU_2_EXPECTED_maxFragmentOutputColorsCount_8); // maxFragmentOutputAttachments; D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT (8)
  REDGPU_2_EXPECTWG(gpuInfo->maxFragmentOutputColorsCountBlendDualSource >= REDGPU_2_EXPECTED_maxFragmentOutputColorsCountBlendDualSource_1); // maxFragmentDualSrcAttachments
  REDGPU_2_EXPECTWG(gpuInfo->maxFragmentOutputResourcesCount             >= REDGPU_2_EXPECTED_maxFragmentOutputResourcesCount_16); // maxFragmentCombinedOutputResources; Tegra X1 on Linux, D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT (8) + D3D12_PS_CS_UAV_REGISTER_COUNT (8); Not used: Redmi Note 8 on Android 11 is 72
  REDGPU_2_EXPECTWG(gpuInfo->maxComputeSharedMemoryBytesCount            >= REDGPU_2_EXPECTED_maxComputeSharedMemoryBytesCount_16384); // maxComputeSharedMemorySize; Redmi Note 8 on Android 11, D3D12_CS_THREAD_LOCAL_TEMP_REGISTER_POOL (16384); Not used: Arc A770 and Arc B580 and RX 9070 XT on Windows is 32768
  REDGPU_2_EXPECTWG(gpuInfo->maxComputeWorkgroupsCount[0]                >= REDGPU_2_EXPECTED_maxComputeWorkgroupsCountX_65535); // maxComputeWorkGroupCount[0]; D3D12_CS_DISPATCH_MAX_THREAD_GROUPS_PER_DIMENSION (65535)
  REDGPU_2_EXPECTWG(gpuInfo->maxComputeWorkgroupsCount[1]                >= REDGPU_2_EXPECTED_maxComputeWorkgroupsCountY_65535); // maxComputeWorkGroupCount[1]; D3D12_CS_DISPATCH_MAX_THREAD_GROUPS_PER_DIMENSION (65535)
  REDGPU_2_EXPECTWG(gpuInfo->maxComputeWorkgroupsCount[2]                >= REDGPU_2_EXPECTED_maxComputeWorkgroupsCountZ_65535); // maxComputeWorkGroupCount[2]; D3D12_CS_DISPATCH_MAX_THREAD_GROUPS_PER_DIMENSION (65535)
  REDGPU_2_EXPECTWG(gpuInfo->maxComputeWorkgroupInvocationsCount         >= REDGPU_2_EXPECTED_maxComputeWorkgroupInvocationsCount_1024); // maxComputeWorkGroupInvocations; D3D12_CS_THREAD_GROUP_MAX_THREADS_PER_GROUP (1024)
  REDGPU_2_EXPECTWG(gpuInfo->maxComputeWorkgroupDimensions[0]            >= REDGPU_2_EXPECTED_maxComputeWorkgroupDimensionsX_1024); // maxComputeWorkGroupSize[0]; D3D12_CS_THREAD_GROUP_MAX_X (1024)
  REDGPU_2_EXPECTWG(gpuInfo->maxComputeWorkgroupDimensions[1]            >= REDGPU_2_EXPECTED_maxComputeWorkgroupDimensionsY_1024); // maxComputeWorkGroupSize[1]; D3D12_CS_THREAD_GROUP_MAX_Y (1024)
  REDGPU_2_EXPECTWG(gpuInfo->maxComputeWorkgroupDimensions[2]            >= REDGPU_2_EXPECTED_maxComputeWorkgroupDimensionsZ_64); // maxComputeWorkGroupSize[2]; D3D12_CS_THREAD_GROUP_MAX_Z (64)
  REDGPU_2_EXPECTWG(gpuInfo->minImageSampleImageFetchOffset              <= REDGPU_2_EXPECTED_minImageSampleImageFetchOffset_minus_8); // minTexelOffset; D3D12_COMMONSHADER_TEXEL_OFFSET_MAX_NEGATIVE (-8)
  REDGPU_2_EXPECTWG(gpuInfo->maxImageSampleImageFetchOffset              >= REDGPU_2_EXPECTED_maxImageSampleImageFetchOffset_7); // maxTexelOffset; D3D12_COMMONSHADER_TEXEL_OFFSET_MAX_POSITIVE (7)
  REDGPU_2_EXPECTWG(gpuInfo->minImageGatherOffset                        <= REDGPU_2_EXPECTED_minImageGatherOffset_minus_32); // minTexelGatherOffset
  REDGPU_2_EXPECTWG(gpuInfo->maxImageGatherOffset                        >= REDGPU_2_EXPECTED_maxImageGatherOffset_31); // maxTexelGatherOffset
  REDGPU_2_EXPECTWG(gpuInfo->minInterpolateAtOffset                      <= -0.5f); // minInterpolationOffset
  REDGPU_2_EXPECTWG(gpuInfo->maxInterpolateAtOffset                      >= 0.4375f); // maxInterpolationOffset
  REDGPU_2_EXPECTWG(gpuInfo->precisionBitsInterpolateAtOffset            >= 4); // subPixelInterpolationOffsetBits
  REDGPU_2_EXPECTWG(gpuInfo->optimalCopyArrayImageRangeArrayBytesFirstBytesAlignment           <= REDGPU_2_EXPECTED_optimalCopyArrayImageRangeArrayBytesFirstBytesAlignment_128); // optimalBufferCopyOffsetAlignment
  REDGPU_2_EXPECTWG(gpuInfo->optimalCopyArrayImageRangeArrayTexelsCountToNextRowBytesAlignment <= REDGPU_2_EXPECTED_optimalCopyArrayImageRangeArrayTexelsCountToNextRowBytesAlignment_128); // optimalBufferCopyRowPitchAlignment
  REDGPU_2_EXPECTWG(gpuInfo->supportsWsi                                                       == 1);
  REDGPU_2_EXPECTWG(gpuInfo->supportsMemoryGetBudget                                           >= 0); // Not supported on Redmi Note 8 on Android 11
  REDGPU_2_EXPECTWG(gpuInfo->supportsFullArrayIndexUint32Value                                 == 1);
  REDGPU_2_EXPECTWG(gpuInfo->supportsSamplerAnisotropy                                         == 1);
  REDGPU_2_EXPECTWG(gpuInfo->supportsTextureDimensionsCubeLayered                              == 1);
  REDGPU_2_EXPECTWG(gpuInfo->supportsProcedureStateRasterizationDepthClamp                     == 1);
  REDGPU_2_EXPECTWG(gpuInfo->supportsProcedureStateRasterizationDepthBiasDynamic               == 1);
  REDGPU_2_EXPECTWG(gpuInfo->supportsProcedureStateRasterizationDepthBiasClamp                 == 1);
  REDGPU_2_EXPECTWG(gpuInfo->supportsProcedureStateMultisampleSampleShading                    == 1);
  REDGPU_2_EXPECTWG(gpuInfo->supportsProcedureStateMultisampleAlphaToOne                       >= 0); // Not supported on RX 550 and RX 9070 XT on Windows
  REDGPU_2_EXPECTWG(gpuInfo->supportsProcedureStateDepthTestBoundsTest                         >= 0); // Not supported on Intel HD Graphics 520 on Windows
  REDGPU_2_EXPECTWG(gpuInfo->supportsProcedureStateDepthTestBoundsTestDynamic                  >= 0); // Not supported on Intel HD Graphics 520 on Windows
  REDGPU_2_EXPECTWG(gpuInfo->supportsProcedureStateStencilTestFrontAndBackDynamicCompareMask   == 1);
  REDGPU_2_EXPECTWG(gpuInfo->supportsProcedureStateStencilTestFrontAndBackDynamicWriteMask     == 1);
  REDGPU_2_EXPECTWG(gpuInfo->supportsProcedureStateBlendLogicOp                                >= 0); // Not supported on Redmi Note 8 on Android 11
  REDGPU_2_EXPECTWG(gpuInfo->supportsProcedureStateOutputColorsBlendVaryingPerColor            == 1);
  REDGPU_2_EXPECTWG(gpuInfo->supportsProcedureStateOutputColorsBlendDualSource                 == 1);
  REDGPU_2_EXPECTWG(gpuInfo->supportsMultisampleEmptyOutputVariableMultisampleCount            >= 0); // Not supported on Redmi Note 8 on Android 11
  REDGPU_2_EXPECTWG(gpuInfo->supportsMultisampleStandardSampleLocations                        == 1);
  REDGPU_2_EXPECTWG(gpuInfo->supportsGpuCodeWritesAndAtomicsInStageVertex                      == 1);
  REDGPU_2_EXPECTWG(gpuInfo->supportsGpuCodeWritesAndAtomicsInStageFragment                    == 1);
  REDGPU_2_EXPECTWG(gpuInfo->supportsGpuCodeImageGatherExtended                                == 1);
  REDGPU_2_EXPECTWG(gpuInfo->supportsGpuCodeTextureRWExtendedFormats                           == 1);
  REDGPU_2_EXPECTWG(gpuInfo->supportsGpuCodeTextureRWMultisample                               >= 0); // Not supported on Redmi Note 8 on Android 11
  REDGPU_2_EXPECTWG(gpuInfo->supportsGpuCodeTextureRWReadWithoutFormat                         >= 0); // Intel UHD Graphics 730 on Windows 10
  REDGPU_2_EXPECTWG(gpuInfo->supportsGpuCodeTextureRWWriteWithoutFormat                        == 1);
  REDGPU_2_EXPECTWG(gpuInfo->supportsGpuCodeDynamicallyIndexableArraysOfArrayROCs              == 1);
  REDGPU_2_EXPECTWG(gpuInfo->supportsGpuCodeDynamicallyIndexableArraysOfArrayRORWs             == 1);
  REDGPU_2_EXPECTWG(gpuInfo->supportsGpuCodeDynamicallyIndexableArraysOfSamplers               == 1);
  REDGPU_2_EXPECTWG(gpuInfo->supportsGpuCodeDynamicallyIndexableArraysOfTextureROs             == 1);
  REDGPU_2_EXPECTWG(gpuInfo->supportsGpuCodeDynamicallyIndexableArraysOfTextureRWs             == 1);
  REDGPU_2_EXPECTWG(gpuInfo->supportsGpuCodeClipDistance                                       == 1);
  REDGPU_2_EXPECTWG(gpuInfo->supportsGpuCodeCullDistance                                       == 1);
  REDGPU_2_EXPECTWG(gpuInfo->supportsGpuCodeInt64                                              >= 0); // Not supported on Redmi Note 8 on Android 11
  REDGPU_2_EXPECTWG(gpuInfo->supportsGpuCodeFloat64                                            >= 0); // Not supported on Redmi Note 8 on Android 11
  REDGPU_2_EXPECTWG(gpuInfo->supportsGpuCodeMinLod                                             >= 0); // Not supported on Redmi Note 8 on Android 11
}

REDGPU_2_DECLSPEC void REDGPU_2_API red2ExpectMinimumGuaranteesIntelUHDGraphics730(const RedGpuInfo * gpuInfo, const char * optionalFile, int optionalLine) {
  RedHandleGpu gpu = gpuInfo->gpu;
  REDGPU_2_EXPECTWG(gpuInfo->maxMemoryAllocateCount                      >= REDGPU_2_EXPECTED_maxMemoryAllocateCount_4096); // maxMemoryAllocationCount
  REDGPU_2_EXPECTWG(gpuInfo->minMemoryAllocateBytesAlignment             <= REDGPU_2_EXPECTED_minMemoryAllocateBytesAlignment_4096); // minMemoryMapAlignment; RX 550 and Arc A770 and Arc B580 on Linux
  REDGPU_2_EXPECTWG(gpuInfo->maxMemoryAllocateBytesCount                 >= REDGPU_2_EXPECTED_maxMemoryAllocateBytesCount_1073741824); // maxMemoryAllocationSize; Redmi Note 8 on Android 11; Not used: RX 550 and RX 9070 XT on Windows is 2147483648
  REDGPU_2_EXPECTWG(gpuInfo->minMemoryNonCoherentBlockBytesCount         <= REDGPU_2_EXPECTED_minMemoryNonCoherentBlockBytesCount_256); // nonCoherentAtomSize
  REDGPU_2_EXPECTWG(gpuInfo->maxCreateSamplerCount                       >= REDGPU_2_EXPECTED_maxCreateSamplerCount_4000); // maxSamplerAllocationCount
  REDGPU_2_EXPECTWG(gpuInfo->minMemoryPageSeparationArrayImageBytesCount <= REDGPU_2_EXPECTED_minMemoryPageSeparationArrayImageBytesCount_65536); // bufferImageGranularity; D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT (65536)
  REDGPU_2_EXPECTWG(gpuInfo->minArrayROCStructMemberRangeBytesAlignment  <= REDGPU_2_EXPECTED_minArrayROCStructMemberRangeBytesAlignment_256); // minUniformBufferOffsetAlignment; D3D12_CONSTANT_BUFFER_DATA_PLACEMENT_ALIGNMENT (256)
  REDGPU_2_EXPECTWG(gpuInfo->maxArrayROCStructMemberRangeBytesCount      >= REDGPU_2_EXPECTED_maxArrayROCStructMemberRangeBytesCount_65536); // maxUniformBufferRange; D3D12_REQ_CONSTANT_BUFFER_ELEMENT_COUNT (4096) * 16
  REDGPU_2_EXPECTWG(gpuInfo->minArrayRORWStructMemberRangeBytesAlignment <= REDGPU_2_EXPECTED_minArrayRORWStructMemberRangeBytesAlignment_64); // minStorageBufferOffsetAlignment; Arc A770 and Arc B580 on Windows, Redmi Note 8 on Android 11; Not used: D3D12_RAW_UAV_SRV_BYTE_ALIGNMENT (16)
  REDGPU_2_EXPECTWG(gpuInfo->maxArrayRORWStructMemberRangeBytesCount     >= REDGPU_2_EXPECTED_maxArrayRORWStructMemberRangeBytesCount_1073741820); // maxStorageBufferRange; Arc A770 and Arc B580 on Windows
  REDGPU_2_EXPECTWG(gpuInfo->maxArrayIndexUint32Value                    == 0xFFFFFFFF); // maxDrawIndexedIndexValue
  REDGPU_2_EXPECTWG(gpuInfo->maxImageDimensions1D                        >= REDGPU_2_EXPECTED_maxImageDimensions1D_16384); // maxImageDimension1D; D3D12_REQ_TEXTURE1D_U_DIMENSION (16384)
  REDGPU_2_EXPECTWG(gpuInfo->maxImageDimensions2D                        >= REDGPU_2_EXPECTED_maxImageDimensions2D_16384); // maxImageDimension2D; D3D12_REQ_TEXTURE2D_U_OR_V_DIMENSION (16384)
  REDGPU_2_EXPECTWG(gpuInfo->maxImageDimensions3D                        >= REDGPU_2_EXPECTED_maxImageDimensions3D_2048); // maxImageDimension3D; D3D12_REQ_TEXTURE3D_U_V_OR_W_DIMENSION (2048)
  REDGPU_2_EXPECTWG(gpuInfo->maxImageDimensionsCube                      >= REDGPU_2_EXPECTED_maxImageDimensionsCube_16384); // maxImageDimensionCube; D3D12_REQ_TEXTURECUBE_DIMENSION (16384)
  REDGPU_2_EXPECTWG(gpuInfo->maxImageLayersCount                         >= REDGPU_2_EXPECTED_maxImageLayersCount_2048); // maxImageArrayLayers; D3D12_REQ_TEXTURE2D_ARRAY_AXIS_DIMENSION (2048)
  REDGPU_2_EXPECTWG(gpuInfo->maxSamplerMipLodBias                        >= REDGPU_2_EXPECTED_maxSamplerMipLodBias_15); // maxSamplerLodBias; RTX 2060; Not used: D3D12_MIP_LOD_BIAS_MAX (15.99f)
  REDGPU_2_EXPECTWG(gpuInfo->maxSamplerAnisotropy                        >= REDGPU_2_EXPECTED_maxSamplerAnisotropy_16); // maxSamplerAnisotropy; D3D12_MAX_MAXANISOTROPY (16)
  REDGPU_2_EXPECTWG(gpuInfo->precisionBitsSamplerFilteringMagMin         >= REDGPU_2_EXPECTED_precisionBitsSamplerFilteringMagMin_8); // subTexelPrecisionBits; D3D12_SUBTEXEL_FRACTIONAL_BIT_COUNT (8)
  REDGPU_2_EXPECTWG(gpuInfo->precisionBitsSamplerFilteringMip            >= REDGPU_2_EXPECTED_precisionBitsSamplerFilteringMip_8); // mipmapPrecisionBits; D3D12_MIP_LOD_FRACTIONAL_BIT_COUNT (8)
  REDGPU_2_EXPECTWG(gpuInfo->maxOutputWidth                              >= REDGPU_2_EXPECTED_maxOutputWidth_16384); // maxFramebufferWidth; D3D12_REQ_TEXTURE2D_U_OR_V_DIMENSION (16384)
  REDGPU_2_EXPECTWG(gpuInfo->maxOutputHeight                             >= REDGPU_2_EXPECTED_maxOutputHeight_16384); // maxFramebufferHeight; D3D12_REQ_TEXTURE2D_U_OR_V_DIMENSION (16384)
  REDGPU_2_EXPECTWG(gpuInfo->maxOutputColorsCount                        >= REDGPU_2_EXPECTED_maxOutputColorsCount_8); // maxColorAttachments; D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT (8)
  REDGPU_2_EXPECTWG(gpuInfo->supportedMultisampleCountsForOutputDepth    >= (RED_MULTISAMPLE_COUNT_BITFLAG_1 | RED_MULTISAMPLE_COUNT_BITFLAG_4));
  REDGPU_2_EXPECTWG(gpuInfo->supportedMultisampleCountsForOutputStencil  >= (RED_MULTISAMPLE_COUNT_BITFLAG_1 | RED_MULTISAMPLE_COUNT_BITFLAG_4));
  REDGPU_2_EXPECTWG(gpuInfo->supportedMultisampleCountsForOutputColor    >= (RED_MULTISAMPLE_COUNT_BITFLAG_1 | RED_MULTISAMPLE_COUNT_BITFLAG_4));
  REDGPU_2_EXPECTWG(gpuInfo->supportedMultisampleCountsForEmptyOutput    >= (RED_MULTISAMPLE_COUNT_BITFLAG_1 | RED_MULTISAMPLE_COUNT_BITFLAG_4));
  REDGPU_2_EXPECTWG(gpuInfo->maxStructSlotsCount                         >= REDGPU_2_EXPECTED_maxStructSlotsCount_1024); // maxPerSetDescriptors; Arc A770 and Arc B580 on Linux
  REDGPU_2_EXPECTWG(gpuInfo->maxStructPerStageArrayROCsCount             >= REDGPU_2_EXPECTED_maxStructPerStageArrayROCsCount_12); // maxPerStageDescriptorUniformBuffers; Not used: Tegra X1 on Linux is 15
  REDGPU_2_EXPECTWG(gpuInfo->maxStructPerStageArrayRORWsCount            >= 200); // Intel UHD Graphics 730 on Windows 10
  REDGPU_2_EXPECTWG(gpuInfo->maxStructPerStageSamplersCount              >= 64); // Intel UHD Graphics 730 on Windows 10
  REDGPU_2_EXPECTWG(gpuInfo->maxStructPerStageTextureROsCount            >= 200); // Intel UHD Graphics 730 on Windows 10
  REDGPU_2_EXPECTWG(gpuInfo->maxStructPerStageTextureRWsCount            >= 16); // Intel UHD Graphics 730 on Windows 10
  REDGPU_2_EXPECTWG(gpuInfo->maxStructPerStageAllResourcesCount          >= 200); // Intel UHD Graphics 730 on Windows 10
  REDGPU_2_EXPECTWG(gpuInfo->maxStructArrayROCsCount                     >= 1800); // Intel UHD Graphics 730 on Windows 10
  REDGPU_2_EXPECTWG(gpuInfo->maxStructArrayRORWsCount                    >= 1800); // Intel UHD Graphics 730 on Windows 10
  REDGPU_2_EXPECTWG(gpuInfo->maxStructSamplersCount                      >= 576); // Intel UHD Graphics 730 on Windows 10
  REDGPU_2_EXPECTWG(gpuInfo->maxStructTextureROsCount                    >= 1800); // Intel UHD Graphics 730 on Windows 10
  REDGPU_2_EXPECTWG(gpuInfo->maxStructTextureRWsCount                    >= 144); // Intel UHD Graphics 730 on Windows 10
  REDGPU_2_EXPECTWG(gpuInfo->maxProcedureParametersVariablesBytesCount   >= REDGPU_2_EXPECTED_maxProcedureParametersVariablesBytesCount_128); // maxPushConstantsSize; Arc A770 on Linux, RX 550 on Windows, Redmi Note 8 on Android 11
  REDGPU_2_EXPECTWG(gpuInfo->maxViewportDimensions[0]                    >= REDGPU_2_EXPECTED_maxViewportDimensionsX_16384); // maxViewportDimensions[0]; D3D12_REQ_TEXTURE2D_U_OR_V_DIMENSION (16384)
  REDGPU_2_EXPECTWG(gpuInfo->maxViewportDimensions[1]                    >= REDGPU_2_EXPECTED_maxViewportDimensionsY_16384); // maxViewportDimensions[1]; D3D12_REQ_TEXTURE2D_U_OR_V_DIMENSION (16384)
  REDGPU_2_EXPECTWG(gpuInfo->minViewportBoundsRange                      <= REDGPU_2_EXPECTED_minViewportBoundsRange_minus_32768); // viewportBoundsRange[0]; D3D12_VIEWPORT_BOUNDS_MIN (-32768)
  REDGPU_2_EXPECTWG(gpuInfo->maxViewportBoundsRange                      >= REDGPU_2_EXPECTED_maxViewportBoundsRange_32767); // viewportBoundsRange[1]; D3D12_VIEWPORT_BOUNDS_MAX (32767)
  REDGPU_2_EXPECTWG(gpuInfo->precisionBitsViewportCoordinates            >= REDGPU_2_EXPECTED_precisionBitsViewportCoordinates_8); // subPixelPrecisionBits; D3D12_SUBPIXEL_FRACTIONAL_BIT_COUNT (8)
  REDGPU_2_EXPECTWG(gpuInfo->precisionBitsViewportBounds                 >= REDGPU_2_EXPECTED_precisionBitsViewportBounds_8); // viewportSubPixelBits; D3D12_SUBPIXEL_FRACTIONAL_BIT_COUNT (8)
  REDGPU_2_EXPECTWG(gpuInfo->maxPerStageClipDistances                    >= REDGPU_2_EXPECTED_maxPerStageClipDistances_8); // maxClipDistances; D3D12_CLIP_OR_CULL_DISTANCE_COUNT (8)
  REDGPU_2_EXPECTWG(gpuInfo->maxPerStageCullDistances                    >= REDGPU_2_EXPECTED_maxPerStageCullDistances_8); // maxCullDistances; D3D12_CLIP_OR_CULL_DISTANCE_COUNT (8)
  REDGPU_2_EXPECTWG(gpuInfo->maxPerStageClipAndCullDistances             >= REDGPU_2_EXPECTED_maxPerStageClipAndCullDistances_8); // maxCombinedClipAndCullDistances; D3D12_CLIP_OR_CULL_DISTANCE_COUNT (8)
  REDGPU_2_EXPECTWG(gpuInfo->maxVertexOutputLocationsCount               >= REDGPU_2_EXPECTED_maxVertexOutputLocationsCount_128); // maxVertexOutputComponents; D3D12_VS_INPUT_REGISTER_COUNT (32) * D3D12_VS_INPUT_REGISTER_COMPONENTS (4)
  REDGPU_2_EXPECTWG(gpuInfo->maxFragmentInputLocationsCount              >= REDGPU_2_EXPECTED_maxFragmentInputLocationsCount_112); // maxFragmentInputComponents; Redmi Note 8 on Android 11; Not used: Arc A770 and Arc B580 on Linux is 116; Not used: D3D12_PS_INPUT_REGISTER_COUNT (32) * D3D12_PS_INPUT_REGISTER_COMPONENTS (4)
  REDGPU_2_EXPECTWG(gpuInfo->maxFragmentOutputColorsCount                >= REDGPU_2_EXPECTED_maxFragmentOutputColorsCount_8); // maxFragmentOutputAttachments; D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT (8)
  REDGPU_2_EXPECTWG(gpuInfo->maxFragmentOutputColorsCountBlendDualSource >= REDGPU_2_EXPECTED_maxFragmentOutputColorsCountBlendDualSource_1); // maxFragmentDualSrcAttachments
  REDGPU_2_EXPECTWG(gpuInfo->maxFragmentOutputResourcesCount             >= REDGPU_2_EXPECTED_maxFragmentOutputResourcesCount_16); // maxFragmentCombinedOutputResources; Tegra X1 on Linux, D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT (8) + D3D12_PS_CS_UAV_REGISTER_COUNT (8); Not used: Redmi Note 8 on Android 11 is 72
  REDGPU_2_EXPECTWG(gpuInfo->maxComputeSharedMemoryBytesCount            >= REDGPU_2_EXPECTED_maxComputeSharedMemoryBytesCount_16384); // maxComputeSharedMemorySize; Redmi Note 8 on Android 11, D3D12_CS_THREAD_LOCAL_TEMP_REGISTER_POOL (16384); Not used: Arc A770 and Arc B580 and RX 9070 XT on Windows is 32768
  REDGPU_2_EXPECTWG(gpuInfo->maxComputeWorkgroupsCount[0]                >= REDGPU_2_EXPECTED_maxComputeWorkgroupsCountX_65535); // maxComputeWorkGroupCount[0]; D3D12_CS_DISPATCH_MAX_THREAD_GROUPS_PER_DIMENSION (65535)
  REDGPU_2_EXPECTWG(gpuInfo->maxComputeWorkgroupsCount[1]                >= REDGPU_2_EXPECTED_maxComputeWorkgroupsCountY_65535); // maxComputeWorkGroupCount[1]; D3D12_CS_DISPATCH_MAX_THREAD_GROUPS_PER_DIMENSION (65535)
  REDGPU_2_EXPECTWG(gpuInfo->maxComputeWorkgroupsCount[2]                >= REDGPU_2_EXPECTED_maxComputeWorkgroupsCountZ_65535); // maxComputeWorkGroupCount[2]; D3D12_CS_DISPATCH_MAX_THREAD_GROUPS_PER_DIMENSION (65535)
  REDGPU_2_EXPECTWG(gpuInfo->maxComputeWorkgroupInvocationsCount         >= REDGPU_2_EXPECTED_maxComputeWorkgroupInvocationsCount_1024); // maxComputeWorkGroupInvocations; D3D12_CS_THREAD_GROUP_MAX_THREADS_PER_GROUP (1024)
  REDGPU_2_EXPECTWG(gpuInfo->maxComputeWorkgroupDimensions[0]            >= REDGPU_2_EXPECTED_maxComputeWorkgroupDimensionsX_1024); // maxComputeWorkGroupSize[0]; D3D12_CS_THREAD_GROUP_MAX_X (1024)
  REDGPU_2_EXPECTWG(gpuInfo->maxComputeWorkgroupDimensions[1]            >= REDGPU_2_EXPECTED_maxComputeWorkgroupDimensionsY_1024); // maxComputeWorkGroupSize[1]; D3D12_CS_THREAD_GROUP_MAX_Y (1024)
  REDGPU_2_EXPECTWG(gpuInfo->maxComputeWorkgroupDimensions[2]            >= REDGPU_2_EXPECTED_maxComputeWorkgroupDimensionsZ_64); // maxComputeWorkGroupSize[2]; D3D12_CS_THREAD_GROUP_MAX_Z (64)
  REDGPU_2_EXPECTWG(gpuInfo->minImageSampleImageFetchOffset              <= REDGPU_2_EXPECTED_minImageSampleImageFetchOffset_minus_8); // minTexelOffset; D3D12_COMMONSHADER_TEXEL_OFFSET_MAX_NEGATIVE (-8)
  REDGPU_2_EXPECTWG(gpuInfo->maxImageSampleImageFetchOffset              >= REDGPU_2_EXPECTED_maxImageSampleImageFetchOffset_7); // maxTexelOffset; D3D12_COMMONSHADER_TEXEL_OFFSET_MAX_POSITIVE (7)
  REDGPU_2_EXPECTWG(gpuInfo->minImageGatherOffset                        <= REDGPU_2_EXPECTED_minImageGatherOffset_minus_32); // minTexelGatherOffset
  REDGPU_2_EXPECTWG(gpuInfo->maxImageGatherOffset                        >= REDGPU_2_EXPECTED_maxImageGatherOffset_31); // maxTexelGatherOffset
  REDGPU_2_EXPECTWG(gpuInfo->minInterpolateAtOffset                      <= -0.5f); // minInterpolationOffset
  REDGPU_2_EXPECTWG(gpuInfo->maxInterpolateAtOffset                      >= 0.4375f); // maxInterpolationOffset
  REDGPU_2_EXPECTWG(gpuInfo->precisionBitsInterpolateAtOffset            >= 4); // subPixelInterpolationOffsetBits
  REDGPU_2_EXPECTWG(gpuInfo->optimalCopyArrayImageRangeArrayBytesFirstBytesAlignment           <= REDGPU_2_EXPECTED_optimalCopyArrayImageRangeArrayBytesFirstBytesAlignment_128); // optimalBufferCopyOffsetAlignment
  REDGPU_2_EXPECTWG(gpuInfo->optimalCopyArrayImageRangeArrayTexelsCountToNextRowBytesAlignment <= REDGPU_2_EXPECTED_optimalCopyArrayImageRangeArrayTexelsCountToNextRowBytesAlignment_128); // optimalBufferCopyRowPitchAlignment
  REDGPU_2_EXPECTWG(gpuInfo->supportsWsi                                                       == 1);
  REDGPU_2_EXPECTWG(gpuInfo->supportsMemoryGetBudget                                           >= 0); // Not supported on Redmi Note 8 on Android 11
  REDGPU_2_EXPECTWG(gpuInfo->supportsFullArrayIndexUint32Value                                 == 1);
  REDGPU_2_EXPECTWG(gpuInfo->supportsSamplerAnisotropy                                         == 1);
  REDGPU_2_EXPECTWG(gpuInfo->supportsTextureDimensionsCubeLayered                              == 1);
  REDGPU_2_EXPECTWG(gpuInfo->supportsProcedureStateRasterizationDepthClamp                     == 1);
  REDGPU_2_EXPECTWG(gpuInfo->supportsProcedureStateRasterizationDepthBiasDynamic               == 1);
  REDGPU_2_EXPECTWG(gpuInfo->supportsProcedureStateRasterizationDepthBiasClamp                 == 1);
  REDGPU_2_EXPECTWG(gpuInfo->supportsProcedureStateMultisampleSampleShading                    == 1);
  REDGPU_2_EXPECTWG(gpuInfo->supportsProcedureStateMultisampleAlphaToOne                       >= 0); // Not supported on RX 550 and RX 9070 XT on Windows
  REDGPU_2_EXPECTWG(gpuInfo->supportsProcedureStateDepthTestBoundsTest                         >= 0); // Not supported on Intel HD Graphics 520 on Windows
  REDGPU_2_EXPECTWG(gpuInfo->supportsProcedureStateDepthTestBoundsTestDynamic                  >= 0); // Not supported on Intel HD Graphics 520 on Windows
  REDGPU_2_EXPECTWG(gpuInfo->supportsProcedureStateStencilTestFrontAndBackDynamicCompareMask   == 1);
  REDGPU_2_EXPECTWG(gpuInfo->supportsProcedureStateStencilTestFrontAndBackDynamicWriteMask     == 1);
  REDGPU_2_EXPECTWG(gpuInfo->supportsProcedureStateBlendLogicOp                                >= 0); // Not supported on Redmi Note 8 on Android 11
  REDGPU_2_EXPECTWG(gpuInfo->supportsProcedureStateOutputColorsBlendVaryingPerColor            == 1);
  REDGPU_2_EXPECTWG(gpuInfo->supportsProcedureStateOutputColorsBlendDualSource                 == 1);
  REDGPU_2_EXPECTWG(gpuInfo->supportsMultisampleEmptyOutputVariableMultisampleCount            >= 0); // Not supported on Redmi Note 8 on Android 11
  REDGPU_2_EXPECTWG(gpuInfo->supportsMultisampleStandardSampleLocations                        == 1);
  REDGPU_2_EXPECTWG(gpuInfo->supportsGpuCodeWritesAndAtomicsInStageVertex                      == 1);
  REDGPU_2_EXPECTWG(gpuInfo->supportsGpuCodeWritesAndAtomicsInStageFragment                    == 1);
  REDGPU_2_EXPECTWG(gpuInfo->supportsGpuCodeImageGatherExtended                                == 1);
  REDGPU_2_EXPECTWG(gpuInfo->supportsGpuCodeTextureRWExtendedFormats                           == 1);
  REDGPU_2_EXPECTWG(gpuInfo->supportsGpuCodeTextureRWMultisample                               >= 0); // Not supported on Redmi Note 8 on Android 11
  REDGPU_2_EXPECTWG(gpuInfo->supportsGpuCodeTextureRWReadWithoutFormat                         >= 0); // Intel UHD Graphics 730 on Windows 10
  REDGPU_2_EXPECTWG(gpuInfo->supportsGpuCodeTextureRWWriteWithoutFormat                        == 1);
  REDGPU_2_EXPECTWG(gpuInfo->supportsGpuCodeDynamicallyIndexableArraysOfArrayROCs              == 1);
  REDGPU_2_EXPECTWG(gpuInfo->supportsGpuCodeDynamicallyIndexableArraysOfArrayRORWs             == 1);
  REDGPU_2_EXPECTWG(gpuInfo->supportsGpuCodeDynamicallyIndexableArraysOfSamplers               == 1);
  REDGPU_2_EXPECTWG(gpuInfo->supportsGpuCodeDynamicallyIndexableArraysOfTextureROs             == 1);
  REDGPU_2_EXPECTWG(gpuInfo->supportsGpuCodeDynamicallyIndexableArraysOfTextureRWs             == 1);
  REDGPU_2_EXPECTWG(gpuInfo->supportsGpuCodeClipDistance                                       == 1);
  REDGPU_2_EXPECTWG(gpuInfo->supportsGpuCodeCullDistance                                       == 1);
  REDGPU_2_EXPECTWG(gpuInfo->supportsGpuCodeInt64                                              >= 0); // Not supported on Redmi Note 8 on Android 11
  REDGPU_2_EXPECTWG(gpuInfo->supportsGpuCodeFloat64                                            >= 0); // Not supported on Redmi Note 8 on Android 11
  REDGPU_2_EXPECTWG(gpuInfo->supportsGpuCodeMinLod                                             >= 0); // Not supported on Redmi Note 8 on Android 11
}

REDGPU_2_DECLSPEC void REDGPU_2_API red2ExpectAllMemoryToBeCoherent(const RedGpuInfo * gpuInfo, const char * optionalFile, int optionalLine) {
  RedHandleGpu gpu = gpuInfo->gpu;
  for (unsigned i = 0; i < gpuInfo->memoryTypesCount; i += 1) {
    if (gpuInfo->memoryTypes[i].isCpuMappable == 1) {
      REDGPU_2_EXPECTWG(gpuInfo->memoryTypes[i].isCpuCoherent == 1);
    }
  }
}

REDGPU_2_DECLSPEC void REDGPU_2_API red2ExpectMinimumImageFormatsLimitsAndFeatures(const RedGpuInfo * gpuInfo, const char * optionalFile, int optionalLine) {
  // The following minimum image formats limits and features checks are mostly based on Redmi Note 8 on Android 11, unless comments tell otherwise.
  RedHandleGpu gpu = gpuInfo->gpu;
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_R_8_UINT_TO_FLOAT_0_1].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_R_8_UINT_TO_FLOAT_0_1].maxDimensions.maxHeight >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_R_8_UINT_TO_FLOAT_0_1].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_R_8_UINT_TO_FLOAT_0_1].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_R_8_UINT_TO_FLOAT_0_1].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_R_8_UINT_TO_FLOAT_0_1].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_R_8_UINT_TO_FLOAT_0_1].maxBytesCount >= 2147483648); // Intel UHD Graphics 730 on Windows 10. Not used: RTX 2060 on Windows 10 is 1099511627776
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_R_8_UINT_TO_FLOAT_0_1].status == RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_R_8_UINT].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_R_8_UINT].maxDimensions.maxHeight >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_R_8_UINT].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_R_8_UINT].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_R_8_UINT].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_R_8_UINT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_R_8_UINT].maxBytesCount >= 2147483648); // Intel UHD Graphics 730 on Windows 10. Not used: RTX 2060 on Windows 10 is 1099511627776
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_R_8_UINT].status == RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_R_8_SINT].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_R_8_SINT].maxDimensions.maxHeight >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_R_8_SINT].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_R_8_SINT].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_R_8_SINT].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_R_8_SINT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_R_8_SINT].maxBytesCount >= 2147483648); // Intel UHD Graphics 730 on Windows 10. Not used: RTX 2060 on Windows 10 is 1099511627776
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_R_8_SINT].status == RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1].maxDimensions.maxHeight >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1].maxBytesCount >= 2147483648); // Intel UHD Graphics 730 on Windows 10. Not used: RTX 2060 on Windows 10 is 1099511627776
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1].status == RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_RGBA_8_8_8_8_UINT].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_RGBA_8_8_8_8_UINT].maxDimensions.maxHeight >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_RGBA_8_8_8_8_UINT].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_RGBA_8_8_8_8_UINT].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_RGBA_8_8_8_8_UINT].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_RGBA_8_8_8_8_UINT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_RGBA_8_8_8_8_UINT].maxBytesCount >= 2147483648); // Intel UHD Graphics 730 on Windows 10. Not used: RTX 2060 on Windows 10 is 1099511627776
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_RGBA_8_8_8_8_UINT].status == RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_RGBA_8_8_8_8_SINT].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_RGBA_8_8_8_8_SINT].maxDimensions.maxHeight >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_RGBA_8_8_8_8_SINT].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_RGBA_8_8_8_8_SINT].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_RGBA_8_8_8_8_SINT].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_RGBA_8_8_8_8_SINT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_RGBA_8_8_8_8_SINT].maxBytesCount >= 2147483648); // Intel UHD Graphics 730 on Windows 10. Not used: RTX 2060 on Windows 10 is 1099511627776
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_RGBA_8_8_8_8_SINT].status == RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1_GAMMA_CORRECTED].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1_GAMMA_CORRECTED].maxDimensions.maxHeight >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1_GAMMA_CORRECTED].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1_GAMMA_CORRECTED].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1_GAMMA_CORRECTED].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1_GAMMA_CORRECTED].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1_GAMMA_CORRECTED].maxBytesCount >= 2147483648); // Intel UHD Graphics 730 on Windows 10. Not used: RTX 2060 on Windows 10 is 1099511627776
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1_GAMMA_CORRECTED].status == RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_PRESENT_BGRA_8_8_8_8_UINT_TO_FLOAT_0_1].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_PRESENT_BGRA_8_8_8_8_UINT_TO_FLOAT_0_1].maxDimensions.maxHeight >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_PRESENT_BGRA_8_8_8_8_UINT_TO_FLOAT_0_1].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_PRESENT_BGRA_8_8_8_8_UINT_TO_FLOAT_0_1].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_PRESENT_BGRA_8_8_8_8_UINT_TO_FLOAT_0_1].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_PRESENT_BGRA_8_8_8_8_UINT_TO_FLOAT_0_1].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_PRESENT_BGRA_8_8_8_8_UINT_TO_FLOAT_0_1].maxBytesCount >= 2147483648); // Intel UHD Graphics 730 on Windows 10. Not used: RTX 2060 on Windows 10 is 1099511627776
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_PRESENT_BGRA_8_8_8_8_UINT_TO_FLOAT_0_1].status == RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_R_16_UINT].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_R_16_UINT].maxDimensions.maxHeight >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_R_16_UINT].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_R_16_UINT].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_R_16_UINT].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_R_16_UINT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_R_16_UINT].maxBytesCount >= 2147483648); // Intel UHD Graphics 730 on Windows 10. Not used: RTX 2060 on Windows 10 is 1099511627776
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_R_16_UINT].status == RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_R_16_SINT].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_R_16_SINT].maxDimensions.maxHeight >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_R_16_SINT].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_R_16_SINT].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_R_16_SINT].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_R_16_SINT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_R_16_SINT].maxBytesCount >= 2147483648); // Intel UHD Graphics 730 on Windows 10. Not used: RTX 2060 on Windows 10 is 1099511627776
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_R_16_SINT].status == RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_R_16_FLOAT].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_R_16_FLOAT].maxDimensions.maxHeight >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_R_16_FLOAT].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_R_16_FLOAT].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_R_16_FLOAT].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_R_16_FLOAT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_R_16_FLOAT].maxBytesCount >= 2147483648); // Intel UHD Graphics 730 on Windows 10. Not used: RTX 2060 on Windows 10 is 1099511627776
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_R_16_FLOAT].status == RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_RGBA_16_16_16_16_UINT].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_RGBA_16_16_16_16_UINT].maxDimensions.maxHeight >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_RGBA_16_16_16_16_UINT].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_RGBA_16_16_16_16_UINT].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_RGBA_16_16_16_16_UINT].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_RGBA_16_16_16_16_UINT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_RGBA_16_16_16_16_UINT].maxBytesCount >= 2147483648); // Intel UHD Graphics 730 on Windows 10. Not used: RTX 2060 on Windows 10 is 1099511627776
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_RGBA_16_16_16_16_UINT].status == RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_RGBA_16_16_16_16_SINT].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_RGBA_16_16_16_16_SINT].maxDimensions.maxHeight >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_RGBA_16_16_16_16_SINT].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_RGBA_16_16_16_16_SINT].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_RGBA_16_16_16_16_SINT].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_RGBA_16_16_16_16_SINT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_RGBA_16_16_16_16_SINT].maxBytesCount >= 2147483648); // Intel UHD Graphics 730 on Windows 10. Not used: RTX 2060 on Windows 10 is 1099511627776
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_RGBA_16_16_16_16_SINT].status == RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_RGBA_16_16_16_16_FLOAT].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_RGBA_16_16_16_16_FLOAT].maxDimensions.maxHeight >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_RGBA_16_16_16_16_FLOAT].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_RGBA_16_16_16_16_FLOAT].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_RGBA_16_16_16_16_FLOAT].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_RGBA_16_16_16_16_FLOAT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_RGBA_16_16_16_16_FLOAT].maxBytesCount >= 2147483648); // Intel UHD Graphics 730 on Windows 10. Not used: RTX 2060 on Windows 10 is 1099511627776
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_RGBA_16_16_16_16_FLOAT].status == RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_R_32_UINT].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_R_32_UINT].maxDimensions.maxHeight >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_R_32_UINT].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_R_32_UINT].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_R_32_UINT].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_R_32_UINT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_R_32_UINT].maxBytesCount >= 2147483648); // Intel UHD Graphics 730 on Windows 10. Not used: RTX 2060 on Windows 10 is 1099511627776
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_R_32_UINT].status == RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_R_32_SINT].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_R_32_SINT].maxDimensions.maxHeight >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_R_32_SINT].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_R_32_SINT].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_R_32_SINT].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_R_32_SINT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_R_32_SINT].maxBytesCount >= 2147483648); // Intel UHD Graphics 730 on Windows 10. Not used: RTX 2060 on Windows 10 is 1099511627776
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_R_32_SINT].status == RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_R_32_FLOAT].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_R_32_FLOAT].maxDimensions.maxHeight >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_R_32_FLOAT].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_R_32_FLOAT].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_R_32_FLOAT].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_R_32_FLOAT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_R_32_FLOAT].maxBytesCount >= 2147483648); // Intel UHD Graphics 730 on Windows 10. Not used: RTX 2060 on Windows 10 is 1099511627776
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_R_32_FLOAT].status == RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_RGBA_32_32_32_32_UINT].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_RGBA_32_32_32_32_UINT].maxDimensions.maxHeight >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_RGBA_32_32_32_32_UINT].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_RGBA_32_32_32_32_UINT].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_RGBA_32_32_32_32_UINT].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_RGBA_32_32_32_32_UINT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_RGBA_32_32_32_32_UINT].maxBytesCount >= 2147483648); // Intel UHD Graphics 730 on Windows 10. Not used: RTX 2060 on Windows 10 is 1099511627776
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_RGBA_32_32_32_32_UINT].status == RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_RGBA_32_32_32_32_SINT].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_RGBA_32_32_32_32_SINT].maxDimensions.maxHeight >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_RGBA_32_32_32_32_SINT].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_RGBA_32_32_32_32_SINT].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_RGBA_32_32_32_32_SINT].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_RGBA_32_32_32_32_SINT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_RGBA_32_32_32_32_SINT].maxBytesCount >= 2147483648); // Intel UHD Graphics 730 on Windows 10. Not used: RTX 2060 on Windows 10 is 1099511627776
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_RGBA_32_32_32_32_SINT].status == RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_RGBA_32_32_32_32_FLOAT].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_RGBA_32_32_32_32_FLOAT].maxDimensions.maxHeight >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_RGBA_32_32_32_32_FLOAT].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_RGBA_32_32_32_32_FLOAT].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_RGBA_32_32_32_32_FLOAT].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_RGBA_32_32_32_32_FLOAT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_RGBA_32_32_32_32_FLOAT].maxBytesCount >= 2147483648); // Intel UHD Graphics 730 on Windows 10. Not used: RTX 2060 on Windows 10 is 1099511627776
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_RGBA_32_32_32_32_FLOAT].status == RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_DEPTH_16_UINT_TO_FLOAT_0_1].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_DEPTH_16_UINT_TO_FLOAT_0_1].maxDimensions.maxHeight >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_DEPTH_16_UINT_TO_FLOAT_0_1].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_DEPTH_16_UINT_TO_FLOAT_0_1].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_DEPTH_16_UINT_TO_FLOAT_0_1].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_DEPTH_16_UINT_TO_FLOAT_0_1].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_DEPTH_16_UINT_TO_FLOAT_0_1].maxBytesCount >= 2147483648); // Intel UHD Graphics 730 on Windows 10. Not used: RTX 2060 on Windows 10 is 1099511627776
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_DEPTH_16_UINT_TO_FLOAT_0_1].status == RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_DEPTH_32_FLOAT].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_DEPTH_32_FLOAT].maxDimensions.maxHeight >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_DEPTH_32_FLOAT].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_DEPTH_32_FLOAT].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_DEPTH_32_FLOAT].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_DEPTH_32_FLOAT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_DEPTH_32_FLOAT].maxBytesCount >= 2147483648); // Intel UHD Graphics 730 on Windows 10. Not used: RTX 2060 on Windows 10 is 1099511627776
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_DEPTH_32_FLOAT].status == RED_STATUS_SUCCESS);
  if (gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_DEPTH_24_UINT_TO_FLOAT_0_1_STENCIL_8_UINT].status != RED_STATUS_ERROR_FORMAT_IS_NOT_SUPPORTED) { // AMD RX 550 on Windows 10 doesn't support this format
    REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_DEPTH_24_UINT_TO_FLOAT_0_1_STENCIL_8_UINT].maxDimensions.maxWidth >= 16384);
    REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_DEPTH_24_UINT_TO_FLOAT_0_1_STENCIL_8_UINT].maxDimensions.maxHeight >= 1);
    REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_DEPTH_24_UINT_TO_FLOAT_0_1_STENCIL_8_UINT].maxDimensions.maxDepth >= 1);
    REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_DEPTH_24_UINT_TO_FLOAT_0_1_STENCIL_8_UINT].maxLevelsCount >= 15);
    REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_DEPTH_24_UINT_TO_FLOAT_0_1_STENCIL_8_UINT].maxLayersCount >= 2048);
    REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_DEPTH_24_UINT_TO_FLOAT_0_1_STENCIL_8_UINT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
    REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_DEPTH_24_UINT_TO_FLOAT_0_1_STENCIL_8_UINT].maxBytesCount >= 2147483648); // Intel UHD Graphics 730 on Windows 10. Not used: RTX 2060 on Windows 10 is 1099511627776
    REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_DEPTH_24_UINT_TO_FLOAT_0_1_STENCIL_8_UINT].status == RED_STATUS_SUCCESS);
  }
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_DEPTH_32_FLOAT_STENCIL_8_UINT].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_DEPTH_32_FLOAT_STENCIL_8_UINT].maxDimensions.maxHeight >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_DEPTH_32_FLOAT_STENCIL_8_UINT].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_DEPTH_32_FLOAT_STENCIL_8_UINT].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_DEPTH_32_FLOAT_STENCIL_8_UINT].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_DEPTH_32_FLOAT_STENCIL_8_UINT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_DEPTH_32_FLOAT_STENCIL_8_UINT].maxBytesCount >= 2147483648); // Intel UHD Graphics 730 on Windows 10. Not used: RTX 2060 on Windows 10 is 1099511627776
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions1D[RED_FORMAT_DEPTH_32_FLOAT_STENCIL_8_UINT].status == RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_R_8_UINT_TO_FLOAT_0_1].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_R_8_UINT_TO_FLOAT_0_1].maxDimensions.maxHeight >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_R_8_UINT_TO_FLOAT_0_1].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_R_8_UINT_TO_FLOAT_0_1].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_R_8_UINT_TO_FLOAT_0_1].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_R_8_UINT_TO_FLOAT_0_1].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_R_8_UINT_TO_FLOAT_0_1].maxBytesCount >= 2147483648); // Intel UHD Graphics 730 on Windows 10. Not used: RTX 2060 on Windows 10 is 1099511627776
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_R_8_UINT_TO_FLOAT_0_1].status == RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_R_8_UINT].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_R_8_UINT].maxDimensions.maxHeight >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_R_8_UINT].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_R_8_UINT].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_R_8_UINT].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_R_8_UINT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_R_8_UINT].maxBytesCount >= 2147483648); // Intel UHD Graphics 730 on Windows 10. Not used: RTX 2060 on Windows 10 is 1099511627776
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_R_8_UINT].status == RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_R_8_SINT].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_R_8_SINT].maxDimensions.maxHeight >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_R_8_SINT].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_R_8_SINT].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_R_8_SINT].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_R_8_SINT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_R_8_SINT].maxBytesCount >= 2147483648); // Intel UHD Graphics 730 on Windows 10. Not used: RTX 2060 on Windows 10 is 1099511627776
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_R_8_SINT].status == RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1].maxDimensions.maxHeight >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1].maxBytesCount >= 2147483648); // Intel UHD Graphics 730 on Windows 10. Not used: RTX 2060 on Windows 10 is 1099511627776
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1].status == RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_RGBA_8_8_8_8_UINT].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_RGBA_8_8_8_8_UINT].maxDimensions.maxHeight >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_RGBA_8_8_8_8_UINT].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_RGBA_8_8_8_8_UINT].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_RGBA_8_8_8_8_UINT].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_RGBA_8_8_8_8_UINT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_RGBA_8_8_8_8_UINT].maxBytesCount >= 2147483648); // Intel UHD Graphics 730 on Windows 10. Not used: RTX 2060 on Windows 10 is 1099511627776
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_RGBA_8_8_8_8_UINT].status == RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_RGBA_8_8_8_8_SINT].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_RGBA_8_8_8_8_SINT].maxDimensions.maxHeight >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_RGBA_8_8_8_8_SINT].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_RGBA_8_8_8_8_SINT].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_RGBA_8_8_8_8_SINT].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_RGBA_8_8_8_8_SINT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_RGBA_8_8_8_8_SINT].maxBytesCount >= 2147483648); // Intel UHD Graphics 730 on Windows 10. Not used: RTX 2060 on Windows 10 is 1099511627776
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_RGBA_8_8_8_8_SINT].status == RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1_GAMMA_CORRECTED].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1_GAMMA_CORRECTED].maxDimensions.maxHeight >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1_GAMMA_CORRECTED].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1_GAMMA_CORRECTED].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1_GAMMA_CORRECTED].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1_GAMMA_CORRECTED].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_4 | RED_MULTISAMPLE_COUNT_BITFLAG_2 | RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1_GAMMA_CORRECTED].maxBytesCount >= 2147483648); // Intel UHD Graphics 730 on Windows 10. Not used: RTX 2060 on Windows 10 is 1099511627776
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1_GAMMA_CORRECTED].status == RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_PRESENT_BGRA_8_8_8_8_UINT_TO_FLOAT_0_1].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_PRESENT_BGRA_8_8_8_8_UINT_TO_FLOAT_0_1].maxDimensions.maxHeight >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_PRESENT_BGRA_8_8_8_8_UINT_TO_FLOAT_0_1].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_PRESENT_BGRA_8_8_8_8_UINT_TO_FLOAT_0_1].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_PRESENT_BGRA_8_8_8_8_UINT_TO_FLOAT_0_1].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_PRESENT_BGRA_8_8_8_8_UINT_TO_FLOAT_0_1].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_4 | RED_MULTISAMPLE_COUNT_BITFLAG_2 | RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_PRESENT_BGRA_8_8_8_8_UINT_TO_FLOAT_0_1].maxBytesCount >= 2147483648); // Intel UHD Graphics 730 on Windows 10. Not used: RTX 2060 on Windows 10 is 1099511627776
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_PRESENT_BGRA_8_8_8_8_UINT_TO_FLOAT_0_1].status == RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_R_16_UINT].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_R_16_UINT].maxDimensions.maxHeight >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_R_16_UINT].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_R_16_UINT].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_R_16_UINT].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_R_16_UINT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_R_16_UINT].maxBytesCount >= 2147483648); // Intel UHD Graphics 730 on Windows 10. Not used: RTX 2060 on Windows 10 is 1099511627776
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_R_16_UINT].status == RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_R_16_SINT].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_R_16_SINT].maxDimensions.maxHeight >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_R_16_SINT].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_R_16_SINT].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_R_16_SINT].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_R_16_SINT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_R_16_SINT].maxBytesCount >= 2147483648); // Intel UHD Graphics 730 on Windows 10. Not used: RTX 2060 on Windows 10 is 1099511627776
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_R_16_SINT].status == RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_R_16_FLOAT].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_R_16_FLOAT].maxDimensions.maxHeight >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_R_16_FLOAT].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_R_16_FLOAT].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_R_16_FLOAT].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_R_16_FLOAT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_R_16_FLOAT].maxBytesCount >= 2147483648); // Intel UHD Graphics 730 on Windows 10. Not used: RTX 2060 on Windows 10 is 1099511627776
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_R_16_FLOAT].status == RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_RGBA_16_16_16_16_UINT].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_RGBA_16_16_16_16_UINT].maxDimensions.maxHeight >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_RGBA_16_16_16_16_UINT].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_RGBA_16_16_16_16_UINT].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_RGBA_16_16_16_16_UINT].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_RGBA_16_16_16_16_UINT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_RGBA_16_16_16_16_UINT].maxBytesCount >= 2147483648); // Intel UHD Graphics 730 on Windows 10. Not used: RTX 2060 on Windows 10 is 1099511627776
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_RGBA_16_16_16_16_UINT].status == RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_RGBA_16_16_16_16_SINT].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_RGBA_16_16_16_16_SINT].maxDimensions.maxHeight >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_RGBA_16_16_16_16_SINT].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_RGBA_16_16_16_16_SINT].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_RGBA_16_16_16_16_SINT].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_RGBA_16_16_16_16_SINT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_RGBA_16_16_16_16_SINT].maxBytesCount >= 2147483648); // Intel UHD Graphics 730 on Windows 10. Not used: RTX 2060 on Windows 10 is 1099511627776
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_RGBA_16_16_16_16_SINT].status == RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_RGBA_16_16_16_16_FLOAT].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_RGBA_16_16_16_16_FLOAT].maxDimensions.maxHeight >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_RGBA_16_16_16_16_FLOAT].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_RGBA_16_16_16_16_FLOAT].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_RGBA_16_16_16_16_FLOAT].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_RGBA_16_16_16_16_FLOAT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_RGBA_16_16_16_16_FLOAT].maxBytesCount >= 2147483648); // Intel UHD Graphics 730 on Windows 10. Not used: RTX 2060 on Windows 10 is 1099511627776
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_RGBA_16_16_16_16_FLOAT].status == RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_R_32_UINT].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_R_32_UINT].maxDimensions.maxHeight >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_R_32_UINT].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_R_32_UINT].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_R_32_UINT].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_R_32_UINT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_R_32_UINT].maxBytesCount >= 2147483648); // Intel UHD Graphics 730 on Windows 10. Not used: RTX 2060 on Windows 10 is 1099511627776
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_R_32_UINT].status == RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_R_32_SINT].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_R_32_SINT].maxDimensions.maxHeight >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_R_32_SINT].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_R_32_SINT].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_R_32_SINT].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_R_32_SINT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_R_32_SINT].maxBytesCount >= 2147483648); // Intel UHD Graphics 730 on Windows 10. Not used: RTX 2060 on Windows 10 is 1099511627776
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_R_32_SINT].status == RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_R_32_FLOAT].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_R_32_FLOAT].maxDimensions.maxHeight >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_R_32_FLOAT].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_R_32_FLOAT].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_R_32_FLOAT].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_R_32_FLOAT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_R_32_FLOAT].maxBytesCount >= 2147483648); // Intel UHD Graphics 730 on Windows 10. Not used: RTX 2060 on Windows 10 is 1099511627776
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_R_32_FLOAT].status == RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_RGBA_32_32_32_32_UINT].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_RGBA_32_32_32_32_UINT].maxDimensions.maxHeight >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_RGBA_32_32_32_32_UINT].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_RGBA_32_32_32_32_UINT].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_RGBA_32_32_32_32_UINT].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_RGBA_32_32_32_32_UINT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_RGBA_32_32_32_32_UINT].maxBytesCount >= 2147483648); // Intel UHD Graphics 730 on Windows 10. Not used: RTX 2060 on Windows 10 is 1099511627776
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_RGBA_32_32_32_32_UINT].status == RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_RGBA_32_32_32_32_SINT].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_RGBA_32_32_32_32_SINT].maxDimensions.maxHeight >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_RGBA_32_32_32_32_SINT].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_RGBA_32_32_32_32_SINT].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_RGBA_32_32_32_32_SINT].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_RGBA_32_32_32_32_SINT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_RGBA_32_32_32_32_SINT].maxBytesCount >= 2147483648); // Intel UHD Graphics 730 on Windows 10. Not used: RTX 2060 on Windows 10 is 1099511627776
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_RGBA_32_32_32_32_SINT].status == RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_RGBA_32_32_32_32_FLOAT].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_RGBA_32_32_32_32_FLOAT].maxDimensions.maxHeight >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_RGBA_32_32_32_32_FLOAT].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_RGBA_32_32_32_32_FLOAT].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_RGBA_32_32_32_32_FLOAT].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_RGBA_32_32_32_32_FLOAT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_RGBA_32_32_32_32_FLOAT].maxBytesCount >= 2147483648); // Intel UHD Graphics 730 on Windows 10. Not used: RTX 2060 on Windows 10 is 1099511627776
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_RGBA_32_32_32_32_FLOAT].status == RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_DEPTH_16_UINT_TO_FLOAT_0_1].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_DEPTH_16_UINT_TO_FLOAT_0_1].maxDimensions.maxHeight >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_DEPTH_16_UINT_TO_FLOAT_0_1].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_DEPTH_16_UINT_TO_FLOAT_0_1].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_DEPTH_16_UINT_TO_FLOAT_0_1].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_DEPTH_16_UINT_TO_FLOAT_0_1].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_4 | RED_MULTISAMPLE_COUNT_BITFLAG_2 | RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_DEPTH_16_UINT_TO_FLOAT_0_1].maxBytesCount >= 2147483648); // Intel UHD Graphics 730 on Windows 10. Not used: RTX 2060 on Windows 10 is 1099511627776
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_DEPTH_16_UINT_TO_FLOAT_0_1].status == RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_DEPTH_32_FLOAT].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_DEPTH_32_FLOAT].maxDimensions.maxHeight >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_DEPTH_32_FLOAT].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_DEPTH_32_FLOAT].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_DEPTH_32_FLOAT].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_DEPTH_32_FLOAT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_4 | RED_MULTISAMPLE_COUNT_BITFLAG_2 | RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_DEPTH_32_FLOAT].maxBytesCount >= 2147483648); // Intel UHD Graphics 730 on Windows 10. Not used: RTX 2060 on Windows 10 is 1099511627776
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_DEPTH_32_FLOAT].status == RED_STATUS_SUCCESS);
  if (gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_DEPTH_24_UINT_TO_FLOAT_0_1_STENCIL_8_UINT].status != RED_STATUS_ERROR_FORMAT_IS_NOT_SUPPORTED) { // AMD RX 550 on Windows 10 doesn't support this format
    REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_DEPTH_24_UINT_TO_FLOAT_0_1_STENCIL_8_UINT].maxDimensions.maxWidth >= 16384);
    REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_DEPTH_24_UINT_TO_FLOAT_0_1_STENCIL_8_UINT].maxDimensions.maxHeight >= 16384);
    REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_DEPTH_24_UINT_TO_FLOAT_0_1_STENCIL_8_UINT].maxDimensions.maxDepth >= 1);
    REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_DEPTH_24_UINT_TO_FLOAT_0_1_STENCIL_8_UINT].maxLevelsCount >= 15);
    REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_DEPTH_24_UINT_TO_FLOAT_0_1_STENCIL_8_UINT].maxLayersCount >= 2048);
    REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_DEPTH_24_UINT_TO_FLOAT_0_1_STENCIL_8_UINT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_4 | RED_MULTISAMPLE_COUNT_BITFLAG_2 | RED_MULTISAMPLE_COUNT_BITFLAG_1));
    REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_DEPTH_24_UINT_TO_FLOAT_0_1_STENCIL_8_UINT].maxBytesCount >= 2147483648); // Intel UHD Graphics 730 on Windows 10. Not used: RTX 2060 on Windows 10 is 1099511627776
    REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_DEPTH_24_UINT_TO_FLOAT_0_1_STENCIL_8_UINT].status == RED_STATUS_SUCCESS);
  }
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_DEPTH_32_FLOAT_STENCIL_8_UINT].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_DEPTH_32_FLOAT_STENCIL_8_UINT].maxDimensions.maxHeight >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_DEPTH_32_FLOAT_STENCIL_8_UINT].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_DEPTH_32_FLOAT_STENCIL_8_UINT].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_DEPTH_32_FLOAT_STENCIL_8_UINT].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_DEPTH_32_FLOAT_STENCIL_8_UINT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_4 | RED_MULTISAMPLE_COUNT_BITFLAG_2 | RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_DEPTH_32_FLOAT_STENCIL_8_UINT].maxBytesCount >= 2147483648); // Intel UHD Graphics 730 on Windows 10. Not used: RTX 2060 on Windows 10 is 1099511627776
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_DEPTH_32_FLOAT_STENCIL_8_UINT].status == RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_R_8_UINT_TO_FLOAT_0_1].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_R_8_UINT_TO_FLOAT_0_1].maxDimensions.maxHeight >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_R_8_UINT_TO_FLOAT_0_1].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_R_8_UINT_TO_FLOAT_0_1].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_R_8_UINT_TO_FLOAT_0_1].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_R_8_UINT_TO_FLOAT_0_1].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_4 | RED_MULTISAMPLE_COUNT_BITFLAG_2 | RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_R_8_UINT_TO_FLOAT_0_1].maxBytesCount >= 2147483648); // Intel UHD Graphics 730 on Windows 10. Not used: RTX 2060 on Windows 10 is 1099511627776
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_R_8_UINT_TO_FLOAT_0_1].status == RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_R_8_UINT].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_R_8_UINT].maxDimensions.maxHeight >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_R_8_UINT].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_R_8_UINT].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_R_8_UINT].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_R_8_UINT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_4 | RED_MULTISAMPLE_COUNT_BITFLAG_2 | RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_R_8_UINT].maxBytesCount >= 2147483648); // Intel UHD Graphics 730 on Windows 10. Not used: RTX 2060 on Windows 10 is 1099511627776
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_R_8_UINT].status == RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_R_8_SINT].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_R_8_SINT].maxDimensions.maxHeight >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_R_8_SINT].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_R_8_SINT].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_R_8_SINT].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_R_8_SINT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_4 | RED_MULTISAMPLE_COUNT_BITFLAG_2 | RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_R_8_SINT].maxBytesCount >= 2147483648); // Intel UHD Graphics 730 on Windows 10. Not used: RTX 2060 on Windows 10 is 1099511627776
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_R_8_SINT].status == RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1].maxDimensions.maxHeight >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_4 | RED_MULTISAMPLE_COUNT_BITFLAG_2 | RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1].maxBytesCount >= 2147483648); // Intel UHD Graphics 730 on Windows 10. Not used: RTX 2060 on Windows 10 is 1099511627776
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1].status == RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_RGBA_8_8_8_8_UINT].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_RGBA_8_8_8_8_UINT].maxDimensions.maxHeight >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_RGBA_8_8_8_8_UINT].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_RGBA_8_8_8_8_UINT].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_RGBA_8_8_8_8_UINT].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_RGBA_8_8_8_8_UINT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_4 | RED_MULTISAMPLE_COUNT_BITFLAG_2 | RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_RGBA_8_8_8_8_UINT].maxBytesCount >= 2147483648); // Intel UHD Graphics 730 on Windows 10. Not used: RTX 2060 on Windows 10 is 1099511627776
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_RGBA_8_8_8_8_UINT].status == RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_RGBA_8_8_8_8_SINT].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_RGBA_8_8_8_8_SINT].maxDimensions.maxHeight >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_RGBA_8_8_8_8_SINT].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_RGBA_8_8_8_8_SINT].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_RGBA_8_8_8_8_SINT].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_RGBA_8_8_8_8_SINT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_4 | RED_MULTISAMPLE_COUNT_BITFLAG_2 | RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_RGBA_8_8_8_8_SINT].maxBytesCount >= 2147483648); // Intel UHD Graphics 730 on Windows 10. Not used: RTX 2060 on Windows 10 is 1099511627776
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_RGBA_8_8_8_8_SINT].status == RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1_GAMMA_CORRECTED].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1_GAMMA_CORRECTED].maxDimensions.maxHeight >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1_GAMMA_CORRECTED].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1_GAMMA_CORRECTED].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1_GAMMA_CORRECTED].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1_GAMMA_CORRECTED].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_4 | RED_MULTISAMPLE_COUNT_BITFLAG_2 | RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1_GAMMA_CORRECTED].maxBytesCount >= 2147483648); // Intel UHD Graphics 730 on Windows 10. Not used: RTX 2060 on Windows 10 is 1099511627776
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1_GAMMA_CORRECTED].status == RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_PRESENT_BGRA_8_8_8_8_UINT_TO_FLOAT_0_1].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_PRESENT_BGRA_8_8_8_8_UINT_TO_FLOAT_0_1].maxDimensions.maxHeight >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_PRESENT_BGRA_8_8_8_8_UINT_TO_FLOAT_0_1].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_PRESENT_BGRA_8_8_8_8_UINT_TO_FLOAT_0_1].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_PRESENT_BGRA_8_8_8_8_UINT_TO_FLOAT_0_1].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_PRESENT_BGRA_8_8_8_8_UINT_TO_FLOAT_0_1].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_4 | RED_MULTISAMPLE_COUNT_BITFLAG_2 | RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_PRESENT_BGRA_8_8_8_8_UINT_TO_FLOAT_0_1].maxBytesCount >= 2147483648); // Intel UHD Graphics 730 on Windows 10. Not used: RTX 2060 on Windows 10 is 1099511627776
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_PRESENT_BGRA_8_8_8_8_UINT_TO_FLOAT_0_1].status == RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_R_16_UINT].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_R_16_UINT].maxDimensions.maxHeight >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_R_16_UINT].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_R_16_UINT].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_R_16_UINT].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_R_16_UINT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_4 | RED_MULTISAMPLE_COUNT_BITFLAG_2 | RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_R_16_UINT].maxBytesCount >= 2147483648); // Intel UHD Graphics 730 on Windows 10. Not used: RTX 2060 on Windows 10 is 1099511627776
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_R_16_UINT].status == RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_R_16_SINT].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_R_16_SINT].maxDimensions.maxHeight >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_R_16_SINT].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_R_16_SINT].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_R_16_SINT].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_R_16_SINT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_4 | RED_MULTISAMPLE_COUNT_BITFLAG_2 | RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_R_16_SINT].maxBytesCount >= 2147483648); // Intel UHD Graphics 730 on Windows 10. Not used: RTX 2060 on Windows 10 is 1099511627776
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_R_16_SINT].status == RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_R_16_FLOAT].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_R_16_FLOAT].maxDimensions.maxHeight >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_R_16_FLOAT].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_R_16_FLOAT].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_R_16_FLOAT].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_R_16_FLOAT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_4 | RED_MULTISAMPLE_COUNT_BITFLAG_2 | RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_R_16_FLOAT].maxBytesCount >= 2147483648); // Intel UHD Graphics 730 on Windows 10. Not used: RTX 2060 on Windows 10 is 1099511627776
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_R_16_FLOAT].status == RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_RGBA_16_16_16_16_UINT].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_RGBA_16_16_16_16_UINT].maxDimensions.maxHeight >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_RGBA_16_16_16_16_UINT].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_RGBA_16_16_16_16_UINT].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_RGBA_16_16_16_16_UINT].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_RGBA_16_16_16_16_UINT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_4 | RED_MULTISAMPLE_COUNT_BITFLAG_2 | RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_RGBA_16_16_16_16_UINT].maxBytesCount >= 2147483648); // Intel UHD Graphics 730 on Windows 10. Not used: RTX 2060 on Windows 10 is 1099511627776
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_RGBA_16_16_16_16_UINT].status == RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_RGBA_16_16_16_16_SINT].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_RGBA_16_16_16_16_SINT].maxDimensions.maxHeight >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_RGBA_16_16_16_16_SINT].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_RGBA_16_16_16_16_SINT].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_RGBA_16_16_16_16_SINT].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_RGBA_16_16_16_16_SINT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_4 | RED_MULTISAMPLE_COUNT_BITFLAG_2 | RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_RGBA_16_16_16_16_SINT].maxBytesCount >= 2147483648); // Intel UHD Graphics 730 on Windows 10. Not used: RTX 2060 on Windows 10 is 1099511627776
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_RGBA_16_16_16_16_SINT].status == RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_RGBA_16_16_16_16_FLOAT].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_RGBA_16_16_16_16_FLOAT].maxDimensions.maxHeight >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_RGBA_16_16_16_16_FLOAT].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_RGBA_16_16_16_16_FLOAT].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_RGBA_16_16_16_16_FLOAT].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_RGBA_16_16_16_16_FLOAT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_4 | RED_MULTISAMPLE_COUNT_BITFLAG_2 | RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_RGBA_16_16_16_16_FLOAT].maxBytesCount >= 2147483648); // Intel UHD Graphics 730 on Windows 10. Not used: RTX 2060 on Windows 10 is 1099511627776
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_RGBA_16_16_16_16_FLOAT].status == RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_R_32_UINT].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_R_32_UINT].maxDimensions.maxHeight >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_R_32_UINT].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_R_32_UINT].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_R_32_UINT].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_R_32_UINT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_4 | RED_MULTISAMPLE_COUNT_BITFLAG_2 | RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_R_32_UINT].maxBytesCount >= 2147483648); // Intel UHD Graphics 730 on Windows 10. Not used: RTX 2060 on Windows 10 is 1099511627776
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_R_32_UINT].status == RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_R_32_SINT].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_R_32_SINT].maxDimensions.maxHeight >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_R_32_SINT].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_R_32_SINT].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_R_32_SINT].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_R_32_SINT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_4 | RED_MULTISAMPLE_COUNT_BITFLAG_2 | RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_R_32_SINT].maxBytesCount >= 2147483648); // Intel UHD Graphics 730 on Windows 10. Not used: RTX 2060 on Windows 10 is 1099511627776
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_R_32_SINT].status == RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_R_32_FLOAT].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_R_32_FLOAT].maxDimensions.maxHeight >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_R_32_FLOAT].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_R_32_FLOAT].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_R_32_FLOAT].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_R_32_FLOAT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_4 | RED_MULTISAMPLE_COUNT_BITFLAG_2 | RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_R_32_FLOAT].maxBytesCount >= 2147483648); // Intel UHD Graphics 730 on Windows 10. Not used: RTX 2060 on Windows 10 is 1099511627776
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_R_32_FLOAT].status == RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_RGBA_32_32_32_32_UINT].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_RGBA_32_32_32_32_UINT].maxDimensions.maxHeight >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_RGBA_32_32_32_32_UINT].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_RGBA_32_32_32_32_UINT].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_RGBA_32_32_32_32_UINT].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_RGBA_32_32_32_32_UINT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_4 | RED_MULTISAMPLE_COUNT_BITFLAG_2 | RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_RGBA_32_32_32_32_UINT].maxBytesCount >= 2147483648); // Intel UHD Graphics 730 on Windows 10. Not used: RTX 2060 on Windows 10 is 1099511627776
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_RGBA_32_32_32_32_UINT].status == RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_RGBA_32_32_32_32_SINT].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_RGBA_32_32_32_32_SINT].maxDimensions.maxHeight >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_RGBA_32_32_32_32_SINT].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_RGBA_32_32_32_32_SINT].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_RGBA_32_32_32_32_SINT].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_RGBA_32_32_32_32_SINT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_4 | RED_MULTISAMPLE_COUNT_BITFLAG_2 | RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_RGBA_32_32_32_32_SINT].maxBytesCount >= 2147483648); // Intel UHD Graphics 730 on Windows 10. Not used: RTX 2060 on Windows 10 is 1099511627776
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_RGBA_32_32_32_32_SINT].status == RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_RGBA_32_32_32_32_FLOAT].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_RGBA_32_32_32_32_FLOAT].maxDimensions.maxHeight >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_RGBA_32_32_32_32_FLOAT].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_RGBA_32_32_32_32_FLOAT].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_RGBA_32_32_32_32_FLOAT].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_RGBA_32_32_32_32_FLOAT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_4 | RED_MULTISAMPLE_COUNT_BITFLAG_2 | RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_RGBA_32_32_32_32_FLOAT].maxBytesCount >= 2147483648); // Intel UHD Graphics 730 on Windows 10. Not used: RTX 2060 on Windows 10 is 1099511627776
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_RGBA_32_32_32_32_FLOAT].status == RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_DEPTH_16_UINT_TO_FLOAT_0_1].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_DEPTH_16_UINT_TO_FLOAT_0_1].maxDimensions.maxHeight >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_DEPTH_16_UINT_TO_FLOAT_0_1].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_DEPTH_16_UINT_TO_FLOAT_0_1].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_DEPTH_16_UINT_TO_FLOAT_0_1].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_DEPTH_16_UINT_TO_FLOAT_0_1].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_4 | RED_MULTISAMPLE_COUNT_BITFLAG_2 | RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_DEPTH_16_UINT_TO_FLOAT_0_1].maxBytesCount >= 2147483648); // Intel UHD Graphics 730 on Windows 10. Not used: RTX 2060 on Windows 10 is 1099511627776
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_DEPTH_16_UINT_TO_FLOAT_0_1].status == RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_DEPTH_32_FLOAT].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_DEPTH_32_FLOAT].maxDimensions.maxHeight >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_DEPTH_32_FLOAT].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_DEPTH_32_FLOAT].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_DEPTH_32_FLOAT].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_DEPTH_32_FLOAT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_4 | RED_MULTISAMPLE_COUNT_BITFLAG_2 | RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_DEPTH_32_FLOAT].maxBytesCount >= 2147483648); // Intel UHD Graphics 730 on Windows 10. Not used: RTX 2060 on Windows 10 is 1099511627776
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_DEPTH_32_FLOAT].status == RED_STATUS_SUCCESS);
  if (gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_DEPTH_24_UINT_TO_FLOAT_0_1_STENCIL_8_UINT].status != RED_STATUS_ERROR_FORMAT_IS_NOT_SUPPORTED) { // AMD RX 550 on Windows 10 doesn't support this format
    REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_DEPTH_24_UINT_TO_FLOAT_0_1_STENCIL_8_UINT].maxDimensions.maxWidth >= 16384);
    REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_DEPTH_24_UINT_TO_FLOAT_0_1_STENCIL_8_UINT].maxDimensions.maxHeight >= 16384);
    REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_DEPTH_24_UINT_TO_FLOAT_0_1_STENCIL_8_UINT].maxDimensions.maxDepth >= 1);
    REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_DEPTH_24_UINT_TO_FLOAT_0_1_STENCIL_8_UINT].maxLevelsCount >= 15);
    REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_DEPTH_24_UINT_TO_FLOAT_0_1_STENCIL_8_UINT].maxLayersCount >= 2048);
    REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_DEPTH_24_UINT_TO_FLOAT_0_1_STENCIL_8_UINT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_4 | RED_MULTISAMPLE_COUNT_BITFLAG_2 | RED_MULTISAMPLE_COUNT_BITFLAG_1));
    REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_DEPTH_24_UINT_TO_FLOAT_0_1_STENCIL_8_UINT].maxBytesCount >= 2147483648); // Intel UHD Graphics 730 on Windows 10. Not used: RTX 2060 on Windows 10 is 1099511627776
    REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_DEPTH_24_UINT_TO_FLOAT_0_1_STENCIL_8_UINT].status == RED_STATUS_SUCCESS);
  }
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_DEPTH_32_FLOAT_STENCIL_8_UINT].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_DEPTH_32_FLOAT_STENCIL_8_UINT].maxDimensions.maxHeight >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_DEPTH_32_FLOAT_STENCIL_8_UINT].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_DEPTH_32_FLOAT_STENCIL_8_UINT].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_DEPTH_32_FLOAT_STENCIL_8_UINT].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_DEPTH_32_FLOAT_STENCIL_8_UINT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_4 | RED_MULTISAMPLE_COUNT_BITFLAG_2 | RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_DEPTH_32_FLOAT_STENCIL_8_UINT].maxBytesCount >= 2147483648); // Intel UHD Graphics 730 on Windows 10. Not used: RTX 2060 on Windows 10 is 1099511627776
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DMultisample[RED_FORMAT_DEPTH_32_FLOAT_STENCIL_8_UINT].status == RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_R_8_UINT_TO_FLOAT_0_1].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_R_8_UINT_TO_FLOAT_0_1].maxDimensions.maxHeight >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_R_8_UINT_TO_FLOAT_0_1].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_R_8_UINT_TO_FLOAT_0_1].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_R_8_UINT_TO_FLOAT_0_1].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_R_8_UINT_TO_FLOAT_0_1].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_R_8_UINT_TO_FLOAT_0_1].maxBytesCount >= 2147483648); // Intel UHD Graphics 730 on Windows 10. Not used: RTX 2060 on Windows 10 is 1099511627776
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_R_8_UINT_TO_FLOAT_0_1].status == RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_R_8_UINT].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_R_8_UINT].maxDimensions.maxHeight >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_R_8_UINT].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_R_8_UINT].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_R_8_UINT].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_R_8_UINT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_R_8_UINT].maxBytesCount >= 2147483648); // Intel UHD Graphics 730 on Windows 10. Not used: RTX 2060 on Windows 10 is 1099511627776
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_R_8_UINT].status == RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_R_8_SINT].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_R_8_SINT].maxDimensions.maxHeight >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_R_8_SINT].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_R_8_SINT].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_R_8_SINT].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_R_8_SINT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_R_8_SINT].maxBytesCount >= 2147483648); // Intel UHD Graphics 730 on Windows 10. Not used: RTX 2060 on Windows 10 is 1099511627776
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_R_8_SINT].status == RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1].maxDimensions.maxHeight >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1].maxBytesCount >= 2147483648); // Intel UHD Graphics 730 on Windows 10. Not used: RTX 2060 on Windows 10 is 1099511627776
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1].status == RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_RGBA_8_8_8_8_UINT].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_RGBA_8_8_8_8_UINT].maxDimensions.maxHeight >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_RGBA_8_8_8_8_UINT].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_RGBA_8_8_8_8_UINT].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_RGBA_8_8_8_8_UINT].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_RGBA_8_8_8_8_UINT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_RGBA_8_8_8_8_UINT].maxBytesCount >= 2147483648); // Intel UHD Graphics 730 on Windows 10. Not used: RTX 2060 on Windows 10 is 1099511627776
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_RGBA_8_8_8_8_UINT].status == RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_RGBA_8_8_8_8_SINT].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_RGBA_8_8_8_8_SINT].maxDimensions.maxHeight >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_RGBA_8_8_8_8_SINT].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_RGBA_8_8_8_8_SINT].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_RGBA_8_8_8_8_SINT].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_RGBA_8_8_8_8_SINT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_RGBA_8_8_8_8_SINT].maxBytesCount >= 2147483648); // Intel UHD Graphics 730 on Windows 10. Not used: RTX 2060 on Windows 10 is 1099511627776
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_RGBA_8_8_8_8_SINT].status == RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1_GAMMA_CORRECTED].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1_GAMMA_CORRECTED].maxDimensions.maxHeight >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1_GAMMA_CORRECTED].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1_GAMMA_CORRECTED].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1_GAMMA_CORRECTED].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1_GAMMA_CORRECTED].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1_GAMMA_CORRECTED].maxBytesCount >= 2147483648); // Intel UHD Graphics 730 on Windows 10. Not used: RTX 2060 on Windows 10 is 1099511627776
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1_GAMMA_CORRECTED].status == RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_PRESENT_BGRA_8_8_8_8_UINT_TO_FLOAT_0_1].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_PRESENT_BGRA_8_8_8_8_UINT_TO_FLOAT_0_1].maxDimensions.maxHeight >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_PRESENT_BGRA_8_8_8_8_UINT_TO_FLOAT_0_1].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_PRESENT_BGRA_8_8_8_8_UINT_TO_FLOAT_0_1].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_PRESENT_BGRA_8_8_8_8_UINT_TO_FLOAT_0_1].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_PRESENT_BGRA_8_8_8_8_UINT_TO_FLOAT_0_1].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_PRESENT_BGRA_8_8_8_8_UINT_TO_FLOAT_0_1].maxBytesCount >= 2147483648); // Intel UHD Graphics 730 on Windows 10. Not used: RTX 2060 on Windows 10 is 1099511627776
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_PRESENT_BGRA_8_8_8_8_UINT_TO_FLOAT_0_1].status == RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_R_16_UINT].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_R_16_UINT].maxDimensions.maxHeight >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_R_16_UINT].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_R_16_UINT].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_R_16_UINT].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_R_16_UINT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_R_16_UINT].maxBytesCount >= 2147483648); // Intel UHD Graphics 730 on Windows 10. Not used: RTX 2060 on Windows 10 is 1099511627776
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_R_16_UINT].status == RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_R_16_SINT].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_R_16_SINT].maxDimensions.maxHeight >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_R_16_SINT].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_R_16_SINT].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_R_16_SINT].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_R_16_SINT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_R_16_SINT].maxBytesCount >= 2147483648); // Intel UHD Graphics 730 on Windows 10. Not used: RTX 2060 on Windows 10 is 1099511627776
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_R_16_SINT].status == RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_R_16_FLOAT].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_R_16_FLOAT].maxDimensions.maxHeight >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_R_16_FLOAT].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_R_16_FLOAT].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_R_16_FLOAT].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_R_16_FLOAT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_R_16_FLOAT].maxBytesCount >= 2147483648); // Intel UHD Graphics 730 on Windows 10. Not used: RTX 2060 on Windows 10 is 1099511627776
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_R_16_FLOAT].status == RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_RGBA_16_16_16_16_UINT].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_RGBA_16_16_16_16_UINT].maxDimensions.maxHeight >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_RGBA_16_16_16_16_UINT].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_RGBA_16_16_16_16_UINT].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_RGBA_16_16_16_16_UINT].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_RGBA_16_16_16_16_UINT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_RGBA_16_16_16_16_UINT].maxBytesCount >= 2147483648); // Intel UHD Graphics 730 on Windows 10. Not used: RTX 2060 on Windows 10 is 1099511627776
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_RGBA_16_16_16_16_UINT].status == RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_RGBA_16_16_16_16_SINT].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_RGBA_16_16_16_16_SINT].maxDimensions.maxHeight >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_RGBA_16_16_16_16_SINT].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_RGBA_16_16_16_16_SINT].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_RGBA_16_16_16_16_SINT].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_RGBA_16_16_16_16_SINT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_RGBA_16_16_16_16_SINT].maxBytesCount >= 2147483648); // Intel UHD Graphics 730 on Windows 10. Not used: RTX 2060 on Windows 10 is 1099511627776
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_RGBA_16_16_16_16_SINT].status == RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_RGBA_16_16_16_16_FLOAT].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_RGBA_16_16_16_16_FLOAT].maxDimensions.maxHeight >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_RGBA_16_16_16_16_FLOAT].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_RGBA_16_16_16_16_FLOAT].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_RGBA_16_16_16_16_FLOAT].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_RGBA_16_16_16_16_FLOAT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_RGBA_16_16_16_16_FLOAT].maxBytesCount >= 2147483648); // Intel UHD Graphics 730 on Windows 10. Not used: RTX 2060 on Windows 10 is 1099511627776
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_RGBA_16_16_16_16_FLOAT].status == RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_R_32_UINT].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_R_32_UINT].maxDimensions.maxHeight >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_R_32_UINT].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_R_32_UINT].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_R_32_UINT].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_R_32_UINT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_R_32_UINT].maxBytesCount >= 2147483648); // Intel UHD Graphics 730 on Windows 10. Not used: RTX 2060 on Windows 10 is 1099511627776
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_R_32_UINT].status == RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_R_32_SINT].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_R_32_SINT].maxDimensions.maxHeight >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_R_32_SINT].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_R_32_SINT].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_R_32_SINT].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_R_32_SINT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_R_32_SINT].maxBytesCount >= 2147483648); // Intel UHD Graphics 730 on Windows 10. Not used: RTX 2060 on Windows 10 is 1099511627776
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_R_32_SINT].status == RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_R_32_FLOAT].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_R_32_FLOAT].maxDimensions.maxHeight >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_R_32_FLOAT].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_R_32_FLOAT].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_R_32_FLOAT].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_R_32_FLOAT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_R_32_FLOAT].maxBytesCount >= 2147483648); // Intel UHD Graphics 730 on Windows 10. Not used: RTX 2060 on Windows 10 is 1099511627776
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_R_32_FLOAT].status == RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_RGBA_32_32_32_32_UINT].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_RGBA_32_32_32_32_UINT].maxDimensions.maxHeight >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_RGBA_32_32_32_32_UINT].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_RGBA_32_32_32_32_UINT].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_RGBA_32_32_32_32_UINT].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_RGBA_32_32_32_32_UINT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_RGBA_32_32_32_32_UINT].maxBytesCount >= 2147483648); // Intel UHD Graphics 730 on Windows 10. Not used: RTX 2060 on Windows 10 is 1099511627776
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_RGBA_32_32_32_32_UINT].status == RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_RGBA_32_32_32_32_SINT].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_RGBA_32_32_32_32_SINT].maxDimensions.maxHeight >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_RGBA_32_32_32_32_SINT].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_RGBA_32_32_32_32_SINT].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_RGBA_32_32_32_32_SINT].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_RGBA_32_32_32_32_SINT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_RGBA_32_32_32_32_SINT].maxBytesCount >= 2147483648); // Intel UHD Graphics 730 on Windows 10. Not used: RTX 2060 on Windows 10 is 1099511627776
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_RGBA_32_32_32_32_SINT].status == RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_RGBA_32_32_32_32_FLOAT].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_RGBA_32_32_32_32_FLOAT].maxDimensions.maxHeight >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_RGBA_32_32_32_32_FLOAT].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_RGBA_32_32_32_32_FLOAT].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_RGBA_32_32_32_32_FLOAT].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_RGBA_32_32_32_32_FLOAT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_RGBA_32_32_32_32_FLOAT].maxBytesCount >= 2147483648); // Intel UHD Graphics 730 on Windows 10. Not used: RTX 2060 on Windows 10 is 1099511627776
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_RGBA_32_32_32_32_FLOAT].status == RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_DEPTH_16_UINT_TO_FLOAT_0_1].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_DEPTH_16_UINT_TO_FLOAT_0_1].maxDimensions.maxHeight >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_DEPTH_16_UINT_TO_FLOAT_0_1].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_DEPTH_16_UINT_TO_FLOAT_0_1].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_DEPTH_16_UINT_TO_FLOAT_0_1].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_DEPTH_16_UINT_TO_FLOAT_0_1].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_DEPTH_16_UINT_TO_FLOAT_0_1].maxBytesCount >= 2147483648); // Intel UHD Graphics 730 on Windows 10. Not used: RTX 2060 on Windows 10 is 1099511627776
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_DEPTH_16_UINT_TO_FLOAT_0_1].status == RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_DEPTH_32_FLOAT].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_DEPTH_32_FLOAT].maxDimensions.maxHeight >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_DEPTH_32_FLOAT].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_DEPTH_32_FLOAT].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_DEPTH_32_FLOAT].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_DEPTH_32_FLOAT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_DEPTH_32_FLOAT].maxBytesCount >= 2147483648); // Intel UHD Graphics 730 on Windows 10. Not used: RTX 2060 on Windows 10 is 1099511627776
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_DEPTH_32_FLOAT].status == RED_STATUS_SUCCESS);
  if (gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_DEPTH_24_UINT_TO_FLOAT_0_1_STENCIL_8_UINT].status != RED_STATUS_ERROR_FORMAT_IS_NOT_SUPPORTED) { // AMD RX 550 on Windows 10 doesn't support this format
    REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_DEPTH_24_UINT_TO_FLOAT_0_1_STENCIL_8_UINT].maxDimensions.maxWidth >= 16384);
    REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_DEPTH_24_UINT_TO_FLOAT_0_1_STENCIL_8_UINT].maxDimensions.maxHeight >= 16384);
    REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_DEPTH_24_UINT_TO_FLOAT_0_1_STENCIL_8_UINT].maxDimensions.maxDepth >= 1);
    REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_DEPTH_24_UINT_TO_FLOAT_0_1_STENCIL_8_UINT].maxLevelsCount >= 15);
    REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_DEPTH_24_UINT_TO_FLOAT_0_1_STENCIL_8_UINT].maxLayersCount >= 2048);
    REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_DEPTH_24_UINT_TO_FLOAT_0_1_STENCIL_8_UINT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
    REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_DEPTH_24_UINT_TO_FLOAT_0_1_STENCIL_8_UINT].maxBytesCount >= 2147483648); // Intel UHD Graphics 730 on Windows 10. Not used: RTX 2060 on Windows 10 is 1099511627776
    REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_DEPTH_24_UINT_TO_FLOAT_0_1_STENCIL_8_UINT].status == RED_STATUS_SUCCESS);
  }
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_DEPTH_32_FLOAT_STENCIL_8_UINT].maxDimensions.maxWidth >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_DEPTH_32_FLOAT_STENCIL_8_UINT].maxDimensions.maxHeight >= 16384);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_DEPTH_32_FLOAT_STENCIL_8_UINT].maxDimensions.maxDepth >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_DEPTH_32_FLOAT_STENCIL_8_UINT].maxLevelsCount >= 15);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_DEPTH_32_FLOAT_STENCIL_8_UINT].maxLayersCount >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_DEPTH_32_FLOAT_STENCIL_8_UINT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_DEPTH_32_FLOAT_STENCIL_8_UINT].maxBytesCount >= 2147483648); // Intel UHD Graphics 730 on Windows 10. Not used: RTX 2060 on Windows 10 is 1099511627776
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[RED_FORMAT_DEPTH_32_FLOAT_STENCIL_8_UINT].status == RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_R_8_UINT_TO_FLOAT_0_1].maxDimensions.maxWidth >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_R_8_UINT_TO_FLOAT_0_1].maxDimensions.maxHeight >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_R_8_UINT_TO_FLOAT_0_1].maxDimensions.maxDepth >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_R_8_UINT_TO_FLOAT_0_1].maxLevelsCount >= 12);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_R_8_UINT_TO_FLOAT_0_1].maxLayersCount >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_R_8_UINT_TO_FLOAT_0_1].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_R_8_UINT_TO_FLOAT_0_1].maxBytesCount >= 2147483648); // Intel UHD Graphics 730 on Windows 10. Not used: RTX 2060 on Windows 10 is 1099511627776
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_R_8_UINT_TO_FLOAT_0_1].status == RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_R_8_UINT].maxDimensions.maxWidth >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_R_8_UINT].maxDimensions.maxHeight >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_R_8_UINT].maxDimensions.maxDepth >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_R_8_UINT].maxLevelsCount >= 12);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_R_8_UINT].maxLayersCount >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_R_8_UINT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_R_8_UINT].maxBytesCount >= 2147483648); // Intel UHD Graphics 730 on Windows 10. Not used: RTX 2060 on Windows 10 is 1099511627776
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_R_8_UINT].status == RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_R_8_SINT].maxDimensions.maxWidth >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_R_8_SINT].maxDimensions.maxHeight >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_R_8_SINT].maxDimensions.maxDepth >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_R_8_SINT].maxLevelsCount >= 12);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_R_8_SINT].maxLayersCount >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_R_8_SINT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_R_8_SINT].maxBytesCount >= 2147483648); // Intel UHD Graphics 730 on Windows 10. Not used: RTX 2060 on Windows 10 is 1099511627776
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_R_8_SINT].status == RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1].maxDimensions.maxWidth >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1].maxDimensions.maxHeight >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1].maxDimensions.maxDepth >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1].maxLevelsCount >= 12);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1].maxLayersCount >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1].maxBytesCount >= 2147483648); // Intel UHD Graphics 730 on Windows 10. Not used: RTX 2060 on Windows 10 is 1099511627776
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1].status == RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_RGBA_8_8_8_8_UINT].maxDimensions.maxWidth >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_RGBA_8_8_8_8_UINT].maxDimensions.maxHeight >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_RGBA_8_8_8_8_UINT].maxDimensions.maxDepth >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_RGBA_8_8_8_8_UINT].maxLevelsCount >= 12);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_RGBA_8_8_8_8_UINT].maxLayersCount >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_RGBA_8_8_8_8_UINT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_RGBA_8_8_8_8_UINT].maxBytesCount >= 2147483648); // Intel UHD Graphics 730 on Windows 10. Not used: RTX 2060 on Windows 10 is 1099511627776
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_RGBA_8_8_8_8_UINT].status == RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_RGBA_8_8_8_8_SINT].maxDimensions.maxWidth >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_RGBA_8_8_8_8_SINT].maxDimensions.maxHeight >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_RGBA_8_8_8_8_SINT].maxDimensions.maxDepth >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_RGBA_8_8_8_8_SINT].maxLevelsCount >= 12);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_RGBA_8_8_8_8_SINT].maxLayersCount >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_RGBA_8_8_8_8_SINT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_RGBA_8_8_8_8_SINT].maxBytesCount >= 2147483648); // Intel UHD Graphics 730 on Windows 10. Not used: RTX 2060 on Windows 10 is 1099511627776
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_RGBA_8_8_8_8_SINT].status == RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1_GAMMA_CORRECTED].maxDimensions.maxWidth >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1_GAMMA_CORRECTED].maxDimensions.maxHeight >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1_GAMMA_CORRECTED].maxDimensions.maxDepth >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1_GAMMA_CORRECTED].maxLevelsCount >= 12);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1_GAMMA_CORRECTED].maxLayersCount >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1_GAMMA_CORRECTED].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1_GAMMA_CORRECTED].maxBytesCount >= 2147483648); // Intel UHD Graphics 730 on Windows 10. Not used: RTX 2060 on Windows 10 is 1099511627776
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1_GAMMA_CORRECTED].status == RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_PRESENT_BGRA_8_8_8_8_UINT_TO_FLOAT_0_1].maxDimensions.maxWidth >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_PRESENT_BGRA_8_8_8_8_UINT_TO_FLOAT_0_1].maxDimensions.maxHeight >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_PRESENT_BGRA_8_8_8_8_UINT_TO_FLOAT_0_1].maxDimensions.maxDepth >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_PRESENT_BGRA_8_8_8_8_UINT_TO_FLOAT_0_1].maxLevelsCount >= 12);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_PRESENT_BGRA_8_8_8_8_UINT_TO_FLOAT_0_1].maxLayersCount >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_PRESENT_BGRA_8_8_8_8_UINT_TO_FLOAT_0_1].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_PRESENT_BGRA_8_8_8_8_UINT_TO_FLOAT_0_1].maxBytesCount >= 2147483648); // Intel UHD Graphics 730 on Windows 10. Not used: RTX 2060 on Windows 10 is 1099511627776
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_PRESENT_BGRA_8_8_8_8_UINT_TO_FLOAT_0_1].status == RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_R_16_UINT].maxDimensions.maxWidth >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_R_16_UINT].maxDimensions.maxHeight >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_R_16_UINT].maxDimensions.maxDepth >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_R_16_UINT].maxLevelsCount >= 12);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_R_16_UINT].maxLayersCount >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_R_16_UINT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_R_16_UINT].maxBytesCount >= 2147483648); // Intel UHD Graphics 730 on Windows 10. Not used: RTX 2060 on Windows 10 is 1099511627776
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_R_16_UINT].status == RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_R_16_SINT].maxDimensions.maxWidth >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_R_16_SINT].maxDimensions.maxHeight >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_R_16_SINT].maxDimensions.maxDepth >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_R_16_SINT].maxLevelsCount >= 12);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_R_16_SINT].maxLayersCount >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_R_16_SINT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_R_16_SINT].maxBytesCount >= 2147483648); // Intel UHD Graphics 730 on Windows 10. Not used: RTX 2060 on Windows 10 is 1099511627776
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_R_16_SINT].status == RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_R_16_FLOAT].maxDimensions.maxWidth >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_R_16_FLOAT].maxDimensions.maxHeight >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_R_16_FLOAT].maxDimensions.maxDepth >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_R_16_FLOAT].maxLevelsCount >= 12);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_R_16_FLOAT].maxLayersCount >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_R_16_FLOAT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_R_16_FLOAT].maxBytesCount >= 2147483648); // Intel UHD Graphics 730 on Windows 10. Not used: RTX 2060 on Windows 10 is 1099511627776
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_R_16_FLOAT].status == RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_RGBA_16_16_16_16_UINT].maxDimensions.maxWidth >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_RGBA_16_16_16_16_UINT].maxDimensions.maxHeight >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_RGBA_16_16_16_16_UINT].maxDimensions.maxDepth >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_RGBA_16_16_16_16_UINT].maxLevelsCount >= 12);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_RGBA_16_16_16_16_UINT].maxLayersCount >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_RGBA_16_16_16_16_UINT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_RGBA_16_16_16_16_UINT].maxBytesCount >= 2147483648); // Intel UHD Graphics 730 on Windows 10. Not used: RTX 2060 on Windows 10 is 1099511627776
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_RGBA_16_16_16_16_UINT].status == RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_RGBA_16_16_16_16_SINT].maxDimensions.maxWidth >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_RGBA_16_16_16_16_SINT].maxDimensions.maxHeight >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_RGBA_16_16_16_16_SINT].maxDimensions.maxDepth >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_RGBA_16_16_16_16_SINT].maxLevelsCount >= 12);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_RGBA_16_16_16_16_SINT].maxLayersCount >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_RGBA_16_16_16_16_SINT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_RGBA_16_16_16_16_SINT].maxBytesCount >= 2147483648); // Intel UHD Graphics 730 on Windows 10. Not used: RTX 2060 on Windows 10 is 1099511627776
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_RGBA_16_16_16_16_SINT].status == RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_RGBA_16_16_16_16_FLOAT].maxDimensions.maxWidth >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_RGBA_16_16_16_16_FLOAT].maxDimensions.maxHeight >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_RGBA_16_16_16_16_FLOAT].maxDimensions.maxDepth >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_RGBA_16_16_16_16_FLOAT].maxLevelsCount >= 12);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_RGBA_16_16_16_16_FLOAT].maxLayersCount >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_RGBA_16_16_16_16_FLOAT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_RGBA_16_16_16_16_FLOAT].maxBytesCount >= 2147483648); // Intel UHD Graphics 730 on Windows 10. Not used: RTX 2060 on Windows 10 is 1099511627776
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_RGBA_16_16_16_16_FLOAT].status == RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_R_32_UINT].maxDimensions.maxWidth >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_R_32_UINT].maxDimensions.maxHeight >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_R_32_UINT].maxDimensions.maxDepth >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_R_32_UINT].maxLevelsCount >= 12);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_R_32_UINT].maxLayersCount >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_R_32_UINT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_R_32_UINT].maxBytesCount >= 2147483648); // Intel UHD Graphics 730 on Windows 10. Not used: RTX 2060 on Windows 10 is 1099511627776
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_R_32_UINT].status == RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_R_32_SINT].maxDimensions.maxWidth >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_R_32_SINT].maxDimensions.maxHeight >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_R_32_SINT].maxDimensions.maxDepth >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_R_32_SINT].maxLevelsCount >= 12);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_R_32_SINT].maxLayersCount >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_R_32_SINT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_R_32_SINT].maxBytesCount >= 2147483648); // Intel UHD Graphics 730 on Windows 10. Not used: RTX 2060 on Windows 10 is 1099511627776
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_R_32_SINT].status == RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_R_32_FLOAT].maxDimensions.maxWidth >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_R_32_FLOAT].maxDimensions.maxHeight >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_R_32_FLOAT].maxDimensions.maxDepth >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_R_32_FLOAT].maxLevelsCount >= 12);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_R_32_FLOAT].maxLayersCount >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_R_32_FLOAT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_R_32_FLOAT].maxBytesCount >= 2147483648); // Intel UHD Graphics 730 on Windows 10. Not used: RTX 2060 on Windows 10 is 1099511627776
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_R_32_FLOAT].status == RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_RGBA_32_32_32_32_UINT].maxDimensions.maxWidth >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_RGBA_32_32_32_32_UINT].maxDimensions.maxHeight >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_RGBA_32_32_32_32_UINT].maxDimensions.maxDepth >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_RGBA_32_32_32_32_UINT].maxLevelsCount >= 12);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_RGBA_32_32_32_32_UINT].maxLayersCount >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_RGBA_32_32_32_32_UINT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_RGBA_32_32_32_32_UINT].maxBytesCount >= 2147483648); // Intel UHD Graphics 730 on Windows 10. Not used: RTX 2060 on Windows 10 is 1099511627776
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_RGBA_32_32_32_32_UINT].status == RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_RGBA_32_32_32_32_SINT].maxDimensions.maxWidth >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_RGBA_32_32_32_32_SINT].maxDimensions.maxHeight >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_RGBA_32_32_32_32_SINT].maxDimensions.maxDepth >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_RGBA_32_32_32_32_SINT].maxLevelsCount >= 12);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_RGBA_32_32_32_32_SINT].maxLayersCount >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_RGBA_32_32_32_32_SINT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_RGBA_32_32_32_32_SINT].maxBytesCount >= 2147483648); // Intel UHD Graphics 730 on Windows 10. Not used: RTX 2060 on Windows 10 is 1099511627776
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_RGBA_32_32_32_32_SINT].status == RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_RGBA_32_32_32_32_FLOAT].maxDimensions.maxWidth >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_RGBA_32_32_32_32_FLOAT].maxDimensions.maxHeight >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_RGBA_32_32_32_32_FLOAT].maxDimensions.maxDepth >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_RGBA_32_32_32_32_FLOAT].maxLevelsCount >= 12);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_RGBA_32_32_32_32_FLOAT].maxLayersCount >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_RGBA_32_32_32_32_FLOAT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_RGBA_32_32_32_32_FLOAT].maxBytesCount >= 2147483648); // Intel UHD Graphics 730 on Windows 10. Not used: RTX 2060 on Windows 10 is 1099511627776
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_RGBA_32_32_32_32_FLOAT].status == RED_STATUS_SUCCESS);
  if (gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_DEPTH_16_UINT_TO_FLOAT_0_1].status != RED_STATUS_ERROR_FORMAT_IS_NOT_SUPPORTED) { // AMD RX 550 on Windows 10 doesn't support this format
    REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_DEPTH_16_UINT_TO_FLOAT_0_1].maxDimensions.maxWidth >= 2048);
    REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_DEPTH_16_UINT_TO_FLOAT_0_1].maxDimensions.maxHeight >= 2048);
    REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_DEPTH_16_UINT_TO_FLOAT_0_1].maxDimensions.maxDepth >= 2048);
    REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_DEPTH_16_UINT_TO_FLOAT_0_1].maxLevelsCount >= 12);
    REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_DEPTH_16_UINT_TO_FLOAT_0_1].maxLayersCount >= 1);
    REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_DEPTH_16_UINT_TO_FLOAT_0_1].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
    REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_DEPTH_16_UINT_TO_FLOAT_0_1].maxBytesCount >= 2147483648); // Intel UHD Graphics 730 on Windows 10. Not used: RTX 2060 on Windows 10 is 1099511627776
    REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_DEPTH_16_UINT_TO_FLOAT_0_1].status == RED_STATUS_SUCCESS);
  }
  if (gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_DEPTH_32_FLOAT].status != RED_STATUS_ERROR_FORMAT_IS_NOT_SUPPORTED) { // AMD RX 550 on Windows 10 doesn't support this format
    REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_DEPTH_32_FLOAT].maxDimensions.maxWidth >= 2048);
    REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_DEPTH_32_FLOAT].maxDimensions.maxHeight >= 2048);
    REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_DEPTH_32_FLOAT].maxDimensions.maxDepth >= 2048);
    REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_DEPTH_32_FLOAT].maxLevelsCount >= 12);
    REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_DEPTH_32_FLOAT].maxLayersCount >= 1);
    REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_DEPTH_32_FLOAT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
    REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_DEPTH_32_FLOAT].maxBytesCount >= 2147483648); // Intel UHD Graphics 730 on Windows 10. Not used: RTX 2060 on Windows 10 is 1099511627776
    REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_DEPTH_32_FLOAT].status == RED_STATUS_SUCCESS);
  }
  if (gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_DEPTH_24_UINT_TO_FLOAT_0_1_STENCIL_8_UINT].status != RED_STATUS_ERROR_FORMAT_IS_NOT_SUPPORTED) { // AMD RX 550 on Windows 10 doesn't support this format
    REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_DEPTH_24_UINT_TO_FLOAT_0_1_STENCIL_8_UINT].maxDimensions.maxWidth >= 2048);
    REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_DEPTH_24_UINT_TO_FLOAT_0_1_STENCIL_8_UINT].maxDimensions.maxHeight >= 2048);
    REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_DEPTH_24_UINT_TO_FLOAT_0_1_STENCIL_8_UINT].maxDimensions.maxDepth >= 2048);
    REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_DEPTH_24_UINT_TO_FLOAT_0_1_STENCIL_8_UINT].maxLevelsCount >= 12);
    REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_DEPTH_24_UINT_TO_FLOAT_0_1_STENCIL_8_UINT].maxLayersCount >= 1);
    REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_DEPTH_24_UINT_TO_FLOAT_0_1_STENCIL_8_UINT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
    REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_DEPTH_24_UINT_TO_FLOAT_0_1_STENCIL_8_UINT].maxBytesCount >= 2147483648); // Intel UHD Graphics 730 on Windows 10. Not used: RTX 2060 on Windows 10 is 1099511627776
    REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_DEPTH_24_UINT_TO_FLOAT_0_1_STENCIL_8_UINT].status == RED_STATUS_SUCCESS);
  }
  if (gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_DEPTH_32_FLOAT_STENCIL_8_UINT].status != RED_STATUS_ERROR_FORMAT_IS_NOT_SUPPORTED) { // AMD RX 550 on Windows 10 doesn't support this format
    REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_DEPTH_32_FLOAT_STENCIL_8_UINT].maxDimensions.maxWidth >= 2048);
    REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_DEPTH_32_FLOAT_STENCIL_8_UINT].maxDimensions.maxHeight >= 2048);
    REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_DEPTH_32_FLOAT_STENCIL_8_UINT].maxDimensions.maxDepth >= 2048);
    REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_DEPTH_32_FLOAT_STENCIL_8_UINT].maxLevelsCount >= 12);
    REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_DEPTH_32_FLOAT_STENCIL_8_UINT].maxLayersCount >= 1);
    REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_DEPTH_32_FLOAT_STENCIL_8_UINT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
    REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_DEPTH_32_FLOAT_STENCIL_8_UINT].maxBytesCount >= 2147483648); // Intel UHD Graphics 730 on Windows 10. Not used: RTX 2060 on Windows 10 is 1099511627776
    REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3D[RED_FORMAT_DEPTH_32_FLOAT_STENCIL_8_UINT].status == RED_STATUS_SUCCESS);
  }
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_R_8_UINT_TO_FLOAT_0_1].maxDimensions.maxWidth >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_R_8_UINT_TO_FLOAT_0_1].maxDimensions.maxHeight >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_R_8_UINT_TO_FLOAT_0_1].maxDimensions.maxDepth >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_R_8_UINT_TO_FLOAT_0_1].maxLevelsCount >= 12);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_R_8_UINT_TO_FLOAT_0_1].maxLayersCount >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_R_8_UINT_TO_FLOAT_0_1].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_R_8_UINT_TO_FLOAT_0_1].maxBytesCount >= 2147483648); // Intel UHD Graphics 730 on Windows 10. Not used: RTX 2060 on Windows 10 is 1099511627776
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_R_8_UINT_TO_FLOAT_0_1].status == RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_R_8_UINT].maxDimensions.maxWidth >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_R_8_UINT].maxDimensions.maxHeight >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_R_8_UINT].maxDimensions.maxDepth >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_R_8_UINT].maxLevelsCount >= 12);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_R_8_UINT].maxLayersCount >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_R_8_UINT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_R_8_UINT].maxBytesCount >= 2147483648); // Intel UHD Graphics 730 on Windows 10. Not used: RTX 2060 on Windows 10 is 1099511627776
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_R_8_UINT].status == RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_R_8_SINT].maxDimensions.maxWidth >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_R_8_SINT].maxDimensions.maxHeight >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_R_8_SINT].maxDimensions.maxDepth >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_R_8_SINT].maxLevelsCount >= 12);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_R_8_SINT].maxLayersCount >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_R_8_SINT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_R_8_SINT].maxBytesCount >= 2147483648); // Intel UHD Graphics 730 on Windows 10. Not used: RTX 2060 on Windows 10 is 1099511627776
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_R_8_SINT].status == RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1].maxDimensions.maxWidth >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1].maxDimensions.maxHeight >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1].maxDimensions.maxDepth >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1].maxLevelsCount >= 12);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1].maxLayersCount >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1].maxBytesCount >= 2147483648); // Intel UHD Graphics 730 on Windows 10. Not used: RTX 2060 on Windows 10 is 1099511627776
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1].status == RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_RGBA_8_8_8_8_UINT].maxDimensions.maxWidth >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_RGBA_8_8_8_8_UINT].maxDimensions.maxHeight >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_RGBA_8_8_8_8_UINT].maxDimensions.maxDepth >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_RGBA_8_8_8_8_UINT].maxLevelsCount >= 12);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_RGBA_8_8_8_8_UINT].maxLayersCount >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_RGBA_8_8_8_8_UINT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_RGBA_8_8_8_8_UINT].maxBytesCount >= 2147483648); // Intel UHD Graphics 730 on Windows 10. Not used: RTX 2060 on Windows 10 is 1099511627776
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_RGBA_8_8_8_8_UINT].status == RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_RGBA_8_8_8_8_SINT].maxDimensions.maxWidth >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_RGBA_8_8_8_8_SINT].maxDimensions.maxHeight >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_RGBA_8_8_8_8_SINT].maxDimensions.maxDepth >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_RGBA_8_8_8_8_SINT].maxLevelsCount >= 12);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_RGBA_8_8_8_8_SINT].maxLayersCount >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_RGBA_8_8_8_8_SINT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_RGBA_8_8_8_8_SINT].maxBytesCount >= 2147483648); // Intel UHD Graphics 730 on Windows 10. Not used: RTX 2060 on Windows 10 is 1099511627776
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_RGBA_8_8_8_8_SINT].status == RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1_GAMMA_CORRECTED].maxDimensions.maxWidth >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1_GAMMA_CORRECTED].maxDimensions.maxHeight >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1_GAMMA_CORRECTED].maxDimensions.maxDepth >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1_GAMMA_CORRECTED].maxLevelsCount >= 12);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1_GAMMA_CORRECTED].maxLayersCount >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1_GAMMA_CORRECTED].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1_GAMMA_CORRECTED].maxBytesCount >= 2147483648); // Intel UHD Graphics 730 on Windows 10. Not used: RTX 2060 on Windows 10 is 1099511627776
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1_GAMMA_CORRECTED].status == RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_PRESENT_BGRA_8_8_8_8_UINT_TO_FLOAT_0_1].maxDimensions.maxWidth >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_PRESENT_BGRA_8_8_8_8_UINT_TO_FLOAT_0_1].maxDimensions.maxHeight >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_PRESENT_BGRA_8_8_8_8_UINT_TO_FLOAT_0_1].maxDimensions.maxDepth >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_PRESENT_BGRA_8_8_8_8_UINT_TO_FLOAT_0_1].maxLevelsCount >= 12);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_PRESENT_BGRA_8_8_8_8_UINT_TO_FLOAT_0_1].maxLayersCount >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_PRESENT_BGRA_8_8_8_8_UINT_TO_FLOAT_0_1].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_PRESENT_BGRA_8_8_8_8_UINT_TO_FLOAT_0_1].maxBytesCount >= 2147483648); // Intel UHD Graphics 730 on Windows 10. Not used: RTX 2060 on Windows 10 is 1099511627776
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_PRESENT_BGRA_8_8_8_8_UINT_TO_FLOAT_0_1].status == RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_R_16_UINT].maxDimensions.maxWidth >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_R_16_UINT].maxDimensions.maxHeight >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_R_16_UINT].maxDimensions.maxDepth >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_R_16_UINT].maxLevelsCount >= 12);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_R_16_UINT].maxLayersCount >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_R_16_UINT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_R_16_UINT].maxBytesCount >= 2147483648); // Intel UHD Graphics 730 on Windows 10. Not used: RTX 2060 on Windows 10 is 1099511627776
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_R_16_UINT].status == RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_R_16_SINT].maxDimensions.maxWidth >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_R_16_SINT].maxDimensions.maxHeight >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_R_16_SINT].maxDimensions.maxDepth >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_R_16_SINT].maxLevelsCount >= 12);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_R_16_SINT].maxLayersCount >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_R_16_SINT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_R_16_SINT].maxBytesCount >= 2147483648); // Intel UHD Graphics 730 on Windows 10. Not used: RTX 2060 on Windows 10 is 1099511627776
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_R_16_SINT].status == RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_R_16_FLOAT].maxDimensions.maxWidth >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_R_16_FLOAT].maxDimensions.maxHeight >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_R_16_FLOAT].maxDimensions.maxDepth >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_R_16_FLOAT].maxLevelsCount >= 12);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_R_16_FLOAT].maxLayersCount >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_R_16_FLOAT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_R_16_FLOAT].maxBytesCount >= 2147483648); // Intel UHD Graphics 730 on Windows 10. Not used: RTX 2060 on Windows 10 is 1099511627776
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_R_16_FLOAT].status == RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_RGBA_16_16_16_16_UINT].maxDimensions.maxWidth >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_RGBA_16_16_16_16_UINT].maxDimensions.maxHeight >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_RGBA_16_16_16_16_UINT].maxDimensions.maxDepth >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_RGBA_16_16_16_16_UINT].maxLevelsCount >= 12);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_RGBA_16_16_16_16_UINT].maxLayersCount >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_RGBA_16_16_16_16_UINT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_RGBA_16_16_16_16_UINT].maxBytesCount >= 2147483648); // Intel UHD Graphics 730 on Windows 10. Not used: RTX 2060 on Windows 10 is 1099511627776
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_RGBA_16_16_16_16_UINT].status == RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_RGBA_16_16_16_16_SINT].maxDimensions.maxWidth >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_RGBA_16_16_16_16_SINT].maxDimensions.maxHeight >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_RGBA_16_16_16_16_SINT].maxDimensions.maxDepth >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_RGBA_16_16_16_16_SINT].maxLevelsCount >= 12);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_RGBA_16_16_16_16_SINT].maxLayersCount >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_RGBA_16_16_16_16_SINT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_RGBA_16_16_16_16_SINT].maxBytesCount >= 2147483648); // Intel UHD Graphics 730 on Windows 10. Not used: RTX 2060 on Windows 10 is 1099511627776
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_RGBA_16_16_16_16_SINT].status == RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_RGBA_16_16_16_16_FLOAT].maxDimensions.maxWidth >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_RGBA_16_16_16_16_FLOAT].maxDimensions.maxHeight >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_RGBA_16_16_16_16_FLOAT].maxDimensions.maxDepth >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_RGBA_16_16_16_16_FLOAT].maxLevelsCount >= 12);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_RGBA_16_16_16_16_FLOAT].maxLayersCount >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_RGBA_16_16_16_16_FLOAT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_RGBA_16_16_16_16_FLOAT].maxBytesCount >= 2147483648); // Intel UHD Graphics 730 on Windows 10. Not used: RTX 2060 on Windows 10 is 1099511627776
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_RGBA_16_16_16_16_FLOAT].status == RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_R_32_UINT].maxDimensions.maxWidth >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_R_32_UINT].maxDimensions.maxHeight >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_R_32_UINT].maxDimensions.maxDepth >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_R_32_UINT].maxLevelsCount >= 12);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_R_32_UINT].maxLayersCount >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_R_32_UINT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_R_32_UINT].maxBytesCount >= 2147483648); // Intel UHD Graphics 730 on Windows 10. Not used: RTX 2060 on Windows 10 is 1099511627776
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_R_32_UINT].status == RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_R_32_SINT].maxDimensions.maxWidth >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_R_32_SINT].maxDimensions.maxHeight >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_R_32_SINT].maxDimensions.maxDepth >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_R_32_SINT].maxLevelsCount >= 12);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_R_32_SINT].maxLayersCount >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_R_32_SINT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_R_32_SINT].maxBytesCount >= 2147483648); // Intel UHD Graphics 730 on Windows 10. Not used: RTX 2060 on Windows 10 is 1099511627776
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_R_32_SINT].status == RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_R_32_FLOAT].maxDimensions.maxWidth >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_R_32_FLOAT].maxDimensions.maxHeight >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_R_32_FLOAT].maxDimensions.maxDepth >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_R_32_FLOAT].maxLevelsCount >= 12);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_R_32_FLOAT].maxLayersCount >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_R_32_FLOAT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_R_32_FLOAT].maxBytesCount >= 2147483648); // Intel UHD Graphics 730 on Windows 10. Not used: RTX 2060 on Windows 10 is 1099511627776
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_R_32_FLOAT].status == RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_RGBA_32_32_32_32_UINT].maxDimensions.maxWidth >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_RGBA_32_32_32_32_UINT].maxDimensions.maxHeight >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_RGBA_32_32_32_32_UINT].maxDimensions.maxDepth >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_RGBA_32_32_32_32_UINT].maxLevelsCount >= 12);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_RGBA_32_32_32_32_UINT].maxLayersCount >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_RGBA_32_32_32_32_UINT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_RGBA_32_32_32_32_UINT].maxBytesCount >= 2147483648); // Intel UHD Graphics 730 on Windows 10. Not used: RTX 2060 on Windows 10 is 1099511627776
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_RGBA_32_32_32_32_UINT].status == RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_RGBA_32_32_32_32_SINT].maxDimensions.maxWidth >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_RGBA_32_32_32_32_SINT].maxDimensions.maxHeight >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_RGBA_32_32_32_32_SINT].maxDimensions.maxDepth >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_RGBA_32_32_32_32_SINT].maxLevelsCount >= 12);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_RGBA_32_32_32_32_SINT].maxLayersCount >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_RGBA_32_32_32_32_SINT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_RGBA_32_32_32_32_SINT].maxBytesCount >= 2147483648); // Intel UHD Graphics 730 on Windows 10. Not used: RTX 2060 on Windows 10 is 1099511627776
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_RGBA_32_32_32_32_SINT].status == RED_STATUS_SUCCESS);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_RGBA_32_32_32_32_FLOAT].maxDimensions.maxWidth >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_RGBA_32_32_32_32_FLOAT].maxDimensions.maxHeight >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_RGBA_32_32_32_32_FLOAT].maxDimensions.maxDepth >= 2048);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_RGBA_32_32_32_32_FLOAT].maxLevelsCount >= 12);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_RGBA_32_32_32_32_FLOAT].maxLayersCount >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_RGBA_32_32_32_32_FLOAT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_RGBA_32_32_32_32_FLOAT].maxBytesCount >= 2147483648); // Intel UHD Graphics 730 on Windows 10. Not used: RTX 2060 on Windows 10 is 1099511627776
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_RGBA_32_32_32_32_FLOAT].status == RED_STATUS_SUCCESS);
  if (gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_DEPTH_16_UINT_TO_FLOAT_0_1].status != RED_STATUS_ERROR_FORMAT_IS_NOT_SUPPORTED) { // AMD RX 550 on Windows 10 doesn't support this format
    REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_DEPTH_16_UINT_TO_FLOAT_0_1].maxDimensions.maxWidth >= 2048);
    REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_DEPTH_16_UINT_TO_FLOAT_0_1].maxDimensions.maxHeight >= 2048);
    REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_DEPTH_16_UINT_TO_FLOAT_0_1].maxDimensions.maxDepth >= 2048);
    REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_DEPTH_16_UINT_TO_FLOAT_0_1].maxLevelsCount >= 12);
    REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_DEPTH_16_UINT_TO_FLOAT_0_1].maxLayersCount >= 1);
    REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_DEPTH_16_UINT_TO_FLOAT_0_1].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
    REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_DEPTH_16_UINT_TO_FLOAT_0_1].maxBytesCount >= 2147483648); // Intel UHD Graphics 730 on Windows 10. Not used: RTX 2060 on Windows 10 is 1099511627776
    REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_DEPTH_16_UINT_TO_FLOAT_0_1].status == RED_STATUS_SUCCESS);
  }
  if (gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_DEPTH_32_FLOAT].status != RED_STATUS_ERROR_FORMAT_IS_NOT_SUPPORTED) { // AMD RX 550 on Windows 10 doesn't support this format
    REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_DEPTH_32_FLOAT].maxDimensions.maxWidth >= 2048);
    REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_DEPTH_32_FLOAT].maxDimensions.maxHeight >= 2048);
    REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_DEPTH_32_FLOAT].maxDimensions.maxDepth >= 2048);
    REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_DEPTH_32_FLOAT].maxLevelsCount >= 12);
    REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_DEPTH_32_FLOAT].maxLayersCount >= 1);
    REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_DEPTH_32_FLOAT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
    REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_DEPTH_32_FLOAT].maxBytesCount >= 2147483648); // Intel UHD Graphics 730 on Windows 10. Not used: RTX 2060 on Windows 10 is 1099511627776
    REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_DEPTH_32_FLOAT].status == RED_STATUS_SUCCESS);
  }
  if (gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_DEPTH_24_UINT_TO_FLOAT_0_1_STENCIL_8_UINT].status != RED_STATUS_ERROR_FORMAT_IS_NOT_SUPPORTED) { // AMD RX 550 on Windows 10 doesn't support this format
    REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_DEPTH_24_UINT_TO_FLOAT_0_1_STENCIL_8_UINT].maxDimensions.maxWidth >= 2048);
    REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_DEPTH_24_UINT_TO_FLOAT_0_1_STENCIL_8_UINT].maxDimensions.maxHeight >= 2048);
    REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_DEPTH_24_UINT_TO_FLOAT_0_1_STENCIL_8_UINT].maxDimensions.maxDepth >= 2048);
    REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_DEPTH_24_UINT_TO_FLOAT_0_1_STENCIL_8_UINT].maxLevelsCount >= 12);
    REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_DEPTH_24_UINT_TO_FLOAT_0_1_STENCIL_8_UINT].maxLayersCount >= 1);
    REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_DEPTH_24_UINT_TO_FLOAT_0_1_STENCIL_8_UINT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
    REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_DEPTH_24_UINT_TO_FLOAT_0_1_STENCIL_8_UINT].maxBytesCount >= 2147483648); // Intel UHD Graphics 730 on Windows 10. Not used: RTX 2060 on Windows 10 is 1099511627776
    REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_DEPTH_24_UINT_TO_FLOAT_0_1_STENCIL_8_UINT].status == RED_STATUS_SUCCESS);
  }
  if (gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_DEPTH_32_FLOAT_STENCIL_8_UINT].status != RED_STATUS_ERROR_FORMAT_IS_NOT_SUPPORTED) { // AMD RX 550 on Windows 10 doesn't support this format
    REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_DEPTH_32_FLOAT_STENCIL_8_UINT].maxDimensions.maxWidth >= 2048);
    REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_DEPTH_32_FLOAT_STENCIL_8_UINT].maxDimensions.maxHeight >= 2048);
    REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_DEPTH_32_FLOAT_STENCIL_8_UINT].maxDimensions.maxDepth >= 2048);
    REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_DEPTH_32_FLOAT_STENCIL_8_UINT].maxLevelsCount >= 12);
    REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_DEPTH_32_FLOAT_STENCIL_8_UINT].maxLayersCount >= 1);
    REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_DEPTH_32_FLOAT_STENCIL_8_UINT].supportedMultisampleCounts >= (RED_MULTISAMPLE_COUNT_BITFLAG_1));
    REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_DEPTH_32_FLOAT_STENCIL_8_UINT].maxBytesCount >= 2147483648); // Intel UHD Graphics 730 on Windows 10. Not used: RTX 2060 on Windows 10 is 1099511627776
    REDGPU_2_EXPECTWG(gpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[RED_FORMAT_DEPTH_32_FLOAT_STENCIL_8_UINT].status == RED_STATUS_SUCCESS);
  }
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_R_8_UINT_TO_FLOAT_0_1].supportsCopyR >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_R_8_UINT_TO_FLOAT_0_1].supportsCopyW >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_R_8_UINT_TO_FLOAT_0_1].supportsTextureRO >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_R_8_UINT_TO_FLOAT_0_1].supportsTextureRW >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_R_8_UINT_TO_FLOAT_0_1].supportsOutputDepthStencil >= 0);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_R_8_UINT_TO_FLOAT_0_1].supportsOutputColor >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_R_8_UINT_TO_FLOAT_0_1].supportsTextureROFilteringLinear >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_R_8_UINT_TO_FLOAT_0_1].supportsTextureRWAtomics >= 0);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_R_8_UINT_TO_FLOAT_0_1].supportsOutputColorBlend >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_R_8_UINT].supportsCopyR >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_R_8_UINT].supportsCopyW >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_R_8_UINT].supportsTextureRO >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_R_8_UINT].supportsTextureRW >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_R_8_UINT].supportsOutputDepthStencil >= 0);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_R_8_UINT].supportsOutputColor >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_R_8_UINT].supportsTextureROFilteringLinear >= 0);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_R_8_UINT].supportsTextureRWAtomics >= 0);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_R_8_UINT].supportsOutputColorBlend >= 0);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_R_8_SINT].supportsCopyR >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_R_8_SINT].supportsCopyW >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_R_8_SINT].supportsTextureRO >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_R_8_SINT].supportsTextureRW >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_R_8_SINT].supportsOutputDepthStencil >= 0);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_R_8_SINT].supportsOutputColor >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_R_8_SINT].supportsTextureROFilteringLinear >= 0);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_R_8_SINT].supportsTextureRWAtomics >= 0);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_R_8_SINT].supportsOutputColorBlend >= 0);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1].supportsCopyR >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1].supportsCopyW >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1].supportsTextureRO >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1].supportsTextureRW >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1].supportsOutputDepthStencil >= 0);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1].supportsOutputColor >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1].supportsTextureROFilteringLinear >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1].supportsTextureRWAtomics >= 0);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1].supportsOutputColorBlend >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_RGBA_8_8_8_8_UINT].supportsCopyR >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_RGBA_8_8_8_8_UINT].supportsCopyW >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_RGBA_8_8_8_8_UINT].supportsTextureRO >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_RGBA_8_8_8_8_UINT].supportsTextureRW >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_RGBA_8_8_8_8_UINT].supportsOutputDepthStencil >= 0);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_RGBA_8_8_8_8_UINT].supportsOutputColor >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_RGBA_8_8_8_8_UINT].supportsTextureROFilteringLinear >= 0);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_RGBA_8_8_8_8_UINT].supportsTextureRWAtomics >= 0);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_RGBA_8_8_8_8_UINT].supportsOutputColorBlend >= 0);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_RGBA_8_8_8_8_SINT].supportsCopyR >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_RGBA_8_8_8_8_SINT].supportsCopyW >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_RGBA_8_8_8_8_SINT].supportsTextureRO >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_RGBA_8_8_8_8_SINT].supportsTextureRW >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_RGBA_8_8_8_8_SINT].supportsOutputDepthStencil >= 0);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_RGBA_8_8_8_8_SINT].supportsOutputColor >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_RGBA_8_8_8_8_SINT].supportsTextureROFilteringLinear >= 0);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_RGBA_8_8_8_8_SINT].supportsTextureRWAtomics >= 0);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_RGBA_8_8_8_8_SINT].supportsOutputColorBlend >= 0);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1_GAMMA_CORRECTED].supportsCopyR >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1_GAMMA_CORRECTED].supportsCopyW >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1_GAMMA_CORRECTED].supportsTextureRO >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1_GAMMA_CORRECTED].supportsTextureRW >= 0);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1_GAMMA_CORRECTED].supportsOutputDepthStencil >= 0);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1_GAMMA_CORRECTED].supportsOutputColor >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1_GAMMA_CORRECTED].supportsTextureROFilteringLinear >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1_GAMMA_CORRECTED].supportsTextureRWAtomics >= 0);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1_GAMMA_CORRECTED].supportsOutputColorBlend >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_PRESENT_BGRA_8_8_8_8_UINT_TO_FLOAT_0_1].supportsCopyR >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_PRESENT_BGRA_8_8_8_8_UINT_TO_FLOAT_0_1].supportsCopyW >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_PRESENT_BGRA_8_8_8_8_UINT_TO_FLOAT_0_1].supportsTextureRO >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_PRESENT_BGRA_8_8_8_8_UINT_TO_FLOAT_0_1].supportsTextureRW >= 0);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_PRESENT_BGRA_8_8_8_8_UINT_TO_FLOAT_0_1].supportsOutputDepthStencil >= 0);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_PRESENT_BGRA_8_8_8_8_UINT_TO_FLOAT_0_1].supportsOutputColor >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_PRESENT_BGRA_8_8_8_8_UINT_TO_FLOAT_0_1].supportsTextureROFilteringLinear >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_PRESENT_BGRA_8_8_8_8_UINT_TO_FLOAT_0_1].supportsTextureRWAtomics >= 0);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_PRESENT_BGRA_8_8_8_8_UINT_TO_FLOAT_0_1].supportsOutputColorBlend >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_R_16_UINT].supportsCopyR >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_R_16_UINT].supportsCopyW >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_R_16_UINT].supportsTextureRO >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_R_16_UINT].supportsTextureRW >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_R_16_UINT].supportsOutputDepthStencil >= 0);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_R_16_UINT].supportsOutputColor >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_R_16_UINT].supportsTextureROFilteringLinear >= 0);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_R_16_UINT].supportsTextureRWAtomics >= 0);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_R_16_UINT].supportsOutputColorBlend >= 0);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_R_16_SINT].supportsCopyR >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_R_16_SINT].supportsCopyW >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_R_16_SINT].supportsTextureRO >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_R_16_SINT].supportsTextureRW >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_R_16_SINT].supportsOutputDepthStencil >= 0);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_R_16_SINT].supportsOutputColor >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_R_16_SINT].supportsTextureROFilteringLinear >= 0);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_R_16_SINT].supportsTextureRWAtomics >= 0);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_R_16_SINT].supportsOutputColorBlend >= 0);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_R_16_FLOAT].supportsCopyR >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_R_16_FLOAT].supportsCopyW >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_R_16_FLOAT].supportsTextureRO >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_R_16_FLOAT].supportsTextureRW >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_R_16_FLOAT].supportsOutputDepthStencil >= 0);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_R_16_FLOAT].supportsOutputColor >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_R_16_FLOAT].supportsTextureROFilteringLinear >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_R_16_FLOAT].supportsTextureRWAtomics >= 0);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_R_16_FLOAT].supportsOutputColorBlend >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_RGBA_16_16_16_16_UINT].supportsCopyR >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_RGBA_16_16_16_16_UINT].supportsCopyW >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_RGBA_16_16_16_16_UINT].supportsTextureRO >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_RGBA_16_16_16_16_UINT].supportsTextureRW >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_RGBA_16_16_16_16_UINT].supportsOutputDepthStencil >= 0);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_RGBA_16_16_16_16_UINT].supportsOutputColor >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_RGBA_16_16_16_16_UINT].supportsTextureROFilteringLinear >= 0);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_RGBA_16_16_16_16_UINT].supportsTextureRWAtomics >= 0);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_RGBA_16_16_16_16_UINT].supportsOutputColorBlend >= 0);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_RGBA_16_16_16_16_SINT].supportsCopyR >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_RGBA_16_16_16_16_SINT].supportsCopyW >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_RGBA_16_16_16_16_SINT].supportsTextureRO >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_RGBA_16_16_16_16_SINT].supportsTextureRW >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_RGBA_16_16_16_16_SINT].supportsOutputDepthStencil >= 0);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_RGBA_16_16_16_16_SINT].supportsOutputColor >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_RGBA_16_16_16_16_SINT].supportsTextureROFilteringLinear >= 0);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_RGBA_16_16_16_16_SINT].supportsTextureRWAtomics >= 0);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_RGBA_16_16_16_16_SINT].supportsOutputColorBlend >= 0);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_RGBA_16_16_16_16_FLOAT].supportsCopyR >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_RGBA_16_16_16_16_FLOAT].supportsCopyW >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_RGBA_16_16_16_16_FLOAT].supportsTextureRO >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_RGBA_16_16_16_16_FLOAT].supportsTextureRW >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_RGBA_16_16_16_16_FLOAT].supportsOutputDepthStencil >= 0);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_RGBA_16_16_16_16_FLOAT].supportsOutputColor >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_RGBA_16_16_16_16_FLOAT].supportsTextureROFilteringLinear >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_RGBA_16_16_16_16_FLOAT].supportsTextureRWAtomics >= 0);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_RGBA_16_16_16_16_FLOAT].supportsOutputColorBlend >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_R_32_UINT].supportsCopyR >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_R_32_UINT].supportsCopyW >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_R_32_UINT].supportsTextureRO >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_R_32_UINT].supportsTextureRW >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_R_32_UINT].supportsOutputDepthStencil >= 0);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_R_32_UINT].supportsOutputColor >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_R_32_UINT].supportsTextureROFilteringLinear >= 0);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_R_32_UINT].supportsTextureRWAtomics >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_R_32_UINT].supportsOutputColorBlend >= 0);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_R_32_SINT].supportsCopyR >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_R_32_SINT].supportsCopyW >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_R_32_SINT].supportsTextureRO >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_R_32_SINT].supportsTextureRW >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_R_32_SINT].supportsOutputDepthStencil >= 0);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_R_32_SINT].supportsOutputColor >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_R_32_SINT].supportsTextureROFilteringLinear >= 0);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_R_32_SINT].supportsTextureRWAtomics >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_R_32_SINT].supportsOutputColorBlend >= 0);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_R_32_FLOAT].supportsCopyR >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_R_32_FLOAT].supportsCopyW >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_R_32_FLOAT].supportsTextureRO >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_R_32_FLOAT].supportsTextureRW >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_R_32_FLOAT].supportsOutputDepthStencil >= 0);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_R_32_FLOAT].supportsOutputColor >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_R_32_FLOAT].supportsTextureROFilteringLinear >= 0);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_R_32_FLOAT].supportsTextureRWAtomics >= 0);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_R_32_FLOAT].supportsOutputColorBlend >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_RGBA_32_32_32_32_UINT].supportsCopyR >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_RGBA_32_32_32_32_UINT].supportsCopyW >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_RGBA_32_32_32_32_UINT].supportsTextureRO >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_RGBA_32_32_32_32_UINT].supportsTextureRW >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_RGBA_32_32_32_32_UINT].supportsOutputDepthStencil >= 0);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_RGBA_32_32_32_32_UINT].supportsOutputColor >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_RGBA_32_32_32_32_UINT].supportsTextureROFilteringLinear >= 0);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_RGBA_32_32_32_32_UINT].supportsTextureRWAtomics >= 0);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_RGBA_32_32_32_32_UINT].supportsOutputColorBlend >= 0);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_RGBA_32_32_32_32_SINT].supportsCopyR >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_RGBA_32_32_32_32_SINT].supportsCopyW >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_RGBA_32_32_32_32_SINT].supportsTextureRO >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_RGBA_32_32_32_32_SINT].supportsTextureRW >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_RGBA_32_32_32_32_SINT].supportsOutputDepthStencil >= 0);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_RGBA_32_32_32_32_SINT].supportsOutputColor >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_RGBA_32_32_32_32_SINT].supportsTextureROFilteringLinear >= 0);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_RGBA_32_32_32_32_SINT].supportsTextureRWAtomics >= 0);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_RGBA_32_32_32_32_SINT].supportsOutputColorBlend >= 0);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_RGBA_32_32_32_32_FLOAT].supportsCopyR >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_RGBA_32_32_32_32_FLOAT].supportsCopyW >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_RGBA_32_32_32_32_FLOAT].supportsTextureRO >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_RGBA_32_32_32_32_FLOAT].supportsTextureRW >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_RGBA_32_32_32_32_FLOAT].supportsOutputDepthStencil >= 0);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_RGBA_32_32_32_32_FLOAT].supportsOutputColor >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_RGBA_32_32_32_32_FLOAT].supportsTextureROFilteringLinear >= 0);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_RGBA_32_32_32_32_FLOAT].supportsTextureRWAtomics >= 0);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_RGBA_32_32_32_32_FLOAT].supportsOutputColorBlend >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_DEPTH_16_UINT_TO_FLOAT_0_1].supportsCopyR >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_DEPTH_16_UINT_TO_FLOAT_0_1].supportsCopyW >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_DEPTH_16_UINT_TO_FLOAT_0_1].supportsTextureRO >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_DEPTH_16_UINT_TO_FLOAT_0_1].supportsTextureRW >= 0);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_DEPTH_16_UINT_TO_FLOAT_0_1].supportsOutputDepthStencil >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_DEPTH_16_UINT_TO_FLOAT_0_1].supportsOutputColor >= 0);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_DEPTH_16_UINT_TO_FLOAT_0_1].supportsTextureROFilteringLinear >= 0);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_DEPTH_16_UINT_TO_FLOAT_0_1].supportsTextureRWAtomics >= 0);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_DEPTH_16_UINT_TO_FLOAT_0_1].supportsOutputColorBlend >= 0);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_DEPTH_32_FLOAT].supportsCopyR >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_DEPTH_32_FLOAT].supportsCopyW >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_DEPTH_32_FLOAT].supportsTextureRO >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_DEPTH_32_FLOAT].supportsTextureRW >= 0);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_DEPTH_32_FLOAT].supportsOutputDepthStencil >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_DEPTH_32_FLOAT].supportsOutputColor >= 0);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_DEPTH_32_FLOAT].supportsTextureROFilteringLinear >= 0);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_DEPTH_32_FLOAT].supportsTextureRWAtomics >= 0);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_DEPTH_32_FLOAT].supportsOutputColorBlend >= 0);
  if (gpuInfo->imageFormatsLimitsImageDimensions2D[RED_FORMAT_DEPTH_24_UINT_TO_FLOAT_0_1_STENCIL_8_UINT].status != RED_STATUS_ERROR_FORMAT_IS_NOT_SUPPORTED) { // AMD RX 550 on Windows 10 doesn't support this format
    REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_DEPTH_24_UINT_TO_FLOAT_0_1_STENCIL_8_UINT].supportsCopyR >= 1);
    REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_DEPTH_24_UINT_TO_FLOAT_0_1_STENCIL_8_UINT].supportsCopyW >= 1);
    REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_DEPTH_24_UINT_TO_FLOAT_0_1_STENCIL_8_UINT].supportsTextureRO >= 1);
    REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_DEPTH_24_UINT_TO_FLOAT_0_1_STENCIL_8_UINT].supportsTextureRW >= 0);
    REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_DEPTH_24_UINT_TO_FLOAT_0_1_STENCIL_8_UINT].supportsOutputDepthStencil >= 1);
    REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_DEPTH_24_UINT_TO_FLOAT_0_1_STENCIL_8_UINT].supportsOutputColor >= 0);
    REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_DEPTH_24_UINT_TO_FLOAT_0_1_STENCIL_8_UINT].supportsTextureROFilteringLinear >= 1);
    REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_DEPTH_24_UINT_TO_FLOAT_0_1_STENCIL_8_UINT].supportsTextureRWAtomics >= 0);
    REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_DEPTH_24_UINT_TO_FLOAT_0_1_STENCIL_8_UINT].supportsOutputColorBlend >= 0);
  }
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_DEPTH_32_FLOAT_STENCIL_8_UINT].supportsCopyR >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_DEPTH_32_FLOAT_STENCIL_8_UINT].supportsCopyW >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_DEPTH_32_FLOAT_STENCIL_8_UINT].supportsTextureRO >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_DEPTH_32_FLOAT_STENCIL_8_UINT].supportsTextureRW >= 0);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_DEPTH_32_FLOAT_STENCIL_8_UINT].supportsOutputDepthStencil >= 1);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_DEPTH_32_FLOAT_STENCIL_8_UINT].supportsOutputColor >= 0);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_DEPTH_32_FLOAT_STENCIL_8_UINT].supportsTextureROFilteringLinear >= 0);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_DEPTH_32_FLOAT_STENCIL_8_UINT].supportsTextureRWAtomics >= 0);
  REDGPU_2_EXPECTWG(gpuInfo->imageFormatsFeatures[RED_FORMAT_DEPTH_32_FLOAT_STENCIL_8_UINT].supportsOutputColorBlend >= 0);
}

REDGPU_2_DECLSPEC void REDGPU_2_API red2ExpectMemoryTypes(const RedGpuInfo * gpuInfo, unsigned expectedMemoryHeapsCount, const RedMemoryHeap * expectedMemoryHeaps, unsigned expectedMemoryTypesCount, const RedMemoryType * expectedMemoryTypes, const char * optionalFile, int optionalLine) {
  RedHandleGpu gpu = gpuInfo->gpu;
  REDGPU_2_EXPECTWG(expectedMemoryHeapsCount == gpuInfo->memoryHeapsCount);
  REDGPU_2_EXPECTWG(expectedMemoryTypesCount == gpuInfo->memoryTypesCount);
  for (unsigned i = 0; i < gpuInfo->memoryHeapsCount; i += 1) {
    REDGPU_2_EXPECTWG(gpuInfo->memoryHeaps[i].isGpuVram        == expectedMemoryHeaps[i].isGpuVram);
    REDGPU_2_EXPECTWG(gpuInfo->memoryHeaps[i].memoryBytesCount >= expectedMemoryHeaps[i].memoryBytesCount);
  }
  for (unsigned i = 0; i < gpuInfo->memoryTypesCount; i += 1) {
    REDGPU_2_EXPECTWG(gpuInfo->memoryTypes[i].memoryHeapIndex  == expectedMemoryTypes[i].memoryHeapIndex);
    REDGPU_2_EXPECTWG(gpuInfo->memoryTypes[i].isGpuVram        == expectedMemoryTypes[i].isGpuVram);
    REDGPU_2_EXPECTWG(gpuInfo->memoryTypes[i].isCpuMappable    == expectedMemoryTypes[i].isCpuMappable);
    REDGPU_2_EXPECTWG(gpuInfo->memoryTypes[i].isCpuCoherent    == expectedMemoryTypes[i].isCpuCoherent);
    REDGPU_2_EXPECTWG(gpuInfo->memoryTypes[i].isCpuCached      == expectedMemoryTypes[i].isCpuCached);
  }
}

static void red2InternalFillMemoryTypeIsSupportedArray(unsigned resourceMemoryTypesSupported, unsigned char * outMemoryTypeIsSupportedArrayOf32) {
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

static void red2InternalStatusesCopyFromTo(const RedStatuses * from, RedStatuses * to) {
  if (to == NULL) { return; }

  if (from->status != RED_STATUS_SUCCESS) {
    if (to->status == RED_STATUS_SUCCESS) { // NOTE(Constantine): REDGPU keeps the first non-successes only.
      to->status            = from->status;
      to->statusCode        = from->statusCode;
      to->statusHresult     = from->statusHresult;
      to->statusProcedureId = from->statusProcedureId;
      to->statusFile        = from->statusFile;
      to->statusLine        = from->statusLine;
    }
  }

  if (from->statusError != RED_STATUS_SUCCESS) {
    if (to->statusError == RED_STATUS_SUCCESS) { // NOTE(Constantine): REDGPU keeps the first errors only.
      to->statusError            = from->statusError;
      to->statusErrorCode        = from->statusErrorCode;
      to->statusErrorHresult     = from->statusErrorHresult;
      to->statusErrorProcedureId = from->statusErrorProcedureId;
      to->statusErrorFile        = from->statusErrorFile;
      to->statusErrorLine        = from->statusErrorLine;
      for (int i = 0; i < 512; i += 1) {
        to->statusErrorDescription[i] = from->statusErrorDescription[i];
      }
    }
  }
}

REDGPU_2_DECLSPEC void REDGPU_2_API red2CreateArray(RedContext context, RedHandleGpu gpu, const char * handleName, RedArrayType type, uint64_t bytesCount, uint64_t structuredBufferElementBytesCount, RedAccessBitflags restrictToAccess, unsigned initialQueueFamilyIndex, uint64_t maxAllowedOverallocationBytesCount, RedBool32 dedicate, RedBool32 mappable, unsigned dedicateOrMappableMemoryTypeIndex, unsigned dedicateOrMappableMemoryBitflags, unsigned suballocateFromMemoryOnFirstMatchPointersCount, Red2Memory ** suballocateFromMemoryOnFirstMatchPointers, Red2Array * outArray, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData) {
  unsigned gpuIndex = red2InternalGetGpuIndex(context, gpu);
  REDGPU_2_EXPECTWG(gpuIndex != (unsigned)-1);
  
  RedArray array = {0};
  np(redCreateArray,
    "context", context,
    "gpu", gpu,
    "handleName", handleName,
    "type", type,
    "bytesCount", bytesCount,
    "structuredBufferElementBytesCount", structuredBufferElementBytesCount,
    "restrictToAccess", restrictToAccess,
    "initialQueueFamilyIndex", initialQueueFamilyIndex,
    "dedicate", dedicate,
    "outArray", &array,
    "outStatuses", outStatuses,
    "optionalFile", optionalFile,
    "optionalLine", optionalLine,
    "optionalUserData", optionalUserData
  );
  
  if (array.handle == NULL) {
    // Filling
    Red2Array;
    outArray->array = array;
    outArray->handleAllocatedDedicatedOrMappableMemoryOrPickedMemory = NULL;
    return;
  }

  {
    size_t bytesToNextAlignmentBoundary = REDGPU_2_BYTES_TO_NEXT_ALIGNMENT_BOUNDARY(array.memoryBytesCount, array.memoryBytesAlignment);
    REDGPU_2_EXPECTWG(bytesToNextAlignmentBoundary == 0);
    REDGPU_2_EXPECTWG((array.memoryBytesCount - bytesCount) <= maxAllowedOverallocationBytesCount);
  }

  Red2Memory * pickedMemory            = NULL;
  Red2Memory   stackmemForPickedMemory = {0};

  unsigned char arrayMemoryTypeIsSupported[32] = {0};
  red2InternalFillMemoryTypeIsSupportedArray(array.memoryTypesSupported, arrayMemoryTypeIsSupported);

  if (dedicate == 1 || mappable == 1) {
    // NOTE(Constantine):
    // dedicate or mappable arrays use dedicateOrMappableMemoryTypeIndex
    // to allocate their own dedicated memory; suballocateFromMemoryOnFirstMatchPointers array is ignored.
    // This is done so because it's not clear for the user how much space he/she should allocate for the array ahead of time.

    REDGPU_2_EXPECTWG(arrayMemoryTypeIsSupported[dedicateOrMappableMemoryTypeIndex] == 1);

    RedHandleMemory memory = NULL;
    if (mappable == 1) {
      np(redMemoryAllocateMappable,
        "context", context,
        "gpu", gpu,
        "handleName", handleName,
        "dedicate", dedicate,
        "array", array.handle,
        "arrayMemoryBytesCount", array.memoryBytesCount,
        "memoryTypeIndex", dedicateOrMappableMemoryTypeIndex,
        "memoryBitflags", dedicateOrMappableMemoryBitflags,
        "outMemory", &memory,
        "outStatuses", outStatuses,
        "optionalFile", optionalFile,
        "optionalLine", optionalLine,
        "optionalUserData", optionalUserData
      );
    } else {
      np(redMemoryAllocate,
        "context", context,
        "gpu", gpu,
        "handleName", handleName,
        "bytesCount", array.memoryBytesCount,
        "memoryTypeIndex", dedicateOrMappableMemoryTypeIndex,
        "dedicateToArray", array.handle,
        "dedicateToImage", NULL,
        "memoryBitflags", dedicateOrMappableMemoryBitflags,
        "outMemory", &memory,
        "outStatuses", outStatuses,
        "optionalFile", optionalFile,
        "optionalLine", optionalLine,
        "optionalUserData", optionalUserData
      );
    }

    // Filling
    Red2Memory;
    stackmemForPickedMemory.handle                   = memory;
    stackmemForPickedMemory.currentSuballocateOffset = 0;
    stackmemForPickedMemory.bytesCount               = array.memoryBytesCount;
    stackmemForPickedMemory.gpuIndex                 = gpuIndex;
    stackmemForPickedMemory.memoryTypeIndex          = dedicateOrMappableMemoryTypeIndex;

    pickedMemory = &stackmemForPickedMemory;
  } else {
    unsigned suballocateFromMemoryOnFirstMatchPointersIndex = (unsigned)-1;
    {
      // NOTE(Constantine):
      // Currently using only the first matching memory.
      RedBool32 suballocateFromMemoryTypeIndexFound = 0;
      unsigned i = 0;
      for (; i < suballocateFromMemoryOnFirstMatchPointersCount; i += 1) {
        unsigned tryMemoryTypeIndex = suballocateFromMemoryOnFirstMatchPointers[i]->memoryTypeIndex; // NOTE(Constantine): Assumes max possible memoryTypeIndex value of 31.
        if (arrayMemoryTypeIsSupported[tryMemoryTypeIndex] == 1) {
          suballocateFromMemoryTypeIndexFound = 1;
          break;
        }
      }
      REDGPU_2_EXPECTWG(suballocateFromMemoryTypeIndexFound == 1);
      suballocateFromMemoryOnFirstMatchPointersIndex = i;
    }

    pickedMemory = suballocateFromMemoryOnFirstMatchPointers[suballocateFromMemoryOnFirstMatchPointersIndex];
  }

  uint64_t pickedMemoryBytesFirst = pickedMemory->currentSuballocateOffset;
  uint64_t pickedMemoryBytesToNextAlignmentBoundary = REDGPU_2_BYTES_TO_NEXT_ALIGNMENT_BOUNDARY(pickedMemoryBytesFirst, array.memoryBytesAlignment);
  pickedMemoryBytesFirst += pickedMemoryBytesToNextAlignmentBoundary;

  REDGPU_2_EXPECTWG((pickedMemoryBytesFirst + array.memoryBytesCount) <= pickedMemory->bytesCount);

  if (array.handle != NULL && pickedMemory->handle != NULL) {
    RedMemoryArray memoryArray = {0};
    memoryArray.setTo1000157000  = 1000157000;
    memoryArray.setTo0           = 0;
    memoryArray.array            = array.handle;
    memoryArray.memory           = pickedMemory->handle;
    memoryArray.memoryBytesFirst = pickedMemoryBytesFirst;
    RedStatuses opstatuses = {0};
    np(redMemorySet,
      "context", context,
      "gpu", gpu,
      "memoryArraysCount", 1,
      "memoryArrays", &memoryArray,
      "memoryImagesCount", 0,
      "memoryImages", NULL,
      "outStatuses", &opstatuses,
      "optionalFile", optionalFile,
      "optionalLine", optionalLine,
      "optionalUserData", optionalUserData
    );
    red2InternalStatusesCopyFromTo(&opstatuses, outStatuses);

    if (opstatuses.statusError == RED_STATUS_SUCCESS) {
      // NOTE(Constantine):
      // Be aware doing this is not thread-safe for the picked memory,
      // and since it is not known in advance which memory will be picked,
      // effectively it becomes not thread-safe for all the passed memory handles.
      pickedMemory->currentSuballocateOffset += pickedMemoryBytesToNextAlignmentBoundary + array.memoryBytesCount;
    }
  }

  // Filling
  Red2Array;
  outArray->array = array;
  outArray->handleAllocatedDedicatedOrMappableMemoryOrPickedMemory = pickedMemory->handle;
}

REDGPU_2_DECLSPEC void REDGPU_2_API red2CreateImage(RedContext context, RedHandleGpu gpu, const char * handleName, RedImageDimensions dimensions, RedFormat format, unsigned width, unsigned height, unsigned depth, unsigned levelsCount, unsigned layersCount, RedMultisampleCountBitflag multisampleCount, RedAccessBitflags restrictToAccess, unsigned initialQueueFamilyIndex, RedBool32 dedicate, unsigned dedicateMemoryTypeIndex, unsigned dedicateMemoryBitflags, unsigned suballocateFromMemoryOnFirstMatchPointersCount, Red2Memory ** suballocateFromMemoryOnFirstMatchPointers, Red2Image * outImage, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData) {
  unsigned gpuIndex = red2InternalGetGpuIndex(context, gpu);
  REDGPU_2_EXPECTWG(gpuIndex != (unsigned)-1);

  RedImage image = {0};
  np(redCreateImage,
    "context", context,
    "gpu", gpu,
    "handleName", handleName,
    "dimensions", dimensions,
    "format", format,
    "width", width,
    "height", height,
    "depth", depth,
    "levelsCount", levelsCount,
    "layersCount", layersCount,
    "multisampleCount", multisampleCount,
    "restrictToAccess", restrictToAccess,
    "initialQueueFamilyIndex", initialQueueFamilyIndex,
    "dedicate", dedicate,
    "outImage", &image,
    "outStatuses", outStatuses,
    "optionalFile", optionalFile,
    "optionalLine", optionalLine,
    "optionalUserData", optionalUserData
  );

  if (image.handle == NULL) {
    // Filling
    Red2Image;
    outImage->image = image;
    outImage->handleAllocatedDedicatedMemoryOrPickedMemory = NULL;
    return;
  }

  if (0) {
    // NOTE(Constantine):
    // Sep 06, 2025: RTX 2060, NVIDIA driver on Windows 10 returns image.memoryBytesCount not aligned by image.memoryBytesAlignment.
    size_t bytesToNextAlignmentBoundary = REDGPU_2_BYTES_TO_NEXT_ALIGNMENT_BOUNDARY(image.memoryBytesCount, image.memoryBytesAlignment);
    REDGPU_2_EXPECTWG(bytesToNextAlignmentBoundary == 0);
  }

  Red2Memory * pickedMemory            = NULL;
  Red2Memory   stackmemForPickedMemory = {0};

  unsigned char imageMemoryTypeIsSupported[32] = {0};
  red2InternalFillMemoryTypeIsSupportedArray(image.memoryTypesSupported, imageMemoryTypeIsSupported);

  if (dedicate == 1) {
    // NOTE(Constantine):
    // dedicate images use dedicateMemoryTypeIndex
    // to allocate their own dedicated memory; suballocateFromMemoryOnFirstMatchPointers array is ignored.
    // This is done so because it's not clear for the user how much space he/she should allocate for the image ahead of time.

    REDGPU_2_EXPECTWG(imageMemoryTypeIsSupported[dedicateMemoryTypeIndex] == 1);

    RedHandleMemory memory = NULL;
    np(redMemoryAllocate,
      "context", context,
      "gpu", gpu,
      "handleName", handleName,
      "bytesCount", image.memoryBytesCount,
      "memoryTypeIndex", dedicateMemoryTypeIndex,
      "dedicateToArray", NULL,
      "dedicateToImage", image.handle,
      "memoryBitflags", dedicateMemoryBitflags,
      "outMemory", &memory,
      "outStatuses", outStatuses,
      "optionalFile", optionalFile,
      "optionalLine", optionalLine,
      "optionalUserData", optionalUserData
    );

    // Filling
    Red2Memory;
    stackmemForPickedMemory.handle                   = memory;
    stackmemForPickedMemory.currentSuballocateOffset = 0;
    stackmemForPickedMemory.bytesCount               = image.memoryBytesCount;
    stackmemForPickedMemory.gpuIndex                 = gpuIndex;
    stackmemForPickedMemory.memoryTypeIndex          = dedicateMemoryTypeIndex;

    pickedMemory = &stackmemForPickedMemory;
  } else {
    unsigned suballocateFromMemoryOnFirstMatchPointersIndex = (unsigned)-1;
    {
      // NOTE(Constantine):
      // Currently using only the first matching memory.
      RedBool32 suballocateFromMemoryTypeIndexFound = 0;
      unsigned i = 0;
      for (; i < suballocateFromMemoryOnFirstMatchPointersCount; i += 1) {
        unsigned tryMemoryTypeIndex = suballocateFromMemoryOnFirstMatchPointers[i]->memoryTypeIndex; // NOTE(Constantine): Assumes max possible memoryTypeIndex value of 31.
        if (imageMemoryTypeIsSupported[tryMemoryTypeIndex] == 1) {
          suballocateFromMemoryTypeIndexFound = 1;
          break;
        }
      }
      REDGPU_2_EXPECTWG(suballocateFromMemoryTypeIndexFound == 1);
      suballocateFromMemoryOnFirstMatchPointersIndex = i;
    }

    pickedMemory = suballocateFromMemoryOnFirstMatchPointers[suballocateFromMemoryOnFirstMatchPointersIndex];
  }

  uint64_t pickedMemoryBytesFirst = pickedMemory->currentSuballocateOffset;
  uint64_t pickedMemoryBytesToNextAlignmentBoundary = REDGPU_2_BYTES_TO_NEXT_ALIGNMENT_BOUNDARY(pickedMemoryBytesFirst, image.memoryBytesAlignment);
  pickedMemoryBytesFirst += pickedMemoryBytesToNextAlignmentBoundary;

  REDGPU_2_EXPECTWG((pickedMemoryBytesFirst + image.memoryBytesCount) <= pickedMemory->bytesCount);

  if (image.handle != NULL && pickedMemory->handle != NULL) {
    RedMemoryImage memoryImage = {0};
    memoryImage.setTo1000157001  = 1000157001;
    memoryImage.setTo0           = 0;
    memoryImage.image            = image.handle;
    memoryImage.memory           = pickedMemory->handle;
    memoryImage.memoryBytesFirst = pickedMemoryBytesFirst;
    RedStatuses opstatuses = {0};
    np(redMemorySet,
      "context", context,
      "gpu", gpu,
      "memoryArraysCount", 0,
      "memoryArrays", NULL,
      "memoryImagesCount", 1,
      "memoryImages", &memoryImage,
      "outStatuses", &opstatuses,
      "optionalFile", optionalFile,
      "optionalLine", optionalLine,
      "optionalUserData", optionalUserData
    );
    red2InternalStatusesCopyFromTo(&opstatuses, outStatuses);

    if (opstatuses.statusError == RED_STATUS_SUCCESS) {
      // NOTE(Constantine):
      // Be aware doing this is not thread-safe for the picked memory,
      // and since it is not known in advance which memory will be picked,
      // effectively it becomes not thread-safe for all the passed memory handles.
      pickedMemory->currentSuballocateOffset += pickedMemoryBytesToNextAlignmentBoundary + image.memoryBytesCount;
    }
  }

  // Filling
  Red2Image;
  outImage->image = image;
  outImage->handleAllocatedDedicatedMemoryOrPickedMemory = pickedMemory->handle;
}

REDGPU_2_DECLSPEC void REDGPU_2_API red2CreateProcedureParameters(RedContext context, RedHandleGpu gpu, const char * handleName, const Red2ProcedureParametersDeclaration * procedureParametersDeclaration, Red2ProcedureParametersAndDeclarations * outProcedureParametersAndDeclarations, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData) {
  RedHandleStructDeclaration structsDeclarations[7] = {0};
  RedHandleStructDeclaration handlesDeclaration     = NULL;

  if (procedureParametersDeclaration != NULL) {
    for (unsigned i = 0; i < procedureParametersDeclaration->structsDeclarationsCount; i += 1) {
      np(redCreateStructDeclaration,
        "context", context,
        "gpu", gpu,
        "handleName", handleName,
        "structDeclarationMembersCount",        procedureParametersDeclaration->structsDeclarations[i].structDeclarationMembersCount,
        "structDeclarationMembers",             procedureParametersDeclaration->structsDeclarations[i].structDeclarationMembers,
        "structDeclarationMembersArrayROCount", procedureParametersDeclaration->structsDeclarations[i].structDeclarationMembersArrayROCount,
        "structDeclarationMembersArrayRO",      procedureParametersDeclaration->structsDeclarations[i].structDeclarationMembersArrayRO,
        "procedureParametersHandlesDeclaration", 0,
        "outStructDeclaration", &structsDeclarations[i],
        "outStatuses", outStatuses,
        "optionalFile", optionalFile,
        "optionalLine", optionalLine,
        "optionalUserData", optionalUserData
      );
    }
    if (procedureParametersDeclaration->handlesDeclaration != NULL) {
      np(redCreateStructDeclaration,
        "context", context,
        "gpu", gpu,
        "handleName", handleName,
        "structDeclarationMembersCount",        procedureParametersDeclaration->handlesDeclaration->structDeclarationMembersCount,
        "structDeclarationMembers",             procedureParametersDeclaration->handlesDeclaration->structDeclarationMembers,
        "structDeclarationMembersArrayROCount", procedureParametersDeclaration->handlesDeclaration->structDeclarationMembersArrayROCount,
        "structDeclarationMembersArrayRO",      procedureParametersDeclaration->handlesDeclaration->structDeclarationMembersArrayRO,
        "procedureParametersHandlesDeclaration", 1,
        "outStructDeclaration", &handlesDeclaration,
        "outStatuses", outStatuses,
        "optionalFile", optionalFile,
        "optionalLine", optionalLine,
        "optionalUserData", optionalUserData
      );
    }
  }

  RedProcedureParametersDeclaration parametersDeclaration = {0};
  if (procedureParametersDeclaration != NULL) {
    parametersDeclaration.variablesSlot            = procedureParametersDeclaration->variablesSlot;
    parametersDeclaration.variablesVisibleToStages = procedureParametersDeclaration->variablesVisibleToStages;
    parametersDeclaration.variablesBytesCount      = procedureParametersDeclaration->variablesBytesCount;
    parametersDeclaration.structsDeclarationsCount = procedureParametersDeclaration->structsDeclarationsCount;
    parametersDeclaration.structsDeclarations[0]   = structsDeclarations[0];
    parametersDeclaration.structsDeclarations[1]   = structsDeclarations[1];
    parametersDeclaration.structsDeclarations[2]   = structsDeclarations[2];
    parametersDeclaration.structsDeclarations[3]   = structsDeclarations[3];
    parametersDeclaration.structsDeclarations[4]   = structsDeclarations[4];
    parametersDeclaration.structsDeclarations[5]   = structsDeclarations[5];
    parametersDeclaration.structsDeclarations[6]   = structsDeclarations[6];
    parametersDeclaration.handlesDeclaration       = handlesDeclaration;
  }

  RedHandleProcedureParameters procedureParameters = NULL;
  np(redCreateProcedureParameters,
    "context", context,
    "gpu", gpu,
    "handleName", handleName,
    "procedureParametersDeclaration", procedureParametersDeclaration == NULL ? NULL : &parametersDeclaration,
    "outProcedureParameters", &procedureParameters,
    "outStatuses", outStatuses,
    "optionalFile", optionalFile,
    "optionalLine", optionalLine,
    "optionalUserData", optionalUserData
  );

  outProcedureParametersAndDeclarations->procedureParameters = procedureParameters;
  for (int i = 0; i < 7; i += 1) {
    outProcedureParametersAndDeclarations->structsDeclarations[i] = structsDeclarations[i];
  }
  outProcedureParametersAndDeclarations->handlesDeclaration = handlesDeclaration;
}

REDGPU_2_DECLSPEC void REDGPU_2_API red2CreateProcedure(RedContext context, RedHandleGpu gpu, const char * handleName, RedHandleProcedureCache procedureCache, const RedOutputDeclarationMembers * outputDeclarationMembers, const RedOutputDeclarationMembersResolveSources * outputDeclarationMembersResolveSources, RedBool32 dependencyByRegion, RedBool32 dependencyByRegionAllowUsageAliasOrderBarriers, RedHandleProcedureParameters procedureParameters, const char * gpuCodeVertexMainProcedureName, RedHandleGpuCode gpuCodeVertex, const char * gpuCodeFragmentMainProcedureName, RedHandleGpuCode gpuCodeFragment, const RedProcedureState * state, const void * stateExtension, RedBool32 deriveBase, RedHandleProcedure deriveFrom, RedHandleProcedure * outProcedure, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData) {
  RedHandleOutputDeclaration outputDeclaration = NULL;
  np(redCreateOutputDeclaration,
    "context", context,
    "gpu", gpu,
    "handleName", handleName,
    "outputDeclarationMembers", outputDeclarationMembers,
    "outputDeclarationMembersResolveSources", outputDeclarationMembersResolveSources,
    "dependencyByRegion", dependencyByRegion,
    "dependencyByRegionAllowUsageAliasOrderBarriers", dependencyByRegionAllowUsageAliasOrderBarriers,
    "outOutputDeclaration", &outputDeclaration,
    "outStatuses", outStatuses,
    "optionalFile", optionalFile,
    "optionalLine", optionalLine,
    "optionalUserData", optionalUserData
  );
  np(redCreateProcedure,
    "context", context,
    "gpu", gpu,
    "handleName", handleName,
    "procedureCache", procedureCache,
    "outputDeclaration", outputDeclaration,
    "procedureParameters", procedureParameters,
    "gpuCodeVertexMainProcedureName", gpuCodeVertexMainProcedureName,
    "gpuCodeVertex", gpuCodeVertex,
    "gpuCodeFragmentMainProcedureName", gpuCodeFragmentMainProcedureName,
    "gpuCodeFragment", gpuCodeFragment,
    "state", state,
    "stateExtension", stateExtension,
    "deriveBase", deriveBase,
    "deriveFrom", deriveFrom,
    "outProcedure", outProcedure,
    "outStatuses", outStatuses,
    "optionalFile", optionalFile,
    "optionalLine", optionalLine,
    "optionalUserData", optionalUserData
  );
  red2DestroyHandle(context, gpu, RED_HANDLE_TYPE_OUTPUT_DECLARATION, outputDeclaration, NULL, optionalFile, optionalLine, optionalUserData);
}

REDGPU_2_DECLSPEC void REDGPU_2_API red2DestroyHandle(RedContext context, RedHandleGpu gpu, RedHandleType handleType, void * handle, void * optionalHandle2, const char * optionalFile, int optionalLine, void * optionalUserData) {
  if (handle == NULL) { return; }
  // Filling
  RedHandleType;
  //if      (handleType == RED_HANDLE_TYPE_CONTEXT)              {}
  //else if (handleType == RED_HANDLE_TYPE_GPU)                  {}
  //else if (handleType == RED_HANDLE_TYPE_GPU_DEVICE)           {}
  //else if (handleType == RED_HANDLE_TYPE_QUEUE)                {}
  if      (handleType == RED_HANDLE_TYPE_MEMORY)               { redMemoryFree(context, gpu, (RedHandleMemory)handle, optionalFile, optionalLine, optionalUserData); }
  else if (handleType == RED_HANDLE_TYPE_ARRAY)                { redDestroyArray(context, gpu, (RedHandleArray)handle, optionalFile, optionalLine, optionalUserData); }
  else if (handleType == RED_HANDLE_TYPE_IMAGE)                { redDestroyImage(context, gpu, (RedHandleImage)handle, optionalFile, optionalLine, optionalUserData); }
  else if (handleType == RED_HANDLE_TYPE_SAMPLER)              { redDestroySampler(context, gpu, (RedHandleSampler)handle, optionalFile, optionalLine, optionalUserData); }
  else if (handleType == RED_HANDLE_TYPE_TEXTURE)              { redDestroyTexture(context, gpu, (RedHandleTexture)handle, optionalFile, optionalLine, optionalUserData); }
  else if (handleType == RED_HANDLE_TYPE_GPU_CODE)             { redDestroyGpuCode(context, gpu, (RedHandleGpuCode)handle, optionalFile, optionalLine, optionalUserData); }
  else if (handleType == RED_HANDLE_TYPE_OUTPUT_DECLARATION)   { redDestroyOutputDeclaration(context, gpu, (RedHandleOutputDeclaration)handle, optionalFile, optionalLine, optionalUserData); }
  else if (handleType == RED_HANDLE_TYPE_STRUCT_DECLARATION)   { redDestroyStructDeclaration(context, gpu, (RedHandleStructDeclaration)handle, optionalFile, optionalLine, optionalUserData); }
  else if (handleType == RED_HANDLE_TYPE_PROCEDURE_PARAMETERS) { redDestroyProcedureParameters(context, gpu, (RedHandleProcedureParameters)handle, optionalFile, optionalLine, optionalUserData); }
  else if (handleType == RED_HANDLE_TYPE_PROCEDURE_CACHE)      { redDestroyProcedureCache(context, gpu, (RedHandleProcedureCache)handle, optionalFile, optionalLine, optionalUserData); }
  else if (handleType == RED_HANDLE_TYPE_PROCEDURE)            { redDestroyProcedure(context, gpu, (RedHandleProcedure)handle, optionalFile, optionalLine, optionalUserData); }
  else if (handleType == RED_HANDLE_TYPE_OUTPUT)               { redDestroyOutput(context, gpu, (RedHandleOutput)handle, optionalFile, optionalLine, optionalUserData); }
  //else if (handleType == RED_HANDLE_TYPE_STRUCT)               {}
  else if (handleType == RED_HANDLE_TYPE_STRUCTS_MEMORY)       { redStructsMemoryFree(context, gpu, (RedHandleStructsMemory)handle, optionalFile, optionalLine, optionalUserData); }
  else if (handleType == RED_HANDLE_TYPE_CALLS)                { redDestroyCalls(context, gpu, (RedHandleCalls)handle, (RedHandleCallsMemory)optionalHandle2, optionalFile, optionalLine, optionalUserData); }
  //else if (handleType == RED_HANDLE_TYPE_CALLS_MEMORY)         {}
  else if (handleType == RED_HANDLE_TYPE_CPU_SIGNAL)           { redDestroyCpuSignal(context, gpu, (RedHandleCpuSignal)handle, optionalFile, optionalLine, optionalUserData); }
  else if (handleType == RED_HANDLE_TYPE_GPU_SIGNAL)           { redDestroyGpuSignal(context, gpu, (RedHandleGpuSignal)handle, optionalFile, optionalLine, optionalUserData); }
  else if (handleType == RED_HANDLE_TYPE_GPU_TO_CPU_SIGNAL)    { redDestroyGpuToCpuSignal(context, gpu, (RedHandleGpuToCpuSignal)handle, optionalFile, optionalLine, optionalUserData); }
  else if (handleType == RED_HANDLE_TYPE_SURFACE)              { redDestroySurface(context, gpu, (RedHandleSurface)handle, optionalFile, optionalLine, optionalUserData); }
  else if (handleType == RED_HANDLE_TYPE_PRESENT)              { redDestroyPresent(context, gpu, (RedHandlePresent)handle, optionalFile, optionalLine, optionalUserData); }
  else {
    RedBool32 handle_type_to_destroy_is_not_found = 1;
    REDGPU_2_EXPECTWG(handle_type_to_destroy_is_not_found == 0);
  }
}

REDGPU_2_DECLSPEC void REDGPU_2_API red2CallsSet(RedContext context, RedHandleGpu gpu, RedHandleCalls calls, RedHandleCallsMemory callsMemory, RedBool32 callsReusable, Red2CallsMutableOutputsArray * mutableOutputsArray, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData) {
  np(redCallsSet,
    "context", context,
    "gpu", gpu,
    "calls", calls,
    "callsMemory", callsMemory,
    "callsReusable", callsReusable,
    "outStatuses", outStatuses,
    "optionalFile", optionalFile,
    "optionalLine", optionalLine,
    "optionalUserData", optionalUserData
  );
  unsigned outputsCount = mutableOutputsArray->count;
  for (unsigned i = 0; i < outputsCount; i += 1) {
    red2DestroyHandle(context, gpu, RED_HANDLE_TYPE_OUTPUT, mutableOutputsArray->items[i].handle, NULL, optionalFile, optionalLine, optionalUserData);
  }
  for (unsigned i = 0; i < outputsCount; i += 1) {
    red2DestroyHandle(context, gpu, RED_HANDLE_TYPE_OUTPUT_DECLARATION, mutableOutputsArray->items[i].handleDeclaration, NULL, optionalFile, optionalLine, optionalUserData);
  }
  for (unsigned i = 0; i < outputsCount; i += 1) {
    // Filling
    Red2Output;
    mutableOutputsArray->items[i].handle = NULL;
    mutableOutputsArray->items[i].handleDeclaration = NULL;
  }
  mutableOutputsArray->count = 0;
}

REDGPU_2_DECLSPEC void REDGPU_2_API red2CallSetProcedureOutput(RedTypeProcedureAddressCallSetProcedureOutput address, RedHandleCalls calls, RedContext context, RedHandleGpu gpu, Red2CallsMutableOutputsArray * mutableOutputsArray, const RedOutputDeclarationMembers * outputDeclarationMembers, const RedOutputDeclarationMembersResolveSources * outputDeclarationMembersResolveSources, RedBool32 dependencyByRegion, RedBool32 dependencyByRegionAllowUsageAliasOrderBarriers, const RedOutputMembers * outputMembers, const RedOutputMembersResolveTargets * outputMembersResolveTargets, unsigned width, unsigned height, float depthClearValue, unsigned stencilClearValue, const RedColorsClearValuesFloat * colorsClearValuesFloat, const RedColorsClearValuesSint * colorsClearValuesSint, const RedColorsClearValuesUint * colorsClearValuesUint, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData) {
  REDGPU_2_EXPECTWG((mutableOutputsArray->count + 1) <= mutableOutputsArray->capacity);
  
  RedHandleOutputDeclaration outputDeclaration = NULL;
  np(redCreateOutputDeclaration,
    "context", context,
    "gpu", gpu,
    "handleName", NULL,
    "outputDeclarationMembers", outputDeclarationMembers,
    "outputDeclarationMembersResolveSources", outputDeclarationMembersResolveSources,
    "dependencyByRegion", dependencyByRegion,
    "dependencyByRegionAllowUsageAliasOrderBarriers", dependencyByRegionAllowUsageAliasOrderBarriers,
    "outOutputDeclaration", &outputDeclaration,
    "outStatuses", outStatuses,
    "optionalFile", optionalFile,
    "optionalLine", optionalLine,
    "optionalUserData", optionalUserData
  );

  RedOutput output = {0};
  np(redCreateOutput,
    "context", context,
    "gpu", gpu,
    "handleName", NULL,
    "outputDeclaration", outputDeclaration,
    "outputMembers", outputMembers,
    "outputMembersResolveTargets", outputMembersResolveTargets,
    "width", width,
    "height", height,
    "outOutput", &output,
    "outStatuses", outStatuses,
    "optionalFile", optionalFile,
    "optionalLine", optionalLine,
    "optionalUserData", optionalUserData
  );

  np(redCallSetProcedureOutput,
    "address", address,
    "calls", calls,
    "outputDeclaration", outputDeclaration,
    "output", output.handle,
    "inlineOutput", NULL,
    "outputWidth", output.width,
    "outputHeight", output.height,
    "outputDepthStencilEnable", output.depthStencilEnable,
    "outputColorsCount", output.colorsCount,
    "depthClearValue", depthClearValue,
    "stencilClearValue", stencilClearValue,
    "colorsClearValuesFloat", colorsClearValuesFloat,
    "colorsClearValuesSint", colorsClearValuesSint,
    "colorsClearValuesUint", colorsClearValuesUint
  );

  {
    uint64_t currentOutputsArrayIndex = mutableOutputsArray->count;

    // Filling
    Red2Output;
    mutableOutputsArray->items[currentOutputsArrayIndex].handle            = output.handle;
    mutableOutputsArray->items[currentOutputsArrayIndex].handleDeclaration = outputDeclaration;

    mutableOutputsArray->count = currentOutputsArrayIndex + 1;
  }
}

REDGPU_2_DECLSPEC void REDGPU_2_API red2CallGlobalOrderBarrier(RedTypeProcedureAddressCallUsageAliasOrderBarrier address, RedHandleCalls calls) {
  VkMemoryBarrier globalBarrier = {0};
  globalBarrier.sType         = VK_STRUCTURE_TYPE_MEMORY_BARRIER;
  globalBarrier.pNext         = NULL;
  globalBarrier.srcAccessMask = VK_ACCESS_MEMORY_READ_BIT | VK_ACCESS_MEMORY_WRITE_BIT;
  globalBarrier.dstAccessMask = VK_ACCESS_MEMORY_READ_BIT | VK_ACCESS_MEMORY_WRITE_BIT;
  ((PFN_vkCmdPipelineBarrier)((void *)address))((VkCommandBuffer)calls, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, 0, 1, &globalBarrier, 0, NULL, 0, NULL);
}

REDGPU_2_DECLSPEC void REDGPU_2_API red2CallGlobalReadbackBarrier(RedTypeProcedureAddressCallUsageAliasOrderBarrier address, RedHandleCalls calls) {
  VkMemoryBarrier globalBarrier = {0};
  globalBarrier.sType         = VK_STRUCTURE_TYPE_MEMORY_BARRIER;
  globalBarrier.pNext         = NULL;
  globalBarrier.srcAccessMask = VK_ACCESS_MEMORY_READ_BIT | VK_ACCESS_MEMORY_WRITE_BIT;
  globalBarrier.dstAccessMask = VK_ACCESS_HOST_READ_BIT   | VK_ACCESS_HOST_WRITE_BIT;
  ((PFN_vkCmdPipelineBarrier)((void *)address))((VkCommandBuffer)calls, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, VK_PIPELINE_STAGE_HOST_BIT, 0, 1, &globalBarrier, 0, NULL, 0, NULL);
}

// red2CallUsageAliasOrderBarrier

typedef unsigned RedVkAccessStageBitflags;
typedef enum RedVkAccessStageBitflag {
  RED_VK_ACCESS_STAGE_BITFLAG_COPY                 = REDGPU_B32(0000,0000,0000,0000,0001,0000,0000,0000),
  RED_VK_ACCESS_STAGE_BITFLAG_COMPUTE              = REDGPU_B32(0000,0000,0000,0000,0000,1000,0000,0000),
  RED_VK_ACCESS_STAGE_BITFLAG_INDEX                = REDGPU_B32(0000,0000,0000,0000,0000,0000,0000,0100),
  RED_VK_ACCESS_STAGE_BITFLAG_VERTEX               = REDGPU_B32(0000,0000,0000,0000,0000,0000,0000,1000),
  RED_VK_ACCESS_STAGE_BITFLAG_FRAGMENT             = REDGPU_B32(0000,0000,0000,0000,0000,0000,1000,0000),
  RED_VK_ACCESS_STAGE_BITFLAG_OUTPUT_DEPTH_STENCIL = REDGPU_B32(0000,0000,0000,0000,0000,0011,0000,0000),
  RED_VK_ACCESS_STAGE_BITFLAG_OUTPUT_COLOR         = REDGPU_B32(0000,0000,0000,0000,0000,0100,0000,0000),
  RED_VK_ACCESS_STAGE_BITFLAG_RESOLVE              = REDGPU_B32(0000,0000,0000,0000,0000,0100,0000,0000),
  RED_VK_ACCESS_STAGE_BITFLAG_CPU                  = REDGPU_B32(0000,0000,0000,0000,0100,0000,0000,0000),
} RedVkAccessStageBitflag;

static inline RedVkAccessStageBitflags red2InternalRedAccessStagesToRedVkAccessStages(RedAccessStageBitflags flags) {
  RedVkAccessStageBitflags out = 0;
  if ((flags & RED_ACCESS_STAGE_BITFLAG_COPY)                 == RED_ACCESS_STAGE_BITFLAG_COPY)                 { out |= RED_VK_ACCESS_STAGE_BITFLAG_COPY;                 }
  if ((flags & RED_ACCESS_STAGE_BITFLAG_COMPUTE)              == RED_ACCESS_STAGE_BITFLAG_COMPUTE)              { out |= RED_VK_ACCESS_STAGE_BITFLAG_COMPUTE;              }
  if ((flags & RED_ACCESS_STAGE_BITFLAG_INDEX)                == RED_ACCESS_STAGE_BITFLAG_INDEX)                { out |= RED_VK_ACCESS_STAGE_BITFLAG_INDEX;                }
  if ((flags & RED_ACCESS_STAGE_BITFLAG_VERTEX)               == RED_ACCESS_STAGE_BITFLAG_VERTEX)               { out |= RED_VK_ACCESS_STAGE_BITFLAG_VERTEX;               }
  if ((flags & RED_ACCESS_STAGE_BITFLAG_FRAGMENT)             == RED_ACCESS_STAGE_BITFLAG_FRAGMENT)             { out |= RED_VK_ACCESS_STAGE_BITFLAG_FRAGMENT;             }
  if ((flags & RED_ACCESS_STAGE_BITFLAG_OUTPUT_DEPTH_STENCIL) == RED_ACCESS_STAGE_BITFLAG_OUTPUT_DEPTH_STENCIL) { out |= RED_VK_ACCESS_STAGE_BITFLAG_OUTPUT_DEPTH_STENCIL; }
  if ((flags & RED_ACCESS_STAGE_BITFLAG_OUTPUT_COLOR)         == RED_ACCESS_STAGE_BITFLAG_OUTPUT_COLOR)         { out |= RED_VK_ACCESS_STAGE_BITFLAG_OUTPUT_COLOR;         }
  if ((flags & RED_ACCESS_STAGE_BITFLAG_RESOLVE)              == RED_ACCESS_STAGE_BITFLAG_RESOLVE)              { out |= RED_VK_ACCESS_STAGE_BITFLAG_RESOLVE;              }
  if ((flags & RED_ACCESS_STAGE_BITFLAG_CPU)                  == RED_ACCESS_STAGE_BITFLAG_CPU)                  { out |= RED_VK_ACCESS_STAGE_BITFLAG_CPU;                  }
  return out;
}

typedef unsigned RedVkAccessBitflags;
typedef enum RedVkAccessBitflag {
  RED_VK_ACCESS_BITFLAG_COPY_R                     = REDGPU_B32(0000,0000,0000,0000,0000,1000,0000,0000),
  RED_VK_ACCESS_BITFLAG_COPY_W                     = REDGPU_B32(0000,0000,0000,0000,0001,0000,0000,0000),
  RED_VK_ACCESS_BITFLAG_INDEX_R                    = REDGPU_B32(0000,0000,0000,0000,0000,0000,0000,0010),
  RED_VK_ACCESS_BITFLAG_STRUCT_ARRAY_RO_CONSTANT_R = REDGPU_B32(0000,0000,0000,0000,0000,0000,0000,1000),
  RED_VK_ACCESS_BITFLAG_STRUCT_RESOURCE_R          = REDGPU_B32(0000,0000,0000,0000,0000,0000,0010,0000),
  RED_VK_ACCESS_BITFLAG_STRUCT_RESOURCE_W          = REDGPU_B32(0000,0000,0000,0000,0000,0000,0100,0000),
  RED_VK_ACCESS_BITFLAG_OUTPUT_DEPTH_STENCIL       = REDGPU_B32(0000,0000,0000,0000,0000,0110,0000,0000),
  RED_VK_ACCESS_BITFLAG_OUTPUT_COLOR               = REDGPU_B32(0000,0000,0000,0000,0000,0001,1000,0000),
  RED_VK_ACCESS_BITFLAG_RESOLVE_SOURCE_R           = REDGPU_B32(0000,0000,0000,0000,0000,0001,1000,0000),
  RED_VK_ACCESS_BITFLAG_RESOLVE_TARGET_W           = REDGPU_B32(0000,0000,0000,0000,0000,0001,0000,0000),
  RED_VK_ACCESS_BITFLAG_CPU                        = REDGPU_B32(0000,0000,0000,0000,0110,0000,0000,0000),
} RedVkAccessBitflag;

static inline RedVkAccessBitflags red2InternalRedAccessToRedVkAccess(RedAccessBitflags flags) {
  RedVkAccessBitflags out = 0;
  if ((flags & RED_ACCESS_BITFLAG_COPY_R)                               == RED_ACCESS_BITFLAG_COPY_R)                               { out |= RED_VK_ACCESS_BITFLAG_COPY_R;                     }
  if ((flags & RED_ACCESS_BITFLAG_COPY_W)                               == RED_ACCESS_BITFLAG_COPY_W)                               { out |= RED_VK_ACCESS_BITFLAG_COPY_W;                     }
  if ((flags & RED_ACCESS_BITFLAG_INDEX_R)                              == RED_ACCESS_BITFLAG_INDEX_R)                              { out |= RED_VK_ACCESS_BITFLAG_INDEX_R;                    }
  if ((flags & RED_ACCESS_BITFLAG_STRUCT_ARRAY_RO_CONSTANT_R)           == RED_ACCESS_BITFLAG_STRUCT_ARRAY_RO_CONSTANT_R)           { out |= RED_VK_ACCESS_BITFLAG_STRUCT_ARRAY_RO_CONSTANT_R; }
  if ((flags & RED_ACCESS_BITFLAG_STRUCT_RESOURCE_NON_FRAGMENT_STAGE_R) == RED_ACCESS_BITFLAG_STRUCT_RESOURCE_NON_FRAGMENT_STAGE_R) { out |= RED_VK_ACCESS_BITFLAG_STRUCT_RESOURCE_R;          }
  if ((flags & RED_ACCESS_BITFLAG_STRUCT_RESOURCE_FRAGMENT_STAGE_R)     == RED_ACCESS_BITFLAG_STRUCT_RESOURCE_FRAGMENT_STAGE_R)     { out |= RED_VK_ACCESS_BITFLAG_STRUCT_RESOURCE_R;          }
  if ((flags & RED_ACCESS_BITFLAG_STRUCT_RESOURCE_W)                    == RED_ACCESS_BITFLAG_STRUCT_RESOURCE_W)                    { out |= RED_VK_ACCESS_BITFLAG_STRUCT_RESOURCE_W;          }
  if ((flags & RED_ACCESS_BITFLAG_OUTPUT_DEPTH_R)                       == RED_ACCESS_BITFLAG_OUTPUT_DEPTH_R)                       { out |= RED_VK_ACCESS_BITFLAG_OUTPUT_DEPTH_STENCIL;       }
  if ((flags & RED_ACCESS_BITFLAG_OUTPUT_DEPTH_RW)                      == RED_ACCESS_BITFLAG_OUTPUT_DEPTH_RW)                      { out |= RED_VK_ACCESS_BITFLAG_OUTPUT_DEPTH_STENCIL;       }
  if ((flags & RED_ACCESS_BITFLAG_OUTPUT_STENCIL_R)                     == RED_ACCESS_BITFLAG_OUTPUT_STENCIL_R)                     { out |= RED_VK_ACCESS_BITFLAG_OUTPUT_DEPTH_STENCIL;       }
  if ((flags & RED_ACCESS_BITFLAG_OUTPUT_STENCIL_RW)                    == RED_ACCESS_BITFLAG_OUTPUT_STENCIL_RW)                    { out |= RED_VK_ACCESS_BITFLAG_OUTPUT_DEPTH_STENCIL;       }
  if ((flags & RED_ACCESS_BITFLAG_OUTPUT_COLOR_W)                       == RED_ACCESS_BITFLAG_OUTPUT_COLOR_W)                       { out |= RED_VK_ACCESS_BITFLAG_OUTPUT_COLOR;               }
  if ((flags & RED_ACCESS_BITFLAG_RESOLVE_SOURCE_R)                     == RED_ACCESS_BITFLAG_RESOLVE_SOURCE_R)                     { out |= RED_VK_ACCESS_BITFLAG_RESOLVE_SOURCE_R;           }
  if ((flags & RED_ACCESS_BITFLAG_RESOLVE_TARGET_W)                     == RED_ACCESS_BITFLAG_RESOLVE_TARGET_W)                     { out |= RED_VK_ACCESS_BITFLAG_RESOLVE_TARGET_W;           }
  if ((flags & RED_ACCESS_BITFLAG_CPU_RW)                               == RED_ACCESS_BITFLAG_CPU_RW)                               { out |= RED_VK_ACCESS_BITFLAG_CPU;                        }
  return out;
}

static inline void red2InlineCallUsageOrderBarrier(RedTypeProcedureAddressCallUsageAliasOrderBarrier address, RedHandleCalls calls, RedVkAccessStageBitflags oldAccessStages, RedVkAccessStageBitflags newAccessStages, unsigned arrayUsagesCount, const Red2UsageArrayTempCallStruct * arrayUsages, unsigned imageUsagesCount, const Red2UsageImageTempCallStruct * imageUsages, RedBool32 dependencyByRegion) {
  if (oldAccessStages == 0) { oldAccessStages = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;    }
  if (newAccessStages == 0) { newAccessStages = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT; }
  ((PFN_vkCmdPipelineBarrier)((void *)address))((VkCommandBuffer)calls, (unsigned)oldAccessStages, (unsigned)newAccessStages, (unsigned)dependencyByRegion, 0, NULL, arrayUsagesCount, (const VkBufferMemoryBarrier *)arrayUsages, imageUsagesCount, (const VkImageMemoryBarrier *)imageUsages);
}

REDGPU_2_DECLSPEC void REDGPU_2_API red2CallUsageAliasOrderBarrier(RedTypeProcedureAddressCallUsageAliasOrderBarrier address, RedHandleCalls calls, RedContext context, unsigned arrayUsagesCount, const RedUsageArray * arrayUsages, Red2UsageArrayTempCallStruct * arrayTempCallStructs, unsigned imageUsagesCount, const RedUsageImage * imageUsages, Red2UsageImageTempCallStruct * imageTempCallStructs, unsigned aliasesCount, const RedAlias * aliases, unsigned ordersCount, const RedOrder * orders, RedBool32 dependencyByRegion) {
  RedVkAccessStageBitflags oldAccessStages = 0;
  RedVkAccessStageBitflags newAccessStages = 0;

  for (unsigned i = 0; i < arrayUsagesCount; i += 1) {
    const RedUsageArray usage = arrayUsages[i];
    oldAccessStages |= red2InternalRedAccessStagesToRedVkAccessStages(usage.oldAccessStages);
    newAccessStages |= red2InternalRedAccessStagesToRedVkAccessStages(usage.newAccessStages);
    arrayTempCallStructs[i]._0 = 44;
    arrayTempCallStructs[i]._1 = 0;
    arrayTempCallStructs[i]._2 = red2InternalRedAccessToRedVkAccess(usage.oldAccess);
    arrayTempCallStructs[i]._3 = red2InternalRedAccessToRedVkAccess(usage.newAccess);
    arrayTempCallStructs[i]._4 = usage.queueFamilyIndexSource;
    arrayTempCallStructs[i]._5 = usage.queueFamilyIndexTarget;
    arrayTempCallStructs[i]._6 = (uint64_t)usage.array;
    arrayTempCallStructs[i]._7 = usage.arrayBytesFirst;
    arrayTempCallStructs[i]._8 = usage.arrayBytesCount;
  }

  for (unsigned i = 0; i < imageUsagesCount; i += 1) {
    const RedUsageImage usage = imageUsages[i];
    oldAccessStages |= red2InternalRedAccessStagesToRedVkAccessStages(usage.oldAccessStages);
    newAccessStages |= red2InternalRedAccessStagesToRedVkAccessStages(usage.newAccessStages);
    imageTempCallStructs[i]._0 = 45;
    imageTempCallStructs[i]._1 = 0;
    imageTempCallStructs[i]._2 = red2InternalRedAccessToRedVkAccess(usage.oldAccess);
    imageTempCallStructs[i]._3 = red2InternalRedAccessToRedVkAccess(usage.newAccess);
    imageTempCallStructs[i]._4 = usage.oldState;
    imageTempCallStructs[i]._5 = usage.newState;
    imageTempCallStructs[i]._6 = usage.queueFamilyIndexSource;
    imageTempCallStructs[i]._7 = usage.queueFamilyIndexTarget;
    imageTempCallStructs[i]._8 = (uint64_t)usage.image;
    imageTempCallStructs[i]._9._10 = usage.imageAllParts;
    imageTempCallStructs[i]._9._11 = usage.imageLevelsFirst;
    imageTempCallStructs[i]._9._12 = usage.imageLevelsCount;
    imageTempCallStructs[i]._9._13 = usage.imageLayersFirst;
    imageTempCallStructs[i]._9._14 = usage.imageLayersCount;
  }

  red2InlineCallUsageOrderBarrier(address, calls, oldAccessStages, newAccessStages, arrayUsagesCount, arrayTempCallStructs, imageUsagesCount, imageTempCallStructs, dependencyByRegion);
}

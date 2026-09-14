#pragma once

#include "redgpu.h"
#include "redgpu_computing_language.h"
#include "redgpu_array_timestamp.h"

#define REDGPU_PRINT_STATUS(x) (\
  (x) == RED_STATUS_SUCCESS                       ? "RED_STATUS_SUCCESS"                       :\
  (x) == RED_STATUS_NOT_READY                     ? "RED_STATUS_NOT_READY"                     :\
  (x) == RED_STATUS_TIMEOUT                       ? "RED_STATUS_TIMEOUT"                       :\
  (x) == RED_STATUS_GPU_TO_CPU_SIGNAL_SIGNALED    ? "RED_STATUS_GPU_TO_CPU_SIGNAL_SIGNALED"    :\
  (x) == RED_STATUS_GPU_TO_CPU_SIGNAL_UNSIGNALED  ? "RED_STATUS_GPU_TO_CPU_SIGNAL_UNSIGNALED"  :\
  (x) == RED_STATUS_INCOMPLETE                    ? "RED_STATUS_INCOMPLETE"                    :\
  (x) == RED_STATUS_PRESENT_IS_SUBOPTIMAL         ? "RED_STATUS_PRESENT_IS_SUBOPTIMAL"         :\
  (x) == RED_STATUS_ERROR_OUT_OF_CPU_MEMORY       ? "RED_STATUS_ERROR_OUT_OF_CPU_MEMORY"       :\
  (x) == RED_STATUS_ERROR_OUT_OF_GPU_MEMORY       ? "RED_STATUS_ERROR_OUT_OF_GPU_MEMORY"       :\
  (x) == RED_STATUS_ERROR_INITIALIZATION_FAILED   ? "RED_STATUS_ERROR_INITIALIZATION_FAILED"   :\
  (x) == RED_STATUS_ERROR_GPU_IS_LOST             ? "RED_STATUS_ERROR_GPU_IS_LOST"             :\
  (x) == RED_STATUS_ERROR_MEMORY_MAPPING_FAILED   ? "RED_STATUS_ERROR_MEMORY_MAPPING_FAILED"   :\
  (x) == RED_STATUS_ERROR_LAYER_IS_NOT_FOUND      ? "RED_STATUS_ERROR_LAYER_IS_NOT_FOUND"      :\
  (x) == RED_STATUS_ERROR_EXTENSION_IS_NOT_FOUND  ? "RED_STATUS_ERROR_EXTENSION_IS_NOT_FOUND"  :\
  (x) == RED_STATUS_ERROR_FEATURE_IS_NOT_FOUND    ? "RED_STATUS_ERROR_FEATURE_IS_NOT_FOUND"    :\
  (x) == RED_STATUS_ERROR_INCOMPATIBLE_DRIVER     ? "RED_STATUS_ERROR_INCOMPATIBLE_DRIVER"     :\
  (x) == RED_STATUS_ERROR_TOO_MANY_HANDLES        ? "RED_STATUS_ERROR_TOO_MANY_HANDLES"        :\
  (x) == RED_STATUS_ERROR_FORMAT_IS_NOT_SUPPORTED ? "RED_STATUS_ERROR_FORMAT_IS_NOT_SUPPORTED" :\
  (x) == RED_STATUS_ERROR_MEMORY_IS_FRAGMENTED    ? "RED_STATUS_ERROR_MEMORY_IS_FRAGMENTED"    :\
  (x) == RED_STATUS_ERROR_MEMORY_OVERFLOW         ? "RED_STATUS_ERROR_MEMORY_OVERFLOW"         :\
  (x) == RED_STATUS_ERROR_WINDOW_IS_ALREADY_USED  ? "RED_STATUS_ERROR_WINDOW_IS_ALREADY_USED"  :\
  (x) == RED_STATUS_ERROR_SURFACE_IS_LOST         ? "RED_STATUS_ERROR_SURFACE_IS_LOST"         :\
  (x) == RED_STATUS_ERROR_PRESENT_IS_OUT_OF_DATE  ? "RED_STATUS_ERROR_PRESENT_IS_OUT_OF_DATE"  :\
  (x) == RED_STATUS_ERROR_VALIDATION_FAILED       ? "RED_STATUS_ERROR_VALIDATION_FAILED"       :\
  (x) == RED_STATUS_ERROR_INVALID_GPU_CODE        ? "RED_STATUS_ERROR_INVALID_GPU_CODE"        :\
  "(unknown)"\
)

#define REDGPU_PRINT_PROCEDURE_ID(x) (\
  (unsigned)(x) == RED_PROCEDURE_ID_UNDEFINED                                      ? "RED_PROCEDURE_ID_UNDEFINED"                                      :\
  (unsigned)(x) == RED_PROCEDURE_ID_redMemoryGetBudget                             ? "RED_PROCEDURE_ID_redMemoryGetBudget"                             :\
  (unsigned)(x) == RED_PROCEDURE_ID_redMemoryAllocate                              ? "RED_PROCEDURE_ID_redMemoryAllocate"                              :\
  (unsigned)(x) == RED_PROCEDURE_ID_redMemoryAllocateMappable                      ? "RED_PROCEDURE_ID_redMemoryAllocateMappable"                      :\
  (unsigned)(x) == RED_PROCEDURE_ID_redMemoryFree                                  ? "RED_PROCEDURE_ID_redMemoryFree"                                  :\
  (unsigned)(x) == RED_PROCEDURE_ID_redMemorySet                                   ? "RED_PROCEDURE_ID_redMemorySet"                                   :\
  (unsigned)(x) == RED_PROCEDURE_ID_redMemoryMap                                   ? "RED_PROCEDURE_ID_redMemoryMap"                                   :\
  (unsigned)(x) == RED_PROCEDURE_ID_redMemoryUnmap                                 ? "RED_PROCEDURE_ID_redMemoryUnmap"                                 :\
  (unsigned)(x) == RED_PROCEDURE_ID_redMemoryNonCoherentFlush                      ? "RED_PROCEDURE_ID_redMemoryNonCoherentFlush"                      :\
  (unsigned)(x) == RED_PROCEDURE_ID_redMemoryNonCoherentInvalidate                 ? "RED_PROCEDURE_ID_redMemoryNonCoherentInvalidate"                 :\
  (unsigned)(x) == RED_PROCEDURE_ID_redStructsMemoryAllocate                       ? "RED_PROCEDURE_ID_redStructsMemoryAllocate"                       :\
  (unsigned)(x) == RED_PROCEDURE_ID_redStructsMemoryAllocateSamplers               ? "RED_PROCEDURE_ID_redStructsMemoryAllocateSamplers"               :\
  (unsigned)(x) == RED_PROCEDURE_ID_redStructsMemorySuballocateStructs             ? "RED_PROCEDURE_ID_redStructsMemorySuballocateStructs"             :\
  (unsigned)(x) == RED_PROCEDURE_ID_redStructsMemoryReset                          ? "RED_PROCEDURE_ID_redStructsMemoryReset"                          :\
  (unsigned)(x) == RED_PROCEDURE_ID_redStructsMemoryFree                           ? "RED_PROCEDURE_ID_redStructsMemoryFree"                           :\
  (unsigned)(x) == RED_PROCEDURE_ID_redStructsSet                                  ? "RED_PROCEDURE_ID_redStructsSet"                                  :\
  (unsigned)(x) == RED_PROCEDURE_ID_redCreateContext                               ? "RED_PROCEDURE_ID_redCreateContext"                               :\
  (unsigned)(x) == RED_PROCEDURE_ID_redCreateArray                                 ? "RED_PROCEDURE_ID_redCreateArray"                                 :\
  (unsigned)(x) == RED_PROCEDURE_ID_redCreateImage                                 ? "RED_PROCEDURE_ID_redCreateImage"                                 :\
  (unsigned)(x) == RED_PROCEDURE_ID_redCreateSampler                               ? "RED_PROCEDURE_ID_redCreateSampler"                               :\
  (unsigned)(x) == RED_PROCEDURE_ID_redCreateTexture                               ? "RED_PROCEDURE_ID_redCreateTexture"                               :\
  (unsigned)(x) == RED_PROCEDURE_ID_redCreateGpuCode                               ? "RED_PROCEDURE_ID_redCreateGpuCode"                               :\
  (unsigned)(x) == RED_PROCEDURE_ID_redCreateOutputDeclaration                     ? "RED_PROCEDURE_ID_redCreateOutputDeclaration"                     :\
  (unsigned)(x) == RED_PROCEDURE_ID_redCreateStructDeclaration                     ? "RED_PROCEDURE_ID_redCreateStructDeclaration"                     :\
  (unsigned)(x) == RED_PROCEDURE_ID_redCreateProcedureParameters                   ? "RED_PROCEDURE_ID_redCreateProcedureParameters"                   :\
  (unsigned)(x) == RED_PROCEDURE_ID_redCreateProcedureCache                        ? "RED_PROCEDURE_ID_redCreateProcedureCache"                        :\
  (unsigned)(x) == RED_PROCEDURE_ID_redCreateProcedure                             ? "RED_PROCEDURE_ID_redCreateProcedure"                             :\
  (unsigned)(x) == RED_PROCEDURE_ID_redCreateProcedureCompute                      ? "RED_PROCEDURE_ID_redCreateProcedureCompute"                      :\
  (unsigned)(x) == RED_PROCEDURE_ID_redCreateOutput                                ? "RED_PROCEDURE_ID_redCreateOutput"                                :\
  (unsigned)(x) == RED_PROCEDURE_ID_redCreateCpuSignal                             ? "RED_PROCEDURE_ID_redCreateCpuSignal"                             :\
  (unsigned)(x) == RED_PROCEDURE_ID_redCreateGpuSignal                             ? "RED_PROCEDURE_ID_redCreateGpuSignal"                             :\
  (unsigned)(x) == RED_PROCEDURE_ID_redCreateGpuToCpuSignal                        ? "RED_PROCEDURE_ID_redCreateGpuToCpuSignal"                        :\
  (unsigned)(x) == RED_PROCEDURE_ID_redCreateCalls                                 ? "RED_PROCEDURE_ID_redCreateCalls"                                 :\
  (unsigned)(x) == RED_PROCEDURE_ID_redCreateCallsReusable                         ? "RED_PROCEDURE_ID_redCreateCallsReusable"                         :\
  (unsigned)(x) == RED_PROCEDURE_ID_redDestroyContext                              ? "RED_PROCEDURE_ID_redDestroyContext"                              :\
  (unsigned)(x) == RED_PROCEDURE_ID_redDestroyArray                                ? "RED_PROCEDURE_ID_redDestroyArray"                                :\
  (unsigned)(x) == RED_PROCEDURE_ID_redDestroyImage                                ? "RED_PROCEDURE_ID_redDestroyImage"                                :\
  (unsigned)(x) == RED_PROCEDURE_ID_redDestroySampler                              ? "RED_PROCEDURE_ID_redDestroySampler"                              :\
  (unsigned)(x) == RED_PROCEDURE_ID_redDestroyTexture                              ? "RED_PROCEDURE_ID_redDestroyTexture"                              :\
  (unsigned)(x) == RED_PROCEDURE_ID_redDestroyGpuCode                              ? "RED_PROCEDURE_ID_redDestroyGpuCode"                              :\
  (unsigned)(x) == RED_PROCEDURE_ID_redDestroyOutputDeclaration                    ? "RED_PROCEDURE_ID_redDestroyOutputDeclaration"                    :\
  (unsigned)(x) == RED_PROCEDURE_ID_redDestroyStructDeclaration                    ? "RED_PROCEDURE_ID_redDestroyStructDeclaration"                    :\
  (unsigned)(x) == RED_PROCEDURE_ID_redDestroyProcedureParameters                  ? "RED_PROCEDURE_ID_redDestroyProcedureParameters"                  :\
  (unsigned)(x) == RED_PROCEDURE_ID_redDestroyProcedureCache                       ? "RED_PROCEDURE_ID_redDestroyProcedureCache"                       :\
  (unsigned)(x) == RED_PROCEDURE_ID_redDestroyProcedure                            ? "RED_PROCEDURE_ID_redDestroyProcedure"                            :\
  (unsigned)(x) == RED_PROCEDURE_ID_redDestroyOutput                               ? "RED_PROCEDURE_ID_redDestroyOutput"                               :\
  (unsigned)(x) == RED_PROCEDURE_ID_redDestroyCpuSignal                            ? "RED_PROCEDURE_ID_redDestroyCpuSignal"                            :\
  (unsigned)(x) == RED_PROCEDURE_ID_redDestroyGpuSignal                            ? "RED_PROCEDURE_ID_redDestroyGpuSignal"                            :\
  (unsigned)(x) == RED_PROCEDURE_ID_redDestroyGpuToCpuSignal                       ? "RED_PROCEDURE_ID_redDestroyGpuToCpuSignal"                       :\
  (unsigned)(x) == RED_PROCEDURE_ID_redDestroyCalls                                ? "RED_PROCEDURE_ID_redDestroyCalls"                                :\
  (unsigned)(x) == RED_PROCEDURE_ID_redProcedureCacheGetBlob                       ? "RED_PROCEDURE_ID_redProcedureCacheGetBlob"                       :\
  (unsigned)(x) == RED_PROCEDURE_ID_redProcedureCacheMergeCaches                   ? "RED_PROCEDURE_ID_redProcedureCacheMergeCaches"                   :\
  (unsigned)(x) == RED_PROCEDURE_ID_redCpuSignalGetStatus                          ? "RED_PROCEDURE_ID_redCpuSignalGetStatus"                          :\
  (unsigned)(x) == RED_PROCEDURE_ID_redCpuSignalWait                               ? "RED_PROCEDURE_ID_redCpuSignalWait"                               :\
  (unsigned)(x) == RED_PROCEDURE_ID_redCpuSignalUnsignal                           ? "RED_PROCEDURE_ID_redCpuSignalUnsignal"                           :\
  (unsigned)(x) == RED_PROCEDURE_ID_redGpuToCpuSignalGetStatus                     ? "RED_PROCEDURE_ID_redGpuToCpuSignalGetStatus"                     :\
  (unsigned)(x) == RED_PROCEDURE_ID_redGpuToCpuSignalUnsignal                      ? "RED_PROCEDURE_ID_redGpuToCpuSignalUnsignal"                      :\
  (unsigned)(x) == RED_PROCEDURE_ID_redCallsSet                                    ? "RED_PROCEDURE_ID_redCallsSet"                                    :\
  (unsigned)(x) == RED_PROCEDURE_ID_redCallsEnd                                    ? "RED_PROCEDURE_ID_redCallsEnd"                                    :\
  (unsigned)(x) == RED_PROCEDURE_ID_redGetCallProceduresAndAddresses               ? "RED_PROCEDURE_ID_redGetCallProceduresAndAddresses"               :\
  (unsigned)(x) == RED_PROCEDURE_ID_redQueueSubmit                                 ? "RED_PROCEDURE_ID_redQueueSubmit"                                 :\
  (unsigned)(x) == RED_PROCEDURE_ID_redMark                                        ? "RED_PROCEDURE_ID_redMark"                                        :\
  (unsigned)(x) == RED_PROCEDURE_ID_redMarkSet                                     ? "RED_PROCEDURE_ID_redMarkSet"                                     :\
  (unsigned)(x) == RED_PROCEDURE_ID_redMarkEnd                                     ? "RED_PROCEDURE_ID_redMarkEnd"                                     :\
  (unsigned)(x) == RED_PROCEDURE_ID_redCreateSurfaceWin32                          ? "RED_PROCEDURE_ID_redCreateSurfaceWin32"                          :\
  (unsigned)(x) == RED_PROCEDURE_ID_redCreateSurfaceXlibOrXcb                      ? "RED_PROCEDURE_ID_redCreateSurfaceXlibOrXcb"                      :\
  (unsigned)(x) == RED_PROCEDURE_ID_redCreatePresent                               ? "RED_PROCEDURE_ID_redCreatePresent"                               :\
  (unsigned)(x) == RED_PROCEDURE_ID_redDestroySurface                              ? "RED_PROCEDURE_ID_redDestroySurface"                              :\
  (unsigned)(x) == RED_PROCEDURE_ID_redDestroyPresent                              ? "RED_PROCEDURE_ID_redDestroyPresent"                              :\
  (unsigned)(x) == RED_PROCEDURE_ID_redQueueFamilyIndexGetSupportsPresent          ? "RED_PROCEDURE_ID_redQueueFamilyIndexGetSupportsPresent"          :\
  (unsigned)(x) == RED_PROCEDURE_ID_redSurfaceGetPresentFeatures                   ? "RED_PROCEDURE_ID_redSurfaceGetPresentFeatures"                   :\
  (unsigned)(x) == RED_PROCEDURE_ID_redSurfaceGetCurrentPropertiesAndPresentLimits ? "RED_PROCEDURE_ID_redSurfaceGetCurrentPropertiesAndPresentLimits" :\
  (unsigned)(x) == RED_PROCEDURE_ID_redPresentGetImageIndex                        ? "RED_PROCEDURE_ID_redPresentGetImageIndex"                        :\
  (unsigned)(x) == RED_PROCEDURE_ID_redQueuePresent                                ? "RED_PROCEDURE_ID_redQueuePresent"                                :\
  (unsigned)(x) == RED_PROCEDURE_ID_redDebugArrayGetHandle                         ? "RED_PROCEDURE_ID_redDebugArrayGetHandle"                         :\
  (unsigned)(x) == RED_PROCEDURE_ID_redDebugArrayCallPrint                         ? "RED_PROCEDURE_ID_redDebugArrayCallPrint"                         :\
  (unsigned)(x) == RED_PROCEDURE_ID_redCreateProcedureComputingLanguage            ? "RED_PROCEDURE_ID_redCreateProcedureComputingLanguage"            :\
  (unsigned)(x) == RED_PROCEDURE_ID_redCreateArrayTimestamp                        ? "RED_PROCEDURE_ID_redCreateArrayTimestamp"                        :\
  (unsigned)(x) == RED_PROCEDURE_ID_redDestroyArrayTimestamp                       ? "RED_PROCEDURE_ID_redDestroyArrayTimestamp"                       :\
  (unsigned)(x) == RED_PROCEDURE_ID_redArrayTimestampRead                          ? "RED_PROCEDURE_ID_redArrayTimestampRead"                          :\
  (unsigned)(x) == RED_PROCEDURE_ID_redStructsMemoryAllocateWithInlineSamplers         ? "RED_PROCEDURE_ID_redStructsMemoryAllocateWithInlineSamplers"         :\
  (unsigned)(x) == RED_PROCEDURE_ID_redStructsMemoryAllocateSamplersWithInlineSamplers ? "RED_PROCEDURE_ID_redStructsMemoryAllocateSamplersWithInlineSamplers" :\
  "(unknown)"\
)

#define REDGPU_PRINT_STATUSES(x)\
  #x".status                 = %s;\n"\
  #x".statusCode             = %d;\n"\
  #x".statusHresult          = %d;\n"\
  #x".statusProcedureId      = %s;\n"\
  #x".statusFile             = %s%s%s;\n"\
  #x".statusLine             = %d;\n"\
  #x".statusError            = %s;\n"\
  #x".statusErrorCode        = %d;\n"\
  #x".statusErrorHresult     = %d;\n"\
  #x".statusErrorProcedureId = %s;\n"\
  #x".statusErrorFile        = %s%s%s;\n"\
  #x".statusErrorLine        = %d;\n"\
  #x".statusErrorDescription = \"%s\";\n"\
  ,\
  REDGPU_PRINT_STATUS((x).status),\
  (x).statusCode,\
  (x).statusHresult,\
  REDGPU_PRINT_PROCEDURE_ID((x).statusProcedureId),\
  (x).statusFile == 0 ? "" : "\"",\
  (x).statusFile,\
  (x).statusFile == 0 ? "" : "\"",\
  (x).statusLine,\
  REDGPU_PRINT_STATUS((x).statusError),\
  (x).statusErrorCode,\
  (x).statusErrorHresult,\
  REDGPU_PRINT_PROCEDURE_ID((x).statusErrorProcedureId),\
  (x).statusErrorFile == 0 ? "" : "\"",\
  (x).statusErrorFile,\
  (x).statusErrorFile == 0 ? "" : "\"",\
  (x).statusErrorLine,\
  (x).statusErrorDescription
  
#define REDGPU_PRINT_MULTISAMPLE_COUNT_BITFLAG(x) (\
  (x) == RED_MULTISAMPLE_COUNT_BITFLAG_1  ? "RED_MULTISAMPLE_COUNT_BITFLAG_1"  :\
  (x) == RED_MULTISAMPLE_COUNT_BITFLAG_2  ? "RED_MULTISAMPLE_COUNT_BITFLAG_2"  :\
  (x) == RED_MULTISAMPLE_COUNT_BITFLAG_4  ? "RED_MULTISAMPLE_COUNT_BITFLAG_4"  :\
  (x) == RED_MULTISAMPLE_COUNT_BITFLAG_8  ? "RED_MULTISAMPLE_COUNT_BITFLAG_8"  :\
  (x) == RED_MULTISAMPLE_COUNT_BITFLAG_16 ? "RED_MULTISAMPLE_COUNT_BITFLAG_16" :\
  "(unknown)"\
)

#define REDGPU_PRINT_MULTISAMPLE_COUNT_BITFLAGS(x)\
  ((x) & RED_MULTISAMPLE_COUNT_BITFLAG_16) == RED_MULTISAMPLE_COUNT_BITFLAG_16 ? "RED_MULTISAMPLE_COUNT_BITFLAG_16" : "0",\
  ((x) & RED_MULTISAMPLE_COUNT_BITFLAG_8 ) == RED_MULTISAMPLE_COUNT_BITFLAG_8  ? "RED_MULTISAMPLE_COUNT_BITFLAG_8"  : "0",\
  ((x) & RED_MULTISAMPLE_COUNT_BITFLAG_4 ) == RED_MULTISAMPLE_COUNT_BITFLAG_4  ? "RED_MULTISAMPLE_COUNT_BITFLAG_4"  : "0",\
  ((x) & RED_MULTISAMPLE_COUNT_BITFLAG_2 ) == RED_MULTISAMPLE_COUNT_BITFLAG_2  ? "RED_MULTISAMPLE_COUNT_BITFLAG_2"  : "0",\
  ((x) & RED_MULTISAMPLE_COUNT_BITFLAG_1 ) == RED_MULTISAMPLE_COUNT_BITFLAG_1  ? "RED_MULTISAMPLE_COUNT_BITFLAG_1"  : "0"

#define REDGPU_PRINT_GPU_TYPE(x) (\
  (x) == RED_GPU_TYPE_OTHER      ? "RED_GPU_TYPE_OTHER"      :\
  (x) == RED_GPU_TYPE_INTEGRATED ? "RED_GPU_TYPE_INTEGRATED" :\
  (x) == RED_GPU_TYPE_DISCRETE   ? "RED_GPU_TYPE_DISCRETE"   :\
  (x) == RED_GPU_TYPE_VIRTUAL    ? "RED_GPU_TYPE_VIRTUAL"    :\
  (x) == RED_GPU_TYPE_CPU        ? "RED_GPU_TYPE_CPU"        :\
  "(unknown)"\
)

#define REDGPU_PRINT_IMAGE_FORMAT_LIMITS(x, y)\
  #x".imageFormatsLimitsImageDimensions1D[%s].maxDimensions.maxWidth  = %d;\n"\
  #x".imageFormatsLimitsImageDimensions1D[%s].maxDimensions.maxHeight = %d;\n"\
  #x".imageFormatsLimitsImageDimensions1D[%s].maxDimensions.maxDepth  = %d;\n"\
  #x".imageFormatsLimitsImageDimensions1D[%s].maxLevelsCount          = %d;\n"\
  #x".imageFormatsLimitsImageDimensions1D[%s].maxLayersCount          = %d;\n"\
  #x".imageFormatsLimitsImageDimensions1D[%s].supportedSampleCounts   = %s | %s | %s | %s | %s;\n"\
  #x".imageFormatsLimitsImageDimensions1D[%s].maxBytesCount           = %lu;\n"\
  #x".imageFormatsLimitsImageDimensions1D[%s].status                  = %s;\n"\
  #x".imageFormatsLimitsImageDimensions2D[%s].maxDimensions.maxWidth  = %d;\n"\
  #x".imageFormatsLimitsImageDimensions2D[%s].maxDimensions.maxHeight = %d;\n"\
  #x".imageFormatsLimitsImageDimensions2D[%s].maxDimensions.maxDepth  = %d;\n"\
  #x".imageFormatsLimitsImageDimensions2D[%s].maxLevelsCount          = %d;\n"\
  #x".imageFormatsLimitsImageDimensions2D[%s].maxLayersCount          = %d;\n"\
  #x".imageFormatsLimitsImageDimensions2D[%s].supportedSampleCounts   = %s | %s | %s | %s | %s;\n"\
  #x".imageFormatsLimitsImageDimensions2D[%s].maxBytesCount           = %lu;\n"\
  #x".imageFormatsLimitsImageDimensions2D[%s].status                  = %s;\n"\
  #x".imageFormatsLimitsImageDimensions2DMultisample[%s].maxDimensions.maxWidth  = %d;\n"\
  #x".imageFormatsLimitsImageDimensions2DMultisample[%s].maxDimensions.maxHeight = %d;\n"\
  #x".imageFormatsLimitsImageDimensions2DMultisample[%s].maxDimensions.maxDepth  = %d;\n"\
  #x".imageFormatsLimitsImageDimensions2DMultisample[%s].maxLevelsCount          = %d;\n"\
  #x".imageFormatsLimitsImageDimensions2DMultisample[%s].maxLayersCount          = %d;\n"\
  #x".imageFormatsLimitsImageDimensions2DMultisample[%s].supportedSampleCounts   = %s | %s | %s | %s | %s;\n"\
  #x".imageFormatsLimitsImageDimensions2DMultisample[%s].maxBytesCount           = %lu;\n"\
  #x".imageFormatsLimitsImageDimensions2DMultisample[%s].status                  = %s;\n"\
  #x".imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[%s].maxDimensions.maxWidth  = %d;\n"\
  #x".imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[%s].maxDimensions.maxHeight = %d;\n"\
  #x".imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[%s].maxDimensions.maxDepth  = %d;\n"\
  #x".imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[%s].maxLevelsCount          = %d;\n"\
  #x".imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[%s].maxLayersCount          = %d;\n"\
  #x".imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[%s].supportedSampleCounts   = %s | %s | %s | %s | %s;\n"\
  #x".imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[%s].maxBytesCount           = %lu;\n"\
  #x".imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[%s].status                  = %s;\n"\
  #x".imageFormatsLimitsImageDimensions3D[%s].maxDimensions.maxWidth  = %d;\n"\
  #x".imageFormatsLimitsImageDimensions3D[%s].maxDimensions.maxHeight = %d;\n"\
  #x".imageFormatsLimitsImageDimensions3D[%s].maxDimensions.maxDepth  = %d;\n"\
  #x".imageFormatsLimitsImageDimensions3D[%s].maxLevelsCount          = %d;\n"\
  #x".imageFormatsLimitsImageDimensions3D[%s].maxLayersCount          = %d;\n"\
  #x".imageFormatsLimitsImageDimensions3D[%s].supportedSampleCounts   = %s | %s | %s | %s | %s;\n"\
  #x".imageFormatsLimitsImageDimensions3D[%s].maxBytesCount           = %lu;\n"\
  #x".imageFormatsLimitsImageDimensions3D[%s].status                  = %s;\n"\
  #x".imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[%s].maxDimensions.maxWidth  = %d;\n"\
  #x".imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[%s].maxDimensions.maxHeight = %d;\n"\
  #x".imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[%s].maxDimensions.maxDepth  = %d;\n"\
  #x".imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[%s].maxLevelsCount          = %d;\n"\
  #x".imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[%s].maxLayersCount          = %d;\n"\
  #x".imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[%s].supportedSampleCounts   = %s | %s | %s | %s | %s;\n"\
  #x".imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[%s].maxBytesCount           = %lu;\n"\
  #x".imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[%s].status                  = %s;\n"\
  ,\
  REDGPU_PRINT_FORMAT((RedFormat)(y)), (x).imageFormatsLimitsImageDimensions1D[y].maxDimensions.maxWidth,\
  REDGPU_PRINT_FORMAT((RedFormat)(y)), (x).imageFormatsLimitsImageDimensions1D[y].maxDimensions.maxHeight,\
  REDGPU_PRINT_FORMAT((RedFormat)(y)), (x).imageFormatsLimitsImageDimensions1D[y].maxDimensions.maxDepth,\
  REDGPU_PRINT_FORMAT((RedFormat)(y)), (x).imageFormatsLimitsImageDimensions1D[y].maxLevelsCount,\
  REDGPU_PRINT_FORMAT((RedFormat)(y)), (x).imageFormatsLimitsImageDimensions1D[y].maxLayersCount,\
  REDGPU_PRINT_FORMAT((RedFormat)(y)), ((x).imageFormatsLimitsImageDimensions1D[y].supportedSampleCounts & RED_MULTISAMPLE_COUNT_BITFLAG_16) == RED_MULTISAMPLE_COUNT_BITFLAG_16 ? "RED_MULTISAMPLE_COUNT_BITFLAG_16" : "0",\
                                       ((x).imageFormatsLimitsImageDimensions1D[y].supportedSampleCounts & RED_MULTISAMPLE_COUNT_BITFLAG_8 ) == RED_MULTISAMPLE_COUNT_BITFLAG_8  ? "RED_MULTISAMPLE_COUNT_BITFLAG_8"  : "0",\
                                       ((x).imageFormatsLimitsImageDimensions1D[y].supportedSampleCounts & RED_MULTISAMPLE_COUNT_BITFLAG_4 ) == RED_MULTISAMPLE_COUNT_BITFLAG_4  ? "RED_MULTISAMPLE_COUNT_BITFLAG_4"  : "0",\
                                       ((x).imageFormatsLimitsImageDimensions1D[y].supportedSampleCounts & RED_MULTISAMPLE_COUNT_BITFLAG_2 ) == RED_MULTISAMPLE_COUNT_BITFLAG_2  ? "RED_MULTISAMPLE_COUNT_BITFLAG_2"  : "0",\
                                       ((x).imageFormatsLimitsImageDimensions1D[y].supportedSampleCounts & RED_MULTISAMPLE_COUNT_BITFLAG_1 ) == RED_MULTISAMPLE_COUNT_BITFLAG_1  ? "RED_MULTISAMPLE_COUNT_BITFLAG_1"  : "0",\
  REDGPU_PRINT_FORMAT((RedFormat)(y)), (unsigned long)(x).imageFormatsLimitsImageDimensions1D[y].maxBytesCount,\
  REDGPU_PRINT_FORMAT((RedFormat)(y)), REDGPU_PRINT_STATUS((x).imageFormatsLimitsImageDimensions1D[y].status),\
  REDGPU_PRINT_FORMAT((RedFormat)(y)), (x).imageFormatsLimitsImageDimensions2D[y].maxDimensions.maxWidth,\
  REDGPU_PRINT_FORMAT((RedFormat)(y)), (x).imageFormatsLimitsImageDimensions2D[y].maxDimensions.maxHeight,\
  REDGPU_PRINT_FORMAT((RedFormat)(y)), (x).imageFormatsLimitsImageDimensions2D[y].maxDimensions.maxDepth,\
  REDGPU_PRINT_FORMAT((RedFormat)(y)), (x).imageFormatsLimitsImageDimensions2D[y].maxLevelsCount,\
  REDGPU_PRINT_FORMAT((RedFormat)(y)), (x).imageFormatsLimitsImageDimensions2D[y].maxLayersCount,\
  REDGPU_PRINT_FORMAT((RedFormat)(y)), ((x).imageFormatsLimitsImageDimensions2D[y].supportedSampleCounts & RED_MULTISAMPLE_COUNT_BITFLAG_16) == RED_MULTISAMPLE_COUNT_BITFLAG_16 ? "RED_MULTISAMPLE_COUNT_BITFLAG_16" : "0",\
                                       ((x).imageFormatsLimitsImageDimensions2D[y].supportedSampleCounts & RED_MULTISAMPLE_COUNT_BITFLAG_8 ) == RED_MULTISAMPLE_COUNT_BITFLAG_8  ? "RED_MULTISAMPLE_COUNT_BITFLAG_8"  : "0",\
                                       ((x).imageFormatsLimitsImageDimensions2D[y].supportedSampleCounts & RED_MULTISAMPLE_COUNT_BITFLAG_4 ) == RED_MULTISAMPLE_COUNT_BITFLAG_4  ? "RED_MULTISAMPLE_COUNT_BITFLAG_4"  : "0",\
                                       ((x).imageFormatsLimitsImageDimensions2D[y].supportedSampleCounts & RED_MULTISAMPLE_COUNT_BITFLAG_2 ) == RED_MULTISAMPLE_COUNT_BITFLAG_2  ? "RED_MULTISAMPLE_COUNT_BITFLAG_2"  : "0",\
                                       ((x).imageFormatsLimitsImageDimensions2D[y].supportedSampleCounts & RED_MULTISAMPLE_COUNT_BITFLAG_1 ) == RED_MULTISAMPLE_COUNT_BITFLAG_1  ? "RED_MULTISAMPLE_COUNT_BITFLAG_1"  : "0",\
  REDGPU_PRINT_FORMAT((RedFormat)(y)), (unsigned long)(x).imageFormatsLimitsImageDimensions2D[y].maxBytesCount,\
  REDGPU_PRINT_FORMAT((RedFormat)(y)), REDGPU_PRINT_STATUS((x).imageFormatsLimitsImageDimensions2D[y].status),\
  REDGPU_PRINT_FORMAT((RedFormat)(y)), (x).imageFormatsLimitsImageDimensions2DMultisample[y].maxDimensions.maxWidth,\
  REDGPU_PRINT_FORMAT((RedFormat)(y)), (x).imageFormatsLimitsImageDimensions2DMultisample[y].maxDimensions.maxHeight,\
  REDGPU_PRINT_FORMAT((RedFormat)(y)), (x).imageFormatsLimitsImageDimensions2DMultisample[y].maxDimensions.maxDepth,\
  REDGPU_PRINT_FORMAT((RedFormat)(y)), (x).imageFormatsLimitsImageDimensions2DMultisample[y].maxLevelsCount,\
  REDGPU_PRINT_FORMAT((RedFormat)(y)), (x).imageFormatsLimitsImageDimensions2DMultisample[y].maxLayersCount,\
  REDGPU_PRINT_FORMAT((RedFormat)(y)), ((x).imageFormatsLimitsImageDimensions2DMultisample[y].supportedSampleCounts & RED_MULTISAMPLE_COUNT_BITFLAG_16) == RED_MULTISAMPLE_COUNT_BITFLAG_16 ? "RED_MULTISAMPLE_COUNT_BITFLAG_16" : "0",\
                                       ((x).imageFormatsLimitsImageDimensions2DMultisample[y].supportedSampleCounts & RED_MULTISAMPLE_COUNT_BITFLAG_8 ) == RED_MULTISAMPLE_COUNT_BITFLAG_8  ? "RED_MULTISAMPLE_COUNT_BITFLAG_8"  : "0",\
                                       ((x).imageFormatsLimitsImageDimensions2DMultisample[y].supportedSampleCounts & RED_MULTISAMPLE_COUNT_BITFLAG_4 ) == RED_MULTISAMPLE_COUNT_BITFLAG_4  ? "RED_MULTISAMPLE_COUNT_BITFLAG_4"  : "0",\
                                       ((x).imageFormatsLimitsImageDimensions2DMultisample[y].supportedSampleCounts & RED_MULTISAMPLE_COUNT_BITFLAG_2 ) == RED_MULTISAMPLE_COUNT_BITFLAG_2  ? "RED_MULTISAMPLE_COUNT_BITFLAG_2"  : "0",\
                                       ((x).imageFormatsLimitsImageDimensions2DMultisample[y].supportedSampleCounts & RED_MULTISAMPLE_COUNT_BITFLAG_1 ) == RED_MULTISAMPLE_COUNT_BITFLAG_1  ? "RED_MULTISAMPLE_COUNT_BITFLAG_1"  : "0",\
  REDGPU_PRINT_FORMAT((RedFormat)(y)), (unsigned long)(x).imageFormatsLimitsImageDimensions2DMultisample[y].maxBytesCount,\
  REDGPU_PRINT_FORMAT((RedFormat)(y)), REDGPU_PRINT_STATUS((x).imageFormatsLimitsImageDimensions2DMultisample[y].status),\
  REDGPU_PRINT_FORMAT((RedFormat)(y)), (x).imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[y].maxDimensions.maxWidth,\
  REDGPU_PRINT_FORMAT((RedFormat)(y)), (x).imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[y].maxDimensions.maxHeight,\
  REDGPU_PRINT_FORMAT((RedFormat)(y)), (x).imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[y].maxDimensions.maxDepth,\
  REDGPU_PRINT_FORMAT((RedFormat)(y)), (x).imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[y].maxLevelsCount,\
  REDGPU_PRINT_FORMAT((RedFormat)(y)), (x).imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[y].maxLayersCount,\
  REDGPU_PRINT_FORMAT((RedFormat)(y)), ((x).imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[y].supportedSampleCounts & RED_MULTISAMPLE_COUNT_BITFLAG_16) == RED_MULTISAMPLE_COUNT_BITFLAG_16 ? "RED_MULTISAMPLE_COUNT_BITFLAG_16" : "0",\
                                       ((x).imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[y].supportedSampleCounts & RED_MULTISAMPLE_COUNT_BITFLAG_8 ) == RED_MULTISAMPLE_COUNT_BITFLAG_8  ? "RED_MULTISAMPLE_COUNT_BITFLAG_8"  : "0",\
                                       ((x).imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[y].supportedSampleCounts & RED_MULTISAMPLE_COUNT_BITFLAG_4 ) == RED_MULTISAMPLE_COUNT_BITFLAG_4  ? "RED_MULTISAMPLE_COUNT_BITFLAG_4"  : "0",\
                                       ((x).imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[y].supportedSampleCounts & RED_MULTISAMPLE_COUNT_BITFLAG_2 ) == RED_MULTISAMPLE_COUNT_BITFLAG_2  ? "RED_MULTISAMPLE_COUNT_BITFLAG_2"  : "0",\
                                       ((x).imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[y].supportedSampleCounts & RED_MULTISAMPLE_COUNT_BITFLAG_1 ) == RED_MULTISAMPLE_COUNT_BITFLAG_1  ? "RED_MULTISAMPLE_COUNT_BITFLAG_1"  : "0",\
  REDGPU_PRINT_FORMAT((RedFormat)(y)), (unsigned long)(x).imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[y].maxBytesCount,\
  REDGPU_PRINT_FORMAT((RedFormat)(y)), REDGPU_PRINT_STATUS((x).imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[y].status),\
  REDGPU_PRINT_FORMAT((RedFormat)(y)), (x).imageFormatsLimitsImageDimensions3D[y].maxDimensions.maxWidth,\
  REDGPU_PRINT_FORMAT((RedFormat)(y)), (x).imageFormatsLimitsImageDimensions3D[y].maxDimensions.maxHeight,\
  REDGPU_PRINT_FORMAT((RedFormat)(y)), (x).imageFormatsLimitsImageDimensions3D[y].maxDimensions.maxDepth,\
  REDGPU_PRINT_FORMAT((RedFormat)(y)), (x).imageFormatsLimitsImageDimensions3D[y].maxLevelsCount,\
  REDGPU_PRINT_FORMAT((RedFormat)(y)), (x).imageFormatsLimitsImageDimensions3D[y].maxLayersCount,\
  REDGPU_PRINT_FORMAT((RedFormat)(y)), ((x).imageFormatsLimitsImageDimensions3D[y].supportedSampleCounts & RED_MULTISAMPLE_COUNT_BITFLAG_16) == RED_MULTISAMPLE_COUNT_BITFLAG_16 ? "RED_MULTISAMPLE_COUNT_BITFLAG_16" : "0",\
                                       ((x).imageFormatsLimitsImageDimensions3D[y].supportedSampleCounts & RED_MULTISAMPLE_COUNT_BITFLAG_8 ) == RED_MULTISAMPLE_COUNT_BITFLAG_8  ? "RED_MULTISAMPLE_COUNT_BITFLAG_8"  : "0",\
                                       ((x).imageFormatsLimitsImageDimensions3D[y].supportedSampleCounts & RED_MULTISAMPLE_COUNT_BITFLAG_4 ) == RED_MULTISAMPLE_COUNT_BITFLAG_4  ? "RED_MULTISAMPLE_COUNT_BITFLAG_4"  : "0",\
                                       ((x).imageFormatsLimitsImageDimensions3D[y].supportedSampleCounts & RED_MULTISAMPLE_COUNT_BITFLAG_2 ) == RED_MULTISAMPLE_COUNT_BITFLAG_2  ? "RED_MULTISAMPLE_COUNT_BITFLAG_2"  : "0",\
                                       ((x).imageFormatsLimitsImageDimensions3D[y].supportedSampleCounts & RED_MULTISAMPLE_COUNT_BITFLAG_1 ) == RED_MULTISAMPLE_COUNT_BITFLAG_1  ? "RED_MULTISAMPLE_COUNT_BITFLAG_1"  : "0",\
  REDGPU_PRINT_FORMAT((RedFormat)(y)), (unsigned long)(x).imageFormatsLimitsImageDimensions3D[y].maxBytesCount,\
  REDGPU_PRINT_FORMAT((RedFormat)(y)), REDGPU_PRINT_STATUS((x).imageFormatsLimitsImageDimensions3D[y].status),\
  REDGPU_PRINT_FORMAT((RedFormat)(y)), (x).imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[y].maxDimensions.maxWidth,\
  REDGPU_PRINT_FORMAT((RedFormat)(y)), (x).imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[y].maxDimensions.maxHeight,\
  REDGPU_PRINT_FORMAT((RedFormat)(y)), (x).imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[y].maxDimensions.maxDepth,\
  REDGPU_PRINT_FORMAT((RedFormat)(y)), (x).imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[y].maxLevelsCount,\
  REDGPU_PRINT_FORMAT((RedFormat)(y)), (x).imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[y].maxLayersCount,\
  REDGPU_PRINT_FORMAT((RedFormat)(y)), ((x).imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[y].supportedSampleCounts & RED_MULTISAMPLE_COUNT_BITFLAG_16) == RED_MULTISAMPLE_COUNT_BITFLAG_16 ? "RED_MULTISAMPLE_COUNT_BITFLAG_16" : "0",\
                                       ((x).imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[y].supportedSampleCounts & RED_MULTISAMPLE_COUNT_BITFLAG_8 ) == RED_MULTISAMPLE_COUNT_BITFLAG_8  ? "RED_MULTISAMPLE_COUNT_BITFLAG_8"  : "0",\
                                       ((x).imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[y].supportedSampleCounts & RED_MULTISAMPLE_COUNT_BITFLAG_4 ) == RED_MULTISAMPLE_COUNT_BITFLAG_4  ? "RED_MULTISAMPLE_COUNT_BITFLAG_4"  : "0",\
                                       ((x).imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[y].supportedSampleCounts & RED_MULTISAMPLE_COUNT_BITFLAG_2 ) == RED_MULTISAMPLE_COUNT_BITFLAG_2  ? "RED_MULTISAMPLE_COUNT_BITFLAG_2"  : "0",\
                                       ((x).imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[y].supportedSampleCounts & RED_MULTISAMPLE_COUNT_BITFLAG_1 ) == RED_MULTISAMPLE_COUNT_BITFLAG_1  ? "RED_MULTISAMPLE_COUNT_BITFLAG_1"  : "0",\
  REDGPU_PRINT_FORMAT((RedFormat)(y)), (unsigned long)(x).imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[y].maxBytesCount,\
  REDGPU_PRINT_FORMAT((RedFormat)(y)), REDGPU_PRINT_STATUS((x).imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[y].status)

#define REDGPU_PRINT_IMAGE_FORMAT_FEATURES(x, y)\
  #x".imageFormatsFeatures[%s].supportsCopyR                    = %d;\n"\
  #x".imageFormatsFeatures[%s].supportsCopyW                    = %d;\n"\
  #x".imageFormatsFeatures[%s].supportsTextureRO                = %d;\n"\
  #x".imageFormatsFeatures[%s].supportsTextureRW                = %d;\n"\
  #x".imageFormatsFeatures[%s].supportsOutputDepthStencil       = %d;\n"\
  #x".imageFormatsFeatures[%s].supportsOutputColor              = %d;\n"\
  #x".imageFormatsFeatures[%s].supportsTextureROFilteringLinear = %d;\n"\
  #x".imageFormatsFeatures[%s].supportsTextureRWAtomics         = %d;\n"\
  #x".imageFormatsFeatures[%s].supportsOutputColorBlend         = %d;\n"\
  ,\
  REDGPU_PRINT_FORMAT((RedFormat)(y)), (x).imageFormatsFeatures[y].supportsCopyR,\
  REDGPU_PRINT_FORMAT((RedFormat)(y)), (x).imageFormatsFeatures[y].supportsCopyW,\
  REDGPU_PRINT_FORMAT((RedFormat)(y)), (x).imageFormatsFeatures[y].supportsTextureRO,\
  REDGPU_PRINT_FORMAT((RedFormat)(y)), (x).imageFormatsFeatures[y].supportsTextureRW,\
  REDGPU_PRINT_FORMAT((RedFormat)(y)), (x).imageFormatsFeatures[y].supportsOutputDepthStencil,\
  REDGPU_PRINT_FORMAT((RedFormat)(y)), (x).imageFormatsFeatures[y].supportsOutputColor,\
  REDGPU_PRINT_FORMAT((RedFormat)(y)), (x).imageFormatsFeatures[y].supportsTextureROFilteringLinear,\
  REDGPU_PRINT_FORMAT((RedFormat)(y)), (x).imageFormatsFeatures[y].supportsTextureRWAtomics,\
  REDGPU_PRINT_FORMAT((RedFormat)(y)), (x).imageFormatsFeatures[y].supportsOutputColorBlend

#define REDGPU_PRINT_DRIVER_ID(x) (\
  (x) == RED_DRIVER_ID_UNKNOWN                   ? "RED_DRIVER_ID_UNKNOWN"                   :\
  (x) == RED_DRIVER_ID_AMD_PROPRIETARY           ? "RED_DRIVER_ID_AMD_PROPRIETARY"           :\
  (x) == RED_DRIVER_ID_AMD_OPEN_SOURCE           ? "RED_DRIVER_ID_AMD_OPEN_SOURCE"           :\
  (x) == RED_DRIVER_ID_MESA_RADV                 ? "RED_DRIVER_ID_MESA_RADV"                 :\
  (x) == RED_DRIVER_ID_NVIDIA_PROPRIETARY        ? "RED_DRIVER_ID_NVIDIA_PROPRIETARY"        :\
  (x) == RED_DRIVER_ID_INTEL_PROPRIETARY_WINDOWS ? "RED_DRIVER_ID_INTEL_PROPRIETARY_WINDOWS" :\
  (x) == RED_DRIVER_ID_INTEL_OPEN_SOURCE_MESA    ? "RED_DRIVER_ID_INTEL_OPEN_SOURCE_MESA"    :\
  (x) == RED_DRIVER_ID_IMAGINATION_PROPRIETARY   ? "RED_DRIVER_ID_IMAGINATION_PROPRIETARY"   :\
  (x) == RED_DRIVER_ID_QUALCOMM_PROPRIETARY      ? "RED_DRIVER_ID_QUALCOMM_PROPRIETARY"      :\
  (x) == RED_DRIVER_ID_ARM_PROPRIETARY           ? "RED_DRIVER_ID_ARM_PROPRIETARY"           :\
  (x) == RED_DRIVER_ID_GOOGLE_SWIFTSHADER        ? "RED_DRIVER_ID_GOOGLE_SWIFTSHADER"        :\
  (x) == RED_DRIVER_ID_GGP_PROPRIETARY           ? "RED_DRIVER_ID_GGP_PROPRIETARY"           :\
  (x) == RED_DRIVER_ID_BROADCOM_PROPRIETARY      ? "RED_DRIVER_ID_BROADCOM_PROPRIETARY"      :\
  "(unknown)"\
)

#define REDGPU_PRINT_OPTIONAL_INFO_ADDITIONAL_INFO_0(x)\
  #x".supportsArraysImagesUsageBeforeMemorySet                                                   = %d;\n"\
  #x".supportsInitialQueueFamilyIndexSetToMaxValueForSimultaneousQueueAccessOfArrays             = %d;\n"\
  #x".supportsInitialQueueFamilyIndexSetToMaxValueForSimultaneousQueueAccessOfImages             = %d;\n"\
  #x".supportsInitialQueueFamilyIndexSetToMaxValueForSimultaneousQueueAccessOfImagesMultisample  = %d;\n"\
  #x".supportsInitialQueueFamilyIndexSetToMaxValueForSimultaneousQueueAccessOfImagesDepthStencil = %d;\n"\
  ,\
  ((const RedGpuInfoOptionalInfoAdditionalInfo0 *)(x))->supportsArraysImagesUsageBeforeMemorySet,\
  ((const RedGpuInfoOptionalInfoAdditionalInfo0 *)(x))->supportsInitialQueueFamilyIndexSetToMaxValueForSimultaneousQueueAccessOfArrays,\
  ((const RedGpuInfoOptionalInfoAdditionalInfo0 *)(x))->supportsInitialQueueFamilyIndexSetToMaxValueForSimultaneousQueueAccessOfImages,\
  ((const RedGpuInfoOptionalInfoAdditionalInfo0 *)(x))->supportsInitialQueueFamilyIndexSetToMaxValueForSimultaneousQueueAccessOfImagesMultisample,\
  ((const RedGpuInfoOptionalInfoAdditionalInfo0 *)(x))->supportsInitialQueueFamilyIndexSetToMaxValueForSimultaneousQueueAccessOfImagesDepthStencil

#define REDGPU_PRINT_OPTIONAL_INFO_DRIVER_PROPERTIES(x)\
  #x".id                                               = %s;\n"\
  #x".name                                             = %s;\n"\
  #x".info                                             = %s;\n"\
  #x".compliantWithConformanceTestSuiteVersionMajor    = %d;\n"\
  #x".compliantWithConformanceTestSuiteVersionMinor    = %d;\n"\
  #x".compliantWithConformanceTestSuiteVersionSubminor = %d;\n"\
  #x".compliantWithConformanceTestSuiteVersionPatch    = %d;\n"\
  ,\
  REDGPU_PRINT_DRIVER_ID(((const RedGpuInfoOptionalInfoDriverProperties *)(x))->id),\
  ((const RedGpuInfoOptionalInfoDriverProperties *)(x))->name,\
  ((const RedGpuInfoOptionalInfoDriverProperties *)(x))->info,\
  ((const RedGpuInfoOptionalInfoDriverProperties *)(x))->compliantWithConformanceTestSuiteVersionMajor,\
  ((const RedGpuInfoOptionalInfoDriverProperties *)(x))->compliantWithConformanceTestSuiteVersionMinor,\
  ((const RedGpuInfoOptionalInfoDriverProperties *)(x))->compliantWithConformanceTestSuiteVersionSubminor,\
  ((const RedGpuInfoOptionalInfoDriverProperties *)(x))->compliantWithConformanceTestSuiteVersionPatch

#define REDGPU_PRINT_OPTIONAL_INFO_RESOLVE_DEPTH_STENCIL(x)\
  #x".supportsResolveDepthStencil          = %d;\n"\
  #x".supportsResolveModeDepthSampleZero   = %d;\n"\
  #x".supportsResolveModeDepthAverage      = %d;\n"\
  #x".supportsResolveModeDepthMin          = %d;\n"\
  #x".supportsResolveModeDepthMax          = %d;\n"\
  #x".supportsResolveModeStencilSampleZero = %d;\n"\
  #x".supportsResolveModeStencilAverage    = %d;\n"\
  #x".supportsResolveModeStencilMin        = %d;\n"\
  #x".supportsResolveModeStencilMax        = %d;\n"\
  #x".supportsResolveIndependentNone       = %d;\n"\
  #x".supportsResolveIndependent           = %d;\n"\
  ,\
  ((const RedGpuInfoOptionalInfoResolveDepthStencil *)(x))->supportsResolveDepthStencil,\
  ((const RedGpuInfoOptionalInfoResolveDepthStencil *)(x))->supportsResolveModeDepthSampleZero,\
  ((const RedGpuInfoOptionalInfoResolveDepthStencil *)(x))->supportsResolveModeDepthAverage,\
  ((const RedGpuInfoOptionalInfoResolveDepthStencil *)(x))->supportsResolveModeDepthMin,\
  ((const RedGpuInfoOptionalInfoResolveDepthStencil *)(x))->supportsResolveModeDepthMax,\
  ((const RedGpuInfoOptionalInfoResolveDepthStencil *)(x))->supportsResolveModeStencilSampleZero,\
  ((const RedGpuInfoOptionalInfoResolveDepthStencil *)(x))->supportsResolveModeStencilAverage,\
  ((const RedGpuInfoOptionalInfoResolveDepthStencil *)(x))->supportsResolveModeStencilMin,\
  ((const RedGpuInfoOptionalInfoResolveDepthStencil *)(x))->supportsResolveModeStencilMax,\
  ((const RedGpuInfoOptionalInfoResolveDepthStencil *)(x))->supportsResolveIndependentNone,\
  ((const RedGpuInfoOptionalInfoResolveDepthStencil *)(x))->supportsResolveIndependent

#define REDGPU_PRINT_OPTIONAL_INFO_PROCEDURE_PARAMETERS_HANDLES(x)\
  #x".supportsProcedureParametersHandles = %d;\n"\
  #x".maxProcedureParametersHandlesCount = %d;\n"\
  ,\
  ((const RedGpuInfoOptionalInfoProcedureParametersHandles *)(x))->supportsProcedureParametersHandles,\
  ((const RedGpuInfoOptionalInfoProcedureParametersHandles *)(x))->maxProcedureParametersHandlesCount

#define REDGPU_PRINT_OPTIONAL_INFO_RASTERIZATION_MODE(x)\
  #x".supportsRasterizationModeOverestimate         = %d;\n"\
  #x".supportsRasterizationModeUnderestimate        = %d;\n"\
  #x".supportsGuaranteedZeroAreaBackFacingTriangles = %d;\n"\
  #x".supportsGpuCodeFullyCoveredEXT                = %d;\n"\
  ,\
  ((const RedGpuInfoOptionalInfoRasterizationMode *)(x))->supportsRasterizationModeOverestimate,\
  ((const RedGpuInfoOptionalInfoRasterizationMode *)(x))->supportsRasterizationModeUnderestimate,\
  ((const RedGpuInfoOptionalInfoRasterizationMode *)(x))->supportsGuaranteedZeroAreaBackFacingTriangles,\
  ((const RedGpuInfoOptionalInfoRasterizationMode *)(x))->supportsGpuCodeFullyCoveredEXT

#define REDGPU_PRINT_OPTIONAL_INFO_FORMAL_MEMORY_MODEL(x)\
  #x".supportsFormalMemoryModel                                = %d;\n"\
  #x".supportsFormalMemoryModelGpuScopeSynchronization         = %d;\n"\
  #x".supportsFormalMemoryModelAvailabilityAndVisibilityChains = %d;\n"\
  ,\
  ((const RedGpuInfoOptionalInfoFormalMemoryModel *)(x))->supportsFormalMemoryModel,\
  ((const RedGpuInfoOptionalInfoFormalMemoryModel *)(x))->supportsFormalMemoryModelGpuScopeSynchronization,\
  ((const RedGpuInfoOptionalInfoFormalMemoryModel *)(x))->supportsFormalMemoryModelAvailabilityAndVisibilityChains

#define REDGPU_PRINT_OPTIONAL_INFO_RAY_TRACING(x)\
  #x".supportsRayTracing = %d;\n"\
  ,\
  ((const RedGpuInfoOptionalInfoRayTracing *)(x))->supportsRayTracing

#define REDGPU_PRINT_OPTIONAL_INFO_COMPUTING_LANGUAGE_FEATURE_LEVEL_1(x)\
  #x".supportsComputingLanguageFeatureLevel1 = %d;\n"\
  ,\
  ((const RedGpuInfoOptionalInfoComputingLanguageFeatureLevel1 *)(x))->supportsComputingLanguageFeatureLevel1

#define REDGPU_PRINT_OPTIONAL_INFO_ARRAY_TIMESTAMP(x)\
  #x".supportsArrayTimestamp = %d;\n"\
  #x".nanosecondsPerTick = %.9g;\n"\
  ,\
  ((const RedGpuInfoOptionalInfoArrayTimestamp *)(x))->supportsArrayTimestamp,\
  ((const RedGpuInfoOptionalInfoArrayTimestamp *)(x))->nanosecondsPerTick

#define REDGPU_PRINT_MEMORY_BUDGET(x)\
  #x".memoryHeapsBudget = {[0] = %lu, [1] = %lu, [2] = %lu, [3] = %lu, [4] = %lu, [5] = %lu, [6] = %lu, [7] = %lu, [8] = %lu, [9] = %lu, [10] = %lu, [11] = %lu, [12] = %lu, [13] = %lu, [14] = %lu, [15] = %lu};\n"\
  #x".memoryHeapsUsage  = {[0] = %lu, [1] = %lu, [2] = %lu, [3] = %lu, [4] = %lu, [5] = %lu, [6] = %lu, [7] = %lu, [8] = %lu, [9] = %lu, [10] = %lu, [11] = %lu, [12] = %lu, [13] = %lu, [14] = %lu, [15] = %lu};\n"\
  ,\
  (unsigned long)(x).memoryHeapsBudget[0],\
  (unsigned long)(x).memoryHeapsBudget[1],\
  (unsigned long)(x).memoryHeapsBudget[2],\
  (unsigned long)(x).memoryHeapsBudget[3],\
  (unsigned long)(x).memoryHeapsBudget[4],\
  (unsigned long)(x).memoryHeapsBudget[5],\
  (unsigned long)(x).memoryHeapsBudget[6],\
  (unsigned long)(x).memoryHeapsBudget[7],\
  (unsigned long)(x).memoryHeapsBudget[8],\
  (unsigned long)(x).memoryHeapsBudget[9],\
  (unsigned long)(x).memoryHeapsBudget[10],\
  (unsigned long)(x).memoryHeapsBudget[11],\
  (unsigned long)(x).memoryHeapsBudget[12],\
  (unsigned long)(x).memoryHeapsBudget[13],\
  (unsigned long)(x).memoryHeapsBudget[14],\
  (unsigned long)(x).memoryHeapsBudget[15],\
  (unsigned long)(x).memoryHeapsUsage[0],\
  (unsigned long)(x).memoryHeapsUsage[1],\
  (unsigned long)(x).memoryHeapsUsage[2],\
  (unsigned long)(x).memoryHeapsUsage[3],\
  (unsigned long)(x).memoryHeapsUsage[4],\
  (unsigned long)(x).memoryHeapsUsage[5],\
  (unsigned long)(x).memoryHeapsUsage[6],\
  (unsigned long)(x).memoryHeapsUsage[7],\
  (unsigned long)(x).memoryHeapsUsage[8],\
  (unsigned long)(x).memoryHeapsUsage[9],\
  (unsigned long)(x).memoryHeapsUsage[10],\
  (unsigned long)(x).memoryHeapsUsage[11],\
  (unsigned long)(x).memoryHeapsUsage[12],\
  (unsigned long)(x).memoryHeapsUsage[13],\
  (unsigned long)(x).memoryHeapsUsage[14],\
  (unsigned long)(x).memoryHeapsUsage[15]

#define REDGPU_PRINT_FORMAT(x) (\
  (x) == RED_FORMAT_UNDEFINED                                      ? "RED_FORMAT_UNDEFINED"                                      :\
  (x) == RED_FORMAT_R_8_UINT_TO_FLOAT_0_1                          ? "RED_FORMAT_R_8_UINT_TO_FLOAT_0_1"                          :\
  (x) == RED_FORMAT_R_8_UINT                                       ? "RED_FORMAT_R_8_UINT"                                       :\
  (x) == RED_FORMAT_R_8_SINT                                       ? "RED_FORMAT_R_8_SINT"                                       :\
  (x) == RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1                 ? "RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1"                 :\
  (x) == RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1_GAMMA_CORRECTED ? "RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1_GAMMA_CORRECTED" :\
  (x) == RED_FORMAT_RGBA_8_8_8_8_UINT                              ? "RED_FORMAT_RGBA_8_8_8_8_UINT"                              :\
  (x) == RED_FORMAT_RGBA_8_8_8_8_SINT                              ? "RED_FORMAT_RGBA_8_8_8_8_SINT"                              :\
  (x) == RED_FORMAT_PRESENT_BGRA_8_8_8_8_UINT_TO_FLOAT_0_1         ? "RED_FORMAT_PRESENT_BGRA_8_8_8_8_UINT_TO_FLOAT_0_1"         :\
  (x) == RED_FORMAT_R_16_UINT                                      ? "RED_FORMAT_R_16_UINT"                                      :\
  (x) == RED_FORMAT_R_16_SINT                                      ? "RED_FORMAT_R_16_SINT"                                      :\
  (x) == RED_FORMAT_R_16_FLOAT                                     ? "RED_FORMAT_R_16_FLOAT"                                     :\
  (x) == RED_FORMAT_RGBA_16_16_16_16_UINT                          ? "RED_FORMAT_RGBA_16_16_16_16_UINT"                          :\
  (x) == RED_FORMAT_RGBA_16_16_16_16_SINT                          ? "RED_FORMAT_RGBA_16_16_16_16_SINT"                          :\
  (x) == RED_FORMAT_RGBA_16_16_16_16_FLOAT                         ? "RED_FORMAT_RGBA_16_16_16_16_FLOAT"                         :\
  (x) == RED_FORMAT_R_32_UINT                                      ? "RED_FORMAT_R_32_UINT"                                      :\
  (x) == RED_FORMAT_R_32_SINT                                      ? "RED_FORMAT_R_32_SINT"                                      :\
  (x) == RED_FORMAT_R_32_FLOAT                                     ? "RED_FORMAT_R_32_FLOAT"                                     :\
  (x) == RED_FORMAT_RGBA_32_32_32_32_UINT                          ? "RED_FORMAT_RGBA_32_32_32_32_UINT"                          :\
  (x) == RED_FORMAT_RGBA_32_32_32_32_SINT                          ? "RED_FORMAT_RGBA_32_32_32_32_SINT"                          :\
  (x) == RED_FORMAT_RGBA_32_32_32_32_FLOAT                         ? "RED_FORMAT_RGBA_32_32_32_32_FLOAT"                         :\
  (x) == RED_FORMAT_DEPTH_16_UINT_TO_FLOAT_0_1                     ? "RED_FORMAT_DEPTH_16_UINT_TO_FLOAT_0_1"                     :\
  (x) == RED_FORMAT_DEPTH_32_FLOAT                                 ? "RED_FORMAT_DEPTH_32_FLOAT"                                 :\
  (x) == RED_FORMAT_DEPTH_24_UINT_TO_FLOAT_0_1_STENCIL_8_UINT      ? "RED_FORMAT_DEPTH_24_UINT_TO_FLOAT_0_1_STENCIL_8_UINT"      :\
  (x) == RED_FORMAT_DEPTH_32_FLOAT_STENCIL_8_UINT                  ? "RED_FORMAT_DEPTH_32_FLOAT_STENCIL_8_UINT"                  :\
  "(unknown)"\
)

#define REDGPU_PRINT_HANDLE_TYPE(x) (\
  (unsigned)(x) == RED_HANDLE_TYPE_CONTEXT              ? "RED_HANDLE_TYPE_CONTEXT"              :\
  (unsigned)(x) == RED_HANDLE_TYPE_GPU                  ? "RED_HANDLE_TYPE_GPU"                  :\
  (unsigned)(x) == RED_HANDLE_TYPE_GPU_DEVICE           ? "RED_HANDLE_TYPE_GPU_DEVICE"           :\
  (unsigned)(x) == RED_HANDLE_TYPE_QUEUE                ? "RED_HANDLE_TYPE_QUEUE"                :\
  (unsigned)(x) == RED_HANDLE_TYPE_MEMORY               ? "RED_HANDLE_TYPE_MEMORY"               :\
  (unsigned)(x) == RED_HANDLE_TYPE_ARRAY                ? "RED_HANDLE_TYPE_ARRAY"                :\
  (unsigned)(x) == RED_HANDLE_TYPE_IMAGE                ? "RED_HANDLE_TYPE_IMAGE"                :\
  (unsigned)(x) == RED_HANDLE_TYPE_SAMPLER              ? "RED_HANDLE_TYPE_SAMPLER"              :\
  (unsigned)(x) == RED_HANDLE_TYPE_TEXTURE              ? "RED_HANDLE_TYPE_TEXTURE"              :\
  (unsigned)(x) == RED_HANDLE_TYPE_GPU_CODE             ? "RED_HANDLE_TYPE_GPU_CODE"             :\
  (unsigned)(x) == RED_HANDLE_TYPE_OUTPUT_DECLARATION   ? "RED_HANDLE_TYPE_OUTPUT_DECLARATION"   :\
  (unsigned)(x) == RED_HANDLE_TYPE_STRUCT_DECLARATION   ? "RED_HANDLE_TYPE_STRUCT_DECLARATION"   :\
  (unsigned)(x) == RED_HANDLE_TYPE_PROCEDURE_PARAMETERS ? "RED_HANDLE_TYPE_PROCEDURE_PARAMETERS" :\
  (unsigned)(x) == RED_HANDLE_TYPE_PROCEDURE_CACHE      ? "RED_HANDLE_TYPE_PROCEDURE_CACHE"      :\
  (unsigned)(x) == RED_HANDLE_TYPE_PROCEDURE            ? "RED_HANDLE_TYPE_PROCEDURE"            :\
  (unsigned)(x) == RED_HANDLE_TYPE_OUTPUT               ? "RED_HANDLE_TYPE_OUTPUT"               :\
  (unsigned)(x) == RED_HANDLE_TYPE_STRUCT               ? "RED_HANDLE_TYPE_STRUCT"               :\
  (unsigned)(x) == RED_HANDLE_TYPE_STRUCTS_MEMORY       ? "RED_HANDLE_TYPE_STRUCTS_MEMORY"       :\
  (unsigned)(x) == RED_HANDLE_TYPE_CALLS                ? "RED_HANDLE_TYPE_CALLS"                :\
  (unsigned)(x) == RED_HANDLE_TYPE_CALLS_MEMORY         ? "RED_HANDLE_TYPE_CALLS_MEMORY"         :\
  (unsigned)(x) == RED_HANDLE_TYPE_CPU_SIGNAL           ? "RED_HANDLE_TYPE_CPU_SIGNAL"           :\
  (unsigned)(x) == RED_HANDLE_TYPE_GPU_SIGNAL           ? "RED_HANDLE_TYPE_GPU_SIGNAL"           :\
  (unsigned)(x) == RED_HANDLE_TYPE_GPU_TO_CPU_SIGNAL    ? "RED_HANDLE_TYPE_GPU_TO_CPU_SIGNAL"    :\
  (unsigned)(x) == RED_HANDLE_TYPE_SURFACE              ? "RED_HANDLE_TYPE_SURFACE"              :\
  (unsigned)(x) == RED_HANDLE_TYPE_PRESENT              ? "RED_HANDLE_TYPE_PRESENT"              :\
  (unsigned)(x) == RED_HANDLE_TYPE_ARRAY_TIMESTAMP      ? "RED_HANDLE_TYPE_ARRAY_TIMESTAMP"      :\
  "(unknown)"\
)


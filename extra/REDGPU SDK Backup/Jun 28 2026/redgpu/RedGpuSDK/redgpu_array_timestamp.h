#pragma once

#include "redgpu.h"

#ifdef __cplusplus
extern "C" {
#endif

#if defined(_WIN32) && !defined(_WIN64)
typedef uint64_t                             RedHandleArrayTimestamp;
#else
typedef struct RedTypeHandleArrayTimestamp * RedHandleArrayTimestamp;
#endif

typedef enum RedProcedureId2 {
  RED_PROCEDURE_ID_redCreateArrayTimestamp  = 77,
  RED_PROCEDURE_ID_redDestroyArrayTimestamp = 78,
  RED_PROCEDURE_ID_redArrayTimestampRead    = 79,
} RedProcedureId2;

// redCreateContext

typedef enum RedSdkExtension2 {
  RED_SDK_EXTENSION_ARRAY_TIMESTAMP = 15,
} RedSdkExtension2;

typedef enum RedGpuInfoOptionalInfo2 {
  RED_GPU_INFO_OPTIONAL_INFO_ARRAY_TIMESTAMP = 15,
} RedGpuInfoOptionalInfo2;

typedef struct RedGpuInfoOptionalInfoArrayTimestamp {
  RedGpuInfoOptionalInfo2 optionalInfo;
  const void *            next;
  RedBool32               supportsArrayTimestamp;
  const unsigned *        queuesPrecisionBits64BitTicksCount; // Array of RedGpuInfo::queuesCount, can be NULL if supportsArrayTimestamp == 0
  float                   nanosecondsPerTick;
} RedGpuInfoOptionalInfoArrayTimestamp;

typedef enum RedHandleType1 {
  RED_HANDLE_TYPE_ARRAY_TIMESTAMP = 12,
} RedHandleType1;

REDGPU_DECLSPEC void REDGPU_API redCreateArrayTimestamp    (RedContext context, RedHandleGpu gpu, const char * handleName, unsigned count, RedHandleArrayTimestamp * outArrayTimestamp, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData);
REDGPU_DECLSPEC void REDGPU_API redDestroyArrayTimestamp   (RedContext context, RedHandleGpu gpu, RedHandleArrayTimestamp arrayTimestamp, const char * optionalFile, int optionalLine, void * optionalUserData);
REDGPU_DECLSPEC void REDGPU_API redCallArrayTimestampWrite (RedHandleCalls calls, RedContext context, RedHandleArrayTimestamp arrayTimestamp, unsigned index);
REDGPU_DECLSPEC void REDGPU_API redArrayTimestampRead      (RedContext context, RedHandleGpu gpu, RedHandleArrayTimestamp arrayTimestamp, unsigned rangeFirst, unsigned rangeCount, void * out64BitTicksCounts, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData);

#ifdef __cplusplus
}
#endif

#pragma once

#include "redgpu.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*RedTypeProcedureDebugArrayPrintCallback)(RedContext context, RedHandleGpu gpu, unsigned debugArrayStructsCount, unsigned debugArrayStructsFloat4sCount, void * debugArrayData);

typedef struct RedContextOptionalSettingsDebugArray {
  RedContextOptionalSettings              settings;
  const void *                            next;
  const char *                            debugArrayHandleName;
  unsigned                                debugArrayStructsCount;
  unsigned                                debugArrayStructsFloat4sCount;
  RedTypeProcedureDebugArrayPrintCallback printCallback;
} RedContextOptionalSettingsDebugArray;

REDGPU_DECLSPEC RedHandleArray REDGPU_API redDebugArrayGetHandle(RedContext context, RedHandleGpu gpu, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData);
REDGPU_DECLSPEC void           REDGPU_API redDebugArrayCallPrint(RedContext context, RedHandleGpu gpu, RedHandleCalls calls, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData);

#ifdef __cplusplus
}
#endif

#if 0
clang -c C:/RedGpuSDK/redgpu.c
clang -c C:/RedGpuSDK/redgpu_2.c
clang main.c redgpu.o redgpu_2.o
#endif

#ifndef __cplusplus
#define REDGPU_DISABLE_NAMED_PARAMETERS
#endif

#include "C:/RedGpuSDK/redgpu_2.h"

#include "C:/RedGpuSDK/misc/np/np.h"
#include "C:/RedGpuSDK/misc/np/np_redgpu.h"
#include "C:/RedGpuSDK/misc/np/np_redgpu_2.h"

#include <stdlib.h> // For malloc, free
#include <stdio.h>  // For printf

void red2Crash(const char * error, const char * functionName, RedHandleGpu optionalGpuHandle, const char * optionalFile, int optionalLine) {
  printf("[REDGPU 2][Crash][%s:%d][%s] %s\n", optionalFile, optionalLine, functionName, error);
  exit(1);
}

int main() {
  RedContext redcontext = NULL;
  np(redCreateContext,
    "malloc", malloc,
    "free", free,
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
    "outContext", &redcontext,
    "outStatuses", NULL,
    "optionalFile", __FILE__,
    "optionalLine", __LINE__,
    "optionalUserData", NULL
  );

  REDGPU_2_EXPECTFL(redcontext != NULL);
  REDGPU_2_EXPECTFL(redcontext->gpusCount > 0);

  np(red2ExpectMinimumGuarantees,
    "gpuInfo", &redcontext->gpus[0],
    "optionalFile", __FILE__,
    "optionalLine", __LINE__
  );
  np(red2ExpectAllMemoryToBeCoherent,
    "gpuInfo", &redcontext->gpus[0],
    "optionalFile", __FILE__,
    "optionalLine", __LINE__
  );

  printf("Success: redcontext is created successfully and REDGPU 2 minimum guarantees checks are passed.\n");
}

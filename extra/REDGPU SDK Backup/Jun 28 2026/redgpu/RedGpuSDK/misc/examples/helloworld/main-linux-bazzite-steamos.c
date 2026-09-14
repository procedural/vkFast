#if 0
brew install libx11 vulkan-loader
gcc -c /home/linuxbrew/RedGpuSDK/redgpu.c -I/home/linuxbrew/.linuxbrew/include/ -I/home/linuxbrew/.linuxbrew/Cellar/xorgproto/2025.1/include/
gcc main-linux-bazzite-steamos.c redgpu.o -I/home/linuxbrew/.linuxbrew/include/ -I/home/linuxbrew/.linuxbrew/Cellar/xorgproto/2025.1/include/ /home/linuxbrew/.linuxbrew/lib/libvulkan.so
exit
#endif

#ifndef __cplusplus
#define REDGPU_DISABLE_NAMED_PARAMETERS
#endif

#include "/home/linuxbrew/RedGpuSDK/redgpu.h"

#include "/home/linuxbrew/RedGpuSDK/misc/np/np.h"
#include "/home/linuxbrew/RedGpuSDK/misc/np/np_redgpu.h"

#include <stdlib.h> // For malloc, free
#include <stdio.h>  // For printf

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
  if (redcontext == NULL) {
    printf("Error: redcontext == NULL, exiting.\n");
  } else {
    printf("Success: redcontext is created successfully.\n");
  }
}

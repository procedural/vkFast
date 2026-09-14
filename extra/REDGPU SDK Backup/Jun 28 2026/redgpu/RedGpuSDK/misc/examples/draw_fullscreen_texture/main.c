#define REDGPU_PROGRAM_PRESENT_IMAGES_COUNT               2
#define REDGPU_PROGRAM_PRESENT_VSYNC_MODE                 RED_PRESENT_VSYNC_MODE_ON
#define REDGPU_PROGRAM_INCONSISTENT_PRESENT_IMAGE_INDICES 1

#ifndef __cplusplus
#define REDGPU_DISABLE_NAMED_PARAMETERS
#endif

#include "C:/RedGpuSDK/redgpu.h"
#include "C:/RedGpuSDK/redgpu_wsi.h"

#include "C:/RedGpuSDK/misc/np/np.h"
#include "C:/RedGpuSDK/misc/np/np_redgpu.h"
#include "C:/RedGpuSDK/misc/np/np_redgpu_wsi.h"

#ifdef _WIN32
#define GLFW_INCLUDE_NONE
#define GLFW_EXPOSE_NATIVE_WIN32
#include "glfw-3.4.bin.WIN64/include/GLFW/glfw3.h" // https://github.com/glfw/glfw/releases/download/3.4/glfw-3.4.bin.WIN64.zip
#include "glfw-3.4.bin.WIN64/include/GLFW/glfw3native.h" // https://github.com/glfw/glfw/releases/download/3.4/glfw-3.4.bin.WIN64.zip
#pragma comment(lib, "glfw-3.4.bin.WIN64/lib-vc2019/glfw3_mt.lib") // https://github.com/glfw/glfw/releases/download/3.4/glfw-3.4.bin.WIN64.zip
#endif

#include <malloc.h> // For malloc, free
#include <stdio.h>  // For printf, fflush

#ifndef _countof
#define _countof(array) (sizeof(array) / sizeof((array)[0]))
#endif

typedef enum RedHelperMemoryType {
  RED_HELPER_MEMORY_TYPE_VRAM     = 0,
  RED_HELPER_MEMORY_TYPE_UPLOAD   = 1,
  RED_HELPER_MEMORY_TYPE_READBACK = 2,
} RedHelperMemoryType;

REDGPU_DECLSPEC unsigned REDGPU_API redHelperGetMemoryTypeIndex(const RedGpuInfo * gpuInfo, RedHelperMemoryType memoryType, unsigned memoryTypesSupported) {
  const unsigned        memoryTypesCount = gpuInfo->memoryTypesCount;
  const RedMemoryType * memoryTypes      = gpuInfo->memoryTypes;

  unsigned memoryTypeIsSupported[32];
  memoryTypeIsSupported[0]  = (memoryTypesSupported & REDGPU_B32(0000,0000,0000,0000,0000,0000,0000,0001)) == 0 ? 0 : 1;
  memoryTypeIsSupported[1]  = (memoryTypesSupported & REDGPU_B32(0000,0000,0000,0000,0000,0000,0000,0010)) == 0 ? 0 : 1;
  memoryTypeIsSupported[2]  = (memoryTypesSupported & REDGPU_B32(0000,0000,0000,0000,0000,0000,0000,0100)) == 0 ? 0 : 1;
  memoryTypeIsSupported[3]  = (memoryTypesSupported & REDGPU_B32(0000,0000,0000,0000,0000,0000,0000,1000)) == 0 ? 0 : 1;
  memoryTypeIsSupported[4]  = (memoryTypesSupported & REDGPU_B32(0000,0000,0000,0000,0000,0000,0001,0000)) == 0 ? 0 : 1;
  memoryTypeIsSupported[5]  = (memoryTypesSupported & REDGPU_B32(0000,0000,0000,0000,0000,0000,0010,0000)) == 0 ? 0 : 1;
  memoryTypeIsSupported[6]  = (memoryTypesSupported & REDGPU_B32(0000,0000,0000,0000,0000,0000,0100,0000)) == 0 ? 0 : 1;
  memoryTypeIsSupported[7]  = (memoryTypesSupported & REDGPU_B32(0000,0000,0000,0000,0000,0000,1000,0000)) == 0 ? 0 : 1;
  memoryTypeIsSupported[8]  = (memoryTypesSupported & REDGPU_B32(0000,0000,0000,0000,0000,0001,0000,0000)) == 0 ? 0 : 1;
  memoryTypeIsSupported[9]  = (memoryTypesSupported & REDGPU_B32(0000,0000,0000,0000,0000,0010,0000,0000)) == 0 ? 0 : 1;
  memoryTypeIsSupported[10] = (memoryTypesSupported & REDGPU_B32(0000,0000,0000,0000,0000,0100,0000,0000)) == 0 ? 0 : 1;
  memoryTypeIsSupported[11] = (memoryTypesSupported & REDGPU_B32(0000,0000,0000,0000,0000,1000,0000,0000)) == 0 ? 0 : 1;
  memoryTypeIsSupported[12] = (memoryTypesSupported & REDGPU_B32(0000,0000,0000,0000,0001,0000,0000,0000)) == 0 ? 0 : 1;
  memoryTypeIsSupported[13] = (memoryTypesSupported & REDGPU_B32(0000,0000,0000,0000,0010,0000,0000,0000)) == 0 ? 0 : 1;
  memoryTypeIsSupported[14] = (memoryTypesSupported & REDGPU_B32(0000,0000,0000,0000,0100,0000,0000,0000)) == 0 ? 0 : 1;
  memoryTypeIsSupported[15] = (memoryTypesSupported & REDGPU_B32(0000,0000,0000,0000,1000,0000,0000,0000)) == 0 ? 0 : 1;
  memoryTypeIsSupported[16] = (memoryTypesSupported & REDGPU_B32(0000,0000,0000,0001,0000,0000,0000,0000)) == 0 ? 0 : 1;
  memoryTypeIsSupported[17] = (memoryTypesSupported & REDGPU_B32(0000,0000,0000,0010,0000,0000,0000,0000)) == 0 ? 0 : 1;
  memoryTypeIsSupported[18] = (memoryTypesSupported & REDGPU_B32(0000,0000,0000,0100,0000,0000,0000,0000)) == 0 ? 0 : 1;
  memoryTypeIsSupported[19] = (memoryTypesSupported & REDGPU_B32(0000,0000,0000,1000,0000,0000,0000,0000)) == 0 ? 0 : 1;
  memoryTypeIsSupported[20] = (memoryTypesSupported & REDGPU_B32(0000,0000,0001,0000,0000,0000,0000,0000)) == 0 ? 0 : 1;
  memoryTypeIsSupported[21] = (memoryTypesSupported & REDGPU_B32(0000,0000,0010,0000,0000,0000,0000,0000)) == 0 ? 0 : 1;
  memoryTypeIsSupported[22] = (memoryTypesSupported & REDGPU_B32(0000,0000,0100,0000,0000,0000,0000,0000)) == 0 ? 0 : 1;
  memoryTypeIsSupported[23] = (memoryTypesSupported & REDGPU_B32(0000,0000,1000,0000,0000,0000,0000,0000)) == 0 ? 0 : 1;
  memoryTypeIsSupported[24] = (memoryTypesSupported & REDGPU_B32(0000,0001,0000,0000,0000,0000,0000,0000)) == 0 ? 0 : 1;
  memoryTypeIsSupported[25] = (memoryTypesSupported & REDGPU_B32(0000,0010,0000,0000,0000,0000,0000,0000)) == 0 ? 0 : 1;
  memoryTypeIsSupported[26] = (memoryTypesSupported & REDGPU_B32(0000,0100,0000,0000,0000,0000,0000,0000)) == 0 ? 0 : 1;
  memoryTypeIsSupported[27] = (memoryTypesSupported & REDGPU_B32(0000,1000,0000,0000,0000,0000,0000,0000)) == 0 ? 0 : 1;
  memoryTypeIsSupported[28] = (memoryTypesSupported & REDGPU_B32(0001,0000,0000,0000,0000,0000,0000,0000)) == 0 ? 0 : 1;
  memoryTypeIsSupported[29] = (memoryTypesSupported & REDGPU_B32(0010,0000,0000,0000,0000,0000,0000,0000)) == 0 ? 0 : 1;
  memoryTypeIsSupported[30] = (memoryTypesSupported & REDGPU_B32(0100,0000,0000,0000,0000,0000,0000,0000)) == 0 ? 0 : 1;
  memoryTypeIsSupported[31] = (memoryTypesSupported & REDGPU_B32(1000,0000,0000,0000,0000,0000,0000,0000)) == 0 ? 0 : 1;

  if (memoryType == RED_HELPER_MEMORY_TYPE_VRAM) {
    for (unsigned i = 0; i < memoryTypesCount; i += 1) {
      const RedMemoryType * type = &memoryTypes[i];
      if (type->isGpuVram == 1 && memoryTypeIsSupported[i] == 1) {
        return i;
      }
    }
  } else if (memoryType == RED_HELPER_MEMORY_TYPE_UPLOAD) {
    for (unsigned i = 0; i < memoryTypesCount; i += 1) {
      const RedMemoryType * type = &memoryTypes[i];
      if (type->isCpuMappable == 1 &&
          type->isCpuCoherent == 1 &&
          type->isCpuCached   == 0 && memoryTypeIsSupported[i] == 1)
      {
        return i;
      }
    }
    for (unsigned i = 0; i < memoryTypesCount; i += 1) {
      const RedMemoryType * type = &memoryTypes[i];
      if (type->isCpuMappable == 1 &&
          type->isCpuCoherent == 1 && memoryTypeIsSupported[i] == 1)
      {
        return i;
      }
    }
  } else if (memoryType == RED_HELPER_MEMORY_TYPE_READBACK) {
    for (unsigned i = 0; i < memoryTypesCount; i += 1) {
      const RedMemoryType * type = &memoryTypes[i];
      if (type->isCpuMappable == 1 &&
          type->isCpuCoherent == 1 &&
          type->isCpuCached   == 1 && memoryTypeIsSupported[i] == 1)
      {
        return i;
      }
    }
    for (unsigned i = 0; i < memoryTypesCount; i += 1) {
      const RedMemoryType * type = &memoryTypes[i];
      if (type->isCpuMappable == 1 &&
          type->isCpuCoherent == 1 && memoryTypeIsSupported[i] == 1)
      {
        return i;
      }
    }
  }

  return -1;
}

void redHelperImageSetStateUsable(RedContext context, RedHandleGpu gpu, RedHandleImage image, RedImagePartBitflags allParts, RedHandleQueue queueToSubmitImageStateChange, unsigned queueFamilyIndexToSubmitImageStateChange, RedStatuses * outStatuses, char * optionalFile, int optionalLine, void * optionalUserData) {
  RedCallProceduresAndAddresses callProceduresAndAddresses = {0};
  np(redGetCallProceduresAndAddresses,
    "context", context,
    "gpu", gpu,
    "outCallProceduresAndAddresses", &callProceduresAndAddresses,
    "outStatuses", outStatuses,
    "optionalFile", optionalFile,
    "optionalLine", optionalLine,
    "optionalUserData", optionalUserData
  );

  RedCalls calls = {0};
  np(redCreateCalls,
    "context", context,
    "gpu", gpu,
    "handleName", NULL,
    "queueFamilyIndex", queueFamilyIndexToSubmitImageStateChange,
    "outCalls", &calls,
    "outStatuses", outStatuses,
    "optionalFile", optionalFile,
    "optionalLine", optionalLine,
    "optionalUserData", optionalUserData
  );

  np(redCallsSet,
    "context", context,
    "gpu", gpu,
    "calls", calls.handle,
    "callsMemory", calls.memory,
    "callsReusable", calls.reusable,
    "outStatuses", outStatuses,
    "optionalFile", optionalFile,
    "optionalLine", optionalLine,
    "optionalUserData", optionalUserData
  );

  RedUsageImage imageUsage = {0};
  imageUsage.barrierSplit           = RED_BARRIER_SPLIT_NONE;
  imageUsage.oldAccessStages        = 0;
  imageUsage.newAccessStages        = 0;
  imageUsage.oldAccess              = 0;
  imageUsage.newAccess              = 0;
  imageUsage.oldState               = RED_STATE_UNUSABLE;
  imageUsage.newState               = RED_STATE_USABLE;
  imageUsage.queueFamilyIndexSource = -1;
  imageUsage.queueFamilyIndexTarget = -1;
  imageUsage.image                  = image;
  imageUsage.imageAllParts          = allParts;
  imageUsage.imageLevelsFirst       = 0;
  imageUsage.imageLevelsCount       = -1;
  imageUsage.imageLayersFirst       = 0;
  imageUsage.imageLayersCount       = -1;
  np(redCallUsageAliasOrderBarrier,
    "address", callProceduresAndAddresses.redCallUsageAliasOrderBarrier,
    "calls", calls.handle,
    "context", context,
    "arrayUsagesCount", 0,
    "arrayUsages", NULL,
    "imageUsagesCount", 1,
    "imageUsages", &imageUsage,
    "aliasesCount", 0,
    "aliases", NULL,
    "ordersCount", 0,
    "orders", NULL,
    "dependencyByRegion", 0
  );

  np(redCallsEnd,
    "context", context,
    "gpu", gpu,
    "calls", calls.handle,
    "callsMemory", calls.memory,
    "outStatuses", outStatuses,
    "optionalFile", optionalFile,
    "optionalLine", optionalLine,
    "optionalUserData", optionalUserData
  );

  RedHandleCpuSignal cpuSignal = NULL;
  np(redCreateCpuSignal,
    "context", context,
    "gpu", gpu,
    "handleName", NULL,
    "createSignaled", 0,
    "outCpuSignal", &cpuSignal,
    "outStatuses", outStatuses,
    "optionalFile", optionalFile,
    "optionalLine", optionalLine,
    "optionalUserData", optionalUserData
  );

  RedGpuTimeline timeline = {0};
  timeline.setTo4                            = 4;
  timeline.setTo0                            = 0;
  timeline.waitForAndUnsignalGpuSignalsCount = 0;
  timeline.waitForAndUnsignalGpuSignals      = NULL;
  timeline.setTo65536                        = NULL;
  timeline.callsCount                        = 1;
  timeline.calls                             = &calls.handle;
  timeline.signalGpuSignalsCount             = 0;
  timeline.signalGpuSignals                  = NULL;
  np(redQueueSubmit,
    "context", context,
    "gpu", gpu,
    "queue", queueToSubmitImageStateChange,
    "timelinesCount", 1,
    "timelines", &timeline,
    "signalCpuSignal", cpuSignal,
    "outStatuses", outStatuses,
    "optionalFile", optionalFile,
    "optionalLine", optionalLine,
    "optionalUserData", optionalUserData
  );

  np(redCpuSignalWait,
    "context", context,
    "gpu", gpu,
    "cpuSignalsCount", 1,
    "cpuSignals", &cpuSignal,
    "waitAll", 1,
    "outStatuses", outStatuses,
    "optionalFile", optionalFile,
    "optionalLine", optionalLine,
    "optionalUserData", optionalUserData
  );

  np(redDestroyCpuSignal,
    "context", context,
    "gpu", gpu,
    "cpuSignal", cpuSignal,
    "optionalFile", optionalFile,
    "optionalLine", optionalLine,
    "optionalUserData", optionalUserData
  );

  np(redDestroyCalls,
    "context", context,
    "gpu", gpu,
    "calls", calls.handle,
    "callsMemory", calls.memory,
    "optionalFile", optionalFile,
    "optionalLine", optionalLine,
    "optionalUserData", optionalUserData
  );
}

int main() {
  unsigned windowWidth  = 700;
  unsigned windowHeight = 700;

  glfwInit();
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
  GLFWwindow * window = glfwCreateWindow(windowWidth, windowHeight, "REDGPU Example: Texture", NULL, NULL);

#ifdef _WIN32
  unsigned extensions[] = {
    RED_SDK_EXTENSION_TREAT_ALL_MAPPABLE_MEMORY_AS_NON_COHERENT,
    RED_SDK_EXTENSION_DRIVER_PROPERTIES,
    RED_SDK_EXTENSION_RESOLVE_DEPTH_STENCIL,
    RED_SDK_EXTENSION_DEDICATE_MEMORY,
    RED_SDK_EXTENSION_PROCEDURE_PARAMETERS_HANDLES,
    RED_SDK_EXTENSION_BATCH_MEMORY_SET,
    RED_SDK_EXTENSION_RASTERIZATION_MODE,
    RED_SDK_EXTENSION_FORMAL_MEMORY_MODEL,
    RED_SDK_EXTENSION_WSI_WIN32
  };
#endif
  RedContext context = NULL;
  np(redCreateContext,
    "malloc", malloc,
    "free", free,
    "optionalMallocTagged", NULL,
    "optionalFreeTagged", NULL,
    "debugCallback", NULL,
    "sdkVersion", RED_SDK_VERSION_1_0_135,
    "sdkExtensionsCount", _countof(extensions),
    "sdkExtensions", extensions,
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

  RedHandleGpu       gpu              =  context->gpus[0].gpu;
  const RedGpuInfo * gpuInfo          = &context->gpus[0];
  RedHandleQueue     queue            =  context->gpus[0].queues[0];
  unsigned           queueFamilyIndex =  context->gpus[0].queuesFamilyIndex[0];

  RedCallProceduresAndAddresses callsPA = {0};
  np(redGetCallProceduresAndAddresses,
    "context", context,
    "gpu", gpu,
    "outCallProceduresAndAddresses", &callsPA,
    "outStatuses", NULL,
    "optionalFile", __FILE__,
    "optionalLine", __LINE__,
    "optionalUserData", NULL
  );

  RedHandleSampler imageSampler = NULL;
  np(redCreateSampler,
    "context", context,
    "gpu", gpu,
    "handleName", "imageSampler",
    "filteringMag", RED_SAMPLER_FILTERING_NEAREST,
    "filteringMin", RED_SAMPLER_FILTERING_NEAREST,
    "filteringMip", RED_SAMPLER_FILTERING_MIP_NEAREST,
    "behaviorOutsideTextureCoordinateU", RED_SAMPLER_BEHAVIOR_OUTSIDE_TEXTURE_COORDINATE_REPEAT,
    "behaviorOutsideTextureCoordinateV", RED_SAMPLER_BEHAVIOR_OUTSIDE_TEXTURE_COORDINATE_REPEAT,
    "behaviorOutsideTextureCoordinateW", RED_SAMPLER_BEHAVIOR_OUTSIDE_TEXTURE_COORDINATE_REPEAT,
    "mipLodBias", 0.f,
    "enableAnisotropy", 0,
    "maxAnisotropy", 0.f,
    "enableCompare", 0,
    "compareOp", RED_COMPARE_OP_NEVER,
    "minLod", 0.f,
    "maxLod", 1.f,
    "outSampler", &imageSampler,
    "outStatuses", NULL,
    "optionalFile", __FILE__,
    "optionalLine", __LINE__,
    "optionalUserData", NULL
  );

  typedef struct float4 {
    float r;
    float g;
    float b;
    float a;
  } float4;

  typedef struct Pixel {
    float4 color;
  } Pixel;

  uint64_t pixelsBytesCount = sizeof(Pixel [16 * 16]);

  RedArray pixels = {0};
  np(redCreateArray,
    "context", context,
    "gpu", gpu,
    "handleName", "pixels",
    "type", RED_ARRAY_TYPE_ARRAY_RO,
    "bytesCount", pixelsBytesCount,
    "structuredBufferElementBytesCount", 0,
    "restrictToAccess", 0,
    "initialQueueFamilyIndex", queueFamilyIndex,
    "dedicate", 0,
    "outArray", &pixels,
    "outStatuses", NULL,
    "optionalFile", __FILE__,
    "optionalLine", __LINE__,
    "optionalUserData", NULL
  );

  RedHandleMemory pixelsMemory = NULL;
  np(redMemoryAllocateMappable,
    "context", context,
    "gpu", gpu,
    "handleName", "pixelsMemory",
    "dedicate", 0,
    "array", pixels.handle,
    "arrayMemoryBytesCount", pixels.memoryBytesCount,
    "memoryTypeIndex", redHelperGetMemoryTypeIndex(gpuInfo, RED_HELPER_MEMORY_TYPE_UPLOAD, pixels.memoryTypesSupported),
    "memoryBitflags", 0,
    "outMemory", &pixelsMemory,
    "outStatuses", NULL,
    "optionalFile", __FILE__,
    "optionalLine", __LINE__,
    "optionalUserData", NULL
  );
    
  RedMemoryArray memoryArray = {0};
  memoryArray.setTo1000157000  = 1000157000;
  memoryArray.setTo0           = 0;
  memoryArray.array            = pixels.handle;
  memoryArray.memory           = pixelsMemory;
  memoryArray.memoryBytesFirst = 0;
  np(redMemorySet,
    "context", context,
    "gpu", gpu,
    "memoryArraysCount", 1,
    "memoryArrays", &memoryArray,
    "memoryImagesCount", 0,
    "memoryImages", NULL,
    "outStatuses", NULL,
    "optionalFile", __FILE__,
    "optionalLine", __LINE__,
    "optionalUserData", NULL
  );

  void * pointer = NULL;
  np(redMemoryMap,
    "context", context,
    "gpu", gpu,
    "mappableMemory", pixelsMemory,
    "mappableMemoryBytesFirst", 0,
    "mappableMemoryBytesCount", pixelsBytesCount,
    "outVolatilePointer", &pointer,
    "outStatuses", NULL,
    "optionalFile", __FILE__,
    "optionalLine", __LINE__,
    "optionalUserData", NULL
  );
  Pixel * volatile pixelsPointer = (Pixel * volatile)pointer;

  Pixel data[16 * 16] = {
    {1.f,1.f,1.f,1.f},{1.f,1.f,1.f,1.f},{1.f,1.f,1.f,1.f},{1.f,1.f,1.f,1.f},{1.f,1.f,1.f,1.f},{1.f,1.f,1.f,1.f},{1.f,1.f,1.f,1.f},{1.f,1.f,1.f,1.f},  {0.f,0.f,0.f,0.f},{0.f,0.f,0.f,0.f},{0.f,0.f,0.f,0.f},{0.f,0.f,0.f,0.f},{0.f,0.f,0.f,0.f},{0.f,0.f,0.f,0.f},{0.f,0.f,0.f,0.f},{0.f,0.f,0.f,0.f},
    {1.f,1.f,1.f,1.f},{1.f,0.f,0.f,1.f},{1.f,1.f,1.f,1.f},{1.f,1.f,1.f,1.f},{1.f,1.f,1.f,1.f},{1.f,1.f,1.f,1.f},{0.f,1.f,0.f,1.f},{1.f,1.f,1.f,1.f},  {0.f,0.f,0.f,0.f},{0.f,0.f,0.f,0.f},{0.f,0.f,0.f,0.f},{0.f,0.f,0.f,0.f},{0.f,0.f,0.f,0.f},{0.f,0.f,0.f,0.f},{0.f,0.f,0.f,0.f},{0.f,0.f,0.f,0.f},
    {1.f,1.f,1.f,1.f},{1.f,1.f,1.f,1.f},{1.f,1.f,1.f,1.f},{1.f,1.f,1.f,1.f},{1.f,1.f,1.f,1.f},{1.f,1.f,1.f,1.f},{1.f,1.f,1.f,1.f},{1.f,1.f,1.f,1.f},  {0.f,0.f,0.f,0.f},{0.f,0.f,0.f,0.f},{0.f,0.f,0.f,0.f},{0.f,0.f,0.f,0.f},{0.f,0.f,0.f,0.f},{0.f,0.f,0.f,0.f},{0.f,0.f,0.f,0.f},{0.f,0.f,0.f,0.f},
    {1.f,1.f,1.f,1.f},{1.f,1.f,1.f,1.f},{1.f,1.f,1.f,1.f},{1.f,1.f,1.f,1.f},{1.f,1.f,1.f,1.f},{1.f,1.f,1.f,1.f},{1.f,1.f,1.f,1.f},{1.f,1.f,1.f,1.f},  {0.f,0.f,0.f,0.f},{0.f,0.f,0.f,0.f},{0.f,0.f,0.f,0.f},{0.f,0.f,0.f,0.f},{0.f,0.f,0.f,0.f},{0.f,0.f,0.f,0.f},{0.f,0.f,0.f,0.f},{0.f,0.f,0.f,0.f},
    {1.f,1.f,1.f,1.f},{1.f,1.f,1.f,1.f},{1.f,1.f,1.f,1.f},{1.f,1.f,1.f,1.f},{1.f,1.f,1.f,1.f},{1.f,1.f,1.f,1.f},{1.f,1.f,1.f,1.f},{1.f,1.f,1.f,1.f},  {0.f,0.f,0.f,0.f},{0.f,0.f,0.f,0.f},{0.f,0.f,0.f,0.f},{0.f,0.f,0.f,0.f},{0.f,0.f,0.f,0.f},{0.f,0.f,0.f,0.f},{0.f,0.f,0.f,0.f},{0.f,0.f,0.f,0.f},
    {1.f,1.f,1.f,1.f},{1.f,1.f,1.f,1.f},{1.f,1.f,1.f,1.f},{1.f,1.f,1.f,1.f},{1.f,1.f,1.f,1.f},{1.f,1.f,1.f,1.f},{1.f,1.f,1.f,1.f},{1.f,1.f,1.f,1.f},  {0.f,0.f,0.f,0.f},{0.f,0.f,0.f,0.f},{0.f,0.f,0.f,0.f},{0.f,0.f,0.f,0.f},{0.f,0.f,0.f,0.f},{0.f,0.f,0.f,0.f},{0.f,0.f,0.f,0.f},{0.f,0.f,0.f,0.f},
    {1.f,1.f,1.f,1.f},{0.f,0.f,1.f,1.f},{1.f,1.f,1.f,1.f},{1.f,1.f,1.f,1.f},{1.f,1.f,1.f,1.f},{1.f,1.f,1.f,1.f},{1.f,1.f,1.f,1.f},{1.f,1.f,1.f,1.f},  {0.f,0.f,0.f,0.f},{0.f,0.f,0.f,0.f},{0.f,0.f,0.f,0.f},{0.f,0.f,0.f,0.f},{0.f,0.f,0.f,0.f},{0.f,0.f,0.f,0.f},{0.f,0.f,0.f,0.f},{0.f,0.f,0.f,0.f},
    {1.f,1.f,1.f,1.f},{1.f,1.f,1.f,1.f},{1.f,1.f,1.f,1.f},{1.f,1.f,1.f,1.f},{1.f,1.f,1.f,1.f},{1.f,1.f,1.f,1.f},{1.f,1.f,1.f,1.f},{1.f,1.f,1.f,1.f},  {0.f,0.f,0.f,0.f},{0.f,0.f,0.f,0.f},{0.f,0.f,0.f,0.f},{0.f,0.f,0.f,0.f},{0.f,0.f,0.f,0.f},{0.f,0.f,0.f,0.f},{0.f,0.f,0.f,0.f},{0.f,0.f,0.f,0.f},
  };
  for (unsigned i = 0; i < 16 * 16; i += 1) {
    pixelsPointer[i] = data[i];
  }

  RedMappableMemoryRange memoryRangeToFlush = {0};
  memoryRangeToFlush.setTo6                        = 6;
  memoryRangeToFlush.setTo0                        = 0;
  memoryRangeToFlush.mappableMemory                = pixelsMemory;
  memoryRangeToFlush.mappableMemoryRangeBytesFirst = 0;
  memoryRangeToFlush.mappableMemoryRangeBytesCount = sizeof(data);
  np(redMemoryNonCoherentFlush,
    "context", context,
    "gpu", gpu,
    "mappableMemoryRangesCount", 1,
    "mappableMemoryRanges", &memoryRangeToFlush,
    "outStatuses", NULL,
    "optionalFile", __FILE__,
    "optionalLine", __LINE__,
    "optionalUserData", NULL
  );

  RedImage image = {0};
  np(redCreateImage,
    "context", context,
    "gpu", gpu,
    "handleName", "image",
    "dimensions", RED_IMAGE_DIMENSIONS_2D,
    "format", RED_FORMAT_RGBA_32_32_32_32_FLOAT,
    "width", 8,
    "height", 8,
    "depth", 1,
    "levelsCount", 1,
    "layersCount", 1,
    "multisampleCount", RED_MULTISAMPLE_COUNT_BITFLAG_1,
    "restrictToAccess", 0,
    "initialQueueFamilyIndex", queueFamilyIndex,
    "dedicate", 0,
    "outImage", &image,
    "outStatuses", NULL,
    "optionalFile", __FILE__,
    "optionalLine", __LINE__,
    "optionalUserData", NULL
  );
    
  RedHandleMemory imageMemory = NULL;
  np(redMemoryAllocate,
    "context", context,
    "gpu", gpu,
    "handleName", "imageMemory",
    "bytesCount", image.memoryBytesCount,
    "memoryTypeIndex", redHelperGetMemoryTypeIndex(gpuInfo, RED_HELPER_MEMORY_TYPE_VRAM, image.memoryTypesSupported),
    "dedicateToArray", NULL,
    "dedicateToImage", NULL,
    "memoryBitflags", 0,
    "outMemory", &imageMemory,
    "outStatuses", NULL,
    "optionalFile", __FILE__,
    "optionalLine", __LINE__,
    "optionalUserData", NULL
  );

  RedMemoryImage memoryImage = {0};
  memoryImage.setTo1000157001  = 1000157001;
  memoryImage.setTo0           = 0;
  memoryImage.image            = image.handle;
  memoryImage.memory           = imageMemory;
  memoryImage.memoryBytesFirst = 0;
  np(redMemorySet,
    "context", context,
    "gpu", gpu,
    "memoryArraysCount", 0,
    "memoryArrays", NULL,
    "memoryImagesCount", 1,
    "memoryImages", &memoryImage,
    "outStatuses", NULL,
    "optionalFile", __FILE__,
    "optionalLine", __LINE__,
    "optionalUserData", NULL
  );

  RedHandleTexture imageTexture = NULL;
  np(redCreateTexture,
    "context", context,
    "gpu", gpu,
    "handleName", "imageTexture",
    "image", image.handle,
    "parts", RED_IMAGE_PART_BITFLAG_COLOR,
    "dimensions", RED_TEXTURE_DIMENSIONS_2D,
    "format", RED_FORMAT_RGBA_32_32_32_32_FLOAT,
    "levelsFirst", 0,
    "levelsCount", 1,
    "layersFirst", 0,
    "layersCount", 1,
    "restrictToAccess", 0,
    "outTexture", &imageTexture,
    "outStatuses", NULL,
    "optionalFile", __FILE__,
    "optionalLine", __LINE__,
    "optionalUserData", NULL
  );

  redHelperImageSetStateUsable(context, gpu, image.handle, RED_IMAGE_PART_BITFLAG_COLOR, queue, queueFamilyIndex, NULL, __FILE__, __LINE__, NULL);

  RedHandleSurface  surface                                               = NULL;
  RedHandlePresent  present                                               = NULL;
  RedAccessBitflags presentImagesAccess                                   = RED_ACCESS_BITFLAG_OUTPUT_COLOR_W;
  RedHandleImage    presentImages   [REDGPU_PROGRAM_PRESENT_IMAGES_COUNT] = {0};
  RedHandleTexture  presentTextures [REDGPU_PROGRAM_PRESENT_IMAGES_COUNT] = {0};

#ifdef _WIN32
  np(redCreateSurfaceWin32,
    "context", context,
    "gpu", gpu,
    "handleName", "surface",
    "win32Hinstance", GetModuleHandle(NULL),
    "win32Hwnd", glfwGetWin32Window(window),
    "outSurface", &surface,
    "outStatuses", NULL,
    "optionalFile", __FILE__,
    "optionalLine", __LINE__,
    "optionalUserData", NULL
  );
#endif
  RedQueueFamilyIndexGetSupportsPresentOnSurface queueFamilyIndexSupportsPresentOnSurface = {0};
  queueFamilyIndexSupportsPresentOnSurface.surface                                     = surface;
  queueFamilyIndexSupportsPresentOnSurface.outQueueFamilyIndexSupportsPresentOnSurface = 0;
  np(redQueueFamilyIndexGetSupportsPresent,
    "context", context,
    "gpu", gpu,
    "queueFamilyIndex", queueFamilyIndex,
    "supportsPresentOnWin32", NULL,
    "supportsPresentOnXlib", NULL,
    "supportsPresentOnXcb", NULL,
    "supportsPresentOnSurface", &queueFamilyIndexSupportsPresentOnSurface,
    "outStatuses", NULL,
    "optionalFile", __FILE__,
    "optionalLine", __LINE__,
    "optionalUserData", NULL
  );
  if (queueFamilyIndexSupportsPresentOnSurface.outQueueFamilyIndexSupportsPresentOnSurface == 0) {
    printf("Error: queueFamilyIndexSupportsPresentOnSurface.outQueueFamilyIndexSupportsPresentOnSurface == 0\n");
    fflush(stdout);
  }

  RedSurfaceCurrentPropertiesAndPresentLimits surfaceCurrentPropertiesAndPresentLimits = {0};
  np(redSurfaceGetCurrentPropertiesAndPresentLimits,
    "context", context,
    "gpu", gpu,
    "surface", surface,
    "outSurfaceCurrentPropertiesAndPresentLimits", &surfaceCurrentPropertiesAndPresentLimits,
    "outStatuses", NULL,
    "optionalFile", __FILE__,
    "optionalLine", __LINE__,
    "optionalUserData", NULL
  );

  if (surfaceCurrentPropertiesAndPresentLimits.currentSurfaceWidth  != -1) { windowWidth  = surfaceCurrentPropertiesAndPresentLimits.currentSurfaceWidth;  }
  if (surfaceCurrentPropertiesAndPresentLimits.currentSurfaceHeight != -1) { windowHeight = surfaceCurrentPropertiesAndPresentLimits.currentSurfaceHeight; }

  np(redCreatePresent,
    "context", context,
    "gpu", gpu,
    "queue", queue,
    "handleName", "present",
    "surface", surface,
    "imagesCount", REDGPU_PROGRAM_PRESENT_IMAGES_COUNT,
    "imagesWidth", windowWidth,
    "imagesHeight", windowHeight,
    "imagesLayersCount", 1,
    "imagesRestrictToAccess", presentImagesAccess,
    "transform", RED_SURFACE_TRANSFORM_BITFLAG_IDENTITY,
    "compositeAlpha", RED_SURFACE_COMPOSITE_ALPHA_BITFLAG_OPAQUE,
    "vsyncMode", REDGPU_PROGRAM_PRESENT_VSYNC_MODE,
    "clipped", 0,
    "discardAfterPresent", 0,
    "oldPresent", NULL,
    "outPresent", &present,
    "outImages", presentImages,
    "outTextures", presentTextures,
    "outStatuses", NULL,
    "optionalFile", __FILE__,
    "optionalLine", __LINE__,
    "optionalUserData", NULL
  );

  RedHandleGpuSignal gpusigPresent [REDGPU_PROGRAM_PRESENT_IMAGES_COUNT] = {0};
  RedHandleCpuSignal cpusig        [REDGPU_PROGRAM_PRESENT_IMAGES_COUNT] = {0};
  RedHandleGpuSignal gpusig        [REDGPU_PROGRAM_PRESENT_IMAGES_COUNT] = {0};
  for (unsigned i = 0; i < REDGPU_PROGRAM_PRESENT_IMAGES_COUNT; i += 1) {
    np(redCreateGpuSignal,
      "context", context,
      "gpu", gpu,
      "handleName", "gpusigPresent",
      "outGpuSignal", &gpusigPresent[i],
      "outStatuses", NULL,
      "optionalFile", __FILE__,
      "optionalLine", __LINE__,
      "optionalUserData", NULL
    );
    np(redCreateCpuSignal,
      "context", context,
      "gpu", gpu,
      "handleName", "cpusig",
      "createSignaled", 1,
      "outCpuSignal", &cpusig[i],
      "outStatuses", NULL,
      "optionalFile", __FILE__,
      "optionalLine", __LINE__,
      "optionalUserData", NULL
    );
    np(redCreateGpuSignal,
      "context", context,
      "gpu", gpu,
      "handleName", "gpusig",
      "outGpuSignal", &gpusig[i],
      "outStatuses", NULL,
      "optionalFile", __FILE__,
      "optionalLine", __LINE__,
      "optionalUserData", NULL
    );
  }

  RedCalls calls[REDGPU_PROGRAM_PRESENT_IMAGES_COUNT] = {0};
  for (unsigned i = 0; i < REDGPU_PROGRAM_PRESENT_IMAGES_COUNT; i += 1) {
    np(redCreateCalls,
      "context", context,
      "gpu", gpu,
      "handleName", "calls",
      "queueFamilyIndex", queueFamilyIndex,
      "outCalls", &calls[i],
      "outStatuses", NULL,
      "optionalFile", __FILE__,
      "optionalLine", __LINE__,
      "optionalUserData", NULL
    );
  }

  RedHandleOutputDeclaration outputDeclaration = NULL;
  {
    RedOutputDeclarationMembers members = {0};
    members.depthStencilEnable                        = 0;
    members.depthStencilFormat                        = RED_FORMAT_UNDEFINED;
    members.depthStencilMultisampleCount              = RED_MULTISAMPLE_COUNT_BITFLAG_1;
    members.depthStencilDepthSetProcedureOutputOp     = RED_SET_PROCEDURE_OUTPUT_OP_PRESERVE;
    members.depthStencilDepthEndProcedureOutputOp     = RED_END_PROCEDURE_OUTPUT_OP_PRESERVE;
    members.depthStencilStencilSetProcedureOutputOp   = RED_SET_PROCEDURE_OUTPUT_OP_PRESERVE;
    members.depthStencilStencilEndProcedureOutputOp   = RED_END_PROCEDURE_OUTPUT_OP_PRESERVE;
    members.depthStencilSharesMemoryWithAnotherMember = 0;
    members.colorsCount                               = 1;
    members.colorsFormat[0]                           = RED_FORMAT_PRESENT_BGRA_8_8_8_8_UINT_TO_FLOAT_0_1;
    members.colorsFormat[1]                           = RED_FORMAT_PRESENT_BGRA_8_8_8_8_UINT_TO_FLOAT_0_1;
    members.colorsFormat[2]                           = RED_FORMAT_PRESENT_BGRA_8_8_8_8_UINT_TO_FLOAT_0_1;
    members.colorsFormat[3]                           = RED_FORMAT_PRESENT_BGRA_8_8_8_8_UINT_TO_FLOAT_0_1;
    members.colorsFormat[4]                           = RED_FORMAT_PRESENT_BGRA_8_8_8_8_UINT_TO_FLOAT_0_1;
    members.colorsFormat[5]                           = RED_FORMAT_PRESENT_BGRA_8_8_8_8_UINT_TO_FLOAT_0_1;
    members.colorsFormat[6]                           = RED_FORMAT_PRESENT_BGRA_8_8_8_8_UINT_TO_FLOAT_0_1;
    members.colorsFormat[7]                           = RED_FORMAT_PRESENT_BGRA_8_8_8_8_UINT_TO_FLOAT_0_1;
    members.colorsMultisampleCount[0]                 = RED_MULTISAMPLE_COUNT_BITFLAG_1;
    members.colorsMultisampleCount[1]                 = RED_MULTISAMPLE_COUNT_BITFLAG_1;
    members.colorsMultisampleCount[2]                 = RED_MULTISAMPLE_COUNT_BITFLAG_1;
    members.colorsMultisampleCount[3]                 = RED_MULTISAMPLE_COUNT_BITFLAG_1;
    members.colorsMultisampleCount[4]                 = RED_MULTISAMPLE_COUNT_BITFLAG_1;
    members.colorsMultisampleCount[5]                 = RED_MULTISAMPLE_COUNT_BITFLAG_1;
    members.colorsMultisampleCount[6]                 = RED_MULTISAMPLE_COUNT_BITFLAG_1;
    members.colorsMultisampleCount[7]                 = RED_MULTISAMPLE_COUNT_BITFLAG_1;
    members.colorsSetProcedureOutputOp[0]             = RED_SET_PROCEDURE_OUTPUT_OP_PRESERVE;
    members.colorsSetProcedureOutputOp[1]             = RED_SET_PROCEDURE_OUTPUT_OP_PRESERVE;
    members.colorsSetProcedureOutputOp[2]             = RED_SET_PROCEDURE_OUTPUT_OP_PRESERVE;
    members.colorsSetProcedureOutputOp[3]             = RED_SET_PROCEDURE_OUTPUT_OP_PRESERVE;
    members.colorsSetProcedureOutputOp[4]             = RED_SET_PROCEDURE_OUTPUT_OP_PRESERVE;
    members.colorsSetProcedureOutputOp[5]             = RED_SET_PROCEDURE_OUTPUT_OP_PRESERVE;
    members.colorsSetProcedureOutputOp[6]             = RED_SET_PROCEDURE_OUTPUT_OP_PRESERVE;
    members.colorsSetProcedureOutputOp[7]             = RED_SET_PROCEDURE_OUTPUT_OP_PRESERVE;
    members.colorsEndProcedureOutputOp[0]             = RED_END_PROCEDURE_OUTPUT_OP_PRESERVE;
    members.colorsEndProcedureOutputOp[1]             = RED_END_PROCEDURE_OUTPUT_OP_PRESERVE;
    members.colorsEndProcedureOutputOp[2]             = RED_END_PROCEDURE_OUTPUT_OP_PRESERVE;
    members.colorsEndProcedureOutputOp[3]             = RED_END_PROCEDURE_OUTPUT_OP_PRESERVE;
    members.colorsEndProcedureOutputOp[4]             = RED_END_PROCEDURE_OUTPUT_OP_PRESERVE;
    members.colorsEndProcedureOutputOp[5]             = RED_END_PROCEDURE_OUTPUT_OP_PRESERVE;
    members.colorsEndProcedureOutputOp[6]             = RED_END_PROCEDURE_OUTPUT_OP_PRESERVE;
    members.colorsEndProcedureOutputOp[7]             = RED_END_PROCEDURE_OUTPUT_OP_PRESERVE;
    members.colorsSharesMemoryWithAnotherMember[0]    = 0;
    members.colorsSharesMemoryWithAnotherMember[1]    = 0;
    members.colorsSharesMemoryWithAnotherMember[2]    = 0;
    members.colorsSharesMemoryWithAnotherMember[3]    = 0;
    members.colorsSharesMemoryWithAnotherMember[4]    = 0;
    members.colorsSharesMemoryWithAnotherMember[5]    = 0;
    members.colorsSharesMemoryWithAnotherMember[6]    = 0;
    members.colorsSharesMemoryWithAnotherMember[7]    = 0;
    np(redCreateOutputDeclaration,
      "context", context,
      "gpu", gpu,
      "handleName", "outputDeclaration",
      "outputDeclarationMembers", &members,
      "outputDeclarationMembersResolveSources", NULL,
      "dependencyByRegion", 0,
      "dependencyByRegionAllowUsageAliasOrderBarriers", 0,
      "outOutputDeclaration", &outputDeclaration,
      "outStatuses", NULL,
      "optionalFile", __FILE__,
      "optionalLine", __LINE__,
      "optionalUserData", NULL
    );
  }

  RedOutput rendertargets[REDGPU_PROGRAM_PRESENT_IMAGES_COUNT] = {0};
  for (int i = 0; i < REDGPU_PROGRAM_PRESENT_IMAGES_COUNT; i += 1) {
    RedOutputMembers outputMembers = {0};
    outputMembers.depthStencil     = NULL;
    outputMembers.colorsCount      = 1;
    outputMembers.colors[0]        = presentTextures[i];
    outputMembers.colors[1]        = NULL;
    outputMembers.colors[2]        = NULL;
    outputMembers.colors[3]        = NULL;
    outputMembers.colors[4]        = NULL;
    outputMembers.colors[5]        = NULL;
    outputMembers.colors[6]        = NULL;
    outputMembers.colors[7]        = NULL;
    np(redCreateOutput,
      "context", context,
      "gpu", gpu,
      "handleName", NULL,
      "outputDeclaration", outputDeclaration,
      "outputMembers", &outputMembers,
      "outputMembersResolveTargets", NULL,
      "width", windowWidth,
      "height", windowHeight,
      "outOutput", &rendertargets[i],
      "outStatuses", NULL,
      "optionalFile", __FILE__,
      "optionalLine", __LINE__,
      "optionalUserData", NULL
    );
  }

  RedStructDeclarationMember structDeclarationMembers[2] = {0};
  structDeclarationMembers[0].slot            = 0;
  structDeclarationMembers[0].type            = RED_STRUCT_MEMBER_TYPE_SAMPLER;
  structDeclarationMembers[0].count           = 1;
  structDeclarationMembers[0].visibleToStages = RED_VISIBLE_TO_STAGE_BITFLAG_VERTEX | RED_VISIBLE_TO_STAGE_BITFLAG_FRAGMENT;
  structDeclarationMembers[0].inlineSampler   = &imageSampler;
  structDeclarationMembers[1].slot            = 1;
  structDeclarationMembers[1].type            = RED_STRUCT_MEMBER_TYPE_TEXTURE_RO;
  structDeclarationMembers[1].count           = 1;
  structDeclarationMembers[1].visibleToStages = RED_VISIBLE_TO_STAGE_BITFLAG_VERTEX | RED_VISIBLE_TO_STAGE_BITFLAG_FRAGMENT;
  structDeclarationMembers[1].inlineSampler   = NULL;
  RedHandleStructDeclaration structDeclaration = NULL;
  np(redCreateStructDeclaration,
    "context", context,
    "gpu", gpu,
    "handleName", "structDeclaration",
    "structDeclarationMembersCount", _countof(structDeclarationMembers),
    "structDeclarationMembers", structDeclarationMembers,
    "structDeclarationMembersArrayROCount", 0,
    "structDeclarationMembersArrayRO", NULL,
    "procedureParametersHandlesDeclaration", 0,
    "outStructDeclaration", &structDeclaration,
    "outStatuses", NULL,
    "optionalFile", __FILE__,
    "optionalLine", __LINE__,
    "optionalUserData", NULL
  );

  RedProcedureParametersDeclaration procedureParametersDeclaration = {0};
  procedureParametersDeclaration.variablesSlot              = 0;
  procedureParametersDeclaration.variablesVisibleToStages   = 0;
  procedureParametersDeclaration.variablesBytesCount        = 0;
  procedureParametersDeclaration.structsDeclarationsCount   = 1;
  procedureParametersDeclaration.structsDeclarations[0]     = structDeclaration;
  procedureParametersDeclaration.structsDeclarations[1]     = NULL;
  procedureParametersDeclaration.structsDeclarations[2]     = NULL;
  procedureParametersDeclaration.structsDeclarations[3]     = NULL;
  procedureParametersDeclaration.structsDeclarations[4]     = NULL;
  procedureParametersDeclaration.structsDeclarations[5]     = NULL;
  procedureParametersDeclaration.structsDeclarations[6]     = NULL;
  procedureParametersDeclaration.handlesDeclaration         = NULL;
  RedHandleProcedureParameters procedureParameters = NULL;
  np(redCreateProcedureParameters,
    "context", context,
    "gpu", gpu,
    "handleName", "procedureParameters",
    "procedureParametersDeclaration", &procedureParametersDeclaration,
    "outProcedureParameters", &procedureParameters,
    "outStatuses", NULL,
    "optionalFile", __FILE__,
    "optionalLine", __LINE__,
    "optionalUserData", NULL
  );

  RedHandleStructsMemory structsMemory = NULL;
  np(redStructsMemoryAllocateWithInlineSamplers,
    "context", context,
    "gpu", gpu,
    "handleName", "structsMemory",
    "maxStructsCount", 1,
    "maxStructsMembersOfTypeArrayROConstantCount", 0,
    "maxStructsMembersOfTypeArrayROOrArrayRWCount", 0,
    "maxStructsMembersOfTypeTextureROCount", 1,
    "maxStructsMembersOfTypeTextureRWCount", 0,
    "maxStructsMembersOfTypeInlineSamplerCount", 1,
    "outStructsMemory", &structsMemory,
    "outStatuses", NULL,
    "optionalFile", __FILE__,
    "optionalLine", __LINE__,
    "optionalUserData", NULL
  );

  const char * structsHandleNames[1] = {0};
  structsHandleNames[0] = "structs[0]";
  RedHandleStruct structs[1] = {0};
  np(redStructsMemorySuballocateStructs,
    "context", context,
    "gpu", gpu,
    "handleNames", structsHandleNames,
    "structsMemory", structsMemory,
    "structsDeclarationsCount", 1,
    "structsDeclarations", &structDeclaration,
    "outStructs", structs,
    "outStatuses", NULL,
    "optionalFile", __FILE__,
    "optionalLine", __LINE__,
    "optionalUserData", NULL
  );

  RedStructMemberTexture texture = {0};
  texture.sampler = NULL;
  texture.texture = imageTexture;
  texture.setTo1  = 1;
  RedStructMember structMembers[1] = {0};
  structMembers[0].setTo35   = 35;
  structMembers[0].setTo0    = 0;
  structMembers[0].structure = structs[0];
  structMembers[0].slot      = 1;
  structMembers[0].first     = 0;
  structMembers[0].count     = 1;
  structMembers[0].type      = RED_STRUCT_MEMBER_TYPE_TEXTURE_RO;
  structMembers[0].textures  = &texture;
  structMembers[0].arrays    = NULL;
  structMembers[0].setTo00   = 0;
  np(redStructsSet,
    "context", context,
    "gpu", gpu,
    "structsMembersCount", _countof(structMembers),
    "structsMembers", structMembers,
    "optionalFile", __FILE__,
    "optionalLine", __LINE__,
    "optionalUserData", NULL
  );

  RedHandleGpuCode gpuCodeVS = NULL;
  RedHandleGpuCode gpuCodeFS = NULL;
  {
    const char * filepath         = "fullscreen_triangle_vs.ir";
    long         irCodeBytesCount = 0;
    void *       irCode           = NULL;
    FILE *       irCodeFd         = fopen(filepath, "rb");

    if (irCodeFd != NULL) {
      fseek(irCodeFd, 0, SEEK_END);
      irCodeBytesCount = ftell(irCodeFd);
      fseek(irCodeFd, 0, SEEK_SET);
      irCode = malloc(irCodeBytesCount);
      fread(irCode, 1, irCodeBytesCount, irCodeFd);
      fclose(irCodeFd);
    } else {
      printf("[REDGPU]: gpuCodeVS::filepath(%s): fopen returned NULL\n", filepath);
      fflush(stdout);
    }

    np(redCreateGpuCode,
      "context", context,
      "gpu", gpu,
      "handleName", "gpuCodeVS",
      "irBytesCount", irCodeBytesCount,
      "ir", irCode,
      "outGpuCode", &gpuCodeVS,
      "outStatuses", NULL,
      "optionalFile", __FILE__,
      "optionalLine", __LINE__,
      "optionalUserData", NULL
    );

    if (irCode != NULL) {
      free(irCode);
    }
  }
  {
    const char * filepath         = "fullscreen_triangle_fs.ir";
    long         irCodeBytesCount = 0;
    void *       irCode           = NULL;
    FILE *       irCodeFd         = fopen(filepath, "rb");

    if (irCodeFd != NULL) {
      fseek(irCodeFd, 0, SEEK_END);
      irCodeBytesCount = ftell(irCodeFd);
      fseek(irCodeFd, 0, SEEK_SET);
      irCode = malloc(irCodeBytesCount);
      fread(irCode, 1, irCodeBytesCount, irCodeFd);
      fclose(irCodeFd);
    } else {
      printf("[REDGPU]: gpuCodeFS::filepath(%s): fopen returned NULL\n", filepath);
      fflush(stdout);
    }

    np(redCreateGpuCode,
      "context", context,
      "gpu", gpu,
      "handleName", "gpuCodeFS",
      "irBytesCount", irCodeBytesCount,
      "ir", irCode,
      "outGpuCode", &gpuCodeFS,
      "outStatuses", NULL,
      "optionalFile", __FILE__,
      "optionalLine", __LINE__,
      "optionalUserData", NULL
    );

    if (irCode != NULL) {
      free(irCode);
    }
  }

  RedProcedureState state = {0};
  state.inputAssemblyTopology                          = RED_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
  state.inputAssemblyPrimitiveRestartEnable            = 0;
  state.viewportDynamic                                = 0;
  state.viewportStaticX                                = 0;
  state.viewportStaticY                                = 0;
  state.viewportStaticWidth                            = windowWidth;
  state.viewportStaticHeight                           = windowHeight;
  state.viewportStaticDepthMin                         = 0;
  state.viewportStaticDepthMax                         = 1;
  state.scissorDynamic                                 = 0;
  state.scissorStaticX                                 = 0;
  state.scissorStaticY                                 = 0;
  state.scissorStaticWidth                             = windowWidth;
  state.scissorStaticHeight                            = windowHeight;
  state.rasterizationDepthClampEnable                  = 0;
  state.rasterizationDiscardAllPrimitivesEnable        = 0;
  state.rasterizationCullMode                          = RED_CULL_MODE_BACK;
  state.rasterizationFrontFace                         = RED_FRONT_FACE_CLOCKWISE;
  state.rasterizationDepthBiasEnable                   = 0;
  state.rasterizationDepthBiasDynamic                  = 0;
  state.rasterizationDepthBiasStaticConstantFactor     = 0;
  state.rasterizationDepthBiasStaticClamp              = 0;
  state.rasterizationDepthBiasStaticSlopeFactor        = 0;
  state.multisampleCount                               = RED_MULTISAMPLE_COUNT_BITFLAG_1;
  state.multisampleSampleMask                          = NULL;
  state.multisampleSampleShadingEnable                 = 0;
  state.multisampleSampleShadingMin                    = 0;
  state.multisampleAlphaToCoverageEnable               = 1;
  state.multisampleAlphaToOneEnable                    = 0;
  state.depthTestEnable                                = 0;
  state.depthTestDepthWriteEnable                      = 0;
  state.depthTestDepthCompareOp                        = RED_COMPARE_OP_GREATER_OR_EQUAL;
  state.depthTestBoundsTestEnable                      = 0;
  state.depthTestBoundsTestDynamic                     = 0;
  state.depthTestBoundsTestStaticMin                   = 0;
  state.depthTestBoundsTestStaticMax                   = 1;
  state.stencilTestEnable                              = 0;
  state.stencilTestFrontAndBackDynamicCompareMask      = 0;
  state.stencilTestFrontAndBackDynamicWriteMask        = 0;
  state.stencilTestFrontAndBackDynamicReference        = 0;
  state.stencilTestFrontStencilTestFailOp              = RED_STENCIL_OP_KEEP;
  state.stencilTestFrontStencilTestPassDepthTestPassOp = RED_STENCIL_OP_KEEP;
  state.stencilTestFrontStencilTestPassDepthTestFailOp = RED_STENCIL_OP_KEEP;
  state.stencilTestFrontCompareOp                      = RED_COMPARE_OP_NEVER;
  state.stencilTestBackStencilTestFailOp               = RED_STENCIL_OP_KEEP;
  state.stencilTestBackStencilTestPassDepthTestPassOp  = RED_STENCIL_OP_KEEP;
  state.stencilTestBackStencilTestPassDepthTestFailOp  = RED_STENCIL_OP_KEEP;
  state.stencilTestBackCompareOp                       = RED_COMPARE_OP_NEVER;
  state.stencilTestFrontAndBackDynamicCompareMask      = 0;
  state.stencilTestFrontAndBackDynamicWriteMask        = 0;
  state.stencilTestFrontAndBackDynamicReference        = 0;
  state.stencilTestFrontAndBackStaticCompareMask       = 0;
  state.stencilTestFrontAndBackStaticWriteMask         = 0;
  state.stencilTestFrontAndBackStaticReference         = 0;
  state.blendLogicOpEnable                             = 0;
  state.blendLogicOp                                   = RED_LOGIC_OP_CLEAR;
  state.blendConstantsDynamic                          = 0;
  state.blendConstantsStatic[0]                        = 0;
  state.blendConstantsStatic[1]                        = 0;
  state.blendConstantsStatic[2]                        = 0;
  state.blendConstantsStatic[3]                        = 0;
  state.outputColorsCount                              = 1;
  state.outputColorsWriteMask[0]                       = RED_COLOR_COMPONENT_BITFLAG_R | RED_COLOR_COMPONENT_BITFLAG_G | RED_COLOR_COMPONENT_BITFLAG_B | RED_COLOR_COMPONENT_BITFLAG_A;
  state.outputColorsWriteMask[1]                       = RED_COLOR_COMPONENT_BITFLAG_R | RED_COLOR_COMPONENT_BITFLAG_G | RED_COLOR_COMPONENT_BITFLAG_B | RED_COLOR_COMPONENT_BITFLAG_A;
  state.outputColorsWriteMask[2]                       = RED_COLOR_COMPONENT_BITFLAG_R | RED_COLOR_COMPONENT_BITFLAG_G | RED_COLOR_COMPONENT_BITFLAG_B | RED_COLOR_COMPONENT_BITFLAG_A;
  state.outputColorsWriteMask[3]                       = RED_COLOR_COMPONENT_BITFLAG_R | RED_COLOR_COMPONENT_BITFLAG_G | RED_COLOR_COMPONENT_BITFLAG_B | RED_COLOR_COMPONENT_BITFLAG_A;
  state.outputColorsWriteMask[4]                       = RED_COLOR_COMPONENT_BITFLAG_R | RED_COLOR_COMPONENT_BITFLAG_G | RED_COLOR_COMPONENT_BITFLAG_B | RED_COLOR_COMPONENT_BITFLAG_A;
  state.outputColorsWriteMask[5]                       = RED_COLOR_COMPONENT_BITFLAG_R | RED_COLOR_COMPONENT_BITFLAG_G | RED_COLOR_COMPONENT_BITFLAG_B | RED_COLOR_COMPONENT_BITFLAG_A;
  state.outputColorsWriteMask[6]                       = RED_COLOR_COMPONENT_BITFLAG_R | RED_COLOR_COMPONENT_BITFLAG_G | RED_COLOR_COMPONENT_BITFLAG_B | RED_COLOR_COMPONENT_BITFLAG_A;
  state.outputColorsWriteMask[7]                       = RED_COLOR_COMPONENT_BITFLAG_R | RED_COLOR_COMPONENT_BITFLAG_G | RED_COLOR_COMPONENT_BITFLAG_B | RED_COLOR_COMPONENT_BITFLAG_A;
  for (unsigned i = 0; i < 8; i += 1) {
    state.outputColorsBlendEnable[i]                   = 0;
    state.outputColorsBlendColorFactorSource[i]        = RED_BLEND_FACTOR_SOURCE_ALPHA;
    state.outputColorsBlendColorFactorTarget[i]        = RED_BLEND_FACTOR_ONE_MINUS_SOURCE_ALPHA;
    state.outputColorsBlendColorOp[i]                  = RED_BLEND_OP_ADD;
    state.outputColorsBlendAlphaFactorSource[i]        = RED_BLEND_FACTOR_SOURCE_ALPHA;
    state.outputColorsBlendAlphaFactorTarget[i]        = RED_BLEND_FACTOR_ONE_MINUS_SOURCE_ALPHA;
    state.outputColorsBlendAlphaOp[i]                  = RED_BLEND_OP_ADD;
  }
  RedHandleProcedure procedureDraw = NULL;
  np(redCreateProcedure,
    "context", context,
    "gpu", gpu,
    "handleName", "procedureDraw",
    "procedureCache", NULL,
    "outputDeclaration", outputDeclaration,
    "procedureParameters", procedureParameters,
    "gpuCodeVertexMainProcedureName", "main",
    "gpuCodeVertex", gpuCodeVS,
    "gpuCodeFragmentMainProcedureName", "main",
    "gpuCodeFragment", gpuCodeFS,
    "state", &state,
    "stateExtension", NULL,
    "deriveBase", 0,
    "deriveFrom", NULL,
    "outProcedure", &procedureDraw,
    "outStatuses", NULL,
    "optionalFile", __FILE__,
    "optionalLine", __LINE__,
    "optionalUserData", NULL
  );

  unsigned F = 0;
  unsigned f = 0;

  while (glfwWindowShouldClose(window) == 0) {
    glfwPollEvents();

    RedHandleGpuSignal gpusigPresentInconsistent = NULL;
    if (REDGPU_PROGRAM_INCONSISTENT_PRESENT_IMAGE_INDICES == 1) {
      np(redCreateGpuSignal,
        "context", context,
        "gpu", gpu,
        "handleName", "gpusigPresent",
        "outGpuSignal", &gpusigPresentInconsistent,
        "outStatuses", NULL,
        "optionalFile", __FILE__,
        "optionalLine", __LINE__,
        "optionalUserData", NULL
      );

      F = f;
      np(redPresentGetImageIndex,
        "context", context,
        "gpu", gpu,
        "present", present,
        "signalCpuSignal", NULL,
        "signalGpuSignal", gpusigPresentInconsistent,
        "outImageIndex", &f,
        "outStatuses", NULL,
        "optionalFile", __FILE__,
        "optionalLine", __LINE__,
        "optionalUserData", NULL
      );
    }

    np(redCpuSignalWait,
      "context", context,
      "gpu", gpu,
      "cpuSignalsCount", 1,
      "cpuSignals", &cpusig[f],
      "waitAll", 1,
      "outStatuses", NULL,
      "optionalFile", __FILE__,
      "optionalLine", __LINE__,
      "optionalUserData", NULL
    );

    if (REDGPU_PROGRAM_INCONSISTENT_PRESENT_IMAGE_INDICES == 1) {
      redDestroyGpuSignal(context, gpu, gpusigPresent[f], __FILE__, __LINE__, NULL);
      gpusigPresent[f] = gpusigPresentInconsistent;
    }

    np(redCpuSignalUnsignal,
      "context", context,
      "gpu", gpu,
      "cpuSignalsCount", 1,
      "cpuSignals", &cpusig[f],
      "outStatuses", NULL,
      "optionalFile", __FILE__,
      "optionalLine", __LINE__,
      "optionalUserData", NULL
    );

    np(redCallsSet,
      "context", context,
      "gpu", gpu,
      "calls", calls[f].handle,
      "callsMemory", calls[f].memory,
      "callsReusable", calls[f].reusable,
      "outStatuses", NULL,
      "optionalFile", __FILE__,
      "optionalLine", __LINE__,
      "optionalUserData", NULL
    );
    {
      {
        np(redCallMarkSet, "address", callsPA.redCallMarkSet, "calls", calls[f].handle, "mark", "redCallUsageAliasOrderBarrier RED_STATE_UNUSABLE -> RED_STATE_USABLE");
        RedUsageImage imageUsage = {0};
        imageUsage.barrierSplit           = RED_BARRIER_SPLIT_NONE;
        imageUsage.oldAccessStages        = 0;
        imageUsage.newAccessStages        = RED_ACCESS_STAGE_BITFLAG_OUTPUT_COLOR;
        imageUsage.oldAccess              = 0;
        imageUsage.newAccess              = RED_ACCESS_BITFLAG_OUTPUT_COLOR_W;
        imageUsage.oldState               = RED_STATE_UNUSABLE;
        imageUsage.newState               = RED_STATE_USABLE;
        imageUsage.queueFamilyIndexSource = -1;
        imageUsage.queueFamilyIndexTarget = -1;
        imageUsage.image                  = presentImages[f];
        imageUsage.imageAllParts          = RED_IMAGE_PART_BITFLAG_COLOR;
        imageUsage.imageLevelsFirst       = 0;
        imageUsage.imageLevelsCount       = -1;
        imageUsage.imageLayersFirst       = 0;
        imageUsage.imageLayersCount       = -1;
        np(redCallUsageAliasOrderBarrier,
          "address", callsPA.redCallUsageAliasOrderBarrier,
          "calls", calls[f].handle,
          "context", context,
          "arrayUsagesCount", 0,
          "arrayUsages", NULL,
          "imageUsagesCount", 1,
          "imageUsages", &imageUsage,
          "aliasesCount", 0,
          "aliases", NULL,
          "ordersCount", 0,
          "orders", NULL,
          "dependencyByRegion", 0
        );
        np(redCallMarkEnd, "address", callsPA.redCallMarkEnd, "calls", calls[f].handle);
      }

      {
        np(redCallMarkSet, "address", callsPA.redCallMarkSet, "calls", calls[f].handle, "mark", "Copy array to image");
        RedCopyArrayImageRange copy = {0};
        copy.arrayBytesFirst               = 0;
        copy.arrayTexelsCountToNextRow     = 16;
        copy.arrayTexelsCountToNextLayerOr3DDepthSliceDividedByTexelsCountToNextRow = 0;
        copy.imageParts.allParts           = RED_IMAGE_PART_BITFLAG_COLOR;
        copy.imageParts.level              = 0;
        copy.imageParts.layersFirst        = 0;
        copy.imageParts.layersCount        = 1;
        copy.imageOffset.texelX            = 0;
        copy.imageOffset.texelY            = 0;
        copy.imageOffset.texelZ            = 0;
        copy.imageExtent.texelsCountWidth  = 8;
        copy.imageExtent.texelsCountHeight = 8;
        copy.imageExtent.texelsCountDepth  = 1;
        npfp(redCallCopyArrayToImage, callsPA.redCallCopyArrayToImage,
          "calls", calls[f].handle,
          "arrayR", pixels.handle,
          "imageW", image.handle,
          "setTo1", 1,
          "rangesCount", 1,
          "ranges", &copy
        );
        np(redCallMarkEnd, "address", callsPA.redCallMarkEnd, "calls", calls[f].handle);
      }

      {
        np(redCallMarkSet, "address", callsPA.redCallMarkSet, "calls", calls[f].handle, "mark", "redCallUsageAliasOrderBarrier COPY_W -> STRUCT_RESOURCE_R");
        RedUsageImage imageUsage = {0};
        imageUsage.barrierSplit           = RED_BARRIER_SPLIT_NONE;
        imageUsage.oldAccessStages        = RED_ACCESS_STAGE_BITFLAG_COPY;
        imageUsage.newAccessStages        = RED_ACCESS_STAGE_BITFLAG_FRAGMENT;
        imageUsage.oldAccess              = RED_ACCESS_BITFLAG_COPY_W;
        imageUsage.newAccess              = RED_ACCESS_BITFLAG_STRUCT_RESOURCE_NON_FRAGMENT_STAGE_R | RED_ACCESS_BITFLAG_STRUCT_RESOURCE_FRAGMENT_STAGE_R;
        imageUsage.oldState               = RED_STATE_USABLE;
        imageUsage.newState               = RED_STATE_USABLE;
        imageUsage.queueFamilyIndexSource = -1;
        imageUsage.queueFamilyIndexTarget = -1;
        imageUsage.image                  = image.handle;
        imageUsage.imageAllParts          = RED_IMAGE_PART_BITFLAG_COLOR;
        imageUsage.imageLevelsFirst       = 0;
        imageUsage.imageLevelsCount       = -1;
        imageUsage.imageLayersFirst       = 0;
        imageUsage.imageLayersCount       = -1;
        np(redCallUsageAliasOrderBarrier,
          "address", callsPA.redCallUsageAliasOrderBarrier,
          "calls", calls[f].handle,
          "context", context,
          "arrayUsagesCount", 0,
          "arrayUsages", NULL,
          "imageUsagesCount", 1,
          "imageUsages", &imageUsage,
          "aliasesCount", 0,
          "aliases", NULL,
          "ordersCount", 0,
          "orders", NULL,
          "dependencyByRegion", 0
        );
        np(redCallMarkEnd, "address", callsPA.redCallMarkEnd, "calls", calls[f].handle);
      }

      {
        np(redCallMarkSet, "address", callsPA.redCallMarkSet, "calls", calls[f].handle, "mark", "redCallProcedure");

        np(redCallSetProcedureOutput,
          "address", callsPA.redCallSetProcedureOutput,
          "calls", calls[f].handle,
          "outputDeclaration", outputDeclaration,
          "output", rendertargets[f].handle,
          "inlineOutput", NULL,
          "outputWidth", rendertargets[f].width,
          "outputHeight", rendertargets[f].height,
          "outputDepthStencilEnable", rendertargets[f].depthStencilEnable,
          "outputColorsCount", rendertargets[f].colorsCount,
          "depthClearValue", 0.f,
          "stencilClearValue", 0,
          "colorsClearValuesFloat", NULL,
          "colorsClearValuesSint", NULL,
          "colorsClearValuesUint", NULL
        );
        np(redCallSetStructsMemory,
          "address", callsPA.redCallSetStructsMemory,
          "calls", calls[f].handle,
          "structsMemory", structsMemory,
          "structsMemorySamplers", NULL
        );
        npfp(redCallSetProcedure, callsPA.redCallSetProcedure,
          "calls", calls[f].handle,
          "procedureType", RED_PROCEDURE_TYPE_DRAW,
          "procedure", procedureDraw
        );
        np(redCallSetProcedureParameters,
          "address", callsPA.redCallSetProcedureParameters,
          "calls", calls[f].handle,
          "procedureType", RED_PROCEDURE_TYPE_DRAW,
          "procedureParameters", procedureParameters
        );
        npfp(redCallSetProcedureParametersStructs, callsPA.redCallSetProcedureParametersStructs,
          "calls", calls[f].handle,
          "procedureType", RED_PROCEDURE_TYPE_DRAW,
          "procedureParameters", procedureParameters,
          "procedureParametersDeclarationStructsDeclarationsFirst", 0,
          "structsCount", 1,
          "structs", &structs[0],
          "setTo0", 0,
          "setTo00", 0
        );
        npfp(redCallProcedure, callsPA.redCallProcedure,
          "calls", calls[f].handle,
          "vertexCount", 3,
          "instanceCount", 1,
          "vertexFirst", 0,
          "instanceFirst", 0
        );
        np(redCallEndProcedureOutput, "address", callsPA.redCallEndProcedureOutput, "calls", calls[f].handle);

        np(redCallMarkEnd, "address", callsPA.redCallMarkEnd, "calls", calls[f].handle);
      }

      {
        np(redCallMarkSet, "address", callsPA.redCallMarkSet, "calls", calls[f].handle, "mark", "redCallUsageAliasOrderBarrier RED_STATE_USABLE -> RED_STATE_PRESENT");
        RedUsageImage imageUsage = {0};
        imageUsage.barrierSplit           = RED_BARRIER_SPLIT_NONE;
        imageUsage.oldAccessStages        = RED_ACCESS_STAGE_BITFLAG_OUTPUT_COLOR;
        imageUsage.newAccessStages        = 0;
        imageUsage.oldAccess              = RED_ACCESS_BITFLAG_OUTPUT_COLOR_W;
        imageUsage.newAccess              = 0;
        imageUsage.oldState               = RED_STATE_USABLE;
        imageUsage.newState               = RED_STATE_PRESENT;
        imageUsage.queueFamilyIndexSource = -1;
        imageUsage.queueFamilyIndexTarget = -1;
        imageUsage.image                  = presentImages[f];
        imageUsage.imageAllParts          = RED_IMAGE_PART_BITFLAG_COLOR;
        imageUsage.imageLevelsFirst       = 0;
        imageUsage.imageLevelsCount       = -1;
        imageUsage.imageLayersFirst       = 0;
        imageUsage.imageLayersCount       = -1;
        np(redCallUsageAliasOrderBarrier,
          "address", callsPA.redCallUsageAliasOrderBarrier,
          "calls", calls[f].handle,
          "context", context,
          "arrayUsagesCount", 0,
          "arrayUsages", NULL,
          "imageUsagesCount", 1,
          "imageUsages", &imageUsage,
          "aliasesCount", 0,
          "aliases", NULL,
          "ordersCount", 0,
          "orders", NULL,
          "dependencyByRegion", 0
        );
        np(redCallMarkEnd, "address", callsPA.redCallMarkEnd, "calls", calls[f].handle);
      }
    }
    np(redCallsEnd,
      "context", context,
      "gpu", gpu,
      "calls", calls[f].handle,
      "callsMemory", calls[f].memory,
      "outStatuses", NULL,
      "optionalFile", __FILE__,
      "optionalLine", __LINE__,
      "optionalUserData", NULL
    );

    if (REDGPU_PROGRAM_INCONSISTENT_PRESENT_IMAGE_INDICES == 0) {
      F = f;
      np(redPresentGetImageIndex,
        "context", context,
        "gpu", gpu,
        "present", present,
        "signalCpuSignal", NULL,
        "signalGpuSignal", gpusigPresent[F],
        "outImageIndex", &f,
        "outStatuses", NULL,
        "optionalFile", __FILE__,
        "optionalLine", __LINE__,
        "optionalUserData", NULL
      );
      if (F != f) {
        printf("%s\n", "[REDGPU][Assert]: Present image indices are inconsistent. Set REDGPU_PROGRAM_INCONSISTENT_PRESENT_IMAGE_INDICES to 1 and recompile the program.\n");
        return 1;
      }
    }

    {
      RedHandleCalls callsHandles[] = {
        calls[f].handle,
      };
      RedGpuTimeline timelines[1] = {0};
      unsigned arrayOf65536[1] = {65536};
      timelines[0].setTo4                            = 4;
      timelines[0].setTo0                            = 0;
      timelines[0].waitForAndUnsignalGpuSignalsCount = 1;
      timelines[0].waitForAndUnsignalGpuSignals      = &gpusigPresent[f];
      timelines[0].setTo65536                        = arrayOf65536;
      timelines[0].callsCount                        = _countof(callsHandles);
      timelines[0].calls                             = callsHandles;
      timelines[0].signalGpuSignalsCount             = 1;
      timelines[0].signalGpuSignals                  = &gpusig[f];
      np(redQueueSubmit,
        "context", context,
        "gpu", gpu,
        "queue", queue,
        "timelinesCount", _countof(timelines),
        "timelines", timelines,
        "signalCpuSignal", cpusig[f],
        "outStatuses", NULL,
        "optionalFile", __FILE__,
        "optionalLine", __LINE__,
        "optionalUserData", NULL
      );
    }

    np(redQueuePresent,
      "context", context,
      "gpu", gpu,
      "queue", queue,
      "waitForAndUnsignalGpuSignalsCount", 1,
      "waitForAndUnsignalGpuSignals", &gpusig[f],
      "presentsCount", 1,
      "presents", &present,
      "presentsImageIndex", &f,
      "outPresentsStatus", NULL,
      "outStatuses", NULL,
      "optionalFile", __FILE__,
      "optionalLine", __LINE__,
      "optionalUserData", NULL
    );

    f += 1;
    f %= REDGPU_PROGRAM_PRESENT_IMAGES_COUNT;
  }

  redQueuePresent(context, gpu, queue, 0, NULL, 0, NULL, NULL, NULL, NULL, __FILE__, __LINE__, NULL);

  np(redCpuSignalWait,
    "context", context,
    "gpu", gpu,
    "cpuSignalsCount", REDGPU_PROGRAM_PRESENT_IMAGES_COUNT,
    "cpuSignals", cpusig,
    "waitAll", 1,
    "outStatuses", NULL,
    "optionalFile", __FILE__,
    "optionalLine", __LINE__,
    "optionalUserData", NULL
  );

  for (unsigned i = 0; i < REDGPU_PROGRAM_PRESENT_IMAGES_COUNT; i += 1) {
    redDestroyCalls(context, gpu, calls[i].handle, calls[i].memory, __FILE__, __LINE__, NULL);
  }
  for (unsigned i = 0; i < REDGPU_PROGRAM_PRESENT_IMAGES_COUNT; i += 1) {
    redDestroyGpuSignal(context, gpu, gpusig[i], __FILE__, __LINE__, NULL);
  }
  for (unsigned i = 0; i < REDGPU_PROGRAM_PRESENT_IMAGES_COUNT; i += 1) {
    redDestroyGpuSignal(context, gpu, gpusigPresent[i], __FILE__, __LINE__, NULL);
  }
  for (unsigned i = 0; i < REDGPU_PROGRAM_PRESENT_IMAGES_COUNT; i += 1) {
    redDestroyCpuSignal(context, gpu, cpusig[i], __FILE__, __LINE__, NULL);
  }
  redDestroyArray(context, gpu, pixels.handle, __FILE__, __LINE__, NULL);
  redMemoryFree(context, gpu, pixelsMemory, __FILE__, __LINE__, NULL);
  redDestroySampler(context, gpu, imageSampler, __FILE__, __LINE__, NULL);
  redDestroyTexture(context, gpu, imageTexture, __FILE__, __LINE__, NULL);
  redDestroyImage(context, gpu, image.handle, __FILE__, __LINE__, NULL);
  redMemoryFree(context, gpu, imageMemory, __FILE__, __LINE__, NULL);
  redDestroyGpuCode(context, gpu, gpuCodeVS, __FILE__, __LINE__, NULL);
  redDestroyGpuCode(context, gpu, gpuCodeFS, __FILE__, __LINE__, NULL);
  for (unsigned i = 0; i < REDGPU_PROGRAM_PRESENT_IMAGES_COUNT; i += 1) {
    redDestroyOutput(context, gpu, rendertargets[i].handle, __FILE__, __LINE__, NULL);
  }
  redDestroyOutputDeclaration(context, gpu, outputDeclaration, __FILE__, __LINE__, NULL);
  redDestroyStructDeclaration(context, gpu, structDeclaration, __FILE__, __LINE__, NULL);
  redStructsMemoryFree(context, gpu, structsMemory, __FILE__, __LINE__, NULL);
  redDestroyProcedureParameters(context, gpu, procedureParameters, __FILE__, __LINE__, NULL);
  redDestroyProcedure(context, gpu, procedureDraw, __FILE__, __LINE__, NULL);
  for (unsigned i = 0; i < REDGPU_PROGRAM_PRESENT_IMAGES_COUNT; i += 1) {
    redDestroyTexture(context, gpu, presentTextures[i], __FILE__, __LINE__, NULL);
  }
  redDestroyPresent(context, gpu, present, __FILE__, __LINE__, NULL);
  redDestroySurface(context, gpu, surface, __FILE__, __LINE__, NULL);

  redDestroyContext(context, __FILE__, __LINE__, NULL);
}

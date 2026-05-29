// REDGPU RTE version from 03 Apr 2023.
//
// Compile instructions:
//
// git clone https://github.com/nvpro-samples/nvpro_core
// cd nvpro_core
// git checkout 7bb287cd89265e732a51af56496a7973256ec6cc
// git submodule update --init --depth 1 --progress
// cd ..
//
// (Release build):
//
// cd build/
// cl /c /O2 /DNDEBUG /I../nvpro_core/third_party/glfw/include /I../nvpro_core/third_party/glfw/src /I../nvpro_core/third_party/glfw/build/src /Dglfw_EXPORTS /D_WINDLL /D_GLFW_BUILD_DLL /D_GLFW_WIN32 /DWIN32 /D_WINDOWS /DUNICODE /D_UNICODE /D_CRT_SECURE_NO_WARNINGS /D_UNICODE /DUNICODE ../redgpu_rte_glfw3.c
// cl /c C:/RedGpuSDK/redgpu.c
// cl /LD /O2 /DNDEBUG /EHsc /std:c++20 ../redgpu_rte.cpp redgpu.obj redgpu_rte_glfw3.obj /IC:/VulkanSDK/1.4.341.1/Include /I../nvpro_core /I../nvpro_core/nvp /I../nvpro_core/third_party/tinygltf /I../nvpro_core/third_party/vma/include /I../nvpro_core/third_party/glfw/include /I../nvpro_core/third_party/imgui
//
// (Debug build):
//
// cd build/
// cl /c /MDd /Od /Zi /D_DEBUG /I../nvpro_core/third_party/glfw/include /I../nvpro_core/third_party/glfw/src /I../nvpro_core/third_party/glfw/build/src /Dglfw_EXPORTS /D_WINDLL /D_GLFW_BUILD_DLL /D_GLFW_WIN32 /DWIN32 /D_WINDOWS /DUNICODE /D_UNICODE /D_CRT_SECURE_NO_WARNINGS /D_UNICODE /DUNICODE ../redgpu_rte_glfw3.c
// cl /c /MDd /Od /Zi /D_DEBUG C:/RedGpuSDK/redgpu.c
// cl /LD /MDd /Od /Zi /D_DEBUG /EHsc /std:c++20 ../redgpu_rte.cpp redgpu.obj redgpu_rte_glfw3.obj /IC:/VulkanSDK/1.4.341.1/Include /I../nvpro_core /I../nvpro_core/nvp /I../nvpro_core/third_party/tinygltf /I../nvpro_core/third_party/vma/include /I../nvpro_core/third_party/glfw/include /I../nvpro_core/third_party/imgui
//
// In nvpro_core/nvvk/context_vk.hpp
// comment the line:
// private:
//
// In nvpro_core/nvh/misc.hpp
// replace:
// size_t charsNeeded = static_cast<size_t>(vsnprintf(str.data(), str.size(), msg, list));  // charsNeeded doesn't count \0
// with:
// size_t charsNeeded = static_cast<size_t>(vsnprintf((char *)str.data(), str.size(), msg, list));  // charsNeeded doesn't count \0
//
// In nvpro_core/nvh/parametertools.cpp
// comment the whole function:
// static bool endsWith(std::string const& s, std::string const& end)
//
// In nvpro_core/third_party/tinygltf/json.hpp
// replace all:
// (std::snprintf)
// with:
// (snprintf)
//
// In nvpro_core/imgui/backends/imgui_vk_extra.cpp
// comment the whole function:
// static void check_vk_result(VkResult err)
//
// In nvpro_core/nvvkhl/application.hpp
// comment the line:
// private:
//
// In nvpro_core/nvvkhl/application.hpp
// replace:
// std::shared_ptr<nvvk::Context>            m_context;
// with:
// nvvk::Context *                           m_context;
//
// In nvpro_core/nvvkhl/application.hpp
// replace:
// inline std::shared_ptr<nvvk::Context> getContext() { return m_context; }  // Return the Vulkan context
// with:
// inline nvvk::Context *   getContext() { return m_context; }  // Return the Vulkan context
//
// In nvpro_core/nvvkhl/application.cpp
// replace:
// m_context = std::make_shared<nvvk::Context>();
// with:
// m_context = new nvvk::Context();
//
// In nvpro_core/nvvkhl/application.cpp
// replace:
// m_context->init(vk_setup);
// with:
// if (false) { m_context->init(vk_setup); } else { unsigned gpuIndex = 0; try { gpuIndex = (unsigned)std::stoi(info.name); } catch (...) {} if (m_context->initInstance(vk_setup) == true) { std::vector<uint32_t> compatibleDevices = m_context->getCompatibleDevices(vk_setup); if (gpuIndex >= compatibleDevices.size()) { gpuIndex = 0; } if (compatibleDevices.size() > 0) { m_context->initDevice(compatibleDevices[gpuIndex], vk_setup); } } }
//
// In nvpro_core/nvvkhl/application.cpp
// comment the line:
// m_context->deinit();
//
// In nvpro_core/nvvkhl/appbase_vkpp.cpp
// replace:
// vk::DynamicLoader         dl;
// with:
// vk::detail::DynamicLoader         dl;
//
#pragma comment(lib, "C:/VulkanSDK/1.4.341.1/Lib/vulkan-1.lib")
#pragma comment(lib, "kernel32.lib")
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "shell32.lib")
#pragma comment(lib, "comdlg32.lib")

#define REDGPU_RTE_DECLSPEC __declspec(dllexport)
#include "redgpu_rte.h"

#include "C:/RedGpuSDK/redgpu.h"
#include "C:/RedGpuSDK/redgpu_wsi.h"
#include "C:/RedGpuSDK/redgpu_array_timestamp.h"
#include "C:/RedGpuSDK/redgpu_context_from_vk.h"

#include "nvpro_core/nvvk/context_vk.hpp"           // For nvvk::Context
#include "nvpro_core/nvvk/descriptorsets_vk.hpp"    // For nvvk::DescriptorSetContainer
#include "nvpro_core/nvvk/raytraceKHR_vk.hpp"       // For nvvk::RaytracingBuilderKHR
#include "nvpro_core/nvvk/memorymanagement_vk.hpp"  // For nvvk::DeviceMemoryAllocator
#include "nvpro_core/nvvk/resourceallocator_vk.hpp" // For nvvk::ResourceAllocatorDma
#include "nvpro_core/nvvk/memallocator_vma_vk.hpp"  // For nvvk::ResourceAllocatorVma
#define VMA_IMPLEMENTATION
#include "nvpro_core/third_party/vma/include/vk_mem_alloc.h" // For VMA implementation
#include "nvpro_core/nvvkhl/application.hpp"        // For nvvkhl::IAppElement
#include "nvpro_core/nvvkhl/gbuffer.hpp"            // For nvvkhl::GBuffer
#include "nvpro_core/nvvk/sbtwrapper_vk.hpp"        // For nvvk::SBTWrapper

#include <vulkan/vulkan.h>
#include <string>

static int redRteEnumeratePhysicalDevicesReturning0(void * instance, unsigned * outPhysicalDevicesCount, void ** outPhysicalDevices) {
  if (outPhysicalDevicesCount != 0) {
    outPhysicalDevicesCount[0] = 0;
  }
  return 0;
}

static int redRteEnumeratePhysicalDevicesReturning1(void * instance, unsigned * outPhysicalDevicesCount, void ** outPhysicalDevices) {
  if (outPhysicalDevicesCount != 0) {
    outPhysicalDevicesCount[0] = 1;
  }
  return 0;
}

static void * redRteGetInstanceProcAddrSpecialCasedForRedRteEnumeratePhysicalDevicesReturning0(void * instance, const char * name) {
  std::string procName = name;
  if      (procName == "vkEnumeratePhysicalDevices")              { return redRteEnumeratePhysicalDevicesReturning0; }
  else if (procName == "vkGetPhysicalDeviceMemoryProperties2KHR") { return (void *)vkGetInstanceProcAddr((VkInstance)instance, "vkGetPhysicalDeviceMemoryProperties2"); }
  else if (procName == "vkGetPhysicalDeviceFeatures2KHR")         { return (void *)vkGetInstanceProcAddr((VkInstance)instance, "vkGetPhysicalDeviceFeatures2"); }
  else if (procName == "vkGetPhysicalDeviceProperties2KHR")       { return (void *)vkGetInstanceProcAddr((VkInstance)instance, "vkGetPhysicalDeviceProperties2"); }
  else {
    return (void *)vkGetInstanceProcAddr((VkInstance)instance, name);
  }
}

static void * redRteGetInstanceProcAddrSpecialCasedForRedRteEnumeratePhysicalDevicesReturning1(void * instance, const char * name) {
  std::string procName = name;
  if      (procName == "vkEnumeratePhysicalDevices")              { return redRteEnumeratePhysicalDevicesReturning1; }
  else if (procName == "vkGetPhysicalDeviceMemoryProperties2KHR") { return (void *)vkGetInstanceProcAddr((VkInstance)instance, "vkGetPhysicalDeviceMemoryProperties2"); }
  else if (procName == "vkGetPhysicalDeviceFeatures2KHR")         { return (void *)vkGetInstanceProcAddr((VkInstance)instance, "vkGetPhysicalDeviceFeatures2"); }
  else if (procName == "vkGetPhysicalDeviceProperties2KHR")       { return (void *)vkGetInstanceProcAddr((VkInstance)instance, "vkGetPhysicalDeviceProperties2"); }
  else {
    return (void *)vkGetInstanceProcAddr((VkInstance)instance, name);
  }
}

static nvvk::ContextCreateInfo redRteGetContextCreateInfoFromCreateContextParameters(const char * optionalProgramName, const char * optionalEngineName, RedRteCreateContextParameters * rteParameters) {
  nvvk::ContextCreateInfo nvvkContextCreateInfo = nvvk::ContextCreateInfo();
  nvvkContextCreateInfo.useDeviceGroups           = false;
  nvvkContextCreateInfo.appEngine                 = optionalEngineName  == 0 ? "" : optionalEngineName;
  nvvkContextCreateInfo.appTitle                  = optionalProgramName == 0 ? "" : optionalProgramName;
  nvvkContextCreateInfo.disableRobustBufferAccess = rteParameters->disableRobustBufferAccess == 1 ? true : false;
  nvvkContextCreateInfo.verboseCompatibleDevices  = rteParameters->verboseCompatibleDevices  == 1 ? true : false;
  nvvkContextCreateInfo.verboseUsed               = rteParameters->verboseUsed               == 1 ? true : false;
  nvvkContextCreateInfo.verboseAvailable          = rteParameters->verboseAvailable          == 1 ? true : false;
  nvvkContextCreateInfo.enableAftermath           = rteParameters->enableAftermath           == 1 ? true : false;
  nvvkContextCreateInfo.apiMajor                  = rteParameters->vkVersionMajor;
  nvvkContextCreateInfo.apiMinor                  = rteParameters->vkVersionMinor;
  nvvkContextCreateInfo.defaultQueueGCT           = 0x00000001 | 0x00000002 | 0x00000004; // VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_COMPUTE_BIT | VK_QUEUE_TRANSFER_BIT
  nvvkContextCreateInfo.defaultQueueT             = 0;
  nvvkContextCreateInfo.defaultQueueC             = 0;
  nvvkContextCreateInfo.defaultPriorityGCT        = 1.0f;
  nvvkContextCreateInfo.defaultPriorityT          = 1.0f;
  nvvkContextCreateInfo.defaultPriorityC          = 1.0f;
  nvvkContextCreateInfo.requestedQueues           = {};
  nvvkContextCreateInfo.requestedQueues.push_back({0x00000001 | 0x00000002 | 0x00000004, 1, 1.0f});
  for (uint64_t i = 0; i < rteParameters->vkInstanceExtensionsCount; i += 1) {
    nvvkContextCreateInfo.addInstanceExtension(rteParameters->vkInstanceExtensions[i]);
  }
  for (uint64_t i = 0; i < rteParameters->vkInstanceLayersCount; i += 1) {
    nvvkContextCreateInfo.addInstanceLayer(rteParameters->vkInstanceLayers[i]);
  }
  for (uint64_t i = 0; i < rteParameters->vkDeviceExtensionsCount; i += 1) {
    void *   featureStruct = 0;
    unsigned version       = 0;
    if (rteParameters->vkDeviceExtensionsOutVkFeaturesStruct != 0) {
      featureStruct = rteParameters->vkDeviceExtensionsOutVkFeaturesStruct[i];
    }
    if (rteParameters->vkDeviceExtensionsVersion != 0) {
      version = rteParameters->vkDeviceExtensionsVersion[i];
    }
    nvvkContextCreateInfo.addDeviceExtension(rteParameters->vkDeviceExtensions[i], false, featureStruct, version);
  }
  for (uint64_t i = 0; i < rteParameters->requestQueueFamilysCount; i += 1) {
    nvvkContextCreateInfo.addRequestedQueue((VkQueueFlags)rteParameters->requestQueueFamilysFlags[i], rteParameters->requestQueueFamilysQueuesCount[i], rteParameters->requestQueueFamilysPriority[i]);
  }
  return nvvkContextCreateInfo;
}

void * redRteGetInstanceProcAddr(const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters) {
  return vkGetInstanceProcAddr;
}

int64_t redRteCreateContextFromRteContext(RedRteHandleContext rteContext, RedTypeProcedureMalloc mallocFn, RedTypeProcedureFree freeFn, RedTypeProcedureMallocTagged optionalMallocTagged, RedTypeProcedureFreeTagged optionalFreeTagged, RedTypeProcedureDebugCallback debugCallback, RedSdkVersion sdkVersion, unsigned sdkExtensionsCount, const unsigned * sdkExtensions, const char * optionalProgramName, unsigned optionalProgramVersion, const char * optionalEngineName, unsigned optionalEngineVersion, const void * optionalSettings, RedContext * outContext, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters) {
  nvvk::Context * nvvkctx = (nvvk::Context *)rteContext;
  
  bool success = nvvkctx != 0 ? true : false;

  uint64_t instance       = 0;
  uint64_t physicalDevice = 0;
  uint64_t device         = 0;
  if (success == true) {
    instance       = (uint64_t)(void *)nvvkctx->m_instance;
    physicalDevice = (uint64_t)(void *)nvvkctx->m_physicalDevice;
    device         = (uint64_t)(void *)nvvkctx->m_device;
  }

  RedContextOptionalSettings0 settings0 = {};
  settings0.settings                      = RED_CONTEXT_OPTIONAL_SETTINGS_0;
  settings0.next                          = optionalSettings;
  settings0.skipCheckingContextLayers     = 0;
  settings0.skipCheckingContextExtensions = 0;
  settings0.gpusExposeOnlyOneQueue        = 1;
  RedContextOptionalSettingsContextFromVkGetInstanceProcAddr settingsContextFromVkGetInstanceProcAddr = {};
  settingsContextFromVkGetInstanceProcAddr.settings            = RED_CONTEXT_OPTIONAL_SETTINGS_CONTEXT_FROM_VK_GET_INSTANCE_PROC_ADDR;
  settingsContextFromVkGetInstanceProcAddr.next                = &settings0;
  settingsContextFromVkGetInstanceProcAddr.getInstanceProcAddr = success == true ? redRteGetInstanceProcAddrSpecialCasedForRedRteEnumeratePhysicalDevicesReturning1 : redRteGetInstanceProcAddrSpecialCasedForRedRteEnumeratePhysicalDevicesReturning0;
  RedContextOptionalSettingsContextFromVk settingsContextFromVk = {};
  settingsContextFromVk.settings             = RED_CONTEXT_OPTIONAL_SETTINGS_CONTEXT_FROM_VK;
  settingsContextFromVk.next                 = &settingsContextFromVkGetInstanceProcAddr;
  settingsContextFromVk.instance             = success == true ? instance        : 0;
  settingsContextFromVk.physicalDevicesCount = success == true ? 1               : 0;
  settingsContextFromVk.physicalDevices      = success == true ? &physicalDevice : 0;
  settingsContextFromVk.devices              = success == true ? &device         : 0;
  redCreateContext(mallocFn, freeFn, optionalMallocTagged, optionalFreeTagged, debugCallback, sdkVersion, sdkExtensionsCount, sdkExtensions, optionalProgramName, optionalProgramVersion, optionalEngineName, optionalEngineVersion, &settingsContextFromVk, outContext, outStatuses, optionalFile, optionalLine, optionalUserData);

  return success == true ? 0 : -1;
}

int64_t redRteCreateContext(RedTypeProcedureMalloc mallocFn, RedTypeProcedureFree freeFn, RedTypeProcedureMallocTagged optionalMallocTagged, RedTypeProcedureFreeTagged optionalFreeTagged, RedTypeProcedureDebugCallback debugCallback, RedSdkVersion sdkVersion, unsigned sdkExtensionsCount, const unsigned * sdkExtensions, const char * optionalProgramName, unsigned optionalProgramVersion, const char * optionalEngineName, unsigned optionalEngineVersion, const void * optionalSettings, RedContext * outContext, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData, RedRteCreateContextParameters * rteParameters) {
  bool success = true;
  
  nvvk::ContextCreateInfo nvvkContextCreateInfo = redRteGetContextCreateInfoFromCreateContextParameters(optionalProgramName, optionalEngineName, rteParameters);

  nvvk::Context * nvvkContext = new(std::nothrow) nvvk::Context();
  if (nvvkContext == 0) {
    success = false;
  }

  if (success == true) {
    success = nvvkContext->initInstance(nvvkContextCreateInfo);
    if (success == true) {
      std::vector<uint32_t> compatibleDevices = nvvkContext->getCompatibleDevices(nvvkContextCreateInfo);
      if (rteParameters->gpuIndex < compatibleDevices.size()) {
        success = nvvkContext->initDevice(compatibleDevices[rteParameters->gpuIndex], nvvkContextCreateInfo);
      } else {
        success = false;
      }
    }
  }

  if (success == true) {
    for (uint64_t i = 0; i < rteParameters->ignoreDebugMessagesCount; i += 1) {
      nvvkContext->ignoreDebugMessage(rteParameters->ignoreDebugMessages[i]);
    }
    nvvkContext->setDebugSeverityFilterMask(rteParameters->debugSeverityFilterMask);
  }

  if (success == true) {
    success = redRteCreateContextFromRteContext((RedRteHandleContext)nvvkContext, mallocFn, freeFn, optionalMallocTagged, optionalFreeTagged, debugCallback, sdkVersion, sdkExtensionsCount, sdkExtensions, optionalProgramName, optionalProgramVersion, optionalEngineName, optionalEngineVersion, optionalSettings, outContext, outStatuses, optionalFile, optionalLine, optionalUserData, rteParameters) == 1 ? true : false;
  }

  rteParameters->rteContext          = (RedRteHandleContext)nvvkContext;
  rteParameters->getInstanceProcAddr = vkGetInstanceProcAddr;

  return success == true ? 0 : -1;
}

int64_t redRteDestroyContext(RedContext context, const char * optionalFile, int optionalLine, void * optionalUserData, RedRteDestroyContextParameters * rteParameters) {
  nvvk::Context * nvvkctx = (nvvk::Context *)rteParameters->rteContext;

  // NOTE(Constantine): This destroy procedure assumes the use of nvpro_core commit 7bb287cd89265e732a51af56496a7973256ec6cc from https://github.com/nvpro-samples/nvpro_core/tree/7bb287cd89265e732a51af56496a7973256ec6cc
  if (nvvkctx != 0) {
    if (nvvkctx->m_destroyDebugUtilsMessengerEXT != 0) { // NOTE(Constantine): Comment class Context's private: keyword in context_vk.hpp.
      nvvkctx->m_destroyDebugUtilsMessengerEXT(nvvkctx->m_instance, nvvkctx->m_dbgMessenger, 0); // NOTE(Constantine): Destroying m_dbgMessenger manually here because it needs a live m_instance that gets destroyed in the next redDestroyContext() call.
      nvvkctx->m_destroyDebugUtilsMessengerEXT = 0;
    }
  }
  redDestroyContext(context, optionalFile, optionalLine, optionalUserData);
  if (nvvkctx != 0) {
    nvvkctx->m_device   = 0; // NOTE(Constantine): Already destroyed by redDestroyContext().
    nvvkctx->m_instance = 0; // NOTE(Constantine): Already destroyed by redDestroyContext().
    nvvkctx->deinit();
    delete nvvkctx;
    nvvkctx = 0;
  }

  return 0;
}

int64_t redRteCreateQueue(RedRteHandleContext rteContext, const char * handleName, RedRteQueueBitflags queueFlags, float queuePriority, RedRteQueue * outRteQueue, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters) {
  nvvk::Context * nvvkctx = (nvvk::Context *)rteContext;

  std::string debugName = "";
  if (handleName != 0) {
    debugName = handleName;
  }
  nvvk::Context::Queue nvvkqueue = nvvkctx->createQueue((unsigned)queueFlags, debugName, queuePriority);
  outRteQueue->handle           = (RedHandleQueue)nvvkqueue.queue;
  outRteQueue->queueFamilyIndex = nvvkqueue.familyIndex;
  outRteQueue->queueIndex       = nvvkqueue.queueIndex;
  outRteQueue->queuePriority    = nvvkqueue.priority;

  return nvvkqueue.queue != 0 ? 0 : -1;
}

uint64_t redRteArrayGetGpuAddress(RedRteHandleContext rteContext, RedHandleGpu gpu, RedHandleArray array, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters) {
  VkBufferDeviceAddressInfo info = {VK_STRUCTURE_TYPE_BUFFER_DEVICE_ADDRESS_INFO};
  info.buffer = (VkBuffer)array;
  return vkGetBufferDeviceAddress((VkDevice)gpu, &info);
}

// Helper allocator DMA

int64_t redRteCreateHelperAllocatorDma(RedRteHandleContext rteContext, RedRteHandleHelperAllocatorDma * outHelperAllocatorDma, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters) {
  nvvk::Context * nvvkctx = (nvvk::Context *)rteContext;
  
  bool success = true;
  
  nvvk::ResourceAllocatorDma * allocator = new(std::nothrow) nvvk::ResourceAllocatorDma();
  if (allocator == 0) {
    success = false;
  }

  if (success == true) {
    allocator->init(nvvkctx->m_device, nvvkctx->m_physicalDevice);
  }

  outHelperAllocatorDma[0] = (RedRteHandleHelperAllocatorDma)allocator;

  return success == true ? 0 : -1;
}

int64_t redRteDestroyHelperAllocatorDma(RedRteHandleHelperAllocatorDma helperAllocatorDma, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters) {
  nvvk::ResourceAllocatorDma * allocator = (nvvk::ResourceAllocatorDma *)helperAllocatorDma;
  
  if (allocator != 0) {
    allocator->deinit();
    delete allocator;
    allocator = 0;
  }

  return 0;
}

int64_t redRteHelperAllocatorDmaCreateArray(RedRteHandleHelperAllocatorDma helperAllocatorDma, const RedRteHelperCreateArrayInfo * arrayInfo, unsigned memoryPropertyFlagsSuggestedDefaultValueIsMemoryPropertyDeviceLocalBit, RedRteHelperArray * outRteHelperArray, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters) {
  nvvk::ResourceAllocatorDma * allocator = (nvvk::ResourceAllocatorDma *)helperAllocatorDma;

  bool success = true;

  const VkBufferCreateInfo * info_p = (const VkBufferCreateInfo *)arrayInfo;
  VkBufferCreateInfo         info   = info_p[0];
  nvvk::Buffer buffer = allocator->createBuffer(info, memoryPropertyFlagsSuggestedDefaultValueIsMemoryPropertyDeviceLocalBit);
  if (buffer.buffer == 0) {
    success = false;
  }

  outRteHelperArray->handle = (RedHandleArray)buffer.buffer;
  outRteHelperArray->rteHelperAllocatorResourceMemory = buffer.memHandle;

  return success == true ? 0 : -1;
}

int64_t redRteHelperAllocatorDmaCreateArraySimple(RedRteHandleHelperAllocatorDma helperAllocatorDma, uint64_t arrayBytesCount, unsigned arrayUsageFlags, unsigned memoryPropertyFlagsSuggestedDefaultValueIsMemoryPropertyDeviceLocalBit, RedRteHelperArray * outRteHelperArray, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters) {
  nvvk::ResourceAllocatorDma * allocator = (nvvk::ResourceAllocatorDma *)helperAllocatorDma;

  bool success = true;

  nvvk::Buffer buffer = allocator->createBuffer(arrayBytesCount, arrayUsageFlags, memoryPropertyFlagsSuggestedDefaultValueIsMemoryPropertyDeviceLocalBit);
  if (buffer.buffer == 0) {
    success = false;
  }

  outRteHelperArray->handle = (RedHandleArray)buffer.buffer;
  outRteHelperArray->rteHelperAllocatorResourceMemory = buffer.memHandle;

  return success == true ? 0 : -1;
}

int64_t redRteHelperAllocatorDmaCreateArraySimpleWithData(RedRteHandleHelperAllocatorDma helperAllocatorDma, RedHandleCalls callsSetToUploadArrayData, uint64_t arrayDataBytesCount, const void * arrayData, unsigned arrayUsageFlags, unsigned memoryPropertyFlagsSuggestedDefaultValueIsMemoryPropertyDeviceLocalBit, RedRteHelperArray * outRteHelperArray, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters) {
  nvvk::ResourceAllocatorDma * allocator = (nvvk::ResourceAllocatorDma *)helperAllocatorDma;

  bool success = true;

  VkCommandBuffer commandBuffer = (VkCommandBuffer)callsSetToUploadArrayData;
  nvvk::Buffer buffer = allocator->createBuffer(commandBuffer, arrayDataBytesCount, arrayData, arrayUsageFlags, memoryPropertyFlagsSuggestedDefaultValueIsMemoryPropertyDeviceLocalBit);
  if (buffer.buffer == 0) {
    success = false;
  }

  outRteHelperArray->handle = (RedHandleArray)buffer.buffer;
  outRteHelperArray->rteHelperAllocatorResourceMemory = buffer.memHandle;

  return success == true ? 0 : -1;
}

int64_t redRteHelperAllocatorDmaCreateImage(RedRteHandleHelperAllocatorDma helperAllocatorDma, const RedRteHelperCreateImageInfo * imageInfo, unsigned memoryPropertyFlagsSuggestedDefaultValueIsMemoryPropertyDeviceLocalBit, RedRteHelperImage * outRteHelperImage, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters) {
  nvvk::ResourceAllocatorDma * allocator = (nvvk::ResourceAllocatorDma *)helperAllocatorDma;

  bool success = true;

  const VkImageCreateInfo * info_p = (const VkImageCreateInfo *)imageInfo;
  VkImageCreateInfo         info   = info_p[0];
  nvvk::Image image = allocator->createImage(info, memoryPropertyFlagsSuggestedDefaultValueIsMemoryPropertyDeviceLocalBit);
  if (image.image == 0) {
    success = false;
  }

  outRteHelperImage->handle = (RedHandleImage)image.image;
  outRteHelperImage->rteHelperAllocatorResourceMemory = image.memHandle;

  return success == true ? 0 : -1;
}

int64_t redRteHelperAllocatorDmaCreateImageSimpleWithData(RedRteHandleHelperAllocatorDma helperAllocatorDma, RedHandleCalls callsSetToUploadImageData, uint64_t imageDataBytesCount, const void * imageData, const RedRteHelperCreateImageInfo * imageInfo, unsigned imageLayoutSuggestedDefaultValueIsImageLayoutGeneral, RedRteHelperImage * outRteHelperImage, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters) {
  nvvk::ResourceAllocatorDma * allocator = (nvvk::ResourceAllocatorDma *)helperAllocatorDma;

  bool success = true;

  VkCommandBuffer           commandBuffer = (VkCommandBuffer)callsSetToUploadImageData;
  const VkImageCreateInfo * info_p        = (const VkImageCreateInfo *)imageInfo;
  VkImageCreateInfo         info          = info_p[0];
  VkImageLayout             layout        = (VkImageLayout)imageLayoutSuggestedDefaultValueIsImageLayoutGeneral;
  nvvk::Image image = allocator->createImage(commandBuffer, imageDataBytesCount, imageData, info, layout);
  if (image.image == 0) {
    success = false;
  }

  outRteHelperImage->handle = (RedHandleImage)image.image;
  outRteHelperImage->rteHelperAllocatorResourceMemory = image.memHandle;

  return success == true ? 0 : -1;
}

int64_t redRteHelperAllocatorDmaFinalizeAndReleaseStagingMemory(RedRteHandleHelperAllocatorDma helperAllocatorDma, RedHandleCpuSignal optionalCpuSignal, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters) {
  nvvk::ResourceAllocatorDma * allocator = (nvvk::ResourceAllocatorDma *)helperAllocatorDma;

  allocator->finalizeAndReleaseStaging((VkFence)optionalCpuSignal);

  return 0;
}

int64_t redRteHelperAllocatorDmaFinalizeStagingMemory(RedRteHandleHelperAllocatorDma helperAllocatorDma, RedHandleCpuSignal optionalCpuSignal, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters) {
  nvvk::ResourceAllocatorDma * allocator = (nvvk::ResourceAllocatorDma *)helperAllocatorDma;
  
  allocator->finalizeStaging((VkFence)optionalCpuSignal);

  return 0;
}

int64_t redRteHelperAllocatorDmaReleaseStagingMemory(RedRteHandleHelperAllocatorDma helperAllocatorDma, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters) {
  nvvk::ResourceAllocatorDma * allocator = (nvvk::ResourceAllocatorDma *)helperAllocatorDma;

  allocator->releaseStaging();

  return 0;
}

int64_t redRteHelperAllocatorDmaMapArray(RedRteHandleHelperAllocatorDma helperAllocatorDma, RedHandleArray rteHelperArray, void * rteHelperAllocatorResourceMemory, void ** outVolatilePointer, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters) {
  nvvk::ResourceAllocatorDma * allocator = (nvvk::ResourceAllocatorDma *)helperAllocatorDma;

  nvvk::Buffer nvvkbuffer = nvvk::Buffer();
  nvvkbuffer.buffer    = (VkBuffer)rteHelperArray;
  nvvkbuffer.memHandle = (nvvk::MemHandle)rteHelperAllocatorResourceMemory;
  outVolatilePointer[0] = allocator->map(nvvkbuffer);

  return 0;
}

int64_t redRteHelperAllocatorDmaUnmapArray(RedRteHandleHelperAllocatorDma helperAllocatorDma, RedHandleArray rteHelperArray, void * rteHelperAllocatorResourceMemory, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters) {
  nvvk::ResourceAllocatorDma * allocator = (nvvk::ResourceAllocatorDma *)helperAllocatorDma;

  nvvk::Buffer nvvkbuffer = nvvk::Buffer();
  nvvkbuffer.buffer    = (VkBuffer)rteHelperArray;
  nvvkbuffer.memHandle = (nvvk::MemHandle)rteHelperAllocatorResourceMemory;
  allocator->unmap(nvvkbuffer);

  return 0;
}

int64_t redRteHelperAllocatorDmaDestroyArray(RedRteHandleHelperAllocatorDma helperAllocatorDma, RedHandleArray rteHelperArray, void * rteHelperAllocatorResourceMemory, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters) {
  nvvk::ResourceAllocatorDma * allocator = (nvvk::ResourceAllocatorDma *)helperAllocatorDma;

  nvvk::Buffer nvvkbuffer = nvvk::Buffer();
  nvvkbuffer.buffer    = (VkBuffer)rteHelperArray;
  nvvkbuffer.memHandle = (nvvk::MemHandle)rteHelperAllocatorResourceMemory;
  allocator->destroy(nvvkbuffer);

  return 0;
}

int64_t redRteHelperAllocatorDmaDestroyImage(RedRteHandleHelperAllocatorDma helperAllocatorDma, RedHandleImage rteHelperImage, void * rteHelperAllocatorResourceMemory, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters) {
  nvvk::ResourceAllocatorDma * allocator = (nvvk::ResourceAllocatorDma *)helperAllocatorDma;

  nvvk::Image nvvkimage = nvvk::Image();
  nvvkimage.image     = (VkImage)rteHelperImage;
  nvvkimage.memHandle = (nvvk::MemHandle)rteHelperAllocatorResourceMemory;
  allocator->destroy(nvvkimage);

  return 0;
}

// Helper allocator VMA

int64_t redRteCreateHelperAllocatorVma(RedRteHandleContext rteContext, RedRteHandleHelperAllocatorVma * outHelperAllocatorVma, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters) {
  nvvk::Context * nvvkctx = (nvvk::Context *)rteContext;
  
  bool success = true;
  
  nvvk::ResourceAllocatorVma * allocator = new(std::nothrow) nvvk::ResourceAllocatorVma();
  if (allocator == 0) {
    success = false;
  }

  if (success == true) {
    allocator->init(nvvkctx->m_instance, nvvkctx->m_device, nvvkctx->m_physicalDevice);
  }

  outHelperAllocatorVma[0] = (RedRteHandleHelperAllocatorVma)allocator;

  return success == true ? 0 : -1;
}

int64_t redRteDestroyHelperAllocatorVma(RedRteHandleHelperAllocatorVma helperAllocatorVma, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters) {
  nvvk::ResourceAllocatorVma * allocator = (nvvk::ResourceAllocatorVma *)helperAllocatorVma;
  
  if (allocator != 0) {
    allocator->deinit();
    delete allocator;
    allocator = 0;
  }

  return 0;
}

int64_t redRteHelperAllocatorVmaCreateArray(RedRteHandleHelperAllocatorVma helperAllocatorVma, const RedRteHelperCreateArrayInfo * arrayInfo, unsigned memoryPropertyFlagsSuggestedDefaultValueIsMemoryPropertyDeviceLocalBit, RedRteHelperArray * outRteHelperArray, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters) {
  nvvk::ResourceAllocatorVma * allocator = (nvvk::ResourceAllocatorVma *)helperAllocatorVma;

  bool success = true;

  const VkBufferCreateInfo * info_p = (const VkBufferCreateInfo *)arrayInfo;
  VkBufferCreateInfo         info   = info_p[0];
  nvvk::Buffer buffer = allocator->createBuffer(info, memoryPropertyFlagsSuggestedDefaultValueIsMemoryPropertyDeviceLocalBit);
  if (buffer.buffer == 0) {
    success = false;
  }

  outRteHelperArray->handle = (RedHandleArray)buffer.buffer;
  outRteHelperArray->rteHelperAllocatorResourceMemory = buffer.memHandle;

  return success == true ? 0 : -1;
}

int64_t redRteHelperAllocatorVmaCreateArraySimple(RedRteHandleHelperAllocatorVma helperAllocatorVma, uint64_t arrayBytesCount, unsigned arrayUsageFlags, unsigned memoryPropertyFlagsSuggestedDefaultValueIsMemoryPropertyDeviceLocalBit, RedRteHelperArray * outRteHelperArray, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters) {
  nvvk::ResourceAllocatorVma * allocator = (nvvk::ResourceAllocatorVma *)helperAllocatorVma;

  bool success = true;

  nvvk::Buffer buffer = allocator->createBuffer(arrayBytesCount, arrayUsageFlags, memoryPropertyFlagsSuggestedDefaultValueIsMemoryPropertyDeviceLocalBit);
  if (buffer.buffer == 0) {
    success = false;
  }

  outRteHelperArray->handle = (RedHandleArray)buffer.buffer;
  outRteHelperArray->rteHelperAllocatorResourceMemory = buffer.memHandle;

  return success == true ? 0 : -1;
}

int64_t redRteHelperAllocatorVmaCreateArraySimpleWithData(RedRteHandleHelperAllocatorVma helperAllocatorVma, RedHandleCalls callsSetToUploadArrayData, uint64_t arrayDataBytesCount, const void * arrayData, unsigned arrayUsageFlags, unsigned memoryPropertyFlagsSuggestedDefaultValueIsMemoryPropertyDeviceLocalBit, RedRteHelperArray * outRteHelperArray, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters) {
  nvvk::ResourceAllocatorVma * allocator = (nvvk::ResourceAllocatorVma *)helperAllocatorVma;

  bool success = true;

  VkCommandBuffer commandBuffer = (VkCommandBuffer)callsSetToUploadArrayData;
  nvvk::Buffer buffer = allocator->createBuffer(commandBuffer, arrayDataBytesCount, arrayData, arrayUsageFlags, memoryPropertyFlagsSuggestedDefaultValueIsMemoryPropertyDeviceLocalBit);
  if (buffer.buffer == 0) {
    success = false;
  }

  outRteHelperArray->handle = (RedHandleArray)buffer.buffer;
  outRteHelperArray->rteHelperAllocatorResourceMemory = buffer.memHandle;

  return success == true ? 0 : -1;
}

int64_t redRteHelperAllocatorVmaCreateImage(RedRteHandleHelperAllocatorVma helperAllocatorVma, const RedRteHelperCreateImageInfo * imageInfo, unsigned memoryPropertyFlagsSuggestedDefaultValueIsMemoryPropertyDeviceLocalBit, RedRteHelperImage * outRteHelperImage, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters) {
  nvvk::ResourceAllocatorVma * allocator = (nvvk::ResourceAllocatorVma *)helperAllocatorVma;

  bool success = true;

  const VkImageCreateInfo * info_p = (const VkImageCreateInfo *)imageInfo;
  VkImageCreateInfo         info   = info_p[0];
  nvvk::Image image = allocator->createImage(info, memoryPropertyFlagsSuggestedDefaultValueIsMemoryPropertyDeviceLocalBit);
  if (image.image == 0) {
    success = false;
  }

  outRteHelperImage->handle = (RedHandleImage)image.image;
  outRteHelperImage->rteHelperAllocatorResourceMemory = image.memHandle;

  return success == true ? 0 : -1;
}

int64_t redRteHelperAllocatorVmaCreateImageSimpleWithData(RedRteHandleHelperAllocatorVma helperAllocatorVma, RedHandleCalls callsSetToUploadImageData, uint64_t imageDataBytesCount, const void * imageData, const RedRteHelperCreateImageInfo * imageInfo, unsigned imageLayoutSuggestedDefaultValueIsImageLayoutGeneral, RedRteHelperImage * outRteHelperImage, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters) {
  nvvk::ResourceAllocatorVma * allocator = (nvvk::ResourceAllocatorVma *)helperAllocatorVma;

  bool success = true;

  VkCommandBuffer           commandBuffer = (VkCommandBuffer)callsSetToUploadImageData;
  const VkImageCreateInfo * info_p        = (const VkImageCreateInfo *)imageInfo;
  VkImageCreateInfo         info          = info_p[0];
  VkImageLayout             layout        = (VkImageLayout)imageLayoutSuggestedDefaultValueIsImageLayoutGeneral;
  nvvk::Image image = allocator->createImage(commandBuffer, imageDataBytesCount, imageData, info, layout);
  if (image.image == 0) {
    success = false;
  }

  outRteHelperImage->handle = (RedHandleImage)image.image;
  outRteHelperImage->rteHelperAllocatorResourceMemory = image.memHandle;

  return success == true ? 0 : -1;
}

int64_t redRteHelperAllocatorVmaFinalizeAndReleaseStagingMemory(RedRteHandleHelperAllocatorVma helperAllocatorVma, RedHandleCpuSignal optionalCpuSignal, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters) {
  nvvk::ResourceAllocatorVma * allocator = (nvvk::ResourceAllocatorVma *)helperAllocatorVma;

  allocator->finalizeAndReleaseStaging((VkFence)optionalCpuSignal);

  return 0;
}

int64_t redRteHelperAllocatorVmaFinalizeStagingMemory(RedRteHandleHelperAllocatorVma helperAllocatorVma, RedHandleCpuSignal optionalCpuSignal, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters) {
  nvvk::ResourceAllocatorVma * allocator = (nvvk::ResourceAllocatorVma *)helperAllocatorVma;
  
  allocator->finalizeStaging((VkFence)optionalCpuSignal);

  return 0;
}

int64_t redRteHelperAllocatorVmaReleaseStagingMemory(RedRteHandleHelperAllocatorVma helperAllocatorVma, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters) {
  nvvk::ResourceAllocatorVma * allocator = (nvvk::ResourceAllocatorVma *)helperAllocatorVma;

  allocator->releaseStaging();

  return 0;
}

int64_t redRteHelperAllocatorVmaMapArray(RedRteHandleHelperAllocatorVma helperAllocatorVma, RedHandleArray rteHelperArray, void * rteHelperAllocatorResourceMemory, void ** outVolatilePointer, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters) {
  nvvk::ResourceAllocatorVma * allocator = (nvvk::ResourceAllocatorVma *)helperAllocatorVma;

  nvvk::Buffer nvvkbuffer = nvvk::Buffer();
  nvvkbuffer.buffer    = (VkBuffer)rteHelperArray;
  nvvkbuffer.memHandle = (nvvk::MemHandle)rteHelperAllocatorResourceMemory;
  outVolatilePointer[0] = allocator->map(nvvkbuffer);

  return 0;
}

int64_t redRteHelperAllocatorVmaUnmapArray(RedRteHandleHelperAllocatorVma helperAllocatorVma, RedHandleArray rteHelperArray, void * rteHelperAllocatorResourceMemory, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters) {
  nvvk::ResourceAllocatorVma * allocator = (nvvk::ResourceAllocatorVma *)helperAllocatorVma;

  nvvk::Buffer nvvkbuffer = nvvk::Buffer();
  nvvkbuffer.buffer    = (VkBuffer)rteHelperArray;
  nvvkbuffer.memHandle = (nvvk::MemHandle)rteHelperAllocatorResourceMemory;
  allocator->unmap(nvvkbuffer);

  return 0;
}

int64_t redRteHelperAllocatorVmaDestroyArray(RedRteHandleHelperAllocatorVma helperAllocatorVma, RedHandleArray rteHelperArray, void * rteHelperAllocatorResourceMemory, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters) {
  nvvk::ResourceAllocatorVma * allocator = (nvvk::ResourceAllocatorVma *)helperAllocatorVma;

  nvvk::Buffer nvvkbuffer = nvvk::Buffer();
  nvvkbuffer.buffer    = (VkBuffer)rteHelperArray;
  nvvkbuffer.memHandle = (nvvk::MemHandle)rteHelperAllocatorResourceMemory;
  allocator->destroy(nvvkbuffer);

  return 0;
}

int64_t redRteHelperAllocatorVmaDestroyImage(RedRteHandleHelperAllocatorVma helperAllocatorVma, RedHandleImage rteHelperImage, void * rteHelperAllocatorResourceMemory, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters) {
  nvvk::ResourceAllocatorVma * allocator = (nvvk::ResourceAllocatorVma *)helperAllocatorVma;

  nvvk::Image nvvkimage = nvvk::Image();
  nvvkimage.image     = (VkImage)rteHelperImage;
  nvvkimage.memHandle = (nvvk::MemHandle)rteHelperAllocatorResourceMemory;
  allocator->destroy(nvvkimage);

  return 0;
}

// Helper structs

int64_t redRteCreateHelperStructs(RedRteHandleContext rteContext, RedRteHandleHelperStructs * outHelperStructs, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters) {
  nvvk::Context * nvvkctx = (nvvk::Context *)rteContext;
  
  bool success = true;
  
  nvvk::DescriptorSetContainer * structs = new(std::nothrow) nvvk::DescriptorSetContainer();
  if (structs == 0) {
    success = false;
  }

  if (success == true) {
    structs->init(nvvkctx->m_device);
  }

  outHelperStructs[0] = (RedRteHandleHelperStructs)structs;

  return success == true ? 0 : -1;
}

int64_t redRteDestroyHelperStructs(RedRteHandleHelperStructs helperStructs, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters) {
  nvvk::DescriptorSetContainer * structs = (nvvk::DescriptorSetContainer *)helperStructs;
  
  if (structs != 0) {
    structs->deinit();
    delete structs;
    structs = 0;
  }

  return 0;
}

int64_t redRteHelperStructsSetStructDeclarationMembers(RedRteHandleHelperStructs helperStructs, unsigned structDeclarationMembersCount, const RedStructDeclarationMember * structDeclarationMembers, unsigned structDeclarationMembersArrayROCount, const RedStructDeclarationMemberArrayRO * structDeclarationMembersArrayRO, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters) {
  nvvk::DescriptorSetContainer * structs = (nvvk::DescriptorSetContainer *)helperStructs;

  const VkDescriptorSetLayoutBinding * bindings = (const VkDescriptorSetLayoutBinding *)structDeclarationMembers;
  std::vector<VkDescriptorSetLayoutBinding> bindingsVector(bindings, bindings + structDeclarationMembersCount);
  structs->setBindings(bindingsVector);

  return 0;
}

int64_t redRteHelperStructsAddStructDeclarationMember(RedRteHandleHelperStructs helperStructs, unsigned slot, unsigned type, unsigned count, unsigned visibleToStages, const RedHandleSampler * inlineSampler, RedBool32 slotTypeIsArrayRO, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters) {
  nvvk::DescriptorSetContainer * structs = (nvvk::DescriptorSetContainer *)helperStructs;

  structs->addBinding(slot, (VkDescriptorType)type, count, visibleToStages, (const VkSampler *)inlineSampler);

  return 0;
}

int64_t redRteHelperStructsSetStructDeclarationMemberFlags(RedRteHandleHelperStructs helperStructs, unsigned slot, unsigned descriptorBindingFlags_RequiresApiSupportGreaterThanVkVersion10, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters) {
  nvvk::DescriptorSetContainer * structs = (nvvk::DescriptorSetContainer *)helperStructs;

  structs->setBindingFlags(slot, descriptorBindingFlags_RequiresApiSupportGreaterThanVkVersion10);

  return 0;
}

int64_t redRteHelperStructsCreateStructDeclaration(RedRteHandleHelperStructs helperStructs, unsigned descriptorSetLayoutCreateFlags, RedRteHelperStructsApiSupport apiSupport, RedHandleStructDeclaration * outStructDeclaration, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters) {
  nvvk::DescriptorSetContainer * structs = (nvvk::DescriptorSetContainer *)helperStructs;

  VkDescriptorSetLayout descriptorSetLayout = structs->initLayout(descriptorSetLayoutCreateFlags, (nvvk::DescriptorSupport)apiSupport);

  outStructDeclaration[0] = (RedHandleStructDeclaration)descriptorSetLayout;

  return descriptorSetLayout != 0 ? 0 : -1;
}

int64_t redRteHelperStructsCreateStructsMemory(RedRteHandleHelperStructs helperStructs, unsigned allocateStructsCount, RedHandleStructsMemory * outStructsMemory, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters) {
  nvvk::DescriptorSetContainer * structs = (nvvk::DescriptorSetContainer *)helperStructs;

  VkDescriptorPool descriptorPool = structs->initPool(allocateStructsCount);

  outStructsMemory[0] = (RedHandleStructsMemory)descriptorPool;

  return descriptorPool != 0 ? 0 : -1;
}

int64_t redRteHelperStructsDestroyStructDeclaration(RedRteHandleHelperStructs helperStructs, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters) {
  nvvk::DescriptorSetContainer * structs = (nvvk::DescriptorSetContainer *)helperStructs;

  structs->deinitLayout();

  return 0;
}

int64_t redRteHelperStructsDestroyStructsMemory(RedRteHandleHelperStructs helperStructs, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters) {
  nvvk::DescriptorSetContainer * structs = (nvvk::DescriptorSetContainer *)helperStructs;

  structs->deinitPool();

  return 0;
}

RedHandleStruct redRteHelperStructsGetStruct(RedRteHandleHelperStructs helperStructs, unsigned structIndex, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters) {
  nvvk::DescriptorSetContainer * structs = (nvvk::DescriptorSetContainer *)helperStructs;

  return (RedHandleStruct)structs->getSet(structIndex);
}

unsigned redRteHelperStructsGetStructsCount(RedRteHandleHelperStructs helperStructs, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters) {
  nvvk::DescriptorSetContainer * structs = (nvvk::DescriptorSetContainer *)helperStructs;

  return structs->getSetsCount();
}

RedHandleStruct * redRteHelperStructsGetStructs(RedRteHandleHelperStructs helperStructs, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters) {
  nvvk::DescriptorSetContainer * structs = (nvvk::DescriptorSetContainer *)helperStructs;

  return (RedHandleStruct *)structs->getSets();
}

int64_t redRteHelperStructsClear(RedRteHandleHelperStructs helperStructs, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters) {
  nvvk::DescriptorSetContainer * structs = (nvvk::DescriptorSetContainer *)helperStructs;

  structs->deinit();

  return 0;
}

unsigned redRteHelperStructsGetStructDeclarationMembersCount(RedRteHandleHelperStructs helperStructs, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters) {
  nvvk::DescriptorSetContainer * structs = (nvvk::DescriptorSetContainer *)helperStructs;

  return (unsigned)structs->getBindings().size();
}

RedStructDeclarationMember * redRteHelperStructsGetStructDeclarationMembers(RedRteHandleHelperStructs helperStructs, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters) {
  nvvk::DescriptorSetContainer * structs = (nvvk::DescriptorSetContainer *)helperStructs;

  return (RedStructDeclarationMember *)structs->getBindings().data();
}

// Ray tracing

int64_t redRteCreateRayTracingBuilder(RedRteHandleContext rteContext, void * rteHandleHelperAllocator, unsigned queueFamilyIndex, RedRteHandleRayTracingBuilder * outRayTracingBuilder, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters) {
  nvvk::Context * nvvkctx = (nvvk::Context *)rteContext;
  
  bool success = true;
  
  nvvk::RaytracingBuilderKHR * builder = new(std::nothrow) nvvk::RaytracingBuilderKHR();
  if (builder == 0) {
    success = false;
  }

  if (success == true) {
    builder->setup(nvvkctx->m_device, (nvvk::ResourceAllocator *)rteHandleHelperAllocator, queueFamilyIndex);
  }

  outRayTracingBuilder[0] = (RedRteHandleRayTracingBuilder)builder;

  return success == true ? 0 : -1;
}

int64_t redRteDestroyRayTracingBuilder(RedRteHandleRayTracingBuilder rayTracingBuilder, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters) {
  nvvk::RaytracingBuilderKHR * builder = (nvvk::RaytracingBuilderKHR *)rayTracingBuilder;
  
  if (builder != 0) {
    builder->destroy();
    delete builder;
    builder = 0;
  }

  return 0;
}

int64_t redRteRayTracingBuilderBuildBlas(RedRteHandleRayTracingBuilder rayTracingBuilder, unsigned blasInputsCount, const RedRteRayTracingBuilderBlasInput * blasInputs, unsigned buildAccelerationStructureFlagsSuggestedDefaultValueIsBuildAccelerationStructurePreferFastTraceBit, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters) {
  nvvk::RaytracingBuilderKHR * builder = (nvvk::RaytracingBuilderKHR *)rayTracingBuilder;

  bool success = true;

  std::vector<nvvk::RaytracingBuilderKHR::BlasInput> blasInputsVector = std::vector<nvvk::RaytracingBuilderKHR::BlasInput>();
  blasInputsVector.reserve(blasInputsCount);
  for (unsigned i = 0; i < blasInputsCount; i += 1) {
    RedRteRayTracingBuilderBlasInput blas = blasInputs[i];

    const VkAccelerationStructureGeometryKHR       * asGeometrys       = (const VkAccelerationStructureGeometryKHR       *)blas.geometrys;
    const VkAccelerationStructureBuildRangeInfoKHR * asBuildRangeInfos = (const VkAccelerationStructureBuildRangeInfoKHR *)blas.buildRangeInfos;

    nvvk::RaytracingBuilderKHR::BlasInput blasInput = nvvk::RaytracingBuilderKHR::BlasInput();
    blasInput.asGeometry        = std::vector<VkAccelerationStructureGeometryKHR>(asGeometrys, asGeometrys + blas.geometrysCount);
    blasInput.asBuildOffsetInfo = std::vector<VkAccelerationStructureBuildRangeInfoKHR>(asBuildRangeInfos, asBuildRangeInfos + blas.buildRangeInfosCount);
    blasInput.flags             = (VkBuildAccelerationStructureFlagsKHR)blas.flags;
    blasInputsVector.push_back(blasInput);
  }

  builder->buildBlas(blasInputsVector, buildAccelerationStructureFlagsSuggestedDefaultValueIsBuildAccelerationStructurePreferFastTraceBit);

  return success == true ? 0 : -1;
}

uint64_t redRteRayTracingBuilderGetBlasGpuAddress(RedRteHandleRayTracingBuilder rayTracingBuilder, unsigned blasIndex, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters) {
  nvvk::RaytracingBuilderKHR * builder = (nvvk::RaytracingBuilderKHR *)rayTracingBuilder;

  return builder->getBlasDeviceAddress(blasIndex);
}

int64_t redRteRayTracingBuilderBuildTlas(RedRteHandleRayTracingBuilder rayTracingBuilder, unsigned instancesCount, const RedRteRayTracingAccelerationStructureInstance * instances, unsigned buildAccelerationStructureFlagsSuggestedDefaultValueIsBuildAccelerationStructurePreferFastTraceBit, RedBool32 update, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters) {
  nvvk::RaytracingBuilderKHR * builder = (nvvk::RaytracingBuilderKHR *)rayTracingBuilder;

  bool success = true;

  const VkAccelerationStructureInstanceKHR * asInstances = (const VkAccelerationStructureInstanceKHR *)instances;
  std::vector<VkAccelerationStructureInstanceKHR> instancesVector = std::vector<VkAccelerationStructureInstanceKHR>(asInstances, asInstances + instancesCount);
  builder->buildTlas(instancesVector, buildAccelerationStructureFlagsSuggestedDefaultValueIsBuildAccelerationStructurePreferFastTraceBit, update == 1 ? true : false);

  return success == true ? 0 : -1;
}

RedRteHandleRayTracingAccelerationStructure redRteRayTracingBuilderGetTlas(RedRteHandleRayTracingBuilder rayTracingBuilder, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters) {
  nvvk::RaytracingBuilderKHR * builder = (nvvk::RaytracingBuilderKHR *)rayTracingBuilder;

  return (RedRteHandleRayTracingAccelerationStructure)builder->getAccelerationStructure();
}

int64_t redRteRayTracingBuilderUpdateBlas(RedRteHandleRayTracingBuilder rayTracingBuilder, unsigned blasIndex, const RedRteRayTracingBuilderBlasInput * blasInput, unsigned buildAccelerationStructureFlagsSuggestedDefaultValueIsBuildAccelerationStructurePreferFastTraceBit, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters) {
  nvvk::RaytracingBuilderKHR * builder = (nvvk::RaytracingBuilderKHR *)rayTracingBuilder;

  bool success = true;

  const nvvk::RaytracingBuilderKHR::BlasInput * nvvkBlasInput = (const nvvk::RaytracingBuilderKHR::BlasInput *)blasInput;
  nvvk::RaytracingBuilderKHR::BlasInput blas = nvvkBlasInput[0];
  builder->updateBlas(blasIndex, blas, buildAccelerationStructureFlagsSuggestedDefaultValueIsBuildAccelerationStructurePreferFastTraceBit);

  return success == true ? 0 : -1;
}

// Ray tracing procedure

int64_t redRteCreateDeferredOperation(RedRteHandleContext rteContext, RedRteHandleDeferredOperation * outDeferredOperation, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters) {
  nvvk::Context * nvvkctx = (nvvk::Context *)rteContext;

  return (int64_t)vkCreateDeferredOperationKHR(nvvkctx->m_device, 0, (VkDeferredOperationKHR *)outDeferredOperation);
}

int64_t redRteDestroyDeferredOperation(RedRteHandleContext rteContext, RedRteHandleDeferredOperation deferredOperation, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters) {
  nvvk::Context * nvvkctx = (nvvk::Context *)rteContext;

  vkDestroyDeferredOperationKHR(nvvkctx->m_device, (VkDeferredOperationKHR)deferredOperation, 0);

  return 0;
}

int64_t redRteDeferredOperationGetMaxConcurrency(RedRteHandleContext rteContext, RedRteHandleDeferredOperation deferredOperation, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters) {
  nvvk::Context * nvvkctx = (nvvk::Context *)rteContext;

  return vkGetDeferredOperationMaxConcurrencyKHR(nvvkctx->m_device, (VkDeferredOperationKHR)deferredOperation);
}

int64_t redRteDeferredOperationGetStatus(RedRteHandleContext rteContext, RedRteHandleDeferredOperation deferredOperation, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters) {
  nvvk::Context * nvvkctx = (nvvk::Context *)rteContext;

  return (int64_t)vkGetDeferredOperationResultKHR(nvvkctx->m_device, (VkDeferredOperationKHR)deferredOperation);
}

int64_t redRteDeferredOperationJoin(RedRteHandleContext rteContext, RedRteHandleDeferredOperation deferredOperation, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters) {
  nvvk::Context * nvvkctx = (nvvk::Context *)rteContext;

  return (int64_t)vkDeferredOperationJoinKHR(nvvkctx->m_device, (VkDeferredOperationKHR)deferredOperation);
}

int64_t redRteCreateProcedureRayTracing(RedRteHandleContext rteContext, RedRteHandleDeferredOperation deferredOperation, RedHandleProcedureCache procedureCache, const RedRteProcedureRayTracingState * state, RedHandleProcedure * outProcedure, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters) {
  nvvk::Context * nvvkctx = (nvvk::Context *)rteContext;
  
  return (int64_t)vkCreateRayTracingPipelinesKHR(nvvkctx->m_device, (VkDeferredOperationKHR)deferredOperation, (VkPipelineCache)procedureCache, 1, (const VkRayTracingPipelineCreateInfoKHR *)state, 0, (VkPipeline *)outProcedure);
}

int64_t redRteProcedureRayTracingGetGroupCaptureReplayHandles(RedRteHandleContext rteContext, RedHandleProcedure procedureRayTracing, unsigned groupIndexFirst, unsigned groupIndexCount, uint64_t dataBytesCount, void * data, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters) {
  nvvk::Context * nvvkctx = (nvvk::Context *)rteContext;
  
  return (int64_t)vkGetRayTracingCaptureReplayShaderGroupHandlesKHR(nvvkctx->m_device, (VkPipeline)procedureRayTracing, groupIndexFirst, groupIndexCount, dataBytesCount, data);
}

uint64_t redRteProcedureRayTracingGetGroupStackBytesCount(RedRteHandleContext rteContext, RedHandleProcedure procedureRayTracing, unsigned groupIndex, RedRteProcedureRayTracingGroupStage groupStage, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters) {
  nvvk::Context * nvvkctx = (nvvk::Context *)rteContext;

  return vkGetRayTracingShaderGroupStackSizeKHR((VkDevice)nvvkctx->m_device, (VkPipeline)procedureRayTracing, groupIndex, (VkShaderGroupShaderKHR)groupStage);
}

int64_t redRteCallProcedureRayTracing(RedHandleCalls calls, RedRteHandleContext rteContext, const RedRteGpuAddressStrideAndRange * shaderBindingTableRayGeneration, const RedRteGpuAddressStrideAndRange * shaderBindingTableMiss, const RedRteGpuAddressStrideAndRange * shaderBindingTableHit, const RedRteGpuAddressStrideAndRange * shaderBindingTableCallable, unsigned threadsCountX, unsigned threadsCountY, unsigned threadsCountZ, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters) {
  vkCmdTraceRaysKHR((VkCommandBuffer)calls, (const VkStridedDeviceAddressRegionKHR *)shaderBindingTableRayGeneration, (const VkStridedDeviceAddressRegionKHR *)shaderBindingTableMiss, (const VkStridedDeviceAddressRegionKHR *)shaderBindingTableHit, (const VkStridedDeviceAddressRegionKHR *)shaderBindingTableCallable, threadsCountX, threadsCountY, threadsCountZ);

  return 0;
}

int64_t redRteCallProcedureIndirect(RedHandleCalls calls, RedRteHandleContext rteContext, RedHandleArray array, uint64_t arrayBytesFirst, unsigned count, unsigned bytesStride, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters) {
  vkCmdDrawIndirect((VkCommandBuffer)calls, (VkBuffer)array, arrayBytesFirst, count, bytesStride);

  return 0;
}

int64_t redRteCallProcedureIndexedIndirect(RedHandleCalls calls, RedRteHandleContext rteContext, RedHandleArray array, uint64_t arrayBytesFirst, unsigned count, unsigned bytesStride, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters) {
  vkCmdDrawIndexedIndirect((VkCommandBuffer)calls, (VkBuffer)array, arrayBytesFirst, count, bytesStride);

  return 0;
}

int64_t redRteCallProcedureComputeIndirect(RedHandleCalls calls, RedRteHandleContext rteContext, RedHandleArray array, uint64_t arrayBytesFirst, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters) {
  vkCmdDispatchIndirect((VkCommandBuffer)calls, (VkBuffer)array, arrayBytesFirst);

  return 0;
}

int64_t redRteCallProcedureRayTracingIndirect(RedHandleCalls calls, RedRteHandleContext rteContext, const RedRteGpuAddressStrideAndRange * shaderBindingTableRayGeneration, const RedRteGpuAddressStrideAndRange * shaderBindingTableMiss, const RedRteGpuAddressStrideAndRange * shaderBindingTableHit, const RedRteGpuAddressStrideAndRange * shaderBindingTableCallable, uint64_t gpuAddress, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters) {
  vkCmdTraceRaysIndirectKHR((VkCommandBuffer)calls, (const VkStridedDeviceAddressRegionKHR *)shaderBindingTableRayGeneration, (const VkStridedDeviceAddressRegionKHR *)shaderBindingTableMiss, (const VkStridedDeviceAddressRegionKHR *)shaderBindingTableHit, (const VkStridedDeviceAddressRegionKHR *)shaderBindingTableCallable, gpuAddress);

  return 0;
}

int64_t redRteCallSetProcedureRayTracingStackBytesCount(RedHandleCalls calls, RedRteHandleContext rteContext, unsigned stackBytesCount, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters) {
  vkCmdSetRayTracingPipelineStackSizeKHR((VkCommandBuffer)calls, stackBytesCount);

  return 0;
}

// Helper shader binding table

int64_t redRteCreateHelperShaderBindingTable(RedRteHandleContext rteContext, void * rteHandleHelperAllocator, unsigned queueFamilyIndex, RedRteHandleHelperShaderBindingTable * outHelperShaderBindingTable, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters) {
  nvvk::Context * nvvkctx = (nvvk::Context *)rteContext;
  
  bool success = true;
  
  nvvk::SBTWrapper * sbt = new(std::nothrow) nvvk::SBTWrapper();
  if (sbt == 0) {
    success = false;
  }

  if (success == true) {
    VkPhysicalDeviceRayTracingPipelinePropertiesKHR rtPipelineProperties = {VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RAY_TRACING_PIPELINE_PROPERTIES_KHR};
    VkPhysicalDeviceProperties2                     properties2          = {VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2};
    properties2.pNext = &rtPipelineProperties;
    vkGetPhysicalDeviceProperties2(nvvkctx->m_physicalDevice, &properties2);
    sbt->setup(nvvkctx->m_device, queueFamilyIndex, (nvvk::ResourceAllocator *)rteHandleHelperAllocator, rtPipelineProperties);
  }

  outHelperShaderBindingTable[0] = (RedRteHandleHelperShaderBindingTable)sbt;

  return success == true ? 0 : -1;
}

int64_t redRteDestroyHelperShaderBindingTable(RedRteHandleHelperShaderBindingTable helperShaderBindingTable, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters) {
  nvvk::SBTWrapper * sbt = (nvvk::SBTWrapper *)helperShaderBindingTable;
  
  if (sbt != 0) {
    sbt->destroy();
    delete sbt;
    sbt = 0;
  }

  return 0;
}

int64_t redRteHelperShaderBindingTableCreate(RedRteHandleHelperShaderBindingTable helperShaderBindingTable, RedHandleProcedure procedureRayTracing, const RedRteProcedureRayTracingState * procedureRayTracingState, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters) {
  nvvk::SBTWrapper * sbt = (nvvk::SBTWrapper *)helperShaderBindingTable;

  VkRayTracingPipelineCreateInfoKHR * info_p = (VkRayTracingPipelineCreateInfoKHR *)procedureRayTracingState;
  VkRayTracingPipelineCreateInfoKHR   info   = info_p[0];

  sbt->create((VkPipeline)procedureRayTracing, info);

  return 0;
}

int64_t redRteHelperShaderBindingTableGetGpuAddressStrideAndRange(RedRteHandleHelperShaderBindingTable helperShaderBindingTable, RedRteGpuAddressStrideAndRange * outShaderBindingTableRayGeneration, RedRteGpuAddressStrideAndRange * outShaderBindingTableMiss, RedRteGpuAddressStrideAndRange * outShaderBindingTableHit, RedRteGpuAddressStrideAndRange * outShaderBindingTableCallable, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters) {
  nvvk::SBTWrapper * sbt = (nvvk::SBTWrapper *)helperShaderBindingTable;

  const std::array<VkStridedDeviceAddressRegionKHR, 4> regionsArray = sbt->getRegions();
  const VkStridedDeviceAddressRegionKHR *              regions_p    = regionsArray.data();
  const RedRteGpuAddressStrideAndRange *               regions      = (const RedRteGpuAddressStrideAndRange *)regions_p;

  outShaderBindingTableRayGeneration[0] = regions[0];
  outShaderBindingTableMiss[0]          = regions[1];
  outShaderBindingTableHit[0]           = regions[2];
  outShaderBindingTableCallable[0]      = regions[3];

  return 0;
}

// Helper app

class RedRteHelperAppClass : public nvvkhl::IAppElement {
public:
  explicit RedRteHelperAppClass(const RedRteHelperAppCallbacks * callbacks) {
    if (callbacks != 0) {
      m_callbacks = callbacks[0];
    }
  }

  ~RedRteHelperAppClass() override = default;

  void onAttach(nvvkhl::Application * app) override {
    if (m_callbacks.onAttach != 0) {
      m_callbacks.onAttach((RedRteHandleHelperAppWindow)app);
    }
  }

  void onDetach() override {
    if (m_callbacks.onDetach != 0) {
      m_callbacks.onDetach();
    }
  }

  void onResize(uint32_t width, uint32_t height) override {
    if (m_callbacks.onResize != 0) {
      m_callbacks.onResize(width, height);
    }
  }

  void onUIRender() override {
    if (m_callbacks.onUiRender != 0) {
      m_callbacks.onUiRender();
    }
  }

  void onRender(VkCommandBuffer commandBuffer) override {
    if (m_callbacks.onRender != 0) {
      m_callbacks.onRender((RedHandleCalls)commandBuffer);
    }
  }

  void onFileDrop(const char * filename) override {
    if (m_callbacks.onFileDrop != 0) {
      m_callbacks.onFileDrop(filename);
    }
  }

  RedRteHelperAppCallbacks m_callbacks = {};
};

int64_t redRteCreateHelperAppWindow(const RedRteHelperAppCallbacks * callbacks, int initialWindowWidth, int initialWindowHeight, RedBool32 initialIsVsyncEnabled, float initialViewportClearColorR, float initialViewportClearColorG, float initialViewportClearColorB, float initialViewportClearColorA, const char * optionalProgramName, const char * optionalEngineName, RedRteCreateContextParameters * rteCreateContextParameters, RedRteHandleHelperAppWindow * outHelperAppWindow, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters) {
  bool success = true;

  std::string gpuIndexString = std::to_string(rteCreateContextParameters->gpuIndex);
  std::vector<int> ignoreDebugMessages = std::vector<int>();
  if (rteCreateContextParameters->ignoreDebugMessagesCount > 0 && rteCreateContextParameters->ignoreDebugMessages != 0) {
    std::vector<int> ignoreDebugMessagesVector(rteCreateContextParameters->ignoreDebugMessages, rteCreateContextParameters->ignoreDebugMessages + rteCreateContextParameters->ignoreDebugMessagesCount);
    ignoreDebugMessages = ignoreDebugMessagesVector;
  }
  nvvkhl::ApplicationCreateInfo info = nvvkhl::ApplicationCreateInfo();
  info.name              = gpuIndexString;
  info.width             = initialWindowWidth;
  info.height            = initialWindowHeight;
  info.vSync             = initialIsVsyncEnabled == 1 ? true : false;
  info.useMenu           = false;
  info.useDockMenu       = false;
  info.vkSetup           = redRteGetContextCreateInfoFromCreateContextParameters(optionalProgramName, optionalEngineName, rteCreateContextParameters);
  info.ignoreDbgMessages = ignoreDebugMessages;
  info.clearColor.x      = initialViewportClearColorR;
  info.clearColor.y      = initialViewportClearColorG;
  info.clearColor.z      = initialViewportClearColorB;
  info.clearColor.w      = initialViewportClearColorA;
  info.dockSetup         = 0;
  nvvkhl::Application * app = new(std::nothrow) nvvkhl::Application(info);
  if (app == 0) {
    success = false;
  }

  if (success == true) {
    app->addElement(std::make_shared<RedRteHelperAppClass>(callbacks));
  }

  outHelperAppWindow[0] = (RedRteHandleHelperAppWindow)app;

  return success == true ? 0 : -1;
}

int64_t redRteDestroyHelperAppWindow(RedRteHandleHelperAppWindow helperAppWindow, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters) {
  nvvkhl::Application * app = (nvvkhl::Application *)helperAppWindow;
  
  if (app != 0) {
    delete app;
    app = 0;
  }

  return 0;
}

int64_t redRteHelperAppWindowRun(RedRteHandleHelperAppWindow helperAppWindow, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters) {
  nvvkhl::Application * app = (nvvkhl::Application *)helperAppWindow;

  app->run();

  return 0;
}

int64_t redRteHelperAppWindowClose(RedRteHandleHelperAppWindow helperAppWindow, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters) {
  nvvkhl::Application * app = (nvvkhl::Application *)helperAppWindow;

  app->close();

  return 0;
}

int64_t redRteHelperAppWindowSetViewport(RedRteHandleHelperAppWindow helperAppWindow, RedHandleCalls calls, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters) {
  nvvkhl::Application * app = (nvvkhl::Application *)helperAppWindow;

  VkCommandBuffer commandBuffer = (VkCommandBuffer)calls;
  app->setViewport(commandBuffer);

  return 0;
}

RedBool32 redRteHelperAppWindowIsVsyncEnabled(RedRteHandleHelperAppWindow helperAppWindow, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters) {
  nvvkhl::Application * app = (nvvkhl::Application *)helperAppWindow;

  return app->isVsync() == true ? 1 : 0;
}

int64_t redRteHelperAppWindowSetVsyncEnabled(RedRteHandleHelperAppWindow helperAppWindow, RedBool32 enableVsync, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters) {
  nvvkhl::Application * app = (nvvkhl::Application *)helperAppWindow;

  app->setVsync(enableVsync == 1 ? true : false);

  return 0;
}

int64_t redRteHelperAppWindowSetViewportClearColor(RedRteHandleHelperAppWindow helperAppWindow, float viewportClearColorR, float viewportClearColorG, float viewportClearColorB, float viewportClearColorA, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters) {
  nvvkhl::Application * app = (nvvkhl::Application *)helperAppWindow;

  ImVec4 clearColor = ImVec4(viewportClearColorR, viewportClearColorG, viewportClearColorB, viewportClearColorA);
  app->setViewportClearColor(clearColor);

  return 0;
}

int64_t redRteHelperAppWindowAddWaitSemaphore(RedRteHandleHelperAppWindow helperAppWindow, void * vkSemaphoreSubmitInfoKHR, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters) {
  nvvkhl::Application * app = (nvvkhl::Application *)helperAppWindow;

  VkSemaphoreSubmitInfoKHR * info_p = (VkSemaphoreSubmitInfoKHR *)vkSemaphoreSubmitInfoKHR;
  VkSemaphoreSubmitInfoKHR   info   = info_p[0];
  app->addWaitSemaphore(info);

  return 0;
}

RedHandleCalls redRteHelperAppWindowTempCallsCreateAndSet(RedRteHandleHelperAppWindow helperAppWindow, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters) {
  nvvkhl::Application * app = (nvvkhl::Application *)helperAppWindow;

  return (RedHandleCalls)app->createTempCmdBuffer();
}

int64_t redRteHelperAppWindowTempCallsEndSubmitWaitAndDestroy(RedRteHandleHelperAppWindow helperAppWindow, RedHandleCalls tempCalls, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters) {
  nvvkhl::Application * app = (nvvkhl::Application *)helperAppWindow;

  app->submitAndWaitTempCmdBuffer((VkCommandBuffer)tempCalls);

  return 0;
}

int64_t redRteHelperAppWindowOnFileDrop(RedRteHandleHelperAppWindow helperAppWindow, const char * filename, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters) {
  nvvkhl::Application * app = (nvvkhl::Application *)helperAppWindow;

  app->onFileDrop(filename);

  return 0;
}

RedRteHandleContext redRteHelperAppWindowGetRteContext(RedRteHandleHelperAppWindow helperAppWindow, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters) {
  nvvkhl::Application * app = (nvvkhl::Application *)helperAppWindow;

  nvvk::Context * nvvkctx = app->m_context;

  return (RedRteHandleContext)nvvkctx;
}

void * redRteHelperAppWindowGetGlfwWindow(RedRteHandleHelperAppWindow helperAppWindow, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters) {
  nvvkhl::Application * app = (nvvkhl::Application *)helperAppWindow;

  GLFWwindow * glfwWindow = app->getWindowHandle();

  return (void *)glfwWindow;
}

int64_t redRteHelperAppWindowGetViewportWidth(RedRteHandleHelperAppWindow helperAppWindow, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters) {
  nvvkhl::Application * app = (nvvkhl::Application *)helperAppWindow;

  return app->getViewportSize().width;
}

int64_t redRteHelperAppWindowGetViewportHeight(RedRteHandleHelperAppWindow helperAppWindow, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters) {
  nvvkhl::Application * app = (nvvkhl::Application *)helperAppWindow;

  return app->getViewportSize().height;
}

int64_t redRteHelperAppWindowGetWindowWidth(RedRteHandleHelperAppWindow helperAppWindow, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters) {
  nvvkhl::Application * app = (nvvkhl::Application *)helperAppWindow;

  return app->getWindowSize().width;
}

int64_t redRteHelperAppWindowGetWindowHeight(RedRteHandleHelperAppWindow helperAppWindow, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters) {
  nvvkhl::Application * app = (nvvkhl::Application *)helperAppWindow;

  return app->getWindowSize().height;
}

void * redRteHelperAppWindowGetImGuiImplVulkanHWindowData(RedRteHandleHelperAppWindow helperAppWindow, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters) {
  nvvkhl::Application * app = (nvvkhl::Application *)helperAppWindow;

  ImGui_ImplVulkanH_Window * data = app->m_mainWindowData.get();

  return (void *)data;
}

// Helper gbuffer

int64_t redRteCreateHelperGBuffer(RedRteHandleContext rteContext, void * rteHandleHelperAllocator, unsigned width, unsigned height, unsigned colorFormatsCount, unsigned * colorFormats, unsigned depthFormat, RedRteHandleHelperGBuffer * outHelperGBuffer, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters) {
  nvvk::Context * nvvkctx = (nvvk::Context *)rteContext;
  
  bool success = true;
  
  std::vector<VkFormat> colors = std::vector<VkFormat>();
  if (colorFormatsCount > 0 && colorFormats != 0) {
    std::vector<VkFormat> colorsVector = std::vector<VkFormat>((VkFormat *)colorFormats, (VkFormat *)colorFormats + colorFormatsCount);
    colors = colorsVector;
  }
  VkExtent2D extent = {};
  extent.width  = width;
  extent.height = height;

  nvvkhl::GBuffer * gbuffer = new(std::nothrow) nvvkhl::GBuffer(nvvkctx->m_device, (nvvk::ResourceAllocator *)rteHandleHelperAllocator, extent, colors, (VkFormat)depthFormat);
  if (gbuffer == 0) {
    success = false;
  }

  outHelperGBuffer[0] = (RedRteHandleHelperGBuffer)gbuffer;

  return success == true ? 0 : -1;
}

int64_t redRteDestroyHelperGBuffer(RedRteHandleHelperGBuffer helperGBuffer, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters) {
  nvvkhl::GBuffer * gbuffer = (nvvkhl::GBuffer *)helperGBuffer;
  
  if (gbuffer != 0) {
    gbuffer->destroy();
    delete gbuffer;
    gbuffer = 0;
  }

  return 0;
}

RedHandleStruct redRteHelperGBufferGetStruct(RedRteHandleHelperGBuffer helperGBuffer, unsigned index, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters) {
  nvvkhl::GBuffer * gbuffer = (nvvkhl::GBuffer *)helperGBuffer;

  return (RedHandleStruct)gbuffer->getDescriptorSet(index);
}

int64_t redRteHelperGBufferGetWidth(RedRteHandleHelperGBuffer helperGBuffer, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters) {
  nvvkhl::GBuffer * gbuffer = (nvvkhl::GBuffer *)helperGBuffer;

  return gbuffer->getSize().width;
}

int64_t redRteHelperGBufferGetHeight(RedRteHandleHelperGBuffer helperGBuffer, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters) {
  nvvkhl::GBuffer * gbuffer = (nvvkhl::GBuffer *)helperGBuffer;

  return gbuffer->getSize().height;
}

RedHandleImage redRteHelperGBufferGetColorImage(RedRteHandleHelperGBuffer helperGBuffer, unsigned index, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters) {
  nvvkhl::GBuffer * gbuffer = (nvvkhl::GBuffer *)helperGBuffer;

  return (RedHandleImage)gbuffer->getColorImage(index);
}

RedHandleImage redRteHelperGBufferGetDepthImage(RedRteHandleHelperGBuffer helperGBuffer, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters) {
  nvvkhl::GBuffer * gbuffer = (nvvkhl::GBuffer *)helperGBuffer;

  return (RedHandleImage)gbuffer->getDepthImage();
}

int64_t redRteHelperGBufferGetStructMemberTexture(RedRteHandleHelperGBuffer helperGBuffer, unsigned index, RedStructMemberTexture * outStructMemberTexture, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters) {
  nvvkhl::GBuffer * gbuffer = (nvvkhl::GBuffer *)helperGBuffer;

  VkDescriptorImageInfo descriptorImageInfo = gbuffer->getDescriptorImageInfo(index);

  outStructMemberTexture[0].sampler = (RedHandleSampler)descriptorImageInfo.sampler;
  outStructMemberTexture[0].texture = (RedHandleTexture)descriptorImageInfo.imageView;
  outStructMemberTexture[0].setTo1  = descriptorImageInfo.imageLayout;

  return 0;
}

RedHandleTexture redRteHelperGBufferGetColorTexture(RedRteHandleHelperGBuffer helperGBuffer, unsigned index, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters) {
  nvvkhl::GBuffer * gbuffer = (nvvkhl::GBuffer *)helperGBuffer;

  return (RedHandleTexture)gbuffer->getColorImageView(index);
}

RedHandleTexture redRteHelperGBufferGetDepthTexture(RedRteHandleHelperGBuffer helperGBuffer, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters) {
  nvvkhl::GBuffer * gbuffer = (nvvkhl::GBuffer *)helperGBuffer;

  return (RedHandleTexture)gbuffer->getDepthImageView();
}

unsigned redRteHelperGBufferGetColorFormat(RedRteHandleHelperGBuffer helperGBuffer, unsigned index, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters) {
  nvvkhl::GBuffer * gbuffer = (nvvkhl::GBuffer *)helperGBuffer;

  return gbuffer->getColorFormat(index);
}

unsigned redRteHelperGBufferGetDepthFormat(RedRteHandleHelperGBuffer helperGBuffer, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters) {
  nvvkhl::GBuffer * gbuffer = (nvvkhl::GBuffer *)helperGBuffer;

  return gbuffer->getDepthFormat();
}

float redRteHelperGBufferGetAspectRatio(RedRteHandleHelperGBuffer helperGBuffer, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters) {
  nvvkhl::GBuffer * gbuffer = (nvvkhl::GBuffer *)helperGBuffer;

  return gbuffer->getAspectRatio();
}

// Various

int64_t redRteGpuWaitIdle(RedRteHandleContext rteContext, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters) {
  nvvk::Context * nvvkctx = (nvvk::Context *)rteContext;

  return (int64_t)vkDeviceWaitIdle(nvvkctx->m_device);
}

int64_t redRteQueueWaitIdle(RedRteHandleContext rteContext, RedHandleQueue queue, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters) {
  return (int64_t)vkQueueWaitIdle((VkQueue)queue);
}

int64_t redRteArrayGetMemoryRequirements(RedRteHandleContext rteContext, RedArray * inoutArray, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters) {
  nvvk::Context * nvvkctx = (nvvk::Context *)rteContext;

  VkMemoryRequirements memoryRequirements = {};
  vkGetBufferMemoryRequirements(nvvkctx->m_device, (VkBuffer)inoutArray->handle, &memoryRequirements);
  inoutArray->memoryBytesCount     = memoryRequirements.size;
  inoutArray->memoryBytesAlignment = memoryRequirements.alignment;
  inoutArray->memoryTypesSupported = memoryRequirements.memoryTypeBits;

  return 0;
}

int64_t redRteImageGetMemoryRequirements(RedRteHandleContext rteContext, RedImage * inoutImage, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters) {
  nvvk::Context * nvvkctx = (nvvk::Context *)rteContext;

  VkMemoryRequirements memoryRequirements = {};
  vkGetImageMemoryRequirements(nvvkctx->m_device, (VkImage)inoutImage->handle, &memoryRequirements);
  inoutImage->memoryBytesCount     = memoryRequirements.size;
  inoutImage->memoryBytesAlignment = memoryRequirements.alignment;
  inoutImage->memoryTypesSupported = memoryRequirements.memoryTypeBits;

  return 0;
}

int64_t redRteCallUsageAliasOrderBarrier(RedHandleCalls calls, RedRteHandleContext rteContext, unsigned arrayUsagesCount, const RedRteUsageArray * arrayUsages, unsigned imageUsagesCount, const RedRteUsageImage * imageUsages, unsigned aliasesCount, const RedAlias * aliases, unsigned ordersCount, const RedOrder * orders, unsigned dependencyFlags) {
  VkBufferMemoryBarrier * barriersArray = 0;
  VkImageMemoryBarrier  * barriersImage = 0;

  bool success = true;

  if (arrayUsagesCount > 0) {
    barriersArray = new(std::nothrow) VkBufferMemoryBarrier[arrayUsagesCount]();
    if (barriersArray == 0) {
      success = false;
    }
  }
  if (imageUsagesCount > 0) {
    barriersImage = new(std::nothrow) VkImageMemoryBarrier[imageUsagesCount]();
    if (barriersImage == 0) {
      success = false;
    }
  }
  
  if (success == true) {
    VkPipelineStageFlags oldAccessStages = 0;
    VkPipelineStageFlags newAccessStages = 0;
  
    for (unsigned i = 0; i < arrayUsagesCount; i += 1) {
      const RedRteUsageArray usage = arrayUsages[i];
      oldAccessStages |= usage.oldAccessStages;
      newAccessStages |= usage.newAccessStages;
      barriersArray[i].sType               = VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER;
      barriersArray[i].pNext               = 0;
      barriersArray[i].srcAccessMask       = usage.oldAccess;
      barriersArray[i].dstAccessMask       = usage.newAccess;
      barriersArray[i].srcQueueFamilyIndex = usage.queueFamilyIndexSource;
      barriersArray[i].dstQueueFamilyIndex = usage.queueFamilyIndexTarget;
      barriersArray[i].buffer              = (VkBuffer)usage.array;
      barriersArray[i].offset              = usage.arrayBytesFirst;
      barriersArray[i].size                = usage.arrayBytesCount;
    }

    for (unsigned i = 0; i < imageUsagesCount; i += 1) {
      const RedRteUsageImage usage = imageUsages[i];
      oldAccessStages |= usage.oldAccessStages;
      newAccessStages |= usage.newAccessStages;
      barriersImage[i].sType                           = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
      barriersImage[i].pNext                           = 0;
      barriersImage[i].srcAccessMask                   = usage.oldAccess;
      barriersImage[i].dstAccessMask                   = usage.newAccess;
      barriersImage[i].oldLayout                       = (VkImageLayout)usage.oldState;
      barriersImage[i].newLayout                       = (VkImageLayout)usage.newState;
      barriersImage[i].srcQueueFamilyIndex             = usage.queueFamilyIndexSource;
      barriersImage[i].dstQueueFamilyIndex             = usage.queueFamilyIndexTarget;
      barriersImage[i].image                           = (VkImage)usage.image;
      barriersImage[i].subresourceRange.aspectMask     = usage.imageAllParts;
      barriersImage[i].subresourceRange.baseMipLevel   = usage.imageLevelsFirst;
      barriersImage[i].subresourceRange.levelCount     = usage.imageLevelsCount;
      barriersImage[i].subresourceRange.baseArrayLayer = usage.imageLayersFirst;
      barriersImage[i].subresourceRange.layerCount     = usage.imageLayersCount;
    }

    if (oldAccessStages == 0) { oldAccessStages = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;    } // 0 if oldState == RED_STATE_PRESENT or new images are in the initial unusable state
    if (newAccessStages == 0) { newAccessStages = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT; } // 0 if newState == RED_STATE_PRESENT
  
    vkCmdPipelineBarrier((VkCommandBuffer)calls, (unsigned)oldAccessStages, (unsigned)newAccessStages, dependencyFlags, 0, 0, arrayUsagesCount, barriersArray, imageUsagesCount, barriersImage);

    delete[] barriersArray;
    delete[] barriersImage;
  }

  return success == true ? 0 : -1;
}

// nvpro_core

#define PROJECT_NAME "REDGPU RTE"
#define NOMINMAX
#define VULKAN_HPP_DISPATCH_LOADER_DYNAMIC 1
//#include "nvpro_core/nvvk/appwindowprofiler_vk.cpp"
#include "nvpro_core/nvvk/buffersuballocator_vk.cpp"
#include "nvpro_core/nvvk/commands_vk.cpp"
#include "nvpro_core/nvvk/context_vk.cpp"
#include "nvpro_core/nvvk/debug_util_vk.cpp"
#include "nvpro_core/nvvk/descriptorsets_vk.cpp"
#include "nvpro_core/nvvk/dynamicrendering_vk.cpp"
#include "nvpro_core/nvvk/error_vk.cpp"
#include "nvpro_core/nvvk/extensions_vk.cpp"
#include "nvpro_core/nvvk/gizmos_vk.cpp"
#include "nvpro_core/nvvk/images_vk.cpp"
#include "nvpro_core/nvvk/memallocator_dedicated_vk.cpp"
#include "nvpro_core/nvvk/memallocator_vk.cpp"
#include "nvpro_core/nvvk/memorymanagement_vk.cpp"
#include "nvpro_core/nvvk/memorymanagement_vkgl.cpp"
#include "nvpro_core/nvvk/nsight_aftermath_vk.cpp"
#include "nvpro_core/nvvk/pipeline_vk.cpp"
#include "nvpro_core/nvvk/profiler_vk.cpp"
#include "nvpro_core/nvvk/raytraceKHR_vk.cpp"
#include "nvpro_core/nvvk/raytraceNV_vk.cpp"
#include "nvpro_core/nvvk/renderpasses_vk.cpp"
#include "nvpro_core/nvvk/resourceallocator_vk.cpp"
#include "nvpro_core/nvvk/samplers_vk.cpp"
#include "nvpro_core/nvvk/sbtwrapper_vk.cpp"
#include "nvpro_core/nvvk/shadermodulemanager_vk.cpp"
#include "nvpro_core/nvvk/stagingmemorymanager_vk.cpp"
#include "nvpro_core/nvvk/swapchain_vk.cpp"
//#include "nvpro_core/nvh/appwindowprofiler.cpp"
#include "nvpro_core/nvh/bitarray.cpp"
#include "nvpro_core/nvh/cameramanipulator.cpp"
#include "nvpro_core/nvh/filemapping.cpp"
#include "nvpro_core/nvh/gltfscene.cpp"
#include "nvpro_core/nvh/nvprint.cpp"
#include "nvpro_core/nvh/parametertools.cpp"
#include "nvpro_core/nvh/primitives.cpp"
#include "nvpro_core/nvh/profiler.cpp"
#include "nvpro_core/nvh/shaderfilemanager.cpp"
#include "nvpro_core/nvp/nvpfilesystem.cpp"
#include "nvpro_core/nvp/nvpsystem.cpp"
#if defined(_WIN32)
#include "nvpro_core/nvp/nvpsystem_win32.cpp"
#elif defined(__linux__)
#include "nvpro_core/nvp/nvpsystem_linux.cpp"
#endif
#include "nvpro_core/nvp/nvpwindow.cpp"
#include "nvpro_core/nvp/perproject_globals.cpp"
#include "nvpro_core/nvvkhl/appbase_vk.cpp"
#include "nvpro_core/nvvkhl/appbase_vkpp.cpp"
#include "nvpro_core/nvvkhl/application.cpp"
#include "nvpro_core/nvvkhl/gbuffer.cpp"
//#include "nvpro_core/nvvkhl/gltf_scene_rtx.cpp"
//#include "nvpro_core/nvvkhl/gltf_scene_vk.cpp"
//#include "nvpro_core/nvvkhl/hdr_env.cpp"
//#include "nvpro_core/nvvkhl/hdr_env_dome.cpp"
//#include "nvpro_core/nvvkhl/sky.cpp"
//#include "nvpro_core/nvvkhl/tonemap_postprocess.cpp"
#include "nvpro_core/third_party/imgui/imgui.cpp"
#include "nvpro_core/third_party/imgui/imgui_demo.cpp"
#include "nvpro_core/third_party/imgui/imgui_draw.cpp"
#include "nvpro_core/third_party/imgui/imgui_tables.cpp"
#include "nvpro_core/third_party/imgui/imgui_widgets.cpp"
#include "nvpro_core/third_party/imgui/backends/imgui_impl_glfw.cpp"
#include "nvpro_core/imgui/imgui_camera_widget.cpp"
#include "nvpro_core/imgui/imgui_helper.cpp"
#include "nvpro_core/imgui/imgui_orient.cpp"
#include "nvpro_core/imgui/backends/imgui_impl_vulkan.cpp"
#include "nvpro_core/imgui/backends/imgui_vk_extra.cpp"
#define STB_IMAGE_IMPLEMENTATION
#include "nvpro_core/third_party/stb/stb_image.h"
VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE;

// cimgui

#include "redgpu_rte_cimgui.cpp"
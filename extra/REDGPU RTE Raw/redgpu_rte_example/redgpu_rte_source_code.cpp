#include "C:/RedGpuSDK/redgpu.h"
#include "C:/RedGpuSDK/redgpu_context_from_vk.h"
#include <string>

#if 0
extern "C" void * vkGetInstanceProcAddr(void * instance, const char * name);
#endif

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
  if      (procName == "vkEnumeratePhysicalDevices")              { return (void *)redRteEnumeratePhysicalDevicesReturning0; }
  else if (procName == "vkGetPhysicalDeviceMemoryProperties2KHR") { return (void *)vkGetInstanceProcAddr((VkInstance)instance, "vkGetPhysicalDeviceMemoryProperties2"); }
  else if (procName == "vkGetPhysicalDeviceFeatures2KHR")         { return (void *)vkGetInstanceProcAddr((VkInstance)instance, "vkGetPhysicalDeviceFeatures2"); }
  else if (procName == "vkGetPhysicalDeviceProperties2KHR")       { return (void *)vkGetInstanceProcAddr((VkInstance)instance, "vkGetPhysicalDeviceProperties2"); }
  else {
    return (void *)vkGetInstanceProcAddr((VkInstance)instance, name);
  }
}

static void * redRteGetInstanceProcAddrSpecialCasedForRedRteEnumeratePhysicalDevicesReturning1(void * instance, const char * name) {
  std::string procName = name;
  if      (procName == "vkEnumeratePhysicalDevices")              { return (void *)redRteEnumeratePhysicalDevicesReturning1; }
  else if (procName == "vkGetPhysicalDeviceMemoryProperties2KHR") { return (void *)vkGetInstanceProcAddr((VkInstance)instance, "vkGetPhysicalDeviceMemoryProperties2"); }
  else if (procName == "vkGetPhysicalDeviceFeatures2KHR")         { return (void *)vkGetInstanceProcAddr((VkInstance)instance, "vkGetPhysicalDeviceFeatures2"); }
  else if (procName == "vkGetPhysicalDeviceProperties2KHR")       { return (void *)vkGetInstanceProcAddr((VkInstance)instance, "vkGetPhysicalDeviceProperties2"); }
  else {
    return (void *)vkGetInstanceProcAddr((VkInstance)instance, name);
  }
}

typedef struct RedRteCreateContextParameters {
  unsigned      apiMajor;                       // Suggested default value: 1
  unsigned      apiMinor;                       // Suggested default value: 2
  uint64_t      vkInstanceExtensionsCount;
  const char ** vkInstanceExtensions;
  uint64_t      vkInstanceLayersCount;
  const char ** vkInstanceLayers;
  uint64_t      vkDeviceExtensionsCount;        // Suggested default value: 3
  const char ** vkDeviceExtensions;             // Suggested default value: ["VK_KHR_deferred_host_operations", "VK_KHR_acceleration_structure", "VK_KHR_ray_query"]
  RedBool32     disableRobustBufferAccess;      // Suggested default value: 1
  RedBool32     verboseCompatibleDevices;       // Suggested default value: 1
  RedBool32     verboseUsed;                    // Suggested default value: 1
  RedBool32     verboseAvailable;               // Suggested default value: 1
  RedBool32     enableAftermath;                // Suggested default value: 1
  uint64_t      ignoreDebugMessagesCount;
  int *         ignoreDebugMessages;
  int           debugSeverityFilterMask;        // Suggested default value: VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT
  uint64_t      requestQueueFamilysCount;
  unsigned *    requestQueueFamilysFlags;       // VkQueueFlags
  unsigned *    requestQueueFamilysQueuesCount;
  float *       requestQueueFamilysPriority;
  void *        nvvkContext;
} RedRteCreateContextParameters;

void redRteCreateContext(RedTypeProcedureMalloc mallocFn, RedTypeProcedureFree freeFn, RedTypeProcedureMallocTagged optionalMallocTagged, RedTypeProcedureFreeTagged optionalFreeTagged, RedTypeProcedureDebugCallback debugCallback, RedSdkVersion sdkVersion, unsigned sdkExtensionsCount, const unsigned * sdkExtensions, const char * optionalProgramName, unsigned optionalProgramVersion, const char * optionalEngineName, unsigned optionalEngineVersion, const void * optionalSettings, RedContext * outContext, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData, RedRteCreateContextParameters * rteParameters) {
  nvvk::ContextCreateInfo nvvkContextCreateInfo = nvvk::ContextCreateInfo();
  nvvkContextCreateInfo.useDeviceGroups           = false;
  nvvkContextCreateInfo.appEngine                 = optionalEngineName  == 0 ? "" : optionalEngineName;
  nvvkContextCreateInfo.appTitle                  = optionalProgramName == 0 ? "" : optionalProgramName;
  nvvkContextCreateInfo.disableRobustBufferAccess = rteParameters->disableRobustBufferAccess == 1 ? true : false;
  nvvkContextCreateInfo.verboseCompatibleDevices  = rteParameters->verboseCompatibleDevices  == 1 ? true : false;
  nvvkContextCreateInfo.verboseUsed               = rteParameters->verboseUsed               == 1 ? true : false;
  nvvkContextCreateInfo.verboseAvailable          = rteParameters->verboseAvailable          == 1 ? true : false;
  nvvkContextCreateInfo.enableAftermath           = rteParameters->enableAftermath           == 1 ? true : false;
  nvvkContextCreateInfo.apiMajor                  = rteParameters->apiMajor;
  nvvkContextCreateInfo.apiMinor                  = rteParameters->apiMinor;
  nvvkContextCreateInfo.defaultQueueGCT           = 0x00000001 | 0x00000002 | 0x00000004; // VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_COMPUTE_BIT | VK_QUEUE_TRANSFER_BIT
  nvvkContextCreateInfo.defaultQueueT             = 0;
  nvvkContextCreateInfo.defaultQueueC             = 0;
  nvvkContextCreateInfo.defaultPriorityGCT        = 1.0f;
  nvvkContextCreateInfo.defaultPriorityT          = 1.0f;
  nvvkContextCreateInfo.defaultPriorityC          = 1.0f;
  for (uint64_t i = 0; i < rteParameters->vkInstanceExtensionsCount; i += 1) {
    nvvkContextCreateInfo.addInstanceExtension(rteParameters->vkInstanceExtensions[i]);
  }
  for (uint64_t i = 0; i < rteParameters->vkInstanceLayersCount; i += 1) {
    nvvkContextCreateInfo.addInstanceLayer(rteParameters->vkInstanceLayers[i]);
  }
  for (uint64_t i = 0; i < rteParameters->vkDeviceExtensionsCount; i += 1) {
    nvvkContextCreateInfo.addDeviceExtension(rteParameters->vkDeviceExtensions[i]);
  }
  for (uint64_t i = 0; i < rteParameters->requestQueueFamilysCount; i += 1) {
    nvvkContextCreateInfo.addRequestedQueue((VkQueueFlags)rteParameters->requestQueueFamilysFlags[i], rteParameters->requestQueueFamilysQueuesCount[i], rteParameters->requestQueueFamilysPriority[i]);
  }

  bool success = true;

  nvvk::Context * nvvkContext = new(std::nothrow) nvvk::Context();
  if (nvvkContext == 0) {
    success = false;
  }

  if (success == true) {
    success = nvvkContext->init(nvvkContextCreateInfo);
  }

  if (success == true) {
    for (uint64_t i = 0; i < rteParameters->ignoreDebugMessagesCount; i += 1) {
      nvvkContext->ignoreDebugMessage(rteParameters->ignoreDebugMessages[i]);
    }
    nvvkContext->setDebugSeverityFilterMask(rteParameters->debugSeverityFilterMask);
  }

  uint64_t instance       = 0;
  uint64_t physicalDevice = 0;
  uint64_t device         = 0;
  if (success == true) {
    instance       = (uint64_t)(void *)nvvkContext->m_instance;
    physicalDevice = (uint64_t)(void *)nvvkContext->m_physicalDevice;
    device         = (uint64_t)(void *)nvvkContext->m_device;
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
  settingsContextFromVkGetInstanceProcAddr.getInstanceProcAddr = success == true ? (void *)redRteGetInstanceProcAddrSpecialCasedForRedRteEnumeratePhysicalDevicesReturning1 : (void *)redRteGetInstanceProcAddrSpecialCasedForRedRteEnumeratePhysicalDevicesReturning0;
  RedContextOptionalSettingsContextFromVk settingsContextFromVk = {};
  settingsContextFromVk.settings             = RED_CONTEXT_OPTIONAL_SETTINGS_CONTEXT_FROM_VK;
  settingsContextFromVk.next                 = &settingsContextFromVkGetInstanceProcAddr;
  settingsContextFromVk.instance             = success == true ? instance        : 0;
  settingsContextFromVk.physicalDevicesCount = success == true ? 1               : 0;
  settingsContextFromVk.physicalDevices      = success == true ? &physicalDevice : 0;
  settingsContextFromVk.devices              = success == true ? &device         : 0;
  redCreateContext(mallocFn, freeFn, optionalMallocTagged, optionalFreeTagged, debugCallback, sdkVersion, sdkExtensionsCount, sdkExtensions, optionalProgramName, optionalProgramVersion, optionalEngineName, optionalEngineVersion, &settingsContextFromVk, outContext, outStatuses, optionalFile, optionalLine, optionalUserData);

  rteParameters->nvvkContext = nvvkContext;
}
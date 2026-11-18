#pragma once

#include "redgpu.h"

#ifdef __cplusplus
extern "C" {
#endif

// redCreateContext

typedef enum RedContextOptionalSettings1 {
  RED_CONTEXT_OPTIONAL_SETTINGS_CONTEXT_FROM_VK                        = 2,
  RED_CONTEXT_OPTIONAL_SETTINGS_CONTEXT_FROM_VK_GET_INSTANCE_PROC_ADDR = 3,
} RedContextOptionalSettings1;

typedef struct RedContextOptionalSettingsContextFromVk {
  RedContextOptionalSettings1 settings;
  const void *                next;
  uint64_t                    instance;
  unsigned                    physicalDevicesCount;
  uint64_t *                  physicalDevices;
  uint64_t *                  devices;              // Array of physicalDevicesCount
} RedContextOptionalSettingsContextFromVk;

typedef struct RedContextOptionalSettingsContextFromVkGetInstanceProcAddr {
  RedContextOptionalSettings1 settings;
  const void *                next;
  void *                      getInstanceProcAddr;
} RedContextOptionalSettingsContextFromVkGetInstanceProcAddr;

#ifdef __cplusplus
}
#endif

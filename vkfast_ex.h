#pragma once

#include "vkfast.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct gpu_context_ex2_parameters_t {
  uint64_t  external_VkInstance;       // NOTE(Constantine): Make sure the VK_KHR_get_physical_device_properties2 instance extension is enabled on this external VkInstance.
  uint64_t  external_VkPhysicalDevice;
  uint64_t  external_VkDevice;         // NOTE(Constantine): Make sure the VK_KHR_maintenance1 device extension is enabled on this external VkDevice.
  RedBool32 exposeOnlyOneGpu;
  RedBool32 exposeOnlyOneQueue;
} gpu_context_ex2_parameters_t;

GPU_API_PRE gpu_handle_context_t GPU_API_POST vfContextInitEx2(int enable_debug_mode, unsigned gpu_index, const gpu_context_optional_parameters_t * optional_parameters, const gpu_context_ex2_parameters_t * optional_ex2_parameters, const char * optional_file, int optional_line);
GPU_API_PRE uint64_t GPU_API_POST vfBatchBeginEx(gpu_handle_context_t context, uint64_t existing_batch_id, const gpu_batch_info_t * batch_info, unsigned queue_family_index, const char * optional_debug_name, const char * optional_file, int optional_line);
GPU_API_PRE uint64_t GPU_API_POST vfAsyncBatchExecuteRawEx(gpu_handle_context_t context, RedHandleQueue queue, uint64_t batch_raw_count, const RedHandleCalls * batch_raw, unsigned gpu_threads_count, gpu_thread_t * gpu_threads_inout, gpu_thread_t * gpu_threads_out, const unsigned * gpu_threads_array_of_65536_int_values, const char * optional_file, int optional_line);
GPU_API_PRE void GPU_API_POST vfBatchBindTextureRWEx(gpu_handle_context_t context, uint64_t batch_id, int slot, int textures_rw_count, const RedStructMemberTexture * textures_rw, const char * optional_file, int optional_line);

#ifdef __cplusplus
}
#endif

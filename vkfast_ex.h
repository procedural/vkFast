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

typedef struct gpu_context_ex3_parameters_t {
  unsigned * optionalSpecificMemoryTypesGpuVram;
  unsigned * optionalSpecificMemoryTypesCpuUpload;
  unsigned * optionalSpecificMemoryTypesCpuReadback;
} gpu_context_ex3_parameters_t;

GPU_API_PRE gpu_handle_context_t GPU_API_POST vfContextInitEx2(int enable_debug_mode, unsigned gpu_index, const gpu_context_optional_parameters_t * optional_parameters, const gpu_context_ex2_parameters_t * optional_ex2_parameters, const char * optional_file, int optional_line);
GPU_API_PRE gpu_handle_context_t GPU_API_POST vfContextInitEx3(int enable_debug_mode, unsigned gpu_index, const gpu_context_optional_parameters_t * optional_parameters, const gpu_context_ex2_parameters_t * optional_ex2_parameters, const gpu_context_ex3_parameters_t * optional_ex3_parameters, const char * optional_file, int optional_line);
GPU_API_PRE int GPU_API_POST vfWindowFullscreenEx(gpu_handle_context_t context, void * optional_external_window_handle, const char * window_title, int screen_width, int screen_height, unsigned draw_queue_index, RedPresentVsyncMode present_vsync_mode, int present_images_count, const char * optional_file, int optional_line);
GPU_API_PRE uint64_t GPU_API_POST vfBatchBeginEx(gpu_handle_context_t context, uint64_t existing_batch_id, const gpu_batch_info_t * batch_info, unsigned queue_family_index, const char * optional_debug_name, const char * optional_file, int optional_line);
GPU_API_PRE uint64_t GPU_API_POST vfAsyncBatchExecuteRawEx(gpu_handle_context_t context, RedHandleQueue queue, uint64_t batch_raw_count, const RedHandleCalls * batch_raw, unsigned gpu_threads_count, gpu_thread_t * gpu_threads, const unsigned * gpu_threads_array_of_65536_int_values, const char * optional_file, int optional_line);
GPU_API_PRE void GPU_API_POST vfBatchBindTextureRWEx(gpu_handle_context_t context, uint64_t batch_id, int slot, int textures_rw_count, const RedStructMemberTexture * textures_rw, const char * optional_file, int optional_line);

#ifdef __cplusplus
}
#endif

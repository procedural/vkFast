#pragma once

#if defined(_WIN32)
  #if defined(VKFAST_INCLUDE_TERMUX_PATHS)
    #include "/data/data/com.termux/files/home/RedGpuSDK/redgpu.h"     // Source: https://github.com/redgpu/redgpu
    #include "/data/data/com.termux/files/home/RedGpuSDK/redgpu_wsi.h" // Source: https://github.com/redgpu/redgpu
    #include "/data/data/com.termux/files/home/RedGpuSDK/redgpu_2.h"   // Source: https://github.com/redgpu/redgpu2
    #include "/data/data/com.termux/files/home/RedGpuSDK/redgpu_32.h"  // Source: https://github.com/redgpu/redgpu2
  #elif defined(VKFAST_INCLUDE_LINUX_PATHS)
    #include "/home/linuxbrew/RedGpuSDK/redgpu.h"     // Source: https://github.com/redgpu/redgpu
    #include "/home/linuxbrew/RedGpuSDK/redgpu_wsi.h" // Source: https://github.com/redgpu/redgpu
    #include "/home/linuxbrew/RedGpuSDK/redgpu_2.h"   // Source: https://github.com/redgpu/redgpu2
    #include "/home/linuxbrew/RedGpuSDK/redgpu_32.h"  // Source: https://github.com/redgpu/redgpu2
  #else
    #include "C:/RedGpuSDK/redgpu.h"     // Source: https://github.com/redgpu/redgpu
    #include "C:/RedGpuSDK/redgpu_wsi.h" // Source: https://github.com/redgpu/redgpu
    #include "C:/RedGpuSDK/redgpu_2.h"   // Source: https://github.com/redgpu/redgpu2
    #include "C:/RedGpuSDK/redgpu_32.h"  // Source: https://github.com/redgpu/redgpu2
  #endif
#elif defined(__linux__) && !defined(__ANDROID__)
  #include "/home/linuxbrew/RedGpuSDK/redgpu.h"     // Source: https://github.com/redgpu/redgpu
  #include "/home/linuxbrew/RedGpuSDK/redgpu_wsi.h" // Source: https://github.com/redgpu/redgpu
  #include "/home/linuxbrew/RedGpuSDK/redgpu_2.h"   // Source: https://github.com/redgpu/redgpu2
  #include "/home/linuxbrew/RedGpuSDK/redgpu_32.h"  // Source: https://github.com/redgpu/redgpu2
#else
  #error Unsupported OS for now
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef struct gpu_type_handle_context_t * gpu_handle_context_t;

typedef struct gpu_context_optional_parameters_t {
  void * optional_pointer_to_custom_vf_handle_context;
} gpu_context_optional_parameters_t;

typedef struct gpu_program_info_t {
  uint64_t     program_binary_bytes_count;
  const void * program_binary;
  const char * optional_debug_name;
} gpu_program_info_t;

typedef struct gpu_program_pipeline_info_t {
  unsigned                           variables_slot;
  unsigned                           variables_bytes_count;
  unsigned                           struct_members_count;
  const RedStructDeclarationMember * struct_members;
} gpu_program_pipeline_info_t;

typedef struct gpu_batch_info_t {
  int max_new_bindings_sets_count;
  int max_storage_binds_count;
  int max_texture_rw_binds_count;
  int max_texture_ro_binds_count;
  int max_sampler_binds_count;
} gpu_batch_info_t;

typedef RedHandleGpuSignal gpu_thread_t;

#ifndef GPU_API_PRE
#define GPU_API_PRE
#endif

#ifndef GPU_API_POST
#define GPU_API_POST
#endif

npf(GPU_API_PRE gpu_handle_context_t GPU_API_POST, vfContextInit, int,enable_debug_mode, const gpu_context_optional_parameters_t *,optional_parameters, const char *,optional_file, int,optional_line);
npf(GPU_API_PRE gpu_handle_context_t GPU_API_POST, vfContextInitEx, int,enable_debug_mode, unsigned,gpu_index, const gpu_context_optional_parameters_t *,optional_parameters, const char *,optional_file, int,optional_line);
npf(GPU_API_PRE void GPU_API_POST, vfContextDeinit, gpu_handle_context_t,context, const char *,optional_file, int,optional_line);
npf(GPU_API_PRE RedContext GPU_API_POST, vfContextGetRaw, gpu_handle_context_t,context, const char *,optional_file, int,optional_line);
npf(GPU_API_PRE void GPU_API_POST, vfIdDestroy, uint64_t,ids_count, const uint64_t *,ids, const char *,optional_file, int,optional_line);
npf(GPU_API_PRE void GPU_API_POST, vfGetMainMonitorAreaRectangle, int *,out4ints, const char *,optional_file, int,optional_line);
npf(GPU_API_PRE int  GPU_API_POST, vfWindowFullscreen, gpu_handle_context_t,context, void *,optional_external_window_handle, const char *,window_title, int,screen_width, int,screen_height, unsigned,draw_queue_index, RedPresentVsyncMode,present_vsync_mode, const char *,optional_file, int,optional_line);
npf(GPU_API_PRE int  GPU_API_POST, vfWindowLoop, gpu_handle_context_t,context);
npf(GPU_API_PRE int  GPU_API_POST, vfWindowIsMinimized, gpu_handle_context_t,context);
npf(GPU_API_PRE void GPU_API_POST, vfWindowGetSize, gpu_handle_context_t,context, int *,out_window_width, int *,out_window_height);
npf(GPU_API_PRE void GPU_API_POST, vfExit, int,exit_code);
npf(GPU_API_PRE uint64_t GPU_API_POST, vfProgramCreateFromBinaryCompute, gpu_handle_context_t,context, const gpu_program_info_t *,program_info, const char *,optional_file, int,optional_line);
npf(GPU_API_PRE uint64_t GPU_API_POST, vfProgramPipelineCreateCompute, gpu_handle_context_t,context, uint64_t,compute_program, const gpu_program_pipeline_info_t *,program_pipeline_info,  const char *,optional_debug_name, const char *,optional_file, int,optional_line);
npf(GPU_API_PRE uint64_t GPU_API_POST, vfBatchBegin, gpu_handle_context_t,context, uint64_t,existing_batch_id, const gpu_batch_info_t *,batch_info, const char *,optional_debug_name, const char *,optional_file, int,optional_line);
npf(GPU_API_PRE void GPU_API_POST, vfBatchStorageCopyRaw, gpu_handle_context_t,context, uint64_t,batch_id, RedHandleArray,from_storage_raw, RedHandleArray,to_storage_raw, const RedCopyArrayRange *,range, const char *,optional_file, int,optional_line);
npf(GPU_API_PRE void GPU_API_POST, vfBatchBindProgramPipelineCompute, gpu_handle_context_t,context, uint64_t,batch_id, uint64_t,program_pipeline_compute_id, const char *,optional_file, int,optional_line);
npf(GPU_API_PRE void GPU_API_POST, vfBatchBindNewBindingsSet, gpu_handle_context_t,context, uint64_t,batch_id, int,slots_count, const RedStructDeclarationMember *,slots, const char *,optional_file, int,optional_line);
npf(GPU_API_PRE void GPU_API_POST, vfBatchBindStorageRaw, gpu_handle_context_t,context, uint64_t,batch_id, int,slot, int,storage_raw_count, const RedStructMemberArray *,storage_raw, const char *,optional_file, int,optional_line); // HLSL: RWByteAddressBuffer
npf(GPU_API_PRE void GPU_API_POST, vfBatchBindNewBindingsEnd, gpu_handle_context_t,context, uint64_t,batch_id, const char *,optional_file, int,optional_line);
npf(GPU_API_PRE void GPU_API_POST, vfBatchBindVariablesCopy, gpu_handle_context_t,context, uint64_t,batch_id, unsigned,variables_bytes_offset, unsigned,data_bytes_count, const void *,data, const char *,optional_file, int,optional_line);
npf(GPU_API_PRE void GPU_API_POST, vfBatchCompute, gpu_handle_context_t,context, uint64_t,batch_id, unsigned,workgroups_count_x, unsigned,workgroups_count_y, unsigned,workgroups_count_z, const char *,optional_file, int,optional_line);
npf(GPU_API_PRE void GPU_API_POST, vfBatchBarrierMemory, gpu_handle_context_t,context, uint64_t,batch_id, const char *,optional_file, int,optional_line);
npf(GPU_API_PRE void GPU_API_POST, vfBatchBarrierCpuReadback, gpu_handle_context_t,context, uint64_t,batch_id, const char *,optional_file, int,optional_line);
npf(GPU_API_PRE void GPU_API_POST, vfBatchEnd, gpu_handle_context_t,context, uint64_t,batch_id, const char *,optional_file, int,optional_line);
npf(GPU_API_PRE void GPU_API_POST, vfGpuThreadCreate, gpu_handle_context_t,context, unsigned,gpu_threads_count, gpu_thread_t *,out_gpu_threads, const char **,optional_gpu_threads_debug_name, const char *,optional_file, int,optional_line);
npf(GPU_API_PRE void GPU_API_POST, vfGpuThreadDestroy, gpu_handle_context_t,context, gpu_thread_t,gpu_thread);
npf(GPU_API_PRE RedHandleCalls GPU_API_POST, vfBatchGetRawHandle, gpu_handle_context_t,context, uint64_t,batch_id, const char *,optional_file, int,optional_line);
npf(GPU_API_PRE uint64_t GPU_API_POST, vfAsyncBatchExecuteRaw, gpu_handle_context_t,context, uint64_t,batch_raw_count, const RedHandleCalls *,batch_raw, unsigned,gpu_threads_count, gpu_thread_t *,gpu_threads, const unsigned *,gpu_threads_array_of_65536_int_values, const char *,optional_file, int,optional_line);
npf(GPU_API_PRE void GPU_API_POST, vfAsyncWaitToFinish, gpu_handle_context_t,context, uint64_t,async_id, const char *,optional_file, int,optional_line);
npf(GPU_API_PRE int  GPU_API_POST, vfAsyncDrawPixelsRaw, gpu_handle_context_t,context, const RedStructMemberArray *,pixels_storage_raw, int *,out_optional_internal_present_image_index, unsigned,gpu_threads_count_plus_one_empty, gpu_thread_t *,gpu_threads, const unsigned *,gpu_threads_array_of_65536_int_values, const char *,optional_file, int,optional_line);
npf(GPU_API_PRE int  GPU_API_POST, vfAsyncDrawImageRaw, gpu_handle_context_t,context, RedHandleImage,image_raw, int *,out_optional_is_image_copy_finished_cpu_signal_index, unsigned,gpu_threads_count_plus_one_empty, gpu_thread_t *,gpu_threads, const unsigned *,gpu_threads_array_of_65536_int_values, const char *,optional_file, int,optional_line);
npf(GPU_API_PRE RedHandleCpuSignal GPU_API_POST, vfAsyncDrawGetCpuSignal, gpu_handle_context_t,context);
npf(GPU_API_PRE void GPU_API_POST, vfAllQueuesWaitIdle, gpu_handle_context_t,context, const char *,optional_file, int,optional_line);

#ifdef __cplusplus
}
#endif

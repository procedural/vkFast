// LICENSE
// 
// Constantine Tarasenkov (iamvfx@gmail.com), 2025.
// 
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// 
//     http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

// Sources:
// https://github.com/redgpu/redgpu
// https://github.com/redgpu/redgpu2
#include "C:/RedGpuSDK/redgpu_2.h"
#include "C:/RedGpuSDK/redgpu_32.h"

#ifdef __cplusplus
extern "C" {
#endif

// TYPES ///////////////////////////////////////////////////////////////////////

typedef struct gpu_type_handle_context_t * gpu_handle_context_t;

typedef struct gpu_internal_memory_allocation_sizes_t {
  uint64_t bytes_count_for_memory_gpu_vram_arrays;
  uint64_t bytes_count_for_memory_cpu_visible;
  uint64_t bytes_count_for_memory_cpu_readback;
} gpu_internal_memory_allocation_sizes_t;

typedef struct gpu_context_optional_parameters_t {
  gpu_internal_memory_allocation_sizes_t * internal_memory_allocation_sizes;
} gpu_context_optional_parameters_t;

typedef enum gpu_storage_type_t {
  GPU_STORAGE_TYPE_NONE         = 0,
  GPU_STORAGE_TYPE_GPU_ONLY     = 1,
  GPU_STORAGE_TYPE_CPU_UPLOAD   = 2,
  GPU_STORAGE_TYPE_CPU_READBACK = 3,
} gpu_storage_type_t;

typedef struct gpu_storage_info_t {
  gpu_storage_type_t storage_type;
  uint64_t           bytes_count;
} gpu_storage_info_t;

typedef struct gpu_storage_t {
  uint64_t           id;
  gpu_storage_info_t info;
  uint64_t           alignment;
  union
  {
    void              * mapped_void_ptr;
    volatile uint8_t  * ptr;
    volatile int8_t   * as_i8;
    volatile uint8_t  * as_u8;
    volatile int16_t  * as_i16;
    volatile uint16_t * as_u16;
    volatile int32_t  * as_i32;
    volatile uint32_t * as_u32;
    volatile float    * as_f32;
    volatile struct
    {
      union { float x; float u; };
      union { float y; float v; };
    } * as_vec2;
    volatile struct
    {
      union { float x; float r; };
      union { float y; float g; };
      union { float z; float b; };
    } * as_vec3;
    volatile struct
    {
      union { float x; float r; };
      union { float y; float g; };
      union { float z; float b; };
      union { float w; float a; };
    } * as_vec4;
  };
} gpu_storage_t;

typedef struct gpu_program_info_t {
  uint64_t     program_binary_bytes_count;
  const void * program_binary;
  const char * optional_debug_name;
} gpu_program_info_t;

typedef struct gpu_program_pipeline_compute_info_t {
  uint64_t                           compute_program;
  unsigned                           variables_slot;
  unsigned                           variables_bytes_count;
  unsigned                           struct_members_count;
  const RedStructDeclarationMember * struct_members;
  const char *                       optional_debug_name;
} gpu_program_pipeline_compute_info_t;

typedef struct gpu_batch_info_t {
  int max_new_bindings_sets_count;
  int max_storage_binds_count;
} gpu_batch_info_t;

#ifndef GPU_API_PRE
#define GPU_API_PRE
#endif

#ifndef GPU_API_POST
#define GPU_API_POST
#endif

// PROTOTYPES //////////////////////////////////////////////////////////////////

GPU_API_PRE gpu_handle_context_t GPU_API_POST vfContextInit(int enable_debug_mode, const gpu_context_optional_parameters_t * optional_parameters, const char * optional_file, int optional_line);
GPU_API_PRE void GPU_API_POST vfContextDeinit(gpu_handle_context_t context, const char * optional_file, int optional_line);
GPU_API_PRE RedContext GPU_API_POST vfContextGetRaw(gpu_handle_context_t context, const char * optional_file, int optional_line);
GPU_API_PRE void GPU_API_POST vfIdDestroy(gpu_handle_context_t context, uint64_t ids_count, const uint64_t * ids, const char * optional_file, int optional_line);
GPU_API_PRE void GPU_API_POST vfGetMainMonitorAreaRectangle(int * out4ints, const char * optional_file, int optional_line);
GPU_API_PRE void GPU_API_POST vfWindowFullscreen(gpu_handle_context_t context, void * optional_external_window_handle, const char * window_title, int screen_width, int screen_height, const char * optional_file, int optional_line);
GPU_API_PRE int  GPU_API_POST vfWindowLoop(gpu_handle_context_t context);
GPU_API_PRE void GPU_API_POST vfDrawPixels(gpu_handle_context_t context, const void * pixels, const char * optional_file, int optional_line);
GPU_API_PRE void GPU_API_POST vfExit(int exit_code);
GPU_API_PRE void GPU_API_POST vfStorageCreate(gpu_handle_context_t context, const gpu_storage_info_t * storage_info, gpu_storage_t * out_storage, const char * optional_file, int optional_line);
GPU_API_PRE void GPU_API_POST vfStorageGetRaw(gpu_handle_context_t context, uint64_t storage_id, RedStructMemberArray * out_storage_raw, const char * optional_file, int optional_line);
GPU_API_PRE uint64_t GPU_API_POST vfProgramCreateFromBinaryCompute(gpu_handle_context_t context, const gpu_program_info_t * program_info, const char * optional_file, int optional_line);
GPU_API_PRE uint64_t GPU_API_POST vfProgramPipelineCreateCompute(gpu_handle_context_t context, const gpu_program_pipeline_compute_info_t * program_pipeline_compute_info, const char * optional_file, int optional_line);
GPU_API_PRE uint64_t GPU_API_POST vfBatchBegin(gpu_handle_context_t context, uint64_t existing_batch_id, const gpu_batch_info_t * batch_info, const char * optional_debug_name, const char * optional_file, int optional_line);
GPU_API_PRE void GPU_API_POST vfBatchStorageCopyFromCpuToGpu(gpu_handle_context_t context, uint64_t batch_id, uint64_t from_cpu_storage_id, uint64_t to_gpu_storage_id, const char * optional_file, int optional_line);
GPU_API_PRE void GPU_API_POST vfBatchStorageCopyFromGpuToCpu(gpu_handle_context_t context, uint64_t batch_id, uint64_t from_gpu_storage_id, uint64_t to_cpu_storage_id, const char * optional_file, int optional_line);
GPU_API_PRE void GPU_API_POST vfBatchStorageCopyRaw(gpu_handle_context_t context, uint64_t batch_id, RedHandleArray from_storage_raw, RedHandleArray to_storage_raw, const RedCopyArrayRange * range, const char * optional_file, int optional_line);
GPU_API_PRE void GPU_API_POST vfBatchBindProgramPipelineCompute(gpu_handle_context_t context, uint64_t batch_id, uint64_t program_pipeline_compute_id, const char * optional_file, int optional_line);
GPU_API_PRE void GPU_API_POST vfBatchBindNewBindingsSet(gpu_handle_context_t context, uint64_t batch_id, int slots_count, const RedStructDeclarationMember * slots, const char * optional_file, int optional_line);
GPU_API_PRE void GPU_API_POST vfBatchBindStorage(gpu_handle_context_t context, uint64_t batch_id, int slot, int storage_ids_count, const uint64_t * storage_ids, const char * optional_file, int optional_line); // HLSL: RWByteAddressBuffer
GPU_API_PRE void GPU_API_POST vfBatchBindStorageRaw(gpu_handle_context_t context, uint64_t batch_id, int slot, int storage_raw_count, const RedStructMemberArray * storage_raw, const char * optional_file, int optional_line); // HLSL: RWByteAddressBuffer
GPU_API_PRE void GPU_API_POST vfBatchBindNewBindingsEnd(gpu_handle_context_t context, uint64_t batch_id, const char * optional_file, int optional_line);
GPU_API_PRE void GPU_API_POST vfBatchBindVariablesCopy(gpu_handle_context_t context, uint64_t batch_id, unsigned variables_bytes_count, const void * variables, const char * optional_file, int optional_line);
GPU_API_PRE void GPU_API_POST vfBatchCompute(gpu_handle_context_t context, uint64_t batch_id, unsigned workgroups_count_x, unsigned workgroups_count_y, unsigned workgroups_count_z, const char * optional_file, int optional_line);
GPU_API_PRE void GPU_API_POST vfBatchBarrierMemory(gpu_handle_context_t context, uint64_t batch_id, const char * optional_file, int optional_line);
GPU_API_PRE void GPU_API_POST vfBatchBarrierCpuReadback(gpu_handle_context_t context, uint64_t batch_id, const char * optional_file, int optional_line);
GPU_API_PRE void GPU_API_POST vfBatchEnd(gpu_handle_context_t context, uint64_t batch_id, const char * optional_file, int optional_line);
GPU_API_PRE void GPU_API_POST vfBatchGetRaw(gpu_handle_context_t context, uint64_t batch_id, RedCalls * out_batch_raw, const char * optional_file, int optional_line);
GPU_API_PRE uint64_t GPU_API_POST vfAsyncBatchExecute(gpu_handle_context_t context, uint64_t batch_ids_count, const uint64_t * batch_ids, const char * optional_file, int optional_line);
GPU_API_PRE void GPU_API_POST vfAsyncWaitToFinish(gpu_handle_context_t context, uint64_t async_id, const char * optional_file, int optional_line);

#ifdef __cplusplus
}
#endif

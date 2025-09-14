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
#include "C:/RedGpuSDK/redgpu_wsi.h"

#ifdef __cplusplus
extern "C" {
#endif

// TYPES ///////////////////////////////////////////////////////////////////////

typedef struct gpu_internal_memory_allocation_sizes_t {
  uint64_t bytesCountForMemoryGpuVramArrays;
  uint64_t bytesCountForMemoryGpuVramImages;
  uint64_t bytesCountForMemoryCpuVisible;
  uint64_t bytesCountForMemoryCpuReadback;
} gpu_internal_memory_allocation_sizes_t;

typedef struct gpu_context_optional_parameters_t {
  gpu_internal_memory_allocation_sizes_t * internal_memory_allocation_sizes;
} gpu_context_optional_parameters_t;

typedef struct gpu_storage_info_t {
  int          isCpuReadbackBuffer;
  int          isGpuOnly;
  uint64_t     bytesCount;
  uint64_t     optionalVertexCount; // Optional property to fill for gpu_cmd_t::count
  const char * optionalDebugName;
} gpu_storage_info_t;

typedef struct gpu_storage_t {
  uint64_t           id;
  gpu_storage_info_t info;
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

typedef struct gpu_storage_gpu_only_t {
  uint64_t           id;
  gpu_storage_info_t info;
} gpu_storage_gpu_only_t;

typedef struct gpu_texture_info_t {
  int TODO;
} gpu_texture_info_t;

typedef struct gpu_sampler_info_t {
  int TODO;
} gpu_sampler_info_t;

typedef struct gpu_cmd_t {
  uint64_t count;
  uint64_t instance_count;
  uint64_t first;
  uint64_t instance_first;
} gpu_cmd_t;

#ifndef VF_API_PRE
#define VF_API_PRE
#endif

#ifndef VF_API_POST
#define VF_API_POST
#endif

// PROTOTYPES //////////////////////////////////////////////////////////////////

VF_API_PRE void VF_API_POST vfContextInit(int enable_debug_mode, const gpu_context_optional_parameters_t * optional_parameters, const char * optional_file, int optional_line);
VF_API_PRE void VF_API_POST vfContextDeinit(uint64_t ids_count, const uint64_t * ids, const char * optional_file, int optional_line);
VF_API_PRE void VF_API_POST vfWindowFullscreen(void * optional_existing_window_handle, const char * window_title, int screen_width, int screen_height, int msaa_samples, const char * optional_file, int optional_line);
VF_API_PRE int VF_API_POST vfWindowLoop();
VF_API_PRE void VF_API_POST vfExit(int exit_code);
VF_API_PRE void VF_API_POST vfStorageCreateFromStruct(const gpu_storage_info_t * storage_info, gpu_storage_t * out_storage, const char * optional_file, int optional_line);
VF_API_PRE uint64_t VF_API_POST vfTextureCreateFromStruct(const gpu_texture_info_t * texture_info, const char * optional_file, int optional_line);
VF_API_PRE uint64_t VF_API_POST vfSamplerCreateFromStruct(const gpu_sampler_info_t * sampler_info, const char * optional_file, int optional_line);
VF_API_PRE uint64_t VF_API_POST vfTextureCreateFromBmp(int width, int height, int generate_mip_levels, int texture_count, const char ** texture_paths, const char * optional_file, int optional_line);
VF_API_PRE uint64_t VF_API_POST vfCubemapCreateFromBmp(int width, int height, int generate_mip_levels, int texture_count, const char ** pos_x_texture_paths, const char ** neg_x_texture_paths, const char ** pos_y_texture_paths, const char ** neg_y_texture_paths, const char ** pos_z_texture_paths, const char ** neg_z_texture_paths, const char * optional_file, int optional_line);
VF_API_PRE uint64_t VF_API_POST vfProgramCreateFromFileVertProgram(const char * shader_filepath, const char * optional_file, int optional_line);
VF_API_PRE uint64_t VF_API_POST vfProgramCreateFromFileFragProgram(const char * shader_filepath, const char * optional_file, int optional_line);
VF_API_PRE uint64_t VF_API_POST vfProgramCreateFromFileCompProgram(const char * shader_filepath, const char * optional_file, int optional_line);
VF_API_PRE uint64_t VF_API_POST vfProgramCreateFromBinaryVertProgram(const char * shader_binary, const char * optional_file, int optional_line);
VF_API_PRE uint64_t VF_API_POST vfProgramCreateFromBinaryFragProgram(const char * shader_binary, const char * optional_file, int optional_line);
VF_API_PRE uint64_t VF_API_POST vfProgramCreateFromBinaryCompProgram(const char * shader_binary, const char * optional_file, int optional_line);
VF_API_PRE uint64_t VF_API_POST vfProgramPipelineCreate(uint64_t vert_program, uint64_t frag_program, const char * optional_file, int optional_line);
VF_API_PRE uint64_t VF_API_POST vfProgramPipelineCreateCompute(uint64_t comp_program, const char * optional_file, int optional_line);
VF_API_PRE uint64_t VF_API_POST vfBatchBegin(const char * optional_file, int optional_line);
VF_API_PRE void VF_API_POST vfBatchStorageCopyFromCpuToGpu(uint64_t batch_id, uint64_t storage_id, const char * optional_file, int optional_line);
VF_API_PRE void VF_API_POST vfBatchStorageCopyFromGpuToCpu(uint64_t batch_id, uint64_t storage_id, const char * optional_file, int optional_line);
VF_API_PRE void VF_API_POST vfBatchTexturePixelsCopyFromCpuToGpu(uint64_t batch_id, uint64_t texture_id, int texture_layer, int mip_level, int x, int y, int width, int height, uint64_t copy_source_storage_id, const char * optional_file, int optional_line);
VF_API_PRE void VF_API_POST vfBatchTexturePixelsCopyFromGpuToCpu(uint64_t batch_id, uint64_t texture_id, int texture_layer, int mip_level, int x, int y, int width, int height, uint64_t copy_target_storage_id, const char * optional_file, int optional_line);
VF_API_PRE void VF_API_POST vfBatchBindStorage(uint64_t batch_id, int storage_ids_count, const uint64_t * storage_ids, const char * optional_file, int optional_line); // HLSL: RWByteAddressBuffer
VF_API_PRE void VF_API_POST vfBatchBindTexture(uint64_t batch_id, int texture_ids_count, const uint64_t * texture_ids, const char * optional_file, int optional_line); // HLSL: Texture2DArray
VF_API_PRE void VF_API_POST vfBatchBindCubemap(uint64_t batch_id, int texture_ids_count, const uint64_t * texture_ids, const char * optional_file, int optional_line); // HLSL: TextureCubeArray<float4>
VF_API_PRE void VF_API_POST vfBatchBindSampler(uint64_t batch_id, int sampler_ids_count, const uint64_t * sampler_ids, const char * optional_file, int optional_line); // HLSL: Sampler
VF_API_PRE void VF_API_POST vfBatchBindRWTexture(uint64_t batch_id, int texture_ids_count, const uint64_t * texture_ids, const char * optional_file, int optional_line); // HLSL: RWTexture2DArray
VF_API_PRE void VF_API_POST vfBatchBindProgramPipeline(uint64_t batch_id, uint64_t program_pipeline_id, const char * optional_file, int optional_line);
VF_API_PRE void VF_API_POST vfBatchBindProgramPipelineCompute(uint64_t batch_id, uint64_t program_pipeline_compute_id, const char * optional_file, int optional_line);
VF_API_PRE void VF_API_POST vfBatchClear(uint64_t batch_id, const char * optional_file, int optional_line);
VF_API_PRE void VF_API_POST vfBatchDraw(uint64_t batch_id, uint64_t gpu_cmd_count, const gpu_cmd_t * gpu_cmd, const char * optional_file, int optional_line);
VF_API_PRE void VF_API_POST vfBatchFire(uint64_t batch_id, uint64_t count, const char * optional_file, int optional_line);
VF_API_PRE void VF_API_POST vfBatchCompute(uint64_t batch_id, unsigned workgroups_count_x, unsigned workgroups_count_y, unsigned workgroups_count_z, const char * optional_file, int optional_line);
VF_API_PRE void VF_API_POST vfBatchMemoryBarrier(uint64_t batch_id, const char * optional_file, int optional_line);
VF_API_PRE void VF_API_POST vfBatchCpuReadbackBarrier(uint64_t batch_id, const char * optional_file, int optional_line);
VF_API_PRE void VF_API_POST vfBatchEnd(uint64_t batch_id, const char * optional_file, int optional_line);
VF_API_PRE void VF_API_POST vfBatchExecute(uint64_t batch_ids_count, const uint64_t * batch_ids, const char * optional_file, int optional_line);
VF_API_PRE uint64_t VF_API_POST vfAsyncBatchExecute(uint64_t batch_ids_count, const uint64_t * batch_ids, const char * optional_file, int optional_line);
VF_API_PRE void VF_API_POST vfAsyncWaitToFinish(uint64_t async_id, const char * optionalFile, int optionalLine);

#ifdef __cplusplus
}
#endif

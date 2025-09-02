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

typedef struct gpu_storage_info_t {
  uint64_t bytesCount;
  uint64_t optionalVertexCount; // Optional property to fill for gpu_cmd_t::count
} gpu_storage_info_t;

typedef struct gpu_storage_t {
  uint64_t           id;
  gpu_storage_info_t info;
  union
  {
    void     * mapped_void_ptr;
    uint8_t  * ptr;
    int8_t   * as_i8;
    uint8_t  * as_u8;
    int16_t  * as_i16;
    uint16_t * as_u16;
    int32_t  * as_i32;
    uint32_t * as_u32;
    float    * as_f32;
    struct
    {
      union { float x; float u; };
      union { float y; float v; };
    } * as_vec2;
    struct
    {
      union { float x; float r; };
      union { float y; float g; };
      union { float z; float b; };
    } * as_vec3;
    struct
    {
      union { float x; float r; };
      union { float y; float g; };
      union { float z; float b; };
      union { float w; float a; };
    } * as_vec4;
  };
} gpu_storage_t;

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

// PROTOTYPES //////////////////////////////////////////////////////////////////

void vfWindowFullscreen(void * optional_existing_window_handle, int enable_debug_mode, int screen_width, int screen_height, const char * window_title, int msaa_samples, const char * optional_file, int optional_line);
int vfWindowLoop();
void vfExit(int exit_code);
gpu_storage_t vfStorageCreateFromStruct(gpu_storage_info_t storage, const char * optional_file, int optional_line);
uint64_t vfTextureCreateFromStruct(gpu_texture_info_t texture, const char * optional_file, int optional_line);
uint64_t vfSamplerCreateFromStruct(gpu_sampler_info_t sampler, const char * optional_file, int optional_line);
uint64_t vfTextureCreateFromBmp(int width, int height, int generate_mip_levels, int texture_count, const char ** texture_paths, const char * optional_file, int optional_line);
uint64_t vfCubemapCreateFromBmp(int width, int height, int generate_mip_levels, int texture_count, const char ** pos_x_texture_paths, const char ** neg_x_texture_paths, const char ** pos_y_texture_paths, const char ** neg_y_texture_paths, const char ** pos_z_texture_paths, const char ** neg_z_texture_paths, const char * optional_file, int optional_line);
uint64_t vfProgramCreateFromFileVertProgram(const char * shader_filepath, const char * optional_file, int optional_line);
uint64_t vfProgramCreateFromFileFragProgram(const char * shader_filepath, const char * optional_file, int optional_line);
uint64_t vfProgramCreateFromStringVertProgram(const char * shader_string, const char * optional_file, int optional_line);
uint64_t vfProgramCreateFromStringFragProgram(const char * shader_string, const char * optional_file, int optional_line);
uint64_t vfProgramPipelineCreate(uint64_t vert_program, uint64_t frag_program, const char * optional_file, int optional_line);
uint64_t vfBatchBegin(const char * optional_file, int optional_line);
void vfBatchStorageCopyFromCpuToGpu(uint64_t batch_id, uint64_t storage_id, const char * optional_file, int optional_line);
void vfBatchStorageCopyFromGpuToCpu(uint64_t batch_id, uint64_t storage_id, const char * optional_file, int optional_line);
void vfBatchTexturePixelsCopyFromCpuToGpu(uint64_t batch_id, uint64_t texture_id, int texture_layer, int mip_level, int x, int y, int width, int height, uint64_t copy_source_storage_id, const char * optional_file, int optional_line);
void vfBatchTexturePixelsCopyFromGpuToCpu(uint64_t batch_id, uint64_t texture_id, int texture_layer, int mip_level, int x, int y, int width, int height, uint64_t copy_target_storage_id, const char * optional_file, int optional_line);
void vfBatchBindStorage(uint64_t batch_id, int storage_ids_count, const uint64_t * storage_ids, const char * optional_file, int optional_line);
void vfBatchBindTexture(uint64_t batch_id, int texture_ids_count, const uint64_t * texture_ids, const char * optional_file, int optional_line);
void vfBatchBindCubemap(uint64_t batch_id, int texture_ids_count, const uint64_t * texture_ids, const char * optional_file, int optional_line);
void vfBatchBindSampler(uint64_t batch_id, int sampler_ids_count, const uint64_t * sampler_ids, const char * optional_file, int optional_line);
void vfBatchBindProgramPipeline(uint64_t batch_id, uint64_t program_pipeline_id, const char * optional_file, int optional_line);
void vfBatchClear(uint64_t batch_id, const char * optional_file, int optional_line);
void vfBatchDraw(uint64_t batch_id, uint64_t gpu_cmd_count, const gpu_cmd_t * gpu_cmd, const char * optional_file, int optional_line);
void vfBatchFire(uint64_t batch_id, uint64_t count, const char * optional_file, int optional_line);
void vfBatchEnd(uint64_t batch_id, const char * optional_file, int optional_line);
void vfBatchExecute(uint64_t batch_ids_count, const uint64_t * batch_ids, const char * optional_file, int optional_line);

#ifdef __cplusplus
}
#endif

#pragma once

#include "vkfast_extra_banzai.h"

typedef struct gpu_extra_banzai_pointer_t {
  uint64_t id;
  uint64_t bytes_first;
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
    volatile struct
    {
      union { uint8_t x; uint8_t r; };
      union { uint8_t y; uint8_t g; };
      union { uint8_t z; uint8_t b; };
      union { uint8_t w; uint8_t a; };
    } * as_vec4_u8;
  };
} gpu_extra_banzai_pointer_t;

#ifdef __cplusplus
extern "C" {
#endif

GPU_API_PRE void GPU_API_POST vfeBanzaiGetPointer(const gpu_storage_t * storage, uint64_t bytes_first, gpu_extra_banzai_pointer_t * out_banzai_pointer, const char * optional_file, int optional_line);
GPU_API_PRE void GPU_API_POST vfeBanzaiPointerGetRaw(const gpu_extra_banzai_pointer_t * banzai_pointer, RedStructMemberArray * out_banzai_pointer_raw, const char * optional_file, int optional_line);
GPU_API_PRE void GPU_API_POST vfeBanzaiBatchPointerCopyFromCpuToGpu(gpu_handle_context_t context, uint64_t batch_id, const gpu_extra_banzai_pointer_t * from_cpu_pointer, const gpu_extra_banzai_pointer_t * to_gpu_pointer, uint64_t bytes_count, const char * optional_file, int optional_line);
GPU_API_PRE void GPU_API_POST vfeBanzaiBatchPointerCopyFromGpuToCpu(gpu_handle_context_t context, uint64_t batch_id, const gpu_extra_banzai_pointer_t * from_gpu_pointer, const gpu_extra_banzai_pointer_t * to_cpu_pointer, uint64_t bytes_count, const char * optional_file, int optional_line);

#ifdef __cplusplus
}
#endif

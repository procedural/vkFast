#pragma once

#include "../../vkfast.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct gpu_extra_cpu_gpu_array {
  void *               cpu_ptr;
  RedStructMemberArray cpu;
  RedStructMemberArray gpu;
} gpu_extra_cpu_gpu_array;

GPU_API_PRE void GPU_API_POST vfeCpuGpuArrayBatchCopyFromCpuToGpu(gpu_handle_context_t context, uint64_t batch_id, gpu_extra_cpu_gpu_array * cpu_gpu_array, uint64_t bytes_first, uint64_t bytes_count, const char * optional_file, int optional_line);
GPU_API_PRE void GPU_API_POST vfeCpuGpuArrayBatchCopyFromGpuToCpu(gpu_handle_context_t context, uint64_t batch_id, gpu_extra_cpu_gpu_array * cpu_gpu_array, uint64_t bytes_first, uint64_t bytes_count, const char * optional_file, int optional_line);

#ifdef __cplusplus
}
#endif

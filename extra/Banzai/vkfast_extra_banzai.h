#pragma once

#include "../../vkfast.h"

#ifdef __cplusplus
extern "C" {
#endif

GPU_API_PRE void GPU_API_POST vfeBanzaiStoragesCreate(gpu_handle_context_t context, gpu_storage_t * out_storage_id_gpu_only, gpu_storage_t * out_storage_id_cpu_upload, gpu_storage_t * out_storage_id_cpu_readback, const char * optional_file, int optional_line);
GPU_API_PRE void GPU_API_POST vfeBanzaiBatchStorageCopyFromCpuToGpu(gpu_handle_context_t context, uint64_t batch_id, uint64_t from_cpu_storage_id, uint64_t to_gpu_storage_id, uint64_t from_cpu_storage_bytes_first, uint64_t to_gpu_storage_bytes_first, uint64_t bytes_count, const char * optional_file, int optional_line);
GPU_API_PRE void GPU_API_POST vfeBanzaiBatchStorageCopyFromGpuToCpu(gpu_handle_context_t context, uint64_t batch_id, uint64_t from_gpu_storage_id, uint64_t to_cpu_storage_id, uint64_t from_gpu_storage_bytes_first, uint64_t to_cpu_storage_bytes_first, uint64_t bytes_count, const char * optional_file, int optional_line);

#ifdef __cplusplus
}
#endif

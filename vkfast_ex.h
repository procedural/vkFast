#pragma once

#include "vkfast.h"

#ifdef __cplusplus
extern "C" {
#endif

GPU_API_PRE uint64_t GPU_API_POST vfBatchBeginEx(gpu_handle_context_t context, uint64_t existing_batch_id, const gpu_batch_info_t * batch_info, unsigned queue_family_index, const char * optional_debug_name, const char * optional_file, int optional_line);
GPU_API_PRE uint64_t GPU_API_POST vfAsyncBatchExecuteEx(gpu_handle_context_t context, RedHandleQueue queue, uint64_t batch_ids_count, const uint64_t * batch_ids, const char * optional_file, int optional_line);

#ifdef __cplusplus
}
#endif

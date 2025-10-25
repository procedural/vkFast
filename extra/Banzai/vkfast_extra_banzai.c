#include "vkfast_extra_banzai.h"
#include "../../vkfast_ids.h"

#ifndef __cplusplus
#define REDGPU_DISABLE_NAMED_PARAMETERS
#endif
#include "C:/RedGpuSDK/misc/np/np.h"
#include "C:/RedGpuSDK/misc/np/np_redgpu.h"
#include "C:/RedGpuSDK/misc/np/np_redgpu_2.h"
#include "C:/RedGpuSDK/misc/np/np_redgpu_wsi.h"

GPU_API_PRE void GPU_API_POST vfeBanzaiStoragesCreate(gpu_handle_context_t context, gpu_storage_t * out_storage_id_gpu_only, gpu_storage_t * out_storage_id_cpu_upload, gpu_storage_t * out_storage_id_cpu_readback, const char * optionalFile, int optionalLine) {
  vf_handle_context_t * vkfast = (vf_handle_context_t *)(void *)context;

  if (out_storage_id_gpu_only != NULL && vkfast->memoryAllocationSizeGpuVram > 0) {
    gpu_storage_info_t storage_info = {0};
    storage_info.storage_type = GPU_STORAGE_TYPE_GPU_ONLY;
    storage_info.bytes_count  = vkfast->memoryAllocationSizeGpuVram;
    vfStorageCreate(context, &storage_info, out_storage_id_gpu_only, optionalFile, optionalLine);
  }

  if (out_storage_id_cpu_upload != NULL && vkfast->memoryAllocationSizeCpuUpload > 0) {
    gpu_storage_info_t storage_info = {0};
    storage_info.storage_type = GPU_STORAGE_TYPE_CPU_UPLOAD;
    storage_info.bytes_count  = vkfast->memoryAllocationSizeCpuUpload;
    vfStorageCreate(context, &storage_info, out_storage_id_cpu_upload, optionalFile, optionalLine);
  }

  if (out_storage_id_cpu_readback != NULL && vkfast->memoryAllocationSizeCpuReadback > 0) {
    gpu_storage_info_t storage_info = {0};
    storage_info.storage_type = GPU_STORAGE_TYPE_CPU_READBACK;
    storage_info.bytes_count  = vkfast->memoryAllocationSizeCpuReadback;
    vfStorageCreate(context, &storage_info, out_storage_id_cpu_readback, optionalFile, optionalLine);
  }
}

GPU_API_PRE void GPU_API_POST vfeBanzaiBatchStorageCopyFromCpuToGpu(gpu_handle_context_t context, uint64_t batch_id, uint64_t from_cpu_storage_id, uint64_t to_gpu_storage_id, uint64_t from_cpu_storage_bytes_first, uint64_t to_gpu_storage_bytes_first, uint64_t bytes_count, const char * optionalFile, int optionalLine) {
  vf_handle_t * batch = (vf_handle_t *)(void *)batch_id;
  vf_handle_context_t * vkfast = batch->vkfast;
  RedHandleGpu gpu = vkfast->gpu;
  REDGPU_2_EXPECTWG(batch->handle_id == VF_HANDLE_ID_BATCH);
  
  vf_handle_t * from_cpu_storage = (vf_handle_t *)(void *)from_cpu_storage_id;
  REDGPU_2_EXPECTWG(from_cpu_storage->handle_id == VF_HANDLE_ID_STORAGE);

  vf_handle_t * to_gpu_storage = (vf_handle_t *)(void *)to_gpu_storage_id;
  REDGPU_2_EXPECTWG(to_gpu_storage->handle_id == VF_HANDLE_ID_STORAGE);

  RedCopyArrayRange range = {0};
  range.arrayRBytesFirst  = from_cpu_storage_bytes_first;
  range.arrayWBytesFirst  = to_gpu_storage_bytes_first;
  range.bytesCount        = bytes_count;
  npfp(redCallCopyArrayToArray, batch->batch.addresses.redCallCopyArrayToArray,
    "calls", batch->batch.calls.handle,
    "arrayR", from_cpu_storage->storage.arrayRangeInfo.array,
    "arrayW", to_gpu_storage->storage.arrayRangeInfo.array,
    "rangesCount", 1,
    "ranges", &range
  );
}

GPU_API_PRE void GPU_API_POST vfeBanzaiBatchStorageCopyFromGpuToCpu(gpu_handle_context_t context, uint64_t batch_id, uint64_t from_gpu_storage_id, uint64_t to_cpu_storage_id, uint64_t from_gpu_storage_bytes_first, uint64_t to_cpu_storage_bytes_first, uint64_t bytes_count, const char * optionalFile, int optionalLine) {
  vf_handle_t * batch = (vf_handle_t *)(void *)batch_id;
  vf_handle_context_t * vkfast = batch->vkfast;
  RedHandleGpu gpu = vkfast->gpu;
  REDGPU_2_EXPECTWG(batch->handle_id == VF_HANDLE_ID_BATCH);
  
  vf_handle_t * from_gpu_storage = (vf_handle_t *)(void *)from_gpu_storage_id;
  REDGPU_2_EXPECTWG(from_gpu_storage->handle_id == VF_HANDLE_ID_STORAGE);

  vf_handle_t * to_cpu_storage = (vf_handle_t *)(void *)to_cpu_storage_id;
  REDGPU_2_EXPECTWG(to_cpu_storage->handle_id == VF_HANDLE_ID_STORAGE);

  RedCopyArrayRange range = {0};
  range.arrayRBytesFirst  = from_gpu_storage_bytes_first;
  range.arrayWBytesFirst  = to_cpu_storage_bytes_first;
  range.bytesCount        = bytes_count;
  npfp(redCallCopyArrayToArray, batch->batch.addresses.redCallCopyArrayToArray,
    "calls", batch->batch.calls.handle,
    "arrayR", from_gpu_storage->storage.arrayRangeInfo.array,
    "arrayW", to_cpu_storage->storage.arrayRangeInfo.array,
    "rangesCount", 1,
    "ranges", &range
  );
}

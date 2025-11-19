#include "vkfast_extra_banzai_pointer.h"
#include "../../vkfast_ids.h"

GPU_API_PRE void GPU_API_POST vfeBanzaiGetPointer(const gpu_storage_t * banzai_storage, uint64_t bytes_first, gpu_extra_banzai_pointer_t * out_banzai_pointer, const char * optionalFile, int optionalLine) {
  REDGPU_2_EXPECT(0 == REDGPU_2_BYTES_TO_NEXT_ALIGNMENT_BOUNDARY(bytes_first, 4) || !"Currently, only 32-bit Banzai pointer offsets are supported.");
  
  // Filling
  gpu_extra_banzai_pointer_t pointer = {0};
  pointer.id          = banzai_storage->id;
  pointer.bytes_first = bytes_first;
  if (banzai_storage->mapped_void_ptr == NULL) {
    pointer.mapped_void_ptr = NULL;
  } else {
    pointer.mapped_void_ptr = (void *)(&banzai_storage->as_u8[bytes_first]);
  }
  out_banzai_pointer[0] = pointer;
}

GPU_API_PRE void GPU_API_POST vfeBanzaiPointerGetRaw(const gpu_extra_banzai_pointer_t * banzai_pointer, RedStructMemberArray * out_banzai_pointer_raw, const char * optionalFile, int optionalLine) {
  vf_handle_t * storage = (vf_handle_t *)(void *)banzai_pointer->id;
  vf_handle_context_t * vkfast = storage->vkfast;
  RedHandleGpu gpu = vkfast->gpu;
  REDGPU_2_EXPECTWG(storage->handle_id == VF_HANDLE_ID_STORAGE);
  // Filling
  RedStructMemberArray raw = {0};
  raw.array = storage->storage.arrayRangeInfo.array;
  raw.arrayRangeBytesFirst = banzai_pointer->bytes_first;
  raw.arrayRangeBytesCount = storage->storage.arrayRangeInfo.arrayRangeBytesCount - banzai_pointer->bytes_first;
  out_banzai_pointer_raw[0] = raw;
}

GPU_API_PRE void GPU_API_POST vfeBanzaiBatchPointerCopyFromCpuToGpu(gpu_handle_context_t context, uint64_t batch_id, const gpu_extra_banzai_pointer_t * from_cpu_pointer, const gpu_extra_banzai_pointer_t * to_gpu_pointer, uint64_t bytes_count, const char * optionalFile, int optionalLine) {
  vfeBanzaiBatchStorageCopyFromCpuToGpu(context, batch_id, from_cpu_pointer->id, to_gpu_pointer->id, from_cpu_pointer->bytes_first, to_gpu_pointer->bytes_first, bytes_count, optionalFile, optionalLine);
}

GPU_API_PRE void GPU_API_POST vfeBanzaiBatchPointerCopyFromGpuToCpu(gpu_handle_context_t context, uint64_t batch_id, const gpu_extra_banzai_pointer_t * from_gpu_pointer, const gpu_extra_banzai_pointer_t * to_cpu_pointer, uint64_t bytes_count, const char * optionalFile, int optionalLine) {
  vfeBanzaiBatchStorageCopyFromGpuToCpu(context, batch_id, from_gpu_pointer->id, to_cpu_pointer->id, from_gpu_pointer->bytes_first, to_cpu_pointer->bytes_first, bytes_count, optionalFile, optionalLine);
}

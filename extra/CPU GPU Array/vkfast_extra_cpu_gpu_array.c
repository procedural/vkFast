#include "../../vkfast.h"
#include "../../vkfast_ids.h"

#undef GPU_API_PRE
#undef GPU_API_POST

#if defined(_WIN32)
  #define GPU_API_PRE  __declspec(dllexport)
  #define GPU_API_POST
#elif defined(__linux__)
  #define GPU_API_PRE  __attribute__((visibility("default")))
  #define GPU_API_POST
#else
  #error Unsupported OS for now
#endif

#include "vkfast_extra_cpu_gpu_array.h"

GPU_API_PRE void GPU_API_POST vfeCpuGpuArrayBatchCopyFromCpuToGpu(gpu_handle_context_t context, uint64_t batch_id, gpu_extra_cpu_gpu_array * cpu_gpu_array, uint64_t bytes_first, uint64_t bytes_count, const char * optionalFile, int optionalLine) {
  vf_handle_t * batch = (vf_handle_t *)(void *)batch_id;
  vf_handle_context_t * vkfast = batch->vkfast;
  RedHandleGpu gpu = vkfast->gpu;
  REDGPU_2_EXPECTWG(batch->handle_id == VF_HANDLE_ID_BATCH);

  RedCopyArrayRange range = {0};
  range.arrayRBytesFirst  = cpu_gpu_array->cpu.arrayRangeBytesFirst + bytes_first;
  range.arrayWBytesFirst  = cpu_gpu_array->gpu.arrayRangeBytesFirst + bytes_first;
  range.bytesCount        = bytes_count;
  npfp(redCallCopyArrayToArray, batch->batch.addresses.redCallCopyArrayToArray,
    "calls", batch->batch.calls.handle,
    "arrayR", cpu_gpu_array->cpu.array,
    "arrayW", cpu_gpu_array->gpu.array,
    "rangesCount", 1,
    "ranges", &range
  );
}

GPU_API_PRE void GPU_API_POST vfeCpuGpuArrayBatchCopyFromGpuToCpu(gpu_handle_context_t context, uint64_t batch_id, gpu_extra_cpu_gpu_array * cpu_gpu_array, uint64_t bytes_first, uint64_t bytes_count, const char * optionalFile, int optionalLine) {
  vf_handle_t * batch = (vf_handle_t *)(void *)batch_id;
  vf_handle_context_t * vkfast = batch->vkfast;
  RedHandleGpu gpu = vkfast->gpu;
  REDGPU_2_EXPECTWG(batch->handle_id == VF_HANDLE_ID_BATCH);

  RedCopyArrayRange range = {0};
  range.arrayRBytesFirst  = cpu_gpu_array->gpu.arrayRangeBytesFirst + bytes_first;
  range.arrayWBytesFirst  = cpu_gpu_array->cpu.arrayRangeBytesFirst + bytes_first;
  range.bytesCount        = bytes_count;
  npfp(redCallCopyArrayToArray, batch->batch.addresses.redCallCopyArrayToArray,
    "calls", batch->batch.calls.handle,
    "arrayR", cpu_gpu_array->gpu.array,
    "arrayW", cpu_gpu_array->cpu.array,
    "rangesCount", 1,
    "ranges", &range
  );
}

GPU_API_PRE void GPU_API_POST vfeCpuGpuArrayOffset(gpu_extra_cpu_gpu_array * cpu_gpu_array, uint64_t bytes_offset) {
  REDGPU_2_EXPECTFL(bytes_offset <= cpu_gpu_array->cpu.arrayRangeBytesCount);
  REDGPU_2_EXPECTFL(bytes_offset <= cpu_gpu_array->gpu.arrayRangeBytesCount);

  unsigned char * cpu_ptr = (unsigned char *)cpu_gpu_array->cpu_ptr;
  cpu_ptr += bytes_offset;

  cpu_gpu_array->cpu_ptr = cpu_ptr;
  cpu_gpu_array->cpu.arrayRangeBytesFirst += bytes_offset;
  cpu_gpu_array->cpu.arrayRangeBytesCount -= bytes_offset;
  cpu_gpu_array->gpu.arrayRangeBytesFirst += bytes_offset;
  cpu_gpu_array->gpu.arrayRangeBytesCount -= bytes_offset;
}

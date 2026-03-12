#include "../../vkfast.h"
#include "../../vkfast_ids.h"

#ifdef _WIN32
#undef GPU_API_PRE
#undef GPU_API_POST
#define GPU_API_PRE __declspec(dllexport)
#define GPU_API_POST
#endif

#include "vkfast_extra_cpu_gpu_array.h"

#ifndef __cplusplus
#define REDGPU_DISABLE_NAMED_PARAMETERS
#endif
#include "C:/RedGpuSDK/misc/np/np.h"
#include "C:/RedGpuSDK/misc/np/np_redgpu.h"
#include "C:/RedGpuSDK/misc/np/np_redgpu_2.h"
#include "C:/RedGpuSDK/misc/np/np_redgpu_wsi.h"

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

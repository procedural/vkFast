#pragma once

#include "../../../vkfast.h"
#include "../../../extra/REII/vkfast_extra_reii.h"
#include "../../../extra/vkFast Extensions/ReBAR/vkfast_ext_rebar.h"
#include "../../../extra/vkFast Extensions/I dont care/idc_imgui.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../../../extra/stb_image 2017/stb_image.h"

static inline void vfBatchComputeLaunchThreads(gpu_handle_context_t context, uint64_t batchId, unsigned threadsCountX, unsigned threadsCountY, unsigned threadsCountZ, unsigned threadGroupsCountX, unsigned threadGroupsCountY, unsigned threadGroupsCountZ, const char * optionalFile, int optionalLine) {
  // https://www.reddit.com/r/vulkan/comments/1wx1a7m/
  unsigned workgroupsCountX = (threadsCountX / threadGroupsCountX) + (threadsCountX % threadGroupsCountX == 0 ? 0 : 1);
  unsigned workgroupsCountY = (threadsCountY / threadGroupsCountY) + (threadsCountY % threadGroupsCountY == 0 ? 0 : 1);
  unsigned workgroupsCountZ = (threadsCountZ / threadGroupsCountZ) + (threadsCountZ % threadGroupsCountZ == 0 ? 0 : 1);
  vfBatchCompute(context, batchId, workgroupsCountX == 0 ? 1 : workgroupsCountX, workgroupsCountY == 0 ? 1 : workgroupsCountY, workgroupsCountZ == 0 ? 1 : workgroupsCountZ, optionalFile, optionalLine);
}

static inline void vfBatchComputeLaunchThreadsExact(gpu_handle_context_t context, uint64_t batchId, unsigned threadsCountX, unsigned threadsCountY, unsigned threadsCountZ, unsigned threadGroupsCountX, unsigned threadGroupsCountY, unsigned threadGroupsCountZ, const char * optionalFile, int optionalLine) {
  REDGPU_2_EXPECT(threadsCountX % threadGroupsCountX == 0);
  REDGPU_2_EXPECT(threadsCountY % threadGroupsCountY == 0);
  REDGPU_2_EXPECT(threadsCountZ % threadGroupsCountZ == 0);
  vfBatchComputeLaunchThreads(context, batchId, threadsCountX, threadsCountY, threadsCountZ, threadGroupsCountX, threadGroupsCountY, threadGroupsCountZ, optionalFile, optionalLine);
}

static inline void reiiCommandMeshLaunchThreads(gpu_handle_context_t context, ReiiHandleCommandList * list, unsigned vertexFirst, unsigned vertexCount, unsigned instanceFirst, unsigned instanceCount) {
  reiiCommandUnorderedArrayDrawInstancedEx(context, list, vertexFirst, vertexCount, instanceFirst, instanceCount);
}

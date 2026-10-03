#pragma once

#include "../../../vkfast.h"
#include "../../../extra/REII/vkfast_extra_reii.h"
#include "../../../extra/vkFast Extensions/ReBAR/vkfast_ext_rebar.h"
#include "../../../extra/vkFast Extensions/I dont care/idc_imgui.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../../../extra/stb_image 2017/stb_image.h"

static inline void vfBatchComputeLaunchThreadGroups(gpu_handle_context_t context, uint64_t batch_id, unsigned workgroups_count_x, unsigned workgroups_count_y, unsigned workgroups_count_z, const char * optional_file, int optional_line) {
  vfBatchCompute(context, batch_id, workgroups_count_x, workgroups_count_y, workgroups_count_z, optional_file, optional_line);
}

static inline void reiiCommandMeshLaunchThreads(gpu_handle_context_t context, ReiiHandleCommandList * list, unsigned vertexFirst, unsigned vertexCount, unsigned instanceFirst, unsigned instanceCount) {
  reiiCommandUnorderedArrayDrawInstancedEx(context, list, vertexFirst, vertexCount, instanceFirst, instanceCount);
}

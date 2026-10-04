// /opt/dxc custom_msaa_resolve.cs.hlsl -T cs_6_0 -Fh custom_msaa_resolve.cs.h -Vn custom_msaa_resolve_cs_code -spirv

#include "shared_data.h"

[[vk::binding(0, 0)]] RWStructuredBuffer<SharedData> sharedData;
[[vk::image_format("rgba8")]] [[vk::binding(1, 0)]] RWTexture2D<float4> texture;

[[vk::push_constant]] ConstantBuffer<Variables> variables;

[numthreads(8, 8, 1)]
void main(uint3 tid: SV_DispatchThreadId) {
  if (tid.x >= 700 || tid.y >= 700) { // NOTE(Constantine): Hardcoded resolution, see main.c comment.
    return;
  }

  texture[uint2(tid.x, tid.y)] = sharedData[0].renderTargetFloat4[tid.y][tid.x] / variables.msaaSamplesCount;
}

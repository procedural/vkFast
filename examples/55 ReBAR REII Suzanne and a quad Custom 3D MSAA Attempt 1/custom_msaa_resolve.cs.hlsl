// /opt/dxc custom_msaa_resolve.cs.hlsl -T cs_6_0 -Fh custom_msaa_resolve.cs.h -Vn custom_msaa_resolve_cs_code -spirv

#include "shared_data.h"

[[vk::binding(0, 0)]] RWStructuredBuffer<SharedData> sharedData;
[[vk::image_format("rgba8")]] [[vk::binding(1, 0)]] RWTexture2D<float4> texture;

[[vk::push_constant]] ConstantBuffer<Variables> variables;

float4 unpackUnorm4x8(uint p) { // Shader Model 6.6+: https://microsoft.github.io/DirectX-Specs/d3d/HLSL_SM_6_6_Pack_Unpack_Intrinsics.html
  float4 unpacked;
  unpacked.x = float(p & 0xFF);
  unpacked.y = float((p >> 8) & 0xFF);
  unpacked.z = float((p >> 16) & 0xFF);
  unpacked.w = float((p >> 24) & 0xFF);

  // Normalize to [0, 1] range
  return unpacked / 255.0;
}

[numthreads(8, 8, 1)]
void main(uint3 tid: SV_DispatchThreadId) {
  if (tid.x >= 700 || tid.y >= 700) { // NOTE(Constantine): Hardcoded resolution, see main.c comment.
    return;
  }

  float4 accumulatedColor = float4(0, 0, 0, 0);
  [[loop]]
  for (int i = 0; i < 16; i += 1) {
    accumulatedColor += unpackUnorm4x8(sharedData[0].msaaRenderTargets[i][tid.y][tid.x]);
  }

  texture[uint2(tid.x, tid.y)] = accumulatedColor / variables.msaaSamplesCount;
}

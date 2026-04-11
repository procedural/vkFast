// C:\VulkanSDK\1.4.341.1\Bin\dxc.exe embree_rgb_float32_to_bgra8.cs.hlsl -T cs_6_0 -Fh embree_rgb_float32_to_bgra8.cs.h -spirv

[[vk::binding(0, 0)]] RWStructuredBuffer<float4> array0;
[[vk::binding(1, 0)]] RWStructuredBuffer<uint>   array1;

/*
float4 unpackUnorm4x8(uint p) { // Shader Model 6.6+: https://microsoft.github.io/DirectX-Specs/d3d/HLSL_SM_6_6_Pack_Unpack_Intrinsics.html
  float4 unpacked;
  unpacked.x = float(p & 0xFF);
  unpacked.y = float((p >> 8) & 0xFF);
  unpacked.z = float((p >> 16) & 0xFF);
  unpacked.w = float((p >> 24) & 0xFF);

  // Normalize to [0, 1] range
  return unpacked / 255.0;
}
*/

uint packBgra8(float4 v) { // packUnorm4x8
  // 1. Clamp to [0.0, 1.0] to ensure validity
  // 2. Scale by 255
  // 3. Round to nearest unsigned integer
  uint4 packed = uint4(round(clamp(v, 0.0, 1.0) * 255.0));

  // 4. Pack into 32-bit unsigned integer
  return (packed.w << 24) | (packed.x << 16) | (packed.y << 8) | packed.z; // ARGB
}

[numthreads(32, 1, 1)]
void main(uint3 tid: SV_DispatchThreadId) {
  if (tid.x < (700 * 700)) { // NOTE(Constantine): Hardcoded window dimensions
    array1[tid.x] = packBgra8(float4(array0[tid.x].xyz, 1.0));
  }
}

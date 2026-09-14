// dxc.exe add.cs.hlsl -T cs_6_0 -Fh add.cs.h -spirv

[[vk::binding(0, 0)]] RWStructuredBuffer<float4> array0;
[[vk::binding(1, 0)]] RWStructuredBuffer<float4> array1;
[[vk::binding(2, 0)]] RWStructuredBuffer<float4> array2;

[numthreads(1, 1, 1)]
void main(uint3 tid: SV_DispatchThreadId) {
  array2[0] = array0[0] + array1[0];
}

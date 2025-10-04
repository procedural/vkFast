// dxc.exe add.cs.hlsl -T cs_6_0 -Fh add.cs.h -spirv

[[vk::binding(0, 0)]] RWStructuredBuffer<float4> array0;
[[vk::binding(1, 0)]] RWStructuredBuffer<float4> array1;

struct Variables {
  float4 salt;
};
[[vk::push_constant]] ConstantBuffer<Variables> variables;

[numthreads(1, 1, 1)]
void main(uint3 tid: SV_DispatchThreadId) {
  array1[0] = array0[0] + array0[1] + variables.salt;
}

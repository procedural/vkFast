// dxc.exe add.cs.hlsl -T cs_6_0 -Fh add.cs.h -spirv

#include "shared_data.h"

[[vk::binding(0, 0)]] RWStructuredBuffer<SharedData> sharedData;

[[vk::push_constant]] ConstantBuffer<Variables> variables;

[numthreads(1, 1, 1)]
void main(uint3 tid: SV_DispatchThreadId) {
  sharedData[0].output = sharedData[0].input[0] + sharedData[0].input[1] + variables.salt;
}

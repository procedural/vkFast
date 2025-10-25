// dxc.exe add.cs.hlsl -T cs_6_0 -Fh add.cs.h -spirv

[[vk::binding(0, 0)]] RWByteAddressBuffer memory;

struct Variables {
  uint ptr_offset_storage_input_gpu;
  uint ptr_offset_storage_output_gpu;
  uint _0;
  uint _1;
  float4 salt;
};
[[vk::push_constant]] ConstantBuffer<Variables> variables;

[numthreads(1, 1, 1)]
void main(uint3 tid: SV_DispatchThreadId) {
  uint storage_input_gpu  = variables.ptr_offset_storage_input_gpu;
  uint storage_output_gpu = variables.ptr_offset_storage_output_gpu;

  float4 val0 = memory.Load<float4>(storage_input_gpu + 0*sizeof(float4));
  float4 val1 = memory.Load<float4>(storage_input_gpu + 1*sizeof(float4));

  float4 sum = val0 + val1 + variables.salt;

  memory.Store(storage_output_gpu + 0*sizeof(float4), sum);
}

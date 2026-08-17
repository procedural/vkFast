// dxc.exe mesh_text_non3d.hlsl -DVS -T vs_6_0 -Fh mesh_text_non3d.vs.h -spirv
// dxc.exe mesh_text_non3d.hlsl -DFS -T ps_6_0 -Fh mesh_text_non3d.fs.h -spirv

[[vk::binding(0, 0)]] StructuredBuffer<float4> vertex_positions;

struct Variables {
  float4 params;
};
[[vk::push_constant]] ConstantBuffer<Variables> variables;

struct interpolated {
  float4 position: SV_Position;
};

struct render {
  float4 color: SV_Target0;
};

#ifdef VS
interpolated main(uint vid: SV_VertexID, uint iid: SV_InstanceID) {
  float4 pos = vertex_positions[variables.params.z + vid];

  interpolated output;
  output.position.x = pos.x + variables.params.x;
  output.position.y = pos.y + variables.params.y;
  output.position.z = 0;
  output.position.w = 1;
  return output;
}
#endif

#ifdef FS
render main(interpolated input) {
  render output;
  output.color = float4(1, 0, 0, 1);
  return output;
}
#endif

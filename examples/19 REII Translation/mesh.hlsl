// dxc.exe mesh.hlsl -DVS -T vs_6_0 -Fh mesh.vs.h -spirv
// dxc.exe mesh.hlsl -DFS -T ps_6_0 -Fh mesh.fs.h -spirv

[[vk::binding(0, 0)]] StructuredBuffer<float4> positions;
[[vk::binding(1, 0)]] StructuredBuffer<float4> colors;

struct interpolated {
  float4 position: SV_Position;
  float4 color:    Color;
};

struct render {
  float4 color: SV_Target0;
};

struct Variables {
  float4 camera_pos;
};
[[vk::push_constant]] ConstantBuffer<Variables> variables;

#ifdef VS
interpolated main(uint vid: SV_VertexID, uint iid: SV_InstanceID) {
  float4 pos = positions[vid];
  float4 col = colors[vid];

  interpolated output;
  output.position.x = pos.x - variables.camera_pos.x;
  output.position.y = pos.y - variables.camera_pos.y;
  output.position.z = 0.1;
  output.position.w = pos.z - variables.camera_pos.z;
  output.color      = col;
  return output;
}
#endif

#ifdef FS
render main(interpolated input) {
  render output;
  output.color = input.color;
  return output;
}
#endif

// dxc.exe mesh.hlsl -DVS -T vs_6_0 -Fh mesh.vs.h -spirv
// dxc.exe mesh.hlsl -DFS -T ps_6_0 -Fh mesh.fs.h -spirv

[[vk::binding(0, 0)]] StructuredBuffer<float4> mesh_vertex_positions;
[[vk::binding(1, 0)]] StructuredBuffer<float4> instance_colors;
[[vk::binding(2, 0)]] StructuredBuffer<float4> instance_positions;

struct interpolated {
  float4 position: SV_Position;
  float4 color:    Color;
};

struct render {
  float4 color: SV_Target0;
};

struct Variables {
  float4 camera_pos;
  float4 camera_quat;
};
[[vk::push_constant]] ConstantBuffer<Variables> variables;

float3 quatRotateVec3Fast(float3 v, float4 q) {
  return (cross(q.xyz, cross(q.xyz, v) + (v * q.w)) * 2.f) + v;
}

float4 quatNeg(float4 q) {
  return float4(-q.xyz, q.w);
}

#ifdef VS
interpolated main(uint vid: SV_VertexID, uint iid: SV_InstanceID) {
  float4 pos = mesh_vertex_positions[vid];
  float4 col = instance_colors[iid];
  float4 off = instance_positions[iid];

  float3 scaled_off = (pos.xyz * off.w) + off.xyz;
  float3 translated = scaled_off - variables.camera_pos.xyz;
  float3 rotated    = quatRotateVec3Fast(translated, quatNeg(variables.camera_quat));

  interpolated output;
  output.position.x = rotated.x;
  output.position.y = rotated.y;
  output.position.z = 0.1;
  output.position.w = rotated.z;
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

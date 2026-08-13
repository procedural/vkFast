// dxc.exe mesh_capsule.hlsl -DVS -T vs_6_0 -Fh mesh_capsule.vs.h -spirv
// dxc.exe mesh_capsule.hlsl -DFS -T ps_6_0 -Fh mesh_capsule.fs.h -spirv

[[vk::binding(0, 0)]] StructuredBuffer<float4> vertex_positions;
[[vk::binding(1, 0)]] StructuredBuffer<float4> instance_positions;
[[vk::binding(2, 0)]] StructuredBuffer<float4> instance_quaterions;

struct interpolated {
  float4 position: SV_Position;
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
  float4 pos = vertex_positions[vid];
  float4 instance_pos = instance_positions[iid];
  float4 instance_quat = instance_quaterions[iid];

  int move_after_scale = 0;
  if (pos.y >= 0.01) {
    move_after_scale = 1;
    pos.y -= 1; // Make it a sphere first, for scale to not affect capsule's final length
  }
  pos *= variables.camera_pos.w; // NOTE(Constantine): Capsule's scale is stored in variables.camera_pos.w coordinate
  if (move_after_scale == 1) {
    pos.y += instance_pos.w; // Final length is stored in instance_positions[iid].w coordinate
  }

  // Instance xforms
  float3 pos_moved = pos.xyz + instance_pos.xyz;
  pos.xyz = quatRotateVec3Fast(pos_moved, instance_quat);

  float3 translated = pos.xyz - variables.camera_pos.xyz;
  float3 rotated    = quatRotateVec3Fast(translated, quatNeg(variables.camera_quat));

  interpolated output;
  output.position.x = rotated.x;
  output.position.y = rotated.y;
  output.position.z = 0.1;
  output.position.w = rotated.z;
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

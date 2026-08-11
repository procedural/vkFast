// dxc.exe mesh_capsule.hlsl -DVS -T vs_6_0 -Fh mesh_capsule.vs.h -spirv
// dxc.exe mesh_capsule.hlsl -DFS -T ps_6_0 -Fh mesh_capsule.fs.h -spirv

[[vk::binding(0, 0)]] StructuredBuffer<float4> positions;

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
  float4 pos = positions[vid];

  int move_after_scale = 0;
  if (pos.y >= 0.01) {
    move_after_scale = 1;
    pos.y -= 1; // Make it a sphere first, for scale to not affect capsule's final length
  }
  pos *= 0.15; // NOTE(Constantine): For this example, the capsule's scale is currently hardcoded :P
  if (move_after_scale == 1) {
    pos.y += 1; // NOTE(Constantine): For this example, the capsule's final length is currently hardcoded :P
  }

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

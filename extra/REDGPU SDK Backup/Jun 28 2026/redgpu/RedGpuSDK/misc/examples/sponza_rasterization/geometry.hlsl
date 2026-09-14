// dxc.exe geometry.hlsl -DVS -T vs_6_0 -Fo geometry_vs.ir -spirv
// dxc.exe geometry.hlsl -DFS -T ps_6_0 -Fo geometry_fs.ir -spirv

struct RootSignatureConstants {
  float4 cameraPosition;
  float4 cameraRotation;
  uint   texturesArrayIndex;
  uint   texturesLayerIndex;
};
[[vk::push_constant]] ConstantBuffer<RootSignatureConstants> variables: register(b0, space4);

[[vk::binding(0, 0)]] StructuredBuffer<float4> positions:       register(t0, space0);
[[vk::binding(1, 0)]] StructuredBuffer<float2> uvs:             register(t0, space1);
[[vk::binding(2, 1)]] SamplerState             texturesSampler: register(s0, space2);
[[vk::binding(3, 0)]] Texture2DArray           textures[4]:     register(t0, space3);

struct interpolated {
  float4 position: SV_Position;
  float2 uv:       Uv;
};

struct render {
  float4 color: SV_Target0;
};

#ifdef VS
interpolated main(uint vid: SV_VertexID, uint iid: SV_InstanceID) {
  float4 pos = positions[vid];

  pos *= 0.005f;
  pos -= variables.cameraPosition;

  float4 R0;
  R0    = pos.xzzx * variables.cameraRotation.xyxy;
  pos.x = R0.x - R0.y;
  pos.z = R0.z + R0.w;
  R0    = pos.yzzy * variables.cameraRotation.zwzw;
  pos.y = R0.x - R0.y;
  pos.z = R0.z + R0.w;

  pos.xy *= float2(0.613861, 1.091308);

  interpolated output;
  output.position = float4(pos.xy, 0.01, pos.z);
  output.uv       = uvs[vid];
  return output;
}
#endif

#ifdef FS
render main(interpolated input) {
  render output;
  output.color = textures[variables.texturesArrayIndex].Sample(texturesSampler, float3(input.uv.x, 1.0f - input.uv.y, variables.texturesLayerIndex));
  return output;
}
#endif

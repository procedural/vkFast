// dxc.exe imgui_shader.hlsl -DVS -T vs_6_0 -Fh imgui_shader.vs.h -spirv
// dxc.exe imgui_shader.hlsl -DFS -T ps_6_0 -Fh imgui_shader.fs.h -spirv

[[vk::binding(0, 0)]] StructuredBuffer<float4> positions;
[[vk::binding(1, 0)]] StructuredBuffer<float4> colors;
[[vk::binding(2, 0)]] Texture2D texture;
[[vk::binding(0, 1)]] SamplerState tsampler;

struct interpolated {
  float4 position: SV_Position;
  float4 color:    Color;
  float2 uv:       UV;
};

struct render {
  float4 color: SV_Target0;
};

#ifdef VS
interpolated main(uint vid: SV_VertexID, uint iid: SV_InstanceID) {
  float4 pos = positions[vid];
  float4 col = colors[vid];

  interpolated output;
  output.position = float4(pos.x, pos.y, 0, 1);
  output.color    = col;
  output.uv       = pos.zw;
  return output;
}
#endif

#ifdef FS
render main(interpolated input) {
  render output;
  float4 image = texture.Sample(tsampler, input.uv);
  output.color = input.color * image;
  return output;
}
#endif

// dxc.exe fullscreen_triangle.hlsl -DVS -T vs_6_0 -Fo fullscreen_triangle_vs.ir -spirv
// dxc.exe fullscreen_triangle.hlsl -DFS -T ps_6_0 -Fo fullscreen_triangle_fs.ir -spirv

[[vk::binding(0, 0)]] SamplerState imageSampler: register(s0, space0);
[[vk::binding(1, 0)]] Texture2D    imageTexture: register(t0, space1);

struct interpolated {
  float4 position: SV_Position;
  float2 uv:       UV;
};

struct render {
  float4 color: SV_Target0;
};

static const float2 fullscreenTriangle[] = {
  float2(-1,-1),
  float2(-1, 3),
  float2( 3,-1)
};

#ifdef VS
interpolated main(uint vid: SV_VertexID, uint iid: SV_InstanceID) {
  interpolated output;
  output.position = float4(fullscreenTriangle[vid].xy, 1, 1);
  output.uv       = float2(output.position.x * 0.5f + 0.5f, 1.f - (output.position.y * 0.5f + 0.5f));
  return output;
}
#endif

#ifdef FS
render main(interpolated input) {
  render output;
  output.color = imageTexture.Sample(imageSampler, input.uv);
  return output;
}
#endif

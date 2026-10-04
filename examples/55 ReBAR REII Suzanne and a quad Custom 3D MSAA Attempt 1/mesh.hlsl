#include "shared_data.h"

[[vk::binding(0, 0)]] RWStructuredBuffer<SharedData> sharedData;
[[vk::binding(1, 0)]] Texture2D texture;
[[vk::binding(0, 1)]] SamplerState tsampler;

[[vk::push_constant]] ConstantBuffer<Variables> variables;

struct interpolated {
  float4 position: SV_Position;
  float2 uv:       UV;
};

float3 quatRotateVec3Fast(float3 v, float4 q) {
  return (cross(q.xyz, cross(q.xyz, v) + (v * q.w)) * 2.f) + v;
}

float4 quatNeg(float4 q) {
  return float4(-q.xyz, q.w);
}

uint packBgra8(float4 v) { // packUnorm4x8
  // 1. Clamp to [0.0, 1.0] to ensure validity
  // 2. Scale by 255
  // 3. Round to nearest unsigned integer
  uint4 packed = uint4(round(clamp(v, 0.0, 1.0) * 255.0));

  // 4. Pack into 32-bit unsigned integer
  return (packed.w << 24) | (packed.x << 16) | (packed.y << 8) | packed.z; // ARGB
}

#ifdef VS
interpolated main(uint vid: SV_VertexID, uint iid: SV_InstanceID) {
  float4 pos = sharedData[0].meshSuzanneHeadVertexPos[vid];

  float3 translated = pos.xyz - variables.cameraPos.xyz;
  float3 rotated    = quatRotateVec3Fast(translated, quatNeg(variables.cameraRotQuaternion));

  float2 uv = float2(pos.x * 0.5f + 0.5f, 1.f - (pos.y * 0.5f + 0.5f));

  interpolated output;
  output.position.x = rotated.x;
  output.position.y = rotated.y;
  output.position.z = 0.1;
  output.position.w = rotated.z;
  output.uv         = uv;
  return output;
}
#endif

#ifdef FS

[[vk::ext_extension("SPV_EXT_fragment_shader_interlock")]]

[[vk::ext_instruction(/* OpBeginInvocationInterlockEXT */ 5364)]]
void beginInvocationInterlockEXT();
[[vk::ext_instruction(/* OpEndInvocationInterlockEXT */ 5365)]]
void endInvocationInterlockEXT();

void main(interpolated input) {
  [[vk::ext_capability(/*FragmentShaderPixelInterlockEXT*/ 5378)]]
  [[vk::ext_extension("SPV_EXT_fragment_shader_interlock")]]
  vk::ext_execution_mode(/*PixelInterlockOrderedEXT*/ 5366);

  float4 c = texture.Sample(tsampler, input.uv);
  c *= c; // Gamma correcting texture colors loaded from disk.

  int x = input.position.x;
  int y = input.position.y;

  beginInvocationInterlockEXT();
  sharedData[0].msaaRenderTargets[variables.msaaCurrentRenderTargetIndex][y][x] = packBgra8(c);
  endInvocationInterlockEXT();
}
#endif

#include "shared_data.h"

[[vk::binding(0, 0)]] RWStructuredBuffer<SharedData> sharedData;
[[vk::binding(1, 0)]] Texture2D texture;
[[vk::binding(0, 1)]] SamplerState tsampler;

[[vk::push_constant]] ConstantBuffer<Variables> variables;

struct interpolated {
  float4 position: SV_Position;
  float2 uv:       UV;
};

struct render {
  float4 color: SV_Target0;
};

float3 quatRotateVec3Fast(float3 v, float4 q) {
  return (cross(q.xyz, cross(q.xyz, v) + (v * q.w)) * 2.f) + v;
}

float4 quatNeg(float4 q) {
  return float4(-q.xyz, q.w);
}

#ifdef VS
interpolated main(uint vid: SV_VertexID, uint iid: SV_InstanceID) {
  float4 pos = sharedData[0].meshQuadVertexPos[vid];
  float2 uv  = sharedData[0].meshQuadVertexUVs[vid];

  float3 cameraPos = variables.cameraPos.xyz;
  float4 cameraRotQuaternion = variables.cameraRotQuaternion;

  // Custom 3D MSAA
  float cameraOffsetMultiplier = variables.msaaCameraOffsetMultiplier;
  {
    int msaaCurrentSample = variables.msaaCurrentSample;

    float3 vertexTranslated = pos.xyz - cameraPos;

    float3 msaa_camera_offset_xyz;
    msaa_camera_offset_xyz.x = sharedData[0].msaaSamplesOffsetTableX[msaaCurrentSample] * cameraOffsetMultiplier * length(vertexTranslated);
    msaa_camera_offset_xyz.y = sharedData[0].msaaSamplesOffsetTableY[msaaCurrentSample] * cameraOffsetMultiplier * length(vertexTranslated);
    msaa_camera_offset_xyz.z = sharedData[0].msaaSamplesOffsetTableZ[msaaCurrentSample] * cameraOffsetMultiplier * length(vertexTranslated);

    msaa_camera_offset_xyz = quatRotateVec3Fast(msaa_camera_offset_xyz, cameraRotQuaternion);

    cameraPos += msaa_camera_offset_xyz;
  }

  float3 translated = pos.xyz - cameraPos;
  float3 rotated    = quatRotateVec3Fast(translated, quatNeg(cameraRotQuaternion));

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
render main(interpolated input) {
  render output;
  float4 c = texture.Sample(tsampler, input.uv);
  c *= c; // Gamma correcting texture colors loaded from disk.
  output.color = c;
  return output;
}
#endif

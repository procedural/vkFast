struct SharedData {
  float4 meshSuzanneHeadVertexPos[2904];
  float4 meshSuzanneHeadVertexCol[2904];
  float4 meshQuadVertexPos[6];
  float2 meshQuadVertexUVs[6];
  uint   msaaRenderTargets[16][700][700]; // NOTE(Constantine): Hardcoded resolution for now, see the related comment in main.c. 16 is the max number of MSAA samples.
};

struct Variables {
  float4 cameraPos;
  float4 cameraRotQuaternion;
  float  msaaSamplesCount;
  uint   msaaCurrentRenderTargetIndex;
  float  _[2];
};

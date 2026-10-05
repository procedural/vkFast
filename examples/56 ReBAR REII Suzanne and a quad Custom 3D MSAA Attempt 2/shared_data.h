struct SharedData {
  float4 meshSuzanneHeadVertexPos[2904];
  float4 meshSuzanneHeadVertexCol[2904];
  float4 meshQuadVertexPos[6];
  float2 meshQuadVertexUVs[6];
  float4 renderTargetFloat4[700][700]; // NOTE(Constantine): Hardcoded resolution for now, see the related comment in main.c.
};

struct Variables {
  float4 cameraPos;
  float4 cameraRotQuaternion;
  int    msaaSamplesCount;
  int    msaaResolve;
  float  _[2];
};

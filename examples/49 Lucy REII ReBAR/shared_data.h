struct SharedData {
  float4 meshSuzanneHeadVertexPos[2904];
  float4 meshSuzanneHeadVertexCol[2904];
  float4 meshLucyVertexPos[14027872];
  int4   meshLucyTrianglesThreeVertexIndices[28055728];
};

struct Variables {
  float4 cameraPos;
  float4 cameraRotQuaternion;

  int meshIndex;
  int _[3];
};

#include "../../vkfast.h"

#define FF __FILE__
#define LL __LINE__

#define SNL() "\n"
#define STR(x) #x SNL()
#define countof(x) (sizeof(x) / sizeof((x)[0]))

int main() {
  vfContextInit(1, FF, LL);
  vfWindowFullscreen(NULL, "Hello Triangle", 1920, 1080, 1, FF, LL);
  
  gpu_storage_info_t mesh_info = {0};
  mesh_info.optionalVertexCount = 3;
  mesh_info.bytesCount = mesh_info.optionalVertexCount * 3*sizeof(float);
  gpu_storage_t mesh = {0};
  vfStorageCreateFromStruct(&mesh_info, &mesh, FF, LL);
  
  mesh.as_vec3[0].x = -0.5f;
  mesh.as_vec3[0].y = -0.5f;
  mesh.as_vec3[0].z =  0.0f;
  
  mesh.as_vec3[1].x =  0.0f;
  mesh.as_vec3[1].y =  0.5f;
  mesh.as_vec3[1].z =  0.0f;
  
  mesh.as_vec3[2].x =  0.5f;
  mesh.as_vec3[2].y = -0.5f;
  mesh.as_vec3[2].z =  0.0f;
  
  uint64_t copy = vfBatchBegin(FF, LL);
  vfBatchStorageCopyFromCpuToGpu(copy, mesh.id, FF, LL);
  vfBatchEnd(copy, FF, LL);
  uint64_t async = vfAsyncBatchExecute(1, &copy, FF, LL);
  vfAsyncWaitToFinish(async, FF, LL);

  gpu_cmd_t cmd[1] = {0};
  cmd[0].count = mesh.info.optionalVertexCount;
  cmd[0].instance_count = 1;
  
  const char * vs_str =
  SNL(                                                     )
  STR(  layout(binding = 0) uniform samplerBuffer in_pos;  )
  SNL(                                                     )
  STR(  void main()                                        )
  STR(  {                                                  )
  STR(    vec3 pos = texelFetch(in_pos, gl_VertexID).xyz;  )
  STR(    gl_Position = vec4(pos, 1.f);                    )
  STR(  }                                                  );
  
  const char * fs_str =
  SNL(                        )
  STR(  out vec4 color;       )
  SNL(                        )
  STR(  void main()           )
  STR(  {                     )
  STR(    color = vec4(1.f);  )
  STR(  }                     );

  uint64_t vs = vfProgramCreateFromStringVertProgram(vs_str, FF, LL);
  uint64_t fs = vfProgramCreateFromStringFragProgram(fs_str, FF, LL);
  uint64_t pp = vfProgramPipelineCreate(vs, fs, FF, LL);
  
  uint64_t state_storages[1] = {mesh.id};
  
  while (vfWindowLoop()) {
    uint64_t batch = vfBatchBegin(FF, LL);
    vfBatchBindStorage(batch, countof(state_storages), state_storages, FF, LL);
    vfBatchBindProgramPipeline(batch, pp, FF, LL);
    vfBatchClear(batch, FF, LL);
    vfBatchDraw(batch, countof(cmd), cmd, FF, LL);
    vfBatchEnd(batch, FF, LL);
    vfBatchExecute(1, &batch, FF, LL);
  }
  
  vfContextDeinit(0, NULL, FF, LL);
  vfExit(0);
}

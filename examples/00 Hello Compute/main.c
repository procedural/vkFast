#include "../../vkfast.h"

#define FF __FILE__
#define LL __LINE__

#define SNL() "\n"
#define STR(x) #x SNL()
#define countof(x) (sizeof(x) / sizeof((x)[0]))

int main() {
  const int window_w = 1920;
  const int window_h = 1080;

  vfContextInit(1, NULL, FF, LL);
  vfWindowFullscreen(NULL, "Hello Triangle", window_w, window_h, 1, FF, LL);
  
  gpu_storage_info_t mesh_info = {0};
  mesh_info.storage_type = GPU_STORAGE_TYPE_CPU_UPLOAD;
  mesh_info.optional_vertex_count = 3;
  mesh_info.bytes_count = mesh_info.optional_vertex_count * 3*sizeof(float);
  gpu_storage_t mesh = {0};
  gpu_storage_t mesh_gpu = {0};
  vfStorageCreateFromStruct(&mesh_info, &mesh, FF, LL);
  mesh_info.storage_type = GPU_STORAGE_TYPE_GPU_ONLY;
  vfStorageCreateFromStruct(&mesh_info, &mesh_gpu, FF, LL);
  
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
  vfBatchStorageCopyFromCpuToGpu(copy, mesh.id, mesh_gpu.id, FF, LL);
  vfBatchEnd(copy, FF, LL);
  uint64_t async = vfAsyncBatchExecute(1, &copy, FF, LL);
  vfAsyncWaitToFinish(async, FF, LL);

  gpu_cmd_t cmd[1] = {0};
  cmd[0].count = mesh.info.optional_vertex_count;
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

  gpu_program_info_t vs_info = {0};
  vs_info.program_binary_bytes_count = countof(vs_binary);
  vs_info.program_binary             = vs_binary;

  gpu_program_info_t fs_info = {0};
  fs_info.program_binary_bytes_count = countof(fs_binary);
  fs_info.program_binary             = fs_binary;

  uint64_t vs = vfProgramCreateFromBinaryVertProgram(&vs_info, FF, LL);
  uint64_t fs = vfProgramCreateFromBinaryFragProgram(&fs_info, FF, LL);

  gpu_program_pipeline_info_t pp_info = {0};
  pp_info.vert_program                        = vs;
  pp_info.frag_program                        = fs;
  pp_info.parameters.variablesSlot            = 0;
  pp_info.parameters.variablesVisibleToStages = RED_VISIBLE_TO_STAGE_BITFLAG_VERTEX;
  pp_info.parameters.variablesBytesCount      = 0;
  pp_info.parameters.structsDeclarationsCount = 1;
  pp_info.parameters.structsDeclarations[0];
  pp_info.parameters.structsDeclarations[0].structDeclarationMembersCount = 1;
  RedStructDeclarationMember slot0 = {0};
  slot0.slot            = 0;
  slot0.type            = RED_STRUCT_MEMBER_TYPE_ARRAY_RO_RW;
  slot0.count           = 1;
  slot0.visibleToStages = RED_VISIBLE_TO_STAGE_BITFLAG_VERTEX;
  slot0.inlineSampler   = NULL;
  pp_info.parameters.structsDeclarations[0].structDeclarationMembers             = &slot0;
  pp_info.parameters.structsDeclarations[0].structDeclarationMembersArrayROCount = 0;
  pp_info.parameters.structsDeclarations[0].structDeclarationMembersArrayRO      = NULL;
  RedProcedureState state = {0};
  state.inputAssemblyTopology                          = RED_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
  state.inputAssemblyPrimitiveRestartEnable            = 0;
  state.viewportDynamic                                = 0;
  state.viewportStaticX                                = 0;
  state.viewportStaticY                                = 0;
  state.viewportStaticWidth                            = window_w;
  state.viewportStaticHeight                           = window_h;
  state.viewportStaticDepthMin                         = 0;
  state.viewportStaticDepthMax                         = 1;
  state.scissorDynamic                                 = 0;
  state.scissorStaticX                                 = 0;
  state.scissorStaticY                                 = 0;
  state.scissorStaticWidth                             = window_w;
  state.scissorStaticHeight                            = window_h;
  state.rasterizationDepthClampEnable                  = 0;
  state.rasterizationDiscardAllPrimitivesEnable        = 0;
  state.rasterizationCullMode                          = RED_CULL_MODE_NONE;
  state.rasterizationFrontFace                         = RED_FRONT_FACE_CLOCKWISE;
  state.rasterizationDepthBiasEnable                   = 0;
  state.rasterizationDepthBiasDynamic                  = 0;
  state.rasterizationDepthBiasStaticConstantFactor     = 0;
  state.rasterizationDepthBiasStaticClamp              = 0;
  state.rasterizationDepthBiasStaticSlopeFactor        = 0;
  state.multisampleCount                               = RED_MULTISAMPLE_COUNT_BITFLAG_1;
  state.multisampleSampleMask                          = NULL;
  state.multisampleSampleShadingEnable                 = 0;
  state.multisampleSampleShadingMin                    = 0;
  state.multisampleAlphaToCoverageEnable               = 1;
  state.multisampleAlphaToOneEnable                    = 0;
  state.depthTestEnable                                = 0;
  state.depthTestDepthWriteEnable                      = 0;
  state.depthTestDepthCompareOp                        = RED_COMPARE_OP_GREATER_OR_EQUAL;
  state.depthTestBoundsTestEnable                      = 0;
  state.depthTestBoundsTestDynamic                     = 0;
  state.depthTestBoundsTestStaticMin                   = 0;
  state.depthTestBoundsTestStaticMax                   = 1;
  state.stencilTestEnable                              = 0;
  state.stencilTestFrontAndBackDynamicCompareMask      = 0;
  state.stencilTestFrontAndBackDynamicWriteMask        = 0;
  state.stencilTestFrontAndBackDynamicReference        = 0;
  state.stencilTestFrontStencilTestFailOp              = RED_STENCIL_OP_KEEP;
  state.stencilTestFrontStencilTestPassDepthTestPassOp = RED_STENCIL_OP_KEEP;
  state.stencilTestFrontStencilTestPassDepthTestFailOp = RED_STENCIL_OP_KEEP;
  state.stencilTestFrontCompareOp                      = RED_COMPARE_OP_NEVER;
  state.stencilTestBackStencilTestFailOp               = RED_STENCIL_OP_KEEP;
  state.stencilTestBackStencilTestPassDepthTestPassOp  = RED_STENCIL_OP_KEEP;
  state.stencilTestBackStencilTestPassDepthTestFailOp  = RED_STENCIL_OP_KEEP;
  state.stencilTestBackCompareOp                       = RED_COMPARE_OP_NEVER;
  state.stencilTestFrontAndBackDynamicCompareMask      = 0;
  state.stencilTestFrontAndBackDynamicWriteMask        = 0;
  state.stencilTestFrontAndBackDynamicReference        = 0;
  state.stencilTestFrontAndBackStaticCompareMask       = 0;
  state.stencilTestFrontAndBackStaticWriteMask         = 0;
  state.stencilTestFrontAndBackStaticReference         = 0;
  state.blendLogicOpEnable                             = 0;
  state.blendLogicOp                                   = RED_LOGIC_OP_CLEAR;
  state.blendConstantsDynamic                          = 0;
  state.blendConstantsStatic[0]                        = 0;
  state.blendConstantsStatic[1]                        = 0;
  state.blendConstantsStatic[2]                        = 0;
  state.blendConstantsStatic[3]                        = 0;
  state.outputColorsCount                              = 1;
  state.outputColorsWriteMask[0]                       = RED_COLOR_COMPONENT_BITFLAG_R | RED_COLOR_COMPONENT_BITFLAG_G | RED_COLOR_COMPONENT_BITFLAG_B | RED_COLOR_COMPONENT_BITFLAG_A;
  state.outputColorsWriteMask[1]                       = RED_COLOR_COMPONENT_BITFLAG_R | RED_COLOR_COMPONENT_BITFLAG_G | RED_COLOR_COMPONENT_BITFLAG_B | RED_COLOR_COMPONENT_BITFLAG_A;
  state.outputColorsWriteMask[2]                       = RED_COLOR_COMPONENT_BITFLAG_R | RED_COLOR_COMPONENT_BITFLAG_G | RED_COLOR_COMPONENT_BITFLAG_B | RED_COLOR_COMPONENT_BITFLAG_A;
  state.outputColorsWriteMask[3]                       = RED_COLOR_COMPONENT_BITFLAG_R | RED_COLOR_COMPONENT_BITFLAG_G | RED_COLOR_COMPONENT_BITFLAG_B | RED_COLOR_COMPONENT_BITFLAG_A;
  state.outputColorsWriteMask[4]                       = RED_COLOR_COMPONENT_BITFLAG_R | RED_COLOR_COMPONENT_BITFLAG_G | RED_COLOR_COMPONENT_BITFLAG_B | RED_COLOR_COMPONENT_BITFLAG_A;
  state.outputColorsWriteMask[5]                       = RED_COLOR_COMPONENT_BITFLAG_R | RED_COLOR_COMPONENT_BITFLAG_G | RED_COLOR_COMPONENT_BITFLAG_B | RED_COLOR_COMPONENT_BITFLAG_A;
  state.outputColorsWriteMask[6]                       = RED_COLOR_COMPONENT_BITFLAG_R | RED_COLOR_COMPONENT_BITFLAG_G | RED_COLOR_COMPONENT_BITFLAG_B | RED_COLOR_COMPONENT_BITFLAG_A;
  state.outputColorsWriteMask[7]                       = RED_COLOR_COMPONENT_BITFLAG_R | RED_COLOR_COMPONENT_BITFLAG_G | RED_COLOR_COMPONENT_BITFLAG_B | RED_COLOR_COMPONENT_BITFLAG_A;
  for (unsigned i = 0; i < 8; i += 1) {
    state.outputColorsBlendEnable[i]                   = 0;
    state.outputColorsBlendColorFactorSource[i]        = RED_BLEND_FACTOR_SOURCE_ALPHA;
    state.outputColorsBlendColorFactorTarget[i]        = RED_BLEND_FACTOR_ONE_MINUS_SOURCE_ALPHA;
    state.outputColorsBlendColorOp[i]                  = RED_BLEND_OP_ADD;
    state.outputColorsBlendAlphaFactorSource[i]        = RED_BLEND_FACTOR_SOURCE_ALPHA;
    state.outputColorsBlendAlphaFactorTarget[i]        = RED_BLEND_FACTOR_ONE_MINUS_SOURCE_ALPHA;
    state.outputColorsBlendAlphaOp[i]                  = RED_BLEND_OP_ADD;
  }
  uint64_t pp = vfProgramPipelineCreate(&pp_info, FF, LL);
  
  uint64_t state_storages[1] = {mesh_gpu.id};
  
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

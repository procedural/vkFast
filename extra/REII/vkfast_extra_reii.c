#include "../../vkfast.h"
#include "../../vkfast_ids.h"

#ifdef _WIN32
#undef GPU_API_PRE
#undef GPU_API_POST
#define GPU_API_PRE __declspec(dllexport)
#define GPU_API_POST
#endif

#include "vkfast_extra_reii.h"
#include <math.h> // For log2

#ifndef __cplusplus
#define REDGPU_DISABLE_NAMED_PARAMETERS
#endif
#include "C:/RedGpuSDK/misc/np/np.h"
#include "C:/RedGpuSDK/misc/np/np_redgpu.h"
#include "C:/RedGpuSDK/misc/np/np_redgpu_2.h"
#include "C:/RedGpuSDK/misc/np/np_redgpu_wsi.h"

static RedCullMode ReiiCullModeToRed(ReiiCullMode x) {
  const char * optionalFile = NULL;
  int optionalLine = 0;
  if      (x == REII_CULL_MODE_NONE)  { return RED_CULL_MODE_NONE; }
  else if (x == REII_CULL_MODE_FRONT) { return RED_CULL_MODE_FRONT; }
  else if (x == REII_CULL_MODE_BACK)  { return RED_CULL_MODE_BACK; }
  REDGPU_2_EXPECT(0 || !"Invalid enum value");
  return (RedCullMode)0;
}

static RedFrontFace ReiiFrontFaceToRed(ReiiFrontFace x) {
  const char * optionalFile = NULL;
  int optionalLine = 0;
  if      (x == REII_FRONT_FACE_COUNTER_CLOCKWISE) { return RED_FRONT_FACE_COUNTER_CLOCKWISE; }
  else if (x == REII_FRONT_FACE_CLOCKWISE)         { return RED_FRONT_FACE_CLOCKWISE; }
  REDGPU_2_EXPECT(0 || !"Invalid enum value");
  return (RedFrontFace)0;
}

static RedCompareOp ReiiCompareOpToRed(ReiiCompareOp x) {
  const char * optionalFile = NULL;
  int optionalLine = 0;
  if      (x == REII_COMPARE_OP_NEVER)            { return RED_COMPARE_OP_NEVER; }
  else if (x == REII_COMPARE_OP_LESS)             { return RED_COMPARE_OP_LESS; }
  else if (x == REII_COMPARE_OP_EQUAL)            { return RED_COMPARE_OP_EQUAL; }
  else if (x == REII_COMPARE_OP_LESS_OR_EQUAL)    { return RED_COMPARE_OP_LESS_OR_EQUAL; }
  else if (x == REII_COMPARE_OP_GREATER)          { return RED_COMPARE_OP_GREATER; }
  else if (x == REII_COMPARE_OP_NOT_EQUAL)        { return RED_COMPARE_OP_NOT_EQUAL; }
  else if (x == REII_COMPARE_OP_GREATER_OR_EQUAL) { return RED_COMPARE_OP_GREATER_OR_EQUAL; }
  else if (x == REII_COMPARE_OP_ALWAYS)           { return RED_COMPARE_OP_ALWAYS; }
  REDGPU_2_EXPECT(0 || !"Invalid enum value");
  return (RedCompareOp)0;
}

static RedStencilOp ReiiStencilOpToRed(ReiiStencilOp x) {
  const char * optionalFile = NULL;
  int optionalLine = 0;
  if      (x == REII_STENCIL_OP_KEEP)                { return RED_STENCIL_OP_KEEP; }
  else if (x == REII_STENCIL_OP_ZERO)                { return RED_STENCIL_OP_ZERO; }
  else if (x == REII_STENCIL_OP_REPLACE)             { return RED_STENCIL_OP_REPLACE; }
  else if (x == REII_STENCIL_OP_INCREMENT_AND_CLAMP) { return RED_STENCIL_OP_INCREMENT_AND_CLAMP; }
  else if (x == REII_STENCIL_OP_DECREMENT_AND_CLAMP) { return RED_STENCIL_OP_DECREMENT_AND_CLAMP; }
  else if (x == REII_STENCIL_OP_INVERT)              { return RED_STENCIL_OP_INVERT; }
  REDGPU_2_EXPECT(0 || !"Invalid enum value");
  return (RedStencilOp)0;
}

static RedLogicOp ReiiLogicOpToRed(ReiiLogicOp x) {
  const char * optionalFile = NULL;
  int optionalLine = 0;
  if      (x == REII_LOGIC_OP_CLEAR)         { return RED_LOGIC_OP_CLEAR; }
  else if (x == REII_LOGIC_OP_AND)           { return RED_LOGIC_OP_AND; }
  else if (x == REII_LOGIC_OP_AND_REVERSE)   { return RED_LOGIC_OP_AND_REVERSE; }
  else if (x == REII_LOGIC_OP_COPY)          { return RED_LOGIC_OP_COPY; }
  else if (x == REII_LOGIC_OP_AND_INVERTED)  { return RED_LOGIC_OP_AND_INVERTED; }
  else if (x == REII_LOGIC_OP_NO_OP)         { return RED_LOGIC_OP_NO_OP; }
  else if (x == REII_LOGIC_OP_XOR)           { return RED_LOGIC_OP_XOR; }
  else if (x == REII_LOGIC_OP_OR)            { return RED_LOGIC_OP_OR; }
  else if (x == REII_LOGIC_OP_NOR)           { return RED_LOGIC_OP_NOR; }
  else if (x == REII_LOGIC_OP_EQUIVALENT)    { return RED_LOGIC_OP_EQUIVALENT; }
  else if (x == REII_LOGIC_OP_INVERT)        { return RED_LOGIC_OP_INVERT; }
  else if (x == REII_LOGIC_OP_OR_REVERSE)    { return RED_LOGIC_OP_OR_REVERSE; }
  else if (x == REII_LOGIC_OP_COPY_INVERTED) { return RED_LOGIC_OP_COPY_INVERTED; }
  else if (x == REII_LOGIC_OP_OR_INVERTED)   { return RED_LOGIC_OP_OR_INVERTED; }
  else if (x == REII_LOGIC_OP_NAND)          { return RED_LOGIC_OP_NAND; }
  else if (x == REII_LOGIC_OP_SET)           { return RED_LOGIC_OP_SET; }
  REDGPU_2_EXPECT(0 || !"Invalid enum value");
  return (RedLogicOp)0;
}

static RedBlendFactor ReiiBlendFactorToRed(ReiiBlendFactor x) {
  const char * optionalFile = NULL;
  int optionalLine = 0;
  if      (x == REII_BLEND_FACTOR_ZERO)                     { return RED_BLEND_FACTOR_ZERO; }
  else if (x == REII_BLEND_FACTOR_ONE)                      { return RED_BLEND_FACTOR_ONE; }
  else if (x == REII_BLEND_FACTOR_SOURCE_COLOR)             { return RED_BLEND_FACTOR_SOURCE_COLOR; }
  else if (x == REII_BLEND_FACTOR_ONE_MINUS_SOURCE_COLOR)   { return RED_BLEND_FACTOR_ONE_MINUS_SOURCE_COLOR; }
  else if (x == REII_BLEND_FACTOR_TARGET_COLOR)             { return RED_BLEND_FACTOR_TARGET_COLOR; }
  else if (x == REII_BLEND_FACTOR_ONE_MINUS_TARGET_COLOR)   { return RED_BLEND_FACTOR_ONE_MINUS_TARGET_COLOR; }
  else if (x == REII_BLEND_FACTOR_SOURCE_ALPHA)             { return RED_BLEND_FACTOR_SOURCE_ALPHA; }
  else if (x == REII_BLEND_FACTOR_ONE_MINUS_SOURCE_ALPHA)   { return RED_BLEND_FACTOR_ONE_MINUS_SOURCE_ALPHA; }
  else if (x == REII_BLEND_FACTOR_TARGET_ALPHA)             { return RED_BLEND_FACTOR_TARGET_ALPHA; }
  else if (x == REII_BLEND_FACTOR_ONE_MINUS_TARGET_ALPHA)   { return RED_BLEND_FACTOR_ONE_MINUS_TARGET_ALPHA; }
  else if (x == REII_BLEND_FACTOR_CONSTANT_COLOR)           { return RED_BLEND_FACTOR_CONSTANT_COLOR; }
  else if (x == REII_BLEND_FACTOR_ONE_MINUS_CONSTANT_COLOR) { return RED_BLEND_FACTOR_ONE_MINUS_CONSTANT_COLOR; }
  else if (x == REII_BLEND_FACTOR_SOURCE_ALPHA_SATURATE)    { return RED_BLEND_FACTOR_SOURCE_ALPHA_SATURATE; }
  REDGPU_2_EXPECT(0 || !"Invalid enum value");
  return (RedBlendFactor)0;
}

static RedBlendOp ReiiBlendOpToRed(ReiiBlendOp x) {
  const char * optionalFile = NULL;
  int optionalLine = 0;
  if      (x == REII_BLEND_OP_ADD)              { return RED_BLEND_OP_ADD; }
  else if (x == REII_BLEND_OP_SUBTRACT)         { return RED_BLEND_OP_SUBTRACT; }
  else if (x == REII_BLEND_OP_REVERSE_SUBTRACT) { return RED_BLEND_OP_REVERSE_SUBTRACT; }
  else if (x == REII_BLEND_OP_MIN)              { return RED_BLEND_OP_MIN; }
  else if (x == REII_BLEND_OP_MAX)              { return RED_BLEND_OP_MAX; }
  REDGPU_2_EXPECT(0 || !"Invalid enum value");
  return (RedBlendOp)0;
}

static RedSamplerFiltering RiiSamplerFilteringToRed(ReiiSamplerFiltering x) {
  const char * optionalFile = NULL;
  int optionalLine = 0;
  if      (x == REII_SAMPLER_FILTERING_NEAREST)             { return RED_SAMPLER_FILTERING_NEAREST; }
  else if (x == REII_SAMPLER_FILTERING_NEAREST_MIP_NEAREST) { return RED_SAMPLER_FILTERING_NEAREST; }
  else if (x == REII_SAMPLER_FILTERING_NEAREST_MIP_LINEAR)  { return RED_SAMPLER_FILTERING_NEAREST; }
  else if (x == REII_SAMPLER_FILTERING_LINEAR)              { return RED_SAMPLER_FILTERING_LINEAR; }
  else if (x == REII_SAMPLER_FILTERING_LINEAR_MIP_NEAREST)  { return RED_SAMPLER_FILTERING_LINEAR; }
  else if (x == REII_SAMPLER_FILTERING_LINEAR_MIP_LINEAR)   { return RED_SAMPLER_FILTERING_LINEAR; }
  REDGPU_2_EXPECT(0 || !"Invalid enum value");
  return (RedSamplerFiltering)0;
}

static RedSamplerFilteringMip RiiSamplerFilteringMipToRed(ReiiSamplerFiltering x) {
  const char * optionalFile = NULL;
  int optionalLine = 0;
  if      (x == REII_SAMPLER_FILTERING_NEAREST)             { return RED_SAMPLER_FILTERING_MIP_NEAREST; }
  else if (x == REII_SAMPLER_FILTERING_NEAREST_MIP_NEAREST) { return RED_SAMPLER_FILTERING_MIP_NEAREST; }
  else if (x == REII_SAMPLER_FILTERING_NEAREST_MIP_LINEAR)  { return RED_SAMPLER_FILTERING_MIP_LINEAR; }
  else if (x == REII_SAMPLER_FILTERING_LINEAR)              { return RED_SAMPLER_FILTERING_MIP_LINEAR; }
  else if (x == REII_SAMPLER_FILTERING_LINEAR_MIP_NEAREST)  { return RED_SAMPLER_FILTERING_MIP_NEAREST; }
  else if (x == REII_SAMPLER_FILTERING_LINEAR_MIP_LINEAR)   { return RED_SAMPLER_FILTERING_MIP_LINEAR; }
  REDGPU_2_EXPECT(0 || !"Invalid enum value");
  return (RedSamplerFilteringMip)0;
}

static RedSamplerBehaviorOutsideTextureCoordinate ReiiSamplerBehaviorOutsideTextureCoordinateToRed(ReiiSamplerBehaviorOutsideTextureCoordinate x) {
  const char * optionalFile = NULL;
  int optionalLine = 0;
  if      (x == REII_SAMPLER_BEHAVIOR_OUTSIDE_TEXTURE_COORDINATE_REPEAT)              { return RED_SAMPLER_BEHAVIOR_OUTSIDE_TEXTURE_COORDINATE_REPEAT; }
  else if (x == REII_SAMPLER_BEHAVIOR_OUTSIDE_TEXTURE_COORDINATE_CLAMP_TO_EDGE_VALUE) { return RED_SAMPLER_BEHAVIOR_OUTSIDE_TEXTURE_COORDINATE_CLAMP_TO_EDGE_VALUE; }
  REDGPU_2_EXPECT(0 || !"Invalid enum value");
  return (RedSamplerBehaviorOutsideTextureCoordinate)0;
}

GPU_API_PRE void GPU_API_POST reiiMeshStateCompile(gpu_handle_context_t context, gpu_extra_reii_mesh_state_compile_info_t * state) {
  const char * optionalFile = NULL;
  int optionalLine = 0;

  vf_handle_context_t * vkfast = (vf_handle_context_t *)(void *)context;

  RedHandleGpu gpu = vkfast->gpu;

  REDGPU_2_EXPECTWG(state->state_multisample_count != 0);
  if (state->output_depth_stencil_enable == 1) { 
    REDGPU_2_EXPECTWG(state->output_depth_stencil_format != RED_FORMAT_UNDEFINED);
    // NOTE(Constantine): Check for the only supported texture formats for now.
    REDGPU_2_EXPECTWG(
      state->output_depth_stencil_format == RED_FORMAT_DEPTH_32_FLOAT ||
      state->output_depth_stencil_format == RED_FORMAT_DEPTH_24_UINT_TO_FLOAT_0_1_STENCIL_8_UINT
    );
  }
  REDGPU_2_EXPECTWG(state->output_color_format != RED_FORMAT_UNDEFINED);
  // NOTE(Constantine): Check for the only supported texture formats for now.
  REDGPU_2_EXPECTWG(
    state->output_color_format == RED_FORMAT_PRESENT_BGRA_8_8_8_8_UINT_TO_FLOAT_0_1
  );

  // To destroy
  RedHandleGpuCode gpuCodeVertex = NULL;
  np(redCreateGpuCode,
    "context", vkfast->context,
    "gpu", vkfast->gpu,
    "handleName", state->state->programVertex.optional_debug_name,
    "irBytesCount", state->state->programVertex.program_binary_bytes_count,
    "ir", (const void *)state->state->programVertex.program_binary,
    "outGpuCode", &gpuCodeVertex,
    "outStatuses", NULL,
    "optionalFile", optionalFile,
    "optionalLine", optionalLine,
    "optionalUserData", NULL
  );
  REDGPU_2_EXPECTWG(gpuCodeVertex != NULL);

  // To destroy
  RedHandleGpuCode gpuCodeFragment = NULL;
  np(redCreateGpuCode,
    "context", vkfast->context,
    "gpu", vkfast->gpu,
    "handleName", state->state->programFragment.optional_debug_name,
    "irBytesCount", state->state->programFragment.program_binary_bytes_count,
    "ir", (const void *)state->state->programFragment.program_binary,
    "outGpuCode", &gpuCodeFragment,
    "outStatuses", NULL,
    "optionalFile", optionalFile,
    "optionalLine", optionalLine,
    "optionalUserData", NULL
  );
  REDGPU_2_EXPECTWG(gpuCodeFragment != NULL);

  if (state->variables_bytes_count > 0) {
    for (unsigned i = 0; i < state->struct_members_count; i += 1) {
      REDGPU_2_EXPECTWG(state->variables_slot != state->struct_members[i].slot);
    }
  }

  Red2ProcedureParametersDeclaration parameters = {0};
  parameters.variablesSlot            = state->variables_slot;
  parameters.variablesVisibleToStages = state->variables_bytes_count == 0 ? 0 : (RED_VISIBLE_TO_STAGE_BITFLAG_VERTEX | RED_VISIBLE_TO_STAGE_BITFLAG_FRAGMENT);
  parameters.variablesBytesCount      = state->variables_bytes_count;
  parameters.structsDeclarationsCount = state->struct_members_count == 0 ? 0 : 1;
  parameters.structsDeclarations[0].structDeclarationMembersCount        = state->struct_members_count;
  parameters.structsDeclarations[0].structDeclarationMembers             = state->struct_members;
  parameters.structsDeclarations[0].structDeclarationMembersArrayROCount = 0;
  parameters.structsDeclarations[0].structDeclarationMembersArrayRO      = NULL;

  // To destroy
  Red2ProcedureParametersAndDeclarations procedureParameters = {0};
  np(red2CreateProcedureParameters,
    "context", vkfast->context,
    "gpu", vkfast->gpu,
    "handleName", state->optional_debug_name,
    "procedureParametersDeclaration", &parameters,
    "outProcedureParametersAndDeclarations", &procedureParameters,
    "outStatuses", NULL,
    "optionalFile", optionalFile,
    "optionalLine", optionalLine,
    "optionalUserData", NULL
  );
  REDGPU_2_EXPECTWG(procedureParameters.procedureParameters != NULL);

  RedOutputDeclarationMembers outputs = {0};
  outputs.depthStencilEnable                        = state->output_depth_stencil_enable;
  outputs.depthStencilFormat                        = state->output_depth_stencil_format;
  outputs.depthStencilMultisampleCount              = state->state_multisample_count;
  outputs.depthStencilDepthSetProcedureOutputOp     = RED_SET_PROCEDURE_OUTPUT_OP_PRESERVE;
  outputs.depthStencilDepthEndProcedureOutputOp     = RED_END_PROCEDURE_OUTPUT_OP_PRESERVE;
  outputs.depthStencilStencilSetProcedureOutputOp   = RED_SET_PROCEDURE_OUTPUT_OP_PRESERVE;
  outputs.depthStencilStencilEndProcedureOutputOp   = RED_END_PROCEDURE_OUTPUT_OP_PRESERVE;
  outputs.depthStencilSharesMemoryWithAnotherMember = 0;
  outputs.colorsCount                               = 1;
  outputs.colorsFormat[0]                           = state->output_color_format;
  outputs.colorsFormat[1]                           = RED_FORMAT_UNDEFINED;
  outputs.colorsFormat[2]                           = RED_FORMAT_UNDEFINED;
  outputs.colorsFormat[3]                           = RED_FORMAT_UNDEFINED;
  outputs.colorsFormat[4]                           = RED_FORMAT_UNDEFINED;
  outputs.colorsFormat[5]                           = RED_FORMAT_UNDEFINED;
  outputs.colorsFormat[6]                           = RED_FORMAT_UNDEFINED;
  outputs.colorsFormat[7]                           = RED_FORMAT_UNDEFINED;
  outputs.colorsMultisampleCount[0]                 = state->state_multisample_count;
  outputs.colorsMultisampleCount[1]                 = RED_MULTISAMPLE_COUNT_BITFLAG_1;
  outputs.colorsMultisampleCount[2]                 = RED_MULTISAMPLE_COUNT_BITFLAG_1;
  outputs.colorsMultisampleCount[3]                 = RED_MULTISAMPLE_COUNT_BITFLAG_1;
  outputs.colorsMultisampleCount[4]                 = RED_MULTISAMPLE_COUNT_BITFLAG_1;
  outputs.colorsMultisampleCount[5]                 = RED_MULTISAMPLE_COUNT_BITFLAG_1;
  outputs.colorsMultisampleCount[6]                 = RED_MULTISAMPLE_COUNT_BITFLAG_1;
  outputs.colorsMultisampleCount[7]                 = RED_MULTISAMPLE_COUNT_BITFLAG_1;
  outputs.colorsSetProcedureOutputOp[0]             = RED_SET_PROCEDURE_OUTPUT_OP_PRESERVE;
  outputs.colorsSetProcedureOutputOp[1]             = RED_SET_PROCEDURE_OUTPUT_OP_PRESERVE;
  outputs.colorsSetProcedureOutputOp[2]             = RED_SET_PROCEDURE_OUTPUT_OP_PRESERVE;
  outputs.colorsSetProcedureOutputOp[3]             = RED_SET_PROCEDURE_OUTPUT_OP_PRESERVE;
  outputs.colorsSetProcedureOutputOp[4]             = RED_SET_PROCEDURE_OUTPUT_OP_PRESERVE;
  outputs.colorsSetProcedureOutputOp[5]             = RED_SET_PROCEDURE_OUTPUT_OP_PRESERVE;
  outputs.colorsSetProcedureOutputOp[6]             = RED_SET_PROCEDURE_OUTPUT_OP_PRESERVE;
  outputs.colorsSetProcedureOutputOp[7]             = RED_SET_PROCEDURE_OUTPUT_OP_PRESERVE;
  outputs.colorsEndProcedureOutputOp[0]             = RED_END_PROCEDURE_OUTPUT_OP_PRESERVE;
  outputs.colorsEndProcedureOutputOp[1]             = RED_END_PROCEDURE_OUTPUT_OP_PRESERVE;
  outputs.colorsEndProcedureOutputOp[2]             = RED_END_PROCEDURE_OUTPUT_OP_PRESERVE;
  outputs.colorsEndProcedureOutputOp[3]             = RED_END_PROCEDURE_OUTPUT_OP_PRESERVE;
  outputs.colorsEndProcedureOutputOp[4]             = RED_END_PROCEDURE_OUTPUT_OP_PRESERVE;
  outputs.colorsEndProcedureOutputOp[5]             = RED_END_PROCEDURE_OUTPUT_OP_PRESERVE;
  outputs.colorsEndProcedureOutputOp[6]             = RED_END_PROCEDURE_OUTPUT_OP_PRESERVE;
  outputs.colorsEndProcedureOutputOp[7]             = RED_END_PROCEDURE_OUTPUT_OP_PRESERVE;
  outputs.colorsSharesMemoryWithAnotherMember[0]    = 0;
  outputs.colorsSharesMemoryWithAnotherMember[1]    = 0;
  outputs.colorsSharesMemoryWithAnotherMember[2]    = 0;
  outputs.colorsSharesMemoryWithAnotherMember[3]    = 0;
  outputs.colorsSharesMemoryWithAnotherMember[4]    = 0;
  outputs.colorsSharesMemoryWithAnotherMember[5]    = 0;
  outputs.colorsSharesMemoryWithAnotherMember[6]    = 0;
  outputs.colorsSharesMemoryWithAnotherMember[7]    = 0;

  RedProcedureState procstate = {0};
  procstate.inputAssemblyTopology                          = RED_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
  procstate.inputAssemblyPrimitiveRestartEnable            = 0;
  procstate.viewportDynamic                                = 1;
  procstate.viewportStaticX                                = 0;
  procstate.viewportStaticY                                = 0;
  procstate.viewportStaticWidth                            = 0;
  procstate.viewportStaticHeight                           = 0;
  procstate.viewportStaticDepthMin                         = 0;
  procstate.viewportStaticDepthMax                         = 0;
  procstate.scissorDynamic                                 = 1;
  procstate.scissorStaticX                                 = 0;
  procstate.scissorStaticY                                 = 0;
  procstate.scissorStaticWidth                             = 0;
  procstate.scissorStaticHeight                            = 0;
  procstate.rasterizationDepthClampEnable                  = state->state->rasterizationDepthClampEnable;
  procstate.rasterizationDiscardAllPrimitivesEnable        = 0;
  procstate.rasterizationCullMode                          = ReiiCullModeToRed(state->state->rasterizationCullMode);
  procstate.rasterizationFrontFace                         = ReiiFrontFaceToRed(state->state->rasterizationFrontFace);
  procstate.rasterizationDepthBiasEnable                   = state->state->rasterizationDepthBiasEnable;
  procstate.rasterizationDepthBiasDynamic                  = 0;
  procstate.rasterizationDepthBiasStaticConstantFactor     = state->state->rasterizationDepthBiasConstantFactor;
  procstate.rasterizationDepthBiasStaticClamp              = 0;
  procstate.rasterizationDepthBiasStaticSlopeFactor        = state->state->rasterizationDepthBiasSlopeFactor;
  procstate.multisampleCount                               = state->state_multisample_count;
  procstate.multisampleSampleMask                          = NULL;
  procstate.multisampleSampleShadingEnable                 = 0;
  procstate.multisampleSampleShadingMin                    = 0;
  procstate.multisampleAlphaToCoverageEnable               = state->state->multisampleAlphaToCoverageEnable;
  procstate.multisampleAlphaToOneEnable                    = state->state->multisampleAlphaToOneEnable;
  procstate.depthTestEnable                                = state->state->depthTestEnable;
  procstate.depthTestDepthWriteEnable                      = state->state->depthTestDepthWriteEnable;
  procstate.depthTestDepthCompareOp                        = ReiiCompareOpToRed(state->state->depthTestDepthCompareOp);
  procstate.depthTestBoundsTestEnable                      = 0;
  procstate.depthTestBoundsTestDynamic                     = 0;
  procstate.depthTestBoundsTestStaticMin                   = 0;
  procstate.depthTestBoundsTestStaticMax                   = 0;
  procstate.stencilTestEnable                              = state->state->stencilTestEnable;
  procstate.stencilTestFrontAndBackDynamicCompareMask      = 0;
  procstate.stencilTestFrontAndBackDynamicWriteMask        = 0;
  procstate.stencilTestFrontAndBackDynamicReference        = 0;
  procstate.stencilTestFrontStencilTestFailOp              = ReiiStencilOpToRed(state->state->stencilTestFrontStencilTestFailOp);
  procstate.stencilTestFrontStencilTestPassDepthTestPassOp = ReiiStencilOpToRed(state->state->stencilTestFrontStencilTestPassDepthTestPassOp);
  procstate.stencilTestFrontStencilTestPassDepthTestFailOp = ReiiStencilOpToRed(state->state->stencilTestFrontStencilTestPassDepthTestFailOp);
  procstate.stencilTestFrontCompareOp                      = ReiiCompareOpToRed(state->state->stencilTestFrontCompareOp);
  procstate.stencilTestBackStencilTestFailOp               = ReiiStencilOpToRed(state->state->stencilTestBackStencilTestFailOp);
  procstate.stencilTestBackStencilTestPassDepthTestPassOp  = ReiiStencilOpToRed(state->state->stencilTestBackStencilTestPassDepthTestPassOp);
  procstate.stencilTestBackStencilTestPassDepthTestFailOp  = ReiiStencilOpToRed(state->state->stencilTestBackStencilTestPassDepthTestFailOp);
  procstate.stencilTestBackCompareOp                       = ReiiCompareOpToRed(state->state->stencilTestBackCompareOp);
  procstate.stencilTestFrontAndBackDynamicCompareMask      = 0;
  procstate.stencilTestFrontAndBackDynamicWriteMask        = 0;
  procstate.stencilTestFrontAndBackDynamicReference        = 0;
  procstate.stencilTestFrontAndBackStaticCompareMask       = state->state->stencilTestFrontAndBackCompareMask;
  procstate.stencilTestFrontAndBackStaticWriteMask         = state->state->stencilTestFrontAndBackWriteMask;
  procstate.stencilTestFrontAndBackStaticReference         = state->state->stencilTestFrontAndBackReference;
  procstate.blendLogicOpEnable                             = state->state->blendLogicOpEnable;
  procstate.blendLogicOp                                   = ReiiLogicOpToRed(state->state->blendLogicOp);
  procstate.blendConstantsDynamic                          = 0;
  procstate.blendConstantsStatic[0]                        = state->state->blendConstants[0];
  procstate.blendConstantsStatic[1]                        = state->state->blendConstants[1];
  procstate.blendConstantsStatic[2]                        = state->state->blendConstants[2];
  procstate.blendConstantsStatic[3]                        = state->state->blendConstants[3];
  procstate.outputColorsCount                              = 1;
  procstate.outputColorsWriteMask[0]                       = (state->state->outputColorWriteEnableR ? RED_COLOR_COMPONENT_BITFLAG_R : 0) | (state->state->outputColorWriteEnableG ? RED_COLOR_COMPONENT_BITFLAG_G : 0) | (state->state->outputColorWriteEnableB ? RED_COLOR_COMPONENT_BITFLAG_B : 0) | (state->state->outputColorWriteEnableA ? RED_COLOR_COMPONENT_BITFLAG_A : 0);
  procstate.outputColorsWriteMask[1]                       = 0;
  procstate.outputColorsWriteMask[2]                       = 0;
  procstate.outputColorsWriteMask[3]                       = 0;
  procstate.outputColorsWriteMask[4]                       = 0;
  procstate.outputColorsWriteMask[5]                       = 0;
  procstate.outputColorsWriteMask[6]                       = 0;
  procstate.outputColorsWriteMask[7]                       = 0;
  procstate.outputColorsBlendEnable[0]                     = state->state->outputColorBlendEnable;
  procstate.outputColorsBlendEnable[1]                     = 0;
  procstate.outputColorsBlendEnable[2]                     = 0;
  procstate.outputColorsBlendEnable[3]                     = 0;
  procstate.outputColorsBlendEnable[4]                     = 0;
  procstate.outputColorsBlendEnable[5]                     = 0;
  procstate.outputColorsBlendEnable[6]                     = 0;
  procstate.outputColorsBlendEnable[7]                     = 0;
  procstate.outputColorsBlendColorFactorSource[0]          = ReiiBlendFactorToRed(state->state->outputColorBlendColorFactorSource);
  procstate.outputColorsBlendColorFactorSource[1]          = RED_BLEND_FACTOR_ZERO;
  procstate.outputColorsBlendColorFactorSource[2]          = RED_BLEND_FACTOR_ZERO;
  procstate.outputColorsBlendColorFactorSource[3]          = RED_BLEND_FACTOR_ZERO;
  procstate.outputColorsBlendColorFactorSource[4]          = RED_BLEND_FACTOR_ZERO;
  procstate.outputColorsBlendColorFactorSource[5]          = RED_BLEND_FACTOR_ZERO;
  procstate.outputColorsBlendColorFactorSource[6]          = RED_BLEND_FACTOR_ZERO;
  procstate.outputColorsBlendColorFactorSource[7]          = RED_BLEND_FACTOR_ZERO;
  procstate.outputColorsBlendColorFactorTarget[0]          = ReiiBlendFactorToRed(state->state->outputColorBlendColorFactorTarget);
  procstate.outputColorsBlendColorFactorTarget[1]          = RED_BLEND_FACTOR_ZERO;
  procstate.outputColorsBlendColorFactorTarget[2]          = RED_BLEND_FACTOR_ZERO;
  procstate.outputColorsBlendColorFactorTarget[3]          = RED_BLEND_FACTOR_ZERO;
  procstate.outputColorsBlendColorFactorTarget[4]          = RED_BLEND_FACTOR_ZERO;
  procstate.outputColorsBlendColorFactorTarget[5]          = RED_BLEND_FACTOR_ZERO;
  procstate.outputColorsBlendColorFactorTarget[6]          = RED_BLEND_FACTOR_ZERO;
  procstate.outputColorsBlendColorFactorTarget[7]          = RED_BLEND_FACTOR_ZERO;
  procstate.outputColorsBlendColorOp[0]                    = ReiiBlendOpToRed(state->state->outputColorBlendColorOp);
  procstate.outputColorsBlendColorOp[1]                    = RED_BLEND_OP_ADD;
  procstate.outputColorsBlendColorOp[2]                    = RED_BLEND_OP_ADD;
  procstate.outputColorsBlendColorOp[3]                    = RED_BLEND_OP_ADD;
  procstate.outputColorsBlendColorOp[4]                    = RED_BLEND_OP_ADD;
  procstate.outputColorsBlendColorOp[5]                    = RED_BLEND_OP_ADD;
  procstate.outputColorsBlendColorOp[6]                    = RED_BLEND_OP_ADD;
  procstate.outputColorsBlendColorOp[7]                    = RED_BLEND_OP_ADD;
  procstate.outputColorsBlendAlphaFactorSource[0]          = ReiiBlendFactorToRed(state->state->outputColorBlendAlphaFactorSource);
  procstate.outputColorsBlendAlphaFactorSource[1]          = RED_BLEND_FACTOR_ZERO;
  procstate.outputColorsBlendAlphaFactorSource[2]          = RED_BLEND_FACTOR_ZERO;
  procstate.outputColorsBlendAlphaFactorSource[3]          = RED_BLEND_FACTOR_ZERO;
  procstate.outputColorsBlendAlphaFactorSource[4]          = RED_BLEND_FACTOR_ZERO;
  procstate.outputColorsBlendAlphaFactorSource[5]          = RED_BLEND_FACTOR_ZERO;
  procstate.outputColorsBlendAlphaFactorSource[6]          = RED_BLEND_FACTOR_ZERO;
  procstate.outputColorsBlendAlphaFactorSource[7]          = RED_BLEND_FACTOR_ZERO;
  procstate.outputColorsBlendAlphaFactorTarget[0]          = ReiiBlendFactorToRed(state->state->outputColorBlendAlphaFactorTarget);
  procstate.outputColorsBlendAlphaFactorTarget[1]          = RED_BLEND_FACTOR_ZERO;
  procstate.outputColorsBlendAlphaFactorTarget[2]          = RED_BLEND_FACTOR_ZERO;
  procstate.outputColorsBlendAlphaFactorTarget[3]          = RED_BLEND_FACTOR_ZERO;
  procstate.outputColorsBlendAlphaFactorTarget[4]          = RED_BLEND_FACTOR_ZERO;
  procstate.outputColorsBlendAlphaFactorTarget[5]          = RED_BLEND_FACTOR_ZERO;
  procstate.outputColorsBlendAlphaFactorTarget[6]          = RED_BLEND_FACTOR_ZERO;
  procstate.outputColorsBlendAlphaFactorTarget[7]          = RED_BLEND_FACTOR_ZERO;
  procstate.outputColorsBlendAlphaOp[0]                    = ReiiBlendOpToRed(state->state->outputColorBlendAlphaOp);
  procstate.outputColorsBlendAlphaOp[1]                    = RED_BLEND_OP_ADD;
  procstate.outputColorsBlendAlphaOp[2]                    = RED_BLEND_OP_ADD;
  procstate.outputColorsBlendAlphaOp[3]                    = RED_BLEND_OP_ADD;
  procstate.outputColorsBlendAlphaOp[4]                    = RED_BLEND_OP_ADD;
  procstate.outputColorsBlendAlphaOp[5]                    = RED_BLEND_OP_ADD;
  procstate.outputColorsBlendAlphaOp[6]                    = RED_BLEND_OP_ADD;
  procstate.outputColorsBlendAlphaOp[7]                    = RED_BLEND_OP_ADD;

  // To destroy
  RedHandleProcedure procedure = NULL;
  np(red2CreateProcedure,
    "context", vkfast->context,
    "gpu", vkfast->gpu,
    "handleName", state->optional_debug_name,
    "procedureCache", NULL,
    "outputDeclarationMembers", &outputs,
    "outputDeclarationMembersResolveSources", NULL,
    "dependencyByRegion", 0,
    "dependencyByRegionAllowUsageAliasOrderBarriers", 0,
    "procedureParameters", procedureParameters.procedureParameters,
    "gpuCodeVertexMainProcedureName", "main",
    "gpuCodeVertex", gpuCodeVertex,
    "gpuCodeFragmentMainProcedureName", "main",
    "gpuCodeFragment", gpuCodeFragment,
    "state", &procstate,
    "stateExtension", NULL,
    "deriveBase", 0,
    "deriveFrom", NULL,
    "outProcedure", &procedure,
    "outStatuses", NULL,
    "optionalFile", optionalFile,
    "optionalLine", optionalLine,
    "optionalUserData", NULL
  );
  REDGPU_2_EXPECTWG(procedure != NULL);

  // Filling
  state->state->gpuCodeVertex       = gpuCodeVertex;
  state->state->gpuCodeFragment     = gpuCodeFragment;
  state->state->procedureParameters = procedureParameters;
  state->state->procedure           = procedure;
}

GPU_API_PRE void GPU_API_POST reiiCreateTextureMemory(gpu_handle_context_t context, gpu_extra_reii_texture_type textureType, uint64_t bytesCount, ReiiHandleTextureMemory * outTextureMemory) {
  const char * optionalFile = NULL;
  int optionalLine = 0;

  vf_handle_context_t * vkfast = (vf_handle_context_t *)(void *)context;
  
  RedHandleGpu gpu = vkfast->gpu;

  const char * optional_debug_name = outTextureMemory->optional_debug_name;
  ReiiHandleTextureMemory clear = {0};
  outTextureMemory[0] = clear;

  RedBool32 texturesUseTheirOwnDedicatedMemory = 0;
  RedHandleMemory memory = NULL;

  if (textureType == GPU_EXTRA_REII_TEXTURE_TYPE_GENERAL) {
    // To destroy
    np(redMemoryAllocate,
      "context", vkfast->context,
      "gpu", vkfast->gpu,
      "handleName", optional_debug_name,
      "bytesCount", bytesCount,
      "memoryTypeIndex", vkfast->specificMemoryTypesGpuVram,
      "dedicateToArray", NULL,
      "dedicateToImage", NULL,
      "memoryBitflags", 0,
      "outMemory", &memory,
      "outStatuses", NULL,
      "optionalFile", optionalFile,
      "optionalLine", optionalLine,
      "optionalUserData", NULL
    );
    REDGPU_2_EXPECT(memory != NULL);
    texturesUseTheirOwnDedicatedMemory = 0;
  } else {
    // NOTE(Constantine): For now, depth stencil and color textures always use their own dedicated memory handles.
    memory = NULL;
    texturesUseTheirOwnDedicatedMemory = 1;
  }

  // Filling
  outTextureMemory->texturesType = textureType;
  outTextureMemory->texturesUseTheirOwnDedicatedMemory = texturesUseTheirOwnDedicatedMemory;
  outTextureMemory->bytesCount   = bytesCount;
  outTextureMemory->memory       = memory;
  outTextureMemory->bytesOffset  = 0;
}

GPU_API_PRE void GPU_API_POST reiiCreateTextureFromTextureMemory(gpu_handle_context_t context, ReiiHandleTextureMemory * textureMemory, ReiiTextureBinding binding, ReiiHandleTexture * outTexture) {
  const char * optionalFile = NULL;
  int optionalLine = 0;

  vf_handle_context_t * vkfast = (vf_handle_context_t *)(void *)context;
  
  RedHandleGpu gpu = vkfast->gpu;

  const char * optional_debug_name = outTexture->optional_debug_name;
  ReiiHandleTexture clear = {0};
  outTexture[0] = clear;

  REDGPU_2_EXPECTWG(textureMemory != NULL);
  REDGPU_2_EXPECTWG(textureMemory->texturesType != GPU_EXTRA_REII_TEXTURE_TYPE_INVALID);
  REDGPU_2_EXPECTWG(binding != REII_TEXTURE_BINDING_CUBE_POSITIVE_X);
  REDGPU_2_EXPECTWG(binding != REII_TEXTURE_BINDING_CUBE_POSITIVE_Y);
  REDGPU_2_EXPECTWG(binding != REII_TEXTURE_BINDING_CUBE_POSITIVE_Z);
  REDGPU_2_EXPECTWG(binding != REII_TEXTURE_BINDING_CUBE_NEGATIVE_X);
  REDGPU_2_EXPECTWG(binding != REII_TEXTURE_BINDING_CUBE_NEGATIVE_Y);
  REDGPU_2_EXPECTWG(binding != REII_TEXTURE_BINDING_CUBE_NEGATIVE_Z);

  if (binding == REII_TEXTURE_BINDING_CUBE && textureMemory->texturesType != GPU_EXTRA_REII_TEXTURE_TYPE_GENERAL) {
    REDGPU_2_EXPECTWG(0 || !"Can't suballocate a Cube texture from a non-General texture memory.");
  }

  RedImage imageClear = {0};

  // Filling
  outTexture->optional_debug_name  = optional_debug_name;
  outTexture->textureMemory        = textureMemory;
  outTexture->binding              = binding;
  outTexture->generateMipLevels    = 1;
  outTexture->mipLevelsCount       = 0;
  outTexture->width                = 0;
  outTexture->height               = 0;
  outTexture->format               = RED_FORMAT_UNDEFINED;
  outTexture->msaaCount            = RED_MULTISAMPLE_COUNT_BITFLAG_1;
  outTexture->sampler              = NULL;
  outTexture->image                = imageClear;
  outTexture->imageDedicatedMemory = NULL;
  outTexture->texture              = NULL;
  outTexture->textureDepthOnly     = NULL;
  outTexture->textureStencilOnly   = NULL;
  outTexture->textureCubeFace[0]   = NULL;
  outTexture->textureCubeFace[1]   = NULL;
  outTexture->textureCubeFace[2]   = NULL;
  outTexture->textureCubeFace[3]   = NULL;
  outTexture->textureCubeFace[4]   = NULL;
  outTexture->textureCubeFace[5]   = NULL;
}

GPU_API_PRE void GPU_API_POST reiiTextureSetStateMipmap(gpu_handle_context_t context, ReiiTextureBinding binding, ReiiHandleTexture * bindingTexture, ReiiBool32 generateMipLevels) {
  const char * optionalFile = NULL;
  int optionalLine = 0;

  vf_handle_context_t * vkfast = (vf_handle_context_t *)(void *)context;

  RedHandleGpu gpu = vkfast->gpu;

  REDGPU_2_EXPECTWG(binding == bindingTexture->binding);

  bindingTexture->generateMipLevels = generateMipLevels;
}

GPU_API_PRE void GPU_API_POST reiiTextureSetStateMipmapLevelsCount(gpu_handle_context_t context, ReiiTextureBinding binding, ReiiHandleTexture * bindingTexture, int mipLevelsCount) {
  const char * optionalFile = NULL;
  int optionalLine = 0;

  vf_handle_context_t * vkfast = (vf_handle_context_t *)(void *)context;

  RedHandleGpu gpu = vkfast->gpu;

  REDGPU_2_EXPECTWG(binding == bindingTexture->binding);

  bindingTexture->mipLevelsCount = mipLevelsCount;
}

GPU_API_PRE void GPU_API_POST reiiTextureSetStateMsaa(gpu_handle_context_t context, ReiiTextureBinding binding, ReiiHandleTexture * bindingTexture, RedMultisampleCountBitflag msaaCount) {
  const char * optionalFile = NULL;
  int optionalLine = 0;

  vf_handle_context_t * vkfast = (vf_handle_context_t *)(void *)context;

  RedHandleGpu gpu = vkfast->gpu;

  bindingTexture->msaaCount = msaaCount;
}

GPU_API_PRE void GPU_API_POST reiiTextureSetStateSampler(gpu_handle_context_t context, ReiiTextureBinding binding, ReiiHandleTexture * bindingTexture, ReiiSamplerFiltering magFiltering, ReiiSamplerFiltering minFiltering, ReiiSamplerBehaviorOutsideTextureCoordinate behaviorOutsideTextureCoordinateU, ReiiSamplerBehaviorOutsideTextureCoordinate behaviorOutsideTextureCoordinateV, int maxAnisotropy) {
  const char * optionalFile = NULL;
  int optionalLine = 0;

  vf_handle_context_t * vkfast = (vf_handle_context_t *)(void *)context;

  RedHandleGpu gpu = vkfast->gpu;

  REDGPU_2_EXPECTWG(binding == bindingTexture->binding);
  REDGPU_2_EXPECTWG(magFiltering == REII_SAMPLER_FILTERING_NEAREST || magFiltering == REII_SAMPLER_FILTERING_LINEAR);
  REDGPU_2_EXPECTWG(minFiltering == REII_SAMPLER_FILTERING_NEAREST || minFiltering == REII_SAMPLER_FILTERING_LINEAR ||
    minFiltering == REII_SAMPLER_FILTERING_NEAREST_MIP_NEAREST || minFiltering == REII_SAMPLER_FILTERING_NEAREST_MIP_LINEAR ||
    minFiltering == REII_SAMPLER_FILTERING_LINEAR_MIP_NEAREST  || minFiltering == REII_SAMPLER_FILTERING_LINEAR_MIP_LINEAR
  );

  RedHandleSampler sampler = bindingTexture->sampler;
  np(red2DestroyHandle,
    "context", vkfast->context,
    "gpu", vkfast->gpu,
    "handleType", RED_HANDLE_TYPE_SAMPLER,
    "handle", sampler,
    "optionalHandle2", NULL,
    "optionalFile", optionalFile,
    "optionalLine", optionalLine,
    "optionalUserData", NULL
  );
  sampler = NULL;

  RedSamplerFiltering    filteringMag = RiiSamplerFilteringToRed(magFiltering);
  RedSamplerFiltering    filteringMin = RiiSamplerFilteringToRed(minFiltering);
  RedSamplerFilteringMip filteringMip = RiiSamplerFilteringMipToRed(minFiltering);

  // To destroy
  np(redCreateSampler,
    "context", vkfast->context,
    "gpu", vkfast->gpu,
    "handleName", bindingTexture->optional_debug_name,
    "filteringMag", filteringMag,
    "filteringMin", filteringMin,
    "filteringMip", filteringMip,
    "behaviorOutsideTextureCoordinateU", ReiiSamplerBehaviorOutsideTextureCoordinateToRed(behaviorOutsideTextureCoordinateU),
    "behaviorOutsideTextureCoordinateV", ReiiSamplerBehaviorOutsideTextureCoordinateToRed(behaviorOutsideTextureCoordinateV),
    "behaviorOutsideTextureCoordinateW", RED_SAMPLER_BEHAVIOR_OUTSIDE_TEXTURE_COORDINATE_REPEAT, // NOTE(Constantine): We don't use these samplers on 3D textures, so whatever.
    "mipLodBias", 0.f,
    "enableAnisotropy", maxAnisotropy > 1,
    "maxAnisotropy", maxAnisotropy,
    "enableCompare", 0,
    "compareOp", RED_COMPARE_OP_NEVER,
    "minLod",-1000.f,
    "maxLod", 1000.f,
    "outSampler", &sampler,
    "outStatuses", NULL,
    "optionalFile", optionalFile,
    "optionalLine", optionalLine,
    "optionalUserData", NULL
  );
  REDGPU_2_EXPECTWG(sampler != NULL);

  bindingTexture->sampler = sampler;
}

static void redHelperImageSetStateUsable(RedContext context, RedHandleGpu gpu, RedHandleImage image, RedImagePartBitflags allParts, RedHandleQueue queueToSubmitImageStateChange, unsigned queueFamilyIndexToSubmitImageStateChange, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData) {
  RedCallProceduresAndAddresses callProceduresAndAddresses = {0};
  np(redGetCallProceduresAndAddresses,
    "context", context,
    "gpu", gpu,
    "outCallProceduresAndAddresses", &callProceduresAndAddresses,
    "outStatuses", outStatuses,
    "optionalFile", optionalFile,
    "optionalLine", optionalLine,
    "optionalUserData", optionalUserData
  );

  // To destroy
  RedCalls calls = {0};
  np(redCreateCalls,
    "context", context,
    "gpu", gpu,
    "handleName", NULL,
    "queueFamilyIndex", queueFamilyIndexToSubmitImageStateChange,
    "outCalls", &calls,
    "outStatuses", outStatuses,
    "optionalFile", optionalFile,
    "optionalLine", optionalLine,
    "optionalUserData", optionalUserData
  );
  REDGPU_2_EXPECTWG(calls.handle != NULL);

  np(redCallsSet,
    "context", context,
    "gpu", gpu,
    "calls", calls.handle,
    "callsMemory", calls.memory,
    "callsReusable", calls.reusable,
    "outStatuses", outStatuses,
    "optionalFile", optionalFile,
    "optionalLine", optionalLine,
    "optionalUserData", optionalUserData
  );

  RedUsageImage imageUsage = {0};
  imageUsage.barrierSplit           = RED_BARRIER_SPLIT_NONE;
  imageUsage.oldAccessStages        = 0;
  imageUsage.newAccessStages        = 0;
  imageUsage.oldAccess              = 0;
  imageUsage.newAccess              = 0;
  imageUsage.oldState               = RED_STATE_UNUSABLE;
  imageUsage.newState               = RED_STATE_USABLE;
  imageUsage.queueFamilyIndexSource = -1;
  imageUsage.queueFamilyIndexTarget = -1;
  imageUsage.image                  = image;
  imageUsage.imageAllParts          = allParts;
  imageUsage.imageLevelsFirst       = 0;
  imageUsage.imageLevelsCount       = -1;
  imageUsage.imageLayersFirst       = 0;
  imageUsage.imageLayersCount       = -1;
  np(redCallUsageAliasOrderBarrier,
    "address", callProceduresAndAddresses.redCallUsageAliasOrderBarrier,
    "calls", calls.handle,
    "context", context,
    "arrayUsagesCount", 0,
    "arrayUsages", NULL,
    "imageUsagesCount", 1,
    "imageUsages", &imageUsage,
    "aliasesCount", 0,
    "aliases", NULL,
    "ordersCount", 0,
    "orders", NULL,
    "dependencyByRegion", 0
  );

  np(redCallsEnd,
    "context", context,
    "gpu", gpu,
    "calls", calls.handle,
    "callsMemory", calls.memory,
    "outStatuses", outStatuses,
    "optionalFile", optionalFile,
    "optionalLine", optionalLine,
    "optionalUserData", optionalUserData
  );

  // To destroy
  RedHandleCpuSignal cpuSignal = NULL;
  np(redCreateCpuSignal,
    "context", context,
    "gpu", gpu,
    "handleName", NULL,
    "createSignaled", 0,
    "outCpuSignal", &cpuSignal,
    "outStatuses", outStatuses,
    "optionalFile", optionalFile,
    "optionalLine", optionalLine,
    "optionalUserData", optionalUserData
  );
  REDGPU_2_EXPECTWG(cpuSignal != NULL);

  RedGpuTimeline timeline = {0};
  timeline.setTo4                            = 4;
  timeline.setTo0                            = 0;
  timeline.waitForAndUnsignalGpuSignalsCount = 0;
  timeline.waitForAndUnsignalGpuSignals      = NULL;
  timeline.setTo65536                        = NULL;
  timeline.callsCount                        = 1;
  timeline.calls                             = &calls.handle;
  timeline.signalGpuSignalsCount             = 0;
  timeline.signalGpuSignals                  = NULL;
  np(redQueueSubmit,
    "context", context,
    "gpu", gpu,
    "queue", queueToSubmitImageStateChange,
    "timelinesCount", 1,
    "timelines", &timeline,
    "signalCpuSignal", cpuSignal,
    "outStatuses", outStatuses,
    "optionalFile", optionalFile,
    "optionalLine", optionalLine,
    "optionalUserData", optionalUserData
  );

  np(redCpuSignalWait,
    "context", context,
    "gpu", gpu,
    "cpuSignalsCount", 1,
    "cpuSignals", &cpuSignal,
    "waitAll", 1,
    "outStatuses", outStatuses,
    "optionalFile", optionalFile,
    "optionalLine", optionalLine,
    "optionalUserData", optionalUserData
  );

  np(redDestroyCpuSignal,
    "context", context,
    "gpu", gpu,
    "cpuSignal", cpuSignal,
    "optionalFile", optionalFile,
    "optionalLine", optionalLine,
    "optionalUserData", optionalUserData
  );

  np(redDestroyCalls,
    "context", context,
    "gpu", gpu,
    "calls", calls.handle,
    "callsMemory", calls.memory,
    "optionalFile", optionalFile,
    "optionalLine", optionalLine,
    "optionalUserData", optionalUserData
  );
}

GPU_API_PRE void GPU_API_POST reiiTextureDefineAndCopyFromCpu(gpu_handle_context_t context, ReiiTextureBinding binding, ReiiHandleTexture * bindingTexture, int bindingLevel, ReiiTextureTexelFormat bindingTexelFormat, int width, int height, ReiiTextureTexelFormat texelsFormat, ReiiTextureTexelType texelsType, int texelsBytesAlignment, const ReiiCpuGpuTexture * texels) {
  const char * optionalFile = NULL;
  int optionalLine = 0;

  vf_handle_context_t * vkfast = (vf_handle_context_t *)(void *)context;

  RedHandleGpu gpu = vkfast->gpu;

  const gpu_extra_reii_texture_type textureType = bindingTexture->textureMemory->texturesType;

  REDGPU_2_EXPECTWG(binding != REII_TEXTURE_BINDING_CUBE);
  if (bindingTexture->binding == REII_TEXTURE_BINDING_2D) {
    REDGPU_2_EXPECTWG(binding == REII_TEXTURE_BINDING_2D);
  }
  int w_power = 1;
  while(w_power < width) {
    w_power *= 2;
  }
  int h_power = 1;
  while(h_power < height) {
    h_power *= 2;
  }
  REDGPU_2_EXPECTWG(texelsBytesAlignment == 4);
  if (textureType == GPU_EXTRA_REII_TEXTURE_TYPE_GENERAL) {
    REDGPU_2_EXPECTWG(width  == w_power);
    REDGPU_2_EXPECTWG(height == h_power);
    REDGPU_2_EXPECTWG(width  == height);
  }
  REDGPU_2_EXPECTWG(bindingTexture->generateMipLevels == 0 || !"TODO(Constantine): Generating mipmaps is not supported right now, use extra/ad_mipmap to generate and upload mip levels manually.");
  REDGPU_2_EXPECTWG(bindingTexture->mipLevelsCount >= 1);
  REDGPU_2_EXPECTWG(bindingTexture->mipLevelsCount <= ((int)log2(width) + 1));
  REDGPU_2_EXPECTWG(bindingTexture->sampler != NULL);
  if (textureType != GPU_EXTRA_REII_TEXTURE_TYPE_GENERAL) {
    REDGPU_2_EXPECTWG(texels == NULL);
  }

  RedFormat  format = RED_FORMAT_UNDEFINED;
  ReiiBool32 formatHasStencil = 0;
  // NOTE(Constantine): Check for the only supported texture formats for now.
  if (textureType == GPU_EXTRA_REII_TEXTURE_TYPE_GENERAL) {
    REDGPU_2_EXPECTWG(bindingTexelFormat == REII_TEXTURE_TEXEL_FORMAT_RGBA);
    REDGPU_2_EXPECTWG(texelsFormat       == REII_TEXTURE_TEXEL_FORMAT_RGBA);
    REDGPU_2_EXPECTWG(texelsType         == REII_TEXTURE_TEXEL_TYPE_U8);
    format = RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1_GAMMA_CORRECTED;
  } else if (textureType == GPU_EXTRA_REII_TEXTURE_TYPE_OUTPUT_COLOR || textureType == GPU_EXTRA_REII_TEXTURE_TYPE_OUTPUT_COLOR_MSAA) {
    REDGPU_2_EXPECTWG(bindingTexelFormat == REII_TEXTURE_TEXEL_FORMAT_RGBA);
    REDGPU_2_EXPECTWG(texelsFormat       == REII_TEXTURE_TEXEL_FORMAT_RGBA);
    REDGPU_2_EXPECTWG(texelsType         == REII_TEXTURE_TEXEL_TYPE_U8);
    format = RED_FORMAT_PRESENT_BGRA_8_8_8_8_UINT_TO_FLOAT_0_1;
  } else if (textureType == GPU_EXTRA_REII_TEXTURE_TYPE_OUTPUT_DEPTH_STENCIL || textureType == GPU_EXTRA_REII_TEXTURE_TYPE_OUTPUT_DEPTH_STENCIL_MSAA) {
    REDGPU_2_EXPECTWG(bindingTexelFormat == REII_TEXTURE_TEXEL_FORMAT_DS);
    REDGPU_2_EXPECTWG(texelsFormat       == REII_TEXTURE_TEXEL_FORMAT_DS);
    REDGPU_2_EXPECTWG(texelsType         == REII_TEXTURE_TEXEL_TYPE_FLOAT || texelsType == REII_TEXTURE_TEXEL_TYPE_U24_U8);
    if (REII_TEXTURE_TEXEL_TYPE_FLOAT) {
      format = RED_FORMAT_DEPTH_32_FLOAT;
      formatHasStencil = 0;
    } else {
      format = RED_FORMAT_DEPTH_24_UINT_TO_FLOAT_0_1_STENCIL_8_UINT;
      formatHasStencil = 1;
    }
  } else {
    REDGPU_2_EXPECTWG(
      !"Unsupported texture memory type."
    );
  }

  RedImage         image                = bindingTexture->image;
  RedHandleMemory  imageDedicatedMemory = bindingTexture->imageDedicatedMemory;
  RedHandleTexture texture              = bindingTexture->texture;
  RedHandleTexture textureDepthOnly     = bindingTexture->textureDepthOnly;
  RedHandleTexture textureStencilOnly   = bindingTexture->textureStencilOnly;
  RedHandleTexture textureCubeFace[6]   = {0};
  textureCubeFace[0] = bindingTexture->textureCubeFace[0];
  textureCubeFace[1] = bindingTexture->textureCubeFace[1];
  textureCubeFace[2] = bindingTexture->textureCubeFace[2];
  textureCubeFace[3] = bindingTexture->textureCubeFace[3];
  textureCubeFace[4] = bindingTexture->textureCubeFace[4];
  textureCubeFace[5] = bindingTexture->textureCubeFace[5];
  if (image.handle == NULL) {
    // To destroy
    np(redCreateImage,
      "context", vkfast->context,
      "gpu", vkfast->gpu,
      "handleName", bindingTexture->optional_debug_name,
      "dimensions", binding == REII_TEXTURE_BINDING_2D ? RED_IMAGE_DIMENSIONS_2D : RED_IMAGE_DIMENSIONS_2D_WITH_TEXTURE_DIMENSIONS_CUBE_AND_CUBE_LAYERED,
      "format", format,
      "width", width,
      "height", height,
      "depth", 1,
      "levelsCount", bindingTexture->mipLevelsCount,
      "layersCount", binding == REII_TEXTURE_BINDING_2D ? 1 : 6,
      "multisampleCount", bindingTexture->msaaCount,
      "restrictToAccess", 0,
      "initialQueueFamilyIndex", vkfast->mainQueueFamilyIndex,
      "dedicate", bindingTexture->textureMemory->texturesUseTheirOwnDedicatedMemory,
      "outImage", &image,
      "outStatuses", NULL,
      "optionalFile", optionalFile,
      "optionalLine", optionalLine,
      "optionalUserData", NULL
    );
    REDGPU_2_EXPECTWG(image.handle != NULL);

    if (bindingTexture->textureMemory->texturesUseTheirOwnDedicatedMemory == 1) {
      unsigned memoryTypeIsSupported[32];
      memoryTypeIsSupported[31] = (image.memoryTypesSupported & REDGPU_B32(1000,0000,0000,0000,0000,0000,0000,0000)) == 0 ? 0 : 1;
      memoryTypeIsSupported[30] = (image.memoryTypesSupported & REDGPU_B32(0100,0000,0000,0000,0000,0000,0000,0000)) == 0 ? 0 : 1;
      memoryTypeIsSupported[29] = (image.memoryTypesSupported & REDGPU_B32(0010,0000,0000,0000,0000,0000,0000,0000)) == 0 ? 0 : 1;
      memoryTypeIsSupported[28] = (image.memoryTypesSupported & REDGPU_B32(0001,0000,0000,0000,0000,0000,0000,0000)) == 0 ? 0 : 1;
      memoryTypeIsSupported[27] = (image.memoryTypesSupported & REDGPU_B32(0000,1000,0000,0000,0000,0000,0000,0000)) == 0 ? 0 : 1;
      memoryTypeIsSupported[26] = (image.memoryTypesSupported & REDGPU_B32(0000,0100,0000,0000,0000,0000,0000,0000)) == 0 ? 0 : 1;
      memoryTypeIsSupported[25] = (image.memoryTypesSupported & REDGPU_B32(0000,0010,0000,0000,0000,0000,0000,0000)) == 0 ? 0 : 1;
      memoryTypeIsSupported[24] = (image.memoryTypesSupported & REDGPU_B32(0000,0001,0000,0000,0000,0000,0000,0000)) == 0 ? 0 : 1;
      memoryTypeIsSupported[23] = (image.memoryTypesSupported & REDGPU_B32(0000,0000,1000,0000,0000,0000,0000,0000)) == 0 ? 0 : 1;
      memoryTypeIsSupported[22] = (image.memoryTypesSupported & REDGPU_B32(0000,0000,0100,0000,0000,0000,0000,0000)) == 0 ? 0 : 1;
      memoryTypeIsSupported[21] = (image.memoryTypesSupported & REDGPU_B32(0000,0000,0010,0000,0000,0000,0000,0000)) == 0 ? 0 : 1;
      memoryTypeIsSupported[20] = (image.memoryTypesSupported & REDGPU_B32(0000,0000,0001,0000,0000,0000,0000,0000)) == 0 ? 0 : 1;
      memoryTypeIsSupported[19] = (image.memoryTypesSupported & REDGPU_B32(0000,0000,0000,1000,0000,0000,0000,0000)) == 0 ? 0 : 1;
      memoryTypeIsSupported[18] = (image.memoryTypesSupported & REDGPU_B32(0000,0000,0000,0100,0000,0000,0000,0000)) == 0 ? 0 : 1;
      memoryTypeIsSupported[17] = (image.memoryTypesSupported & REDGPU_B32(0000,0000,0000,0010,0000,0000,0000,0000)) == 0 ? 0 : 1;
      memoryTypeIsSupported[16] = (image.memoryTypesSupported & REDGPU_B32(0000,0000,0000,0001,0000,0000,0000,0000)) == 0 ? 0 : 1;
      memoryTypeIsSupported[15] = (image.memoryTypesSupported & REDGPU_B32(0000,0000,0000,0000,1000,0000,0000,0000)) == 0 ? 0 : 1;
      memoryTypeIsSupported[14] = (image.memoryTypesSupported & REDGPU_B32(0000,0000,0000,0000,0100,0000,0000,0000)) == 0 ? 0 : 1;
      memoryTypeIsSupported[13] = (image.memoryTypesSupported & REDGPU_B32(0000,0000,0000,0000,0010,0000,0000,0000)) == 0 ? 0 : 1;
      memoryTypeIsSupported[12] = (image.memoryTypesSupported & REDGPU_B32(0000,0000,0000,0000,0001,0000,0000,0000)) == 0 ? 0 : 1;
      memoryTypeIsSupported[11] = (image.memoryTypesSupported & REDGPU_B32(0000,0000,0000,0000,0000,1000,0000,0000)) == 0 ? 0 : 1;
      memoryTypeIsSupported[10] = (image.memoryTypesSupported & REDGPU_B32(0000,0000,0000,0000,0000,0100,0000,0000)) == 0 ? 0 : 1;
      memoryTypeIsSupported[9]  = (image.memoryTypesSupported & REDGPU_B32(0000,0000,0000,0000,0000,0010,0000,0000)) == 0 ? 0 : 1;
      memoryTypeIsSupported[8]  = (image.memoryTypesSupported & REDGPU_B32(0000,0000,0000,0000,0000,0001,0000,0000)) == 0 ? 0 : 1;
      memoryTypeIsSupported[7]  = (image.memoryTypesSupported & REDGPU_B32(0000,0000,0000,0000,0000,0000,1000,0000)) == 0 ? 0 : 1;
      memoryTypeIsSupported[6]  = (image.memoryTypesSupported & REDGPU_B32(0000,0000,0000,0000,0000,0000,0100,0000)) == 0 ? 0 : 1;
      memoryTypeIsSupported[5]  = (image.memoryTypesSupported & REDGPU_B32(0000,0000,0000,0000,0000,0000,0010,0000)) == 0 ? 0 : 1;
      memoryTypeIsSupported[4]  = (image.memoryTypesSupported & REDGPU_B32(0000,0000,0000,0000,0000,0000,0001,0000)) == 0 ? 0 : 1;
      memoryTypeIsSupported[3]  = (image.memoryTypesSupported & REDGPU_B32(0000,0000,0000,0000,0000,0000,0000,1000)) == 0 ? 0 : 1;
      memoryTypeIsSupported[2]  = (image.memoryTypesSupported & REDGPU_B32(0000,0000,0000,0000,0000,0000,0000,0100)) == 0 ? 0 : 1;
      memoryTypeIsSupported[1]  = (image.memoryTypesSupported & REDGPU_B32(0000,0000,0000,0000,0000,0000,0000,0010)) == 0 ? 0 : 1;
      memoryTypeIsSupported[0]  = (image.memoryTypesSupported & REDGPU_B32(0000,0000,0000,0000,0000,0000,0000,0001)) == 0 ? 0 : 1;
      REDGPU_2_EXPECTWG(memoryTypeIsSupported[vkfast->specificMemoryTypesGpuVram] == 1);
      // To destroy
      np(redMemoryAllocate,
        "context", vkfast->context,
        "gpu", vkfast->gpu,
        "handleName", bindingTexture->optional_debug_name,
        "bytesCount", image.memoryBytesCount,
        "memoryTypeIndex", vkfast->specificMemoryTypesGpuVram,
        "dedicateToArray", NULL,
        "dedicateToImage", image.handle,
        "memoryBitflags", 0,
        "outMemory", &imageDedicatedMemory,
        "outStatuses", NULL,
        "optionalFile", optionalFile,
        "optionalLine", optionalLine,
        "optionalUserData", NULL
      );
      REDGPU_2_EXPECTWG(imageDedicatedMemory != NULL);

      RedMemoryImage memoryImage = {0};
      memoryImage.setTo1000157001  = 1000157001;
      memoryImage.setTo0           = 0;
      memoryImage.image            = image.handle;
      memoryImage.memory           = imageDedicatedMemory;
      memoryImage.memoryBytesFirst = 0;
      np(redMemorySet,
        "context", vkfast->context,
        "gpu", vkfast->gpu,
        "memoryArraysCount", 0,
        "memoryArrays", NULL,
        "memoryImagesCount", 1,
        "memoryImages", &memoryImage,
        "outStatuses", NULL,
        "optionalFile", optionalFile,
        "optionalLine", optionalLine,
        "optionalUserData", NULL
      );
    } else {
      REDGPU_2_EXPECTWG((bindingTexture->textureMemory->bytesOffset + image.memoryBytesCount) <= bindingTexture->textureMemory->bytesCount);
      // NOTE(Constantine): Assuming here images do not have any alignment or padding between them.
      RedMemoryImage memoryImage = {0};
      memoryImage.setTo1000157001  = 1000157001;
      memoryImage.setTo0           = 0;
      memoryImage.image            = image.handle;
      memoryImage.memory           = bindingTexture->textureMemory->memory;
      memoryImage.memoryBytesFirst = bindingTexture->textureMemory->bytesOffset;
      np(redMemorySet,
        "context", vkfast->context,
        "gpu", vkfast->gpu,
        "memoryArraysCount", 0,
        "memoryArrays", NULL,
        "memoryImagesCount", 1,
        "memoryImages", &memoryImage,
        "outStatuses", NULL,
        "optionalFile", optionalFile,
        "optionalLine", optionalLine,
        "optionalUserData", NULL
      );
      bindingTexture->textureMemory->bytesOffset += image.memoryBytesCount;
    }

    RedImagePartBitflags imageParts = RED_IMAGE_PART_BITFLAG_COLOR;
    if (
      textureType == GPU_EXTRA_REII_TEXTURE_TYPE_OUTPUT_DEPTH_STENCIL ||
      textureType == GPU_EXTRA_REII_TEXTURE_TYPE_OUTPUT_DEPTH_STENCIL_MSAA
    )
    {
      // NOTE(Constantine): Assumes, a stencil-only image is not possible.
      imageParts = RED_IMAGE_PART_BITFLAG_DEPTH | (formatHasStencil == 1 ? RED_IMAGE_PART_BITFLAG_STENCIL : 0);
    }

    if (
      textureType == GPU_EXTRA_REII_TEXTURE_TYPE_OUTPUT_DEPTH_STENCIL ||
      textureType == GPU_EXTRA_REII_TEXTURE_TYPE_OUTPUT_DEPTH_STENCIL_MSAA
    )
    {
      // To destroy
      np(redCreateTexture,
        "context", vkfast->context,
        "gpu", vkfast->gpu,
        "handleName", bindingTexture->optional_debug_name,
        "image", image.handle,
        "parts", imageParts,
        "dimensions", RED_TEXTURE_DIMENSIONS_2D,
        "format", format,
        "levelsFirst", 0,
        "levelsCount", bindingTexture->mipLevelsCount,
        "layersFirst", 0,
        "layersCount", 1,
        "restrictToAccess", 0,
        "outTexture", &texture,
        "outStatuses", NULL,
        "optionalFile", optionalFile,
        "optionalLine", optionalLine,
        "optionalUserData", NULL
      );
      REDGPU_2_EXPECTWG(texture != NULL);
    } else {
      // To destroy
      np(redCreateTexture,
        "context", vkfast->context,
        "gpu", vkfast->gpu,
        "handleName", bindingTexture->optional_debug_name,
        "image", image.handle,
        "parts", imageParts,
        "dimensions", binding == REII_TEXTURE_BINDING_2D ? RED_TEXTURE_DIMENSIONS_2D : RED_TEXTURE_DIMENSIONS_CUBE,
        "format", format,
        "levelsFirst", 0,
        "levelsCount", bindingTexture->mipLevelsCount,
        "layersFirst", 0,
        "layersCount", binding == REII_TEXTURE_BINDING_2D ? 1 : 6,
        "restrictToAccess", 0,
        "outTexture", &texture,
        "outStatuses", NULL,
        "optionalFile", optionalFile,
        "optionalLine", optionalLine,
        "optionalUserData", NULL
      );
      REDGPU_2_EXPECTWG(texture != NULL);
    }

    if (
      textureType == GPU_EXTRA_REII_TEXTURE_TYPE_OUTPUT_DEPTH_STENCIL ||
      textureType == GPU_EXTRA_REII_TEXTURE_TYPE_OUTPUT_DEPTH_STENCIL_MSAA
    )
    {
      // To destroy
      np(redCreateTexture,
        "context", vkfast->context,
        "gpu", vkfast->gpu,
        "handleName", bindingTexture->optional_debug_name,
        "image", image.handle,
        "parts", RED_IMAGE_PART_BITFLAG_DEPTH,
        "dimensions", RED_TEXTURE_DIMENSIONS_2D,
        "format", format,
        "levelsFirst", 0,
        "levelsCount", bindingTexture->mipLevelsCount,
        "layersFirst", 0,
        "layersCount", 1,
        "restrictToAccess", 0,
        "outTexture", &textureDepthOnly,
        "outStatuses", NULL,
        "optionalFile", optionalFile,
        "optionalLine", optionalLine,
        "optionalUserData", NULL
      );
      REDGPU_2_EXPECTWG(textureDepthOnly != NULL);
      if (formatHasStencil == 1) {
        // To destroy
        np(redCreateTexture,
          "context", vkfast->context,
          "gpu", vkfast->gpu,
          "handleName", bindingTexture->optional_debug_name,
          "image", image.handle,
          "parts", RED_IMAGE_PART_BITFLAG_STENCIL,
          "dimensions", RED_TEXTURE_DIMENSIONS_2D,
          "format", format,
          "levelsFirst", 0,
          "levelsCount", bindingTexture->mipLevelsCount,
          "layersFirst", 0,
          "layersCount", 1,
          "restrictToAccess", 0,
          "outTexture", &textureStencilOnly,
          "outStatuses", NULL,
          "optionalFile", optionalFile,
          "optionalLine", optionalLine,
          "optionalUserData", NULL
        );
        REDGPU_2_EXPECTWG(textureStencilOnly != NULL);
      }
    }

    if (
      binding == REII_TEXTURE_BINDING_CUBE_POSITIVE_X ||
      binding == REII_TEXTURE_BINDING_CUBE_NEGATIVE_X ||
      binding == REII_TEXTURE_BINDING_CUBE_POSITIVE_Y ||
      binding == REII_TEXTURE_BINDING_CUBE_NEGATIVE_Y ||
      binding == REII_TEXTURE_BINDING_CUBE_POSITIVE_Z ||
      binding == REII_TEXTURE_BINDING_CUBE_NEGATIVE_Z
    )
    {
      for (int i = 0; i < 6; i += 1) {
        // To destroy
        np(redCreateTexture,
          "context", vkfast->context,
          "gpu", vkfast->gpu,
          "handleName", bindingTexture->optional_debug_name,
          "image", image.handle,
          "parts", RED_IMAGE_PART_BITFLAG_COLOR,
          "dimensions", RED_TEXTURE_DIMENSIONS_2D,
          "format", format,
          "levelsFirst", 0,
          "levelsCount", bindingTexture->mipLevelsCount,
          "layersFirst", i,
          "layersCount", 1,
          "restrictToAccess", 0,
          "outTexture", &textureCubeFace[i],
          "outStatuses", NULL,
          "optionalFile", optionalFile,
          "optionalLine", optionalLine,
          "optionalUserData", NULL
        );
        REDGPU_2_EXPECTWG(textureCubeFace[i] != NULL);
      }
    }

    redHelperImageSetStateUsable(
      vkfast->context,
      vkfast->gpu,
      image.handle,
      imageParts,
      vkfast->mainQueue,
      vkfast->mainQueueFamilyIndex,
      NULL,
      optionalFile,
      optionalLine,
      NULL
    );
  }

  if (texels != NULL) {
    reiiTextureCopyFromCpu(
      context, // gpu_handle_context_t context
      binding, // ReiiTextureBinding binding
      bindingTexture, // ReiiHandleTexture * bindingTexture
      bindingLevel, // int bindingLevel
      0, // int bindingX
      0, // int bindingY
      width, // int width
      height, // int height
      texelsFormat, // ReiiTextureTexelFormat texelsFormat
      texelsType, // ReiiTextureTexelType texelsType
      texelsBytesAlignment, // int texelsBytesAlignment
      texels // const ReiiCpuGpuTexture * texels
    );
  }

  // Filling
  bindingTexture->width                = width;
  bindingTexture->height               = height;
  bindingTexture->format               = format;
  bindingTexture->image                = image;
  bindingTexture->imageDedicatedMemory = imageDedicatedMemory;
  bindingTexture->texture              = texture;
  bindingTexture->textureDepthOnly     = textureDepthOnly;
  bindingTexture->textureStencilOnly   = textureStencilOnly;
  bindingTexture->textureCubeFace[0]   = textureCubeFace[0];
  bindingTexture->textureCubeFace[1]   = textureCubeFace[1];
  bindingTexture->textureCubeFace[2]   = textureCubeFace[2];
  bindingTexture->textureCubeFace[3]   = textureCubeFace[3];
  bindingTexture->textureCubeFace[4]   = textureCubeFace[4];
  bindingTexture->textureCubeFace[5]   = textureCubeFace[5];
}

GPU_API_PRE void GPU_API_POST reiiTextureCopyFromCpu(gpu_handle_context_t context, ReiiTextureBinding binding, ReiiHandleTexture * bindingTexture, int bindingLevel, int bindingX, int bindingY, int width, int height, ReiiTextureTexelFormat texelsFormat, ReiiTextureTexelType texelsType, int texelsBytesAlignment, const ReiiCpuGpuTexture * texels) {
  const char * optionalFile = NULL;
  int optionalLine = 0;

  vf_handle_context_t * vkfast = (vf_handle_context_t *)(void *)context;

  RedHandleGpu gpu = vkfast->gpu;

  const gpu_extra_reii_texture_type textureType = bindingTexture->textureMemory->texturesType;

  REDGPU_2_EXPECTWG(binding != REII_TEXTURE_BINDING_CUBE);
  if (bindingTexture->binding == REII_TEXTURE_BINDING_2D) {
    REDGPU_2_EXPECTWG(binding == REII_TEXTURE_BINDING_2D);
  }
  REDGPU_2_EXPECTWG(texelsBytesAlignment == 4);
  REDGPU_2_EXPECTWG(bindingTexture->image.handle != NULL);
  REDGPU_2_EXPECTWG(texels != NULL);
  REDGPU_2_EXPECTWG(textureType == GPU_EXTRA_REII_TEXTURE_TYPE_GENERAL);

  RedCallProceduresAndAddresses callProceduresAndAddresses = {0};
  np(redGetCallProceduresAndAddresses,
    "context", vkfast->context,
    "gpu", vkfast->gpu,
    "outCallProceduresAndAddresses", &callProceduresAndAddresses,
    "outStatuses", NULL,
    "optionalFile", optionalFile,
    "optionalLine", optionalLine,
    "optionalUserData", NULL
  );

  // To destroy
  RedCalls calls = {0};
  np(redCreateCalls,
    "context", vkfast->context,
    "gpu", vkfast->gpu,
    "handleName", NULL,
    "queueFamilyIndex", vkfast->mainQueueFamilyIndex,
    "outCalls", &calls,
    "outStatuses", NULL,
    "optionalFile", optionalFile,
    "optionalLine", optionalLine,
    "optionalUserData", NULL
  );
  REDGPU_2_EXPECTWG(calls.handle != NULL);

  np(redCallsSet,
    "context", vkfast->context,
    "gpu", vkfast->gpu,
    "calls", calls.handle,
    "callsMemory", calls.memory,
    "callsReusable", calls.reusable,
    "outStatuses", NULL,
    "optionalFile", optionalFile,
    "optionalLine", optionalLine,
    "optionalUserData", NULL
  );

  RedImagePartBitflags imageParts = RED_IMAGE_PART_BITFLAG_COLOR;
  if (
    textureType == GPU_EXTRA_REII_TEXTURE_TYPE_OUTPUT_DEPTH_STENCIL ||
    textureType == GPU_EXTRA_REII_TEXTURE_TYPE_OUTPUT_DEPTH_STENCIL_MSAA
  )
  {
    // NOTE(Constantine): Assumes, a stencil-only image is not possible.
    imageParts = RED_IMAGE_PART_BITFLAG_DEPTH | (bindingTexture->textureStencilOnly != NULL ? RED_IMAGE_PART_BITFLAG_STENCIL : 0);
  }

  unsigned layerFirst = 0;
  if      (binding == REII_TEXTURE_BINDING_CUBE_POSITIVE_X) { layerFirst = 0; }
  else if (binding == REII_TEXTURE_BINDING_CUBE_NEGATIVE_X) { layerFirst = 1; }
  else if (binding == REII_TEXTURE_BINDING_CUBE_POSITIVE_Y) { layerFirst = 2; }
  else if (binding == REII_TEXTURE_BINDING_CUBE_NEGATIVE_Y) { layerFirst = 3; }
  else if (binding == REII_TEXTURE_BINDING_CUBE_POSITIVE_Z) { layerFirst = 4; }
  else if (binding == REII_TEXTURE_BINDING_CUBE_NEGATIVE_Z) { layerFirst = 5; }

  RedCopyArrayImageRange copy = {0};
  copy.arrayBytesFirst               = texels->cpu_scratch_buffer.arrayRangeBytesFirst;
  copy.arrayTexelsCountToNextRow     = width;
  copy.arrayTexelsCountToNextLayerOr3DDepthSliceDividedByTexelsCountToNextRow = 0;
  copy.imageParts.allParts           = imageParts;
  copy.imageParts.level              = bindingLevel;
  copy.imageParts.layersFirst        = layerFirst;
  copy.imageParts.layersCount        = 1;
  copy.imageOffset.texelX            = bindingX;
  copy.imageOffset.texelY            = bindingY;
  copy.imageOffset.texelZ            = 0;
  copy.imageExtent.texelsCountWidth  = width;
  copy.imageExtent.texelsCountHeight = height;
  copy.imageExtent.texelsCountDepth  = 1;
  npfp(redCallCopyArrayToImage, callProceduresAndAddresses.redCallCopyArrayToImage,
    "calls", calls.handle,
    "arrayR", texels->cpu_scratch_buffer.array,
    "imageW", bindingTexture->image.handle,
    "setTo1", 1,
    "rangesCount", 1,
    "ranges", &copy
  );

  np(redCallsEnd,
    "context", vkfast->context,
    "gpu", vkfast->gpu,
    "calls", calls.handle,
    "callsMemory", calls.memory,
    "outStatuses", NULL,
    "optionalFile", optionalFile,
    "optionalLine", optionalLine,
    "optionalUserData", NULL
  );

  // To destroy
  RedHandleCpuSignal cpuSignal = NULL;
  np(redCreateCpuSignal,
    "context", vkfast->context,
    "gpu", vkfast->gpu,
    "handleName", NULL,
    "createSignaled", 0,
    "outCpuSignal", &cpuSignal,
    "outStatuses", NULL,
    "optionalFile", optionalFile,
    "optionalLine", optionalLine,
    "optionalUserData", NULL
  );
  REDGPU_2_EXPECTWG(cpuSignal != NULL);

  RedGpuTimeline timeline = {0};
  timeline.setTo4                            = 4;
  timeline.setTo0                            = 0;
  timeline.waitForAndUnsignalGpuSignalsCount = 0;
  timeline.waitForAndUnsignalGpuSignals      = NULL;
  timeline.setTo65536                        = NULL;
  timeline.callsCount                        = 1;
  timeline.calls                             = &calls.handle;
  timeline.signalGpuSignalsCount             = 0;
  timeline.signalGpuSignals                  = NULL;
  np(redQueueSubmit,
    "context", vkfast->context,
    "gpu", vkfast->gpu,
    "queue", vkfast->mainQueue,
    "timelinesCount", 1,
    "timelines", &timeline,
    "signalCpuSignal", cpuSignal,
    "outStatuses", NULL,
    "optionalFile", optionalFile,
    "optionalLine", optionalLine,
    "optionalUserData", NULL
  );

  np(redCpuSignalWait,
    "context", vkfast->context,
    "gpu", vkfast->gpu,
    "cpuSignalsCount", 1,
    "cpuSignals", &cpuSignal,
    "waitAll", 1,
    "outStatuses", NULL,
    "optionalFile", optionalFile,
    "optionalLine", optionalLine,
    "optionalUserData", NULL
  );

  np(redDestroyCpuSignal,
    "context", vkfast->context,
    "gpu", vkfast->gpu,
    "cpuSignal", cpuSignal,
    "optionalFile", optionalFile,
    "optionalLine", optionalLine,
    "optionalUserData", NULL
  );

  np(redDestroyCalls,
    "context", vkfast->context,
    "gpu", vkfast->gpu,
    "calls", calls.handle,
    "callsMemory", calls.memory,
    "optionalFile", optionalFile,
    "optionalLine", optionalLine,
    "optionalUserData", NULL
  );
}

GPU_API_PRE void GPU_API_POST reiiCommandListReset(gpu_handle_context_t context, ReiiHandleCommandList * list) {
  const char * optionalFile = NULL;
  int optionalLine = 0;

  vf_handle_context_t * vkfast = (vf_handle_context_t *)(void *)context;

  RedHandleGpu gpu = vkfast->gpu;

  {
    vf_handle_t * batch = (vf_handle_t *)(void *)list->batch_id;
    REDGPU_2_EXPECTWG(batch != NULL);
    REDGPU_2_EXPECTWG(batch->handle_id == VF_HANDLE_ID_BATCH);
  }

  {
    list->dynamicMeshPositionVec4Offset = 0;
    list->dynamicMeshColorVec4Offset    = 0;
    list->dynamicMeshNormalVec4Offset   = 0;
    for (int i = 0; i < REII_TEXCOORDS_MAX_COUNT; i += 1) {
      list->dynamicMeshTexcoordVec4Offset[i] = 0;
    }
    list->dynamicMeshPositionVec4CurrentStart = 0;
    list->dynamicMeshColorVec4CurrentStart    = 0;
    list->dynamicMeshNormalVec4CurrentStart   = 0;
    for (int i = 0; i < REII_TEXCOORDS_MAX_COUNT; i += 1) {
      list->dynamicMeshTexcoordVec4CurrentStart[i] = 0;
    }
    list->currentProcedureParametersDraw = NULL;
  }

  {
    np(redGetCallProceduresAndAddresses,
      "context", vkfast->context,
      "gpu", vkfast->gpu,
      "outCallProceduresAndAddresses", &list->callProceduresAndAddresses,
      "outStatuses", NULL,
      "optionalFile", optionalFile,
      "optionalLine", optionalLine,
      "optionalUserData", NULL
    );
  }

  {
    unsigned outputsCount = list->mutable_outputs_array.count;
    for (unsigned i = 0; i < outputsCount; i += 1) {
      red2DestroyHandle(vkfast->context, vkfast->gpu, RED_HANDLE_TYPE_OUTPUT, list->mutable_outputs_array.items[i].handle, NULL, optionalFile, optionalLine, NULL);
    }
    for (unsigned i = 0; i < outputsCount; i += 1) {
      red2DestroyHandle(vkfast->context, vkfast->gpu, RED_HANDLE_TYPE_OUTPUT_DECLARATION, list->mutable_outputs_array.items[i].handleDeclaration, NULL, optionalFile, optionalLine, NULL);
    }
    for (unsigned i = 0; i < outputsCount; i += 1) {
      // Filling
      Red2Output;
      list->mutable_outputs_array.items[i].handle = NULL;
      list->mutable_outputs_array.items[i].handleDeclaration = NULL;
    }
    list->mutable_outputs_array.count = 0;
  }
}

GPU_API_PRE void GPU_API_POST reiiCommandSetViewportEx(gpu_handle_context_t context, ReiiHandleCommandList * list, int x, int y, int width, int height, float depthMin, float depthMax) {
  const char * optionalFile = NULL;
  int optionalLine = 0;

  vf_handle_t * batch = (vf_handle_t *)(void *)list->batch_id;

  np(redCallSetDynamicViewport,
    "address", list->callProceduresAndAddresses.redCallSetDynamicViewport,
    "calls", batch->batch.calls.handle,
    "x", x,
    "y", y,
    "width", width,
    "height", height,
    "depthMin", depthMin,
    "depthMax", depthMax
  );
}

GPU_API_PRE void GPU_API_POST reiiCommandSetScissor(gpu_handle_context_t context, ReiiHandleCommandList * list, int x, int y, int width, int height) {
  const char * optionalFile = NULL;
  int optionalLine = 0;

  vf_handle_t * batch = (vf_handle_t *)(void *)list->batch_id;

  np(redCallSetDynamicScissor,
    "address", list->callProceduresAndAddresses.redCallSetDynamicScissor,
    "calls", batch->batch.calls.handle,
    "x", x,
    "y", y,
    "width", width,
    "height", height
  );
}

GPU_API_PRE void GPU_API_POST reiiCommandClearTexture(gpu_handle_context_t context, ReiiHandleCommandList * list, ReiiHandleTexture * depthStencilTexture, ReiiHandleTexture * colorTexture, RedHandleTexture colorTextureHandle, ReiiClearFlags clear, float depthValue, unsigned stencilValue, float colorR, float colorG, float colorB, float colorA) {
  const char * optionalFile = NULL;
  int optionalLine = 0;

  if (depthStencilTexture == NULL && colorTexture == NULL) {
    return;
  }

  vf_handle_t * batch = (vf_handle_t *)(void *)list->batch_id;
  vf_handle_context_t * vkfast = batch->vkfast;
  RedHandleGpu gpu = vkfast->gpu;

  unsigned                   depthStencilWidth            = 0;
  unsigned                   depthStencilHeight           = 0;
  RedFormat                  depthStencilFormat           = RED_FORMAT_UNDEFINED;
  RedMultisampleCountBitflag depthStencilMultisampleCount = (RedMultisampleCountBitflag)0;
  unsigned                   colorWidth                   = 0;
  unsigned                   colorHeight                  = 0;
  RedFormat                  colorFormat                  = RED_FORMAT_UNDEFINED;
  RedMultisampleCountBitflag colorMultisampleCount        = (RedMultisampleCountBitflag)0;

  RedOutputMembers            outputMembers            = {0};
  RedOutputDeclarationMembers outputDeclarationMembers = {0};
  outputDeclarationMembers.depthStencilEnable                        = 0;
  outputDeclarationMembers.depthStencilFormat                        = RED_FORMAT_UNDEFINED;
  outputDeclarationMembers.depthStencilMultisampleCount              = RED_MULTISAMPLE_COUNT_BITFLAG_1;
  outputDeclarationMembers.depthStencilDepthSetProcedureOutputOp     = RED_SET_PROCEDURE_OUTPUT_OP_PRESERVE;
  outputDeclarationMembers.depthStencilDepthEndProcedureOutputOp     = RED_END_PROCEDURE_OUTPUT_OP_PRESERVE;
  outputDeclarationMembers.depthStencilStencilSetProcedureOutputOp   = RED_SET_PROCEDURE_OUTPUT_OP_PRESERVE;
  outputDeclarationMembers.depthStencilStencilEndProcedureOutputOp   = RED_END_PROCEDURE_OUTPUT_OP_PRESERVE;
  outputDeclarationMembers.depthStencilSharesMemoryWithAnotherMember = 0;
  outputDeclarationMembers.colorsCount                               = 0;
  outputDeclarationMembers.colorsFormat[0]                           = RED_FORMAT_UNDEFINED;
  outputDeclarationMembers.colorsMultisampleCount[0]                 = RED_MULTISAMPLE_COUNT_BITFLAG_1;
  outputDeclarationMembers.colorsSetProcedureOutputOp[0]             = RED_SET_PROCEDURE_OUTPUT_OP_PRESERVE;
  outputDeclarationMembers.colorsEndProcedureOutputOp[0]             = RED_END_PROCEDURE_OUTPUT_OP_PRESERVE;
  outputDeclarationMembers.colorsSharesMemoryWithAnotherMember[0]    = 0;
  if (
    ((clear & REII_CLEAR_DEPTH_BIT)   == REII_CLEAR_DEPTH_BIT) &&
    ((clear & REII_CLEAR_STENCIL_BIT) == REII_CLEAR_STENCIL_BIT)
  )
  {
    REDGPU_2_EXPECTWG(depthStencilTexture != NULL);

    depthStencilWidth            = depthStencilTexture->width;
    depthStencilHeight           = depthStencilTexture->height;
    depthStencilFormat           = depthStencilTexture->format;
    depthStencilMultisampleCount = depthStencilTexture->msaaCount;

    outputDeclarationMembers.depthStencilEnable                      = 1;
    outputDeclarationMembers.depthStencilFormat                      = depthStencilFormat;
    outputDeclarationMembers.depthStencilMultisampleCount            = depthStencilMultisampleCount;
    outputDeclarationMembers.depthStencilDepthSetProcedureOutputOp   = RED_SET_PROCEDURE_OUTPUT_OP_CLEAR;
    outputDeclarationMembers.depthStencilStencilSetProcedureOutputOp = RED_SET_PROCEDURE_OUTPUT_OP_CLEAR;

    outputMembers.depthStencil = depthStencilTexture->texture;
  } else if ((clear & REII_CLEAR_DEPTH_BIT) == REII_CLEAR_DEPTH_BIT) {
    REDGPU_2_EXPECTWG(depthStencilTexture != NULL);

    depthStencilWidth            = depthStencilTexture->width;
    depthStencilHeight           = depthStencilTexture->height;
    depthStencilFormat           = depthStencilTexture->format;
    depthStencilMultisampleCount = depthStencilTexture->msaaCount;

    outputDeclarationMembers.depthStencilEnable                    = 1;
    outputDeclarationMembers.depthStencilFormat                    = depthStencilFormat;
    outputDeclarationMembers.depthStencilMultisampleCount          = depthStencilMultisampleCount;
    outputDeclarationMembers.depthStencilDepthSetProcedureOutputOp = RED_SET_PROCEDURE_OUTPUT_OP_CLEAR;

    outputMembers.depthStencil = depthStencilTexture->textureDepthOnly;
  } else if ((clear & REII_CLEAR_STENCIL_BIT) == REII_CLEAR_STENCIL_BIT) {
    REDGPU_2_EXPECTWG(depthStencilTexture != NULL);

    depthStencilWidth            = depthStencilTexture->width;
    depthStencilHeight           = depthStencilTexture->height;
    depthStencilFormat           = depthStencilTexture->format;
    depthStencilMultisampleCount = depthStencilTexture->msaaCount;

    outputDeclarationMembers.depthStencilEnable                      = 1;
    outputDeclarationMembers.depthStencilFormat                      = depthStencilFormat;
    outputDeclarationMembers.depthStencilMultisampleCount            = depthStencilMultisampleCount;
    outputDeclarationMembers.depthStencilStencilSetProcedureOutputOp = RED_SET_PROCEDURE_OUTPUT_OP_CLEAR;

    outputMembers.depthStencil = depthStencilTexture->textureStencilOnly;
  }
  if ((clear & REII_CLEAR_COLOR_BIT) == REII_CLEAR_COLOR_BIT) {
    REDGPU_2_EXPECTWG(colorTexture != NULL);
    REDGPU_2_EXPECTWG(colorTextureHandle != NULL);

    colorWidth            = colorTexture->width;
    colorHeight           = colorTexture->height;
    colorFormat           = colorTexture->format;
    colorMultisampleCount = colorTexture->msaaCount;

    outputDeclarationMembers.colorsCount                   = 1;
    outputDeclarationMembers.colorsFormat[0]               = colorFormat;
    outputDeclarationMembers.colorsMultisampleCount[0]     = colorMultisampleCount;
    outputDeclarationMembers.colorsSetProcedureOutputOp[0] = RED_SET_PROCEDURE_OUTPUT_OP_CLEAR;

    outputMembers.colorsCount = 1;
    outputMembers.colors[0]   = colorTextureHandle;
  }

  if (depthStencilWidth != 0 && depthStencilHeight != 0 && colorWidth != 0 && colorHeight != 0) {
    REDGPU_2_EXPECTWG(depthStencilWidth  == colorWidth);
    REDGPU_2_EXPECTWG(depthStencilHeight == colorHeight);
  }
  unsigned width  = depthStencilWidth  != 0 ? depthStencilWidth  : colorWidth;
  unsigned height = depthStencilHeight != 0 ? depthStencilHeight : colorHeight;

  RedColorsClearValuesFloat colorsClearValuesFloat = {0};
  colorsClearValuesFloat.r[0] = colorR;
  colorsClearValuesFloat.g[0] = colorG;
  colorsClearValuesFloat.b[0] = colorB;
  colorsClearValuesFloat.a[0] = colorA;

  np(red2CallSetProcedureOutput,
    "address", list->callProceduresAndAddresses.redCallSetProcedureOutput,
    "calls", batch->batch.calls.handle,
    "context", vkfast->context,
    "gpu", vkfast->gpu,
    "mutableOutputsArray", &list->mutable_outputs_array,
    "outputDeclarationMembers", &outputDeclarationMembers,
    "outputDeclarationMembersResolveSources", NULL,
    "dependencyByRegion", 0,
    "dependencyByRegionAllowUsageAliasOrderBarriers", 0,
    "outputMembers", &outputMembers,
    "outputMembersResolveTargets", NULL,
    "width", width,
    "height", height,
    "depthClearValue", depthValue,
    "stencilClearValue", stencilValue,
    "colorsClearValuesFloat", &colorsClearValuesFloat,
    "colorsClearValuesSint", NULL,
    "colorsClearValuesUint", NULL,
    "outStatuses", NULL,
    "optionalFile", optionalFile,
    "optionalLine", optionalLine,
    "optionalUserData", NULL
  );

  np(redCallEndProcedureOutput,
    "address", list->callProceduresAndAddresses.redCallEndProcedureOutput,
    "calls", batch->batch.calls.handle
  );
}

GPU_API_PRE void GPU_API_POST reiiCommandMeshSetState(gpu_handle_context_t context, ReiiHandleCommandList * list, ReiiMeshState * state, void * _) {
  const char * optionalFile = NULL;
  int optionalLine = 0;

  vf_handle_t * batch = (vf_handle_t *)(void *)list->batch_id;

  npfp(redCallSetProcedure, list->callProceduresAndAddresses.redCallSetProcedure,
    "calls", batch->batch.calls.handle,
    "procedureType", RED_PROCEDURE_TYPE_DRAW,
    "procedure", state->procedure
  );

  list->currentProcedureParametersDraw = state->procedureParameters.procedureParameters;
}

GPU_API_PRE void GPU_API_POST reiiCommandBindNewBindingsSet(gpu_handle_context_t context, ReiiHandleCommandList * list, int slotsCount, const RedStructDeclarationMember * slots) {
  const char * optionalFile = NULL;
  int optionalLine = 0;

  vf_handle_t * batch = (vf_handle_t *)(void *)list->batch_id;
  vf_handle_context_t * vkfast = batch->vkfast;
  RedHandleGpu gpu = vkfast->gpu;

  REDGPU_2_EXPECTWG(batch->batch.structsMemory != NULL || !"vfBatchBegin()::batch_bindings_info was likely set to NULL?");
  if (list->currentProcedureParametersDraw == NULL) {
    REDGPU_2_EXPECTWG(!"Was reiiCommandMeshSetState() ever called previously?");
  }

  Red2Struct structure = {0};
  np(red2StructsMemorySuballocateStruct,
    "context", vkfast->context,
    "gpu", vkfast->gpu,
    "handleName", NULL,
    "structsMemory", batch->batch.structsMemory,
    "structDeclarationMembersCount", slotsCount,
    "structDeclarationMembers", slots,
    "structDeclarationMembersArrayROCount", 0,
    "structDeclarationMembersArrayRO", NULL,
    "outStruct", &structure,
    "outStatuses", NULL,
    "optionalFile", optionalFile,
    "optionalLine", optionalLine,
    "optionalUserData", NULL
  );
  batch->batch.currentStruct = structure;

  np(redCallSetProcedureParameters,
    "address", batch->batch.addresses.redCallSetProcedureParameters,
    "calls", batch->batch.calls.handle,
    "procedureType", RED_PROCEDURE_TYPE_DRAW,
    "procedureParameters", list->currentProcedureParametersDraw
  );
}

GPU_API_PRE void GPU_API_POST reiiCommandBindStorageRaw(gpu_handle_context_t context, ReiiHandleCommandList * list, int slot, int storageRawCount, const RedStructMemberArray * storageRaw) {
  const char * optionalFile = NULL;
  int optionalLine = 0;

  vf_handle_t * batch = (vf_handle_t *)(void *)list->batch_id;
  vf_handle_context_t * vkfast = batch->vkfast;
  RedHandleGpu gpu = vkfast->gpu;

  REDGPU_2_EXPECTWG(batch->batch.currentStruct.handle != NULL || !"Was reiiCommandBindNewBindingsSet() ever called previously?");

  RedStructMember member = {0};
  member.setTo35   = 35;
  member.setTo0    = 0;
  member.structure = batch->batch.currentStruct.handle;
  member.slot      = slot;
  member.first     = 0;
  member.count     = storageRawCount;
  member.type      = RED_STRUCT_MEMBER_TYPE_ARRAY_RO_RW;
  member.textures  = NULL;
  member.arrays    = storageRaw;
  member.setTo00   = 0;
  np(redStructsSet,
    "context", vkfast->context,
    "gpu", vkfast->gpu,
    "structsMembersCount", 1,
    "structsMembers", &member,
    "optionalFile", optionalFile,
    "optionalLine", optionalLine,
    "optionalUserData", NULL
  );
}

GPU_API_PRE void GPU_API_POST reiiCommandBindNewBindingsEnd(gpu_handle_context_t context, ReiiHandleCommandList * list) {
  const char * optionalFile = NULL;
  int optionalLine = 0;

  vf_handle_t * batch = (vf_handle_t *)(void *)list->batch_id;
  vf_handle_context_t * vkfast = batch->vkfast;
  RedHandleGpu gpu = vkfast->gpu;

  npfp(redCallSetProcedureParametersStructs, batch->batch.addresses.redCallSetProcedureParametersStructs,
    "calls", batch->batch.calls.handle,
    "procedureType", RED_PROCEDURE_TYPE_DRAW,
    "procedureParameters", list->currentProcedureParametersDraw,
    "procedureParametersDeclarationStructsDeclarationsFirst", 0,
    "structsCount", 1, // NOTE(Constantine): Only one struct for now.
    "structs", &batch->batch.currentStruct.handle,
    "setTo0", 0,
    "setTo00", 0
  );

  np(red2DestroyHandle,
    "context", vkfast->context,
    "gpu", vkfast->gpu,
    "handleType", RED_HANDLE_TYPE_STRUCT_DECLARATION,
    "handle", batch->batch.currentStruct.handleDeclaration,
    "optionalHandle2", NULL,
    "optionalFile", optionalFile,
    "optionalLine", optionalLine,
    "optionalUserData", NULL
  );
  batch->batch.currentStruct.handleDeclaration = NULL;
  batch->batch.currentStruct.handle = NULL;
}

GPU_API_PRE void GPU_API_POST reiiCommandBindVariablesCopy(gpu_handle_context_t context, ReiiHandleCommandList * list, unsigned variablesBytesOffset, unsigned dataBytesCount, const void * data) {
  const char * optionalFile = NULL;
  int optionalLine = 0;

  vf_handle_t * batch = (vf_handle_t *)(void *)list->batch_id;
  vf_handle_context_t * vkfast = batch->vkfast;
  RedHandleGpu gpu = vkfast->gpu;

  REDGPU_2_EXPECTWG(list->currentProcedureParametersDraw != NULL || !"Was reiiCommandMeshSetState() ever called previously?");

  npfp(redCallSetProcedureParametersVariables, batch->batch.addresses.redCallSetProcedureParametersVariables,
    "calls", batch->batch.calls.handle,
    "procedureParameters", list->currentProcedureParametersDraw,
    "visibleToStages", RED_VISIBLE_TO_STAGE_BITFLAG_VERTEX | RED_VISIBLE_TO_STAGE_BITFLAG_FRAGMENT,
    "variablesBytesFirst", variablesBytesOffset,
    "dataBytesCount", dataBytesCount,
    "data", data
  );
}

GPU_API_PRE void GPU_API_POST reiiCommandRenderTargetSet(gpu_handle_context_t context, ReiiHandleCommandList * list, ReiiHandleTexture * depthStencilTexture, ReiiHandleTexture * colorTexture, RedHandleTexture colorTextureHandle) {
  const char * optionalFile = NULL;
  int optionalLine = 0;

  vf_handle_t * batch = (vf_handle_t *)(void *)list->batch_id;
  vf_handle_context_t * vkfast = batch->vkfast;
  RedHandleGpu gpu = vkfast->gpu;

  unsigned                   depthStencilWidth            = 0;
  unsigned                   depthStencilHeight           = 0;
  RedFormat                  depthStencilFormat           = RED_FORMAT_UNDEFINED;
  RedMultisampleCountBitflag depthStencilMultisampleCount = (RedMultisampleCountBitflag)0;
  unsigned                   colorWidth                   = 0;
  unsigned                   colorHeight                  = 0;
  RedFormat                  colorFormat                  = RED_FORMAT_UNDEFINED;
  RedMultisampleCountBitflag colorMultisampleCount        = (RedMultisampleCountBitflag)0;

  RedOutputMembers            outputMembers            = {0};
  RedOutputDeclarationMembers outputDeclarationMembers = {0};
  outputDeclarationMembers.depthStencilEnable                        = 0;
  outputDeclarationMembers.depthStencilFormat                        = RED_FORMAT_UNDEFINED;
  outputDeclarationMembers.depthStencilMultisampleCount              = RED_MULTISAMPLE_COUNT_BITFLAG_1;
  outputDeclarationMembers.depthStencilDepthSetProcedureOutputOp     = RED_SET_PROCEDURE_OUTPUT_OP_PRESERVE;
  outputDeclarationMembers.depthStencilDepthEndProcedureOutputOp     = RED_END_PROCEDURE_OUTPUT_OP_PRESERVE;
  outputDeclarationMembers.depthStencilStencilSetProcedureOutputOp   = RED_SET_PROCEDURE_OUTPUT_OP_PRESERVE;
  outputDeclarationMembers.depthStencilStencilEndProcedureOutputOp   = RED_END_PROCEDURE_OUTPUT_OP_PRESERVE;
  outputDeclarationMembers.depthStencilSharesMemoryWithAnotherMember = 0;
  outputDeclarationMembers.colorsCount                               = 0;
  outputDeclarationMembers.colorsFormat[0]                           = RED_FORMAT_UNDEFINED;
  outputDeclarationMembers.colorsMultisampleCount[0]                 = RED_MULTISAMPLE_COUNT_BITFLAG_1;
  outputDeclarationMembers.colorsSetProcedureOutputOp[0]             = RED_SET_PROCEDURE_OUTPUT_OP_PRESERVE;
  outputDeclarationMembers.colorsEndProcedureOutputOp[0]             = RED_END_PROCEDURE_OUTPUT_OP_PRESERVE;
  outputDeclarationMembers.colorsSharesMemoryWithAnotherMember[0]    = 0;
  if (depthStencilTexture != NULL) {
    depthStencilWidth            = depthStencilTexture->width;
    depthStencilHeight           = depthStencilTexture->height;
    depthStencilFormat           = depthStencilTexture->format;
    depthStencilMultisampleCount = depthStencilTexture->msaaCount;

    outputDeclarationMembers.depthStencilEnable           = 1;
    outputDeclarationMembers.depthStencilFormat           = depthStencilFormat;
    outputDeclarationMembers.depthStencilMultisampleCount = depthStencilMultisampleCount;

    outputMembers.depthStencil = depthStencilTexture->texture;
  }
  if (colorTexture != NULL) {
    REDGPU_2_EXPECTWG(colorTextureHandle != NULL);

    colorWidth            = colorTexture->width;
    colorHeight           = colorTexture->height;
    colorFormat           = colorTexture->format;
    colorMultisampleCount = colorTexture->msaaCount;

    outputDeclarationMembers.colorsCount                   = 1;
    outputDeclarationMembers.colorsFormat[0]               = colorFormat;
    outputDeclarationMembers.colorsMultisampleCount[0]     = colorMultisampleCount;

    outputMembers.colorsCount = 1;
    outputMembers.colors[0]   = colorTextureHandle;
  }

  if (depthStencilWidth != 0 && depthStencilHeight != 0 && colorWidth != 0 && colorHeight != 0) {
    REDGPU_2_EXPECTWG(depthStencilWidth  == colorWidth);
    REDGPU_2_EXPECTWG(depthStencilHeight == colorHeight);
  }
  unsigned width  = depthStencilWidth  != 0 ? depthStencilWidth  : colorWidth;
  unsigned height = depthStencilHeight != 0 ? depthStencilHeight : colorHeight;

  np(red2CallSetProcedureOutput,
    "address", list->callProceduresAndAddresses.redCallSetProcedureOutput,
    "calls", batch->batch.calls.handle,
    "context", vkfast->context,
    "gpu", vkfast->gpu,
    "mutableOutputsArray", &list->mutable_outputs_array,
    "outputDeclarationMembers", &outputDeclarationMembers,
    "outputDeclarationMembersResolveSources", NULL,
    "dependencyByRegion", 0,
    "dependencyByRegionAllowUsageAliasOrderBarriers", 0,
    "outputMembers", &outputMembers,
    "outputMembersResolveTargets", NULL,
    "width", width,
    "height", height,
    "depthClearValue", 0,
    "stencilClearValue", 0,
    "colorsClearValuesFloat", NULL,
    "colorsClearValuesSint", NULL,
    "colorsClearValuesUint", NULL,
    "outStatuses", NULL,
    "optionalFile", optionalFile,
    "optionalLine", optionalLine,
    "optionalUserData", NULL
  );
}

GPU_API_PRE void GPU_API_POST reiiCommandRenderTargetEnd(gpu_handle_context_t context, ReiiHandleCommandList * list) {
  const char * optionalFile = NULL;
  int optionalLine = 0;

  vf_handle_t * batch = (vf_handle_t *)(void *)list->batch_id;

  np(redCallEndProcedureOutput,
    "address", list->callProceduresAndAddresses.redCallEndProcedureOutput,
    "calls", batch->batch.calls.handle
  );
}

GPU_API_PRE void GPU_API_POST reiiCommandMeshSet(gpu_handle_context_t context, ReiiHandleCommandList * list) {
  const char * optionalFile = NULL;
  int optionalLine = 0;

  list->dynamicMeshPositionVec4CurrentStart = list->dynamicMeshPositionVec4Offset;
  list->dynamicMeshColorVec4CurrentStart    = list->dynamicMeshColorVec4Offset;
  list->dynamicMeshNormalVec4CurrentStart   = list->dynamicMeshNormalVec4Offset;
  for (int i = 0; i < REII_TEXCOORDS_MAX_COUNT; i += 1) {
    list->dynamicMeshTexcoordVec4CurrentStart[i] = list->dynamicMeshTexcoordVec4Offset[i];
  }
}

GPU_API_PRE void GPU_API_POST reiiCommandMeshEndEx(gpu_handle_context_t context, ReiiHandleCommandList * list, ReiiHandleTexture * depthStencilTexture, ReiiHandleTexture * colorTexture, RedHandleTexture colorTextureHandle) {
  const char * optionalFile = NULL;
  int optionalLine = 0;

  vf_handle_t * batch = (vf_handle_t *)(void *)list->batch_id;
  vf_handle_context_t * vkfast = batch->vkfast;
  RedHandleGpu gpu = vkfast->gpu;

  uint64_t dynamicMeshPositionCopyVec4Count = list->dynamicMeshPositionVec4Offset - list->dynamicMeshPositionVec4CurrentStart;
  uint64_t dynamicMeshColorCopyVec4Count    = list->dynamicMeshColorVec4Offset    - list->dynamicMeshColorVec4CurrentStart;
  uint64_t dynamicMeshNormalCopyVec4Count   = list->dynamicMeshNormalVec4Offset   - list->dynamicMeshNormalVec4CurrentStart;
  uint64_t dynamicMeshTexcoordCopyVec4Count[REII_TEXCOORDS_MAX_COUNT] = {0};
  for (int i = 0; i < REII_TEXCOORDS_MAX_COUNT; i += 1) {
    dynamicMeshTexcoordCopyVec4Count[i] = list->dynamicMeshTexcoordVec4Offset[i] - list->dynamicMeshTexcoordVec4CurrentStart[i];
  }

  if (dynamicMeshPositionCopyVec4Count == 0) {
    return;
  }

  REDGPU_2_EXPECTWG(dynamicMeshPositionCopyVec4Count % 3 == 0);

  vfeCpuGpuArrayBatchCopyFromCpuToGpu(
    context,
    list->batch_id,
    &list->dynamic_mesh_position,
    list->dynamicMeshPositionVec4CurrentStart * sizeof(ReiiVec4),
    dynamicMeshPositionCopyVec4Count * sizeof(ReiiVec4),
    optionalFile,
    optionalLine
  );
  if (dynamicMeshColorCopyVec4Count > 0) {
    vfeCpuGpuArrayBatchCopyFromCpuToGpu(
      context,
      list->batch_id,
      &list->dynamic_mesh_color,
      list->dynamicMeshColorVec4CurrentStart * sizeof(ReiiVec4),
      dynamicMeshColorCopyVec4Count * sizeof(ReiiVec4),
      optionalFile,
      optionalLine
    );
  }
  if (dynamicMeshNormalCopyVec4Count > 0) {
    vfeCpuGpuArrayBatchCopyFromCpuToGpu(
      context,
      list->batch_id,
      &list->dynamic_mesh_normal,
      list->dynamicMeshNormalVec4CurrentStart * sizeof(ReiiVec4),
      dynamicMeshNormalCopyVec4Count * sizeof(ReiiVec4),
      optionalFile,
      optionalLine
    );
  }
  for (int i = 0; i < REII_TEXCOORDS_MAX_COUNT; i += 1) {
    if (dynamicMeshTexcoordCopyVec4Count[i] > 0) {
      vfeCpuGpuArrayBatchCopyFromCpuToGpu(
        context,
        list->batch_id,
        &list->dynamic_mesh_texcoord[i],
        list->dynamicMeshTexcoordVec4CurrentStart[i] * sizeof(ReiiVec4),
        dynamicMeshTexcoordCopyVec4Count[i] * sizeof(ReiiVec4),
        optionalFile,
        optionalLine
      );
    }
  }

  // NOTE(Constantine)(Mar 15, 2026): Should I insert a GPU barrier here, to wait for copies to finish?
  // NOTE(Constantine)(Mar 19, 2026): Yes, I should: AMD RX 550 needs this barrier to wait for copies to finish.
  vfBatchBarrierMemory(context, list->batch_id, optionalFile, optionalLine);

  reiiCommandRenderTargetSet(context, list, depthStencilTexture, colorTexture, colorTextureHandle);
  npfp(redCallProcedure, list->callProceduresAndAddresses.redCallProcedure,
    "calls", batch->batch.calls.handle,
    "vertexCount", dynamicMeshPositionCopyVec4Count,
    "instanceCount", 1,
    "vertexFirst", 0,
    "instanceFirst", 0
  );
  reiiCommandRenderTargetEnd(context, list);
}

GPU_API_PRE void GPU_API_POST reiiCommandMeshTexcoord(gpu_handle_context_t context, ReiiHandleCommandList * list, unsigned index, float x, float y, float z, float w) {
  const char * optionalFile = NULL;
  int optionalLine = 0;

  vf_handle_t * batch = (vf_handle_t *)(void *)list->batch_id;
  vf_handle_context_t * vkfast = batch->vkfast;
  RedHandleGpu gpu = vkfast->gpu;

  REDGPU_2_EXPECTWG(index < REII_TEXCOORDS_MAX_COUNT);

  ReiiVec4 * cpu_as_vec4_start = (ReiiVec4 *)list->dynamic_mesh_texcoord[index].cpu_ptr;
  ReiiVec4 * cpu_as_vec4       = &cpu_as_vec4_start[list->dynamicMeshTexcoordVec4Offset[index]];

  cpu_as_vec4[0].x = x;
  cpu_as_vec4[0].y = y;
  cpu_as_vec4[0].z = z;
  cpu_as_vec4[0].w = w;

  list->dynamicMeshTexcoordVec4Offset[index] += 1;
}

GPU_API_PRE void GPU_API_POST reiiCommandMeshColor(gpu_handle_context_t context, ReiiHandleCommandList * list, float r, float g, float b, float a) {
  const char * optionalFile = NULL;
  int optionalLine = 0;

  ReiiVec4 * cpu_as_vec4_start = (ReiiVec4 *)list->dynamic_mesh_color.cpu_ptr;
  ReiiVec4 * cpu_as_vec4       = &cpu_as_vec4_start[list->dynamicMeshColorVec4Offset];

  cpu_as_vec4[0].x = r;
  cpu_as_vec4[0].y = g;
  cpu_as_vec4[0].z = b;
  cpu_as_vec4[0].w = a;

  list->dynamicMeshColorVec4Offset += 1;
}

GPU_API_PRE void GPU_API_POST reiiCommandMeshNormal(gpu_handle_context_t context, ReiiHandleCommandList * list, float x, float y, float z) {
  const char * optionalFile = NULL;
  int optionalLine = 0;

  ReiiVec4 * cpu_as_vec4_start = (ReiiVec4 *)list->dynamic_mesh_normal.cpu_ptr;
  ReiiVec4 * cpu_as_vec4       = &cpu_as_vec4_start[list->dynamicMeshNormalVec4Offset];

  cpu_as_vec4[0].x = x;
  cpu_as_vec4[0].y = y;
  cpu_as_vec4[0].z = z;
  cpu_as_vec4[0].w = 1; // NOTE(Constantine): Hmm... I guess?

  list->dynamicMeshNormalVec4Offset += 1;
}

GPU_API_PRE void GPU_API_POST reiiCommandMeshPosition(gpu_handle_context_t context, ReiiHandleCommandList * list, float x, float y, float z, float w) {
  const char * optionalFile = NULL;
  int optionalLine = 0;

  ReiiVec4 * cpu_as_vec4_start = (ReiiVec4 *)list->dynamic_mesh_position.cpu_ptr;
  ReiiVec4 * cpu_as_vec4       = &cpu_as_vec4_start[list->dynamicMeshPositionVec4Offset];

  cpu_as_vec4[0].x = x;
  cpu_as_vec4[0].y = y;
  cpu_as_vec4[0].z = z;
  cpu_as_vec4[0].w = w;

  list->dynamicMeshPositionVec4Offset += 1;
}

GPU_API_PRE void GPU_API_POST reiiCommandCopyFromColorTextureToStorageRaw(gpu_handle_context_t context, ReiiHandleCommandList * list, ReiiHandleTexture * texture, RedStructMemberArray * storageRaw) {
  const char * optionalFile = NULL;
  int optionalLine = 0;

  vf_handle_t * batch = (vf_handle_t *)(void *)list->batch_id;
  vf_handle_context_t * vkfast = batch->vkfast;
  RedHandleGpu gpu = vkfast->gpu;

  const gpu_extra_reii_texture_type textureType = texture->textureMemory->texturesType;

  RedImagePartBitflags imageParts = RED_IMAGE_PART_BITFLAG_COLOR;
  if (
    textureType == GPU_EXTRA_REII_TEXTURE_TYPE_OUTPUT_DEPTH_STENCIL ||
    textureType == GPU_EXTRA_REII_TEXTURE_TYPE_OUTPUT_DEPTH_STENCIL_MSAA
  )
  {
    // NOTE(Constantine): Assumes, a stencil-only image is not possible.
    imageParts = RED_IMAGE_PART_BITFLAG_DEPTH | (texture->textureStencilOnly != NULL ? RED_IMAGE_PART_BITFLAG_STENCIL : 0);
  }

  RedCopyArrayImageRange copy = {0};
  copy.arrayBytesFirst               = storageRaw->arrayRangeBytesFirst;
  copy.arrayTexelsCountToNextRow     = texture->width;
  copy.arrayTexelsCountToNextLayerOr3DDepthSliceDividedByTexelsCountToNextRow = 0;
  copy.imageParts.allParts           = imageParts;
  copy.imageParts.level              = 0;
  copy.imageParts.layersFirst        = 0;
  copy.imageParts.layersCount        = 1;
  copy.imageOffset.texelX            = 0;
  copy.imageOffset.texelY            = 0;
  copy.imageOffset.texelZ            = 0;
  copy.imageExtent.texelsCountWidth  = texture->width;
  copy.imageExtent.texelsCountHeight = texture->height;
  copy.imageExtent.texelsCountDepth  = 1;
  npfp(redCallCopyImageToArray, batch->batch.addresses.redCallCopyImageToArray,
    "calls", batch->batch.calls.handle,
    "imageR", texture->image.handle,
    "setTo1", 1,
    "arrayW", storageRaw->array,
    "rangesCount", 1,
    "ranges", &copy
  );
}

GPU_API_PRE void GPU_API_POST reiiCommandStaticMeshDraw(gpu_handle_context_t context, ReiiHandleCommandList * list, ReiiHandleStaticMesh * staticMesh) {
  const char * optionalFile = NULL;
  int optionalLine = 0;
  REDGPU_2_EXPECT(0 || !"TODO");
}

GPU_API_PRE void GPU_API_POST reiiCommandStaticMeshDrawInstanced(gpu_handle_context_t context, ReiiHandleCommandList * list, ReiiHandleStaticMesh * staticMesh, unsigned vertexCount, unsigned instanceCount, unsigned vertexFirst, unsigned instanceFirst) {
  const char * optionalFile = NULL;
  int optionalLine = 0;
  REDGPU_2_EXPECT(0 || !"TODO");
}

GPU_API_PRE void GPU_API_POST reiiCreateStaticMesh(gpu_handle_context_t context, gpu_extra_reii_create_static_mesh_parameters_t * parameters, ReiiHandleStaticMesh * outStaticMesh) {
  const char * optionalFile = NULL;
  int optionalLine = 0;
  REDGPU_2_EXPECT(0 || !"TODO");
}

GPU_API_PRE void GPU_API_POST reiiStaticMeshSet(gpu_handle_context_t context, ReiiHandleStaticMesh * staticMesh) {
  const char * optionalFile = NULL;
  int optionalLine = 0;
  REDGPU_2_EXPECT(0 || !"TODO");
}

GPU_API_PRE void GPU_API_POST reiiStaticMeshEnd(gpu_handle_context_t context, ReiiHandleStaticMesh * staticMesh) {
  const char * optionalFile = NULL;
  int optionalLine = 0;
  REDGPU_2_EXPECT(0 || !"TODO");
}

GPU_API_PRE void GPU_API_POST reiiStaticMeshTexcoord(gpu_handle_context_t context, ReiiHandleStaticMesh * staticMesh, unsigned index, float x, float y, float z, float w) {
  const char * optionalFile = NULL;
  int optionalLine = 0;
  REDGPU_2_EXPECT(0 || !"TODO");
}

GPU_API_PRE void GPU_API_POST reiiStaticMeshColor(gpu_handle_context_t context, ReiiHandleStaticMesh * staticMesh, float r, float g, float b, float a) {
  const char * optionalFile = NULL;
  int optionalLine = 0;
  REDGPU_2_EXPECT(0 || !"TODO");
}

GPU_API_PRE void GPU_API_POST reiiStaticMeshNormal(gpu_handle_context_t context, ReiiHandleStaticMesh * staticMesh, float x, float y, float z) {
  const char * optionalFile = NULL;
  int optionalLine = 0;
  REDGPU_2_EXPECT(0 || !"TODO");
}

GPU_API_PRE void GPU_API_POST reiiStaticMeshPosition(gpu_handle_context_t context, ReiiHandleStaticMesh * staticMesh, float x, float y, float z, float w) {
  const char * optionalFile = NULL;
  int optionalLine = 0;
  REDGPU_2_EXPECT(0 || !"TODO");
}

GPU_API_PRE void GPU_API_POST reiiDestroyEx(gpu_handle_context_t context, gpu_extra_reii_destroy_type_e destroyHandleType, void * destroyHandle) {
  const char * optionalFile = NULL;
  int optionalLine = 0;

  vf_handle_context_t * vkfast = (vf_handle_context_t *)(void *)context;

  RedHandleGpu gpu = vkfast->gpu;

  if (destroyHandleType == GPU_EXTRA_REII_DESTROY_TYPE_UNDEFINED) {
    REDGPU_2_EXPECT(0 || !"destroyHandleType == GPU_EXTRA_REII_DESTROY_TYPE_UNDEFINED (0)");
  } else if (destroyHandleType == GPU_EXTRA_REII_DESTROY_TYPE_MESH_STATE) {
    ReiiMeshState * handle = (ReiiMeshState *)destroyHandle;

    RedHandleGpuCode                       gpuCodeVertex       = handle->gpuCodeVertex;
    RedHandleGpuCode                       gpuCodeFragment     = handle->gpuCodeFragment;
    Red2ProcedureParametersAndDeclarations procedureParameters = handle->procedureParameters;
    RedHandleProcedure                     procedure           = handle->procedure;

    np(red2DestroyHandle,
      "context", vkfast->context,
      "gpu", vkfast->gpu,
      "handleType", RED_HANDLE_TYPE_GPU_CODE,
      "handle", gpuCodeVertex,
      "optionalHandle2", NULL,
      "optionalFile", optionalFile,
      "optionalLine", optionalLine,
      "optionalUserData", NULL
    );

    np(red2DestroyHandle,
      "context", vkfast->context,
      "gpu", vkfast->gpu,
      "handleType", RED_HANDLE_TYPE_GPU_CODE,
      "handle", gpuCodeFragment,
      "optionalHandle2", NULL,
      "optionalFile", optionalFile,
      "optionalLine", optionalLine,
      "optionalUserData", NULL
    );

    np(red2DestroyHandle,
      "context", vkfast->context,
      "gpu", vkfast->gpu,
      "handleType", RED_HANDLE_TYPE_PROCEDURE_PARAMETERS,
      "handle", procedureParameters.procedureParameters,
      "optionalHandle2", NULL,
      "optionalFile", optionalFile,
      "optionalLine", optionalLine,
      "optionalUserData", NULL
    );

    for (int i = 0; i < 7; i += 1) {
      np(red2DestroyHandle,
        "context", vkfast->context,
        "gpu", vkfast->gpu,
        "handleType", RED_HANDLE_TYPE_STRUCT_DECLARATION,
        "handle", procedureParameters.structsDeclarations[i],
        "optionalHandle2", NULL,
        "optionalFile", optionalFile,
        "optionalLine", optionalLine,
        "optionalUserData", NULL
      );
    }

    np(red2DestroyHandle,
      "context", vkfast->context,
      "gpu", vkfast->gpu,
      "handleType", RED_HANDLE_TYPE_STRUCT_DECLARATION,
      "handle", procedureParameters.handlesDeclaration,
      "optionalHandle2", NULL,
      "optionalFile", optionalFile,
      "optionalLine", optionalLine,
      "optionalUserData", NULL
    );

    np(red2DestroyHandle,
      "context", vkfast->context,
      "gpu", vkfast->gpu,
      "handleType", RED_HANDLE_TYPE_PROCEDURE,
      "handle", procedure,
      "optionalHandle2", NULL,
      "optionalFile", optionalFile,
      "optionalLine", optionalLine,
      "optionalUserData", NULL
    );
  } else if (destroyHandleType == GPU_EXTRA_REII_DESTROY_TYPE_TEXTURE) {
    ReiiHandleTexture * handle = (ReiiHandleTexture *)destroyHandle;

    np(red2DestroyHandle,
      "context", vkfast->context,
      "gpu", vkfast->gpu,
      "handleType", RED_HANDLE_TYPE_SAMPLER,
      "handle", handle->sampler,
      "optionalHandle2", NULL,
      "optionalFile", optionalFile,
      "optionalLine", optionalLine,
      "optionalUserData", NULL
    );
    np(red2DestroyHandle,
      "context", vkfast->context,
      "gpu", vkfast->gpu,
      "handleType", RED_HANDLE_TYPE_TEXTURE,
      "handle", handle->texture,
      "optionalHandle2", NULL,
      "optionalFile", optionalFile,
      "optionalLine", optionalLine,
      "optionalUserData", NULL
    );
    np(red2DestroyHandle,
      "context", vkfast->context,
      "gpu", vkfast->gpu,
      "handleType", RED_HANDLE_TYPE_TEXTURE,
      "handle", handle->textureDepthOnly,
      "optionalHandle2", NULL,
      "optionalFile", optionalFile,
      "optionalLine", optionalLine,
      "optionalUserData", NULL
    );
    np(red2DestroyHandle,
      "context", vkfast->context,
      "gpu", vkfast->gpu,
      "handleType", RED_HANDLE_TYPE_TEXTURE,
      "handle", handle->textureStencilOnly,
      "optionalHandle2", NULL,
      "optionalFile", optionalFile,
      "optionalLine", optionalLine,
      "optionalUserData", NULL
    );
    for (int i = 0; i < 6; i += 1) {
      np(red2DestroyHandle,
        "context", vkfast->context,
        "gpu", vkfast->gpu,
        "handleType", RED_HANDLE_TYPE_TEXTURE,
        "handle", handle->textureCubeFace[i],
        "optionalHandle2", NULL,
        "optionalFile", optionalFile,
        "optionalLine", optionalLine,
        "optionalUserData", NULL
      );
    }
    np(red2DestroyHandle,
      "context", vkfast->context,
      "gpu", vkfast->gpu,
      "handleType", RED_HANDLE_TYPE_IMAGE,
      "handle", handle->image.handle,
      "optionalHandle2", NULL,
      "optionalFile", optionalFile,
      "optionalLine", optionalLine,
      "optionalUserData", NULL
    );
    np(red2DestroyHandle,
      "context", vkfast->context,
      "gpu", vkfast->gpu,
      "handleType", RED_HANDLE_TYPE_MEMORY,
      "handle", handle->imageDedicatedMemory,
      "optionalHandle2", NULL,
      "optionalFile", optionalFile,
      "optionalLine", optionalLine,
      "optionalUserData", NULL
    );
  } else if (destroyHandleType == GPU_EXTRA_REII_DESTROY_TYPE_TEXTURE_MEMORY) {
    ReiiHandleTextureMemory * handle = (ReiiHandleTextureMemory *)destroyHandle;

    np(red2DestroyHandle,
      "context", vkfast->context,
      "gpu", vkfast->gpu,
      "handleType", RED_HANDLE_TYPE_MEMORY,
      "handle", handle->memory,
      "optionalHandle2", NULL,
      "optionalFile", optionalFile,
      "optionalLine", optionalLine,
      "optionalUserData", NULL
    );
  } else if (destroyHandleType == GPU_EXTRA_REII_DESTROY_TYPE_COMMAND_LIST) {
    ReiiHandleCommandList * handle = (ReiiHandleCommandList *)destroyHandle;

    {
      unsigned outputsCount = handle->mutable_outputs_array.count;
      for (unsigned i = 0; i < outputsCount; i += 1) {
        red2DestroyHandle(vkfast->context, vkfast->gpu, RED_HANDLE_TYPE_OUTPUT, handle->mutable_outputs_array.items[i].handle, NULL, optionalFile, optionalLine, NULL);
      }
      for (unsigned i = 0; i < outputsCount; i += 1) {
        red2DestroyHandle(vkfast->context, vkfast->gpu, RED_HANDLE_TYPE_OUTPUT_DECLARATION, handle->mutable_outputs_array.items[i].handleDeclaration, NULL, optionalFile, optionalLine, NULL);
      }
    }
  } else if (destroyHandleType == GPU_EXTRA_REII_DESTROY_TYPE_STATIC_MESH) {
    REDGPU_2_EXPECT(0 || !"TODO");
  } else {
    REDGPU_2_EXPECT(0 || !"Invalid enum value");
  }
}

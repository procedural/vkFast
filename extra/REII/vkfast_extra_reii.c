#include "../../vkfast_ex.h"
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

#include <stdio.h> // For _popen, _pclose

#define REII_INTERNAL_MAX_SAMPLERS_COUNT 4000

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

GPU_API_PRE void GPU_API_POST reiiMeshStateCompile(gpu_handle_context_t context, ReiiMeshState * state) {
  const char * optionalFile = NULL;
  int optionalLine = 0;

  vf_handle_context_t * vkfast = (vf_handle_context_t *)(void *)context;
  RedHandleGpu gpu = vkfast->gpu;

  REDGPU_2_EXPECTWG(state->compileInfo.state_multisample_count != 0);
  if (state->compileInfo.output_depth_stencil_enable == 1) {
    REDGPU_2_EXPECTWG(state->compileInfo.output_depth_stencil_format != RED_FORMAT_UNDEFINED);
    // NOTE(Constantine): Check for the only supported texture formats for now.
    REDGPU_2_EXPECTWG(
      state->compileInfo.output_depth_stencil_format == RED_FORMAT_DEPTH_32_FLOAT ||
      state->compileInfo.output_depth_stencil_format == RED_FORMAT_DEPTH_24_UINT_TO_FLOAT_0_1_STENCIL_8_UINT
    );
  }
  REDGPU_2_EXPECTWG(state->compileInfo.output_color_format != RED_FORMAT_UNDEFINED);
  // NOTE(Constantine): Check for the only supported texture formats for now.
  REDGPU_2_EXPECTWG(
    state->compileInfo.output_color_format == RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1
  );
  REDGPU_2_EXPECTWG(state->compileInfo.samplers_count <= REII_INTERNAL_MAX_SAMPLERS_COUNT);

  // To destroy
  RedHandleGpuCode gpuCodeVertex = NULL;
  np(redCreateGpuCode,
    "context", vkfast->context,
    "gpu", vkfast->gpu,
    "handleName", state->programVertex.optional_debug_name,
    "irBytesCount", state->programVertex.program_binary_bytes_count,
    "ir", (const void *)state->programVertex.program_binary,
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
    "handleName", state->programFragment.optional_debug_name,
    "irBytesCount", state->programFragment.program_binary_bytes_count,
    "ir", (const void *)state->programFragment.program_binary,
    "outGpuCode", &gpuCodeFragment,
    "outStatuses", NULL,
    "optionalFile", optionalFile,
    "optionalLine", optionalLine,
    "optionalUserData", NULL
  );
  REDGPU_2_EXPECTWG(gpuCodeFragment != NULL);

  if (state->compileInfo.variables_bytes_count > 0) {
    for (unsigned i = 0; i < state->compileInfo.struct_members_count; i += 1) {
      REDGPU_2_EXPECTWG(state->compileInfo.variables_slot != state->compileInfo.struct_members[i].slot);
    }
  }

  Red2ProcedureParametersDeclaration parameters = {0};
  parameters.variablesSlot            = state->compileInfo.variables_slot;
  parameters.variablesVisibleToStages = state->compileInfo.variables_bytes_count == 0 ? 0 : (RED_VISIBLE_TO_STAGE_BITFLAG_VERTEX | RED_VISIBLE_TO_STAGE_BITFLAG_FRAGMENT);
  parameters.variablesBytesCount      = state->compileInfo.variables_bytes_count;
  parameters.structsDeclarationsCount = (state->compileInfo.struct_members_count == 0 ? 0 : 1) + (state->compileInfo.samplers_count == 0 ? 0 : 1);
  parameters.structsDeclarations[0].structDeclarationMembersCount        = state->compileInfo.struct_members_count;
  parameters.structsDeclarations[0].structDeclarationMembers             = state->compileInfo.struct_members;
  parameters.structsDeclarations[0].structDeclarationMembersArrayROCount = 0;
  parameters.structsDeclarations[0].structDeclarationMembersArrayRO      = NULL;
  RedStructDeclarationMember samplers[REII_INTERNAL_MAX_SAMPLERS_COUNT] = {0}; // NOTE(Constantine): Kinda big on stack size, but whatever.
  if (state->compileInfo.samplers_count > 0) {
    for (unsigned i = 0; i < state->compileInfo.samplers_count; i += 1) {
      samplers[i].slot            = i;
      samplers[i].type            = RED_STRUCT_MEMBER_TYPE_SAMPLER;
      samplers[i].count           = 1;
      samplers[i].visibleToStages = RED_VISIBLE_TO_STAGE_BITFLAG_FRAGMENT; // NOTE(Constantine): I doubt anyone needs to sample textures in vertex shaders?
    }
    parameters.structsDeclarations[1].structDeclarationMembersCount        = state->compileInfo.samplers_count;
    parameters.structsDeclarations[1].structDeclarationMembers             = samplers;
    parameters.structsDeclarations[1].structDeclarationMembersArrayROCount = 0;
    parameters.structsDeclarations[1].structDeclarationMembersArrayRO      = NULL;
  }

  // To destroy
  Red2ProcedureParametersAndDeclarations procedureParameters = {0};
  np(red2CreateProcedureParameters,
    "context", vkfast->context,
    "gpu", vkfast->gpu,
    "handleName", state->compileInfo.optional_debug_name,
    "procedureParametersDeclaration", &parameters,
    "outProcedureParametersAndDeclarations", &procedureParameters,
    "outStatuses", NULL,
    "optionalFile", optionalFile,
    "optionalLine", optionalLine,
    "optionalUserData", NULL
  );
  REDGPU_2_EXPECTWG(procedureParameters.procedureParameters != NULL);

  RedOutputDeclarationMembers outputs = {0};
  outputs.depthStencilEnable                        = state->compileInfo.output_depth_stencil_enable;
  outputs.depthStencilFormat                        = state->compileInfo.output_depth_stencil_format;
  outputs.depthStencilMultisampleCount              = state->compileInfo.state_multisample_count;
  outputs.depthStencilDepthSetProcedureOutputOp     = RED_SET_PROCEDURE_OUTPUT_OP_PRESERVE;
  outputs.depthStencilDepthEndProcedureOutputOp     = RED_END_PROCEDURE_OUTPUT_OP_PRESERVE;
  outputs.depthStencilStencilSetProcedureOutputOp   = RED_SET_PROCEDURE_OUTPUT_OP_PRESERVE;
  outputs.depthStencilStencilEndProcedureOutputOp   = RED_END_PROCEDURE_OUTPUT_OP_PRESERVE;
  outputs.depthStencilSharesMemoryWithAnotherMember = 0;
  outputs.colorsCount                               = 1;
  outputs.colorsFormat[0]                           = state->compileInfo.output_color_format;
  outputs.colorsFormat[1]                           = RED_FORMAT_UNDEFINED;
  outputs.colorsFormat[2]                           = RED_FORMAT_UNDEFINED;
  outputs.colorsFormat[3]                           = RED_FORMAT_UNDEFINED;
  outputs.colorsFormat[4]                           = RED_FORMAT_UNDEFINED;
  outputs.colorsFormat[5]                           = RED_FORMAT_UNDEFINED;
  outputs.colorsFormat[6]                           = RED_FORMAT_UNDEFINED;
  outputs.colorsFormat[7]                           = RED_FORMAT_UNDEFINED;
  outputs.colorsMultisampleCount[0]                 = state->compileInfo.state_multisample_count;
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
  procstate.rasterizationDepthClampEnable                  = state->rasterizationDepthClampEnable;
  procstate.rasterizationDiscardAllPrimitivesEnable        = 0;
  procstate.rasterizationCullMode                          = ReiiCullModeToRed(state->rasterizationCullMode);
  procstate.rasterizationFrontFace                         = ReiiFrontFaceToRed(state->rasterizationFrontFace);
  procstate.rasterizationDepthBiasEnable                   = state->rasterizationDepthBiasEnable;
  procstate.rasterizationDepthBiasDynamic                  = 0;
  procstate.rasterizationDepthBiasStaticConstantFactor     = state->rasterizationDepthBiasConstantFactor;
  procstate.rasterizationDepthBiasStaticClamp              = 0;
  procstate.rasterizationDepthBiasStaticSlopeFactor        = state->rasterizationDepthBiasSlopeFactor;
  procstate.multisampleCount                               = state->compileInfo.state_multisample_count;
  procstate.multisampleSampleMask                          = NULL;
  procstate.multisampleSampleShadingEnable                 = 0;
  procstate.multisampleSampleShadingMin                    = 0;
  procstate.multisampleAlphaToCoverageEnable               = state->multisampleAlphaToCoverageEnable;
  procstate.multisampleAlphaToOneEnable                    = state->multisampleAlphaToOneEnable;
  procstate.depthTestEnable                                = state->depthTestEnable;
  procstate.depthTestDepthWriteEnable                      = state->depthTestDepthWriteEnable;
  procstate.depthTestDepthCompareOp                        = ReiiCompareOpToRed(state->depthTestDepthCompareOp);
  procstate.depthTestBoundsTestEnable                      = 0;
  procstate.depthTestBoundsTestDynamic                     = 0;
  procstate.depthTestBoundsTestStaticMin                   = 0;
  procstate.depthTestBoundsTestStaticMax                   = 0;
  procstate.stencilTestEnable                              = state->stencilTestEnable;
  procstate.stencilTestFrontAndBackDynamicCompareMask      = 0;
  procstate.stencilTestFrontAndBackDynamicWriteMask        = 0;
  procstate.stencilTestFrontAndBackDynamicReference        = 0;
  procstate.stencilTestFrontStencilTestFailOp              = ReiiStencilOpToRed(state->stencilTestFrontStencilTestFailOp);
  procstate.stencilTestFrontStencilTestPassDepthTestPassOp = ReiiStencilOpToRed(state->stencilTestFrontStencilTestPassDepthTestPassOp);
  procstate.stencilTestFrontStencilTestPassDepthTestFailOp = ReiiStencilOpToRed(state->stencilTestFrontStencilTestPassDepthTestFailOp);
  procstate.stencilTestFrontCompareOp                      = ReiiCompareOpToRed(state->stencilTestFrontCompareOp);
  procstate.stencilTestBackStencilTestFailOp               = ReiiStencilOpToRed(state->stencilTestBackStencilTestFailOp);
  procstate.stencilTestBackStencilTestPassDepthTestPassOp  = ReiiStencilOpToRed(state->stencilTestBackStencilTestPassDepthTestPassOp);
  procstate.stencilTestBackStencilTestPassDepthTestFailOp  = ReiiStencilOpToRed(state->stencilTestBackStencilTestPassDepthTestFailOp);
  procstate.stencilTestBackCompareOp                       = ReiiCompareOpToRed(state->stencilTestBackCompareOp);
  procstate.stencilTestFrontAndBackDynamicCompareMask      = 0;
  procstate.stencilTestFrontAndBackDynamicWriteMask        = 0;
  procstate.stencilTestFrontAndBackDynamicReference        = 0;
  procstate.stencilTestFrontAndBackStaticCompareMask       = state->stencilTestFrontAndBackCompareMask;
  procstate.stencilTestFrontAndBackStaticWriteMask         = state->stencilTestFrontAndBackWriteMask;
  procstate.stencilTestFrontAndBackStaticReference         = state->stencilTestFrontAndBackReference;
  procstate.blendLogicOpEnable                             = state->blendLogicOpEnable;
  procstate.blendLogicOp                                   = ReiiLogicOpToRed(state->blendLogicOp);
  procstate.blendConstantsDynamic                          = 0;
  procstate.blendConstantsStatic[0]                        = state->blendConstants[0];
  procstate.blendConstantsStatic[1]                        = state->blendConstants[1];
  procstate.blendConstantsStatic[2]                        = state->blendConstants[2];
  procstate.blendConstantsStatic[3]                        = state->blendConstants[3];
  procstate.outputColorsCount                              = 1;
  procstate.outputColorsWriteMask[0]                       = (state->outputColorWriteEnableR ? RED_COLOR_COMPONENT_BITFLAG_R : 0) | (state->outputColorWriteEnableG ? RED_COLOR_COMPONENT_BITFLAG_G : 0) | (state->outputColorWriteEnableB ? RED_COLOR_COMPONENT_BITFLAG_B : 0) | (state->outputColorWriteEnableA ? RED_COLOR_COMPONENT_BITFLAG_A : 0);
  procstate.outputColorsWriteMask[1]                       = 0;
  procstate.outputColorsWriteMask[2]                       = 0;
  procstate.outputColorsWriteMask[3]                       = 0;
  procstate.outputColorsWriteMask[4]                       = 0;
  procstate.outputColorsWriteMask[5]                       = 0;
  procstate.outputColorsWriteMask[6]                       = 0;
  procstate.outputColorsWriteMask[7]                       = 0;
  procstate.outputColorsBlendEnable[0]                     = state->outputColorBlendEnable;
  procstate.outputColorsBlendEnable[1]                     = 0;
  procstate.outputColorsBlendEnable[2]                     = 0;
  procstate.outputColorsBlendEnable[3]                     = 0;
  procstate.outputColorsBlendEnable[4]                     = 0;
  procstate.outputColorsBlendEnable[5]                     = 0;
  procstate.outputColorsBlendEnable[6]                     = 0;
  procstate.outputColorsBlendEnable[7]                     = 0;
  procstate.outputColorsBlendColorFactorSource[0]          = ReiiBlendFactorToRed(state->outputColorBlendColorFactorSource);
  procstate.outputColorsBlendColorFactorSource[1]          = RED_BLEND_FACTOR_ZERO;
  procstate.outputColorsBlendColorFactorSource[2]          = RED_BLEND_FACTOR_ZERO;
  procstate.outputColorsBlendColorFactorSource[3]          = RED_BLEND_FACTOR_ZERO;
  procstate.outputColorsBlendColorFactorSource[4]          = RED_BLEND_FACTOR_ZERO;
  procstate.outputColorsBlendColorFactorSource[5]          = RED_BLEND_FACTOR_ZERO;
  procstate.outputColorsBlendColorFactorSource[6]          = RED_BLEND_FACTOR_ZERO;
  procstate.outputColorsBlendColorFactorSource[7]          = RED_BLEND_FACTOR_ZERO;
  procstate.outputColorsBlendColorFactorTarget[0]          = ReiiBlendFactorToRed(state->outputColorBlendColorFactorTarget);
  procstate.outputColorsBlendColorFactorTarget[1]          = RED_BLEND_FACTOR_ZERO;
  procstate.outputColorsBlendColorFactorTarget[2]          = RED_BLEND_FACTOR_ZERO;
  procstate.outputColorsBlendColorFactorTarget[3]          = RED_BLEND_FACTOR_ZERO;
  procstate.outputColorsBlendColorFactorTarget[4]          = RED_BLEND_FACTOR_ZERO;
  procstate.outputColorsBlendColorFactorTarget[5]          = RED_BLEND_FACTOR_ZERO;
  procstate.outputColorsBlendColorFactorTarget[6]          = RED_BLEND_FACTOR_ZERO;
  procstate.outputColorsBlendColorFactorTarget[7]          = RED_BLEND_FACTOR_ZERO;
  procstate.outputColorsBlendColorOp[0]                    = ReiiBlendOpToRed(state->outputColorBlendColorOp);
  procstate.outputColorsBlendColorOp[1]                    = RED_BLEND_OP_ADD;
  procstate.outputColorsBlendColorOp[2]                    = RED_BLEND_OP_ADD;
  procstate.outputColorsBlendColorOp[3]                    = RED_BLEND_OP_ADD;
  procstate.outputColorsBlendColorOp[4]                    = RED_BLEND_OP_ADD;
  procstate.outputColorsBlendColorOp[5]                    = RED_BLEND_OP_ADD;
  procstate.outputColorsBlendColorOp[6]                    = RED_BLEND_OP_ADD;
  procstate.outputColorsBlendColorOp[7]                    = RED_BLEND_OP_ADD;
  procstate.outputColorsBlendAlphaFactorSource[0]          = ReiiBlendFactorToRed(state->outputColorBlendAlphaFactorSource);
  procstate.outputColorsBlendAlphaFactorSource[1]          = RED_BLEND_FACTOR_ZERO;
  procstate.outputColorsBlendAlphaFactorSource[2]          = RED_BLEND_FACTOR_ZERO;
  procstate.outputColorsBlendAlphaFactorSource[3]          = RED_BLEND_FACTOR_ZERO;
  procstate.outputColorsBlendAlphaFactorSource[4]          = RED_BLEND_FACTOR_ZERO;
  procstate.outputColorsBlendAlphaFactorSource[5]          = RED_BLEND_FACTOR_ZERO;
  procstate.outputColorsBlendAlphaFactorSource[6]          = RED_BLEND_FACTOR_ZERO;
  procstate.outputColorsBlendAlphaFactorSource[7]          = RED_BLEND_FACTOR_ZERO;
  procstate.outputColorsBlendAlphaFactorTarget[0]          = ReiiBlendFactorToRed(state->outputColorBlendAlphaFactorTarget);
  procstate.outputColorsBlendAlphaFactorTarget[1]          = RED_BLEND_FACTOR_ZERO;
  procstate.outputColorsBlendAlphaFactorTarget[2]          = RED_BLEND_FACTOR_ZERO;
  procstate.outputColorsBlendAlphaFactorTarget[3]          = RED_BLEND_FACTOR_ZERO;
  procstate.outputColorsBlendAlphaFactorTarget[4]          = RED_BLEND_FACTOR_ZERO;
  procstate.outputColorsBlendAlphaFactorTarget[5]          = RED_BLEND_FACTOR_ZERO;
  procstate.outputColorsBlendAlphaFactorTarget[6]          = RED_BLEND_FACTOR_ZERO;
  procstate.outputColorsBlendAlphaFactorTarget[7]          = RED_BLEND_FACTOR_ZERO;
  procstate.outputColorsBlendAlphaOp[0]                    = ReiiBlendOpToRed(state->outputColorBlendAlphaOp);
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
    "handleName", state->compileInfo.optional_debug_name,
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
  state->gpuCodeVertex       = gpuCodeVertex;
  state->gpuCodeFragment     = gpuCodeFragment;
  state->procedureParameters = procedureParameters;
  state->procedure           = procedure;
}

GPU_API_PRE void GPU_API_POST reiiMeshStateRecompileEx(gpu_handle_context_t context, ReiiMeshState * state, const char * compileCommandVS, const char * compileCommandFS, const wchar_t * compiledSpvFilepathVS, const wchar_t * compiledSpvFilepathFS) {
  const char * optionalFile = NULL;
  int optionalLine = 0;

  vf_handle_context_t * vkfast = (vf_handle_context_t *)(void *)context;

  RedHandleGpu gpu = vkfast->gpu;

  reiiDestroyEx(context, GPU_EXTRA_REII_DESTROY_TYPE_MESH_STATE, state);
  Red2ProcedureParametersAndDeclarations defaults = {0};
  state->gpuCodeVertex       = NULL;
  state->gpuCodeFragment     = NULL;
  state->procedureParameters = defaults;
  state->procedure           = NULL;

  {
    FILE * reiiMeshStateRecompileEx_compileCommandVS_popen = _popen(compileCommandVS, "r");
    REDGPU_2_EXPECTWG(reiiMeshStateRecompileEx_compileCommandVS_popen != NULL);
    while (!feof(reiiMeshStateRecompileEx_compileCommandVS_popen)) {
      fgetc(reiiMeshStateRecompileEx_compileCommandVS_popen);
    }
    _pclose(reiiMeshStateRecompileEx_compileCommandVS_popen);
  }

  {
    FILE * reiiMeshStateRecompileEx_compileCommandFS_popen = _popen(compileCommandFS, "r");
    REDGPU_2_EXPECTWG(reiiMeshStateRecompileEx_compileCommandFS_popen != NULL);
    while (!feof(reiiMeshStateRecompileEx_compileCommandFS_popen)) {
      fgetc(reiiMeshStateRecompileEx_compileCommandFS_popen);
    }
    _pclose(reiiMeshStateRecompileEx_compileCommandFS_popen);
  }

  void * vs_fh   = (void *)-1;
  void * vs_fmap = (void *)-1;
  size_t vs_spv_bytes_count = 0;
  void * vs_spv  = NULL;
  REDGPU_2_EXPECTWG(0 == red32FileMap(compiledSpvFilepathVS, &vs_fh, &vs_fmap, &vs_spv_bytes_count, &vs_spv));

  void * fs_fh   = (void *)-1;
  void * fs_fmap = (void *)-1;
  size_t fs_spv_bytes_count = 0;
  void * fs_spv  = NULL;
  REDGPU_2_EXPECTWG(0 == red32FileMap(compiledSpvFilepathFS, &fs_fh, &fs_fmap, &fs_spv_bytes_count, &fs_spv));

  state->programVertex.program_binary_bytes_count   = vs_spv_bytes_count;
  state->programVertex.program_binary               = vs_spv;
  state->programFragment.program_binary_bytes_count = fs_spv_bytes_count;
  state->programFragment.program_binary             = fs_spv;

  reiiMeshStateCompile(context, state);

  REDGPU_2_EXPECTWG(0 == red32FileUnmap(vs_fh, vs_fmap, vs_spv));
  REDGPU_2_EXPECTWG(0 == red32FileUnmap(fs_fh, fs_fmap, fs_spv));
}

GPU_API_PRE RedHandleSampler GPU_API_POST reiiCreateSampler(gpu_handle_context_t context, const char * optionalDebugName, ReiiSamplerFiltering magFiltering, ReiiSamplerFiltering minFiltering, ReiiSamplerBehaviorOutsideTextureCoordinate behaviorOutsideTextureCoordinateU, ReiiSamplerBehaviorOutsideTextureCoordinate behaviorOutsideTextureCoordinateV, int maxAnisotropy) {
  const char * optionalFile = NULL;
  int optionalLine = 0;

  vf_handle_context_t * vkfast = (vf_handle_context_t *)(void *)context;

  RedHandleGpu gpu = vkfast->gpu;

  REDGPU_2_EXPECTWG(magFiltering == REII_SAMPLER_FILTERING_NEAREST || magFiltering == REII_SAMPLER_FILTERING_LINEAR);
  REDGPU_2_EXPECTWG(
    minFiltering == REII_SAMPLER_FILTERING_NEAREST             || minFiltering == REII_SAMPLER_FILTERING_LINEAR             ||
    minFiltering == REII_SAMPLER_FILTERING_NEAREST_MIP_NEAREST || minFiltering == REII_SAMPLER_FILTERING_NEAREST_MIP_LINEAR ||
    minFiltering == REII_SAMPLER_FILTERING_LINEAR_MIP_NEAREST  || minFiltering == REII_SAMPLER_FILTERING_LINEAR_MIP_LINEAR
  );

  RedSamplerFiltering    filteringMag = RiiSamplerFilteringToRed(magFiltering);
  RedSamplerFiltering    filteringMin = RiiSamplerFilteringToRed(minFiltering);
  RedSamplerFilteringMip filteringMip = RiiSamplerFilteringMipToRed(minFiltering);

  RedHandleSampler sampler = NULL;
  // To destroy
  np(redCreateSampler,
    "context", vkfast->context,
    "gpu", vkfast->gpu,
    "handleName", optionalDebugName,
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

  return sampler;
}

GPU_API_PRE void GPU_API_POST reiiCreateTextureMemory(gpu_handle_context_t context, gpu_extra_reii_texture_type texturesType, uint64_t bytesCount, ReiiHandleTextureMemory * outTextureMemory) {
  const char * optionalFile = NULL;
  int optionalLine = 0;

  vf_handle_context_t * vkfast = (vf_handle_context_t *)(void *)context;
  RedHandleGpu gpu = vkfast->gpu;

  REDGPU_2_EXPECT(outTextureMemory->texturesType == GPU_EXTRA_REII_TEXTURE_TYPE_INVALID || !"Texture memory's type is not zero, are you passing an existing texture memory accidentally?");

  const char * optional_debug_name = outTextureMemory->optional_debug_name;
  ReiiHandleTextureMemory clear = {0};
  outTextureMemory[0] = clear;

  RedBool32 texturesUseTheirOwnDedicatedMemory = 0;
  RedHandleMemory memory = NULL;

  if (texturesType == GPU_EXTRA_REII_TEXTURE_TYPE_GENERAL) {
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
  outTextureMemory->texturesType = texturesType;
  outTextureMemory->texturesUseTheirOwnDedicatedMemory = texturesUseTheirOwnDedicatedMemory;
  outTextureMemory->bytesCount   = bytesCount;
  outTextureMemory->memory       = memory;
  outTextureMemory->bytesOffset  = 0;
}

GPU_API_PRE void GPU_API_POST reiiResetTextureMemory(gpu_handle_context_t context, ReiiHandleTextureMemory * textureMemory) {
  const char * optionalFile = NULL;
  int optionalLine = 0;

  textureMemory->bytesOffset = 0;
}

GPU_API_PRE uint64_t GPU_API_POST reiiGetTextureMemoryBytesOffset(ReiiHandleTextureMemory * textureMemory) {
  return textureMemory->bytesOffset;
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

  bindingTexture->msaaCount = msaaCount;
}

GPU_API_PRE void GPU_API_POST reiiTextureDefineEx(gpu_handle_context_t context, ReiiTextureBinding binding, ReiiHandleTexture * bindingTexture, int bindingLevel, ReiiTextureTexelFormat bindingTexelFormat, int width, int height, ReiiTextureTexelFormat texelsFormat, ReiiTextureTexelType texelsType, int texelsBytesAlignment) {
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
  if (textureType == GPU_EXTRA_REII_TEXTURE_TYPE_GENERAL && bindingTexture->mipLevelsCount > 1) {
    REDGPU_2_EXPECTWG(width  == w_power);
    REDGPU_2_EXPECTWG(height == h_power);
    REDGPU_2_EXPECTWG(width  == height);
  }
  REDGPU_2_EXPECTWG(bindingTexture->generateMipLevels == 0 || !"Mipmaps are not supported by REII.");
  REDGPU_2_EXPECTWG(bindingTexture->mipLevelsCount == 1 || !"Mipmaps are not supported by REII.");
  //REDGPU_2_EXPECTWG(bindingTexture->mipLevelsCount >= 1);
  //REDGPU_2_EXPECTWG(bindingTexture->mipLevelsCount <= ((int)log2(width) + 1));

  RedFormat  format = RED_FORMAT_UNDEFINED;
  ReiiBool32 formatHasStencil = 0;
  // NOTE(Constantine): Check for the only supported texture formats for now.
  if (textureType == GPU_EXTRA_REII_TEXTURE_TYPE_GENERAL) {
    REDGPU_2_EXPECTWG(bindingTexelFormat == REII_TEXTURE_TEXEL_FORMAT_RGBA);
    REDGPU_2_EXPECTWG(texelsFormat       == REII_TEXTURE_TEXEL_FORMAT_RGBA);
    REDGPU_2_EXPECTWG(texelsType         == REII_TEXTURE_TEXEL_TYPE_U8);
    format = RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1;
  } else if (textureType == GPU_EXTRA_REII_TEXTURE_TYPE_OUTPUT_COLOR || textureType == GPU_EXTRA_REII_TEXTURE_TYPE_OUTPUT_COLOR_MSAA) {
    REDGPU_2_EXPECTWG(bindingTexelFormat == REII_TEXTURE_TEXEL_FORMAT_RGBA);
    REDGPU_2_EXPECTWG(texelsFormat       == REII_TEXTURE_TEXEL_FORMAT_RGBA);
    REDGPU_2_EXPECTWG(texelsType         == REII_TEXTURE_TEXEL_TYPE_U8);
    format = RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1;
  } else if (textureType == GPU_EXTRA_REII_TEXTURE_TYPE_OUTPUT_DEPTH_STENCIL || textureType == GPU_EXTRA_REII_TEXTURE_TYPE_OUTPUT_DEPTH_STENCIL_MSAA) {
    REDGPU_2_EXPECTWG(bindingTexelFormat == REII_TEXTURE_TEXEL_FORMAT_DS);
    REDGPU_2_EXPECTWG(texelsFormat       == REII_TEXTURE_TEXEL_FORMAT_DS);
    REDGPU_2_EXPECTWG(
      texelsType == REII_TEXTURE_TEXEL_TYPE_FLOAT  ||
      texelsType == REII_TEXTURE_TEXEL_TYPE_U24_U8 ||
      texelsType == REII_TEXTURE_TEXEL_TYPE_F32_U8
    );
    if (texelsType == REII_TEXTURE_TEXEL_TYPE_FLOAT) {
      format = RED_FORMAT_DEPTH_32_FLOAT;
      formatHasStencil = 0;
    } else if (texelsType == REII_TEXTURE_TEXEL_TYPE_U24_U8) {
      format = RED_FORMAT_DEPTH_24_UINT_TO_FLOAT_0_1_STENCIL_8_UINT;
      formatHasStencil = 1;
    } else if (texelsType == REII_TEXTURE_TEXEL_TYPE_F32_U8) {
      format = RED_FORMAT_DEPTH_32_FLOAT_STENCIL_8_UINT;
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

      REDGPU_2_EXPECTWG((bindingTexture->textureMemory->bytesOffset + image.memoryBytesCount) <= bindingTexture->textureMemory->bytesCount);

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
      bindingTexture->textureMemory->bytesOffset += image.memoryBytesCount;
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

GPU_API_PRE void GPU_API_POST reiiBatchImageSetUsableStateEx(gpu_handle_context_t context, uint64_t batchId, RedHandleImage image, RedImagePartBitflags imageAllParts) {
  vf_handle_context_t * vkfast = (vf_handle_context_t *)(void *)context;
  RedHandleGpu gpu = vkfast->gpu;

  vf_handle_t * batch = (vf_handle_t *)(void *)batchId;

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
  imageUsage.imageAllParts          = imageAllParts;
  imageUsage.imageLevelsFirst       = 0;
  imageUsage.imageLevelsCount       = -1;
  imageUsage.imageLayersFirst       = 0;
  imageUsage.imageLayersCount       = -1;
  np(redCallUsageAliasOrderBarrier,
    "address", batch->batch.addresses.redCallUsageAliasOrderBarrier,
    "calls", batch->batch.calls.handle,
    "context", vkfast->context,
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
}

GPU_API_PRE void GPU_API_POST reiiBatchImageCopyFromCpuEx(gpu_handle_context_t context, uint64_t batchId, RedHandleImage image, RedImagePartBitflags imageAllParts, int bindingLevel, int bindingLayer, int bindingX, int bindingY, int width, int height, const ReiiCpuScratchBuffer * texels) {
  const char * optionalFile = NULL;
  int optionalLine = 0;

  vf_handle_context_t * vkfast = (vf_handle_context_t *)(void *)context;
  RedHandleGpu gpu = vkfast->gpu;

  REDGPU_2_EXPECTWG(texels != NULL);

  vf_handle_t * batch = (vf_handle_t *)(void *)batchId;

  RedCopyArrayImageRange copy = {0};
  copy.arrayBytesFirst               = texels->cpu_scratch_buffer.arrayRangeBytesFirst;
  copy.arrayTexelsCountToNextRow     = width;
  copy.arrayTexelsCountToNextLayerOr3DDepthSliceDividedByTexelsCountToNextRow = 0;
  copy.imageParts.allParts           = imageAllParts;
  copy.imageParts.level              = bindingLevel;
  copy.imageParts.layersFirst        = bindingLayer;
  copy.imageParts.layersCount        = 1;
  copy.imageOffset.texelX            = bindingX;
  copy.imageOffset.texelY            = bindingY;
  copy.imageOffset.texelZ            = 0;
  copy.imageExtent.texelsCountWidth  = width;
  copy.imageExtent.texelsCountHeight = height;
  copy.imageExtent.texelsCountDepth  = 1;
  npfp(redCallCopyArrayToImage, batch->batch.addresses.redCallCopyArrayToImage,
    "calls", batch->batch.calls.handle,
    "arrayR", texels->cpu_scratch_buffer.array,
    "imageW", image,
    "setTo1", 1,
    "rangesCount", 1,
    "ranges", &copy
  );
}

GPU_API_PRE void GPU_API_POST reiiTextureDefineAndCopyFromCpuEx(gpu_handle_context_t context, ReiiTextureBinding binding, ReiiHandleTexture * bindingTexture, int bindingLevel, ReiiTextureTexelFormat bindingTexelFormat, int width, int height, ReiiTextureTexelFormat texelsFormat, ReiiTextureTexelType texelsType, int texelsBytesAlignment, const ReiiCpuScratchBuffer * texels, unsigned queueFamilyIndexToSubmitCopyCommands, RedHandleQueue queueToSubmitCopyCommands) {
  const char * optionalFile = NULL;
  int optionalLine = 0;

  vf_handle_context_t * vkfast = (vf_handle_context_t *)(void *)context;
  RedHandleGpu gpu = vkfast->gpu;

  const gpu_extra_reii_texture_type textureType = bindingTexture->textureMemory->texturesType;

  if (textureType != GPU_EXTRA_REII_TEXTURE_TYPE_GENERAL) {
    REDGPU_2_EXPECTWG(texels == NULL || !"textureType != GPU_EXTRA_REII_TEXTURE_TYPE_GENERAL");
  }

  RedImagePartBitflags imageParts = RED_IMAGE_PART_BITFLAG_COLOR;
  if (
    textureType == GPU_EXTRA_REII_TEXTURE_TYPE_OUTPUT_DEPTH_STENCIL ||
    textureType == GPU_EXTRA_REII_TEXTURE_TYPE_OUTPUT_DEPTH_STENCIL_MSAA
  )
  {
    // NOTE(Constantine): Assumes, a stencil-only image is not possible.
    imageParts = RED_IMAGE_PART_BITFLAG_DEPTH | (bindingTexture->textureStencilOnly != NULL ? RED_IMAGE_PART_BITFLAG_STENCIL : 0);
  }

  unsigned bindingLayer = 0;
  if      (binding == REII_TEXTURE_BINDING_CUBE_POSITIVE_X) { bindingLayer = 0; }
  else if (binding == REII_TEXTURE_BINDING_CUBE_NEGATIVE_X) { bindingLayer = 1; }
  else if (binding == REII_TEXTURE_BINDING_CUBE_POSITIVE_Y) { bindingLayer = 2; }
  else if (binding == REII_TEXTURE_BINDING_CUBE_NEGATIVE_Y) { bindingLayer = 3; }
  else if (binding == REII_TEXTURE_BINDING_CUBE_POSITIVE_Z) { bindingLayer = 4; }
  else if (binding == REII_TEXTURE_BINDING_CUBE_NEGATIVE_Z) { bindingLayer = 5; }

  reiiTextureDefineEx(context, binding, bindingTexture, bindingLevel, bindingTexelFormat, width, height, texelsFormat, texelsType, texelsBytesAlignment);

  uint64_t batch = vfBatchBeginEx(context, 0, NULL, queueFamilyIndexToSubmitCopyCommands, NULL, optionalFile, optionalLine);

  reiiBatchImageSetUsableStateEx(context, batch, bindingTexture->image.handle, imageParts);
  if (texels != NULL) {
    reiiBatchImageCopyFromCpuEx(context, batch, bindingTexture->image.handle, imageParts, bindingLevel, bindingLayer, 0, 0, width, height, texels);
  }

  vfBatchEnd(context, batch, optionalFile, optionalLine);
  uint64_t async = vfAsyncBatchExecuteEx(context, queueToSubmitCopyCommands, 1, &batch, optionalFile, optionalLine);
  vfAsyncWaitToFinish(context, async, optionalFile, optionalLine);
  vfIdDestroy(1, &batch, optionalFile, optionalLine);
}

GPU_API_PRE void GPU_API_POST reiiTextureCopyFromCpuEx(gpu_handle_context_t context, ReiiTextureBinding binding, ReiiHandleTexture * bindingTexture, int bindingLevel, int bindingX, int bindingY, int width, int height, ReiiTextureTexelFormat texelsFormat, ReiiTextureTexelType texelsType, int texelsBytesAlignment, const ReiiCpuScratchBuffer * texels, unsigned queueFamilyIndexToSubmitCopyCommands, RedHandleQueue queueToSubmitCopyCommands) {
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

  RedImagePartBitflags imageParts = RED_IMAGE_PART_BITFLAG_COLOR;
  if (
    textureType == GPU_EXTRA_REII_TEXTURE_TYPE_OUTPUT_DEPTH_STENCIL ||
    textureType == GPU_EXTRA_REII_TEXTURE_TYPE_OUTPUT_DEPTH_STENCIL_MSAA
  )
  {
    // NOTE(Constantine): Assumes, a stencil-only image is not possible.
    imageParts = RED_IMAGE_PART_BITFLAG_DEPTH | (bindingTexture->textureStencilOnly != NULL ? RED_IMAGE_PART_BITFLAG_STENCIL : 0);
  }

  unsigned bindingLayer = 0;
  if      (binding == REII_TEXTURE_BINDING_CUBE_POSITIVE_X) { bindingLayer = 0; }
  else if (binding == REII_TEXTURE_BINDING_CUBE_NEGATIVE_X) { bindingLayer = 1; }
  else if (binding == REII_TEXTURE_BINDING_CUBE_POSITIVE_Y) { bindingLayer = 2; }
  else if (binding == REII_TEXTURE_BINDING_CUBE_NEGATIVE_Y) { bindingLayer = 3; }
  else if (binding == REII_TEXTURE_BINDING_CUBE_POSITIVE_Z) { bindingLayer = 4; }
  else if (binding == REII_TEXTURE_BINDING_CUBE_NEGATIVE_Z) { bindingLayer = 5; }

  uint64_t batch = vfBatchBeginEx(context, 0, NULL, queueFamilyIndexToSubmitCopyCommands, NULL, optionalFile, optionalLine);

  reiiBatchImageCopyFromCpuEx(context, batch, bindingTexture->image.handle, imageParts, bindingLevel, bindingLayer, 0, 0, width, height, texels);

  vfBatchEnd(context, batch, optionalFile, optionalLine);
  uint64_t async = vfAsyncBatchExecuteEx(context, queueToSubmitCopyCommands, 1, &batch, optionalFile, optionalLine);
  vfAsyncWaitToFinish(context, async, optionalFile, optionalLine);
  vfIdDestroy(1, &batch, optionalFile, optionalLine);
}

GPU_API_PRE void GPU_API_POST reiiTextureDefineAndCopyFromCpu(gpu_handle_context_t context, ReiiTextureBinding binding, ReiiHandleTexture * bindingTexture, int bindingLevel, ReiiTextureTexelFormat bindingTexelFormat, int width, int height, ReiiTextureTexelFormat texelsFormat, ReiiTextureTexelType texelsType, int texelsBytesAlignment, const ReiiCpuScratchBuffer * texels) {
  vf_handle_context_t * vkfast = (vf_handle_context_t *)(void *)context;
  RedHandleGpu gpu = vkfast->gpu;

  reiiTextureDefineAndCopyFromCpuEx(context, binding, bindingTexture, bindingLevel, bindingTexelFormat, width, height, texelsFormat, texelsType, texelsBytesAlignment, texels, vkfast->mainQueueFamilyIndex, vkfast->mainQueue);
}

GPU_API_PRE void GPU_API_POST reiiTextureCopyFromCpu(gpu_handle_context_t context, ReiiTextureBinding binding, ReiiHandleTexture * bindingTexture, int bindingLevel, int bindingX, int bindingY, int width, int height, ReiiTextureTexelFormat texelsFormat, ReiiTextureTexelType texelsType, int texelsBytesAlignment, const ReiiCpuScratchBuffer * texels) {
  vf_handle_context_t * vkfast = (vf_handle_context_t *)(void *)context;
  RedHandleGpu gpu = vkfast->gpu;

  reiiTextureCopyFromCpuEx(context, binding, bindingTexture, bindingLevel, bindingX, bindingY, width, height, texelsFormat, texelsType, texelsBytesAlignment, texels, vkfast->mainQueueFamilyIndex, vkfast->mainQueue);
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

GPU_API_PRE void GPU_API_POST reiiCommandBindSamplers(gpu_handle_context_t context, ReiiHandleCommandList * list, unsigned samplersCount, RedHandleSampler * samplers) {
  const char * optionalFile = NULL;
  int optionalLine = 0;

  if (samplersCount == 0) {
    return;
  }

  vf_handle_t * batch = (vf_handle_t *)(void *)list->batch_id;
  vf_handle_context_t * vkfast = (vf_handle_context_t *)(void *)context;
  RedHandleGpu gpu = vkfast->gpu;

  REDGPU_2_EXPECTWG(batch->batch.currentStructSamplers.handle != NULL || !"vfBatchBegin()::batch_bindings_info was set to NULL? Or vfBatchBegin()::batch_bindings_info::max_sampler_binds_count was set to 0?");
  REDGPU_2_EXPECTWG(list->currentProcedureParametersDraw != NULL || !"reiiCommandBindSamplers() needs at least one reiiCommandMeshSetState() call made before it.");
  REDGPU_2_EXPECTWG(samplersCount <= REII_INTERNAL_MAX_SAMPLERS_COUNT);

  RedStructMemberTexture membersSamplers[REII_INTERNAL_MAX_SAMPLERS_COUNT] = {0}; // NOTE(Constantine): Kinda big on stack size, but whatever.
  RedStructMember        members[REII_INTERNAL_MAX_SAMPLERS_COUNT]         = {0}; // NOTE(Constantine): Kinda big on stack size, but whatever.
  for (unsigned i = 0; i < samplersCount; i += 1) {
    membersSamplers[i].sampler = samplers[i];
    membersSamplers[i].texture = NULL;
    membersSamplers[i].setTo1  = 1;

    members[i].setTo35   = 35;
    members[i].setTo0    = 0;
    members[i].structure = batch->batch.currentStructSamplers.handle;
    members[i].slot      = i;
    members[i].first     = 0;
    members[i].count     = 1;
    members[i].type      = RED_STRUCT_MEMBER_TYPE_SAMPLER;
    members[i].textures  = &membersSamplers[i];
    members[i].arrays    = NULL;
    members[i].setTo00   = 0;
  }
  np(redStructsSet,
    "context", vkfast->context,
    "gpu", vkfast->gpu,
    "structsMembersCount", samplersCount,
    "structsMembers", members,
    "optionalFile", optionalFile,
    "optionalLine", optionalLine,
    "optionalUserData", NULL
  );

  npfp(redCallSetProcedureParametersStructs, batch->batch.addresses.redCallSetProcedureParametersStructs,
    "calls", batch->batch.calls.handle,
    "procedureType", RED_PROCEDURE_TYPE_DRAW,
    "procedureParameters", list->currentProcedureParametersDraw,
    "procedureParametersDeclarationStructsDeclarationsFirst", 1,
    "structsCount", 1,
    "structs", &batch->batch.currentStructSamplers.handle,
    "setTo0", 0,
    "setTo00", 0
  );
}

GPU_API_PRE void GPU_API_POST reiiCommandBindNewBindingsSet(gpu_handle_context_t context, ReiiHandleCommandList * list, int slotsCount, const RedStructDeclarationMember * slots) {
  const char * optionalFile = NULL;
  int optionalLine = 0;

  vf_handle_t * batch = (vf_handle_t *)(void *)list->batch_id;
  vf_handle_context_t * vkfast = (vf_handle_context_t *)(void *)context;
  RedHandleGpu gpu = vkfast->gpu;

  REDGPU_2_EXPECTWG(batch->batch.structsMemory != NULL || !"vfBatchBegin()::batch_bindings_info was set to NULL?");
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
  REDGPU_2_EXPECTWG(structure.handleDeclaration != NULL);
  REDGPU_2_EXPECTWG(structure.handle != NULL || !"red2StructsMemorySuballocateStruct() call returned NULL. Ran out of vfBatchBegin()::batch_bindings_info::max_new_bindings_sets_count and all the other vfBatchBegin()::batch_bindings_info::max_* memory to allocate?");
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
  vf_handle_context_t * vkfast = (vf_handle_context_t *)(void *)context;
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

GPU_API_PRE void GPU_API_POST reiiCommandBindTextureRW(gpu_handle_context_t context, ReiiHandleCommandList * list, int slot, int texturesRWCount, const RedStructMemberTexture * texturesRW) {
  const char * optionalFile = NULL;
  int optionalLine = 0;

  vf_handle_t * batch = (vf_handle_t *)(void *)list->batch_id;
  vf_handle_context_t * vkfast = (vf_handle_context_t *)(void *)context;
  RedHandleGpu gpu = vkfast->gpu;

  REDGPU_2_EXPECTWG(batch->batch.currentStruct.handle != NULL || !"Was reiiCommandBindNewBindingsSet() ever called previously?");

  RedStructMember member = {0};
  member.setTo35   = 35;
  member.setTo0    = 0;
  member.structure = batch->batch.currentStruct.handle;
  member.slot      = slot;
  member.first     = 0;
  member.count     = texturesRWCount;
  member.type      = RED_STRUCT_MEMBER_TYPE_TEXTURE_RW;
  member.textures  = texturesRW;
  member.arrays    = NULL;
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

GPU_API_PRE void GPU_API_POST reiiCommandBindTextureRO(gpu_handle_context_t context, ReiiHandleCommandList * list, int slot, int texturesROCount, const RedStructMemberTexture * texturesRO) {
  const char * optionalFile = NULL;
  int optionalLine = 0;

  vf_handle_t * batch = (vf_handle_t *)(void *)list->batch_id;
  vf_handle_context_t * vkfast = (vf_handle_context_t *)(void *)context;
  RedHandleGpu gpu = vkfast->gpu;

  REDGPU_2_EXPECTWG(batch->batch.currentStruct.handle != NULL || !"Was reiiCommandBindNewBindingsSet() ever called previously?");

  RedStructMember member = {0};
  member.setTo35   = 35;
  member.setTo0    = 0;
  member.structure = batch->batch.currentStruct.handle;
  member.slot      = slot;
  member.first     = 0;
  member.count     = texturesROCount;
  member.type      = RED_STRUCT_MEMBER_TYPE_TEXTURE_RO;
  member.textures  = texturesRO;
  member.arrays    = NULL;
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
  vf_handle_context_t * vkfast = (vf_handle_context_t *)(void *)context;
  RedHandleGpu gpu = vkfast->gpu;

  npfp(redCallSetProcedureParametersStructs, batch->batch.addresses.redCallSetProcedureParametersStructs,
    "calls", batch->batch.calls.handle,
    "procedureType", RED_PROCEDURE_TYPE_DRAW,
    "procedureParameters", list->currentProcedureParametersDraw,
    "procedureParametersDeclarationStructsDeclarationsFirst", 0,
    "structsCount", 1,
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
  vf_handle_context_t * vkfast = (vf_handle_context_t *)(void *)context;
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
  vf_handle_context_t * vkfast = (vf_handle_context_t *)(void *)context;
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
  REDGPU_2_EXPECTWG(list->mutable_outputs_array.items[list->mutable_outputs_array.count-1].handle != NULL);
  REDGPU_2_EXPECTWG(list->mutable_outputs_array.items[list->mutable_outputs_array.count-1].handleDeclaration != NULL);
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

GPU_API_PRE void GPU_API_POST reiiCommandMeshEndExact(gpu_handle_context_t context, ReiiHandleCommandList * list, ReiiHandleTexture * depthStencilTexture, ReiiHandleTexture * colorTexture, RedHandleTexture colorTextureHandle) {
  const char * optionalFile = NULL;
  int optionalLine = 0;

  vf_handle_t * batch = (vf_handle_t *)(void *)list->batch_id;
  vf_handle_context_t * vkfast = (vf_handle_context_t *)(void *)context;
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

GPU_API_PRE void GPU_API_POST reiiCommandMeshEndWithTale64BytesAlign(gpu_handle_context_t context, ReiiHandleCommandList * list, ReiiHandleTexture * depthStencilTexture, ReiiHandleTexture * colorTexture, RedHandleTexture colorTextureHandle) {
  const char * optionalFile = NULL;
  int optionalLine = 0;

  reiiCommandMeshEndExact(context, list, depthStencilTexture, colorTexture, colorTextureHandle);

  while (REDGPU_2_BYTES_TO_NEXT_ALIGNMENT_BOUNDARY(list->dynamicMeshPositionVec4Offset * sizeof(ReiiVec4), 64) > 0) {
    if ((list->dynamicMeshPositionVec4Offset * sizeof(ReiiVec4)) >= list->dynamic_mesh_position.cpu.arrayRangeBytesCount) {
      break;
    }
    list->dynamicMeshPositionVec4Offset += 1;
  }
  while (REDGPU_2_BYTES_TO_NEXT_ALIGNMENT_BOUNDARY(list->dynamicMeshColorVec4Offset * sizeof(ReiiVec4), 64) > 0) {
    if ((list->dynamicMeshColorVec4Offset * sizeof(ReiiVec4)) >= list->dynamic_mesh_color.cpu.arrayRangeBytesCount) {
      break;
    }
    list->dynamicMeshColorVec4Offset += 1;
  }
  while (REDGPU_2_BYTES_TO_NEXT_ALIGNMENT_BOUNDARY(list->dynamicMeshNormalVec4Offset * sizeof(ReiiVec4), 64) > 0) {
    if ((list->dynamicMeshNormalVec4Offset * sizeof(ReiiVec4)) >= list->dynamic_mesh_normal.cpu.arrayRangeBytesCount) {
      break;
    }
    list->dynamicMeshNormalVec4Offset += 1;
  }
  for (int i = 0; i < REII_TEXCOORDS_MAX_COUNT; i += 1) {
    while (REDGPU_2_BYTES_TO_NEXT_ALIGNMENT_BOUNDARY(list->dynamicMeshTexcoordVec4Offset[i] * sizeof(ReiiVec4), 64) > 0) {
      if ((list->dynamicMeshTexcoordVec4Offset[i] * sizeof(ReiiVec4)) >= list->dynamic_mesh_texcoord[i].cpu.arrayRangeBytesCount) {
        break;
      }
      list->dynamicMeshTexcoordVec4Offset[i] += 1;
    }
  }
}

GPU_API_PRE void GPU_API_POST reiiCommandMeshTexcoord(gpu_handle_context_t context, ReiiHandleCommandList * list, unsigned index, float x, float y, float z, float w) {
  const char * optionalFile = NULL;
  int optionalLine = 0;

  REDGPU_2_EXPECT(index < REII_TEXCOORDS_MAX_COUNT);

#ifndef NDEBUG
  {
    const uint64_t bytesNeeded = (list->dynamicMeshTexcoordVec4Offset[index] + 1) * sizeof(ReiiVec4);
    const uint64_t bytesTotal  = list->dynamic_mesh_texcoord[index].cpu.arrayRangeBytesCount;
    REDGPU_2_EXPECT(bytesNeeded <= bytesTotal || !"(list->dynamicMeshTexcoordVec4Offset + 1) * sizeof(ReiiVec4) <= list->dynamic_mesh_texcoord[index].cpu.arrayRangeBytesCount");
  }
#endif

  volatile ReiiVec4 * cpu_as_vec4_start = (volatile ReiiVec4 *)list->dynamic_mesh_texcoord[index].cpu_ptr;
  volatile ReiiVec4 * cpu_as_vec4       = &cpu_as_vec4_start[list->dynamicMeshTexcoordVec4Offset[index]];

  cpu_as_vec4[0].x = x;
  cpu_as_vec4[0].y = y;
  cpu_as_vec4[0].z = z;
  cpu_as_vec4[0].w = w;

  list->dynamicMeshTexcoordVec4Offset[index] += 1;
}

GPU_API_PRE void GPU_API_POST reiiCommandMeshColor(gpu_handle_context_t context, ReiiHandleCommandList * list, float r, float g, float b, float a) {
  const char * optionalFile = NULL;
  int optionalLine = 0;

#ifndef NDEBUG
  {
    const uint64_t bytesNeeded = (list->dynamicMeshColorVec4Offset + 1) * sizeof(ReiiVec4);
    const uint64_t bytesTotal  = list->dynamic_mesh_color.cpu.arrayRangeBytesCount;
    REDGPU_2_EXPECT(bytesNeeded <= bytesTotal || !"(list->dynamicMeshColorVec4Offset + 1) * sizeof(ReiiVec4) <= list->dynamic_mesh_color.cpu.arrayRangeBytesCount");
  }
#endif

  volatile ReiiVec4 * cpu_as_vec4_start = (volatile ReiiVec4 *)list->dynamic_mesh_color.cpu_ptr;
  volatile ReiiVec4 * cpu_as_vec4       = &cpu_as_vec4_start[list->dynamicMeshColorVec4Offset];

  cpu_as_vec4[0].x = r;
  cpu_as_vec4[0].y = g;
  cpu_as_vec4[0].z = b;
  cpu_as_vec4[0].w = a;

  list->dynamicMeshColorVec4Offset += 1;
}

GPU_API_PRE void GPU_API_POST reiiCommandMeshNormal(gpu_handle_context_t context, ReiiHandleCommandList * list, float x, float y, float z) {
  const char * optionalFile = NULL;
  int optionalLine = 0;

#ifndef NDEBUG
  {
    const uint64_t bytesNeeded = (list->dynamicMeshNormalVec4Offset + 1) * sizeof(ReiiVec4);
    const uint64_t bytesTotal  = list->dynamic_mesh_normal.cpu.arrayRangeBytesCount;
    REDGPU_2_EXPECT(bytesNeeded <= bytesTotal || !"(list->dynamicMeshNormalVec4Offset + 1) * sizeof(ReiiVec4) <= list->dynamic_mesh_normal.cpu.arrayRangeBytesCount");
  }
#endif

  volatile ReiiVec4 * cpu_as_vec4_start = (volatile ReiiVec4 *)list->dynamic_mesh_normal.cpu_ptr;
  volatile ReiiVec4 * cpu_as_vec4       = &cpu_as_vec4_start[list->dynamicMeshNormalVec4Offset];

  cpu_as_vec4[0].x = x;
  cpu_as_vec4[0].y = y;
  cpu_as_vec4[0].z = z;
  cpu_as_vec4[0].w = 1; // NOTE(Constantine): Hmm... I guess?

  list->dynamicMeshNormalVec4Offset += 1;
}

GPU_API_PRE void GPU_API_POST reiiCommandMeshPosition(gpu_handle_context_t context, ReiiHandleCommandList * list, float x, float y, float z, float w) {
  const char * optionalFile = NULL;
  int optionalLine = 0;

#ifndef NDEBUG
  {
    const uint64_t bytesNeeded = (list->dynamicMeshPositionVec4Offset + 1) * sizeof(ReiiVec4);
    const uint64_t bytesTotal  = list->dynamic_mesh_position.cpu.arrayRangeBytesCount;
    REDGPU_2_EXPECT(bytesNeeded <= bytesTotal || !"(list->dynamicMeshPositionVec4Offset + 1) * sizeof(ReiiVec4) <= list->dynamic_mesh_position.cpu.arrayRangeBytesCount");
  }
#endif

  volatile ReiiVec4 * cpu_as_vec4_start = (volatile ReiiVec4 *)list->dynamic_mesh_position.cpu_ptr;
  volatile ReiiVec4 * cpu_as_vec4       = &cpu_as_vec4_start[list->dynamicMeshPositionVec4Offset];

  cpu_as_vec4[0].x = x;
  cpu_as_vec4[0].y = y;
  cpu_as_vec4[0].z = z;
  cpu_as_vec4[0].w = w;

  list->dynamicMeshPositionVec4Offset += 1;
}

GPU_API_PRE void GPU_API_POST reiiCommandResolveMsaaColorTexture(gpu_handle_context_t context, ReiiHandleCommandList * list, ReiiHandleTexture * sourceMsaaColorTexture, ReiiHandleTexture * targetColorTexture) {
  const char * optionalFile = NULL;
  int optionalLine = 0;

  vf_handle_t * batch = (vf_handle_t *)(void *)list->batch_id;
  vf_handle_context_t * vkfast = (vf_handle_context_t *)(void *)context;
  RedHandleGpu gpu = vkfast->gpu;

  REDGPU_2_EXPECTWG(sourceMsaaColorTexture->width  == targetColorTexture->width);
  REDGPU_2_EXPECTWG(sourceMsaaColorTexture->height == targetColorTexture->height);

  RedOutputDeclarationMembers outputDeclarationMembers = {0};
  outputDeclarationMembers.depthStencilEnable                        = 0;
  outputDeclarationMembers.depthStencilFormat                        = RED_FORMAT_UNDEFINED;
  outputDeclarationMembers.depthStencilMultisampleCount              = RED_MULTISAMPLE_COUNT_BITFLAG_1;
  outputDeclarationMembers.depthStencilDepthSetProcedureOutputOp     = RED_SET_PROCEDURE_OUTPUT_OP_PRESERVE;
  outputDeclarationMembers.depthStencilDepthEndProcedureOutputOp     = RED_END_PROCEDURE_OUTPUT_OP_PRESERVE;
  outputDeclarationMembers.depthStencilStencilSetProcedureOutputOp   = RED_SET_PROCEDURE_OUTPUT_OP_PRESERVE;
  outputDeclarationMembers.depthStencilStencilEndProcedureOutputOp   = RED_END_PROCEDURE_OUTPUT_OP_PRESERVE;
  outputDeclarationMembers.depthStencilSharesMemoryWithAnotherMember = 0;
  outputDeclarationMembers.colorsCount                               = 1;
  outputDeclarationMembers.colorsFormat[0]                           = sourceMsaaColorTexture->format;
  outputDeclarationMembers.colorsMultisampleCount[0]                 = sourceMsaaColorTexture->msaaCount;
  outputDeclarationMembers.colorsSetProcedureOutputOp[0]             = RED_SET_PROCEDURE_OUTPUT_OP_PRESERVE;
  outputDeclarationMembers.colorsEndProcedureOutputOp[0]             = RED_END_PROCEDURE_OUTPUT_OP_PRESERVE;
  outputDeclarationMembers.colorsSharesMemoryWithAnotherMember[0]    = 0;
  
  RedOutputDeclarationMembersResolveSources outputDeclarationMembersResolveSources = {0};
  outputDeclarationMembersResolveSources.resolveModeDepth    = RED_RESOLVE_MODE_NONE;
  outputDeclarationMembersResolveSources.resolveModeStencil  = RED_RESOLVE_MODE_NONE;
  outputDeclarationMembersResolveSources.resolveDepthStencil = 0;
  outputDeclarationMembersResolveSources.resolveColors       = 1;

  RedOutputMembers outputMembers = {0};
  outputMembers.colorsCount = 1;
  outputMembers.colors[0]   = sourceMsaaColorTexture->texture;

  RedOutputMembersResolveTargets outputMembersResolveTargets = {0};
  outputMembersResolveTargets.colors[0] = targetColorTexture->texture;

  np(red2CallSetProcedureOutput,
    "address", list->callProceduresAndAddresses.redCallSetProcedureOutput,
    "calls", batch->batch.calls.handle,
    "context", vkfast->context,
    "gpu", vkfast->gpu,
    "mutableOutputsArray", &list->mutable_outputs_array,
    "outputDeclarationMembers", &outputDeclarationMembers,
    "outputDeclarationMembersResolveSources", &outputDeclarationMembersResolveSources,
    "dependencyByRegion", 0,
    "dependencyByRegionAllowUsageAliasOrderBarriers", 0,
    "outputMembers", &outputMembers,
    "outputMembersResolveTargets", &outputMembersResolveTargets,
    "width", targetColorTexture->width,
    "height", targetColorTexture->height,
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

  np(redCallEndProcedureOutput,
    "address", list->callProceduresAndAddresses.redCallEndProcedureOutput,
    "calls", batch->batch.calls.handle
  );
}

GPU_API_PRE void GPU_API_POST reiiCommandGammaCorrectColorTextureToTheInversePowerOf2(gpu_handle_context_t context, ReiiHandleCommandList * list, ReiiHandleTexture * colorTexture, int doDoubleGammaCorrection, int doSwapRedAndBlue, ReiiGammaCorrectColorTextureToTheInversePowerOf2StaticState * state) {
  const char * optionalFile = NULL;
  int optionalLine = 0;

  uint64_t batch_id = list->batch_id;

  if (state->programCompute == 0) {
    const unsigned char g_main[] = {
      0x03, 0x02, 0x23, 0x07, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x0e, 0x00, 0x48, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x11, 0x00, 0x02, 0x00,
      0x01, 0x00, 0x00, 0x00, 0x0b, 0x00, 0x06, 0x00, 0x01, 0x00, 0x00, 0x00, 0x47, 0x4c, 0x53, 0x4c, 0x2e, 0x73, 0x74, 0x64, 0x2e, 0x34, 0x35, 0x30,
      0x00, 0x00, 0x00, 0x00, 0x0e, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x06, 0x00, 0x05, 0x00, 0x00, 0x00,
      0x02, 0x00, 0x00, 0x00, 0x6d, 0x61, 0x69, 0x6e, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x10, 0x00, 0x06, 0x00, 0x02, 0x00, 0x00, 0x00,
      0x11, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x03, 0x00, 0x03, 0x00, 0x05, 0x00, 0x00, 0x00,
      0x58, 0x02, 0x00, 0x00, 0x05, 0x00, 0x06, 0x00, 0x04, 0x00, 0x00, 0x00, 0x74, 0x79, 0x70, 0x65, 0x2e, 0x32, 0x64, 0x2e, 0x69, 0x6d, 0x61, 0x67,
      0x65, 0x00, 0x00, 0x00, 0x05, 0x00, 0x04, 0x00, 0x05, 0x00, 0x00, 0x00, 0x69, 0x6d, 0x61, 0x67, 0x65, 0x00, 0x00, 0x00, 0x05, 0x00, 0x0a, 0x00,
      0x06, 0x00, 0x00, 0x00, 0x74, 0x79, 0x70, 0x65, 0x2e, 0x43, 0x6f, 0x6e, 0x73, 0x74, 0x61, 0x6e, 0x74, 0x42, 0x75, 0x66, 0x66, 0x65, 0x72, 0x2e,
      0x56, 0x61, 0x72, 0x69, 0x61, 0x62, 0x6c, 0x65, 0x73, 0x00, 0x00, 0x00, 0x06, 0x00, 0x05, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x69, 0x6d, 0x61, 0x67, 0x65, 0x5f, 0x77, 0x00, 0x06, 0x00, 0x05, 0x00, 0x06, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x69, 0x6d, 0x61, 0x67,
      0x65, 0x5f, 0x68, 0x00, 0x06, 0x00, 0x09, 0x00, 0x06, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x64, 0x6f, 0x75, 0x62, 0x6c, 0x65, 0x5f, 0x67,
      0x61, 0x6d, 0x6d, 0x61, 0x5f, 0x63, 0x6f, 0x72, 0x72, 0x65, 0x63, 0x74, 0x69, 0x6f, 0x6e, 0x00, 0x06, 0x00, 0x07, 0x00, 0x06, 0x00, 0x00, 0x00,
      0x03, 0x00, 0x00, 0x00, 0x73, 0x77, 0x61, 0x70, 0x5f, 0x72, 0x5f, 0x61, 0x6e, 0x64, 0x5f, 0x62, 0x00, 0x00, 0x00, 0x00, 0x05, 0x00, 0x05, 0x00,
      0x07, 0x00, 0x00, 0x00, 0x76, 0x61, 0x72, 0x69, 0x61, 0x62, 0x6c, 0x65, 0x73, 0x00, 0x00, 0x00, 0x05, 0x00, 0x04, 0x00, 0x02, 0x00, 0x00, 0x00,
      0x6d, 0x61, 0x69, 0x6e, 0x00, 0x00, 0x00, 0x00, 0x47, 0x00, 0x04, 0x00, 0x03, 0x00, 0x00, 0x00, 0x0b, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x00, 0x00,
      0x47, 0x00, 0x04, 0x00, 0x05, 0x00, 0x00, 0x00, 0x22, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x47, 0x00, 0x04, 0x00, 0x05, 0x00, 0x00, 0x00,
      0x21, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x48, 0x00, 0x05, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x23, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x48, 0x00, 0x05, 0x00, 0x06, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x23, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00,
      0x48, 0x00, 0x05, 0x00, 0x06, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x23, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x48, 0x00, 0x05, 0x00,
      0x06, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x23, 0x00, 0x00, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x47, 0x00, 0x03, 0x00, 0x06, 0x00, 0x00, 0x00,
      0x02, 0x00, 0x00, 0x00, 0x15, 0x00, 0x04, 0x00, 0x08, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x2b, 0x00, 0x04, 0x00,
      0x08, 0x00, 0x00, 0x00, 0x09, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x2b, 0x00, 0x04, 0x00, 0x08, 0x00, 0x00, 0x00, 0x0a, 0x00, 0x00, 0x00,
      0x01, 0x00, 0x00, 0x00, 0x2b, 0x00, 0x04, 0x00, 0x08, 0x00, 0x00, 0x00, 0x0b, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x2b, 0x00, 0x04, 0x00,
      0x08, 0x00, 0x00, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x14, 0x00, 0x02, 0x00, 0x0d, 0x00, 0x00, 0x00, 0x2a, 0x00, 0x03, 0x00,
      0x0d, 0x00, 0x00, 0x00, 0x0e, 0x00, 0x00, 0x00, 0x16, 0x00, 0x03, 0x00, 0x0f, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x19, 0x00, 0x09, 0x00,
      0x04, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x02, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x20, 0x00, 0x04, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00,
      0x1e, 0x00, 0x06, 0x00, 0x06, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00,
      0x20, 0x00, 0x04, 0x00, 0x11, 0x00, 0x00, 0x00, 0x09, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x15, 0x00, 0x04, 0x00, 0x12, 0x00, 0x00, 0x00,
      0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x17, 0x00, 0x04, 0x00, 0x13, 0x00, 0x00, 0x00, 0x12, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00,
      0x20, 0x00, 0x04, 0x00, 0x14, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x13, 0x00, 0x00, 0x00, 0x13, 0x00, 0x02, 0x00, 0x15, 0x00, 0x00, 0x00,
      0x21, 0x00, 0x03, 0x00, 0x16, 0x00, 0x00, 0x00, 0x15, 0x00, 0x00, 0x00, 0x17, 0x00, 0x04, 0x00, 0x17, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x00,
      0x04, 0x00, 0x00, 0x00, 0x20, 0x00, 0x04, 0x00, 0x18, 0x00, 0x00, 0x00, 0x09, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x17, 0x00, 0x04, 0x00,
      0x19, 0x00, 0x00, 0x00, 0x12, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x17, 0x00, 0x04, 0x00, 0x1a, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x00,
      0x03, 0x00, 0x00, 0x00, 0x3b, 0x00, 0x04, 0x00, 0x10, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3b, 0x00, 0x04, 0x00,
      0x11, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x09, 0x00, 0x00, 0x00, 0x3b, 0x00, 0x04, 0x00, 0x14, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00,
      0x01, 0x00, 0x00, 0x00, 0x2e, 0x00, 0x03, 0x00, 0x17, 0x00, 0x00, 0x00, 0x1b, 0x00, 0x00, 0x00, 0x20, 0x00, 0x04, 0x00, 0x1c, 0x00, 0x00, 0x00,
      0x01, 0x00, 0x00, 0x00, 0x12, 0x00, 0x00, 0x00, 0x36, 0x00, 0x05, 0x00, 0x15, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x16, 0x00, 0x00, 0x00, 0xf8, 0x00, 0x02, 0x00, 0x1d, 0x00, 0x00, 0x00, 0x3d, 0x00, 0x04, 0x00, 0x13, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x00, 0x00,
      0x03, 0x00, 0x00, 0x00, 0x41, 0x00, 0x05, 0x00, 0x18, 0x00, 0x00, 0x00, 0x1f, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x09, 0x00, 0x00, 0x00,
      0x3d, 0x00, 0x04, 0x00, 0x08, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x1f, 0x00, 0x00, 0x00, 0x41, 0x00, 0x05, 0x00, 0x18, 0x00, 0x00, 0x00,
      0x21, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x0a, 0x00, 0x00, 0x00, 0x3d, 0x00, 0x04, 0x00, 0x08, 0x00, 0x00, 0x00, 0x22, 0x00, 0x00, 0x00,
      0x21, 0x00, 0x00, 0x00, 0x41, 0x00, 0x05, 0x00, 0x18, 0x00, 0x00, 0x00, 0x23, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x0b, 0x00, 0x00, 0x00,
      0x3d, 0x00, 0x04, 0x00, 0x08, 0x00, 0x00, 0x00, 0x24, 0x00, 0x00, 0x00, 0x23, 0x00, 0x00, 0x00, 0x41, 0x00, 0x05, 0x00, 0x18, 0x00, 0x00, 0x00,
      0x25, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x3d, 0x00, 0x04, 0x00, 0x08, 0x00, 0x00, 0x00, 0x26, 0x00, 0x00, 0x00,
      0x25, 0x00, 0x00, 0x00, 0x41, 0x00, 0x05, 0x00, 0x1c, 0x00, 0x00, 0x00, 0x27, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x09, 0x00, 0x00, 0x00,
      0x3d, 0x00, 0x04, 0x00, 0x12, 0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00, 0x27, 0x00, 0x00, 0x00, 0x7c, 0x00, 0x04, 0x00, 0x12, 0x00, 0x00, 0x00,
      0x29, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0xb0, 0x00, 0x05, 0x00, 0x0d, 0x00, 0x00, 0x00, 0x2a, 0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00,
      0x29, 0x00, 0x00, 0x00, 0xf7, 0x00, 0x03, 0x00, 0x2b, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfa, 0x00, 0x04, 0x00, 0x2a, 0x00, 0x00, 0x00,
      0x2c, 0x00, 0x00, 0x00, 0x2b, 0x00, 0x00, 0x00, 0xf8, 0x00, 0x02, 0x00, 0x2c, 0x00, 0x00, 0x00, 0x41, 0x00, 0x05, 0x00, 0x1c, 0x00, 0x00, 0x00,
      0x2d, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x0a, 0x00, 0x00, 0x00, 0x3d, 0x00, 0x04, 0x00, 0x12, 0x00, 0x00, 0x00, 0x2e, 0x00, 0x00, 0x00,
      0x2d, 0x00, 0x00, 0x00, 0x7c, 0x00, 0x04, 0x00, 0x12, 0x00, 0x00, 0x00, 0x2f, 0x00, 0x00, 0x00, 0x22, 0x00, 0x00, 0x00, 0xb0, 0x00, 0x05, 0x00,
      0x0d, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x2e, 0x00, 0x00, 0x00, 0x2f, 0x00, 0x00, 0x00, 0xf9, 0x00, 0x02, 0x00, 0x2b, 0x00, 0x00, 0x00,
      0xf8, 0x00, 0x02, 0x00, 0x2b, 0x00, 0x00, 0x00, 0xf5, 0x00, 0x07, 0x00, 0x0d, 0x00, 0x00, 0x00, 0x31, 0x00, 0x00, 0x00, 0x0e, 0x00, 0x00, 0x00,
      0x1d, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x2c, 0x00, 0x00, 0x00, 0xf7, 0x00, 0x03, 0x00, 0x32, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0xfa, 0x00, 0x04, 0x00, 0x31, 0x00, 0x00, 0x00, 0x33, 0x00, 0x00, 0x00, 0x32, 0x00, 0x00, 0x00, 0xf8, 0x00, 0x02, 0x00, 0x33, 0x00, 0x00, 0x00,
      0x4f, 0x00, 0x07, 0x00, 0x19, 0x00, 0x00, 0x00, 0x34, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x01, 0x00, 0x00, 0x00, 0x3d, 0x00, 0x04, 0x00, 0x04, 0x00, 0x00, 0x00, 0x35, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x62, 0x00, 0x06, 0x00,
      0x17, 0x00, 0x00, 0x00, 0x36, 0x00, 0x00, 0x00, 0x35, 0x00, 0x00, 0x00, 0x34, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x4f, 0x00, 0x08, 0x00,
      0x1a, 0x00, 0x00, 0x00, 0x37, 0x00, 0x00, 0x00, 0x36, 0x00, 0x00, 0x00, 0x36, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00,
      0x02, 0x00, 0x00, 0x00, 0x0c, 0x00, 0x06, 0x00, 0x1a, 0x00, 0x00, 0x00, 0x38, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x1f, 0x00, 0x00, 0x00,
      0x37, 0x00, 0x00, 0x00, 0x4f, 0x00, 0x09, 0x00, 0x17, 0x00, 0x00, 0x00, 0x39, 0x00, 0x00, 0x00, 0x36, 0x00, 0x00, 0x00, 0x38, 0x00, 0x00, 0x00,
      0x04, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0xaa, 0x00, 0x05, 0x00, 0x0d, 0x00, 0x00, 0x00,
      0x3a, 0x00, 0x00, 0x00, 0x24, 0x00, 0x00, 0x00, 0x0a, 0x00, 0x00, 0x00, 0xf7, 0x00, 0x03, 0x00, 0x3b, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0xfa, 0x00, 0x04, 0x00, 0x3a, 0x00, 0x00, 0x00, 0x3c, 0x00, 0x00, 0x00, 0x3b, 0x00, 0x00, 0x00, 0xf8, 0x00, 0x02, 0x00, 0x3c, 0x00, 0x00, 0x00,
      0x4f, 0x00, 0x08, 0x00, 0x1a, 0x00, 0x00, 0x00, 0x3d, 0x00, 0x00, 0x00, 0x38, 0x00, 0x00, 0x00, 0x1b, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x01, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x0c, 0x00, 0x06, 0x00, 0x1a, 0x00, 0x00, 0x00, 0x3e, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00,
      0x1f, 0x00, 0x00, 0x00, 0x3d, 0x00, 0x00, 0x00, 0x4f, 0x00, 0x09, 0x00, 0x17, 0x00, 0x00, 0x00, 0x3f, 0x00, 0x00, 0x00, 0x36, 0x00, 0x00, 0x00,
      0x3e, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0xf9, 0x00, 0x02, 0x00,
      0x3b, 0x00, 0x00, 0x00, 0xf8, 0x00, 0x02, 0x00, 0x3b, 0x00, 0x00, 0x00, 0xf5, 0x00, 0x07, 0x00, 0x17, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00,
      0x39, 0x00, 0x00, 0x00, 0x33, 0x00, 0x00, 0x00, 0x3f, 0x00, 0x00, 0x00, 0x3c, 0x00, 0x00, 0x00, 0xaa, 0x00, 0x05, 0x00, 0x0d, 0x00, 0x00, 0x00,
      0x41, 0x00, 0x00, 0x00, 0x26, 0x00, 0x00, 0x00, 0x0a, 0x00, 0x00, 0x00, 0xf7, 0x00, 0x03, 0x00, 0x42, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0xfa, 0x00, 0x04, 0x00, 0x41, 0x00, 0x00, 0x00, 0x43, 0x00, 0x00, 0x00, 0x44, 0x00, 0x00, 0x00, 0xf8, 0x00, 0x02, 0x00, 0x43, 0x00, 0x00, 0x00,
      0x4f, 0x00, 0x09, 0x00, 0x17, 0x00, 0x00, 0x00, 0x45, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00,
      0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x3d, 0x00, 0x04, 0x00, 0x04, 0x00, 0x00, 0x00, 0x46, 0x00, 0x00, 0x00,
      0x05, 0x00, 0x00, 0x00, 0x63, 0x00, 0x05, 0x00, 0x46, 0x00, 0x00, 0x00, 0x34, 0x00, 0x00, 0x00, 0x45, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0xf9, 0x00, 0x02, 0x00, 0x42, 0x00, 0x00, 0x00, 0xf8, 0x00, 0x02, 0x00, 0x44, 0x00, 0x00, 0x00, 0x3d, 0x00, 0x04, 0x00, 0x04, 0x00, 0x00, 0x00,
      0x47, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x63, 0x00, 0x05, 0x00, 0x47, 0x00, 0x00, 0x00, 0x34, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0xf9, 0x00, 0x02, 0x00, 0x42, 0x00, 0x00, 0x00, 0xf8, 0x00, 0x02, 0x00, 0x42, 0x00, 0x00, 0x00, 0xf9, 0x00, 0x02, 0x00,
      0x32, 0x00, 0x00, 0x00, 0xf8, 0x00, 0x02, 0x00, 0x32, 0x00, 0x00, 0x00, 0xfd, 0x00, 0x01, 0x00, 0x38, 0x00, 0x01, 0x00
    };
#if 0
    // C:/VulkanSDK/1.4.341.1/Bin/dxc.exe GammaCorrectColorTextureToTheInversePowerOf2.cs.hlsl -T cs_6_0 -Fh GammaCorrectColorTextureToTheInversePowerOf2.cs.h -spirv

    [[vk::image_format("rgba8")]] [[vk::binding(0, 0)]] RWTexture2D<float4> image;

    struct Variables {
      int image_w;
      int image_h;
      int double_gamma_correction;
      int swap_r_and_b;
    };
    [[vk::push_constant]] ConstantBuffer<Variables> variables;

    [numthreads(8, 8, 1)]
    void main(uint3 tid: SV_DispatchThreadId) {
      const int w = variables.image_w;
      const int h = variables.image_h;
      const int double_gamma_correction = variables.double_gamma_correction;
      const int swap_r_and_b = variables.swap_r_and_b;

      if (tid.x < w && tid.y < h) {
        float4 c = image[tid.xy];
        c.rgb = sqrt(c.rgb); // pow(c.rgb, 1.0f / 2.2f); // https://learnopengl.com/Advanced-Lighting/Gamma-Correction
        if (double_gamma_correction == 1) {
          c.rgb = sqrt(c.rgb);
        }
        if (swap_r_and_b == 1) {
          image[tid.xy] = c.bgra;
        } else {
          image[tid.xy] = c;
        }
      }
    }
#endif

    gpu_program_info_t cs_info = {0};
    cs_info.program_binary_bytes_count = sizeof(g_main);
    cs_info.program_binary             = g_main;
    uint64_t cs = vfProgramCreateFromBinaryCompute(context, &cs_info, optionalFile, optionalLine);

    state->programCompute = cs;
    REDGPU_2_EXPECT(state->programCompute  != 0);
  }

  RedStructDeclarationMember slots[1] = {0};
  slots[0].slot            = 0;
  slots[0].type            = RED_STRUCT_MEMBER_TYPE_TEXTURE_RW;
  slots[0].count           = 1;
  slots[0].visibleToStages = RED_VISIBLE_TO_STAGE_BITFLAG_COMPUTE;

  if (state->programPipeline == 0) {
    gpu_program_pipeline_compute_info_t pp_info = {0};
    pp_info.compute_program       = state->programCompute;
    pp_info.variables_slot        = 1;
    pp_info.variables_bytes_count = 1 * 4*sizeof(int);
    pp_info.struct_members_count  = 1;
    pp_info.struct_members        = slots;
    uint64_t pp = vfProgramPipelineCreateCompute(context, &pp_info, optionalFile, optionalLine);

    state->programPipeline = pp;
    REDGPU_2_EXPECT(state->programPipeline != 0);
  }

  vfBatchBindProgramPipelineCompute(context, batch_id, state->programPipeline, optionalFile, optionalLine);
  vfBatchBindNewBindingsSet(context, batch_id, 1, slots, optionalFile, optionalLine);
  RedStructMemberTexture texture = {0};
  texture.sampler = NULL;
  texture.texture = colorTexture->texture;
  texture.setTo1  = 1;
  vfBatchBindTextureRWEx(context, batch_id, 0, 1, &texture, optionalFile, optionalLine);
  vfBatchBindNewBindingsEnd(context, batch_id, optionalFile, optionalLine);
  int variables[4] = {0};
  variables[0] = colorTexture->width;
  variables[1] = colorTexture->height;
  variables[2] = doDoubleGammaCorrection;
  variables[3] = doSwapRedAndBlue;
  vfBatchBindVariablesCopy(context, batch_id, 0, sizeof(variables), variables, optionalFile, optionalLine);
  vfBatchCompute(context, batch_id, (colorTexture->width / 8) + 1, (colorTexture->height / 8) + 1, 1, optionalFile, optionalLine);
}

GPU_API_PRE void GPU_API_POST reiiCommandCopyFromColorTextureToStorageRaw(gpu_handle_context_t context, ReiiHandleCommandList * list, ReiiHandleTexture * texture, RedStructMemberArray * storageRaw) {
  const char * optionalFile = NULL;
  int optionalLine = 0;

  vf_handle_t * batch = (vf_handle_t *)(void *)list->batch_id;
  vf_handle_context_t * vkfast = (vf_handle_context_t *)(void *)context;
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

GPU_API_PRE void GPU_API_POST reiiCommandUnorderedArrayDraw(gpu_handle_context_t context, ReiiHandleCommandList * list, ReiiHandleUnorderedArray * unorderedArray) {
  const char * optionalFile = NULL;
  int optionalLine = 0;

  vf_handle_t * batch = (vf_handle_t *)(void *)list->batch_id;

  npfp(redCallProcedure, list->callProceduresAndAddresses.redCallProcedure,
    "calls", batch->batch.calls.handle,
    "vertexCount", unorderedArray->positionVec4Count,
    "instanceCount", 1,
    "vertexFirst", 0,
    "instanceFirst", 0
  );
}

GPU_API_PRE void GPU_API_POST reiiCommandUnorderedArrayDrawInstanced(gpu_handle_context_t context, ReiiHandleCommandList * list, ReiiHandleUnorderedArray * unorderedArray, unsigned instanceCount) {
  const char * optionalFile = NULL;
  int optionalLine = 0;

  vf_handle_t * batch = (vf_handle_t *)(void *)list->batch_id;

  npfp(redCallProcedure, list->callProceduresAndAddresses.redCallProcedure,
    "calls", batch->batch.calls.handle,
    "vertexCount", unorderedArray->positionVec4Count,
    "instanceCount", instanceCount,
    "vertexFirst", 0,
    "instanceFirst", 0
  );
}

GPU_API_PRE void GPU_API_POST reiiCommandUnorderedArrayDrawInstancedEx(gpu_handle_context_t context, ReiiHandleCommandList * list, ReiiHandleUnorderedArray * unorderedArray, unsigned instanceCount, unsigned vertexCount) {
  const char * optionalFile = NULL;
  int optionalLine = 0;

  vf_handle_t * batch = (vf_handle_t *)(void *)list->batch_id;

  npfp(redCallProcedure, list->callProceduresAndAddresses.redCallProcedure,
    "calls", batch->batch.calls.handle,
    "vertexCount", vertexCount,
    "instanceCount", instanceCount,
    "vertexFirst", 0,
    "instanceFirst", 0
  );
}

GPU_API_PRE void GPU_API_POST reiiCreateUnorderedArray(gpu_handle_context_t context, ReiiHandleUnorderedArray * outUnorderedArray) {
  const char * optionalFile = NULL;
  int optionalLine = 0;

  vf_handle_context_t * vkfast = (vf_handle_context_t *)(void *)context;
  RedHandleGpu gpu = vkfast->gpu;
}

GPU_API_PRE void GPU_API_POST reiiUnorderedArraySet(gpu_handle_context_t context, ReiiHandleUnorderedArray * unorderedArray) {
  const char * optionalFile = NULL;
  int optionalLine = 0;

  gpu_batch_info_t bindings_info = {0};
  unorderedArray->batchId = vfBatchBegin(context, 0, &bindings_info, NULL, optionalFile, optionalLine);
}

GPU_API_PRE void GPU_API_POST reiiUnorderedArrayEnd(gpu_handle_context_t context, ReiiHandleUnorderedArray * unorderedArray) {
  const char * optionalFile = NULL;
  int optionalLine = 0;

  vf_handle_context_t * vkfast = (vf_handle_context_t *)(void *)context;
  RedHandleGpu gpu = vkfast->gpu;

  if (unorderedArray->positionVec4Count > 0) {
    vfeCpuGpuArrayBatchCopyFromCpuToGpu(
      context,
      unorderedArray->batchId,
      &unorderedArray->position,
      0,
      unorderedArray->positionVec4Count * sizeof(ReiiVec4),
      optionalFile,
      optionalLine
    );
  }
  if (unorderedArray->colorVec4Count > 0) {
    vfeCpuGpuArrayBatchCopyFromCpuToGpu(
      context,
      unorderedArray->batchId,
      &unorderedArray->color,
      0,
      unorderedArray->colorVec4Count * sizeof(ReiiVec4),
      optionalFile,
      optionalLine
    );
  }
  if (unorderedArray->normalVec4Count > 0) {
    vfeCpuGpuArrayBatchCopyFromCpuToGpu(
      context,
      unorderedArray->batchId,
      &unorderedArray->normal,
      0,
      unorderedArray->normalVec4Count * sizeof(ReiiVec4),
      optionalFile,
      optionalLine
    );
  }
  for (int i = 0; i < REII_TEXCOORDS_MAX_COUNT; i += 1) {
    if (unorderedArray->texcoordVec4Count[i] > 0) {
      vfeCpuGpuArrayBatchCopyFromCpuToGpu(
        context,
        unorderedArray->batchId,
        &unorderedArray->texcoord[i],
        0,
        unorderedArray->texcoordVec4Count[i] * sizeof(ReiiVec4),
        optionalFile,
        optionalLine
      );
    }
  }
  vfBatchEnd(context, unorderedArray->batchId, optionalFile, optionalLine);
  uint64_t wait = vfAsyncBatchExecute(context, 1, &unorderedArray->batchId, optionalFile, optionalLine);
  vfAsyncWaitToFinish(context, wait, optionalFile, optionalLine);
  vfIdDestroy(1, &unorderedArray->batchId, optionalFile, optionalLine);
  unorderedArray->batchId = 0;
}

GPU_API_PRE void GPU_API_POST reiiUnorderedArrayTexcoord(gpu_handle_context_t context, ReiiHandleUnorderedArray * unorderedArray, unsigned index, float x, float y, float z, float w) {
  const char * optionalFile = NULL;
  int optionalLine = 0;

  REDGPU_2_EXPECT(index < REII_TEXCOORDS_MAX_COUNT);

#ifndef NDEBUG
  {
    const uint64_t bytesNeeded = (unorderedArray->texcoordVec4Count[index] + 1) * sizeof(ReiiVec4);
    const uint64_t bytesTotal  = unorderedArray->texcoord[index].cpu.arrayRangeBytesCount;
    REDGPU_2_EXPECT(bytesNeeded <= bytesTotal || !"(unorderedArray->texcoordVec4Count + 1) * sizeof(ReiiVec4) <= unorderedArray->texcoord[index].cpu.arrayRangeBytesCount");
  }
#endif

  volatile ReiiVec4 * cpu_as_vec4_start = (volatile ReiiVec4 *)unorderedArray->texcoord[index].cpu_ptr;
  volatile ReiiVec4 * cpu_as_vec4       = &cpu_as_vec4_start[unorderedArray->texcoordVec4Count[index]];

  cpu_as_vec4[0].x = x;
  cpu_as_vec4[0].y = y;
  cpu_as_vec4[0].z = z;
  cpu_as_vec4[0].w = w;

  unorderedArray->texcoordVec4Count[index] += 1;
}

GPU_API_PRE void GPU_API_POST reiiUnorderedArrayColor(gpu_handle_context_t context, ReiiHandleUnorderedArray * unorderedArray, float r, float g, float b, float a) {
  const char * optionalFile = NULL;
  int optionalLine = 0;

#ifndef NDEBUG
  {
    const uint64_t bytesNeeded = (unorderedArray->colorVec4Count + 1) * sizeof(ReiiVec4);
    const uint64_t bytesTotal  = unorderedArray->color.cpu.arrayRangeBytesCount;
    REDGPU_2_EXPECT(bytesNeeded <= bytesTotal || !"(unorderedArray->colorVec4Count + 1) * sizeof(ReiiVec4) <= unorderedArray->color.cpu.arrayRangeBytesCount");
  }
#endif

  volatile ReiiVec4 * cpu_as_vec4_start = (volatile ReiiVec4 *)unorderedArray->color.cpu_ptr;
  volatile ReiiVec4 * cpu_as_vec4       = &cpu_as_vec4_start[unorderedArray->colorVec4Count];

  cpu_as_vec4[0].x = r;
  cpu_as_vec4[0].y = g;
  cpu_as_vec4[0].z = b;
  cpu_as_vec4[0].w = a;

  unorderedArray->colorVec4Count += 1;
}

GPU_API_PRE void GPU_API_POST reiiUnorderedArrayNormal(gpu_handle_context_t context, ReiiHandleUnorderedArray * unorderedArray, float x, float y, float z) {
  const char * optionalFile = NULL;
  int optionalLine = 0;

#ifndef NDEBUG
  {
    const uint64_t bytesNeeded = (unorderedArray->normalVec4Count + 1) * sizeof(ReiiVec4);
    const uint64_t bytesTotal  = unorderedArray->normal.cpu.arrayRangeBytesCount;
    REDGPU_2_EXPECT(bytesNeeded <= bytesTotal || !"(unorderedArray->normalVec4Count + 1) * sizeof(ReiiVec4) <= unorderedArray->normal.cpu.arrayRangeBytesCount");
  }
#endif

  volatile ReiiVec4 * cpu_as_vec4_start = (volatile ReiiVec4 *)unorderedArray->normal.cpu_ptr;
  volatile ReiiVec4 * cpu_as_vec4       = &cpu_as_vec4_start[unorderedArray->normalVec4Count];

  cpu_as_vec4[0].x = x;
  cpu_as_vec4[0].y = y;
  cpu_as_vec4[0].z = z;
  cpu_as_vec4[0].w = 1; // NOTE(Constantine): Hmm... I guess?

  unorderedArray->normalVec4Count += 1;
}

GPU_API_PRE void GPU_API_POST reiiUnorderedArrayPosition(gpu_handle_context_t context, ReiiHandleUnorderedArray * unorderedArray, float x, float y, float z, float w) {
  const char * optionalFile = NULL;
  int optionalLine = 0;

#ifndef NDEBUG
  {
    const uint64_t bytesNeeded = (unorderedArray->positionVec4Count + 1) * sizeof(ReiiVec4);
    const uint64_t bytesTotal  = unorderedArray->position.cpu.arrayRangeBytesCount;
    REDGPU_2_EXPECT(bytesNeeded <= bytesTotal || !"(unorderedArray->positionVec4Count + 1) * sizeof(ReiiVec4) <= unorderedArray->position.cpu.arrayRangeBytesCount");
  }
#endif

  volatile ReiiVec4 * cpu_as_vec4_start = (volatile ReiiVec4 *)unorderedArray->position.cpu_ptr;
  volatile ReiiVec4 * cpu_as_vec4       = &cpu_as_vec4_start[unorderedArray->positionVec4Count];

  cpu_as_vec4[0].x = x;
  cpu_as_vec4[0].y = y;
  cpu_as_vec4[0].z = z;
  cpu_as_vec4[0].w = w;

  unorderedArray->positionVec4Count += 1;
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
  } else if (destroyHandleType == GPU_EXTRA_REII_DESTROY_TYPE_SAMPLER) {
    RedHandleSampler handle = (RedHandleSampler)destroyHandle;

    np(red2DestroyHandle,
      "context", vkfast->context,
      "gpu", vkfast->gpu,
      "handleType", RED_HANDLE_TYPE_SAMPLER,
      "handle", handle,
      "optionalHandle2", NULL,
      "optionalFile", optionalFile,
      "optionalLine", optionalLine,
      "optionalUserData", NULL
    );
  } else {
    REDGPU_2_EXPECT(0 || !"Invalid enum value");
  }
}

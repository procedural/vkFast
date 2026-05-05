#pragma once

#include "../../vkfast.h"
#include "../CPU GPU Array/vkfast_extra_cpu_gpu_array.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef unsigned ReiiBool32;

#define REII_TEXCOORDS_MAX_COUNT 8

typedef struct ReiiRgba8 {
  unsigned char r, g, b, a;
} ReiiRgba8;

typedef struct ReiiVec4 {
  float x, y, z, w;
} ReiiVec4;

typedef enum ReiiTextureBinding {
  REII_TEXTURE_BINDING_2D              = 0x0DE1,
  REII_TEXTURE_BINDING_CUBE            = 0x8513,
  REII_TEXTURE_BINDING_CUBE_POSITIVE_X = 0x8515,
  REII_TEXTURE_BINDING_CUBE_NEGATIVE_X = 0x8516,
  REII_TEXTURE_BINDING_CUBE_POSITIVE_Y = 0x8517,
  REII_TEXTURE_BINDING_CUBE_NEGATIVE_Y = 0x8518,
  REII_TEXTURE_BINDING_CUBE_POSITIVE_Z = 0x8519,
  REII_TEXTURE_BINDING_CUBE_NEGATIVE_Z = 0x851A,
} ReiiTextureBinding;

typedef enum ReiiSamplerFiltering {
  REII_SAMPLER_FILTERING_NEAREST             = 0x2600,
  REII_SAMPLER_FILTERING_NEAREST_MIP_NEAREST = 0x2700,
  REII_SAMPLER_FILTERING_NEAREST_MIP_LINEAR  = 0x2702,
  REII_SAMPLER_FILTERING_LINEAR              = 0x2601,
  REII_SAMPLER_FILTERING_LINEAR_MIP_NEAREST  = 0x2701,
  REII_SAMPLER_FILTERING_LINEAR_MIP_LINEAR   = 0x2703,
} ReiiSamplerFiltering;

typedef enum ReiiSamplerBehaviorOutsideTextureCoordinate {
  REII_SAMPLER_BEHAVIOR_OUTSIDE_TEXTURE_COORDINATE_REPEAT              = 0x2901,
  REII_SAMPLER_BEHAVIOR_OUTSIDE_TEXTURE_COORDINATE_CLAMP_TO_EDGE_VALUE = 0x812F,
} ReiiSamplerBehaviorOutsideTextureCoordinate;

typedef enum ReiiTextureTexelFormat {
  REII_TEXTURE_TEXEL_FORMAT_A    = 0x1906,
  REII_TEXTURE_TEXEL_FORMAT_RGB  = 0x1907,
  REII_TEXTURE_TEXEL_FORMAT_RGBA = 0x1908,
  REII_TEXTURE_TEXEL_FORMAT_DS   = 0x84F9, // NOTE(Constantine): Added for depth stencil textures in vkFast REII.
} ReiiTextureTexelFormat;

typedef enum ReiiTextureTexelType {
  REII_TEXTURE_TEXEL_TYPE_S8     = 0x1400,
  REII_TEXTURE_TEXEL_TYPE_U8     = 0x1401,
  REII_TEXTURE_TEXEL_TYPE_S16    = 0x1402,
  REII_TEXTURE_TEXEL_TYPE_U16    = 0x1403,
  REII_TEXTURE_TEXEL_TYPE_S32    = 0x1404,
  REII_TEXTURE_TEXEL_TYPE_U32    = 0x1405,
  REII_TEXTURE_TEXEL_TYPE_FLOAT  = 0x1406,
  REII_TEXTURE_TEXEL_TYPE_U24_U8 = 0x88F0, // NOTE(Constantine): Added for depth stencil textures in vkFast REII.
  REII_TEXTURE_TEXEL_TYPE_F32_U8 = 0x8CAD, // NOTE(Constantine): Added for depth stencil textures in vkFast REII.
} ReiiTextureTexelType;

typedef unsigned ReiiClearFlags;
typedef enum ReiiClearFlagBits {
  REII_CLEAR_DEPTH_BIT   = 0x00000100,
  REII_CLEAR_STENCIL_BIT = 0x00000400,
  REII_CLEAR_COLOR_BIT   = 0x00004000,
} ReiiClearFlagBits;

typedef enum ReiiCullMode {
  REII_CULL_MODE_NONE  = 0,
  REII_CULL_MODE_FRONT = 0x0404,
  REII_CULL_MODE_BACK  = 0x0405,
} ReiiCullMode;

typedef enum ReiiFrontFace {
  REII_FRONT_FACE_COUNTER_CLOCKWISE = 0x0901,
  REII_FRONT_FACE_CLOCKWISE         = 0x0900,
} ReiiFrontFace;

typedef enum ReiiCompareOp {
  REII_COMPARE_OP_NEVER            = 0x0200,
  REII_COMPARE_OP_LESS             = 0x0201,
  REII_COMPARE_OP_EQUAL            = 0x0202,
  REII_COMPARE_OP_LESS_OR_EQUAL    = 0x0203,
  REII_COMPARE_OP_GREATER          = 0x0204,
  REII_COMPARE_OP_NOT_EQUAL        = 0x0205,
  REII_COMPARE_OP_GREATER_OR_EQUAL = 0x0206,
  REII_COMPARE_OP_ALWAYS           = 0x0207,
} ReiiCompareOp;

typedef enum ReiiStencilOp {
  REII_STENCIL_OP_KEEP                = 0x1E00,
  REII_STENCIL_OP_ZERO                = 0,
  REII_STENCIL_OP_REPLACE             = 0x1E01,
  REII_STENCIL_OP_INCREMENT_AND_CLAMP = 0x1E02,
  REII_STENCIL_OP_DECREMENT_AND_CLAMP = 0x1E03,
  REII_STENCIL_OP_INVERT              = 0x150A,
} ReiiStencilOp;

typedef enum ReiiLogicOp {
  REII_LOGIC_OP_CLEAR         = 0x1500,
  REII_LOGIC_OP_AND           = 0x1501,
  REII_LOGIC_OP_AND_REVERSE   = 0x1502,
  REII_LOGIC_OP_COPY          = 0x1503,
  REII_LOGIC_OP_AND_INVERTED  = 0x1504,
  REII_LOGIC_OP_NO_OP         = 0x1505,
  REII_LOGIC_OP_XOR           = 0x1506,
  REII_LOGIC_OP_OR            = 0x1507,
  REII_LOGIC_OP_NOR           = 0x1508,
  REII_LOGIC_OP_EQUIVALENT    = 0x1509,
  REII_LOGIC_OP_INVERT        = 0x150A,
  REII_LOGIC_OP_OR_REVERSE    = 0x150B,
  REII_LOGIC_OP_COPY_INVERTED = 0x150C,
  REII_LOGIC_OP_OR_INVERTED   = 0x150D,
  REII_LOGIC_OP_NAND          = 0x150E,
  REII_LOGIC_OP_SET           = 0x150F,
} ReiiLogicOp;

typedef enum ReiiBlendFactor {
  REII_BLEND_FACTOR_ZERO                     = 0,
  REII_BLEND_FACTOR_ONE                      = 1,
  REII_BLEND_FACTOR_SOURCE_COLOR             = 0x0300,
  REII_BLEND_FACTOR_ONE_MINUS_SOURCE_COLOR   = 0x0301,
  REII_BLEND_FACTOR_TARGET_COLOR             = 0x0306,
  REII_BLEND_FACTOR_ONE_MINUS_TARGET_COLOR   = 0x0307,
  REII_BLEND_FACTOR_SOURCE_ALPHA             = 0x0302,
  REII_BLEND_FACTOR_ONE_MINUS_SOURCE_ALPHA   = 0x0303,
  REII_BLEND_FACTOR_TARGET_ALPHA             = 0x0304,
  REII_BLEND_FACTOR_ONE_MINUS_TARGET_ALPHA   = 0x0305,
  REII_BLEND_FACTOR_CONSTANT_COLOR           = 0x8001,
  REII_BLEND_FACTOR_ONE_MINUS_CONSTANT_COLOR = 0x8002,
  REII_BLEND_FACTOR_SOURCE_ALPHA_SATURATE    = 0x0308,
} ReiiBlendFactor;

typedef enum ReiiBlendOp {
  REII_BLEND_OP_ADD              = 0x8006,
  REII_BLEND_OP_SUBTRACT         = 0x800A,
  REII_BLEND_OP_REVERSE_SUBTRACT = 0x800B,
  REII_BLEND_OP_MIN              = 0x8007,
  REII_BLEND_OP_MAX              = 0x8008,
} ReiiBlendOp;

typedef struct gpu_extra_reii_mesh_state_compile_info_t {
  RedMultisampleCountBitflag         state_multisample_count;
  unsigned                           variables_slot;
  unsigned                           variables_bytes_count;
  unsigned                           struct_members_count;
  const RedStructDeclarationMember * struct_members;
  unsigned                           samplers_count;
  RedBool32                          output_depth_stencil_enable;
  RedFormat                          output_depth_stencil_format;
  RedFormat                          output_color_format;
  const char *                       optional_debug_name;
} gpu_extra_reii_mesh_state_compile_info_t;

typedef struct ReiiMeshState {
  uint64_t           _; // NOTE(Constantine): Ignore this, it's here for ReiiMeshState's struct layout compatibility with the original GL1.4-based REII.
  ReiiBool32         rasterizationDepthClampEnable;
  ReiiCullMode       rasterizationCullMode;
  ReiiFrontFace      rasterizationFrontFace;
  ReiiBool32         rasterizationDepthBiasEnable;
  float              rasterizationDepthBiasConstantFactor;
  float              rasterizationDepthBiasSlopeFactor;
  ReiiBool32         multisampleEnable;
  ReiiBool32         multisampleAlphaToCoverageEnable;
  ReiiBool32         multisampleAlphaToOneEnable;
  ReiiBool32         depthTestEnable;
  ReiiBool32         depthTestDepthWriteEnable;
  ReiiCompareOp      depthTestDepthCompareOp;
  ReiiBool32         stencilTestEnable;
  ReiiStencilOp      stencilTestFrontStencilTestFailOp;
  ReiiStencilOp      stencilTestFrontStencilTestPassDepthTestPassOp;
  ReiiStencilOp      stencilTestFrontStencilTestPassDepthTestFailOp;
  ReiiCompareOp      stencilTestFrontCompareOp;
  ReiiStencilOp      stencilTestBackStencilTestFailOp;
  ReiiStencilOp      stencilTestBackStencilTestPassDepthTestPassOp;
  ReiiStencilOp      stencilTestBackStencilTestPassDepthTestFailOp;
  ReiiCompareOp      stencilTestBackCompareOp;
  unsigned           stencilTestFrontAndBackCompareMask;
  unsigned           stencilTestFrontAndBackWriteMask;
  unsigned           stencilTestFrontAndBackReference;
  ReiiBool32         blendLogicOpEnable;
  ReiiLogicOp        blendLogicOp;
  float              blendConstants[4];
  ReiiBool32         outputColorWriteEnableR;
  ReiiBool32         outputColorWriteEnableG;
  ReiiBool32         outputColorWriteEnableB;
  ReiiBool32         outputColorWriteEnableA;
  ReiiBool32         outputColorBlendEnable;
  ReiiBlendFactor    outputColorBlendColorFactorSource;
  ReiiBlendFactor    outputColorBlendColorFactorTarget;
  ReiiBlendOp        outputColorBlendColorOp;
  ReiiBlendFactor    outputColorBlendAlphaFactorSource;
  ReiiBlendFactor    outputColorBlendAlphaFactorTarget;
  ReiiBlendOp        outputColorBlendAlphaOp;
  char *             codeVertex;
  char *             codeFragment;
  gpu_program_info_t                       programVertex;
  gpu_program_info_t                       programFragment;
  gpu_extra_reii_mesh_state_compile_info_t compileInfo;
  // Internal
  RedHandleGpuCode                         gpuCodeVertex;       // NOTE(Constantine): To destroy. Not set by the user, set by the reiiMeshStateCompile() call.
  RedHandleGpuCode                         gpuCodeFragment;     // NOTE(Constantine): To destroy. Not set by the user, set by the reiiMeshStateCompile() call.
  Red2ProcedureParametersAndDeclarations   procedureParameters; // NOTE(Constantine): To destroy. Not set by the user, set by the reiiMeshStateCompile() call.
  RedHandleProcedure                       procedure;           // NOTE(Constantine): To destroy. Not set by the user, set by the reiiMeshStateCompile() call.
} ReiiMeshState;

typedef enum gpu_extra_reii_texture_type {
  GPU_EXTRA_REII_TEXTURE_TYPE_INVALID                   = 0,
  GPU_EXTRA_REII_TEXTURE_TYPE_GENERAL                   = 1,
  GPU_EXTRA_REII_TEXTURE_TYPE_OUTPUT_DEPTH_STENCIL      = 2,
  GPU_EXTRA_REII_TEXTURE_TYPE_OUTPUT_COLOR              = 3,
  GPU_EXTRA_REII_TEXTURE_TYPE_OUTPUT_DEPTH_STENCIL_MSAA = 4,
  GPU_EXTRA_REII_TEXTURE_TYPE_OUTPUT_COLOR_MSAA         = 5,
} gpu_extra_reii_texture_type;

typedef struct ReiiHandleTextureMemory {
  const char *                optional_debug_name;
  // Internal
  gpu_extra_reii_texture_type texturesType;
  ReiiBool32                  texturesUseTheirOwnDedicatedMemory;
  uint64_t                    bytesCount;
  RedHandleMemory             memory;
  uint64_t                    bytesOffset;
} ReiiHandleTextureMemory;

typedef struct ReiiHandleTexture {
  const char *               optional_debug_name;
  // Internal
  ReiiHandleTextureMemory *  textureMemory;
  ReiiTextureBinding         binding;
  ReiiBool32                 generateMipLevels;
  int                        mipLevelsCount;
  unsigned                   width;
  unsigned                   height;
  RedFormat                  format;
  RedMultisampleCountBitflag msaaCount;
  RedImage                   image;
  RedHandleMemory            imageDedicatedMemory;
  RedHandleTexture           texture;
  RedHandleTexture           textureDepthOnly;
  RedHandleTexture           textureStencilOnly;
  RedHandleTexture           textureCubeFace[6];
} ReiiHandleTexture;

typedef struct ReiiCpuScratchBuffer {
  void *               cpu_scratch_buffer_ptr;
  RedStructMemberArray cpu_scratch_buffer;
} ReiiCpuScratchBuffer;

typedef struct ReiiHandleCommandList {
  uint64_t                      batch_id;
  Red2CallsMutableOutputsArray  mutable_outputs_array;
  gpu_extra_cpu_gpu_array       dynamic_mesh_position;
  gpu_extra_cpu_gpu_array       dynamic_mesh_color;
  gpu_extra_cpu_gpu_array       dynamic_mesh_normal; // NOTE(Constantine): Treated as vec4, unlike in GL1.4-based REII that treats them as vec3.
  gpu_extra_cpu_gpu_array       dynamic_mesh_texcoord[REII_TEXCOORDS_MAX_COUNT];
  // Internal
  uint64_t                      dynamicMeshPositionVec4Offset;
  uint64_t                      dynamicMeshColorVec4Offset;
  uint64_t                      dynamicMeshNormalVec4Offset;
  uint64_t                      dynamicMeshTexcoordVec4Offset[REII_TEXCOORDS_MAX_COUNT];
  uint64_t                      dynamicMeshPositionVec4CurrentStart;
  uint64_t                      dynamicMeshColorVec4CurrentStart;
  uint64_t                      dynamicMeshNormalVec4CurrentStart;
  uint64_t                      dynamicMeshTexcoordVec4CurrentStart[REII_TEXCOORDS_MAX_COUNT];
  RedHandleProcedureParameters  currentProcedureParametersDraw;
  RedCallProceduresAndAddresses callProceduresAndAddresses;
} ReiiHandleCommandList;

typedef struct ReiiHandleUnorderedArray {
  gpu_extra_cpu_gpu_array position;
  gpu_extra_cpu_gpu_array color;
  gpu_extra_cpu_gpu_array normal; // NOTE(Constantine): Treated as vec4, unlike in GL1.4-based REII that treats them as vec3.
  gpu_extra_cpu_gpu_array texcoord[REII_TEXCOORDS_MAX_COUNT];
  // Internal
  uint64_t                batchId;
  uint64_t                positionVec4Count;
  uint64_t                colorVec4Count;
  uint64_t                normalVec4Count;
  uint64_t                texcoordVec4Count[REII_TEXCOORDS_MAX_COUNT];
} ReiiHandleUnorderedArray;

typedef struct ReiiGammaCorrectColorTextureToTheInversePowerOf2StaticState {
  uint64_t programCompute;
  uint64_t programPipeline;
} ReiiGammaCorrectColorTextureToTheInversePowerOf2StaticState;

typedef enum gpu_extra_reii_destroy_type_e {
  GPU_EXTRA_REII_DESTROY_TYPE_UNDEFINED      = 0,
  GPU_EXTRA_REII_DESTROY_TYPE_MESH_STATE     = 1,
  GPU_EXTRA_REII_DESTROY_TYPE_TEXTURE        = 2,
  GPU_EXTRA_REII_DESTROY_TYPE_TEXTURE_MEMORY = 3,
  GPU_EXTRA_REII_DESTROY_TYPE_COMMAND_LIST   = 4,
  GPU_EXTRA_REII_DESTROY_TYPE_SAMPLER        = 5,
} gpu_extra_reii_destroy_type_e;

// Pipeline

GPU_API_PRE void GPU_API_POST reiiMeshStateCompile                   (gpu_handle_context_t context, ReiiMeshState * state);

// Sampler

GPU_API_PRE RedHandleSampler GPU_API_POST reiiCreateSampler          (gpu_handle_context_t context, const char * optionalDebugName, ReiiSamplerFiltering magFiltering, ReiiSamplerFiltering minFiltering, ReiiSamplerBehaviorOutsideTextureCoordinate behaviorOutsideTextureCoordinateU, ReiiSamplerBehaviorOutsideTextureCoordinate behaviorOutsideTextureCoordinateV, int maxAnisotropy);

// Texture

GPU_API_PRE void GPU_API_POST reiiCreateTextureMemory                (gpu_handle_context_t context, gpu_extra_reii_texture_type texturesType, uint64_t bytesCount, ReiiHandleTextureMemory * outTextureMemory);
GPU_API_PRE void GPU_API_POST reiiResetTextureMemory                 (gpu_handle_context_t context, ReiiHandleTextureMemory * textureMemory);
GPU_API_PRE uint64_t GPU_API_POST reiiGetTextureMemoryBytesOffset    (ReiiHandleTextureMemory * textureMemory);
GPU_API_PRE void GPU_API_POST reiiCreateTextureFromTextureMemory     (gpu_handle_context_t context, ReiiHandleTextureMemory * textureMemory, ReiiTextureBinding binding, ReiiHandleTexture * outTexture);
GPU_API_PRE void GPU_API_POST reiiTextureSetStateMipmap              (gpu_handle_context_t context, ReiiTextureBinding binding, ReiiHandleTexture * bindingTexture, ReiiBool32 generateMipLevels);
GPU_API_PRE void GPU_API_POST reiiTextureSetStateMipmapLevelsCount   (gpu_handle_context_t context, ReiiTextureBinding binding, ReiiHandleTexture * bindingTexture, int mipLevelsCount);
GPU_API_PRE void GPU_API_POST reiiTextureSetStateMsaa                (gpu_handle_context_t context, ReiiTextureBinding binding, ReiiHandleTexture * bindingTexture, RedMultisampleCountBitflag msaaCount);
GPU_API_PRE void GPU_API_POST reiiTextureDefineAndCopyFromCpu        (gpu_handle_context_t context, ReiiTextureBinding binding, ReiiHandleTexture * bindingTexture, int bindingLevel, ReiiTextureTexelFormat bindingTexelFormat, int width, int height, ReiiTextureTexelFormat texelsFormat, ReiiTextureTexelType texelsType, int texelsBytesAlignment, const ReiiCpuScratchBuffer * texels);
GPU_API_PRE void GPU_API_POST reiiTextureCopyFromCpu                 (gpu_handle_context_t context, ReiiTextureBinding binding, ReiiHandleTexture * bindingTexture, int bindingLevel, int bindingX, int bindingY, int width, int height, ReiiTextureTexelFormat texelsFormat, ReiiTextureTexelType texelsType, int texelsBytesAlignment, const ReiiCpuScratchBuffer * texels);

// Command list

GPU_API_PRE void GPU_API_POST reiiCommandListReset                   (gpu_handle_context_t context, ReiiHandleCommandList * list);
GPU_API_PRE void GPU_API_POST reiiCommandSetViewportEx               (gpu_handle_context_t context, ReiiHandleCommandList * list, int x, int y, int width, int height, float depthMin, float depthMax);
GPU_API_PRE void GPU_API_POST reiiCommandSetScissor                  (gpu_handle_context_t context, ReiiHandleCommandList * list, int x, int y, int width, int height);
GPU_API_PRE void GPU_API_POST reiiCommandClearTexture                (gpu_handle_context_t context, ReiiHandleCommandList * list, ReiiHandleTexture * depthStencilTexture, ReiiHandleTexture * colorTexture, RedHandleTexture colorTextureHandle, ReiiClearFlags clear, float depthValue, unsigned stencilValue, float colorR, float colorG, float colorB, float colorA);
GPU_API_PRE void GPU_API_POST reiiCommandMeshSetState                (gpu_handle_context_t context, ReiiHandleCommandList * list, ReiiMeshState * state, void * _);
GPU_API_PRE void GPU_API_POST reiiCommandBindSamplers                (gpu_handle_context_t context, ReiiHandleCommandList * list, unsigned samplersCount, RedHandleSampler * samplers);
GPU_API_PRE void GPU_API_POST reiiCommandBindNewBindingsSet          (gpu_handle_context_t context, ReiiHandleCommandList * list, int slotsCount, const RedStructDeclarationMember * slots);
GPU_API_PRE void GPU_API_POST reiiCommandBindStorageRaw              (gpu_handle_context_t context, ReiiHandleCommandList * list, int slot, int storageRawCount, const RedStructMemberArray * storageRaw);
GPU_API_PRE void GPU_API_POST reiiCommandBindTextureRW               (gpu_handle_context_t context, ReiiHandleCommandList * list, int slot, int texturesRWCount, const RedStructMemberTexture * texturesRW);
GPU_API_PRE void GPU_API_POST reiiCommandBindTextureRO               (gpu_handle_context_t context, ReiiHandleCommandList * list, int slot, int texturesROCount, const RedStructMemberTexture * texturesRO);
GPU_API_PRE void GPU_API_POST reiiCommandBindNewBindingsEnd          (gpu_handle_context_t context, ReiiHandleCommandList * list);
GPU_API_PRE void GPU_API_POST reiiCommandBindVariablesCopy           (gpu_handle_context_t context, ReiiHandleCommandList * list, unsigned variablesBytesOffset, unsigned dataBytesCount, const void * data);
GPU_API_PRE void GPU_API_POST reiiCommandRenderTargetSet             (gpu_handle_context_t context, ReiiHandleCommandList * list, ReiiHandleTexture * depthStencilTexture, ReiiHandleTexture * colorTexture, RedHandleTexture colorTextureHandle);
GPU_API_PRE void GPU_API_POST reiiCommandRenderTargetEnd             (gpu_handle_context_t context, ReiiHandleCommandList * list);
GPU_API_PRE void GPU_API_POST reiiCommandMeshSet                     (gpu_handle_context_t context, ReiiHandleCommandList * list);
GPU_API_PRE void GPU_API_POST reiiCommandMeshEndExact                (gpu_handle_context_t context, ReiiHandleCommandList * list, ReiiHandleTexture * depthStencilTexture, ReiiHandleTexture * colorTexture, RedHandleTexture colorTextureHandle);
GPU_API_PRE void GPU_API_POST reiiCommandMeshEndWithTale64BytesAlign (gpu_handle_context_t context, ReiiHandleCommandList * list, ReiiHandleTexture * depthStencilTexture, ReiiHandleTexture * colorTexture, RedHandleTexture colorTextureHandle);
GPU_API_PRE void GPU_API_POST reiiCommandMeshTexcoord                (gpu_handle_context_t context, ReiiHandleCommandList * list, unsigned index, float x, float y, float z, float w);
GPU_API_PRE void GPU_API_POST reiiCommandMeshColor                   (gpu_handle_context_t context, ReiiHandleCommandList * list, float r, float g, float b, float a);
GPU_API_PRE void GPU_API_POST reiiCommandMeshNormal                  (gpu_handle_context_t context, ReiiHandleCommandList * list, float x, float y, float z);
GPU_API_PRE void GPU_API_POST reiiCommandMeshPosition                (gpu_handle_context_t context, ReiiHandleCommandList * list, float x, float y, float z, float w);
GPU_API_PRE void GPU_API_POST reiiCommandResolveMsaaColorTexture     (gpu_handle_context_t context, ReiiHandleCommandList * list, ReiiHandleTexture * sourceMsaaColorTexture, ReiiHandleTexture * targetColorTexture);
GPU_API_PRE void GPU_API_POST reiiCommandGammaCorrectColorTextureToTheInversePowerOf2 (gpu_handle_context_t context, ReiiHandleCommandList * list, ReiiHandleTexture * colorTexture, int doDoubleGammaCorrection, int doSwapRedAndBlue, ReiiGammaCorrectColorTextureToTheInversePowerOf2StaticState * state);
GPU_API_PRE void GPU_API_POST reiiCommandCopyFromColorTextureToStorageRaw (gpu_handle_context_t context, ReiiHandleCommandList * list, ReiiHandleTexture * texture, RedStructMemberArray * storageRaw);
GPU_API_PRE void GPU_API_POST reiiCommandUnorderedArrayDraw               (gpu_handle_context_t context, ReiiHandleCommandList * list, ReiiHandleUnorderedArray * unorderedArray);
GPU_API_PRE void GPU_API_POST reiiCommandUnorderedArrayDrawInstanced      (gpu_handle_context_t context, ReiiHandleCommandList * list, ReiiHandleUnorderedArray * unorderedArray, unsigned instanceCount);
GPU_API_PRE void GPU_API_POST reiiCommandUnorderedArrayDrawInstancedEx    (gpu_handle_context_t context, ReiiHandleCommandList * list, ReiiHandleUnorderedArray * unorderedArray, unsigned instanceCount, unsigned vertexCount);

// Unordered array

GPU_API_PRE void GPU_API_POST reiiCreateUnorderedArray                (gpu_handle_context_t context, ReiiHandleUnorderedArray * outUnorderedArray);
GPU_API_PRE void GPU_API_POST reiiUnorderedArraySet                   (gpu_handle_context_t context, ReiiHandleUnorderedArray * unorderedArray);
GPU_API_PRE void GPU_API_POST reiiUnorderedArrayEnd                   (gpu_handle_context_t context, ReiiHandleUnorderedArray * unorderedArray);
GPU_API_PRE void GPU_API_POST reiiUnorderedArrayTexcoord              (gpu_handle_context_t context, ReiiHandleUnorderedArray * unorderedArray, unsigned index, float x, float y, float z, float w);
GPU_API_PRE void GPU_API_POST reiiUnorderedArrayColor                 (gpu_handle_context_t context, ReiiHandleUnorderedArray * unorderedArray, float r, float g, float b, float a);
GPU_API_PRE void GPU_API_POST reiiUnorderedArrayNormal                (gpu_handle_context_t context, ReiiHandleUnorderedArray * unorderedArray, float x, float y, float z);
GPU_API_PRE void GPU_API_POST reiiUnorderedArrayPosition              (gpu_handle_context_t context, ReiiHandleUnorderedArray * unorderedArray, float x, float y, float z, float w);

// Destroy

GPU_API_PRE void GPU_API_POST reiiDestroyEx                           (gpu_handle_context_t context, gpu_extra_reii_destroy_type_e destroyHandleType, void * destroyHandle);

// Misc Ex

GPU_API_PRE void GPU_API_POST reiiMeshStateRecompileEx                (gpu_handle_context_t context, ReiiMeshState * state, const char * compileCommandVS, const char * compileCommandFS, const wchar_t * compiledSpvFilepathVS, const wchar_t * compiledSpvFilepathFS);
GPU_API_PRE void GPU_API_POST reiiTextureDefineEx                     (gpu_handle_context_t context, ReiiTextureBinding binding, ReiiHandleTexture * bindingTexture, int bindingLevel, ReiiTextureTexelFormat bindingTexelFormat, int width, int height, ReiiTextureTexelFormat texelsFormat, ReiiTextureTexelType texelsType, int texelsBytesAlignment);
GPU_API_PRE void GPU_API_POST reiiBatchImageSetUsableStateEx          (gpu_handle_context_t context, uint64_t batchId, RedHandleImage image, RedImagePartBitflags imageAllParts);
GPU_API_PRE void GPU_API_POST reiiBatchImageCopyFromCpuEx             (gpu_handle_context_t context, uint64_t batchId, RedHandleImage image, RedImagePartBitflags imageAllParts, int bindingLevel, int bindingLayer, int bindingX, int bindingY, int width, int height, const ReiiCpuScratchBuffer * texels);
GPU_API_PRE void GPU_API_POST reiiTextureDefineAndCopyFromCpuEx       (gpu_handle_context_t context, ReiiTextureBinding binding, ReiiHandleTexture * bindingTexture, int bindingLevel, ReiiTextureTexelFormat bindingTexelFormat, int width, int height, ReiiTextureTexelFormat texelsFormat, ReiiTextureTexelType texelsType, int texelsBytesAlignment, const ReiiCpuScratchBuffer * texels, unsigned queueFamilyIndexToSubmitCopyCommands, RedHandleQueue queueToSubmitCopyCommands);
GPU_API_PRE void GPU_API_POST reiiTextureCopyFromCpuEx                (gpu_handle_context_t context, ReiiTextureBinding binding, ReiiHandleTexture * bindingTexture, int bindingLevel, int bindingX, int bindingY, int width, int height, ReiiTextureTexelFormat texelsFormat, ReiiTextureTexelType texelsType, int texelsBytesAlignment, const ReiiCpuScratchBuffer * texels, unsigned queueFamilyIndexToSubmitCopyCommands, RedHandleQueue queueToSubmitCopyCommands);

#ifdef __cplusplus
}
#endif

#include "reii.h"

#ifdef _WIN32
#undef GPU_API_PRE
#undef GPU_API_POST
#define GPU_API_PRE __declspec(dllexport)
#define GPU_API_POST
#endif

#include "../REII/vkfast_extra_reii.h"

#include <stdio.h> // For _popen, _pclose

GPU_API_PRE void GPU_API_POST reiiMeshStateCompile(gpu_handle_context_t context, ReiiMeshState * state) {
  const char * optionalFile = NULL;
  int optionalLine = 0;
  REDGPU_2_EXPECTWG(0 || !"TODO");
}

GPU_API_PRE void GPU_API_POST reiiMeshStateRecompileEx(gpu_handle_context_t context, ReiiMeshState * state, const char * compileCommandVS, const char * compileCommandFS, const wchar_t * compiledSpvFilepathVS, const wchar_t * compiledSpvFilepathFS) {
  const char * optionalFile = NULL;
  int optionalLine = 0;
  REDGPU_2_EXPECTWG(0 || !"TODO");
}

GPU_API_PRE RedHandleSampler GPU_API_POST reiiCreateSampler(gpu_handle_context_t context, const char * optionalDebugName, ReiiSamplerFiltering magFiltering, ReiiSamplerFiltering minFiltering, ReiiSamplerBehaviorOutsideTextureCoordinate behaviorOutsideTextureCoordinateU, ReiiSamplerBehaviorOutsideTextureCoordinate behaviorOutsideTextureCoordinateV, int maxAnisotropy) {
  const char * optionalFile = NULL;
  int optionalLine = 0;
  REDGPU_2_EXPECTWG(0 || !"TODO");
}

GPU_API_PRE void GPU_API_POST reiiCreateTextureMemory(gpu_handle_context_t context, gpu_extra_reii_texture_type texturesType, uint64_t bytesCount, ReiiHandleTextureMemory * outTextureMemory) {
  const char * optionalFile = NULL;
  int optionalLine = 0;
  REDGPU_2_EXPECTWG(0 || !"TODO");
}

GPU_API_PRE void GPU_API_POST reiiResetTextureMemory(gpu_handle_context_t context, ReiiHandleTextureMemory * textureMemory) {
  const char * optionalFile = NULL;
  int optionalLine = 0;
  REDGPU_2_EXPECTWG(0 || !"TODO");
}

GPU_API_PRE void GPU_API_POST reiiCreateTextureFromTextureMemory(gpu_handle_context_t context, ReiiHandleTextureMemory * textureMemory, ReiiTextureBinding binding, ReiiHandleTexture * outTexture) {
  const char * optionalFile = NULL;
  int optionalLine = 0;
  REDGPU_2_EXPECTWG(0 || !"TODO");
}

GPU_API_PRE void GPU_API_POST reiiTextureSetStateMipmap(gpu_handle_context_t context, ReiiTextureBinding binding, ReiiHandleTexture * bindingTexture, ReiiBool32 generateMipLevels) {
  const char * optionalFile = NULL;
  int optionalLine = 0;
  REDGPU_2_EXPECTWG(0 || !"TODO");
}

GPU_API_PRE void GPU_API_POST reiiTextureSetStateMipmapLevelsCount(gpu_handle_context_t context, ReiiTextureBinding binding, ReiiHandleTexture * bindingTexture, int mipLevelsCount) {
  const char * optionalFile = NULL;
  int optionalLine = 0;
  REDGPU_2_EXPECTWG(0 || !"TODO");
}

GPU_API_PRE void GPU_API_POST reiiTextureSetStateMsaa(gpu_handle_context_t context, ReiiTextureBinding binding, ReiiHandleTexture * bindingTexture, RedMultisampleCountBitflag msaaCount) {
  const char * optionalFile = NULL;
  int optionalLine = 0;
  REDGPU_2_EXPECTWG(0 || !"TODO");
}

GPU_API_PRE void GPU_API_POST reiiTextureDefineEx(gpu_handle_context_t context, ReiiTextureBinding binding, ReiiHandleTexture * bindingTexture, int bindingLevel, ReiiTextureTexelFormat bindingTexelFormat, int width, int height, ReiiTextureTexelFormat texelsFormat, ReiiTextureTexelType texelsType, int texelsBytesAlignment) {
  const char * optionalFile = NULL;
  int optionalLine = 0;
  REDGPU_2_EXPECTWG(0 || !"TODO");
}

GPU_API_PRE void GPU_API_POST reiiBatchImageSetUsableStateEx(gpu_handle_context_t context, uint64_t batchId, RedHandleImage image, RedImagePartBitflags imageAllParts) {
  const char * optionalFile = NULL;
  int optionalLine = 0;
  REDGPU_2_EXPECTWG(0 || !"TODO");
}

GPU_API_PRE void GPU_API_POST reiiBatchImageCopyFromCpuEx(gpu_handle_context_t context, uint64_t batchId, RedHandleImage image, RedImagePartBitflags imageAllParts, int bindingLevel, int bindingLayer, int bindingX, int bindingY, int width, int height, const ReiiCpuScratchBuffer * texels) {
  const char * optionalFile = NULL;
  int optionalLine = 0;
  REDGPU_2_EXPECTWG(0 || !"TODO");
}

GPU_API_PRE void GPU_API_POST reiiTextureDefineAndCopyFromCpuEx(gpu_handle_context_t context, ReiiTextureBinding binding, ReiiHandleTexture * bindingTexture, int bindingLevel, ReiiTextureTexelFormat bindingTexelFormat, int width, int height, ReiiTextureTexelFormat texelsFormat, ReiiTextureTexelType texelsType, int texelsBytesAlignment, const ReiiCpuScratchBuffer * texels, unsigned queueFamilyIndexToSubmitCopyCommands, RedHandleQueue queueToSubmitCopyCommands) {
  const char * optionalFile = NULL;
  int optionalLine = 0;
  REDGPU_2_EXPECTWG(0 || !"TODO");
}

GPU_API_PRE void GPU_API_POST reiiTextureCopyFromCpuEx(gpu_handle_context_t context, ReiiTextureBinding binding, ReiiHandleTexture * bindingTexture, int bindingLevel, int bindingX, int bindingY, int width, int height, ReiiTextureTexelFormat texelsFormat, ReiiTextureTexelType texelsType, int texelsBytesAlignment, const ReiiCpuScratchBuffer * texels, unsigned queueFamilyIndexToSubmitCopyCommands, RedHandleQueue queueToSubmitCopyCommands) {
  const char * optionalFile = NULL;
  int optionalLine = 0;
  REDGPU_2_EXPECTWG(0 || !"TODO");
}

GPU_API_PRE void GPU_API_POST reiiTextureDefineAndCopyFromCpu(gpu_handle_context_t context, ReiiTextureBinding binding, ReiiHandleTexture * bindingTexture, int bindingLevel, ReiiTextureTexelFormat bindingTexelFormat, int width, int height, ReiiTextureTexelFormat texelsFormat, ReiiTextureTexelType texelsType, int texelsBytesAlignment, const ReiiCpuScratchBuffer * texels) {
  const char * optionalFile = NULL;
  int optionalLine = 0;
  REDGPU_2_EXPECTWG(0 || !"TODO");
}

GPU_API_PRE void GPU_API_POST reiiTextureCopyFromCpu(gpu_handle_context_t context, ReiiTextureBinding binding, ReiiHandleTexture * bindingTexture, int bindingLevel, int bindingX, int bindingY, int width, int height, ReiiTextureTexelFormat texelsFormat, ReiiTextureTexelType texelsType, int texelsBytesAlignment, const ReiiCpuScratchBuffer * texels) {
  const char * optionalFile = NULL;
  int optionalLine = 0;
  REDGPU_2_EXPECTWG(0 || !"TODO");
}

GPU_API_PRE void GPU_API_POST reiiCommandListReset(gpu_handle_context_t context, ReiiHandleCommandList * list) {
  const char * optionalFile = NULL;
  int optionalLine = 0;
  REDGPU_2_EXPECTWG(0 || !"TODO");
}

GPU_API_PRE void GPU_API_POST reiiCommandSetViewportEx(gpu_handle_context_t context, ReiiHandleCommandList * list, int x, int y, int width, int height, float depthMin, float depthMax) {
  const char * optionalFile = NULL;
  int optionalLine = 0;
  REDGPU_2_EXPECTWG(0 || !"TODO");
}

GPU_API_PRE void GPU_API_POST reiiCommandSetScissor(gpu_handle_context_t context, ReiiHandleCommandList * list, int x, int y, int width, int height) {
  const char * optionalFile = NULL;
  int optionalLine = 0;
  REDGPU_2_EXPECTWG(0 || !"TODO");
}

GPU_API_PRE void GPU_API_POST reiiCommandClearTexture(gpu_handle_context_t context, ReiiHandleCommandList * list, ReiiHandleTexture * depthStencilTexture, ReiiHandleTexture * colorTexture, RedHandleTexture colorTextureHandle, ReiiClearFlags clear, float depthValue, unsigned stencilValue, float colorR, float colorG, float colorB, float colorA) {
  const char * optionalFile = NULL;
  int optionalLine = 0;
  REDGPU_2_EXPECTWG(0 || !"TODO");
}

GPU_API_PRE void GPU_API_POST reiiCommandMeshSetState(gpu_handle_context_t context, ReiiHandleCommandList * list, ReiiMeshState * state, void * _) {
  const char * optionalFile = NULL;
  int optionalLine = 0;
  REDGPU_2_EXPECTWG(0 || !"TODO");
}

GPU_API_PRE void GPU_API_POST reiiCommandBindSamplers(gpu_handle_context_t context, ReiiHandleCommandList * list, unsigned samplersCount, RedHandleSampler * samplers) {
  const char * optionalFile = NULL;
  int optionalLine = 0;
  REDGPU_2_EXPECTWG(0 || !"TODO");
}

GPU_API_PRE void GPU_API_POST reiiCommandBindNewBindingsSet(gpu_handle_context_t context, ReiiHandleCommandList * list, int slotsCount, const RedStructDeclarationMember * slots) {
  const char * optionalFile = NULL;
  int optionalLine = 0;
  REDGPU_2_EXPECTWG(0 || !"TODO");
}

GPU_API_PRE void GPU_API_POST reiiCommandBindStorageRaw(gpu_handle_context_t context, ReiiHandleCommandList * list, int slot, int storageRawCount, const RedStructMemberArray * storageRaw) {
  const char * optionalFile = NULL;
  int optionalLine = 0;
  REDGPU_2_EXPECTWG(0 || !"TODO");
}

GPU_API_PRE void GPU_API_POST reiiCommandBindTextureRW(gpu_handle_context_t context, ReiiHandleCommandList * list, int slot, int texturesRWCount, const RedStructMemberTexture * texturesRW) {
  const char * optionalFile = NULL;
  int optionalLine = 0;
  REDGPU_2_EXPECTWG(0 || !"TODO");
}

GPU_API_PRE void GPU_API_POST reiiCommandBindTextureRO(gpu_handle_context_t context, ReiiHandleCommandList * list, int slot, int texturesROCount, const RedStructMemberTexture * texturesRO) {
  const char * optionalFile = NULL;
  int optionalLine = 0;
  REDGPU_2_EXPECTWG(0 || !"TODO");
}

GPU_API_PRE void GPU_API_POST reiiCommandBindNewBindingsEnd(gpu_handle_context_t context, ReiiHandleCommandList * list) {
  const char * optionalFile = NULL;
  int optionalLine = 0;
  REDGPU_2_EXPECTWG(0 || !"TODO");
}

GPU_API_PRE void GPU_API_POST reiiCommandBindVariablesCopy(gpu_handle_context_t context, ReiiHandleCommandList * list, unsigned variablesBytesOffset, unsigned dataBytesCount, const void * data) {
  const char * optionalFile = NULL;
  int optionalLine = 0;
  REDGPU_2_EXPECTWG(0 || !"TODO");
}

GPU_API_PRE void GPU_API_POST reiiCommandRenderTargetSet(gpu_handle_context_t context, ReiiHandleCommandList * list, ReiiHandleTexture * depthStencilTexture, ReiiHandleTexture * colorTexture, RedHandleTexture colorTextureHandle) {
  const char * optionalFile = NULL;
  int optionalLine = 0;
  REDGPU_2_EXPECTWG(0 || !"TODO");
}

GPU_API_PRE void GPU_API_POST reiiCommandRenderTargetEnd(gpu_handle_context_t context, ReiiHandleCommandList * list) {
  const char * optionalFile = NULL;
  int optionalLine = 0;
  REDGPU_2_EXPECTWG(0 || !"TODO");
}

GPU_API_PRE void GPU_API_POST reiiCommandMeshSet(gpu_handle_context_t context, ReiiHandleCommandList * list) {
  const char * optionalFile = NULL;
  int optionalLine = 0;
  REDGPU_2_EXPECTWG(0 || !"TODO");
}

GPU_API_PRE void GPU_API_POST reiiCommandMeshEndExact(gpu_handle_context_t context, ReiiHandleCommandList * list, ReiiHandleTexture * depthStencilTexture, ReiiHandleTexture * colorTexture, RedHandleTexture colorTextureHandle) {
  const char * optionalFile = NULL;
  int optionalLine = 0;
  REDGPU_2_EXPECTWG(0 || !"TODO");
}

GPU_API_PRE void GPU_API_POST reiiCommandMeshEndWithTale64BytesAlign(gpu_handle_context_t context, ReiiHandleCommandList * list, ReiiHandleTexture * depthStencilTexture, ReiiHandleTexture * colorTexture, RedHandleTexture colorTextureHandle) {
  const char * optionalFile = NULL;
  int optionalLine = 0;
  REDGPU_2_EXPECTWG(0 || !"TODO");
}

GPU_API_PRE void GPU_API_POST reiiCommandMeshTexcoord(gpu_handle_context_t context, ReiiHandleCommandList * list, unsigned index, float x, float y, float z, float w) {
  const char * optionalFile = NULL;
  int optionalLine = 0;
  REDGPU_2_EXPECTWG(0 || !"TODO");
}

GPU_API_PRE void GPU_API_POST reiiCommandMeshColor(gpu_handle_context_t context, ReiiHandleCommandList * list, float r, float g, float b, float a) {
  const char * optionalFile = NULL;
  int optionalLine = 0;
  REDGPU_2_EXPECTWG(0 || !"TODO");
}

GPU_API_PRE void GPU_API_POST reiiCommandMeshNormal(gpu_handle_context_t context, ReiiHandleCommandList * list, float x, float y, float z) {
  const char * optionalFile = NULL;
  int optionalLine = 0;
  REDGPU_2_EXPECTWG(0 || !"TODO");
}

GPU_API_PRE void GPU_API_POST reiiCommandMeshPosition(gpu_handle_context_t context, ReiiHandleCommandList * list, float x, float y, float z, float w) {
  const char * optionalFile = NULL;
  int optionalLine = 0;
  REDGPU_2_EXPECTWG(0 || !"TODO");
}

GPU_API_PRE void GPU_API_POST reiiCommandResolveMsaaColorTexture(gpu_handle_context_t context, ReiiHandleCommandList * list, ReiiHandleTexture * sourceMsaaColorTexture, ReiiHandleTexture * targetColorTexture) {
  const char * optionalFile = NULL;
  int optionalLine = 0;
  REDGPU_2_EXPECTWG(0 || !"TODO");
}

GPU_API_PRE void GPU_API_POST reiiCommandCopyFromColorTextureToStorageRaw(gpu_handle_context_t context, ReiiHandleCommandList * list, ReiiHandleTexture * texture, RedStructMemberArray * storageRaw) {
  const char * optionalFile = NULL;
  int optionalLine = 0;
  REDGPU_2_EXPECTWG(0 || !"TODO");
}

GPU_API_PRE void GPU_API_POST reiiCommandStaticArrayDraw(gpu_handle_context_t context, ReiiHandleCommandList * list, ReiiHandleStaticArray * staticArray) {
  const char * optionalFile = NULL;
  int optionalLine = 0;
  REDGPU_2_EXPECTWG(0 || !"TODO");
}

GPU_API_PRE void GPU_API_POST reiiCommandStaticArrayDrawInstanced(gpu_handle_context_t context, ReiiHandleCommandList * list, ReiiHandleStaticArray * staticArray, unsigned instanceCount) {
  const char * optionalFile = NULL;
  int optionalLine = 0;
  REDGPU_2_EXPECTWG(0 || !"TODO");
}

GPU_API_PRE void GPU_API_POST reiiCommandStaticArrayDrawInstancedEx(gpu_handle_context_t context, ReiiHandleCommandList * list, ReiiHandleStaticArray * staticArray, unsigned instanceCount, unsigned vertexCount) {
  const char * optionalFile = NULL;
  int optionalLine = 0;
  REDGPU_2_EXPECTWG(0 || !"TODO");
}

GPU_API_PRE void GPU_API_POST reiiCreateStaticArray(gpu_handle_context_t context, ReiiHandleStaticArray * outStaticArray) {
  const char * optionalFile = NULL;
  int optionalLine = 0;
  REDGPU_2_EXPECTWG(0 || !"TODO");
}

GPU_API_PRE void GPU_API_POST reiiStaticArraySet(gpu_handle_context_t context, ReiiHandleStaticArray * staticArray) {
  const char * optionalFile = NULL;
  int optionalLine = 0;
  REDGPU_2_EXPECTWG(0 || !"TODO");
}

GPU_API_PRE void GPU_API_POST reiiStaticArrayEnd(gpu_handle_context_t context, ReiiHandleStaticArray * staticArray) {
  const char * optionalFile = NULL;
  int optionalLine = 0;
  REDGPU_2_EXPECTWG(0 || !"TODO");
}

GPU_API_PRE void GPU_API_POST reiiStaticArrayTexcoord(gpu_handle_context_t context, ReiiHandleStaticArray * staticArray, unsigned index, float x, float y, float z, float w) {
  const char * optionalFile = NULL;
  int optionalLine = 0;
  REDGPU_2_EXPECTWG(0 || !"TODO");
}

GPU_API_PRE void GPU_API_POST reiiStaticArrayColor(gpu_handle_context_t context, ReiiHandleStaticArray * staticArray, float r, float g, float b, float a) {
  const char * optionalFile = NULL;
  int optionalLine = 0;
  REDGPU_2_EXPECTWG(0 || !"TODO");
}

GPU_API_PRE void GPU_API_POST reiiStaticArrayNormal(gpu_handle_context_t context, ReiiHandleStaticArray * staticArray, float x, float y, float z) {
  const char * optionalFile = NULL;
  int optionalLine = 0;
  REDGPU_2_EXPECTWG(0 || !"TODO");
}

GPU_API_PRE void GPU_API_POST reiiStaticArrayPosition(gpu_handle_context_t context, ReiiHandleStaticArray * staticArray, float x, float y, float z, float w) {
  const char * optionalFile = NULL;
  int optionalLine = 0;
  REDGPU_2_EXPECTWG(0 || !"TODO");
}

GPU_API_PRE void GPU_API_POST reiiDestroyEx(gpu_handle_context_t context, gpu_extra_reii_destroy_type_e destroyHandleType, void * destroyHandle) {
  const char * optionalFile = NULL;
  int optionalLine = 0;
  REDGPU_2_EXPECTWG(0 || !"TODO");
}

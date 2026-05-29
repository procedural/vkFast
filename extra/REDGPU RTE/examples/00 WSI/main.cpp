// Copyright 2020-2021 NVIDIA Corporation
// SPDX-License-Identifier: Apache-2.0
//
// Compile instructions:
//
// glslangValidator.exe --target-env vulkan1.2 raytrace.comp.glsl -o raytrace.comp.glsl.spv
// cl /EHsc /std:c++20 main.cpp /link ../../build/redgpu_rte.lib ../Common/glfw-3.4.bin.WIN64/lib-vc2019/glfw3_mt.lib user32.lib gdi32.lib shell32.lib

#include "../../redgpu_rte.h"
#include "C:/RedGpuSDK/redgpu_wsi.h"

#define GLFW_INCLUDE_NONE
#define GLFW_EXPOSE_NATIVE_WIN32
#include "../Common/glfw-3.4.bin.WIN64/include/GLFW/glfw3.h"
#include "../Common/glfw-3.4.bin.WIN64/include/GLFW/glfw3native.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../Common/stb_image_write.h"
#define TINYOBJLOADER_IMPLEMENTATION
#include "../Common/tiny_obj_loader.h"

static const uint64_t g_render_width     = 800;
static const uint64_t g_render_height    = 600;
static const uint32_t g_workgroup_width  = 16;
static const uint32_t g_workgroup_height = 8;

float g_camera_pos_x = -0.001f;
float g_camera_pos_y = 1.0f;
float g_camera_pos_z = 6.0f;

std::string LoadFile(std::string filename, bool binary) {
  std::string result;

  std::ifstream stream(filename, std::ios::ate | (binary ? std::ios::binary : std::ios_base::openmode(0)));
  if (stream.is_open() == false) {
    return result;
  }

  result.reserve(stream.tellg());
  stream.seekg(0, std::ios::beg);

  result.assign((std::istreambuf_iterator<char>(stream)), std::istreambuf_iterator<char>());

  return result;
}

RedCalls CallsCreateAndSet(RedContext context, RedStatuses * outStatuses) {
  RedCalls calls = {};
  redCreateCalls(context, context->gpus[0].gpu, 0, context->gpus[0].queuesFamilyIndex[0], &calls, outStatuses, __FILE__, __LINE__, 0);
  redCallsSet(context, context->gpus[0].gpu, calls.handle, calls.memory, calls.reusable, outStatuses, __FILE__, __LINE__, 0);
  return calls;
}

void CallsEndSubmitWaitAndDestroy(RedContext context, RedCalls calls, RedHandleGpuSignal signalGpuSignal, RedStatuses * outStatuses) {
  redCallsEnd(context, context->gpus[0].gpu, calls.handle, calls.memory, outStatuses, __FILE__, __LINE__, 0);
  RedHandleCpuSignal cpuSignal = 0;
  redCreateCpuSignal(context, context->gpus[0].gpu, 0, 0, &cpuSignal, outStatuses, __FILE__, __LINE__, 0);
  RedGpuTimeline timeline = {};
  timeline.setTo4                            = 4;
  timeline.setTo0                            = 0;
  timeline.waitForAndUnsignalGpuSignalsCount = signalGpuSignal != 0 ? 1                : 0;
  timeline.waitForAndUnsignalGpuSignals      = signalGpuSignal != 0 ? &signalGpuSignal : 0;
  timeline.setTo65536                        = 0;
  timeline.callsCount                        = 1;
  timeline.calls                             = &calls.handle;
  timeline.signalGpuSignalsCount             = signalGpuSignal != 0 ? 1                : 0;
  timeline.signalGpuSignals                  = signalGpuSignal != 0 ? &signalGpuSignal : 0;
  redQueueSubmit(context, context->gpus[0].gpu, context->gpus[0].queues[0], 1, &timeline, cpuSignal, outStatuses, __FILE__, __LINE__, 0);
  redCpuSignalWait(context, context->gpus[0].gpu, 1, &cpuSignal, 1, outStatuses, __FILE__, __LINE__, 0);
  redDestroyCpuSignal(context, context->gpus[0].gpu, cpuSignal, __FILE__, __LINE__, 0);
  redDestroyCalls(context, context->gpus[0].gpu, calls.handle, calls.memory, __FILE__, __LINE__, 0);
}

int main() {
  glfwInit();
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
  GLFWwindow * window = glfwCreateWindow(g_render_width, g_render_height, "REDGPU RTE", NULL, NULL);

  struct PhysicalDeviceAccelerationStructureFeatures {
    unsigned  setTo1000150013;
    size_t    setTo0;
    RedBool32 accelerationStructure;
    RedBool32 accelerationStructureCaptureReplay;
    RedBool32 accelerationStructureIndirectBuild;
    RedBool32 accelerationStructureHostCommands;
    RedBool32 descriptorBindingAccelerationStructureUpdateAfterBind;
  };

  struct PhysicalDeviceRayQueryFeatures {
    unsigned  setTo1000348013;
    size_t    setTo0;
    RedBool32 rayQuery;
  };

  struct PhysicalDeviceAccelerationStructureFeatures asFeatures       = {1000150013};
  struct PhysicalDeviceRayQueryFeatures              rayQueryFeatures = {1000348013};

  const unsigned redgpuExtensionsCount = 1;
  const unsigned redgpuExtensions[redgpuExtensionsCount] = {
    RED_SDK_EXTENSION_WSI_WIN32,
  };
  const unsigned nvvkInstanceExtensionsCount = 3;
  const char * nvvkInstanceExtensions[nvvkInstanceExtensionsCount] = {
    "VK_KHR_surface",
    "VK_KHR_win32_surface",
    "VK_EXT_debug_utils",
  };
  const unsigned nvvkInstanceLayersCount = 1;
  const char * nvvkInstanceLayers[nvvkInstanceLayersCount] = {
    "VK_LAYER_KHRONOS_validation",
  };
  const unsigned nvvkDeviceExtensionsCount = 4;
  const char * nvvkDeviceExtensions[nvvkDeviceExtensionsCount] = {
    "VK_KHR_swapchain",
    "VK_KHR_deferred_host_operations",
    "VK_KHR_acceleration_structure",
    "VK_KHR_ray_query",
  };
  void * nvvkDeviceExtensionsVkFeaturesStructs[nvvkDeviceExtensionsCount] = {
    0,
    0,
    &asFeatures,
    &rayQueryFeatures,
  };
  RedRteQueueBitflags secondQueueFamilyFlags       = RED_RTE_QUEUE_BITFLAG_CAN_DRAW | RED_RTE_QUEUE_BITFLAG_CAN_COMPUTE | RED_RTE_QUEUE_BITFLAG_CAN_COPY;
  unsigned            secondQueueFamilyQueuesCount = 1;
  float               secondQueueFamilyPriority    = 1.0f;
  RedRteCreateContextParameters rteInfo = {};
  rteInfo.reservedStructId                      = 0;
  rteInfo.gpuIndex                              = 0;
  rteInfo.vkVersionMajor                        = 1;
  rteInfo.vkVersionMinor                        = 2;
  rteInfo.vkInstanceExtensionsCount             = nvvkInstanceExtensionsCount;
  rteInfo.vkInstanceExtensions                  = nvvkInstanceExtensions;
  rteInfo.vkInstanceLayersCount                 = nvvkInstanceLayersCount;
  rteInfo.vkInstanceLayers                      = nvvkInstanceLayers;
  rteInfo.vkDeviceExtensionsCount               = nvvkDeviceExtensionsCount;
  rteInfo.vkDeviceExtensions                    = nvvkDeviceExtensions;
  rteInfo.vkDeviceExtensionsOutVkFeaturesStruct = nvvkDeviceExtensionsVkFeaturesStructs;
  rteInfo.vkDeviceExtensionsVersion             = 0;
  rteInfo.disableRobustBufferAccess             = 1;
  rteInfo.verboseCompatibleDevices              = 1;
  rteInfo.verboseUsed                           = 1;
  rteInfo.verboseAvailable                      = 1;
  rteInfo.enableAftermath                       = 1;
  rteInfo.ignoreDebugMessagesCount              = 0;
  rteInfo.ignoreDebugMessages                   = 0;
  rteInfo.debugSeverityFilterMask               = 0x00000100 | 0x00001000; // VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT
  rteInfo.requestQueueFamilysCount              = 1;
  rteInfo.requestQueueFamilysFlags              = &secondQueueFamilyFlags;
  rteInfo.requestQueueFamilysQueuesCount        = &secondQueueFamilyQueuesCount;
  rteInfo.requestQueueFamilysPriority           = &secondQueueFamilyPriority;
  rteInfo.rteContext                            = 0;
  rteInfo.getInstanceProcAddr                   = 0;
  RedStatuses redstatuses = {};
  RedContext context = 0;
  int64_t rtestatus = redRteCreateContext(malloc, free, 0, 0, 0, RED_SDK_VERSION_1_0_135, redgpuExtensionsCount, redgpuExtensions, 0, 0, 0, 0, 0, &context, &redstatuses, __FILE__, __LINE__, 0, &rteInfo);
  
  RedRteQueue secondQueue = {};
  rtestatus = redRteCreateQueue(rteInfo.rteContext, 0, RED_RTE_QUEUE_BITFLAG_CAN_DRAW | RED_RTE_QUEUE_BITFLAG_CAN_COMPUTE | RED_RTE_QUEUE_BITFLAG_CAN_COPY, 1.0f, &secondQueue, &redstatuses, __FILE__, __LINE__, 0, 0);

  assert(asFeatures.accelerationStructure == 1);
  assert(rayQueryFeatures.rayQuery == 1);

  RedRteHandleHelperAllocatorVma allocator = 0;
  rtestatus = redRteCreateHelperAllocatorVma(rteInfo.rteContext, &allocator, &redstatuses, __FILE__, __LINE__, 0, 0);

  tinyobj::ObjReader objReader = tinyobj::ObjReader();
  objReader.ParseFromFile("CornellBox-Original-Merged.obj");
  assert(objReader.Valid() == true);
  const std::vector<tinyobj::real_t>    objVertices = objReader.GetAttrib().GetVertices();
  const std::vector<tinyobj::shape_t> & objShapes   = objReader.GetShapes();
  assert(objShapes.size() >= 1);
  const tinyobj::shape_t & objShape = objShapes[0];
  std::vector<uint32_t> objIndices = std::vector<uint32_t>();
  objIndices.reserve(objShape.mesh.indices.size());
  for (const tinyobj::index_t & index : objShape.mesh.indices) {
    objIndices.push_back(index.vertex_index);
  }

  RedRteHelperArray arrayVertices = {};
  RedRteHelperArray arrayIndices  = {};
  {
    RedCalls callsUpload = CallsCreateAndSet(context, &redstatuses);

    rtestatus = redRteHelperAllocatorVmaCreateArraySimpleWithData(
      allocator,
      callsUpload.handle,
      sizeof(objVertices[0]) * objVertices.size(),
      objVertices.data(),
      0x00020000 | 0x00000020 | 0x00080000, // VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR
      0x00000001, // VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
      &arrayVertices,
      &redstatuses, __FILE__, __LINE__, 0, 0
    );

    rtestatus = redRteHelperAllocatorVmaCreateArraySimpleWithData(
      allocator,
      callsUpload.handle,
      sizeof(objIndices[0]) * objIndices.size(),
      objIndices.data(),
      0x00020000 | 0x00000020 | 0x00080000, // VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR
      0x00000001, // VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
      &arrayIndices,
      &redstatuses, __FILE__, __LINE__, 0, 0
    );

    CallsEndSubmitWaitAndDestroy(context, callsUpload, 0, &redstatuses);

    rtestatus = redRteHelperAllocatorVmaFinalizeAndReleaseStagingMemory(
      allocator,
      0,
      &redstatuses, __FILE__, __LINE__, 0, 0
    );
  }

  RedRteHandleRayTracingBuilder rteBuilder = 0;
  rtestatus = redRteCreateRayTracingBuilder(rteInfo.rteContext, allocator, 0, &rteBuilder, &redstatuses, __FILE__, __LINE__, 0, 0);

  // Describe the bottom-level acceleration structure (BLAS)
  RedRteRayTracingAccelerationStructureGeometry       blasGeometry       = {};
  RedRteRayTracingAccelerationStructureBuildRangeInfo blasBuildRangeInfo = {};
  RedRteRayTracingBuilderBlasInput                    blas               = {};
  {
    // Specify where the builder can find the vertices and indices for triangles, and their formats
    RedRteRayTracingAccelerationStructureGeometryTrianglesData triangles = {};
    triangles.setTo1000150005          = 1000150005;
    triangles.setTo0                   = 0;
    triangles.vertexFormat             = 106; // VK_FORMAT_R32G32B32_SFLOAT
    triangles.vertexData.gpuAddress    = redRteArrayGetGpuAddress(rteInfo.rteContext, context->gpus[0].gpu, arrayVertices.handle, __FILE__, __LINE__, 0, 0);
    triangles.vertexStride             = 3 * sizeof(float);
    triangles.maxVertex                = (unsigned)(objVertices.size() / 3) - 1;
    triangles.indexType                = 1; // VK_INDEX_TYPE_UINT32
    triangles.indexData.gpuAddress     = redRteArrayGetGpuAddress(rteInfo.rteContext, context->gpus[0].gpu, arrayIndices.handle, __FILE__, __LINE__, 0, 0);
    triangles.transformData.gpuAddress = 0; // No transform

    // Create a RedRteAccelerationStructureGeometry object that says it handles opaque triangles and points to the above
    blasGeometry.setTo1000150006    = 1000150006;
    blasGeometry.setTo0             = 0;
    blasGeometry.geometryType       = RED_RTE_GEOMETRY_TYPE_TRIANGLES;
    blasGeometry.geometry.triangles = triangles;
    blasGeometry.geometryFlags      = 0x00000001; // VK_GEOMETRY_OPAQUE_BIT_KHR

    // Create offset info that allows us to say how many triangles and vertices to read
    blasBuildRangeInfo.primitiveCount  = (unsigned)(objIndices.size() / 3); // Number of triangles
    blasBuildRangeInfo.primitiveOffset = 0;
    blasBuildRangeInfo.vertexFirst     = 0;
    blasBuildRangeInfo.transformOffset = 0;

    blas.geometrysCount       = 1;
    blas.geometrys            = &blasGeometry;
    blas.buildRangeInfosCount = 1;
    blas.buildRangeInfos      = &blasBuildRangeInfo;
    blas.flags                = 0;
  }

  rtestatus = redRteRayTracingBuilderBuildBlas(rteBuilder, 1, &blas, 0x00000004 /*VK_BUILD_ACCELERATION_STRUCTURE_PREFER_FAST_TRACE_BIT_KHR*/, &redstatuses, __FILE__, __LINE__, 0, 0);

  // Create an instance pointing to this BLAS, and build it into a TLAS
  RedRteRayTracingAccelerationStructureInstance instance = {};
  instance.transform.matrix[0][0]                 = 1.0f;
  instance.transform.matrix[1][1]                 = 1.0f;
  instance.transform.matrix[2][2]                 = 1.0f;
  instance.instanceCustomIndex                    = 0; // 24 bits accessible to ray shaders via rayQueryGetIntersectionInstanceCustomIndexEXT
  instance.mask                                   = 0xFF;
  instance.instanceShaderBindingTableRecordOffset = 0; // Used for a shader offset index, accessible via rayQueryGetIntersectionInstanceShaderBindingTableRecordOffsetEXT
  instance.flags                                  = 0x00000001; // VK_GEOMETRY_INSTANCE_TRIANGLE_FACING_CULL_DISABLE_BIT_KHR // How to trace this instance
  instance.accelerationStructureReference         = redRteRayTracingBuilderGetBlasGpuAddress(rteBuilder, 0, __FILE__, __LINE__, 0, 0);
  rtestatus = redRteRayTracingBuilderBuildTlas(rteBuilder, 1, &instance, 0x00000004 /*VK_BUILD_ACCELERATION_STRUCTURE_PREFER_FAST_TRACE_BIT_KHR*/, false, &redstatuses, __FILE__, __LINE__, 0, 0);

  RedHandleSurface surface = 0;
  redCreateSurfaceWin32(context, context->gpus[0].gpu, 0, GetModuleHandle(0), glfwGetWin32Window(window), &surface, &redstatuses, __FILE__, __LINE__, 0);
  
  RedHandlePresent  present             = 0;
  RedAccessBitflags presentImagesAccess = RED_ACCESS_BITFLAG_STRUCT_RESOURCE_W | RED_ACCESS_BITFLAG_OUTPUT_COLOR_W;
  RedHandleImage    presentImages[2]    = {};
  RedHandleTexture  presentTextures[2]  = {};
  RedQueueFamilyIndexGetSupportsPresentOnSurface queueFamilyIndexSupportsPresentOnSurface = {};
  queueFamilyIndexSupportsPresentOnSurface.surface                                     = surface;
  queueFamilyIndexSupportsPresentOnSurface.outQueueFamilyIndexSupportsPresentOnSurface = 0;
  redQueueFamilyIndexGetSupportsPresent(context, context->gpus[0].gpu, 0, 0, 0, 0, &queueFamilyIndexSupportsPresentOnSurface, &redstatuses, __FILE__, __LINE__, 0);
  RedSurfaceCurrentPropertiesAndPresentLimits surfaceCurrentPropertiesAndPresentLimits = {};
  redSurfaceGetCurrentPropertiesAndPresentLimits(context, context->gpus[0].gpu, surface, &surfaceCurrentPropertiesAndPresentLimits, &redstatuses, __FILE__, __LINE__, 0);
  redCreatePresent(context, context->gpus[0].gpu, context->gpus[0].queues[0], 0, surface, 2, g_render_width, g_render_height, 1, presentImagesAccess, RED_SURFACE_TRANSFORM_BITFLAG_IDENTITY, RED_SURFACE_COMPOSITE_ALPHA_BITFLAG_OPAQUE, RED_PRESENT_VSYNC_MODE_ON, 0, 0, 0, &present, presentImages, presentTextures, &redstatuses, __FILE__, __LINE__, 0);

  RedHandleGpuSignal gpuSignalPresent = 0;
  redCreateGpuSignal(context, context->gpus[0].gpu, 0, &gpuSignalPresent, &redstatuses, __FILE__, __LINE__, 0);

  RedRteHandleHelperStructs structs = 0;
  rtestatus = redRteCreateHelperStructs(rteInfo.rteContext, &structs, &redstatuses, __FILE__, __LINE__, 0, 0);

  RedStructDeclarationMember structMembers[4] = {};

  structMembers[0].slot            = 0;
  structMembers[0].type            = RED_STRUCT_MEMBER_TYPE_TEXTURE_RW;
  structMembers[0].count           = 1;
  structMembers[0].visibleToStages = RED_VISIBLE_TO_STAGE_BITFLAG_COMPUTE;
  structMembers[0].inlineSampler   = 0;

  structMembers[1].slot            = 1;
  structMembers[1].type            = (RedStructMemberType)1000150000; // VK_DESCRIPTOR_TYPE_ACCELERATION_STRUCTURE_KHR
  structMembers[1].count           = 1;
  structMembers[1].visibleToStages = RED_VISIBLE_TO_STAGE_BITFLAG_COMPUTE;
  structMembers[1].inlineSampler   = 0;

  structMembers[2].slot            = 2;
  structMembers[2].type            = RED_STRUCT_MEMBER_TYPE_ARRAY_RO_RW;
  structMembers[2].count           = 1;
  structMembers[2].visibleToStages = RED_VISIBLE_TO_STAGE_BITFLAG_COMPUTE;
  structMembers[2].inlineSampler   = 0;

  structMembers[3].slot            = 3;
  structMembers[3].type            = RED_STRUCT_MEMBER_TYPE_ARRAY_RO_RW;
  structMembers[3].count           = 1;
  structMembers[3].visibleToStages = RED_VISIBLE_TO_STAGE_BITFLAG_COMPUTE;
  structMembers[3].inlineSampler   = 0;

  rtestatus = redRteHelperStructsSetStructDeclarationMembers(structs, sizeof(structMembers) / sizeof(structMembers[0]), structMembers, 0, 0, __FILE__, __LINE__, 0, 0);
  RedHandleStructDeclaration structDeclaration = 0;
  rtestatus = redRteHelperStructsCreateStructDeclaration(structs, 0, RED_RTE_HELPER_STRUCTS_API_SUPPORT_VK_VERSION_1_0, &structDeclaration, &redstatuses, __FILE__, __LINE__, 0, 0);
  RedHandleStructsMemory structsMemory = 0;
  rtestatus = redRteHelperStructsCreateStructsMemory(structs, 1, &structsMemory, &redstatuses, __FILE__, __LINE__, 0, 0);

  {
    RedRteHandleRayTracingAccelerationStructure tlas = redRteRayTracingBuilderGetTlas(rteBuilder, __FILE__, __LINE__, 0, 0);
    RedRteStructMemberRayTracingAccelerationStructure structMemberAccelerationStructure = {};
    structMemberAccelerationStructure.setTo1000150007 = 1000150007; // VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET_ACCELERATION_STRUCTURE_KHR
    structMemberAccelerationStructure.setTo0          = 0;
    structMemberAccelerationStructure.tlasesCount     = 1;
    structMemberAccelerationStructure.tlases          = &tlas;

    RedStructMemberArray structMemberArrayVertices = {};
    structMemberArrayVertices.array                = arrayVertices.handle;
    structMemberArrayVertices.arrayRangeBytesFirst = 0;
    structMemberArrayVertices.arrayRangeBytesCount = -1;

    RedStructMemberArray structMemberArrayIndices = {};
    structMemberArrayIndices.array                = arrayIndices.handle;
    structMemberArrayIndices.arrayRangeBytesFirst = 0;
    structMemberArrayIndices.arrayRangeBytesCount = -1;

    RedStructMember writeDescriptorSets[3] = {};

    writeDescriptorSets[0].setTo35   = 35;
    writeDescriptorSets[0].setTo0    = (uint64_t)(void *)&structMemberAccelerationStructure;
    writeDescriptorSets[0].structure = redRteHelperStructsGetStruct(structs, 0, __FILE__, __LINE__, 0, 0);
    writeDescriptorSets[0].slot      = 1;
    writeDescriptorSets[0].first     = 0;
    writeDescriptorSets[0].count     = 1;
    writeDescriptorSets[0].type      = (RedStructMemberType)1000150000; // VK_DESCRIPTOR_TYPE_ACCELERATION_STRUCTURE_KHR
    writeDescriptorSets[0].textures  = 0;
    writeDescriptorSets[0].arrays    = 0;
    writeDescriptorSets[0].setTo00   = 0;

    writeDescriptorSets[1].setTo35   = 35;
    writeDescriptorSets[1].setTo0    = 0;
    writeDescriptorSets[1].structure = redRteHelperStructsGetStruct(structs, 0, __FILE__, __LINE__, 0, 0);
    writeDescriptorSets[1].slot      = 2;
    writeDescriptorSets[1].first     = 0;
    writeDescriptorSets[1].count     = 1;
    writeDescriptorSets[1].type      = RED_STRUCT_MEMBER_TYPE_ARRAY_RO_RW;
    writeDescriptorSets[1].textures  = 0;
    writeDescriptorSets[1].arrays    = &structMemberArrayVertices;
    writeDescriptorSets[1].setTo00   = 0;

    writeDescriptorSets[2].setTo35   = 35;
    writeDescriptorSets[2].setTo0    = 0;
    writeDescriptorSets[2].structure = redRteHelperStructsGetStruct(structs, 0, __FILE__, __LINE__, 0, 0);
    writeDescriptorSets[2].slot      = 3;
    writeDescriptorSets[2].first     = 0;
    writeDescriptorSets[2].count     = 1;
    writeDescriptorSets[2].type      = RED_STRUCT_MEMBER_TYPE_ARRAY_RO_RW;
    writeDescriptorSets[2].textures  = 0;
    writeDescriptorSets[2].arrays    = &structMemberArrayIndices;
    writeDescriptorSets[2].setTo00   = 0;

    redStructsSet(context, context->gpus[0].gpu, sizeof(writeDescriptorSets) / sizeof(writeDescriptorSets[0]), writeDescriptorSets, __FILE__, __LINE__, 0);
  }

  std::string rayTraceModuleCodeString = LoadFile("raytrace.comp.glsl.spv", true);
  assert(rayTraceModuleCodeString != "");

  RedHandleGpuCode rayTraceGpuCode = 0;
  redCreateGpuCode(context, context->gpus[0].gpu, 0, rayTraceModuleCodeString.size(), rayTraceModuleCodeString.data(), &rayTraceGpuCode, &redstatuses, __FILE__, __LINE__, 0);
  
  RedProcedureParametersDeclaration procedureParametersDeclaration = {};
  procedureParametersDeclaration.variablesSlot            = 0;
  procedureParametersDeclaration.variablesVisibleToStages = RED_VISIBLE_TO_STAGE_BITFLAG_COMPUTE;
  procedureParametersDeclaration.variablesBytesCount      = 3 * sizeof(float);
  procedureParametersDeclaration.structsDeclarationsCount = 1;
  procedureParametersDeclaration.structsDeclarations[0]   = structDeclaration;
  procedureParametersDeclaration.structsDeclarations[1]   = 0;
  procedureParametersDeclaration.structsDeclarations[2]   = 0;
  procedureParametersDeclaration.structsDeclarations[3]   = 0;
  procedureParametersDeclaration.structsDeclarations[4]   = 0;
  procedureParametersDeclaration.structsDeclarations[5]   = 0;
  procedureParametersDeclaration.structsDeclarations[6]   = 0;
  procedureParametersDeclaration.handlesDeclaration       = 0;
  RedHandleProcedureParameters procedureParameters = 0;
  redCreateProcedureParameters(context, context->gpus[0].gpu, 0, &procedureParametersDeclaration, &procedureParameters, &redstatuses, __FILE__, __LINE__, 0);

  RedHandleProcedure computeProcedure = 0;
  redCreateProcedureCompute(context, context->gpus[0].gpu, 0, 0, procedureParameters, "main", rayTraceGpuCode, &computeProcedure, &redstatuses, __FILE__, __LINE__, 0);

  unsigned F = 0;
  unsigned f = 0;

  while (glfwWindowShouldClose(window) == 0) {
    glfwPollEvents();

    F = f;
    redPresentGetImageIndex(context, context->gpus[0].gpu, present, 0, gpuSignalPresent, &f, 0, __FILE__, __LINE__, 0);

    {
      RedStructMember writeDescriptorSets[1] = {};

      RedStructMemberTexture structMemberOutImage = {};
      structMemberOutImage.sampler = 0;
      structMemberOutImage.texture = presentTextures[f];
      structMemberOutImage.setTo1  = 1;

      writeDescriptorSets[0].setTo35   = 35;
      writeDescriptorSets[0].setTo0    = 0;
      writeDescriptorSets[0].structure = redRteHelperStructsGetStruct(structs, 0, __FILE__, __LINE__, 0, 0);
      writeDescriptorSets[0].slot      = 0;
      writeDescriptorSets[0].first     = 0;
      writeDescriptorSets[0].count     = 1;
      writeDescriptorSets[0].type      = RED_STRUCT_MEMBER_TYPE_TEXTURE_RW;
      writeDescriptorSets[0].textures  = &structMemberOutImage;
      writeDescriptorSets[0].arrays    = 0;
      writeDescriptorSets[0].setTo00   = 0;

      redStructsSet(context, context->gpus[0].gpu, sizeof(writeDescriptorSets) / sizeof(writeDescriptorSets[0]), writeDescriptorSets, __FILE__, __LINE__, 0);
    }

    RedCalls cmdBuffer = CallsCreateAndSet(context, &redstatuses);

    RedCallProceduresAndAddresses callPAs = {};
    redGetCallProceduresAndAddresses(context, context->gpus[0].gpu, &callPAs, &redstatuses, __FILE__, __LINE__, 0);

    {
      RedUsageImage usageOutImage = {};
      usageOutImage.barrierSplit           = RED_BARRIER_SPLIT_NONE;
      usageOutImage.oldAccessStages        = 0;
      usageOutImage.newAccessStages        = RED_ACCESS_STAGE_BITFLAG_COMPUTE;
      usageOutImage.oldAccess              = 0;
      usageOutImage.newAccess              = RED_ACCESS_BITFLAG_STRUCT_RESOURCE_W;
      usageOutImage.oldState               = RED_STATE_UNUSABLE;
      usageOutImage.newState               = RED_STATE_USABLE;
      usageOutImage.queueFamilyIndexSource = -1;
      usageOutImage.queueFamilyIndexTarget = -1;
      usageOutImage.image                  = presentImages[f];
      usageOutImage.imageAllParts          = RED_IMAGE_PART_BITFLAG_COLOR;
      usageOutImage.imageLevelsFirst       = 0;
      usageOutImage.imageLevelsCount       = -1;
      usageOutImage.imageLayersFirst       = 0;
      usageOutImage.imageLayersCount       = -1;
      redCallUsageAliasOrderBarrier(callPAs.redCallUsageAliasOrderBarrier, cmdBuffer.handle, context, 0, 0, 1, &usageOutImage, 0, 0, 0, 0, 0);
    }

    redCallSetProcedureParameters(callPAs.redCallSetProcedureParameters, cmdBuffer.handle, RED_PROCEDURE_TYPE_COMPUTE, procedureParameters);
    redCallSetStructsMemory(callPAs.redCallSetStructsMemory, cmdBuffer.handle, structsMemory, 0);
    callPAs.redCallSetProcedureParametersStructs(cmdBuffer.handle, RED_PROCEDURE_TYPE_COMPUTE, procedureParameters, 0, 1, redRteHelperStructsGetStructs(structs, __FILE__, __LINE__, 0, 0), 0, 0);
    callPAs.redCallSetProcedure(cmdBuffer.handle, RED_PROCEDURE_TYPE_COMPUTE, computeProcedure);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
      g_camera_pos_z -= 0.1f;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
      g_camera_pos_x -= 0.1f;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
      g_camera_pos_z += 0.1f;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
      g_camera_pos_x += 0.1f;
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
      g_camera_pos_y += 0.1f;
    }
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
      g_camera_pos_y -= 0.1f;
    }
    float cameraPosition[3] = {};
    cameraPosition[0] = g_camera_pos_x;
    cameraPosition[1] = g_camera_pos_y;
    cameraPosition[2] = g_camera_pos_z;
    callPAs.redCallSetProcedureParametersVariables(cmdBuffer.handle, procedureParameters, RED_VISIBLE_TO_STAGE_BITFLAG_COMPUTE, 0, 3 * sizeof(float), cameraPosition);
    callPAs.redCallProcedureCompute(
      cmdBuffer.handle,
      (uint32_t(g_render_width)  + g_workgroup_width  - 1) / g_workgroup_width,
      (uint32_t(g_render_height) + g_workgroup_height - 1) / g_workgroup_height,
      1
    );

    {
      RedUsageImage usageOutImage = {};
      usageOutImage.barrierSplit           = RED_BARRIER_SPLIT_NONE;
      usageOutImage.oldAccessStages        = RED_ACCESS_STAGE_BITFLAG_COMPUTE;
      usageOutImage.newAccessStages        = 0;
      usageOutImage.oldAccess              = RED_ACCESS_BITFLAG_STRUCT_RESOURCE_W;
      usageOutImage.newAccess              = 0;
      usageOutImage.oldState               = RED_STATE_USABLE;
      usageOutImage.newState               = RED_STATE_PRESENT;
      usageOutImage.queueFamilyIndexSource = -1;
      usageOutImage.queueFamilyIndexTarget = -1;
      usageOutImage.image                  = presentImages[f];
      usageOutImage.imageAllParts          = RED_IMAGE_PART_BITFLAG_COLOR;
      usageOutImage.imageLevelsFirst       = 0;
      usageOutImage.imageLevelsCount       = -1;
      usageOutImage.imageLayersFirst       = 0;
      usageOutImage.imageLayersCount       = -1;
      redCallUsageAliasOrderBarrier(callPAs.redCallUsageAliasOrderBarrier, cmdBuffer.handle, context, 0, 0, 1, &usageOutImage, 0, 0, 0, 0, 0);
    }

    // End and submit the command buffer, then wait for it to finish
    CallsEndSubmitWaitAndDestroy(context, cmdBuffer, gpuSignalPresent, &redstatuses);

    redQueuePresent(context, context->gpus[0].gpu, context->gpus[0].queues[0], 1, &gpuSignalPresent, 1, &present, &f, 0, &redstatuses, __FILE__, __LINE__, 0);

    f += 1;
    f %= 2;
  }

  redQueuePresent(context, context->gpus[0].gpu, context->gpus[0].queues[0], 0, 0, 0, 0, 0, 0, &redstatuses, __FILE__, __LINE__, 0);
  redDestroyGpuSignal(context, context->gpus[0].gpu, gpuSignalPresent, __FILE__, __LINE__, 0);
  redDestroyPresent(context, context->gpus[0].gpu, present, __FILE__, __LINE__, 0);
  redDestroyTexture(context, context->gpus[0].gpu, presentTextures[0], __FILE__, __LINE__, 0);
  redDestroyTexture(context, context->gpus[0].gpu, presentTextures[1], __FILE__, __LINE__, 0);
  redDestroySurface(context, context->gpus[0].gpu, surface, __FILE__, __LINE__, 0);
  redDestroyProcedure(context, context->gpus[0].gpu, computeProcedure, __FILE__, __LINE__, 0);
  redDestroyGpuCode(context, context->gpus[0].gpu, rayTraceGpuCode, __FILE__, __LINE__, 0);
  redDestroyProcedureParameters(context, context->gpus[0].gpu, procedureParameters, __FILE__, __LINE__, 0);
  rtestatus = redRteDestroyHelperStructs(structs, __FILE__, __LINE__, 0, 0);
  rtestatus = redRteDestroyRayTracingBuilder(rteBuilder, __FILE__, __LINE__, 0, 0);
  rtestatus = redRteHelperAllocatorVmaDestroyArray(allocator, arrayVertices.handle, arrayVertices.rteHelperAllocatorResourceMemory, &redstatuses, __FILE__, __LINE__, 0, 0);
  rtestatus = redRteHelperAllocatorVmaDestroyArray(allocator, arrayIndices.handle, arrayIndices.rteHelperAllocatorResourceMemory, &redstatuses, __FILE__, __LINE__, 0, 0);
  rtestatus = redRteDestroyHelperAllocatorVma(allocator, __FILE__, __LINE__, 0, 0);
  RedRteDestroyContextParameters rteDestroyContextInfo = {};
  rteDestroyContextInfo.reservedStructId = 0;
  rteDestroyContextInfo.rteContext       = rteInfo.rteContext;
  rtestatus = redRteDestroyContext(context, __FILE__, __LINE__, 0, &rteDestroyContextInfo);

  glfwTerminate();
}
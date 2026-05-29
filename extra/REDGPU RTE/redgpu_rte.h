#pragma once

#include "C:/RedGpuSDK/redgpu.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct RedRteTypeHandleContext                         * RedRteHandleContext;
typedef struct RedRteTypeHandleHelperAllocatorDma              * RedRteHandleHelperAllocatorDma;
typedef struct RedRteTypeHandleHelperAllocatorVma              * RedRteHandleHelperAllocatorVma;
typedef struct RedRteTypeHandleHelperStructs                   * RedRteHandleHelperStructs;
typedef struct RedRteTypeHandleRayTracingBuilder               * RedRteHandleRayTracingBuilder;
typedef struct RedRteTypeHandleRayTracingAccelerationStructure * RedRteHandleRayTracingAccelerationStructure;
typedef struct RedRteTypeHandleDeferredOperation               * RedRteHandleDeferredOperation;
typedef struct RedRteTypeHandleHelperShaderBindingTable        * RedRteHandleHelperShaderBindingTable;
typedef struct RedRteTypeHandleHelperAppWindow                 * RedRteHandleHelperAppWindow;
typedef struct RedRteTypeHandleHelperGBuffer                   * RedRteHandleHelperGBuffer;

typedef struct RedRteQueue {
  RedHandleQueue handle;
  unsigned       queueFamilyIndex;
  unsigned       queueIndex;
  float          queuePriority;
} RedRteQueue;

typedef unsigned RedRteQueueBitflags;
typedef enum RedRteQueueBitflag {
  RED_RTE_QUEUE_BITFLAG_CAN_DRAW    = REDGPU_B32(0000,0000,0000,0000,0000,0000,0000,0001),
  RED_RTE_QUEUE_BITFLAG_CAN_COMPUTE = REDGPU_B32(0000,0000,0000,0000,0000,0000,0000,0010),
  RED_RTE_QUEUE_BITFLAG_CAN_COPY    = REDGPU_B32(0000,0000,0000,0000,0000,0000,0000,0100),
} RedRteQueueBitflag;

typedef struct RedRteCreateContextParameters {
  uint64_t              reservedStructId;
  unsigned              gpuIndex;
  unsigned              vkVersionMajor;                        // Suggested default value is 1
  unsigned              vkVersionMinor;                        // Suggested default value is 2
  uint64_t              vkInstanceExtensionsCount;
  const char **         vkInstanceExtensions;
  uint64_t              vkInstanceLayersCount;
  const char **         vkInstanceLayers;
  uint64_t              vkDeviceExtensionsCount;               // Suggested default value is 3
  const char **         vkDeviceExtensions;                    // Suggested default value is ["VK_KHR_deferred_host_operations", "VK_KHR_acceleration_structure", "VK_KHR_ray_query"]
  void **               vkDeviceExtensionsOutVkFeaturesStruct;
  unsigned *            vkDeviceExtensionsVersion;
  RedBool32             disableRobustBufferAccess;             // Suggested default value is 1
  RedBool32             verboseCompatibleDevices;              // Suggested default value is 1
  RedBool32             verboseUsed;                           // Suggested default value is 1
  RedBool32             verboseAvailable;                      // Suggested default value is 1
  RedBool32             enableAftermath;                       // Suggested default value is 1
  uint64_t              ignoreDebugMessagesCount;
  int *                 ignoreDebugMessages;
  int                   debugSeverityFilterMask;               // Suggested default value is VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT
  uint64_t              requestQueueFamilysCount;
  RedRteQueueBitflags * requestQueueFamilysFlags;              // Suggested default value is [RED_RTE_QUEUE_BITFLAG_CAN_DRAW | RED_RTE_QUEUE_BITFLAG_CAN_COMPUTE | RED_RTE_QUEUE_BITFLAG_CAN_COPY, ...]
  unsigned *            requestQueueFamilysQueuesCount;
  float *               requestQueueFamilysPriority;
  RedRteHandleContext   rteContext;
  void *                getInstanceProcAddr;
} RedRteCreateContextParameters;

typedef struct RedRteDestroyContextParameters {
  uint64_t            reservedStructId;
  RedRteHandleContext rteContext;
} RedRteDestroyContextParameters;

typedef enum RedRteGeometryType {
  RED_RTE_GEOMETRY_TYPE_TRIANGLES = 0,
  RED_RTE_GEOMETRY_TYPE_AABBS     = 1,
  RED_RTE_GEOMETRY_TYPE_INSTANCES = 2,
} RedRteGeometryType;

typedef union RedRteGpuOrCpuAddress {
  uint64_t gpuAddress;
  void *   cpuAddress;
} RedRteGpuOrCpuAddress;

// Helper allocator

typedef struct RedRteHelperArray {
  RedHandleArray handle;
  void *         rteHelperAllocatorResourceMemory;
} RedRteHelperArray;

typedef struct RedRteHelperImage {
  RedHandleImage handle;
  void *         rteHelperAllocatorResourceMemory;
} RedRteHelperImage;

typedef struct RedRteHelperCreateArrayInfo {
  unsigned setTo12;
  size_t   setTo0;
  unsigned createFlags; // VkBufferCreateFlags
  uint64_t bytesCount;
  unsigned usageFlags;  // VkBufferUsageFlags
  unsigned setTo00;
  unsigned setTo000;
  size_t   setTo0000;
} RedRteHelperCreateArrayInfo;

typedef struct RedRteHelperCreateImageInfo {
  unsigned                   setTo14;
  size_t                     setTo0;
  unsigned                   createFlags;      // VkImageCreateFlags
  unsigned                   type;             // VkImageType
  RedFormat                  format;
  unsigned                   width;
  unsigned                   height;
  unsigned                   depth;
  unsigned                   levelsCount;
  unsigned                   layersCount;
  RedMultisampleCountBitflag multisampleCount;
  unsigned                   setTo00;
  unsigned                   usageFlags;       // VkImageUsageFlags
  unsigned                   setTo000;
  unsigned                   setTo0000;
  size_t                     setTo00000;
  unsigned                   setTo000000;
} RedRteHelperCreateImageInfo;

// Helper structs

typedef enum RedRteHelperStructsApiSupport {
  RED_RTE_HELPER_STRUCTS_API_SUPPORT_VK_VERSION_1_0             = 0,
  RED_RTE_HELPER_STRUCTS_API_SUPPORT_VK_VERSION_1_2             = 1,
  RED_RTE_HELPER_STRUCTS_API_SUPPORT_VK_EXT_DESCRIPTOR_INDEXING = 2,
} RedRteHelperStructsApiSupport;

// Ray tracing

typedef struct RedRteRayTracingAccelerationStructureGeometryTrianglesData {
  unsigned              setTo1000150005;
  size_t                setTo0;
  unsigned              vertexFormat;    // VkFormat
  RedRteGpuOrCpuAddress vertexData;
  uint64_t              vertexStride;
  unsigned              maxVertex;
  unsigned              indexType;       // VkIndexType
  RedRteGpuOrCpuAddress indexData;
  RedRteGpuOrCpuAddress transformData;
} RedRteRayTracingAccelerationStructureGeometryTrianglesData;

typedef struct RedRteRayTracingAccelerationStructureGeometryAabbsData {
  unsigned              setTo1000150003;
  size_t                setTo0;
  RedRteGpuOrCpuAddress data;
  uint64_t              stride;
} RedRteRayTracingAccelerationStructureGeometryAabbsData;

typedef struct RedRteRayTracingAccelerationStructureGeometryInstancesData {
  unsigned              setTo1000150004;
  size_t                setTo0;
  RedBool32             arrayOfPointers;
  RedRteGpuOrCpuAddress data;
} RedRteRayTracingAccelerationStructureGeometryInstancesData;

typedef union RedRteRayTracingAccelerationStructureGeometryData {
  RedRteRayTracingAccelerationStructureGeometryTrianglesData triangles;
  RedRteRayTracingAccelerationStructureGeometryAabbsData     aabbs;
  RedRteRayTracingAccelerationStructureGeometryInstancesData instances;
} RedRteRayTracingAccelerationStructureGeometryData;

typedef struct RedRteRayTracingAccelerationStructureGeometry {
  unsigned                                          setTo1000150006;
  size_t                                            setTo0;
  RedRteGeometryType                                geometryType;
  RedRteRayTracingAccelerationStructureGeometryData geometry;
  unsigned                                          geometryFlags;   // VkGeometryFlagsKHR
} RedRteRayTracingAccelerationStructureGeometry;

typedef struct RedRteRayTracingAccelerationStructureBuildRangeInfo {
  unsigned primitiveCount;
  unsigned primitiveOffset;
  unsigned vertexFirst;
  unsigned transformOffset;
} RedRteRayTracingAccelerationStructureBuildRangeInfo;

typedef struct RedRteRayTracingBuilderBlasInput {
  uint64_t                                              geometrysCount;
  RedRteRayTracingAccelerationStructureGeometry *       geometrys;
  uint64_t                                              buildRangeInfosCount;
  RedRteRayTracingAccelerationStructureBuildRangeInfo * buildRangeInfos;
  unsigned                                              flags;                // VkBuildRayTracingAccelerationStructureFlagsKHR
} RedRteRayTracingBuilderBlasInput;

typedef struct RedRteStructMemberRayTracingAccelerationStructure {
  unsigned                                            setTo1000150007;
  size_t                                              setTo0;
  unsigned                                            tlasesCount;
  const RedRteHandleRayTracingAccelerationStructure * tlases;
} RedRteStructMemberRayTracingAccelerationStructure;

typedef struct RedRteTransformMatrix {
  float matrix[3][4];
} RedRteTransformMatrix;

typedef struct RedRteRayTracingAccelerationStructureInstance {
  RedRteTransformMatrix transform;
  unsigned              instanceCustomIndex:24;
  unsigned              mask:8;
  unsigned              instanceShaderBindingTableRecordOffset:24;
  unsigned              flags:8;                                   // VkGeometryInstanceFlagsKHR
  uint64_t              accelerationStructureReference;
} RedRteRayTracingAccelerationStructureInstance;

// Ray tracing procedure

typedef enum RedRteProcedureRayTracingGroupStage {
  RED_RTE_PROCEDURE_RAY_TRACING_GROUP_STAGE_GENERAL      = 0,
  RED_RTE_PROCEDURE_RAY_TRACING_GROUP_STAGE_CLOSEST_HIT  = 1,
  RED_RTE_PROCEDURE_RAY_TRACING_GROUP_STAGE_ANY_HIT      = 2,
  RED_RTE_PROCEDURE_RAY_TRACING_GROUP_STAGE_INTERSECTION = 3,
} RedRteProcedureRayTracingGroupStage;

typedef struct RedRteProcedureRayTracingStage {
  unsigned         setTo18;
  size_t           setTo0;
  unsigned         flags;              // VkPipelineShaderStageCreateFlags
  unsigned         stage;              // VkShaderStageFlagBits
  RedHandleGpuCode gpuCode;
  const char *     mainProcedureName;
  const void *     specializationInfo; // VkSpecializationInfo
} RedRteProcedureRayTracingStage;

typedef struct RedRteProcedureRayTracingGroup {
  unsigned     setTo1000150016;
  size_t       setTo0;
  unsigned     type;                   // VkRayTracingShaderGroupTypeKHR
  unsigned     stageIndexGeneral;      // Set to max value to ignore
  unsigned     stageIndexClosestHit;   // Set to max value to ignore
  unsigned     stageIndexAnyHit;       // Set to max value to ignore
  unsigned     stageIndexIntersection; // Set to max value to ignore
  const void * captureReplayHandle;
} RedRteProcedureRayTracingGroup;

typedef struct RedRteProcedureRayTracingDynamicState {
  unsigned         setTo27;
  size_t           setTo0;
  unsigned         flags;              // VkPipelineDynamicStateCreateFlags
  unsigned         dynamicStatesCount;
  const unsigned * dynamicStates;      // VkDynamicState
} RedRteProcedureRayTracingDynamicState;

typedef struct RedRteProcedureRayTracingState {
  unsigned                                      setTo1000150015;
  size_t                                        setTo0;
  unsigned                                      flags;                // VkPipelineCreateFlags
  unsigned                                      stagesCount;
  const RedRteProcedureRayTracingStage *        stages;
  unsigned                                      groupsCount;
  const RedRteProcedureRayTracingGroup *        groups;
  unsigned                                      maxRayRecursionDepth;
  size_t                                        setTo00;
  size_t                                        setTo000;
  const RedRteProcedureRayTracingDynamicState * dynamicState;
  RedHandleProcedureParameters                  procedureParameters;
  RedHandleProcedure                            deriveFrom;
  int                                           setToMinus1;
} RedRteProcedureRayTracingState;

typedef struct RedRteGpuAddressStrideAndRange {
  uint64_t gpuAddress;
  uint64_t bytesStride;
  uint64_t bytesCount;
} RedRteGpuAddressStrideAndRange;

// Helper app

typedef struct RedRteHelperAppCallbacks {
  void (*onAttach)(RedRteHandleHelperAppWindow helperAppWindow);
  void (*onDetach)();
  void (*onResize)(unsigned width, unsigned height);
  void (*onUiRender)();
  void (*onRender)(RedHandleCalls calls);
  void (*onFileDrop)(const char * filename);
} RedRteHelperAppCallbacks;

// Various

typedef struct RedRteUsageArray {
  RedBarrierSplit      barrierSplit;
  unsigned             oldAccessStages;        // VkPipelineStageFlags
  unsigned             newAccessStages;        // VkPipelineStageFlags
  unsigned             oldAccess;              // VkAccessFlags
  unsigned             newAccess;              // VkAccessFlags
  unsigned             queueFamilyIndexSource; // Set to max value to ignore
  unsigned             queueFamilyIndexTarget; // Set to max value to ignore
  RedHandleArray       array;
  uint64_t             arrayBytesFirst;
  uint64_t             arrayBytesCount;
} RedRteUsageArray;

typedef struct RedRteUsageImage {
  RedBarrierSplit      barrierSplit;
  unsigned             oldAccessStages;        // VkPipelineStageFlags
  unsigned             newAccessStages;        // VkPipelineStageFlags
  unsigned             oldAccess;              // VkAccessFlags
  unsigned             newAccess;              // VkAccessFlags
  unsigned             oldState;               // VkImageLayout
  unsigned             newState;               // VkImageLayout
  unsigned             queueFamilyIndexSource; // Set to max value to ignore
  unsigned             queueFamilyIndexTarget; // Set to max value to ignore
  RedHandleImage       image;
  unsigned             imageAllParts;          // VkImageAspectFlags
  unsigned             imageLevelsFirst;
  unsigned             imageLevelsCount;
  unsigned             imageLayersFirst;
  unsigned             imageLayersCount;
} RedRteUsageImage;

#ifndef REDGPU_RTE_DECLSPEC
#define REDGPU_RTE_DECLSPEC
#endif

#ifndef REDGPU_RTE_API
#define REDGPU_RTE_API
#endif

REDGPU_RTE_DECLSPEC int64_t  REDGPU_RTE_API redRteCreateContext      (RedTypeProcedureMalloc mallocFn, RedTypeProcedureFree freeFn, RedTypeProcedureMallocTagged optionalMallocTagged, RedTypeProcedureFreeTagged optionalFreeTagged, RedTypeProcedureDebugCallback debugCallback, RedSdkVersion sdkVersion, unsigned sdkExtensionsCount, const unsigned * sdkExtensions, const char * optionalProgramName, unsigned optionalProgramVersion, const char * optionalEngineName, unsigned optionalEngineVersion, const void * optionalSettings, RedContext * outContext, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData, RedRteCreateContextParameters * rteParameters);
REDGPU_RTE_DECLSPEC int64_t  REDGPU_RTE_API redRteDestroyContext     (RedContext context, const char * optionalFile, int optionalLine, void * optionalUserData, RedRteDestroyContextParameters * rteParameters);
REDGPU_RTE_DECLSPEC int64_t  REDGPU_RTE_API redRteCreateQueue        (RedRteHandleContext rteContext, const char * handleName, RedRteQueueBitflags queueFlags, float queuePriority, RedRteQueue * outRteQueue, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters);
REDGPU_RTE_DECLSPEC uint64_t REDGPU_RTE_API redRteArrayGetGpuAddress (RedRteHandleContext rteContext, RedHandleGpu gpu, RedHandleArray array, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters);

// Helper allocator DMA

REDGPU_RTE_DECLSPEC int64_t  REDGPU_RTE_API redRteCreateHelperAllocatorDma                          (RedRteHandleContext rteContext, RedRteHandleHelperAllocatorDma * outHelperAllocatorDma, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters);
REDGPU_RTE_DECLSPEC int64_t  REDGPU_RTE_API redRteDestroyHelperAllocatorDma                         (RedRteHandleHelperAllocatorDma helperAllocatorDma, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters);

REDGPU_RTE_DECLSPEC int64_t  REDGPU_RTE_API redRteHelperAllocatorDmaCreateArray                     (RedRteHandleHelperAllocatorDma helperAllocatorDma, const RedRteHelperCreateArrayInfo * arrayInfo, unsigned memoryPropertyFlagsSuggestedDefaultValueIsMemoryPropertyDeviceLocalBit, RedRteHelperArray * outRteHelperArray, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters);
REDGPU_RTE_DECLSPEC int64_t  REDGPU_RTE_API redRteHelperAllocatorDmaCreateImage                     (RedRteHandleHelperAllocatorDma helperAllocatorDma, const RedRteHelperCreateImageInfo * imageInfo, unsigned memoryPropertyFlagsSuggestedDefaultValueIsMemoryPropertyDeviceLocalBit, RedRteHelperImage * outRteHelperImage, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters);
REDGPU_RTE_DECLSPEC int64_t  REDGPU_RTE_API redRteHelperAllocatorDmaCreateArraySimple               (RedRteHandleHelperAllocatorDma helperAllocatorDma, uint64_t arrayBytesCount, unsigned arrayUsageFlags, unsigned memoryPropertyFlagsSuggestedDefaultValueIsMemoryPropertyDeviceLocalBit, RedRteHelperArray * outRteHelperArray, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters);
REDGPU_RTE_DECLSPEC int64_t  REDGPU_RTE_API redRteHelperAllocatorDmaCreateArraySimpleWithData       (RedRteHandleHelperAllocatorDma helperAllocatorDma, RedHandleCalls callsSetToUploadArrayData, uint64_t arrayDataBytesCount, const void * arrayData, unsigned arrayUsageFlags, unsigned memoryPropertyFlagsSuggestedDefaultValueIsMemoryPropertyDeviceLocalBit, RedRteHelperArray * outRteHelperArray, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters);
REDGPU_RTE_DECLSPEC int64_t  REDGPU_RTE_API redRteHelperAllocatorDmaCreateImageSimpleWithData       (RedRteHandleHelperAllocatorDma helperAllocatorDma, RedHandleCalls callsSetToUploadImageData, uint64_t imageDataBytesCount, const void * imageData, const RedRteHelperCreateImageInfo * imageInfo, unsigned imageLayoutSuggestedDefaultValueIsImageLayoutGeneral, RedRteHelperImage * outRteHelperImage, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters);

REDGPU_RTE_DECLSPEC int64_t  REDGPU_RTE_API redRteHelperAllocatorDmaDestroyArray                    (RedRteHandleHelperAllocatorDma helperAllocatorDma, RedHandleArray rteHelperArray, void * rteHelperAllocatorResourceMemory, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters);
REDGPU_RTE_DECLSPEC int64_t  REDGPU_RTE_API redRteHelperAllocatorDmaDestroyImage                    (RedRteHandleHelperAllocatorDma helperAllocatorDma, RedHandleImage rteHelperImage, void * rteHelperAllocatorResourceMemory, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters);

REDGPU_RTE_DECLSPEC int64_t  REDGPU_RTE_API redRteHelperAllocatorDmaFinalizeAndReleaseStagingMemory (RedRteHandleHelperAllocatorDma helperAllocatorDma, RedHandleCpuSignal optionalCpuSignal, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters);
REDGPU_RTE_DECLSPEC int64_t  REDGPU_RTE_API redRteHelperAllocatorDmaFinalizeStagingMemory           (RedRteHandleHelperAllocatorDma helperAllocatorDma, RedHandleCpuSignal optionalCpuSignal, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters);
REDGPU_RTE_DECLSPEC int64_t  REDGPU_RTE_API redRteHelperAllocatorDmaReleaseStagingMemory            (RedRteHandleHelperAllocatorDma helperAllocatorDma, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters);

REDGPU_RTE_DECLSPEC int64_t  REDGPU_RTE_API redRteHelperAllocatorDmaMapArray                        (RedRteHandleHelperAllocatorDma helperAllocatorDma, RedHandleArray rteHelperArray, void * rteHelperAllocatorResourceMemory, void ** outVolatilePointer, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters);
REDGPU_RTE_DECLSPEC int64_t  REDGPU_RTE_API redRteHelperAllocatorDmaUnmapArray                      (RedRteHandleHelperAllocatorDma helperAllocatorDma, RedHandleArray rteHelperArray, void * rteHelperAllocatorResourceMemory, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters);

// Helper allocator VMA

REDGPU_RTE_DECLSPEC int64_t  REDGPU_RTE_API redRteCreateHelperAllocatorVma                          (RedRteHandleContext rteContext, RedRteHandleHelperAllocatorVma * outHelperAllocatorVma, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters);
REDGPU_RTE_DECLSPEC int64_t  REDGPU_RTE_API redRteDestroyHelperAllocatorVma                         (RedRteHandleHelperAllocatorVma helperAllocatorVma, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters);

REDGPU_RTE_DECLSPEC int64_t  REDGPU_RTE_API redRteHelperAllocatorVmaCreateArray                     (RedRteHandleHelperAllocatorVma helperAllocatorVma, const RedRteHelperCreateArrayInfo * arrayInfo, unsigned memoryPropertyFlagsSuggestedDefaultValueIsMemoryPropertyDeviceLocalBit, RedRteHelperArray * outRteHelperArray, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters);
REDGPU_RTE_DECLSPEC int64_t  REDGPU_RTE_API redRteHelperAllocatorVmaCreateImage                     (RedRteHandleHelperAllocatorVma helperAllocatorVma, const RedRteHelperCreateImageInfo * imageInfo, unsigned memoryPropertyFlagsSuggestedDefaultValueIsMemoryPropertyDeviceLocalBit, RedRteHelperImage * outRteHelperImage, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters);
REDGPU_RTE_DECLSPEC int64_t  REDGPU_RTE_API redRteHelperAllocatorVmaCreateArraySimple               (RedRteHandleHelperAllocatorVma helperAllocatorVma, uint64_t arrayBytesCount, unsigned arrayUsageFlags, unsigned memoryPropertyFlagsSuggestedDefaultValueIsMemoryPropertyDeviceLocalBit, RedRteHelperArray * outRteHelperArray, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters);
REDGPU_RTE_DECLSPEC int64_t  REDGPU_RTE_API redRteHelperAllocatorVmaCreateArraySimpleWithData       (RedRteHandleHelperAllocatorVma helperAllocatorVma, RedHandleCalls callsSetToUploadArrayData, uint64_t arrayDataBytesCount, const void * arrayData, unsigned arrayUsageFlags, unsigned memoryPropertyFlagsSuggestedDefaultValueIsMemoryPropertyDeviceLocalBit, RedRteHelperArray * outRteHelperArray, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters);
REDGPU_RTE_DECLSPEC int64_t  REDGPU_RTE_API redRteHelperAllocatorVmaCreateImageSimpleWithData       (RedRteHandleHelperAllocatorVma helperAllocatorVma, RedHandleCalls callsSetToUploadImageData, uint64_t imageDataBytesCount, const void * imageData, const RedRteHelperCreateImageInfo * imageInfo, unsigned imageLayoutSuggestedDefaultValueIsImageLayoutGeneral, RedRteHelperImage * outRteHelperImage, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters);

REDGPU_RTE_DECLSPEC int64_t  REDGPU_RTE_API redRteHelperAllocatorVmaDestroyArray                    (RedRteHandleHelperAllocatorVma helperAllocatorVma, RedHandleArray rteHelperArray, void * rteHelperAllocatorResourceMemory, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters);
REDGPU_RTE_DECLSPEC int64_t  REDGPU_RTE_API redRteHelperAllocatorVmaDestroyImage                    (RedRteHandleHelperAllocatorVma helperAllocatorVma, RedHandleImage rteHelperImage, void * rteHelperAllocatorResourceMemory, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters);

REDGPU_RTE_DECLSPEC int64_t  REDGPU_RTE_API redRteHelperAllocatorVmaFinalizeAndReleaseStagingMemory (RedRteHandleHelperAllocatorVma helperAllocatorVma, RedHandleCpuSignal optionalCpuSignal, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters);
REDGPU_RTE_DECLSPEC int64_t  REDGPU_RTE_API redRteHelperAllocatorVmaFinalizeStagingMemory           (RedRteHandleHelperAllocatorVma helperAllocatorVma, RedHandleCpuSignal optionalCpuSignal, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters);
REDGPU_RTE_DECLSPEC int64_t  REDGPU_RTE_API redRteHelperAllocatorVmaReleaseStagingMemory            (RedRteHandleHelperAllocatorVma helperAllocatorVma, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters);

REDGPU_RTE_DECLSPEC int64_t  REDGPU_RTE_API redRteHelperAllocatorVmaMapArray                        (RedRteHandleHelperAllocatorVma helperAllocatorVma, RedHandleArray rteHelperArray, void * rteHelperAllocatorResourceMemory, void ** outVolatilePointer, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters);
REDGPU_RTE_DECLSPEC int64_t  REDGPU_RTE_API redRteHelperAllocatorVmaUnmapArray                      (RedRteHandleHelperAllocatorVma helperAllocatorVma, RedHandleArray rteHelperArray, void * rteHelperAllocatorResourceMemory, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters);

// Helper structs

REDGPU_RTE_DECLSPEC int64_t                      REDGPU_RTE_API redRteCreateHelperStructs                           (RedRteHandleContext rteContext, RedRteHandleHelperStructs * outHelperStructs, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters);
REDGPU_RTE_DECLSPEC int64_t                      REDGPU_RTE_API redRteDestroyHelperStructs                          (RedRteHandleHelperStructs helperStructs, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters);

REDGPU_RTE_DECLSPEC int64_t                      REDGPU_RTE_API redRteHelperStructsSetStructDeclarationMembers      (RedRteHandleHelperStructs helperStructs, unsigned structDeclarationMembersCount, const RedStructDeclarationMember * structDeclarationMembers, unsigned structDeclarationMembersArrayROCount, const RedStructDeclarationMemberArrayRO * structDeclarationMembersArrayRO, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters);
REDGPU_RTE_DECLSPEC int64_t                      REDGPU_RTE_API redRteHelperStructsAddStructDeclarationMember       (RedRteHandleHelperStructs helperStructs, unsigned slot, unsigned type, unsigned count, unsigned visibleToStages, const RedHandleSampler * inlineSampler, RedBool32 slotTypeIsArrayRO, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters);
REDGPU_RTE_DECLSPEC int64_t                      REDGPU_RTE_API redRteHelperStructsSetStructDeclarationMemberFlags  (RedRteHandleHelperStructs helperStructs, unsigned slot, unsigned descriptorBindingFlags_RequiresApiSupportGreaterThanVkVersion10, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters);
REDGPU_RTE_DECLSPEC int64_t                      REDGPU_RTE_API redRteHelperStructsCreateStructDeclaration          (RedRteHandleHelperStructs helperStructs, unsigned descriptorSetLayoutCreateFlags, RedRteHelperStructsApiSupport apiSupport, RedHandleStructDeclaration * outStructDeclaration, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters);
REDGPU_RTE_DECLSPEC int64_t                      REDGPU_RTE_API redRteHelperStructsCreateStructsMemory              (RedRteHandleHelperStructs helperStructs, unsigned allocateStructsCount, RedHandleStructsMemory * outStructsMemory, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters);
REDGPU_RTE_DECLSPEC RedHandleStruct              REDGPU_RTE_API redRteHelperStructsGetStruct                        (RedRteHandleHelperStructs helperStructs, unsigned structIndex, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters);
REDGPU_RTE_DECLSPEC unsigned                     REDGPU_RTE_API redRteHelperStructsGetStructsCount                  (RedRteHandleHelperStructs helperStructs, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters);
REDGPU_RTE_DECLSPEC RedHandleStruct *            REDGPU_RTE_API redRteHelperStructsGetStructs                       (RedRteHandleHelperStructs helperStructs, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters);
REDGPU_RTE_DECLSPEC unsigned                     REDGPU_RTE_API redRteHelperStructsGetStructDeclarationMembersCount (RedRteHandleHelperStructs helperStructs, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters);
REDGPU_RTE_DECLSPEC RedStructDeclarationMember * REDGPU_RTE_API redRteHelperStructsGetStructDeclarationMembers      (RedRteHandleHelperStructs helperStructs, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters);
REDGPU_RTE_DECLSPEC int64_t                      REDGPU_RTE_API redRteHelperStructsDestroyStructDeclaration         (RedRteHandleHelperStructs helperStructs, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters);
REDGPU_RTE_DECLSPEC int64_t                      REDGPU_RTE_API redRteHelperStructsDestroyStructsMemory             (RedRteHandleHelperStructs helperStructs, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters);
REDGPU_RTE_DECLSPEC int64_t                      REDGPU_RTE_API redRteHelperStructsClear                            (RedRteHandleHelperStructs helperStructs, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters);

// Ray tracing

REDGPU_RTE_DECLSPEC int64_t                                     REDGPU_RTE_API redRteCreateRayTracingBuilder            (RedRteHandleContext rteContext, void * rteHandleHelperAllocator, unsigned queueFamilyIndex, RedRteHandleRayTracingBuilder * outRayTracingBuilder, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters);
REDGPU_RTE_DECLSPEC int64_t                                     REDGPU_RTE_API redRteDestroyRayTracingBuilder           (RedRteHandleRayTracingBuilder rayTracingBuilder, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters);

REDGPU_RTE_DECLSPEC int64_t                                     REDGPU_RTE_API redRteRayTracingBuilderBuildBlas         (RedRteHandleRayTracingBuilder rayTracingBuilder, unsigned blasInputsCount, const RedRteRayTracingBuilderBlasInput * blasInputs, unsigned buildAccelerationStructureFlagsSuggestedDefaultValueIsBuildAccelerationStructurePreferFastTraceBit, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters);
REDGPU_RTE_DECLSPEC uint64_t                                    REDGPU_RTE_API redRteRayTracingBuilderGetBlasGpuAddress (RedRteHandleRayTracingBuilder rayTracingBuilder, unsigned blasIndex, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters);
REDGPU_RTE_DECLSPEC int64_t                                     REDGPU_RTE_API redRteRayTracingBuilderBuildTlas         (RedRteHandleRayTracingBuilder rayTracingBuilder, unsigned instancesCount, const RedRteRayTracingAccelerationStructureInstance * instances, unsigned buildAccelerationStructureFlagsSuggestedDefaultValueIsBuildAccelerationStructurePreferFastTraceBit, RedBool32 update, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters);
REDGPU_RTE_DECLSPEC RedRteHandleRayTracingAccelerationStructure REDGPU_RTE_API redRteRayTracingBuilderGetTlas           (RedRteHandleRayTracingBuilder rayTracingBuilder, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters);
REDGPU_RTE_DECLSPEC int64_t                                     REDGPU_RTE_API redRteRayTracingBuilderUpdateBlas        (RedRteHandleRayTracingBuilder rayTracingBuilder, unsigned blasIndex, const RedRteRayTracingBuilderBlasInput * blasInput, unsigned buildAccelerationStructureFlagsSuggestedDefaultValueIsBuildAccelerationStructurePreferFastTraceBit, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters);

// Ray tracing procedure

REDGPU_RTE_DECLSPEC int64_t  REDGPU_RTE_API redRteCreateDeferredOperation                         (RedRteHandleContext rteContext, RedRteHandleDeferredOperation * outDeferredOperation, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters);
REDGPU_RTE_DECLSPEC int64_t  REDGPU_RTE_API redRteDestroyDeferredOperation                        (RedRteHandleContext rteContext, RedRteHandleDeferredOperation deferredOperation, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters);

REDGPU_RTE_DECLSPEC int64_t  REDGPU_RTE_API redRteDeferredOperationGetMaxConcurrency              (RedRteHandleContext rteContext, RedRteHandleDeferredOperation deferredOperation, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters);
REDGPU_RTE_DECLSPEC int64_t  REDGPU_RTE_API redRteDeferredOperationGetStatus                      (RedRteHandleContext rteContext, RedRteHandleDeferredOperation deferredOperation, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters);
REDGPU_RTE_DECLSPEC int64_t  REDGPU_RTE_API redRteDeferredOperationJoin                           (RedRteHandleContext rteContext, RedRteHandleDeferredOperation deferredOperation, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters);

REDGPU_RTE_DECLSPEC int64_t  REDGPU_RTE_API redRteCreateProcedureRayTracing                       (RedRteHandleContext rteContext, RedRteHandleDeferredOperation deferredOperation, RedHandleProcedureCache procedureCache, const RedRteProcedureRayTracingState * state, RedHandleProcedure * outProcedure, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters);

REDGPU_RTE_DECLSPEC int64_t  REDGPU_RTE_API redRteProcedureRayTracingGetGroupCaptureReplayHandles (RedRteHandleContext rteContext, RedHandleProcedure procedureRayTracing, unsigned groupIndexFirst, unsigned groupIndexCount, uint64_t dataBytesCount, void * data, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters);
REDGPU_RTE_DECLSPEC uint64_t REDGPU_RTE_API redRteProcedureRayTracingGetGroupStackBytesCount      (RedRteHandleContext rteContext, RedHandleProcedure procedureRayTracing, unsigned groupIndex, RedRteProcedureRayTracingGroupStage groupStage, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters);
REDGPU_RTE_DECLSPEC int64_t  REDGPU_RTE_API redRteCallProcedureRayTracing                         (RedHandleCalls calls, RedRteHandleContext rteContext, const RedRteGpuAddressStrideAndRange * shaderBindingTableRayGeneration, const RedRteGpuAddressStrideAndRange * shaderBindingTableMiss, const RedRteGpuAddressStrideAndRange * shaderBindingTableHit, const RedRteGpuAddressStrideAndRange * shaderBindingTableCallable, unsigned threadsCountX, unsigned threadsCountY, unsigned threadsCountZ, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters);
REDGPU_RTE_DECLSPEC int64_t  REDGPU_RTE_API redRteCallProcedureIndirect                           (RedHandleCalls calls, RedRteHandleContext rteContext, RedHandleArray array, uint64_t arrayBytesFirst, unsigned count, unsigned bytesStride, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters);
REDGPU_RTE_DECLSPEC int64_t  REDGPU_RTE_API redRteCallProcedureIndexedIndirect                    (RedHandleCalls calls, RedRteHandleContext rteContext, RedHandleArray array, uint64_t arrayBytesFirst, unsigned count, unsigned bytesStride, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters);
REDGPU_RTE_DECLSPEC int64_t  REDGPU_RTE_API redRteCallProcedureComputeIndirect                    (RedHandleCalls calls, RedRteHandleContext rteContext, RedHandleArray array, uint64_t arrayBytesFirst, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters);
REDGPU_RTE_DECLSPEC int64_t  REDGPU_RTE_API redRteCallProcedureRayTracingIndirect                 (RedHandleCalls calls, RedRteHandleContext rteContext, const RedRteGpuAddressStrideAndRange * shaderBindingTableRayGeneration, const RedRteGpuAddressStrideAndRange * shaderBindingTableMiss, const RedRteGpuAddressStrideAndRange * shaderBindingTableHit, const RedRteGpuAddressStrideAndRange * shaderBindingTableCallable, uint64_t gpuAddress, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters);
REDGPU_RTE_DECLSPEC int64_t  REDGPU_RTE_API redRteCallSetProcedureRayTracingStackBytesCount       (RedHandleCalls calls, RedRteHandleContext rteContext, unsigned stackBytesCount, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters);

// Helper shader binding table

REDGPU_RTE_DECLSPEC int64_t  REDGPU_RTE_API redRteCreateHelperShaderBindingTable                      (RedRteHandleContext rteContext, void * rteHandleHelperAllocator, unsigned queueFamilyIndex, RedRteHandleHelperShaderBindingTable * outHelperShaderBindingTable, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters);
REDGPU_RTE_DECLSPEC int64_t  REDGPU_RTE_API redRteDestroyHelperShaderBindingTable                     (RedRteHandleHelperShaderBindingTable helperShaderBindingTable, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters);

REDGPU_RTE_DECLSPEC int64_t  REDGPU_RTE_API redRteHelperShaderBindingTableCreate                      (RedRteHandleHelperShaderBindingTable helperShaderBindingTable, RedHandleProcedure procedureRayTracing, const RedRteProcedureRayTracingState * procedureRayTracingState, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters);
REDGPU_RTE_DECLSPEC int64_t  REDGPU_RTE_API redRteHelperShaderBindingTableGetGpuAddressStrideAndRange (RedRteHandleHelperShaderBindingTable helperShaderBindingTable, RedRteGpuAddressStrideAndRange * outShaderBindingTableRayGeneration, RedRteGpuAddressStrideAndRange * outShaderBindingTableMiss, RedRteGpuAddressStrideAndRange * outShaderBindingTableHit, RedRteGpuAddressStrideAndRange * outShaderBindingTableCallable, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters);

// Helper app

REDGPU_RTE_DECLSPEC void *              REDGPU_RTE_API redRteGetInstanceProcAddr                             (const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters);
REDGPU_RTE_DECLSPEC int64_t             REDGPU_RTE_API redRteCreateContextFromRteContext                     (RedRteHandleContext rteContext, RedTypeProcedureMalloc malloc, RedTypeProcedureFree free, RedTypeProcedureMallocTagged optionalMallocTagged, RedTypeProcedureFreeTagged optionalFreeTagged, RedTypeProcedureDebugCallback debugCallback, RedSdkVersion sdkVersion, unsigned sdkExtensionsCount, const unsigned * sdkExtensions, const char * optionalProgramName, unsigned optionalProgramVersion, const char * optionalEngineName, unsigned optionalEngineVersion, const void * optionalSettings, RedContext * outContext, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters);

REDGPU_RTE_DECLSPEC int64_t             REDGPU_RTE_API redRteCreateHelperAppWindow                           (const RedRteHelperAppCallbacks * callbacks, int initialWindowWidth, int initialWindowHeight, RedBool32 initialIsVsyncEnabled, float initialViewportClearColorR, float initialViewportClearColorG, float initialViewportClearColorB, float initialViewportClearColorA, const char * optionalProgramName, const char * optionalEngineName, RedRteCreateContextParameters * rteCreateContextParameters, RedRteHandleHelperAppWindow * outHelperAppWindow, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters);
REDGPU_RTE_DECLSPEC int64_t             REDGPU_RTE_API redRteDestroyHelperAppWindow                          (RedRteHandleHelperAppWindow helperAppWindow, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters);

REDGPU_RTE_DECLSPEC int64_t             REDGPU_RTE_API redRteHelperAppWindowRun                              (RedRteHandleHelperAppWindow helperAppWindow, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters);
REDGPU_RTE_DECLSPEC int64_t             REDGPU_RTE_API redRteHelperAppWindowClose                            (RedRteHandleHelperAppWindow helperAppWindow, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters);
REDGPU_RTE_DECLSPEC int64_t             REDGPU_RTE_API redRteHelperAppWindowSetViewport                      (RedRteHandleHelperAppWindow helperAppWindow, RedHandleCalls calls, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters);
REDGPU_RTE_DECLSPEC RedBool32           REDGPU_RTE_API redRteHelperAppWindowIsVsyncEnabled                   (RedRteHandleHelperAppWindow helperAppWindow, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters);
REDGPU_RTE_DECLSPEC int64_t             REDGPU_RTE_API redRteHelperAppWindowSetVsyncEnabled                  (RedRteHandleHelperAppWindow helperAppWindow, RedBool32 enableVsync, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters);
REDGPU_RTE_DECLSPEC int64_t             REDGPU_RTE_API redRteHelperAppWindowSetViewportClearColor            (RedRteHandleHelperAppWindow helperAppWindow, float viewportClearColorR, float viewportClearColorG, float viewportClearColorB, float viewportClearColorA, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters);
REDGPU_RTE_DECLSPEC int64_t             REDGPU_RTE_API redRteHelperAppWindowAddWaitSemaphore                 (RedRteHandleHelperAppWindow helperAppWindow, void * vkSemaphoreSubmitInfoKHR, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters);
REDGPU_RTE_DECLSPEC RedHandleCalls      REDGPU_RTE_API redRteHelperAppWindowTempCallsCreateAndSet            (RedRteHandleHelperAppWindow helperAppWindow, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters);
REDGPU_RTE_DECLSPEC int64_t             REDGPU_RTE_API redRteHelperAppWindowTempCallsEndSubmitWaitAndDestroy (RedRteHandleHelperAppWindow helperAppWindow, RedHandleCalls tempCalls, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters);
REDGPU_RTE_DECLSPEC int64_t             REDGPU_RTE_API redRteHelperAppWindowOnFileDrop                       (RedRteHandleHelperAppWindow helperAppWindow, const char * filename, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters);
REDGPU_RTE_DECLSPEC RedRteHandleContext REDGPU_RTE_API redRteHelperAppWindowGetRteContext                    (RedRteHandleHelperAppWindow helperAppWindow, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters);
REDGPU_RTE_DECLSPEC void *              REDGPU_RTE_API redRteHelperAppWindowGetGlfwWindow                    (RedRteHandleHelperAppWindow helperAppWindow, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters);
REDGPU_RTE_DECLSPEC int64_t             REDGPU_RTE_API redRteHelperAppWindowGetViewportWidth                 (RedRteHandleHelperAppWindow helperAppWindow, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters);
REDGPU_RTE_DECLSPEC int64_t             REDGPU_RTE_API redRteHelperAppWindowGetViewportHeight                (RedRteHandleHelperAppWindow helperAppWindow, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters);
REDGPU_RTE_DECLSPEC int64_t             REDGPU_RTE_API redRteHelperAppWindowGetWindowWidth                   (RedRteHandleHelperAppWindow helperAppWindow, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters);
REDGPU_RTE_DECLSPEC int64_t             REDGPU_RTE_API redRteHelperAppWindowGetWindowHeight                  (RedRteHandleHelperAppWindow helperAppWindow, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters);
REDGPU_RTE_DECLSPEC void *              REDGPU_RTE_API redRteHelperAppWindowGetImGuiImplVulkanHWindowData    (RedRteHandleHelperAppWindow helperAppWindow, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters);

// Helper gbuffer

REDGPU_RTE_DECLSPEC int64_t             REDGPU_RTE_API redRteCreateHelperGBuffer                 (RedRteHandleContext rteContext, void * rteHandleHelperAllocator, unsigned width, unsigned height, unsigned colorFormatsCount, unsigned * colorFormats, unsigned depthFormat, RedRteHandleHelperGBuffer * outHelperGBuffer, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters);
REDGPU_RTE_DECLSPEC int64_t             REDGPU_RTE_API redRteDestroyHelperGBuffer                (RedRteHandleHelperGBuffer helperGBuffer, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters);

REDGPU_RTE_DECLSPEC RedHandleStruct     REDGPU_RTE_API redRteHelperGBufferGetStruct              (RedRteHandleHelperGBuffer helperGBuffer, unsigned index, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters);
REDGPU_RTE_DECLSPEC int64_t             REDGPU_RTE_API redRteHelperGBufferGetWidth               (RedRteHandleHelperGBuffer helperGBuffer, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters);
REDGPU_RTE_DECLSPEC int64_t             REDGPU_RTE_API redRteHelperGBufferGetHeight              (RedRteHandleHelperGBuffer helperGBuffer, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters);
REDGPU_RTE_DECLSPEC RedHandleImage      REDGPU_RTE_API redRteHelperGBufferGetColorImage          (RedRteHandleHelperGBuffer helperGBuffer, unsigned index, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters);
REDGPU_RTE_DECLSPEC RedHandleImage      REDGPU_RTE_API redRteHelperGBufferGetDepthImage          (RedRteHandleHelperGBuffer helperGBuffer, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters);
REDGPU_RTE_DECLSPEC int64_t             REDGPU_RTE_API redRteHelperGBufferGetStructMemberTexture (RedRteHandleHelperGBuffer helperGBuffer, unsigned index, RedStructMemberTexture * outStructMemberTexture, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters);
REDGPU_RTE_DECLSPEC RedHandleTexture    REDGPU_RTE_API redRteHelperGBufferGetColorTexture        (RedRteHandleHelperGBuffer helperGBuffer, unsigned index, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters);
REDGPU_RTE_DECLSPEC RedHandleTexture    REDGPU_RTE_API redRteHelperGBufferGetDepthTexture        (RedRteHandleHelperGBuffer helperGBuffer, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters);
REDGPU_RTE_DECLSPEC unsigned            REDGPU_RTE_API redRteHelperGBufferGetColorFormat         (RedRteHandleHelperGBuffer helperGBuffer, unsigned index, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters);
REDGPU_RTE_DECLSPEC unsigned            REDGPU_RTE_API redRteHelperGBufferGetDepthFormat         (RedRteHandleHelperGBuffer helperGBuffer, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters);
REDGPU_RTE_DECLSPEC float               REDGPU_RTE_API redRteHelperGBufferGetAspectRatio         (RedRteHandleHelperGBuffer helperGBuffer, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters);

// Various

REDGPU_RTE_DECLSPEC int64_t             REDGPU_RTE_API redRteGpuWaitIdle                         (RedRteHandleContext rteContext, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters);
REDGPU_RTE_DECLSPEC int64_t             REDGPU_RTE_API redRteQueueWaitIdle                       (RedRteHandleContext rteContext, RedHandleQueue queue, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters);
REDGPU_RTE_DECLSPEC int64_t             REDGPU_RTE_API redRteArrayGetMemoryRequirements          (RedRteHandleContext rteContext, RedArray * inoutArray, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters);
REDGPU_RTE_DECLSPEC int64_t             REDGPU_RTE_API redRteImageGetMemoryRequirements          (RedRteHandleContext rteContext, RedImage * inoutImage, const char * optionalFile, int optionalLine, void * optionalUserData, void * rteParameters);
REDGPU_RTE_DECLSPEC int64_t             REDGPU_RTE_API redRteCallUsageAliasOrderBarrier          (RedHandleCalls calls, RedRteHandleContext rteContext, unsigned arrayUsagesCount, const RedRteUsageArray * arrayUsages, unsigned imageUsagesCount, const RedRteUsageImage * imageUsages, unsigned aliasesCount, const RedAlias * aliases, unsigned ordersCount, const RedOrder * orders, unsigned dependencyFlags);

#ifdef __cplusplus
}
#endif
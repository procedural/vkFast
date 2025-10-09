#pragma once

// Sources:
// https://github.com/redgpu/redgpu
// https://github.com/redgpu/redgpu2
#include "C:/RedGpuSDK/redgpu.h"
#include "C:/RedGpuSDK/redgpu_2.h"
#include "C:/RedGpuSDK/redgpu_32.h"
#include "C:/RedGpuSDK/redgpu_wsi.h"

#ifdef __cplusplus
extern "C" {
#endif

#define VKFAST_DEFAULT_MEMORY_ALLOCATION_SIZE_GPU_ONLY_512MB           (512 * 1024 * 1024)
#define VKFAST_DEFAULT_MEMORY_ALLOCATION_SIZE_CPU_UPLOAD_512MB         (512 * 1024 * 1024)
#define VKFAST_DEFAULT_MEMORY_ALLOCATION_SIZE_CPU_READBACK_512MB       (512 * 1024 * 1024)
#define VKFAST_DEFAULT_MEMORY_ALLOCATION_SIZE_PRESENT_CPU_UPLOAD_288MB (288 * 1024 * 1024)

typedef struct vf_handle_context_t {
  int                doNotDestroyRawContext;
  int                doNotFreeHandle;

  int                isDebugMode;
  
  RedContext         context;
  const RedGpuInfo * gpuInfo;

  unsigned           gpuIndex;
  RedHandleGpu       gpu;
  unsigned           mainQueueIndex;
  unsigned           mainQueueFamilyIndex;
  RedHandleQueue     mainQueue;

  unsigned           specificMemoryTypesGpuVram;
  unsigned           specificMemoryTypesCpuUpload;
  unsigned           specificMemoryTypesCpuReadback;

  // Memory

  Red2Memory         memoryGpuVramForArrays_memory;
  Red2Array          memoryGpuVramForArrays_array;
  uint64_t           memoryGpuVramForArrays_memory_suballocations_offset;

  Red2Array          memoryCpuUpload_memory_and_array;
  void *             memoryCpuUpload_mapped_void_ptr_original;
  void *             memoryCpuUpload_mapped_void_ptr_offset;
  uint64_t           memoryCpuUpload_memory_suballocations_offset;

  Red2Array          memoryCpuReadback_memory_and_array;
  void *             memoryCpuReadback_mapped_void_ptr_original;
  void *             memoryCpuReadback_mapped_void_ptr_offset;
  uint64_t           memoryCpuReadback_memory_suballocations_offset;

  // WSI

  void *             windowHandle;
  int                screenWidth;
  int                screenHeight;

  unsigned           presentQueueIndex;
  RedHandleSurface   surface;
  RedHandlePresent   present;
  RedHandleImage     presentImages[3];
  RedHandleGpuSignal presentGpuSignal;
  RedCalls           presentCopyCalls;
  uint64_t           presentPixelsCpuUpload_memory_allocation_size;
  Red2Array          presentPixelsCpuUpload_memory_and_array;
  void *             presentPixelsCpuUpload_void_ptr_original;
} vf_handle_context_t;

typedef struct vf_handle_storage_t {
  gpu_storage_info_t   info;           // NOTE(Constantine): Optional debug name is a stale pointer, do not use.
  RedStructMemberArray arrayRangeInfo; // NOTE(Constantine): Kept for GPU copy calls.
} vf_handle_storage_t;

typedef enum vf_gpu_code_type_t {
  VF_GPU_CODE_TYPE_INVALID = 0,
  VF_GPU_CODE_TYPE_COMPUTE = 1,
} vf_gpu_code_type_t;

typedef struct vf_handle_gpu_code_t {
  gpu_program_info_t info;        // NOTE(Constantine): Program binary is a stale pointer, do not use.
  vf_gpu_code_type_t gpuCodeType;
  RedHandleGpuCode   gpuCode;     // NOTE(Constantine): Kept to be destroyed.
} vf_handle_gpu_code_t;

typedef enum vf_procedure_type_t {
  VF_PROCEDURE_TYPE_INVALID = 0,
  VF_PROCEDURE_TYPE_COMPUTE = 1,
} vf_procedure_type_t;

typedef struct vf_handle_procedure_t {
  union {
    gpu_program_pipeline_compute_info_t infoCompute; // NOTE(Constantine): Optional debug name is a stale pointer, do not use.
  };
  vf_procedure_type_t          procedureType;
  RedHandleProcedureParameters procedureParameters;
  RedHandleProcedure           procedure;
} vf_handle_procedure_t;

typedef struct vf_handle_batch_t {
  RedCalls                      calls;
  RedCallProceduresAndAddresses addresses;
  RedHandleStructsMemory        structsMemory;
  Red2Struct                    currentStruct;              // NOTE(Constantine): Only one struct for now.
  RedHandleProcedureParameters  currentProcedureParameters; // NOTE(Constantine): Same parameters for both graphics and compute procedures.
} vf_handle_batch_t;

typedef enum vf_handle_id_t {
  VF_HANDLE_ID_INVALID   = 0,
  VF_HANDLE_ID_STORAGE   = 1,
  VF_HANDLE_ID_GPU_CODE  = 2,
  VF_HANDLE_ID_PROCEDURE = 3,
  VF_HANDLE_ID_BATCH     = 4,
} vf_handle_id_t;

typedef struct vf_handle_t {
  vf_handle_context_t * vkfast;
  vf_handle_id_t        handle_id;
  union {
    vf_handle_storage_t   storage;
    vf_handle_gpu_code_t  gpuCode;
    vf_handle_procedure_t procedure;
    vf_handle_batch_t     batch;
  };
} vf_handle_t;

#ifdef __cplusplus
}
#endif
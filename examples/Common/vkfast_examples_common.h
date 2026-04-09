#pragma once

#include <stdio.h> // For printf
#include <math.h>  // For sin, cos
#include <time.h>  // For time

#include <shellscalingapi.h>   // For SetProcessDpiAwareness
#pragma comment(lib, "shcore") // For SetProcessDpiAwareness

#define countof(x) (sizeof(x) / sizeof((x)[0]))

#define FF __FILE__
#define LL __LINE__

#if defined(_MSC_VER) && defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif

static void RandomInit() {
  srand(time(NULL));
}

static float RandomRange(float min, float max) {
  float n = rand() / (float)RAND_MAX;
  return min + n * (max - min);
}

static void vec3Mulf(float * out, const float * v, float f) {
  out[0] = v[0] * f;
  out[1] = v[1] * f;
  out[2] = v[2] * f;
}

static void vec3Add(float * out, const float * v1, const float * v2) {
  out[0] = v1[0] + v2[0];
  out[1] = v1[1] + v2[1];
  out[2] = v1[2] + v2[2];
}

static void vec3Cross(float * out, const float * v1, const float * v2) {
  float v1x = v1[0];
  float v1y = v1[1];
  float v1z = v1[2];

  float v2x = v2[0];
  float v2y = v2[1];
  float v2z = v2[2];

  out[0] = (v1y * v2z) - (v2y * v1z);
  out[1] = (v1z * v2x) - (v2z * v1x);
  out[2] = (v1x * v2y) - (v2x * v1y);
}

static void quatFromAxisAngle(float * out, const float * axis, float angle_rad) {
  float s = (float)sin(angle_rad / 2.f);
  float c = (float)cos(angle_rad / 2.f);

  out[0] = axis[0] * s;
  out[1] = axis[1] * s;
  out[2] = axis[2] * s;
  out[3] = c;
}

static void quatMul(float * out, const float * q1, const float * q2) {
  float q1x = q1[0];
  float q1y = q1[1];
  float q1z = q1[2];
  float q1w = q1[3];

  float q2x = q2[0];
  float q2y = q2[1];
  float q2z = q2[2];
  float q2w = q2[3];

  out[0] = q1x * q2w + q2x * q1w + (q1y * q2z - q2y * q1z);
  out[1] = q1y * q2w + q2y * q1w + (q1z * q2x - q2z * q1x);
  out[2] = q1z * q2w + q2z * q1w + (q1x * q2y - q2x * q1y);
  out[3] = q1w * q2w - (q1x * q2x + q1y * q2y + q1z * q2z);
}

static void quatRotateVec3(float * out, const float * v, const float * q) {
  float v4[4] = { v[0],  v[1],  v[2], 0};
  float nq[4] = {-q[0], -q[1], -q[2], q[3]};

  float t1[4];
  quatMul(t1, q, v4);
  float t2[4];
  quatMul(t2, t1, nq);
  
  out[0] = t2[0];
  out[1] = t2[1];
  out[2] = t2[2];
}

static void quatRotateVec3Fast(float * out, const float * v, const float * q) {
  // (cross(q.xyz, cross(q.xyz, v) + (v * q.w)) * 2.f) + v
  float vc[3] = {v[0], v[1], v[2]};
  vec3Cross(out, q, vc);
  float t1[3];
  vec3Mulf(t1, vc, q[3]);
  vec3Add(out, out, t1);
  vec3Cross(out, q, out);
  vec3Mulf(out, out, 2.f);
  vec3Add(out, out, vc);
}

#ifdef VKFAST_EXAMPLES_COMMON_INCLUDE_EXTRA_BANZAI
static ReiiCpuScratchBuffer OffsetAllocateCpuScratchBuffer(uint64_t bytesCountToAllocate, gpu_storage_t * storage_cpu, uint64_t * storage_cpu_offset, const char * optionalFile, int optionalLine) {
  gpu_extra_banzai_pointer_t cpu_pointer = {0};
  vfeBanzaiGetPointer(storage_cpu, storage_cpu_offset[0], &cpu_pointer, optionalFile, optionalLine);
  storage_cpu_offset[0] += bytesCountToAllocate;

  RedStructMemberArray cpu_array = {0};
  vfeBanzaiPointerGetRawLimited(&cpu_pointer, bytesCountToAllocate, &cpu_array, optionalFile, optionalLine);
  ReiiCpuScratchBuffer cpu_scratch_buffer = {0};
  cpu_scratch_buffer.cpu_scratch_buffer_ptr = cpu_pointer.mapped_void_ptr;
  cpu_scratch_buffer.cpu_scratch_buffer     = cpu_array;

  return cpu_scratch_buffer;
}

static gpu_extra_cpu_gpu_array OffsetAllocateCpuGpuArrayExact(uint64_t bytesCountToAllocate, gpu_storage_t * storage_cpu, uint64_t * storage_cpu_offset, gpu_storage_t * storage_gpu, uint64_t * storage_gpu_offset, const char * optionalFile, int optionalLine) {
  gpu_extra_banzai_pointer_t cpu_pointer = {0};
  gpu_extra_banzai_pointer_t gpu_pointer = {0};
  vfeBanzaiGetPointer(storage_cpu, storage_cpu_offset[0], &cpu_pointer, optionalFile, optionalLine);
  vfeBanzaiGetPointer(storage_gpu, storage_gpu_offset[0], &gpu_pointer, optionalFile, optionalLine);
  storage_cpu_offset[0] += bytesCountToAllocate;
  storage_gpu_offset[0] += bytesCountToAllocate;

  RedStructMemberArray cpu_array = {0};
  RedStructMemberArray gpu_array = {0};
  vfeBanzaiPointerGetRawLimited(&cpu_pointer, bytesCountToAllocate, &cpu_array, optionalFile, optionalLine);
  vfeBanzaiPointerGetRawLimited(&gpu_pointer, bytesCountToAllocate, &gpu_array, optionalFile, optionalLine);
  gpu_extra_cpu_gpu_array cpu_gpu_array = {0};
  cpu_gpu_array.cpu_ptr = cpu_pointer.mapped_void_ptr;
  cpu_gpu_array.cpu     = cpu_array;
  cpu_gpu_array.gpu     = gpu_array;

  return cpu_gpu_array;
}

static gpu_extra_cpu_gpu_array OffsetAllocateCpuGpuArrayWithTale64BytesAlign(uint64_t bytesCountToAllocate, gpu_storage_t * storage_cpu, uint64_t * storage_cpu_offset, gpu_storage_t * storage_gpu, uint64_t * storage_gpu_offset, const char * optionalFile, int optionalLine) {
  gpu_extra_cpu_gpu_array out = OffsetAllocateCpuGpuArrayExact(bytesCountToAllocate, storage_cpu, storage_cpu_offset, storage_gpu, storage_gpu_offset, optionalFile, optionalLine);
  uint64_t storage_cpu_offset_tale = REDGPU_2_BYTES_TO_NEXT_ALIGNMENT_BOUNDARY(storage_cpu_offset[0], 64);
  uint64_t storage_gpu_offset_tale = REDGPU_2_BYTES_TO_NEXT_ALIGNMENT_BOUNDARY(storage_gpu_offset[0], 64);
  storage_cpu_offset[0] += storage_cpu_offset_tale;
  storage_gpu_offset[0] += storage_gpu_offset_tale;
  return out;
}
#endif // #ifdef VKFAST_EXAMPLES_COMMON_INCLUDE_EXTRA_BANZAI

#ifdef VKFAST_EXAMPLES_COMMON_INCLUDE_GLM // https://github.com/g-truc/glm
  #include "glm/glm/glm.hpp"
#endif

#ifdef VKFAST_EXAMPLES_COMMON_INCLUDE_GLFW3 // https://github.com/glfw/glfw/releases/download/3.4/glfw-3.4.bin.WIN64.zip
  #ifdef _WIN32
    #define GLFW_INCLUDE_NONE
    #define GLFW_EXPOSE_NATIVE_WIN32
    #include "glfw-3.4.bin.WIN64/include/GLFW/glfw3.h" 
    #include "glfw-3.4.bin.WIN64/include/GLFW/glfw3native.h"
    #pragma comment(lib, "../../Common/glfw-3.4.bin.WIN64/lib-vc2019/glfw3_mt.lib") // NOTE(Constantine): Path relative to example's vs2019/ folder.
    #pragma comment(lib, "User32.lib")
    #pragma comment(lib, "Shell32.lib")
    #pragma comment(lib, "Gdi32.lib")
  #else
    #error
  #endif
#endif

#ifdef VKFAST_EXAMPLES_COMMON_INCLUDE_PROFILE
  #include "../../extra/Modified profile/profile.h"
#else
  #define profileBegin(x)
  #define profileEnd(x)
  #define profileInsertBegin(x, y, z, w)
  #define profileInsertEnd(x, y, z, w)
#endif

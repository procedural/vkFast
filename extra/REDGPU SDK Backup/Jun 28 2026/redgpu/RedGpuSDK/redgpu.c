#ifndef REDGPU_COMPILE_SWITCH
#define REDGPU_COMPILE_SWITCH 0
#endif

#define REDGPU_COMPILE_SWITCH_RELEASE 0
#define REDGPU_COMPILE_SWITCH_NVTX    1
#define REDGPU_COMPILE_SWITCH_GPUVIS  2
#define REDGPU_COMPILE_SWITCH_DEBUG   3
#if REDGPU_COMPILE_SWITCH ==          4
#undef  REDGPU_COMPILE_SWITCH
#define REDGPU_COMPILE_SWITCH 3
#define REDGPU_COMPILE_SWITCH_LOG
#endif

#ifdef __ANDROID__
int (*redInternalCreateAndroidSurface)(void * instance, const void * createInfo, const void * allocator, void ** outHandleSurface) = 0;
#endif

#ifdef _WIN32
#define REDGPU_OS_WINDOWS
#endif
#if defined(__linux__) && !defined(__ANDROID__)
#define REDGPU_OS_LINUX
#endif
#ifdef __ANDROID__
#define REDGPU_OS_ANDROID
#endif

#ifdef REDGPU_OS_WINDOWS
#define REDGPU_WSI_WIN32
#endif
#ifdef REDGPU_OS_LINUX
#define REDGPU_WSI_XLIB_OR_XCB
#endif

#ifdef REDGPU_DECLSPEC
#undef REDGPU_DECLSPEC
#endif
#ifdef REDGPU_API
#undef REDGPU_API
#endif
#ifdef REDGPU_OS_WINDOWS
#define REDGPU_DECLSPEC __declspec(dllexport)
#define REDGPU_API
#define REDGPU_DECLTYPE decltype
#endif
#ifdef REDGPU_OS_LINUX
#define REDGPU_DECLSPEC __attribute__((visibility("default")))
#define REDGPU_API
#define REDGPU_DECLTYPE typeof
#endif

#include "redgpu.h"
#include "redgpu_wsi.h"
#include "redgpu_computing_language.h"
#include "redgpu_context_from_vk.h"
#include "redgpu_array_timestamp.h"
#include "redgpu_print.h"

#ifdef REDGPU_OS_WINDOWS
typedef void * HINSTANCE;
typedef void * HWND;
#endif
#ifdef REDGPU_OS_LINUX
#include <X11/Xlib.h>
#include <xcb/xcb.h>
#endif

#ifdef REDGPU_OS_WINDOWS
#define VK_USE_PLATFORM_WIN32_KHR
#endif
#ifdef REDGPU_OS_LINUX
#define VK_USE_PLATFORM_XLIB_KHR
#define VK_USE_PLATFORM_XCB_KHR
#endif
#include "redgpu_vk.h"

#if REDGPU_COMPILE_SWITCH == REDGPU_COMPILE_SWITCH_NVTX
#ifdef REDGPU_OS_WINDOWS
#include <wchar.h>
#include "C:/Program Files/NVIDIA Corporation/Nsight Systems 2020.2.1/target-windows-x64/nvtx/include/nvtx3/nvToolsExt.h"
#endif
#ifdef REDGPU_OS_LINUX
#include "code_dependencies/opt/nvidia/nsight-systems/2021.2.1/target-linux-x64/nvtx/include/nvtx3/nvToolsExt.h"
#endif
#endif

#if REDGPU_COMPILE_SWITCH == REDGPU_COMPILE_SWITCH_GPUVIS
#define GPUVIS_TRACE_IMPLEMENTATION
#include "code_dependencies/home/constantine/Programs/GPUVis/sample/gpuvis_trace_utils.h" // https://github.com/mikesart/gpuvis/commit/14c4600b5d857f0c9ce6e4c0a9524d7e90a6445b
#endif

#if REDGPU_COMPILE_SWITCH == REDGPU_COMPILE_SWITCH_DEBUG
#ifdef REDGPU_OS_WINDOWS
#include "C:/RedGpuSDK/redgpu_debug.h"
#else
#include "/home/linuxbrew/RedGpuSDK/redgpu_debug.h"
#endif
#include <string.h> // For strcmp
#include <stdio.h>  // For printf
#include <mutex>    // For mutex
#ifdef REDGPU_OS_LINUX
#include <dlfcn.h>  // For dlopen
#endif
#include <stdlib.h> // For setenv
#endif

#ifdef REDGPU_COMPILE_SWITCH_LOG
#include <new>      // For std::new
#include <string>   // For std::string
#include <fstream>  // For std::ofstream
#include <stdarg.h> // For va_start, va_end
#ifdef REDGPU_OS_LINUX
#include <unistd.h> // For getpid
#endif
#define STB_SPRINTF_IMPLEMENTATION
#include "redgpu_stb_sprintf.h"
#endif

#ifdef REDGPU_COMPILE_SWITCH_LOG
#define REDGPU_PRINTF redInternalLogDebugPrintf
#define REDGPU_FFLUSH redInternalLogDebugFflush
#elif defined(REDGPU_OS_ANDROID)
#include <fstream> // For std::ofstream
#include <iomanip> // For std::setw
#define REDGPU_PRINTF(fmt, ...) do { \
    size_t size = snprintf(0, 0, fmt, ##__VA_ARGS__); \
    std::string output; \
    output.reserve(size + 1); \
    output.resize(size); \
    snprintf(&output[0], size + 1, fmt, ##__VA_ARGS__); \
    std::ofstream fs(std::string("/storage/emulated/0/Download/redgpu_log_debug.txt"), std::ofstream::app); \
    fs << output; \
    fs.close(); \
  } while(0)
#define REDGPU_FFLUSH(ignored) do { \
    time_t t = time(0); \
    std::string now = ctime(&t); \
    struct timeval tv; \
    struct timezone tz; \
    gettimeofday(&tv, &tz); \
    now.pop_back(); \
    std::ofstream fs(std::string("/storage/emulated/0/Download/redgpu_log_debug.txt"), std::ofstream::app); \
    fs << "[" << now << ", " << std::setw(7) << std::setfill(' ') << tv.tv_usec << " us]\n"; \
    fs.close(); \
  } while(0)
#else
#define REDGPU_PRINTF printf
#define REDGPU_FFLUSH fflush
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _WIN32
#else
#define _countof(array) (sizeof(array) / sizeof((array)[0]))
#endif
#define _lengthof(string) ((sizeof(string) / sizeof((string)[0])) - 1)

#ifdef _WIN32
#define REDGPU_H64_MERGE(a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p) 0x##a##b##c##d##e##f##g##h##i##j##k##l##m##n##o##p
#define REDGPU_H64(a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p) REDGPU_H64_MERGE(a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p)
#define REDGPU_B64(a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p) REDGPU_H64(REDGPU_B4_TO_HEX(a),REDGPU_B4_TO_HEX(b),REDGPU_B4_TO_HEX(c),REDGPU_B4_TO_HEX(d),REDGPU_B4_TO_HEX(e),REDGPU_B4_TO_HEX(f),REDGPU_B4_TO_HEX(g),REDGPU_B4_TO_HEX(h),REDGPU_B4_TO_HEX(i),REDGPU_B4_TO_HEX(j),REDGPU_B4_TO_HEX(k),REDGPU_B4_TO_HEX(l),REDGPU_B4_TO_HEX(m),REDGPU_B4_TO_HEX(n),REDGPU_B4_TO_HEX(o),REDGPU_B4_TO_HEX(p))
#else
#define REDGPU_B64(a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p) 0b##a##b##c##d##e##f##g##h##i##j##k##l##m##n##o##p
#endif

#define REDGPU_PRINT_B32(x)\
  ((unsigned)(x) & REDGPU_B32(1000,0000,0000,0000,0000,0000,0000,0000)) == 0 ? '0' : '1',\
  ((unsigned)(x) & REDGPU_B32(0100,0000,0000,0000,0000,0000,0000,0000)) == 0 ? '0' : '1',\
  ((unsigned)(x) & REDGPU_B32(0010,0000,0000,0000,0000,0000,0000,0000)) == 0 ? '0' : '1',\
  ((unsigned)(x) & REDGPU_B32(0001,0000,0000,0000,0000,0000,0000,0000)) == 0 ? '0' : '1',\
  ((unsigned)(x) & REDGPU_B32(0000,1000,0000,0000,0000,0000,0000,0000)) == 0 ? '0' : '1',\
  ((unsigned)(x) & REDGPU_B32(0000,0100,0000,0000,0000,0000,0000,0000)) == 0 ? '0' : '1',\
  ((unsigned)(x) & REDGPU_B32(0000,0010,0000,0000,0000,0000,0000,0000)) == 0 ? '0' : '1',\
  ((unsigned)(x) & REDGPU_B32(0000,0001,0000,0000,0000,0000,0000,0000)) == 0 ? '0' : '1',\
  ((unsigned)(x) & REDGPU_B32(0000,0000,1000,0000,0000,0000,0000,0000)) == 0 ? '0' : '1',\
  ((unsigned)(x) & REDGPU_B32(0000,0000,0100,0000,0000,0000,0000,0000)) == 0 ? '0' : '1',\
  ((unsigned)(x) & REDGPU_B32(0000,0000,0010,0000,0000,0000,0000,0000)) == 0 ? '0' : '1',\
  ((unsigned)(x) & REDGPU_B32(0000,0000,0001,0000,0000,0000,0000,0000)) == 0 ? '0' : '1',\
  ((unsigned)(x) & REDGPU_B32(0000,0000,0000,1000,0000,0000,0000,0000)) == 0 ? '0' : '1',\
  ((unsigned)(x) & REDGPU_B32(0000,0000,0000,0100,0000,0000,0000,0000)) == 0 ? '0' : '1',\
  ((unsigned)(x) & REDGPU_B32(0000,0000,0000,0010,0000,0000,0000,0000)) == 0 ? '0' : '1',\
  ((unsigned)(x) & REDGPU_B32(0000,0000,0000,0001,0000,0000,0000,0000)) == 0 ? '0' : '1',\
  ((unsigned)(x) & REDGPU_B32(0000,0000,0000,0000,1000,0000,0000,0000)) == 0 ? '0' : '1',\
  ((unsigned)(x) & REDGPU_B32(0000,0000,0000,0000,0100,0000,0000,0000)) == 0 ? '0' : '1',\
  ((unsigned)(x) & REDGPU_B32(0000,0000,0000,0000,0010,0000,0000,0000)) == 0 ? '0' : '1',\
  ((unsigned)(x) & REDGPU_B32(0000,0000,0000,0000,0001,0000,0000,0000)) == 0 ? '0' : '1',\
  ((unsigned)(x) & REDGPU_B32(0000,0000,0000,0000,0000,1000,0000,0000)) == 0 ? '0' : '1',\
  ((unsigned)(x) & REDGPU_B32(0000,0000,0000,0000,0000,0100,0000,0000)) == 0 ? '0' : '1',\
  ((unsigned)(x) & REDGPU_B32(0000,0000,0000,0000,0000,0010,0000,0000)) == 0 ? '0' : '1',\
  ((unsigned)(x) & REDGPU_B32(0000,0000,0000,0000,0000,0001,0000,0000)) == 0 ? '0' : '1',\
  ((unsigned)(x) & REDGPU_B32(0000,0000,0000,0000,0000,0000,1000,0000)) == 0 ? '0' : '1',\
  ((unsigned)(x) & REDGPU_B32(0000,0000,0000,0000,0000,0000,0100,0000)) == 0 ? '0' : '1',\
  ((unsigned)(x) & REDGPU_B32(0000,0000,0000,0000,0000,0000,0010,0000)) == 0 ? '0' : '1',\
  ((unsigned)(x) & REDGPU_B32(0000,0000,0000,0000,0000,0000,0001,0000)) == 0 ? '0' : '1',\
  ((unsigned)(x) & REDGPU_B32(0000,0000,0000,0000,0000,0000,0000,1000)) == 0 ? '0' : '1',\
  ((unsigned)(x) & REDGPU_B32(0000,0000,0000,0000,0000,0000,0000,0100)) == 0 ? '0' : '1',\
  ((unsigned)(x) & REDGPU_B32(0000,0000,0000,0000,0000,0000,0000,0010)) == 0 ? '0' : '1',\
  ((unsigned)(x) & REDGPU_B32(0000,0000,0000,0000,0000,0000,0000,0001)) == 0 ? '0' : '1'

#define REDGPU_SET_B32(y, x)\
  (y)[0]  = ((unsigned)(x) & REDGPU_B32(1000,0000,0000,0000,0000,0000,0000,0000)) == 0 ? '0' : '1';\
  (y)[1]  = ((unsigned)(x) & REDGPU_B32(0100,0000,0000,0000,0000,0000,0000,0000)) == 0 ? '0' : '1';\
  (y)[2]  = ((unsigned)(x) & REDGPU_B32(0010,0000,0000,0000,0000,0000,0000,0000)) == 0 ? '0' : '1';\
  (y)[3]  = ((unsigned)(x) & REDGPU_B32(0001,0000,0000,0000,0000,0000,0000,0000)) == 0 ? '0' : '1';\
  (y)[4]  = ((unsigned)(x) & REDGPU_B32(0000,1000,0000,0000,0000,0000,0000,0000)) == 0 ? '0' : '1';\
  (y)[5]  = ((unsigned)(x) & REDGPU_B32(0000,0100,0000,0000,0000,0000,0000,0000)) == 0 ? '0' : '1';\
  (y)[6]  = ((unsigned)(x) & REDGPU_B32(0000,0010,0000,0000,0000,0000,0000,0000)) == 0 ? '0' : '1';\
  (y)[7]  = ((unsigned)(x) & REDGPU_B32(0000,0001,0000,0000,0000,0000,0000,0000)) == 0 ? '0' : '1';\
  (y)[8]  = ((unsigned)(x) & REDGPU_B32(0000,0000,1000,0000,0000,0000,0000,0000)) == 0 ? '0' : '1';\
  (y)[9]  = ((unsigned)(x) & REDGPU_B32(0000,0000,0100,0000,0000,0000,0000,0000)) == 0 ? '0' : '1';\
  (y)[10] = ((unsigned)(x) & REDGPU_B32(0000,0000,0010,0000,0000,0000,0000,0000)) == 0 ? '0' : '1';\
  (y)[11] = ((unsigned)(x) & REDGPU_B32(0000,0000,0001,0000,0000,0000,0000,0000)) == 0 ? '0' : '1';\
  (y)[12] = ((unsigned)(x) & REDGPU_B32(0000,0000,0000,1000,0000,0000,0000,0000)) == 0 ? '0' : '1';\
  (y)[13] = ((unsigned)(x) & REDGPU_B32(0000,0000,0000,0100,0000,0000,0000,0000)) == 0 ? '0' : '1';\
  (y)[14] = ((unsigned)(x) & REDGPU_B32(0000,0000,0000,0010,0000,0000,0000,0000)) == 0 ? '0' : '1';\
  (y)[15] = ((unsigned)(x) & REDGPU_B32(0000,0000,0000,0001,0000,0000,0000,0000)) == 0 ? '0' : '1';\
  (y)[16] = ((unsigned)(x) & REDGPU_B32(0000,0000,0000,0000,1000,0000,0000,0000)) == 0 ? '0' : '1';\
  (y)[17] = ((unsigned)(x) & REDGPU_B32(0000,0000,0000,0000,0100,0000,0000,0000)) == 0 ? '0' : '1';\
  (y)[18] = ((unsigned)(x) & REDGPU_B32(0000,0000,0000,0000,0010,0000,0000,0000)) == 0 ? '0' : '1';\
  (y)[19] = ((unsigned)(x) & REDGPU_B32(0000,0000,0000,0000,0001,0000,0000,0000)) == 0 ? '0' : '1';\
  (y)[20] = ((unsigned)(x) & REDGPU_B32(0000,0000,0000,0000,0000,1000,0000,0000)) == 0 ? '0' : '1';\
  (y)[21] = ((unsigned)(x) & REDGPU_B32(0000,0000,0000,0000,0000,0100,0000,0000)) == 0 ? '0' : '1';\
  (y)[22] = ((unsigned)(x) & REDGPU_B32(0000,0000,0000,0000,0000,0010,0000,0000)) == 0 ? '0' : '1';\
  (y)[23] = ((unsigned)(x) & REDGPU_B32(0000,0000,0000,0000,0000,0001,0000,0000)) == 0 ? '0' : '1';\
  (y)[24] = ((unsigned)(x) & REDGPU_B32(0000,0000,0000,0000,0000,0000,1000,0000)) == 0 ? '0' : '1';\
  (y)[25] = ((unsigned)(x) & REDGPU_B32(0000,0000,0000,0000,0000,0000,0100,0000)) == 0 ? '0' : '1';\
  (y)[26] = ((unsigned)(x) & REDGPU_B32(0000,0000,0000,0000,0000,0000,0010,0000)) == 0 ? '0' : '1';\
  (y)[27] = ((unsigned)(x) & REDGPU_B32(0000,0000,0000,0000,0000,0000,0001,0000)) == 0 ? '0' : '1';\
  (y)[28] = ((unsigned)(x) & REDGPU_B32(0000,0000,0000,0000,0000,0000,0000,1000)) == 0 ? '0' : '1';\
  (y)[29] = ((unsigned)(x) & REDGPU_B32(0000,0000,0000,0000,0000,0000,0000,0100)) == 0 ? '0' : '1';\
  (y)[30] = ((unsigned)(x) & REDGPU_B32(0000,0000,0000,0000,0000,0000,0000,0010)) == 0 ? '0' : '1';\
  (y)[31] = ((unsigned)(x) & REDGPU_B32(0000,0000,0000,0000,0000,0000,0000,0001)) == 0 ? '0' : '1';

#define REDGPU_PRINT_POINTER(x) (void *)(x)

#define REDGPU_SET_B64(y, x)\
  (y)[0]  = ((uint64_t)(void *)(x) & REDGPU_B64(1000,0000,0000,0000,0000,0000,0000,0000,0000,0000,0000,0000,0000,0000,0000,0000)) == 0 ? '0' : '1';\
  (y)[1]  = ((uint64_t)(void *)(x) & REDGPU_B64(0100,0000,0000,0000,0000,0000,0000,0000,0000,0000,0000,0000,0000,0000,0000,0000)) == 0 ? '0' : '1';\
  (y)[2]  = ((uint64_t)(void *)(x) & REDGPU_B64(0010,0000,0000,0000,0000,0000,0000,0000,0000,0000,0000,0000,0000,0000,0000,0000)) == 0 ? '0' : '1';\
  (y)[3]  = ((uint64_t)(void *)(x) & REDGPU_B64(0001,0000,0000,0000,0000,0000,0000,0000,0000,0000,0000,0000,0000,0000,0000,0000)) == 0 ? '0' : '1';\
  (y)[4]  = ((uint64_t)(void *)(x) & REDGPU_B64(0000,1000,0000,0000,0000,0000,0000,0000,0000,0000,0000,0000,0000,0000,0000,0000)) == 0 ? '0' : '1';\
  (y)[5]  = ((uint64_t)(void *)(x) & REDGPU_B64(0000,0100,0000,0000,0000,0000,0000,0000,0000,0000,0000,0000,0000,0000,0000,0000)) == 0 ? '0' : '1';\
  (y)[6]  = ((uint64_t)(void *)(x) & REDGPU_B64(0000,0010,0000,0000,0000,0000,0000,0000,0000,0000,0000,0000,0000,0000,0000,0000)) == 0 ? '0' : '1';\
  (y)[7]  = ((uint64_t)(void *)(x) & REDGPU_B64(0000,0001,0000,0000,0000,0000,0000,0000,0000,0000,0000,0000,0000,0000,0000,0000)) == 0 ? '0' : '1';\
  (y)[8]  = ((uint64_t)(void *)(x) & REDGPU_B64(0000,0000,1000,0000,0000,0000,0000,0000,0000,0000,0000,0000,0000,0000,0000,0000)) == 0 ? '0' : '1';\
  (y)[9]  = ((uint64_t)(void *)(x) & REDGPU_B64(0000,0000,0100,0000,0000,0000,0000,0000,0000,0000,0000,0000,0000,0000,0000,0000)) == 0 ? '0' : '1';\
  (y)[10] = ((uint64_t)(void *)(x) & REDGPU_B64(0000,0000,0010,0000,0000,0000,0000,0000,0000,0000,0000,0000,0000,0000,0000,0000)) == 0 ? '0' : '1';\
  (y)[11] = ((uint64_t)(void *)(x) & REDGPU_B64(0000,0000,0001,0000,0000,0000,0000,0000,0000,0000,0000,0000,0000,0000,0000,0000)) == 0 ? '0' : '1';\
  (y)[12] = ((uint64_t)(void *)(x) & REDGPU_B64(0000,0000,0000,1000,0000,0000,0000,0000,0000,0000,0000,0000,0000,0000,0000,0000)) == 0 ? '0' : '1';\
  (y)[13] = ((uint64_t)(void *)(x) & REDGPU_B64(0000,0000,0000,0100,0000,0000,0000,0000,0000,0000,0000,0000,0000,0000,0000,0000)) == 0 ? '0' : '1';\
  (y)[14] = ((uint64_t)(void *)(x) & REDGPU_B64(0000,0000,0000,0010,0000,0000,0000,0000,0000,0000,0000,0000,0000,0000,0000,0000)) == 0 ? '0' : '1';\
  (y)[15] = ((uint64_t)(void *)(x) & REDGPU_B64(0000,0000,0000,0001,0000,0000,0000,0000,0000,0000,0000,0000,0000,0000,0000,0000)) == 0 ? '0' : '1';\
  (y)[16] = ((uint64_t)(void *)(x) & REDGPU_B64(0000,0000,0000,0000,1000,0000,0000,0000,0000,0000,0000,0000,0000,0000,0000,0000)) == 0 ? '0' : '1';\
  (y)[17] = ((uint64_t)(void *)(x) & REDGPU_B64(0000,0000,0000,0000,0100,0000,0000,0000,0000,0000,0000,0000,0000,0000,0000,0000)) == 0 ? '0' : '1';\
  (y)[18] = ((uint64_t)(void *)(x) & REDGPU_B64(0000,0000,0000,0000,0010,0000,0000,0000,0000,0000,0000,0000,0000,0000,0000,0000)) == 0 ? '0' : '1';\
  (y)[19] = ((uint64_t)(void *)(x) & REDGPU_B64(0000,0000,0000,0000,0001,0000,0000,0000,0000,0000,0000,0000,0000,0000,0000,0000)) == 0 ? '0' : '1';\
  (y)[20] = ((uint64_t)(void *)(x) & REDGPU_B64(0000,0000,0000,0000,0000,1000,0000,0000,0000,0000,0000,0000,0000,0000,0000,0000)) == 0 ? '0' : '1';\
  (y)[21] = ((uint64_t)(void *)(x) & REDGPU_B64(0000,0000,0000,0000,0000,0100,0000,0000,0000,0000,0000,0000,0000,0000,0000,0000)) == 0 ? '0' : '1';\
  (y)[22] = ((uint64_t)(void *)(x) & REDGPU_B64(0000,0000,0000,0000,0000,0010,0000,0000,0000,0000,0000,0000,0000,0000,0000,0000)) == 0 ? '0' : '1';\
  (y)[23] = ((uint64_t)(void *)(x) & REDGPU_B64(0000,0000,0000,0000,0000,0001,0000,0000,0000,0000,0000,0000,0000,0000,0000,0000)) == 0 ? '0' : '1';\
  (y)[24] = ((uint64_t)(void *)(x) & REDGPU_B64(0000,0000,0000,0000,0000,0000,1000,0000,0000,0000,0000,0000,0000,0000,0000,0000)) == 0 ? '0' : '1';\
  (y)[25] = ((uint64_t)(void *)(x) & REDGPU_B64(0000,0000,0000,0000,0000,0000,0100,0000,0000,0000,0000,0000,0000,0000,0000,0000)) == 0 ? '0' : '1';\
  (y)[26] = ((uint64_t)(void *)(x) & REDGPU_B64(0000,0000,0000,0000,0000,0000,0010,0000,0000,0000,0000,0000,0000,0000,0000,0000)) == 0 ? '0' : '1';\
  (y)[27] = ((uint64_t)(void *)(x) & REDGPU_B64(0000,0000,0000,0000,0000,0000,0001,0000,0000,0000,0000,0000,0000,0000,0000,0000)) == 0 ? '0' : '1';\
  (y)[28] = ((uint64_t)(void *)(x) & REDGPU_B64(0000,0000,0000,0000,0000,0000,0000,1000,0000,0000,0000,0000,0000,0000,0000,0000)) == 0 ? '0' : '1';\
  (y)[29] = ((uint64_t)(void *)(x) & REDGPU_B64(0000,0000,0000,0000,0000,0000,0000,0100,0000,0000,0000,0000,0000,0000,0000,0000)) == 0 ? '0' : '1';\
  (y)[30] = ((uint64_t)(void *)(x) & REDGPU_B64(0000,0000,0000,0000,0000,0000,0000,0010,0000,0000,0000,0000,0000,0000,0000,0000)) == 0 ? '0' : '1';\
  (y)[31] = ((uint64_t)(void *)(x) & REDGPU_B64(0000,0000,0000,0000,0000,0000,0000,0001,0000,0000,0000,0000,0000,0000,0000,0000)) == 0 ? '0' : '1';\
  (y)[32] = ((uint64_t)(void *)(x) & REDGPU_B64(0000,0000,0000,0000,0000,0000,0000,0000,1000,0000,0000,0000,0000,0000,0000,0000)) == 0 ? '0' : '1';\
  (y)[33] = ((uint64_t)(void *)(x) & REDGPU_B64(0000,0000,0000,0000,0000,0000,0000,0000,0100,0000,0000,0000,0000,0000,0000,0000)) == 0 ? '0' : '1';\
  (y)[34] = ((uint64_t)(void *)(x) & REDGPU_B64(0000,0000,0000,0000,0000,0000,0000,0000,0010,0000,0000,0000,0000,0000,0000,0000)) == 0 ? '0' : '1';\
  (y)[35] = ((uint64_t)(void *)(x) & REDGPU_B64(0000,0000,0000,0000,0000,0000,0000,0000,0001,0000,0000,0000,0000,0000,0000,0000)) == 0 ? '0' : '1';\
  (y)[36] = ((uint64_t)(void *)(x) & REDGPU_B64(0000,0000,0000,0000,0000,0000,0000,0000,0000,1000,0000,0000,0000,0000,0000,0000)) == 0 ? '0' : '1';\
  (y)[37] = ((uint64_t)(void *)(x) & REDGPU_B64(0000,0000,0000,0000,0000,0000,0000,0000,0000,0100,0000,0000,0000,0000,0000,0000)) == 0 ? '0' : '1';\
  (y)[38] = ((uint64_t)(void *)(x) & REDGPU_B64(0000,0000,0000,0000,0000,0000,0000,0000,0000,0010,0000,0000,0000,0000,0000,0000)) == 0 ? '0' : '1';\
  (y)[39] = ((uint64_t)(void *)(x) & REDGPU_B64(0000,0000,0000,0000,0000,0000,0000,0000,0000,0001,0000,0000,0000,0000,0000,0000)) == 0 ? '0' : '1';\
  (y)[40] = ((uint64_t)(void *)(x) & REDGPU_B64(0000,0000,0000,0000,0000,0000,0000,0000,0000,0000,1000,0000,0000,0000,0000,0000)) == 0 ? '0' : '1';\
  (y)[41] = ((uint64_t)(void *)(x) & REDGPU_B64(0000,0000,0000,0000,0000,0000,0000,0000,0000,0000,0100,0000,0000,0000,0000,0000)) == 0 ? '0' : '1';\
  (y)[42] = ((uint64_t)(void *)(x) & REDGPU_B64(0000,0000,0000,0000,0000,0000,0000,0000,0000,0000,0010,0000,0000,0000,0000,0000)) == 0 ? '0' : '1';\
  (y)[43] = ((uint64_t)(void *)(x) & REDGPU_B64(0000,0000,0000,0000,0000,0000,0000,0000,0000,0000,0001,0000,0000,0000,0000,0000)) == 0 ? '0' : '1';\
  (y)[44] = ((uint64_t)(void *)(x) & REDGPU_B64(0000,0000,0000,0000,0000,0000,0000,0000,0000,0000,0000,1000,0000,0000,0000,0000)) == 0 ? '0' : '1';\
  (y)[45] = ((uint64_t)(void *)(x) & REDGPU_B64(0000,0000,0000,0000,0000,0000,0000,0000,0000,0000,0000,0100,0000,0000,0000,0000)) == 0 ? '0' : '1';\
  (y)[46] = ((uint64_t)(void *)(x) & REDGPU_B64(0000,0000,0000,0000,0000,0000,0000,0000,0000,0000,0000,0010,0000,0000,0000,0000)) == 0 ? '0' : '1';\
  (y)[47] = ((uint64_t)(void *)(x) & REDGPU_B64(0000,0000,0000,0000,0000,0000,0000,0000,0000,0000,0000,0001,0000,0000,0000,0000)) == 0 ? '0' : '1';\
  (y)[48] = ((uint64_t)(void *)(x) & REDGPU_B64(0000,0000,0000,0000,0000,0000,0000,0000,0000,0000,0000,0000,1000,0000,0000,0000)) == 0 ? '0' : '1';\
  (y)[49] = ((uint64_t)(void *)(x) & REDGPU_B64(0000,0000,0000,0000,0000,0000,0000,0000,0000,0000,0000,0000,0100,0000,0000,0000)) == 0 ? '0' : '1';\
  (y)[50] = ((uint64_t)(void *)(x) & REDGPU_B64(0000,0000,0000,0000,0000,0000,0000,0000,0000,0000,0000,0000,0010,0000,0000,0000)) == 0 ? '0' : '1';\
  (y)[51] = ((uint64_t)(void *)(x) & REDGPU_B64(0000,0000,0000,0000,0000,0000,0000,0000,0000,0000,0000,0000,0001,0000,0000,0000)) == 0 ? '0' : '1';\
  (y)[52] = ((uint64_t)(void *)(x) & REDGPU_B64(0000,0000,0000,0000,0000,0000,0000,0000,0000,0000,0000,0000,0000,1000,0000,0000)) == 0 ? '0' : '1';\
  (y)[53] = ((uint64_t)(void *)(x) & REDGPU_B64(0000,0000,0000,0000,0000,0000,0000,0000,0000,0000,0000,0000,0000,0100,0000,0000)) == 0 ? '0' : '1';\
  (y)[54] = ((uint64_t)(void *)(x) & REDGPU_B64(0000,0000,0000,0000,0000,0000,0000,0000,0000,0000,0000,0000,0000,0010,0000,0000)) == 0 ? '0' : '1';\
  (y)[55] = ((uint64_t)(void *)(x) & REDGPU_B64(0000,0000,0000,0000,0000,0000,0000,0000,0000,0000,0000,0000,0000,0001,0000,0000)) == 0 ? '0' : '1';\
  (y)[56] = ((uint64_t)(void *)(x) & REDGPU_B64(0000,0000,0000,0000,0000,0000,0000,0000,0000,0000,0000,0000,0000,0000,1000,0000)) == 0 ? '0' : '1';\
  (y)[57] = ((uint64_t)(void *)(x) & REDGPU_B64(0000,0000,0000,0000,0000,0000,0000,0000,0000,0000,0000,0000,0000,0000,0100,0000)) == 0 ? '0' : '1';\
  (y)[58] = ((uint64_t)(void *)(x) & REDGPU_B64(0000,0000,0000,0000,0000,0000,0000,0000,0000,0000,0000,0000,0000,0000,0010,0000)) == 0 ? '0' : '1';\
  (y)[59] = ((uint64_t)(void *)(x) & REDGPU_B64(0000,0000,0000,0000,0000,0000,0000,0000,0000,0000,0000,0000,0000,0000,0001,0000)) == 0 ? '0' : '1';\
  (y)[60] = ((uint64_t)(void *)(x) & REDGPU_B64(0000,0000,0000,0000,0000,0000,0000,0000,0000,0000,0000,0000,0000,0000,0000,1000)) == 0 ? '0' : '1';\
  (y)[61] = ((uint64_t)(void *)(x) & REDGPU_B64(0000,0000,0000,0000,0000,0000,0000,0000,0000,0000,0000,0000,0000,0000,0000,0100)) == 0 ? '0' : '1';\
  (y)[62] = ((uint64_t)(void *)(x) & REDGPU_B64(0000,0000,0000,0000,0000,0000,0000,0000,0000,0000,0000,0000,0000,0000,0000,0010)) == 0 ? '0' : '1';\
  (y)[63] = ((uint64_t)(void *)(x) & REDGPU_B64(0000,0000,0000,0000,0000,0000,0000,0000,0000,0000,0000,0000,0000,0000,0000,0001)) == 0 ? '0' : '1';

// Mark

#if REDGPU_COMPILE_SWITCH == REDGPU_COMPILE_SWITCH_NVTX
#define redInternalMark(mark)    nvtxMarkA(mark)
#define redInternalMarkSet(mark) nvtxRangePushA(mark)
#define redInternalMarkEnd()     nvtxRangePop()
#elif REDGPU_COMPILE_SWITCH == REDGPU_COMPILE_SWITCH_GPUVIS
#define redInternalMark(mark)    gpuvis_trace_printf(optionalFile == NULL ? "%s%s%s\n" : "%s [%s:%s]\n", mark, optionalFile == NULL ? "" : optionalFile, optionalFile == NULL ? "" : std::to_string(optionalLine).c_str())
#define redInternalMarkSet(mark) GPUVIS_TRACE_BLOCKF(optionalFile == NULL ? "%s%s%s"   : "%s [%s:%s]",   mark, optionalFile == NULL ? "" : optionalFile, optionalFile == NULL ? "" : std::to_string(optionalLine).c_str())
#define redInternalMarkEnd()
#else
#define redInternalMark(mark)
#define redInternalMarkSet(mark)
#define redInternalMarkEnd()
#endif

static inline void redInternalMarkStatus(RedStatus status) {
#if REDGPU_COMPILE_SWITCH == REDGPU_COMPILE_SWITCH_NVTX
  if      (status == RED_STATUS_SUCCESS)                       { redInternalMark("[REDGPU] outStatuses returned RED_STATUS_SUCCESS");                       }
  else if (status == RED_STATUS_NOT_READY)                     { redInternalMark("[REDGPU] outStatuses returned RED_STATUS_NOT_READY");                     }
  else if (status == RED_STATUS_TIMEOUT)                       { redInternalMark("[REDGPU] outStatuses returned RED_STATUS_TIMEOUT");                       }
  else if (status == RED_STATUS_GPU_TO_CPU_SIGNAL_SIGNALED)    { redInternalMark("[REDGPU] outStatuses returned RED_STATUS_GPU_TO_CPU_SIGNAL_SIGNALED");    }
  else if (status == RED_STATUS_GPU_TO_CPU_SIGNAL_UNSIGNALED)  { redInternalMark("[REDGPU] outStatuses returned RED_STATUS_GPU_TO_CPU_SIGNAL_UNSIGNALED");  }
  else if (status == RED_STATUS_INCOMPLETE)                    { redInternalMark("[REDGPU] outStatuses returned RED_STATUS_INCOMPLETE");                    }
  else if (status == RED_STATUS_PRESENT_IS_SUBOPTIMAL)         { redInternalMark("[REDGPU] outStatuses returned RED_STATUS_PRESENT_IS_SUBOPTIMAL");         }
  else if (status == RED_STATUS_ERROR_OUT_OF_CPU_MEMORY)       { redInternalMark("[REDGPU] outStatuses returned RED_STATUS_ERROR_OUT_OF_CPU_MEMORY");       }
  else if (status == RED_STATUS_ERROR_OUT_OF_GPU_MEMORY)       { redInternalMark("[REDGPU] outStatuses returned RED_STATUS_ERROR_OUT_OF_GPU_MEMORY");       }
  else if (status == RED_STATUS_ERROR_INITIALIZATION_FAILED)   { redInternalMark("[REDGPU] outStatuses returned RED_STATUS_ERROR_INITIALIZATION_FAILED");   }
  else if (status == RED_STATUS_ERROR_GPU_IS_LOST)             { redInternalMark("[REDGPU] outStatuses returned RED_STATUS_ERROR_GPU_IS_LOST");             }
  else if (status == RED_STATUS_ERROR_MEMORY_MAPPING_FAILED)   { redInternalMark("[REDGPU] outStatuses returned RED_STATUS_ERROR_MEMORY_MAPPING_FAILED");   }
  else if (status == RED_STATUS_ERROR_LAYER_IS_NOT_FOUND)      { redInternalMark("[REDGPU] outStatuses returned RED_STATUS_ERROR_LAYER_IS_NOT_FOUND");      }
  else if (status == RED_STATUS_ERROR_EXTENSION_IS_NOT_FOUND)  { redInternalMark("[REDGPU] outStatuses returned RED_STATUS_ERROR_EXTENSION_IS_NOT_FOUND");  }
  else if (status == RED_STATUS_ERROR_FEATURE_IS_NOT_FOUND)    { redInternalMark("[REDGPU] outStatuses returned RED_STATUS_ERROR_FEATURE_IS_NOT_FOUND");    }
  else if (status == RED_STATUS_ERROR_INCOMPATIBLE_DRIVER)     { redInternalMark("[REDGPU] outStatuses returned RED_STATUS_ERROR_INCOMPATIBLE_DRIVER");     }
  else if (status == RED_STATUS_ERROR_TOO_MANY_HANDLES)        { redInternalMark("[REDGPU] outStatuses returned RED_STATUS_ERROR_TOO_MANY_HANDLES");        }
  else if (status == RED_STATUS_ERROR_FORMAT_IS_NOT_SUPPORTED) { redInternalMark("[REDGPU] outStatuses returned RED_STATUS_ERROR_FORMAT_IS_NOT_SUPPORTED"); }
  else if (status == RED_STATUS_ERROR_MEMORY_IS_FRAGMENTED)    { redInternalMark("[REDGPU] outStatuses returned RED_STATUS_ERROR_MEMORY_IS_FRAGMENTED");    }
  else if (status == RED_STATUS_ERROR_MEMORY_OVERFLOW)         { redInternalMark("[REDGPU] outStatuses returned RED_STATUS_ERROR_MEMORY_OVERFLOW");         }
  else if (status == RED_STATUS_ERROR_WINDOW_IS_ALREADY_USED)  { redInternalMark("[REDGPU] outStatuses returned RED_STATUS_ERROR_WINDOW_IS_ALREADY_USED");  }
  else if (status == RED_STATUS_ERROR_SURFACE_IS_LOST)         { redInternalMark("[REDGPU] outStatuses returned RED_STATUS_ERROR_SURFACE_IS_LOST");         }
  else if (status == RED_STATUS_ERROR_PRESENT_IS_OUT_OF_DATE)  { redInternalMark("[REDGPU] outStatuses returned RED_STATUS_ERROR_PRESENT_IS_OUT_OF_DATE");  }
  else if (status == RED_STATUS_ERROR_VALIDATION_FAILED)       { redInternalMark("[REDGPU] outStatuses returned RED_STATUS_ERROR_VALIDATION_FAILED");       }
  else if (status == RED_STATUS_ERROR_INVALID_GPU_CODE)        { redInternalMark("[REDGPU] outStatuses returned RED_STATUS_ERROR_INVALID_GPU_CODE");        }
  else                                                         { redInternalMark("[REDGPU] outStatuses returned RED_STATUS_UNKNOWN");                       }
#endif
}

typedef VkResult (GLAD_API_PTR * PFN_vkCreateRenderPass2KHR) (VkDevice device, const void * pCreateInfo, const VkAllocationCallbacks * pAllocator, VkRenderPass * pRenderPass);
typedef VkResult (GLAD_API_PTR * PFN_vkBindBufferMemory2KHR) (VkDevice device, unsigned bindInfoCount, const RedMemoryArray * pBindInfos);
typedef VkResult (GLAD_API_PTR * PFN_vkBindImageMemory2KHR)  (VkDevice device, unsigned bindInfoCount, const RedMemoryImage * pBindInfos);
typedef struct VkPhysicalDeviceVulkanMemoryModelFeaturesKHR {
  unsigned sType;
  void *   pNext;
  VkBool32 vulkanMemoryModel;
  VkBool32 vulkanMemoryModelDeviceScope;
  VkBool32 vulkanMemoryModelAvailabilityVisibilityChains;
} VkPhysicalDeviceVulkanMemoryModelFeaturesKHR;

#if defined(REDGPU_OS_ANDROID)
#define VK_STRUCTURE_TYPE_DEBUG_REPORT_CREATE_INFO_EXT 1000011000
#define VK_DEBUG_REPORT_WARNING_BIT_EXT 2
#define VK_DEBUG_REPORT_ERROR_BIT_EXT 8
VK_DEFINE_NON_DISPATCHABLE_HANDLE(VkDebugReportCallbackEXT)
typedef struct VkDebugReportCallbackCreateInfoEXT {
  VkStructureType sType;
  const void *    pNext;
  unsigned        flags;
  void *          pfnCallback;
  void *          pUserData;
} VkDebugReportCallbackCreateInfoEXT;
typedef VkResult (GLAD_API_PTR * PFN_vkCreateDebugReportCallbackEXT)  (VkInstance instance, const VkDebugReportCallbackCreateInfoEXT * pCreateInfo, const VkAllocationCallbacks * pAllocator, VkDebugReportCallbackEXT * pCallback);
typedef void     (GLAD_API_PTR * PFN_vkDestroyDebugReportCallbackEXT) (VkInstance instance, VkDebugReportCallbackEXT callback, const VkAllocationCallbacks * pAllocator);
#endif

typedef struct RedInternalInstanceProcedureAddresses {
  PFN_vkEnumerateInstanceLayerProperties             vkEnumerateInstanceLayerProperties;
  PFN_vkEnumerateInstanceExtensionProperties         vkEnumerateInstanceExtensionProperties;
  PFN_vkCreateInstance                               vkCreateInstance;
  PFN_vkGetDeviceProcAddr                            vkGetDeviceProcAddr;
#if defined(REDGPU_OS_ANDROID)
  PFN_vkCreateDebugReportCallbackEXT                 vkCreateDebugReportCallbackEXT;
#else
  PFN_vkCreateDebugUtilsMessengerEXT                 vkCreateDebugUtilsMessengerEXT;
#endif
  PFN_vkEnumeratePhysicalDevices                     vkEnumeratePhysicalDevices;
  PFN_vkEnumerateDeviceExtensionProperties           vkEnumerateDeviceExtensionProperties;
  PFN_vkGetPhysicalDeviceMemoryProperties2KHR        vkGetPhysicalDeviceMemoryProperties2KHR;
  PFN_vkGetPhysicalDeviceFeatures2KHR                vkGetPhysicalDeviceFeatures2KHR;
  PFN_vkGetPhysicalDeviceProperties2KHR              vkGetPhysicalDeviceProperties2KHR;
  PFN_vkGetPhysicalDeviceQueueFamilyProperties       vkGetPhysicalDeviceQueueFamilyProperties;
  PFN_vkCreateDevice                                 vkCreateDevice;
  PFN_vkGetDeviceQueue                               vkGetDeviceQueue;
  PFN_vkGetPhysicalDeviceFormatProperties            vkGetPhysicalDeviceFormatProperties;
  PFN_vkDestroyDevice                                vkDestroyDevice;
#if defined(REDGPU_OS_ANDROID)
  PFN_vkDestroyDebugReportCallbackEXT                vkDestroyDebugReportCallbackEXT;
#else
  PFN_vkDestroyDebugUtilsMessengerEXT                vkDestroyDebugUtilsMessengerEXT;
#endif
  PFN_vkDestroyInstance                              vkDestroyInstance;
  PFN_vkSetDebugUtilsObjectNameEXT                   vkSetDebugUtilsObjectNameEXT;
  PFN_vkGetPhysicalDeviceImageFormatProperties       vkGetPhysicalDeviceImageFormatProperties;
  PFN_vkGetBufferMemoryRequirements                  vkGetBufferMemoryRequirements;
  PFN_vkGetImageMemoryRequirements                   vkGetImageMemoryRequirements;
  PFN_vkAllocateMemory                               vkAllocateMemory;
  PFN_vkFreeMemory                                   vkFreeMemory;
  PFN_vkMapMemory                                    vkMapMemory;
  PFN_vkUnmapMemory                                  vkUnmapMemory;
  PFN_vkBindBufferMemory                             vkBindBufferMemory;
  PFN_vkBindImageMemory                              vkBindImageMemory;
  PFN_vkFlushMappedMemoryRanges                      vkFlushMappedMemoryRanges;
  PFN_vkInvalidateMappedMemoryRanges                 vkInvalidateMappedMemoryRanges;
  PFN_vkCreateBuffer                                 vkCreateBuffer;
  PFN_vkDestroyBuffer                                vkDestroyBuffer;
  PFN_vkCreateImage                                  vkCreateImage;
  PFN_vkDestroyImage                                 vkDestroyImage;
  PFN_vkCreateSampler                                vkCreateSampler;
  PFN_vkDestroySampler                               vkDestroySampler;
  PFN_vkCreateImageView                              vkCreateImageView;
  PFN_vkDestroyImageView                             vkDestroyImageView;
  PFN_vkCreateShaderModule                           vkCreateShaderModule;
  PFN_vkDestroyShaderModule                          vkDestroyShaderModule;
  PFN_vkCreateGraphicsPipelines                      vkCreateGraphicsPipelines;
  PFN_vkCreateComputePipelines                       vkCreateComputePipelines;
  PFN_vkDestroyPipeline                              vkDestroyPipeline;
  PFN_vkCreatePipelineCache                          vkCreatePipelineCache;
  PFN_vkDestroyPipelineCache                         vkDestroyPipelineCache;
  PFN_vkGetPipelineCacheData                         vkGetPipelineCacheData;
  PFN_vkMergePipelineCaches                          vkMergePipelineCaches;
  PFN_vkCreatePipelineLayout                         vkCreatePipelineLayout;
  PFN_vkDestroyPipelineLayout                        vkDestroyPipelineLayout;
  PFN_vkCreateDescriptorSetLayout                    vkCreateDescriptorSetLayout;
  PFN_vkDestroyDescriptorSetLayout                   vkDestroyDescriptorSetLayout;
  PFN_vkCreateDescriptorPool                         vkCreateDescriptorPool;
  PFN_vkAllocateDescriptorSets                       vkAllocateDescriptorSets;
  PFN_vkResetDescriptorPool                          vkResetDescriptorPool;
  PFN_vkDestroyDescriptorPool                        vkDestroyDescriptorPool;
  PFN_vkUpdateDescriptorSets                         vkUpdateDescriptorSets;
  PFN_vkCreateRenderPass                             vkCreateRenderPass;
  PFN_vkDestroyRenderPass                            vkDestroyRenderPass;
  PFN_vkCreateFramebuffer                            vkCreateFramebuffer;
  PFN_vkDestroyFramebuffer                           vkDestroyFramebuffer;
  PFN_vkCreateFence                                  vkCreateFence;
  PFN_vkDestroyFence                                 vkDestroyFence;
  PFN_vkGetFenceStatus                               vkGetFenceStatus;
  PFN_vkWaitForFences                                vkWaitForFences;
  PFN_vkResetFences                                  vkResetFences;
  PFN_vkCreateSemaphore                              vkCreateSemaphore;
  PFN_vkDestroySemaphore                             vkDestroySemaphore;
  PFN_vkCreateEvent                                  vkCreateEvent;
  PFN_vkDestroyEvent                                 vkDestroyEvent;
  PFN_vkGetEventStatus                               vkGetEventStatus;
  PFN_vkResetEvent                                   vkResetEvent;
  PFN_vkCreateCommandPool                            vkCreateCommandPool;
  PFN_vkAllocateCommandBuffers                       vkAllocateCommandBuffers;
  PFN_vkDestroyCommandPool                           vkDestroyCommandPool;
  PFN_vkResetCommandPool                             vkResetCommandPool;
  PFN_vkBeginCommandBuffer                           vkBeginCommandBuffer;
  PFN_vkEndCommandBuffer                             vkEndCommandBuffer;
  PFN_vkCmdInsertDebugUtilsLabelEXT                  vkCmdInsertDebugUtilsLabelEXT;
  PFN_vkCmdBeginDebugUtilsLabelEXT                   vkCmdBeginDebugUtilsLabelEXT;
  PFN_vkCmdEndDebugUtilsLabelEXT                     vkCmdEndDebugUtilsLabelEXT;
  PFN_vkQueueSubmit                                  vkQueueSubmit;
  PFN_vkQueueWaitIdle                                vkQueueWaitIdle;
#if defined(VK_USE_PLATFORM_WIN32_KHR)
  PFN_vkCreateWin32SurfaceKHR                        vkCreateWin32SurfaceKHR;
  PFN_vkGetPhysicalDeviceWin32PresentationSupportKHR vkGetPhysicalDeviceWin32PresentationSupportKHR;
#endif
#if defined(VK_USE_PLATFORM_XLIB_KHR) && defined(VK_USE_PLATFORM_XCB_KHR)
  PFN_vkCreateXcbSurfaceKHR                          vkCreateXcbSurfaceKHR;
  PFN_vkGetPhysicalDeviceXcbPresentationSupportKHR   vkGetPhysicalDeviceXcbPresentationSupportKHR;
  PFN_vkCreateXlibSurfaceKHR                         vkCreateXlibSurfaceKHR;
  PFN_vkGetPhysicalDeviceXlibPresentationSupportKHR  vkGetPhysicalDeviceXlibPresentationSupportKHR;
#endif
  PFN_vkDestroySurfaceKHR                            vkDestroySurfaceKHR;
  PFN_vkCreateSwapchainKHR                           vkCreateSwapchainKHR;
  PFN_vkGetSwapchainImagesKHR                        vkGetSwapchainImagesKHR;
  PFN_vkDestroySwapchainKHR                          vkDestroySwapchainKHR;
  PFN_vkGetPhysicalDeviceSurfaceSupportKHR           vkGetPhysicalDeviceSurfaceSupportKHR;
  PFN_vkGetPhysicalDeviceSurfaceFormatsKHR           vkGetPhysicalDeviceSurfaceFormatsKHR;
  PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR      vkGetPhysicalDeviceSurfaceCapabilitiesKHR;
  PFN_vkGetPhysicalDeviceSurfacePresentModesKHR      vkGetPhysicalDeviceSurfacePresentModesKHR;
  PFN_vkAcquireNextImageKHR                          vkAcquireNextImageKHR;
  PFN_vkQueuePresentKHR                              vkQueuePresentKHR;
  PFN_vkCreateRenderPass2KHR                         vkCreateRenderPass2KHR;
  PFN_vkSubmitDebugUtilsMessageEXT                   vkSubmitDebugUtilsMessageEXT;
  PFN_vkBindBufferMemory2KHR                         vkBindBufferMemory2KHR;
  PFN_vkBindImageMemory2KHR                          vkBindImageMemory2KHR;
#if REDGPU_COMPILE_SWITCH == REDGPU_COMPILE_SWITCH_DEBUG
  PFN_vkSetEvent                                     vkSetEvent;
  PFN_vkCmdWaitEvents                                vkCmdWaitEvents;
  PFN_vkCmdResetEvent                                vkCmdResetEvent;
#endif
  PFN_vkCreateQueryPool                              vkCreateQueryPool;
  PFN_vkDestroyQueryPool                             vkDestroyQueryPool;
  PFN_vkCmdResetQueryPool                            vkCmdResetQueryPool;
  PFN_vkCmdWriteTimestamp                            vkCmdWriteTimestamp;
  PFN_vkGetQueryPoolResults                          vkGetQueryPoolResults;
} RedInternalInstanceProcedureAddresses;

typedef struct RedInternalVirtualRedCallSetProcedureOutput {
  void *   redCallSetProcedureOutput;
  void *   context;
  unsigned gpuIndex;
} RedInternalVirtualRedCallSetProcedureOutput;

typedef struct RedInternalGpuProcedureAddresses {
  void * redCallGpuToCpuSignalSignal;
  void * redCallCopyArrayToArray;
  void * redCallCopyImageToImage;
  void * redCallCopyArrayToImage;
  void * redCallCopyImageToArray;
  void * redCallProcedure;
  void * redCallProcedureIndexed;
  void * redCallProcedureCompute;
  void * redCallSetProcedure;
  void * redCallSetProcedureIndices;
  void * redCallSetProcedureParametersStructs;
  void * redCallSetProcedureParametersHandles;
  void * redCallSetProcedureParametersVariables;
  void * redCallSetDynamicDepthBias;
  void * redCallSetDynamicDepthBounds;
  void * redCallSetDynamicStencilCompareMask;
  void * redCallSetDynamicStencilWriteMask;
  void * redCallSetDynamicStencilReference;
  void * redCallSetDynamicBlendConstants;
  void * redCallSetDynamicViewport;
  void * redCallSetDynamicScissor;
  RedInternalVirtualRedCallSetProcedureOutput redCallSetProcedureOutputVirtual;
  void * redCallEndProcedureOutput;
  void * redCallUsageOrderBarrier;
  void * _0;
  void * _1;
} RedInternalGpuProcedureAddresses;

typedef struct RedInternalQueueFamilyData {
  unsigned  queuesPrioritiesCount;
  float *   queuesPriorities;
  unsigned  queuesCount;
  VkQueue * queues;
} RedInternalQueueFamilyData;

typedef struct RedMemory {
  RedHandleMemory handle;
  void *          pointer;
} RedMemory;

#if REDGPU_COMPILE_SWITCH == REDGPU_COMPILE_SWITCH_DEBUG
typedef enum RedDebugInternalMemoryType {
  RED_DEBUG_INTERNAL_MEMORY_TYPE_VRAM     = 0,
  RED_DEBUG_INTERNAL_MEMORY_TYPE_UPLOAD   = 1,
  RED_DEBUG_INTERNAL_MEMORY_TYPE_READBACK = 2,
} RedDebugInternalMemoryType;

typedef struct RedDebugInternalArray {
  RedArray  array;
  RedMemory memory;
} RedDebugInternalArray;

typedef struct float4 {
  float x;
  float y;
  float z;
  float w;
} float4;
#endif

typedef struct RedInternalVirtualHandleMemoryMappingInfo {
  unsigned char * stagingPointer;
  uint64_t        mappedPointerBytesFirst;
  unsigned char * mappedPointer;
  RedBool32       memoryIsCpuCoherent;
} RedInternalVirtualHandleMemoryMappingInfo;

typedef struct RedInternalVirtualHandleMemory {
  RedHandleMemory                             handleActual;
  RedInternalVirtualHandleMemoryMappingInfo * mappingInfo;
  void *                                      allocation;
} RedInternalVirtualHandleMemory;

typedef struct RedInternalGpuInfo {
  VkPhysicalDeviceMemoryProperties                      vkPhysicalDeviceMemoryProperties;
  VkPhysicalDeviceFeatures                              vkPhysicalDeviceFeatures;
  VkPhysicalDeviceProperties                            vkPhysicalDeviceProperties;
  unsigned                                              vkDeviceExtensionPropertiesCount;
  VkExtensionProperties *                               vkDeviceExtensionProperties;
  unsigned                                              vkDeviceQueueCreateInfoCount;
  VkDeviceQueueCreateInfo *                             vkDeviceQueueCreateInfo;
  unsigned                                              vkQueueFamilyPropertiesCount;
  VkQueueFamilyProperties *                             vkQueueFamilyProperties;
  unsigned                                              vkQueueFamilyIndexesCount;
  unsigned *                                            vkQueueFamilyIndexes;
  unsigned                                              queueFamilyDataCount;
  RedInternalQueueFamilyData *                          queueFamilyData;
  VkPhysicalDeviceVulkanMemoryModelFeaturesKHR          vkPhysicalDeviceVulkanMemoryModelFeatures;
  uint64_t                                              _0;
  uint64_t                                              _1;
  void *                                                _2;
#if REDGPU_COMPILE_SWITCH == REDGPU_COMPILE_SWITCH_DEBUG
  RedBool32                                             debugArrayFeatureIsSupported;
  RedDebugInternalArray                                 debugArray;
  VkEvent                                               debugArrayGpuToCpuEvent;
  VkEvent                                               debugArrayCpuToGpuEvent;
  RedHandleCpuSignal                                    debugArrayCpuSignal;
#endif
} RedInternalGpuInfo;

typedef struct RedInternalContextColdData {
  unsigned                                gpusCount;
  RedInternalGpuInfo *                    gpus;
  unsigned                                vkPhysicalDevicesCount;
  VkPhysicalDevice *                      vkPhysicalDevices;
#if defined(REDGPU_OS_ANDROID)
  VkDebugReportCallbackEXT                vkDebugReportCallback;
#else
  VkDebugUtilsMessengerEXT                vkDebugUtilsMessenger;
#endif
  unsigned                                vkInstanceLayerPropertiesCount;
  VkLayerProperties *                     vkInstanceLayerProperties;
  unsigned                                vkInstanceExtensionPropertiesCount;
  VkExtensionProperties *                 vkInstanceExtensionProperties;
  RedTypeProcedureMalloc                  customMalloc;
  RedTypeProcedureFree                    customFree;
  RedTypeProcedureMallocTagged            customMallocTagged;
  RedTypeProcedureFreeTagged              customFreeTagged;
#if REDGPU_COMPILE_SWITCH == REDGPU_COMPILE_SWITCH_DEBUG
  unsigned                                debugArrayStructsCount;
  unsigned                                debugArrayStructsFloat4sCount;
  RedBool32                               debugArrayPrintWasEverCalled;
  RedTypeProcedureDebugArrayPrintCallback debugArrayPrintCallback;
#endif
} RedInternalContextColdData;

#if REDGPU_COMPILE_SWITCH == REDGPU_COMPILE_SWITCH_DEBUG
std::mutex __REDGPU_DEBUG_GLOBAL_3f5f84a81695ef9521b544bdc3d7d73cbb4aac3c18ce99fd19bdfd306996103e_debugPrintGuardAndSerializeMutex;
std::mutex __REDGPU_DEBUG_GLOBAL_3f5f84a81695ef9521b544bdc3d7d73cbb4aac3c18ce99fd19bdfd306996103e_debugArrayQueueSubmitGuardAndSerializeMutex;
#endif

#ifdef REDGPU_COMPILE_SWITCH_LOG
std::string __REDGPU_LOG_DEBUG_GLOBAL_d42da9280ad4f5cfc9d78b0b2e4b8379c98fa66902b11826264d59fcd722d6cf_toFlush;
#endif

typedef unsigned RedInternalContextExtensionBitflags;
typedef enum RedInternalContextExtensionBitflag {
  RED_INTERNAL_CONTEXT_EXTENSION_BITFLAG_TREAT_ALL_MAPPABLE_MEMORY_AS_NON_COHERENT = REDGPU_B32(0000,0000,0000,0000,0000,0000,0000,0001),
  RED_INTERNAL_CONTEXT_EXTENSION_BITFLAG_MANAGED_OUTPUT                            = REDGPU_B32(0000,0000,0000,0000,0000,0000,0000,0010),
} RedInternalContextExtensionBitflag;

typedef unsigned RedInternalGpuExtensionBitflags;
typedef enum RedInternalGpuExtensionBitflag {
  RED_INTERNAL_GPU_EXTENSION_BITFLAG_MEMORY_GET_BUDGET            = REDGPU_B32(0000,0000,0000,0000,0000,0000,0000,0001),
  RED_INTERNAL_GPU_EXTENSION_BITFLAG_DEPTH_STENCIL_RESOLVE        = REDGPU_B32(0000,0000,0000,0000,0000,0000,0000,0010),
  RED_INTERNAL_GPU_EXTENSION_BITFLAG_DEDICATE_MEMORY_NV           = REDGPU_B32(0000,0000,0000,0000,0000,0000,0000,0100),
  RED_INTERNAL_GPU_EXTENSION_BITFLAG_PROCEDURE_PARAMETERS_HANDLES = REDGPU_B32(0000,0000,0000,0000,0000,0000,0000,1000),
  RED_INTERNAL_GPU_EXTENSION_BITFLAG_BATCH_MEMORY_SET             = REDGPU_B32(0000,0000,0000,0000,0000,0000,0001,0000),
  RED_INTERNAL_GPU_EXTENSION_BITFLAG_RASTERIZATION_MODE           = REDGPU_B32(0000,0000,0000,0000,0000,0000,0010,0000),
  RED_INTERNAL_GPU_EXTENSION_BITFLAG_FORMAL_MEMORY_MODEL          = REDGPU_B32(0000,0000,0000,0000,0000,0000,0100,0000),
  RED_INTERNAL_GPU_EXTENSION_BITFLAG_RAY_TRACING                  = REDGPU_B32(0000,0000,0000,0000,0000,0000,1000,0000),
} RedInternalGpuExtensionBitflag;

// NOTE(Constantine): Copy of RedGpuInfoOptionalInfoArrayTimestamp without const's.
typedef struct RedInternalTypeGpuInfoOptionalInfoArrayTimestamp {
  RedGpuInfoOptionalInfo2 optionalInfo;
  const void *            next;
  RedBool32               supportsArrayTimestamp;
  unsigned *              queuesPrecisionBits64BitTicksCount; // Array of RedGpuInfo::queuesCount, can be NULL if supportsArrayTimestamp == 0
  float                   nanosecondsPerTick;
} RedInternalTypeGpuInfoOptionalInfoArrayTimestamp;

// NOTE(Constantine): Copy of RedGpuInfo without const's.
typedef struct RedInternalTypeGpuInfo {
  RedHandleGpu                   gpu;
  RedHandleGpuDevice             gpuDevice;
  char                           gpuName[256];
  RedGpuType                     gpuType;
  unsigned                       gpuVendorId;
  unsigned                       gpuDeviceId;
  unsigned                       gpuDriverVersion;
  unsigned char                  gpuProcedureCacheUuid[16];
  // Memory
  unsigned                       memoryTypesCount;
  RedMemoryType *                memoryTypes;            // Array of memoryTypesCount
  char **                        memoryTypesDescription; // Array of memoryTypesCount, can be 0
  unsigned                       memoryHeapsCount;
  RedMemoryHeap *                memoryHeaps;            // Array of memoryHeapsCount
  char **                        memoryHeapsDescription; // Array of memoryHeapsCount, can be 0
  // Queues
  unsigned                       queuesCount;
  RedHandleQueue *               queues;
  char **                        queuesDescription; // Array of queuesCount, can be 0
  unsigned *                     queuesFamilyIndex; // Array of queuesCount
  RedBool32 *                    queuesCanCopy;     // Array of queuesCount
  RedBool32 *                    queuesCanDraw;     // Array of queuesCount
  RedBool32 *                    queuesCanCompute;  // Array of queuesCount
  // Limits: Queues
  RedQueueCopyLimits *           queuesCopyLimits;  // Array of queuesCount
  // Limits: Memory                                                           // Guaranteed limits:       // Limits descriptions:
  unsigned                       maxMemoryAllocateCount;                      // Min: 4096                // Max number of allocations redMemoryAllocate* can make
  uint64_t                       minMemoryAllocateBytesAlignment;             // Min: 64                  // Min alignment bytes for redMemoryAllocate*::bytesCount
  uint64_t                       maxMemoryAllocateBytesCount;                 // Min: 2^30                // Max number of bytes for redMemoryAllocate*::bytesCount, even if there is more space available in the heap
  uint64_t                       minMemoryNonCoherentBlockBytesCount;         // Max: 256                 // Min bytes count and alignment bytes for non-coherent memory blocks. Non-coherent memory must operate only on blocks of this size. The end of non-coherent memory must be aligned to this limit (internal issue 611)
  unsigned                       maxCreateSamplerCount;                       // Min: 4000                // Max number of samplers redCreateSampler can allocate
  uint64_t                       minMemoryPageSeparationArrayImageBytesCount; // Min: 1, max: 2 * 65536   // Page separation in bytes between an array and an image to not alias each other, and vice versa. That is, the end of one resource and the beginning of the next resource with a different resource type must be on separate "pages" of size minMemoryPageSeparationArrayImageBytesCount
  // Limits: Arrays
  uint64_t                       minArrayROCStructMemberRangeBytesAlignment;  // Min: 1, max: 256         // Min alignment bytes for RedStructMemberArray::arrayRangeBytesFirst and RedStructMemberArray::arrayRangeBytesCount for RO constant arrays
  uint64_t                       maxArrayROCStructMemberRangeBytesCount;      // Min: 16384, on PC: 65536 // Max number of bytes for RedStructMemberArray::arrayRangeBytesCount                                                for RO constant arrays
  uint64_t                       minArrayRORWStructMemberRangeBytesAlignment; // Min: 1, max: 256         // Min alignment bytes for RedStructMemberArray::arrayRangeBytesFirst and RedStructMemberArray::arrayRangeBytesCount for RO or RW    arrays
  uint64_t                       maxArrayRORWStructMemberRangeBytesCount;     // Min: 2^27 (2048 * 65536) // Max number of bytes for RedStructMemberArray::arrayRangeBytesCount                                                for RO or RW    arrays
  unsigned                       maxArrayIndexUint32Value;                    // Min: (2^24)-1            // Max index value that can be used for indexed procedure calls. -1 excludes the primitive restart index value of 0xFFFFFFFF
  // Limits: Images
  unsigned                       maxImageDimensions1D;                        // Min: 4096                // Max dimension (width)                    supported for RED_IMAGE_DIMENSIONS_1D images
  unsigned                       maxImageDimensions2D;                        // Min: 4096                // Max dimension (width or height)          supported for RED_IMAGE_DIMENSIONS_2D images
  unsigned                       maxImageDimensions3D;                        // Min: 256                 // Max dimension (width or height or depth) supported for RED_IMAGE_DIMENSIONS_3D and RED_IMAGE_DIMENSIONS_3D_WITH_TEXTURE_DIMENSIONS_2D_AND_2D_LAYERED images
  unsigned                       maxImageDimensionsCube;                      // Min: 4096                // Max dimension (width or height)          supported for RED_IMAGE_DIMENSIONS_2D_WITH_TEXTURE_DIMENSIONS_CUBE_AND_CUBE_LAYERED images
  unsigned                       maxImageLayersCount;                         // Min: 256                 // Max number of layers for a layered image
  // Limits: Samplers
  float                          maxSamplerMipLodBias;                        // Min: 2.f                 // Max absolute sampler LOD bias. The sum of the redCreateSampler::mipLodBias and the Bias operand of image sampling operations in GPU code (or 0 if no Bias operand is provided to an image sampling operation) are clamped to the range [-maxSamplerMipLodBias,+maxSamplerMipLodBias]
  float                          maxSamplerAnisotropy;                        // Min: 16.f                // Max degree of sampler anisotropy. The maximum degree of anisotropic filtering used for an image sampling operation is the minimum of the redCreateSampler::maxAnisotropy and this limit
  unsigned                       precisionBitsSamplerFilteringMagMin;         // Min: 4                   // The number of bits of precision in the division along an axis of an image used for magnification and minification filters. 2^precisionBitsSamplerFilteringMagMin is the actual number of divisions along each axis of the image represented. Sub-texel values calculated during image sampling will snap to these locations when generating the filtered results
  unsigned                       precisionBitsSamplerFilteringMip;            // Min: 4                   // The number of bits of precision in the division that the LOD calculation for mip fetching get snapped to when determining the contribution from each mip level to the mip filtered results. 2^precisionBitsSamplerFilteringMip is the actual number of divisions. For example, if this value is 2 bits then when linearly filtering between two levels, each level could contribute 0%, 33%, 66%, or 100% (this is just an example and the amount of contribution should be covered by different equations)
  // Limits: Output declarations and outputs
  unsigned                       maxOutputWidth;                              // Min: 4096                // Max width  for an output
  unsigned                       maxOutputHeight;                             // Min: 4096                // Max height for an output
  unsigned                       maxOutputColorsCount;                        // Min: 4, max: 8           // Max number of colors an output can have
  RedMultisampleCountBitflags    supportedMultisampleCountsForOutputDepth;    // Min: 1 | 4               // Supported multisample counts for output depth
  RedMultisampleCountBitflags    supportedMultisampleCountsForOutputStencil;  // Min: 1 | 4               // Supported multisample counts for output stencil
  RedMultisampleCountBitflags    supportedMultisampleCountsForOutputColor;    // Min: 1 | 4               // Supported multisample counts for output color with floating-point formats. Does not cover integer formats (internal issue 550, c8c0862dee6ba9ed861e87648c59fcdf8a032f3a)
  RedMultisampleCountBitflags    supportedMultisampleCountsForEmptyOutput;    // Min: 1 | 4               // Supported multisample counts for empty output
  // Limits: Struct declarations and structs
  unsigned                       maxStructSlotsCount;                         // Min: 1024                // Max number of slots a struct can have
  unsigned                       maxStructPerStageArrayROCsCount;             // Min: 12                  // Max number of RO constant arrays allowed per-stage in a struct
  unsigned                       maxStructPerStageArrayRORWsCount;            // Min: 4                   // Max number of RO or RW    arrays allowed per-stage in a struct
  unsigned                       maxStructPerStageSamplersCount;              // Min: 16                  // Max number of samplers      allowed per-stage in a struct
  unsigned                       maxStructPerStageTextureROsCount;            // Min: 16                  // Max number of RO textures   allowed per-stage in a struct
  unsigned                       maxStructPerStageTextureRWsCount;            // Min: 4                   // Max number of RW textures   allowed per-stage in a struct
  unsigned                       maxStructPerStageAllResourcesCount;          // Min: 128                 // Max number of all resources allowed per-stage in a struct
  unsigned                       maxStructArrayROCsCount;                     // Min: 12 * stages count   // Max number of RO constant arrays allowed in all stages in a struct
  unsigned                       maxStructArrayRORWsCount;                    // Min: 4  * stages count   // Max number of RO or RW    arrays allowed in all stages in a struct
  unsigned                       maxStructSamplersCount;                      // Min: 16 * stages count   // Max number of samplers      allowed in all stages in a struct
  unsigned                       maxStructTextureROsCount;                    // Min: 16 * stages count   // Max number of RO textures   allowed in all stages in a struct
  unsigned                       maxStructTextureRWsCount;                    // Min: 4  * stages count   // Max number of RW textures   allowed in all stages in a struct
  // Limits: Procedure parameters
  unsigned                       maxProcedureParametersVariablesBytesCount;   // Min: 128                 // Max number of bytes for procedure parameters variables in all stages
  // Limits: Viewport
  unsigned                       maxViewportDimensions[2];                    // Min: 4096, 4096          // Max viewport dimensions (width, height)
  float                          minViewportBoundsRange;                      // Max: -8192.f             // Min viewport bounds range that the corners of a viewport must be contained in
  float                          maxViewportBoundsRange;                      // Min: 8191.f              // Max Viewport bounds range that the corners of a viewport must be contained in
  unsigned                       precisionBitsViewportCoordinates;            // Min: 4                   // The number of bits of subpixel precision in  viewport coordinates x and y
  unsigned                       precisionBitsViewportBounds;                 // Min: 0                   // The number of bits of subpixel precision for viewport bounds. The subpixel precision that floating-point viewport bounds are interpreted at is given by this limit
  // Limits: Vertex stage and fragment stage
  unsigned                       maxPerStageClipDistances;                    // Min: 8                   // Max number of clip distances allowed per-stage
  unsigned                       maxPerStageCullDistances;                    // Min: 8                   // Max number of cull distances allowed per-stage
  unsigned                       maxPerStageClipAndCullDistances;             // Min: 8                   // Max number of clip and cull distances combined allowed per-stage. The sum of the sizes of any pair of arrays declared with the ClipDistance and CullDistance built-in decoration used by a single stage must be less than or equal to this limit
  // Limits: Vertex stage
  unsigned                       maxVertexOutputLocationsCount;               // Min: 64                  // Max number of output locations in the vertex   stage. All location values used in GPU code must be below this limit
  // Limits: Fragment stage
  unsigned                       maxFragmentInputLocationsCount;              // Min: 64                  // Max number of input  locations in the fragment stage. All location values used in GPU code must be below this limit
  unsigned                       maxFragmentOutputColorsCount;                // Min: 4, max: 8           // Max number of output colors in the fragment stage
  unsigned                       maxFragmentOutputColorsCountBlendDualSource; // Min: 1, max: 8           // Max number of output colors in the fragment stage when using dual-source blend
  unsigned                       maxFragmentOutputResourcesCount;             // Min: 4                   // Max number of RW arrays, RW textures and output colors combined
  // Limits: Compute stage
  unsigned                       maxComputeSharedMemoryBytesCount;            // Min: 16384, on PC: 32768 // Max number of bytes for shared memory
  unsigned                       maxComputeWorkgroupsCount[3];                // Min: 65535, 65535, 65535 // Max number of compute workgroups (workgroupsCountX, workgroupsCountY, workgroupsCountZ) that may be dispatched by a single redCallProcedureCompute call
  unsigned                       maxComputeWorkgroupInvocationsCount;         // Min: 128                 // Max number of compute invocations in a single workgroup
  unsigned                       maxComputeWorkgroupDimensions[3];            // Min: 128, 128, 64        // Max workgroup dimensions (x, y, z)
  // Limits: OpImageSample*, OpImageFetch*
  int                            minImageSampleImageFetchOffset;              // Max: -8                  // The minimum offset value for the ConstOffset image operand of any of the OpImageSample* or OpImageFetch* image IR instructions
  unsigned                       maxImageSampleImageFetchOffset;              // Min: 7                   // The maximum offset value for the ConstOffset image operand of any of the OpImageSample* or OpImageFetch* image IR instructions
  // Limits: OpImage*Gather
  int                            minImageGatherOffset;                        // Max: -8                  // The minimum offset value for the Offset or ConstOffsets image operands of any of the OpImage*Gather image IR instructions
  unsigned                       maxImageGatherOffset;                        // Min: 7                   // The maximum offset value for the Offset or ConstOffsets image operands of any of the OpImage*Gather image IR instructions
  // Limits: interpolateAtOffset
  float                          minInterpolateAtOffset;                      // Max: -0.5f                                               // The minimum negative offset value for the offset operand of the InterpolateAtOffset extended IR instruction. The value describes the closed interval of supported interpolation offsets
  float                          maxInterpolateAtOffset;                      // Min: 0.5f - (1.f / 2.f^precisionBitsInterpolateAtOffset) // The maximum positive offset value for the offset operand of the InterpolateAtOffset extended IR instruction. The value describes the closed interval of supported interpolation offsets
  unsigned                       precisionBitsInterpolateAtOffset;            // Min: 4                                                   // The number of subpixel fractional bits that the x and y offsets to the InterpolateAtOffset extended IR instruction may be rounded to as fixed-point values. If precisionBitsInterpolateAtOffset is 4, this provides increments of (1.f / 2.f^4) = 0.0625, and thus the range of supported interpolation offsets would be [-0.5, 0.4375]
  // Limits: Copy calls
  uint64_t                       optimalCopyArrayImageRangeArrayBytesFirstBytesAlignment;
  uint64_t                       optimalCopyArrayImageRangeArrayTexelsCountToNextRowBytesAlignment;
  // Features
  RedBool32                      supportsWsi;                                            // WSI specific functionality is supported by the GPU.                                          If this feature is not available, the GPU handle should not be passed to WSI specific procedures
  RedBool32                      supportsMemoryGetBudget;                                // redMemoryGetBudget can return memory budget values.                                          If this feature is not available, redMemoryGetBudget::outMemoryBudget values won't be set and redMemoryGetBudget::outStatuses will return RED_STATUS_ERROR_FEATURE_IS_NOT_FOUND
  RedBool32                      supportsFullArrayIndexUint32Value;                      // Full 32-bit range of indices is supported.                                                   If this feature is available,     maxArrayIndexUint32Value must be (2^32)-1. -1 excludes the primitive restart index value of 0xFFFFFFFF
  RedBool32                      supportsSamplerAnisotropy;                              // Anisotropic filtering is supported.                                                          If this feature is not available, the redCreateSampler::enableAnisotropy must be set to 0 and the redCreateSampler::maxAnisotropy is ignored
  RedBool32                      supportsTextureDimensionsCubeLayered;                   // Specifies whether RED_TEXTURE_DIMENSIONS_CUBE_LAYERED textures can be created.               If this feature is available,     the SampledCubeArray and ImageCubeArray IR capabilities can be used
  RedBool32                      supportsProcedureStateRasterizationDepthClamp;          // Depth clamping is supported.                                                                 If this feature is not available, the RedProcedureState::rasterizationDepthClampEnable     must be set to 0
  RedBool32                      supportsProcedureStateRasterizationDepthBiasDynamic;    // Dynamic depth bias is supported.                                                             If this feature is not available, the RedProcedureState::rasterizationDepthBiasDynamic     must be set to 0 and redCallSetDynamicDepthBias          must not be called
  RedBool32                      supportsProcedureStateRasterizationDepthBiasClamp;      // Depth bias clamping is supported.                                                            If this feature is not available, the RedProcedureState::rasterizationDepthBiasStaticClamp must be set to 0 and if RedProcedureState::rasterizationDepthBiasDynamic is available redCallSetDynamicDepthBias::clamp must be set to 0
  RedBool32                      supportsProcedureStateMultisampleSampleShading;         // Specifies whether sample shading and multisample interpolation are supported.                If this feature is not available, the RedProcedureState::multisampleSampleShadingEnable    must be set to 0 and the RedProcedureState::multisampleSampleShadingMin is ignored. This also specifies whether IR can declare the SampleRateShading capability
  RedBool32                      supportsProcedureStateMultisampleAlphaToOne;            // The fragment alpha component can be forced to maximum representable alpha value.             If this feature is not available, the RedProcedureState::multisampleAlphaToOneEnable       must be set to 0
  RedBool32                      supportsProcedureStateDepthTestBoundsTest;              // Depth bounds test is supported.                                                              If this feature is not available, the RedProcedureState::depthTestBoundsTestEnable         must be set to 0 and the RedProcedureState::depthTestBoundsTestStaticMin, RedProcedureState::depthTestBoundsTestStaticMax, redCallSetDynamicDepthBounds::min and redCallSetDynamicDepthBounds::max are ignored
  RedBool32                      supportsProcedureStateDepthTestBoundsTestDynamic;       // Dynamic depth bounds test is supported.                                                      If this feature is not available, the RedProcedureState::depthTestBoundsTestDynamic        must be set to 0 and redCallSetDynamicDepthBounds        must not be called
  RedBool32                      supportsProcedureStateStencilTestFrontAndBackDynamicCompareMask; // Dynamic compare mask is supported.                                                  If this feature is not available, the RedProcedureState::stencilTestFrontAndBackDynamicCompareMask must be set to 0 and redCallSetDynamicStencilCompareMask must not be called
  RedBool32                      supportsProcedureStateStencilTestFrontAndBackDynamicWriteMask;   // Dynamic write mask is supported.                                                    If this feature is not available, the RedProcedureState::stencilTestFrontAndBackDynamicWriteMask   must be set to 0 and redCallSetDynamicStencilWriteMask   must not be called
  RedBool32                      supportsProcedureStateBlendLogicOp;                     // Logic operations are supported.                                                              If this feature is not available, the RedProcedureState::blendLogicOpEnable                must be set to 0 and the RedProcedureState::blendLogicOp is ignored
  RedBool32                      supportsProcedureStateOutputColorsBlendVaryingPerColor; // Blend operations are controlled per output color.                                            If this feature is not available, the RedProcedureState::outputColorsBlend* values for all output colors must be identical. Otherwise, different RedProcedureState::outputColorsBlend* values can be provided for output colors
  RedBool32                      supportsProcedureStateOutputColorsBlendDualSource;      // Blend operations which take two sources are supported.                                       If this feature is not available, the RED_BLEND_FACTOR_SOURCE1_COLOR, RED_BLEND_FACTOR_ONE_MINUS_SOURCE1_COLOR, RED_BLEND_FACTOR_SOURCE1_ALPHA or RED_BLEND_FACTOR_ONE_MINUS_SOURCE1_ALPHA must not be used as source or target blend factors
  RedBool32                      supportsMultisampleEmptyOutputVariableMultisampleCount; // Multisample rate can be different for all procedure calls in an empty output.                If this feature is not available, the current output is empty, and this is not the first procedure call for the current output, then the multisample count specified by this procedure must match that set in the previous procedure
  RedBool32                      supportsMultisampleStandardSampleLocations;             // Specifies whether rasterization uses the standard sample locations.                          If this feature is available,     the implementation uses the documented sample locations. Otherwise, the implementation may use different sample locations
  RedBool32                      supportsGpuCodeWritesAndAtomicsInStageVertex;           // Writes and atomic operations on RW textures are supported in the vertex   stage.             If this feature is not available, all RW texture variables used by the vertex   stage in GPU code must be decorated with the NonWritable decoration in IR
  RedBool32                      supportsGpuCodeWritesAndAtomicsInStageFragment;         // Writes and atomic operations on RW textures are supported in the fragment stage.             If this feature is not available, all RW texture variables used by the fragment stage in GPU code must be decorated with the NonWritable decoration in IR
  RedBool32                      supportsGpuCodeImageGatherExtended;                     // The extended set of texture gather IR instructions are available in GPU code.                If this feature is not available, the OpImage*Gather IR instructions do not support the Offset and ConstOffsets operands. This also specifies whether IR can declare the ImageGatherExtended capability
  RedBool32                      supportsGpuCodeTextureRWExtendedFormats;                // All the extended RW texture formats are available in GPU code.                               See StorageImageExtendedFormats in IR specification (internal issue 1273, 0a7a04f32bd473bc7428efdbbbe132f33afad68c)
  RedBool32                      supportsGpuCodeTextureRWMultisample;                    // Multisample RW textures are supported.                                                       If this feature is not available, images set as RED_STRUCT_MEMBER_TYPE_TEXTURE_RW must be created with multisample count equal to RED_MULTISAMPLE_COUNT_BITFLAG_1. This also specifies whether IR can declare the StorageImageMultisample capability
  RedBool32                      supportsGpuCodeTextureRWReadWithoutFormat;              // RW textures don't require a format qualifier to be specified when reading from  RW textures. If this feature is not available, the OpImageRead  IR instruction must not have an OpTypeImage of Unknown. This also specifies whether IR can declare the StorageImageReadWithoutFormat  capability
  RedBool32                      supportsGpuCodeTextureRWWriteWithoutFormat;             // RW textures don't require a format qualifier to be specified when writing to    RW textures. If this feature is not available, the OpImageWrite IR instruction must not have an OpTypeImage of Unknown. This also specifies whether IR can declare the StorageImageWriteWithoutFormat capability
  RedBool32                      supportsGpuCodeDynamicallyIndexableArraysOfArrayROCs;   // Arrays of RO constant arrays can be indexed by dynamically uniform integer expressions in GPU code. If this feature is not available, resources with a struct member type of RED_STRUCT_MEMBER_TYPE_ARRAY_RO_CONSTANT must be indexed only by constant integral expressions when aggregated into arrays in GPU code. This also specifies whether IR can declare the UniformBufferArrayDynamicIndexing capability
  RedBool32                      supportsGpuCodeDynamicallyIndexableArraysOfArrayRORWs;  // Arrays of RO or RW    arrays can be indexed by dynamically uniform integer expressions in GPU code. If this feature is not available, resources with a struct member type of RED_STRUCT_MEMBER_TYPE_ARRAY_RO_RW       must be indexed only by constant integral expressions when aggregated into arrays in GPU code. This also indicates whether IR can declare the StorageBufferArrayDynamicIndexing capability
  RedBool32                      supportsGpuCodeDynamicallyIndexableArraysOfSamplers;    // Arrays of samplers           can be indexed by dynamically uniform integer expressions in GPU code. If this feature is not available, resources with a struct member type of RED_STRUCT_MEMBER_TYPE_SAMPLER           must be indexed only by constant integral expressions when aggregated into arrays in GPU code. This also specifies whether IR can declare the SampledImageArrayDynamicIndexing  capability
  RedBool32                      supportsGpuCodeDynamicallyIndexableArraysOfTextureROs;  // Arrays of RO textures        can be indexed by dynamically uniform integer expressions in GPU code. If this feature is not available, resources with a struct member type of RED_STRUCT_MEMBER_TYPE_TEXTURE_RO        must be indexed only by constant integral expressions when aggregated into arrays in GPU code. This also specifies whether IR can declare the SampledImageArrayDynamicIndexing  capability
  RedBool32                      supportsGpuCodeDynamicallyIndexableArraysOfTextureRWs;  // Arrays of RW textures        can be indexed by dynamically uniform integer expressions in GPU code. If this feature is not available, resources with a struct member type of RED_STRUCT_MEMBER_TYPE_TEXTURE_RW        must be indexed only by constant integral expressions when aggregated into arrays in GPU code. This also specifies whether IR can declare the StorageImageArrayDynamicIndexing  capability
  RedBool32                      supportsGpuCodeClipDistance;                            // Clip distances are supported in GPU code.                                                    If this feature is not available, any members decorated with the ClipDistance built-in decoration must not be read from or written to in GPU code. This also specifies whether IR can declare the ClipDistance capability
  RedBool32                      supportsGpuCodeCullDistance;                            // Cull distances are supported in GPU code.                                                    If this feature is not available, any members decorated with the CullDistance built-in decoration must not be read from or written to in GPU code. This also specifies whether IR can declare the CullDistance capability
  RedBool32                      supportsGpuCodeInt64;                                   // 64-bit integers (signed and unsigned) are supported in GPU code.                             If this feature is not available, 64-bit integer        types must not be used in GPU code. This also specifies whether IR can declare the Int64   capability
  RedBool32                      supportsGpuCodeFloat64;                                 // 64-bit floats   (doubles)             are supported in GPU code.                             If this feature is not available, 64-bit floating-point types must not be used in GPU code. This also specifies whether IR can declare the Float64 capability
  RedBool32                      supportsGpuCodeMinLod;                                  // Image operations that specify the minimum resource LOD are supported in GPU code.            If this feature is not available, the    MinLod image operand must not be used in GPU code. This also specifies whether IR can declare the MinLod  capability
  // Image formats
  RedImageFormatLimits *         imageFormatsLimitsImageDimensions1D;                                        // Count: 131, indexable with RedFormat values
  RedImageFormatLimits *         imageFormatsLimitsImageDimensions2D;                                        // Count: 131, indexable with RedFormat values
  RedImageFormatLimits *         imageFormatsLimitsImageDimensions2DMultisample;                             // Count: 131, indexable with RedFormat values
  RedImageFormatLimits *         imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered; // Count: 131, indexable with RedFormat values
  RedImageFormatLimits *         imageFormatsLimitsImageDimensions3D;                                        // Count: 131, indexable with RedFormat values
  RedImageFormatLimits *         imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered;     // Count: 131, indexable with RedFormat values
  RedImageFormatFeatures *       imageFormatsFeatures;                                                       // Count: 131, indexable with RedFormat values
  void *                         optionalInfo;                                                               // A 0 or a pointer to a singly linked list of RedGpuInfoOptionalInfo* structs
} RedInternalTypeGpuInfo;

typedef struct RedInternalTypeContext {
  unsigned                                                gpusCount;
  RedGpuInfo *                                            gpus;
  RedStatuses *                                           gpusStatuses;
  RedHandleContext                                        contextHandle;
  void *                                                  userData;
  struct {
#if defined(REDGPU_OS_ANDROID)
    void *                                                debugUtilsMessengerCallback;
#else
    PFN_vkDebugUtilsMessengerCallbackEXT                  debugUtilsMessengerCallback;
#endif
    RedInternalInstanceProcedureAddresses *               instanceProcedureAddresses;
    unsigned                                              gpuProcedureAddressesCount;
    RedInternalGpuProcedureAddresses *                    gpuProcedureAddresses;
    unsigned                                              hotGpuHandlesCount;
    RedHandleGpu *                                        hotGpuHandles;
    RedInternalContextColdData *                          cold;
    RedInternalContextExtensionBitflags                   contextExtensionBitflags;
    unsigned                                              gpuExtensionBitflagsCount;
    RedInternalGpuExtensionBitflags *                     gpuExtensionBitflags;
    void *                                                pphi;
  }                                                       internal;
} RedInternalTypeContext;

typedef RedInternalTypeContext * RedInternalContext;

// Internal

#ifdef REDGPU_COMPILE_SWITCH_LOG
static void redInternalLogDebugPrintf(const char * format, ...) {
  std::string text;
  {
    int size = 0;
    {
      va_list varargs;
      va_start(varargs, format);
      size = stbsp_vsnprintf(0, 0, format, varargs);
      va_end(varargs);
    }
    char * temp = new(std::nothrow) char [size + 1];
    if (temp == 0) {
      return;
    }
    {
      va_list varargs;
      va_start(varargs, format);
      stbsp_vsnprintf(temp, size + 1, format, varargs);
      va_end(varargs);
    }
    temp[size] = 0;
    text = temp;
    delete[] temp;
  }
  __REDGPU_LOG_DEBUG_GLOBAL_d42da9280ad4f5cfc9d78b0b2e4b8379c98fa66902b11826264d59fcd722d6cf_toFlush += text;
}

static void redInternalLogDebugFflush(void * stream) {
  time_t _time = time(NULL);
#ifdef REDGPU_OS_WINDOWS
  std::ofstream fsAppend("redgpu.txt", std::ofstream::app);
  fsAppend << "[REDGPU LOG DEBUG LIBRARY] "
#endif
#ifdef REDGPU_OS_LINUX
  std::ofstream fsAppend("/tmp/redgpu.txt", std::ofstream::app);
  fsAppend << "[REDGPU LOG DEBUG LIBRARY] pid: " << getpid() << ", "
#endif
  << "time: " << asctime(localtime(&_time)) << "\n" << __REDGPU_LOG_DEBUG_GLOBAL_d42da9280ad4f5cfc9d78b0b2e4b8379c98fa66902b11826264d59fcd722d6cf_toFlush << "\n";
  fsAppend.close();
  __REDGPU_LOG_DEBUG_GLOBAL_d42da9280ad4f5cfc9d78b0b2e4b8379c98fa66902b11826264d59fcd722d6cf_toFlush = "";
}
#endif

static void * redInternalAdaptiveMalloc(RedTypeProcedureMalloc customMalloc, RedTypeProcedureMallocTagged customMallocTagged, size_t bytesCount, RedProcedureId procedureId, uint64_t memoryAllocationCode, const RedMemoryAllocationTag * memoryAllocationTag, const char * file, int line, void * userData) {
  if (customMallocTagged != NULL) {
    return customMallocTagged(bytesCount, procedureId, memoryAllocationCode, memoryAllocationTag, file, line, userData);
  } else if (customMalloc != NULL) {
    return customMalloc(bytesCount);
  }
  return NULL;
}

static void redInternalAdaptiveFree(RedTypeProcedureFree customFree, RedTypeProcedureFreeTagged customFreeTagged, void * pointer, RedProcedureId procedureId, uint64_t memoryAllocationCode, const RedMemoryAllocationTag * memoryAllocationTag, const char * file, int line, void * userData) {
  if (pointer != NULL) {
    if (customFreeTagged != NULL) {
      customFreeTagged(pointer, procedureId, memoryAllocationCode, memoryAllocationTag, file, line, userData);
    } else if (customFree != NULL) {
      customFree(pointer);
    }
  }
}

#define REDGPU_INTERNAL_ADAPTIVE_MALLOC_STABLE(BYTES_COUNT, ALLOCATION_CODE) redInternalAdaptiveMalloc(customMalloc, customMallocTagged, BYTES_COUNT, procedureId, ALLOCATION_CODE, NULL, optionalFile, optionalLine, optionalUserData)
#define REDGPU_INTERNAL_ADAPTIVE_FREE_STABLE(POINTER_TO_FREE, ALLOCATION_CODE) redInternalAdaptiveFree(customFree, customFreeTagged, (void *)(POINTER_TO_FREE), procedureId, ALLOCATION_CODE, NULL, optionalFile, optionalLine, optionalUserData)
#define REDGPU_INTERNAL_ADAPTIVE_MALLOC(BYTES_COUNT, ALLOCATION_CODE) redInternalAdaptiveMalloc(ctx->internal.cold->customMalloc, ctx->internal.cold->customMallocTagged, BYTES_COUNT, procedureId, ALLOCATION_CODE, NULL, optionalFile, optionalLine, optionalUserData)
#define REDGPU_INTERNAL_ADAPTIVE_FREE(POINTER_TO_FREE, ALLOCATION_CODE) redInternalAdaptiveFree(ctx->internal.cold->customFree, ctx->internal.cold->customFreeTagged, POINTER_TO_FREE, procedureId, ALLOCATION_CODE, NULL, optionalFile, optionalLine, optionalUserData)

static inline void * redInternalMemoryClear(void * s, uint64_t n) {
  unsigned char * cs = (unsigned char *)s;
  for (uint64_t i = 0; i < n; i += 1) {
    cs[i] = 0;
  }
  return s;
}

static inline void * redInternalMemoryCopy(void * dest, const void * src, uint64_t n) {
  const unsigned char * csrc = (const unsigned char *)src;
  unsigned char * cdest = (unsigned char *)dest;
  for (uint64_t i = 0; i < n; i += 1) {
    cdest[i] = csrc[i];
  }
  return dest;
}

static inline int redInternalStringCompare(const char * l, const char * r) {
  for (; *l == *r && *l; l++, r++) {
  }
  return *(unsigned char *)l - *(unsigned char *)r;
}

#ifdef __clang__
static inline __attribute__((always_inline))
#else
static inline
#endif
RedBool32 redInternalFindInstanceLayer(RedInternalContext ctx, const char * layerName, RedProcedureId procedureId, RedBool32 required, const char * optionalFile, int optionalLine) {
  for (unsigned i = 0; i < ctx->internal.cold->vkInstanceLayerPropertiesCount; i += 1) {
    const VkLayerProperties * layerProperties = &ctx->internal.cold->vkInstanceLayerProperties[i];
    if (redInternalStringCompare(layerProperties->layerName, layerName) == 0) {
      return 1;
    }
  }

#if REDGPU_COMPILE_SWITCH == REDGPU_COMPILE_SWITCH_DEBUG
  if (procedureId == RED_PROCEDURE_ID_redCreateContext && required == 1) {
    std::lock_guard<std::mutex> __debugPrintGuardAndSerializeMutexScopeGuard(__REDGPU_DEBUG_GLOBAL_3f5f84a81695ef9521b544bdc3d7d73cbb4aac3c18ce99fd19bdfd306996103e_debugPrintGuardAndSerializeMutex);
    if (optionalFile != NULL) {
      REDGPU_PRINTF("[REDGPU DEBUG LIBRARY][RED_PROCEDURE_ID_redCreateContext][%s:%d]: Requested context layer \"%s\" is not found.\n", optionalFile, optionalLine, layerName);
    } else {
      REDGPU_PRINTF("[REDGPU DEBUG LIBRARY][RED_PROCEDURE_ID_redCreateContext]: Requested context layer \"%s\" is not found.\n", layerName);
    }
  }
#endif

  return 0;
}

#ifdef __clang__
static inline __attribute__((always_inline))
#else
static inline
#endif
RedBool32 redInternalFindInstanceExtension(RedInternalContext ctx, const char * extensionName, RedProcedureId procedureId, RedBool32 required, const char * optionalFile, int optionalLine) {
  for (unsigned i = 0; i < ctx->internal.cold->vkInstanceExtensionPropertiesCount; i += 1) {
    const VkExtensionProperties * extensionProperties = &ctx->internal.cold->vkInstanceExtensionProperties[i];
    if (redInternalStringCompare(extensionProperties->extensionName, extensionName) == 0) {
      return 1;
    }
  }

#if REDGPU_COMPILE_SWITCH == REDGPU_COMPILE_SWITCH_DEBUG
  if (procedureId == RED_PROCEDURE_ID_redCreateContext && required == 1) {
    std::lock_guard<std::mutex> __debugPrintGuardAndSerializeMutexScopeGuard(__REDGPU_DEBUG_GLOBAL_3f5f84a81695ef9521b544bdc3d7d73cbb4aac3c18ce99fd19bdfd306996103e_debugPrintGuardAndSerializeMutex);
    if (optionalFile != NULL) {
      REDGPU_PRINTF("[REDGPU DEBUG LIBRARY][RED_PROCEDURE_ID_redCreateContext][%s:%d]: Requested context extension \"%s\" is not found.\n", optionalFile, optionalLine, extensionName);
    } else {
      REDGPU_PRINTF("[REDGPU DEBUG LIBRARY][RED_PROCEDURE_ID_redCreateContext]: Requested context extension \"%s\" is not found.\n", extensionName);
    }
  }
#endif

  return 0;
}

#ifdef __clang__
static inline __attribute__((always_inline))
#else
static inline
#endif
RedBool32 redInternalFindDeviceExtension(RedInternalGpuInfo * iGpuInfo, const char * extensionName, RedProcedureId procedureId, RedBool32 required, const char * optionalFile, int optionalLine) {
  for (unsigned j = 0; j < iGpuInfo->vkDeviceExtensionPropertiesCount; j += 1) {
    const VkExtensionProperties * extensionProperties = &iGpuInfo->vkDeviceExtensionProperties[j];
    if (redInternalStringCompare(extensionProperties->extensionName, extensionName) == 0) {
      return 1;
    }
  }

#if REDGPU_COMPILE_SWITCH == REDGPU_COMPILE_SWITCH_DEBUG
  if (procedureId == RED_PROCEDURE_ID_redCreateContext && required == 1) {
    std::lock_guard<std::mutex> __debugPrintGuardAndSerializeMutexScopeGuard(__REDGPU_DEBUG_GLOBAL_3f5f84a81695ef9521b544bdc3d7d73cbb4aac3c18ce99fd19bdfd306996103e_debugPrintGuardAndSerializeMutex);
    if (optionalFile != NULL) {
      REDGPU_PRINTF("[REDGPU DEBUG LIBRARY][RED_PROCEDURE_ID_redCreateContext][%s:%d]: Requested GPU extension \"%s\" is not found.\n", optionalFile, optionalLine, extensionName);
    } else {
      REDGPU_PRINTF("[REDGPU DEBUG LIBRARY][RED_PROCEDURE_ID_redCreateContext]: Requested GPU extension \"%s\" is not found.\n", extensionName);
    }
  }
#endif

  return 0;
}

#ifdef __clang__
static inline __attribute__((always_inline))
#else
static inline
#endif
void redInternalSetCapableQueueBools(RedInternalGpuInfo * iGpuInfo, VkQueueFlags capableFlags, unsigned * outCapableQueueBoolsArray) {
  for (unsigned j = 0, k_offset = 0; j < iGpuInfo->queueFamilyDataCount; j += 1) {
    VkQueueFamilyProperties    * queueFamilyProperties = &iGpuInfo->vkQueueFamilyProperties[j];
    RedInternalQueueFamilyData * queueFamilyData       = &iGpuInfo->queueFamilyData[j];

    for (unsigned k = 0; k < queueFamilyData->queuesCount; k += 1) {
      outCapableQueueBoolsArray[k_offset + k] = (queueFamilyProperties->queueFlags & capableFlags) == capableFlags ? 1 : 0;
    }
    k_offset += queueFamilyData->queuesCount;
  }
}

static inline VkImageUsageFlags redInternalImageGetUsage(RedInternalContext ctx, unsigned gpuIndex, RedFormat format, RedBool32 multisample) {
  VkImageUsageFlags usage = 0;

  RedImageFormatFeatures imageFormatFeatures = ctx->gpus[gpuIndex].imageFormatsFeatures[format];

  if (imageFormatFeatures.supportsCopyR == 1) {
    usage |= VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
  }

  if (imageFormatFeatures.supportsCopyW == 1) {
    usage |= VK_IMAGE_USAGE_TRANSFER_DST_BIT;
  }

  if (imageFormatFeatures.supportsTextureRO == 1) {
    usage |= VK_IMAGE_USAGE_SAMPLED_BIT;
  }

  if (imageFormatFeatures.supportsTextureRW == 1) {
    if (multisample == 0) {
      usage |= VK_IMAGE_USAGE_STORAGE_BIT;
    } else {
      if (ctx->gpus[gpuIndex].supportsGpuCodeTextureRWMultisample == 1) {
        usage |= VK_IMAGE_USAGE_STORAGE_BIT;
      }
    }
  }

  if (format == RED_FORMAT_DEPTH_16_UINT_TO_FLOAT_0_1                ||
      format == RED_FORMAT_DEPTH_32_FLOAT                            ||
      format == RED_FORMAT_DEPTH_24_UINT_TO_FLOAT_0_1_STENCIL_8_UINT ||
      format == RED_FORMAT_DEPTH_32_FLOAT_STENCIL_8_UINT)
  {
    if (imageFormatFeatures.supportsOutputDepthStencil == 1) {
      usage |= VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
    }
  } else {
    if (imageFormatFeatures.supportsOutputColor == 1) {
      usage |= VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    }
  }

  return usage;
}

#ifdef __clang__
static inline __attribute__((always_inline))
#else
static inline
#endif
VkResult redInternalImageGetLimits(RedInternalContext ctx, unsigned gpuIndex, VkPhysicalDevice physicalDevice, RedImageDimensions dimensions, RedFormat format, RedBool32 multisample, RedImageFormatLimits * outImageFormatLimits) {
  RedInternalInstanceProcedureAddresses * vk = ctx->internal.instanceProcedureAddresses;

  VkImageUsageFlags usage = redInternalImageGetUsage(ctx, gpuIndex, format, multisample);

  if (usage == 0) {
    return (VkResult)RED_STATUS_ERROR_FORMAT_IS_NOT_SUPPORTED;
  }

  VkImageCreateFlags createFlags = 0;
  {
    RedImageDimensions imageDimensions = dimensions;
    if (imageDimensions == RED_IMAGE_DIMENSIONS_2D_WITH_TEXTURE_DIMENSIONS_CUBE_AND_CUBE_LAYERED) {
      dimensions  = RED_IMAGE_DIMENSIONS_2D;
      createFlags = VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT;
    } else if (imageDimensions == RED_IMAGE_DIMENSIONS_3D_WITH_TEXTURE_DIMENSIONS_2D_AND_2D_LAYERED) {
      dimensions  = RED_IMAGE_DIMENSIONS_3D;
      createFlags = VK_IMAGE_CREATE_2D_ARRAY_COMPATIBLE_BIT;
    }
  }

  return vk->vkGetPhysicalDeviceImageFormatProperties(physicalDevice, (VkFormat)format, (VkImageType)dimensions, VK_IMAGE_TILING_OPTIMAL, usage, createFlags, (VkImageFormatProperties *)outImageFormatLimits);
}

#if REDGPU_COMPILE_SWITCH == REDGPU_COMPILE_SWITCH_DEBUG

#define REDGPU_INTERNAL_DEBUG_SEND_ERROR_MESSAGE(InternalContext, GpuHandle, MessageIdName, StatusCode, Message, OptionalFile, OptionalLine)\
  {\
    std::lock_guard<std::mutex> __debugPrintGuardAndSerializeMutexScopeGuard(__REDGPU_DEBUG_GLOBAL_3f5f84a81695ef9521b544bdc3d7d73cbb4aac3c18ce99fd19bdfd306996103e_debugPrintGuardAndSerializeMutex);\
    if (OptionalFile != NULL) {\
      REDGPU_PRINTF("[REDGPU DEBUG LIBRARY][DEBUG CALLBACK][%s][%s:%d]: %s (statusCode: %d, context: %p, gpu: %p)\n", MessageIdName, OptionalFile, OptionalLine, Message, StatusCode, REDGPU_PRINT_POINTER(InternalContext), REDGPU_PRINT_POINTER(GpuHandle));\
    } else {\
      REDGPU_PRINTF("[REDGPU DEBUG LIBRARY][DEBUG CALLBACK][%s]: %s (statusCode: %d, context: %p, gpu: %p)\n", MessageIdName, Message, StatusCode, REDGPU_PRINT_POINTER(InternalContext), REDGPU_PRINT_POINTER(GpuHandle));\
    }\
    REDGPU_FFLUSH(stdout);\
  }

#else

static void redInternalDebugSendErrorMessage(RedInternalContext InternalContext, RedHandleGpu GpuHandle, const char * MessageIdName, unsigned StatusCode, const char * Message, const char * OptionalFile, int OptionalLine) {
  if (InternalContext != NULL) {
    if (InternalContext->internal.debugUtilsMessengerCallback != NULL && InternalContext->contextHandle != NULL) {
      RedHandleInfo info;
      info._0         = 1000128000;
      info._1         = NULL;
      info.handleType = RED_HANDLE_TYPE_GPU;
      info.handle     = (void *)GpuHandle;
      info.handleName = "";
      RedDebugCallbackData data;
      data._0               = 1000128003;
      data._1               = NULL;
      data.statusCode       = StatusCode;
      data.messageIdName    = MessageIdName;
      data.statusHresult    = 0;
      data.message          = Message;
      data._2               = 0;
      data._3               = NULL;
      data._4               = 0;
      data._5               = NULL;
      data.handleInfosCount = GpuHandle == NULL ? 0    : 1;
      data.handleInfos      = GpuHandle == NULL ? NULL : &info;
      if (InternalContext->internal.instanceProcedureAddresses->vkSubmitDebugUtilsMessageEXT != NULL) {
        InternalContext->internal.instanceProcedureAddresses->vkSubmitDebugUtilsMessageEXT((VkInstance)InternalContext->contextHandle, VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT, VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT, (const VkDebugUtilsMessengerCallbackDataEXT *)&data);
      }
    }
  }
}

#define REDGPU_INTERNAL_DEBUG_SEND_ERROR_MESSAGE(InternalContext, GpuHandle, MessageIdName, StatusCode, Message, OptionalFile, OptionalLine) redInternalDebugSendErrorMessage(InternalContext, GpuHandle, MessageIdName, StatusCode, Message, OptionalFile, OptionalLine);

#endif

#define REDGPU_INTERNAL_FINALIZE_OUTSTATUSES \
  if (outStatuses != NULL) { \
    if ((VkResult)outStatuses->status == VK_SUCCESS) { \
      outStatuses->statusProcedureId = procedureId; \
      outStatuses->statusFile        = optionalFile; \
      outStatuses->statusLine        = optionalLine; \
    } \
    if ((VkResult)outStatuses->statusError == VK_SUCCESS) { \
      outStatuses->statusErrorProcedureId = procedureId; \
      outStatuses->statusErrorFile        = optionalFile; \
      outStatuses->statusErrorLine        = optionalLine; \
    } \
  }

static void redInternalSetStatusErrorDescription(unsigned statusIndex, char * outStatusErrorDescription512) {
  if (statusIndex >=                 381) { return; }
  const char * REDGPUStatusCodesInfo[381] = {
    /*[0] =*/ "",
    /*[1] =*/ "redCallSetProcedureParametersHandles: internal query fail",
    /*[2] =*/ "redCallSetProcedureParametersHandles: malloc returned NULL",
    /*[3] =*/ "redCallSetProcedureParametersHandles: malloc returned NULL",
    /*[4] =*/ "redCallSetProcedureParametersHandles: outStatuses.status != RED_STATUS_SUCCESS",
    /*[5] =*/ "redCallSetProcedureParametersHandles: outStatuses.statusError != RED_STATUS_SUCCESS",
    /*[6] =*/ "redCallSetProcedureParametersHandles: outStatuses.statusError != RED_STATUS_SUCCESS",
    /*[7] =*/ "redCallSetProcedureParametersHandles: outStatuses.status != RED_STATUS_SUCCESS",
    /*[8] =*/ "redCallSetProcedureParametersHandles: outStatuses.statusError != RED_STATUS_SUCCESS",
    /*[9] =*/ "redCallSetProcedureParametersHandles: outStatuses.statusError != RED_STATUS_SUCCESS",
    /*[10] =*/ "redCreateContext: can't find enumerate instance layer properties, enumerate instance extension properties, create instance procedure pointers",
    /*[11] =*/ "redCreateContext: user provided malloc returned NULL",
    /*[12] =*/ "redCreateContext: user provided malloc returned NULL",
    /*[13] =*/ "redCreateContext: required extension get physical device properties 2 is not found",
    /*[14] =*/ "redCreateContext: requested extension surface is not found",
    /*[15] =*/ "redCreateContext: requested extension win32 surface is not found",
    /*[16] =*/ "redCreateContext: requested extension xlib surface is not found",
    /*[17] =*/ "redCreateContext: requested extension xcb surface is not found",
    /*[18] =*/ "redCreateContext: create instance returned outContext->contextHandle == NULL",
    /*[19] =*/ "redCreateContext: RedDriverProceduresAndAddresses::GetDeviceProcAddr == NULL",
    /*[20] =*/ "redCreateContext: RedDriverProceduresAndAddresses::CreateDebugUtilsMessenger == NULL",
    /*[21] =*/ "redCreateContext: RedDriverProceduresAndAddresses::EnumeratePhysicalDevices == NULL",
    /*[22] =*/ "redCreateContext: RedDriverProceduresAndAddresses::EnumerateDeviceExtensionProperties == NULL",
    /*[23] =*/ "redCreateContext: RedDriverProceduresAndAddresses::GetPhysicalDeviceMemoryProperties == NULL",
    /*[24] =*/ "redCreateContext: RedDriverProceduresAndAddresses::GetPhysicalDeviceFeatures == NULL",
    /*[25] =*/ "redCreateContext: RedDriverProceduresAndAddresses::GetPhysicalDeviceProperties == NULL",
    /*[26] =*/ "redCreateContext: RedDriverProceduresAndAddresses::GetPhysicalDeviceQueueFamilyProperties == NULL",
    /*[27] =*/ "redCreateContext: RedDriverProceduresAndAddresses::CreateDevice == NULL",
    /*[28] =*/ "redCreateContext: RedDriverProceduresAndAddresses::GetDeviceQueue == NULL",
    /*[29] =*/ "redCreateContext: RedDriverProceduresAndAddresses::GetPhysicalDeviceFormatProperties == NULL",
    /*[30] =*/ "redCreateContext: RedDriverProceduresAndAddresses::DestroyDevice == NULL",
    /*[31] =*/ "redCreateContext: RedDriverProceduresAndAddresses::DestroyDebugUtilsMessenger == NULL",
    /*[32] =*/ "redCreateContext: RedDriverProceduresAndAddresses::DestroyInstance == NULL",
    /*[33] =*/ "redCreateContext: RedDriverProceduresAndAddresses::SetDebugUtilsObjectName == NULL",
    /*[34] =*/ "redCreateContext: RedDriverProceduresAndAddresses::GetPhysicalDeviceImageFormatProperties == NULL",
    /*[35] =*/ "redCreateContext: RedDriverProceduresAndAddresses::GetBufferMemoryRequirements == NULL",
    /*[36] =*/ "redCreateContext: RedDriverProceduresAndAddresses::GetImageMemoryRequirements == NULL",
    /*[37] =*/ "redCreateContext: RedDriverProceduresAndAddresses::AllocateMemory == NULL",
    /*[38] =*/ "redCreateContext: RedDriverProceduresAndAddresses::FreeMemory == NULL",
    /*[39] =*/ "redCreateContext: RedDriverProceduresAndAddresses::MapMemory == NULL",
    /*[40] =*/ "redCreateContext: RedDriverProceduresAndAddresses::UnmapMemory == NULL",
    /*[41] =*/ "redCreateContext: RedDriverProceduresAndAddresses::BindBufferMemory == NULL",
    /*[42] =*/ "redCreateContext: RedDriverProceduresAndAddresses::BindImageMemory == NULL",
    /*[43] =*/ "redCreateContext: RedDriverProceduresAndAddresses::FlushMappedMemoryRanges == NULL",
    /*[44] =*/ "redCreateContext: RedDriverProceduresAndAddresses::InvalidateMappedMemoryRanges == NULL",
    /*[45] =*/ "redCreateContext: RedDriverProceduresAndAddresses::CreateBuffer == NULL",
    /*[46] =*/ "redCreateContext: RedDriverProceduresAndAddresses::DestroyBuffer == NULL",
    /*[47] =*/ "redCreateContext: RedDriverProceduresAndAddresses::CreateImage == NULL",
    /*[48] =*/ "redCreateContext: RedDriverProceduresAndAddresses::DestroyImage == NULL",
    /*[49] =*/ "redCreateContext: RedDriverProceduresAndAddresses::CreateSampler == NULL",
    /*[50] =*/ "redCreateContext: RedDriverProceduresAndAddresses::DestroySampler == NULL",
    /*[51] =*/ "redCreateContext: RedDriverProceduresAndAddresses::CreateImageView == NULL",
    /*[52] =*/ "redCreateContext: RedDriverProceduresAndAddresses::DestroyImageView == NULL",
    /*[53] =*/ "redCreateContext: RedDriverProceduresAndAddresses::CreateShaderModule == NULL",
    /*[54] =*/ "redCreateContext: RedDriverProceduresAndAddresses::DestroyShaderModule == NULL",
    /*[55] =*/ "redCreateContext: RedDriverProceduresAndAddresses::CreateGraphicsPipelines == NULL",
    /*[56] =*/ "redCreateContext: RedDriverProceduresAndAddresses::CreateComputePipelines == NULL",
    /*[57] =*/ "redCreateContext: RedDriverProceduresAndAddresses::DestroyPipeline == NULL",
    /*[58] =*/ "redCreateContext: RedDriverProceduresAndAddresses::CreatePipelineCache == NULL",
    /*[59] =*/ "redCreateContext: RedDriverProceduresAndAddresses::DestroyPipelineCache == NULL",
    /*[60] =*/ "redCreateContext: RedDriverProceduresAndAddresses::GetPipelineCacheData == NULL",
    /*[61] =*/ "redCreateContext: RedDriverProceduresAndAddresses::MergePipelineCaches == NULL",
    /*[62] =*/ "redCreateContext: RedDriverProceduresAndAddresses::CreatePipelineLayout == NULL",
    /*[63] =*/ "redCreateContext: RedDriverProceduresAndAddresses::DestroyPipelineLayout == NULL",
    /*[64] =*/ "redCreateContext: RedDriverProceduresAndAddresses::CreateDescriptorSetLayout == NULL",
    /*[65] =*/ "redCreateContext: RedDriverProceduresAndAddresses::DestroyDescriptorSetLayout == NULL",
    /*[66] =*/ "redCreateContext: RedDriverProceduresAndAddresses::CreateDescriptorPool == NULL",
    /*[67] =*/ "redCreateContext: RedDriverProceduresAndAddresses::AllocateDescriptorSets == NULL",
    /*[68] =*/ "redCreateContext: RedDriverProceduresAndAddresses::ResetDescriptorPool == NULL",
    /*[69] =*/ "redCreateContext: RedDriverProceduresAndAddresses::DestroyDescriptorPool == NULL",
    /*[70] =*/ "redCreateContext: RedDriverProceduresAndAddresses::UpdateDescriptorSets == NULL",
    /*[71] =*/ "redCreateContext: RedDriverProceduresAndAddresses::CreateRenderPass == NULL",
    /*[72] =*/ "redCreateContext: RedDriverProceduresAndAddresses::DestroyRenderPass == NULL",
    /*[73] =*/ "redCreateContext: RedDriverProceduresAndAddresses::CreateFramebuffer == NULL",
    /*[74] =*/ "redCreateContext: RedDriverProceduresAndAddresses::DestroyFramebuffer == NULL",
    /*[75] =*/ "redCreateContext: RedDriverProceduresAndAddresses::CreateFence == NULL",
    /*[76] =*/ "redCreateContext: RedDriverProceduresAndAddresses::DestroyFence == NULL",
    /*[77] =*/ "redCreateContext: RedDriverProceduresAndAddresses::GetFenceStatus == NULL",
    /*[78] =*/ "redCreateContext: RedDriverProceduresAndAddresses::WaitForFences == NULL",
    /*[79] =*/ "redCreateContext: RedDriverProceduresAndAddresses::ResetFences == NULL",
    /*[80] =*/ "redCreateContext: RedDriverProceduresAndAddresses::CreateSemaphore == NULL",
    /*[81] =*/ "redCreateContext: RedDriverProceduresAndAddresses::DestroySemaphore == NULL",
    /*[82] =*/ "redCreateContext: RedDriverProceduresAndAddresses::CreateEvent == NULL",
    /*[83] =*/ "redCreateContext: RedDriverProceduresAndAddresses::DestroyEvent == NULL",
    /*[84] =*/ "redCreateContext: RedDriverProceduresAndAddresses::GetEventStatus == NULL",
    /*[85] =*/ "redCreateContext: RedDriverProceduresAndAddresses::ResetEvent == NULL",
    /*[86] =*/ "redCreateContext: RedDriverProceduresAndAddresses::CreateCommandPool == NULL",
    /*[87] =*/ "redCreateContext: RedDriverProceduresAndAddresses::AllocateCommandBuffers == NULL",
    /*[88] =*/ "redCreateContext: RedDriverProceduresAndAddresses::DestroyCommandPool == NULL",
    /*[89] =*/ "redCreateContext: RedDriverProceduresAndAddresses::ResetCommandPool == NULL",
    /*[90] =*/ "redCreateContext: RedDriverProceduresAndAddresses::BeginCommandBuffer == NULL",
    /*[91] =*/ "redCreateContext: RedDriverProceduresAndAddresses::EndCommandBuffer == NULL",
    /*[92] =*/ "redCreateContext: RedDriverProceduresAndAddresses::CmdInsertDebugUtilsLabel == NULL",
    /*[93] =*/ "redCreateContext: RedDriverProceduresAndAddresses::CmdBeginDebugUtilsLabel == NULL",
    /*[94] =*/ "redCreateContext: RedDriverProceduresAndAddresses::CmdEndDebugUtilsLabel == NULL",
    /*[95] =*/ "redCreateContext: RedDriverProceduresAndAddresses::QueueSubmit == NULL",
    /*[96] =*/ "redCreateContext: RedDriverProceduresAndAddresses::QueueWaitIdle == NULL",
    /*[97] =*/ "redCreateContext: RedDriverProceduresAndAddresses::CreateWin32Surface == NULL",
    /*[98] =*/ "redCreateContext: RedDriverProceduresAndAddresses::GetPhysicalDeviceWin32PresentationSupport == NULL",
    /*[99] =*/ "redCreateContext: RedDriverProceduresAndAddresses::CreateXcbSurface == NULL",
    /*[100] =*/ "redCreateContext: RedDriverProceduresAndAddresses::GetPhysicalDeviceXcbPresentationSupport == NULL",
    /*[101] =*/ "redCreateContext: RedDriverProceduresAndAddresses::CreateXlibSurface == NULL",
    /*[102] =*/ "redCreateContext: RedDriverProceduresAndAddresses::GetPhysicalDeviceXlibPresentationSupport == NULL",
    /*[103] =*/ "redCreateContext: RedDriverProceduresAndAddresses::DestroySurface == NULL",
    /*[104] =*/ "redCreateContext: RedDriverProceduresAndAddresses::CreateSwapchain == NULL",
    /*[105] =*/ "redCreateContext: RedDriverProceduresAndAddresses::GetSwapchainImages == NULL",
    /*[106] =*/ "redCreateContext: RedDriverProceduresAndAddresses::DestroySwapchain == NULL",
    /*[107] =*/ "redCreateContext: RedDriverProceduresAndAddresses::GetPhysicalDeviceSurfaceSupport == NULL",
    /*[108] =*/ "redCreateContext: RedDriverProceduresAndAddresses::GetPhysicalDeviceSurfaceFormats == NULL",
    /*[109] =*/ "redCreateContext: RedDriverProceduresAndAddresses::GetPhysicalDeviceSurfaceCapabilities == NULL",
    /*[110] =*/ "redCreateContext: RedDriverProceduresAndAddresses::GetPhysicalDeviceSurfacePresentModes == NULL",
    /*[111] =*/ "redCreateContext: RedDriverProceduresAndAddresses::AcquireNextImage == NULL",
    /*[112] =*/ "redCreateContext: RedDriverProceduresAndAddresses::QueuePresent == NULL",
    /*[113] =*/ "redCreateContext: RedDriverProceduresAndAddresses::CreateRenderPass2 == NULL",
    /*[114] =*/ "redCreateContext: RedDriverProceduresAndAddresses::SubmitDebugUtilsMessage == NULL",
    /*[115] =*/ "redCreateContext: RedDriverProceduresAndAddresses::BindBufferMemory2 == NULL",
    /*[116] =*/ "redCreateContext: RedDriverProceduresAndAddresses::BindImageMemory2 == NULL",
    /*[117] =*/ "redCreateContext: RedDriverProceduresAndAddresses::SetEvent == NULL",
    /*[118] =*/ "redCreateContext: RedDriverProceduresAndAddresses::CmdWaitEvents == NULL",
    /*[119] =*/ "redCreateContext: RedDriverProceduresAndAddresses::CmdResetEvent == NULL",
    /*[120] =*/ "redCreateContext: user provided malloc returned NULL",
    /*[121] =*/ "redCreateContext: user provided malloc returned NULL",
    /*[122] =*/ "redCreateContext: user provided malloc returned NULL",
    /*[123] =*/ "redCreateContext: user provided malloc returned NULL",
    /*[124] =*/ "redCreateContext: user provided malloc returned NULL",
    /*[125] =*/ "redCreateContext: user provided malloc returned NULL",
    /*[126] =*/ "redCreateContext: user provided malloc returned NULL",
    /*[127] =*/ "redCreateContext: user provided malloc returned NULL",
    /*[128] =*/ "redCreateContext: user provided malloc returned NULL",
    /*[129] =*/ "redCreateContext: user provided malloc returned NULL",
    /*[130] =*/ "redCreateContext: user provided malloc returned NULL",
    /*[131] =*/ "redCreateContext: user provided malloc returned NULL",
    /*[132] =*/ "redCreateContext: user provided malloc returned NULL",
    /*[133] =*/ "redCreateContext: user provided malloc returned NULL",
    /*[134] =*/ "redCreateContext: user provided malloc returned NULL",
    /*[135] =*/ "redCreateContext: user provided malloc returned NULL",
    /*[136] =*/ "redCreateContext: user provided malloc returned NULL",
    /*[137] =*/ "redCreateContext: user provided malloc returned NULL",
    /*[138] =*/ "redCreateContext: user provided malloc returned NULL",
    /*[139] =*/ "redCreateContext: user provided malloc returned NULL",
    /*[140] =*/ "redCreateContext: user provided malloc returned NULL",
    /*[141] =*/ "redCreateContext: user provided malloc returned NULL",
    /*[142] =*/ "redCreateContext: user provided malloc returned NULL",
    /*[143] =*/ "redCreateContext: user provided malloc returned NULL",
    /*[144] =*/ "redCreateContext: user provided malloc returned NULL",
    /*[145] =*/ "redCreateContext: RedCallProceduresAndAddresses::redCallGpuToCpuSignalSignal == NULL",
    /*[146] =*/ "redCreateContext: RedCallProceduresAndAddresses::redCallCopyArrayToArray == NULL",
    /*[147] =*/ "redCreateContext: RedCallProceduresAndAddresses::redCallCopyImageToImage == NULL",
    /*[148] =*/ "redCreateContext: RedCallProceduresAndAddresses::redCallCopyArrayToImage == NULL",
    /*[149] =*/ "redCreateContext: RedCallProceduresAndAddresses::redCallCopyImageToArray == NULL",
    /*[150] =*/ "redCreateContext: RedCallProceduresAndAddresses::redCallProcedure == NULL",
    /*[151] =*/ "redCreateContext: RedCallProceduresAndAddresses::redCallProcedureIndexed == NULL",
    /*[152] =*/ "redCreateContext: RedCallProceduresAndAddresses::redCallProcedureCompute == NULL",
    /*[153] =*/ "redCreateContext: RedCallProceduresAndAddresses::redCallSetProcedure == NULL",
    /*[154] =*/ "redCreateContext: RedCallProceduresAndAddresses::redCallSetProcedureIndices == NULL",
    /*[155] =*/ "redCreateContext: RedCallProceduresAndAddresses::redCallSetProcedureParametersStructs == NULL",
    /*[156] =*/ "redCreateContext: RedCallProceduresAndAddresses::redCallSetProcedureParametersVariables == NULL",
    /*[157] =*/ "redCreateContext: RedCallProceduresAndAddresses::redCallSetProcedureParametersStructs == NULL",
    /*[158] =*/ "redCreateContext: RedCallProceduresAndAddresses::redCallSetProcedureParametersHandles == NULL",
    /*[159] =*/ "redCreateContext: RedCallProceduresAndAddresses::redCallSetProcedureParametersVariables == NULL",
    /*[160] =*/ "redCreateContext: RedCallProceduresAndAddresses::redCallSetDynamicDepthBias == NULL",
    /*[161] =*/ "redCreateContext: RedCallProceduresAndAddresses::redCallSetDynamicDepthBounds == NULL",
    /*[162] =*/ "redCreateContext: RedCallProceduresAndAddresses::redCallSetDynamicStencilCompareMask == NULL",
    /*[163] =*/ "redCreateContext: RedCallProceduresAndAddresses::redCallSetDynamicStencilWriteMask == NULL",
    /*[164] =*/ "redCreateContext: RedCallProceduresAndAddresses::redCallSetDynamicStencilReference == NULL",
    /*[165] =*/ "redCreateContext: RedCallProceduresAndAddresses::redCallSetDynamicBlendConstants == NULL",
    /*[166] =*/ "redCreateContext: RedCallProceduresAndAddresses::redCallSetDynamicViewport == NULL",
    /*[167] =*/ "redCreateContext: RedCallProceduresAndAddresses::redCallSetDynamicScissor == NULL",
    /*[168] =*/ "redCreateContext: RedCallProceduresAndAddresses::redCallSetProcedureOutput == NULL",
    /*[169] =*/ "redCreateContext: RedCallProceduresAndAddresses::redCallEndProcedureOutput == NULL",
    /*[170] =*/ "redCreateContext: RedCallProceduresAndAddresses::redCallUsageOrderBarrier == NULL",
    /*[171] =*/ "redCreateContext: user provided malloc returned NULL",
    /*[172] =*/ "redCreateContext: user provided malloc returned NULL",
    /*[173] =*/ "redCreateContext: user provided malloc returned NULL",
    /*[174] =*/ "redCreateContext: user provided malloc returned NULL",
    /*[175] =*/ "redCreateContext: user provided malloc returned NULL",
    /*[176] =*/ "redCreateContext: user provided malloc returned NULL",
    /*[177] =*/ "redCreateContext: user provided malloc returned NULL",
    /*[178] =*/ "redCreateContext: user provided malloc returned NULL",
    /*[179] =*/ "redCreateContext: user provided malloc returned NULL",
    /*[180] =*/ "redCreateContext: user provided malloc returned NULL",
    /*[181] =*/ "redCreateContext: user provided malloc returned NULL",
    /*[182] =*/ "redCreateContext: user provided malloc returned NULL",
    /*[183] =*/ "redCreateContext: user provided malloc returned NULL",
    /*[184] =*/ "redCreateContext: user provided malloc returned NULL",
    /*[185] =*/ "redCreateContext: user provided malloc returned NULL",
    /*[186] =*/ "redCreateContext: user provided malloc returned NULL",
    /*[187] =*/ "redCallsEnd: internal query fail",
    /*[188] =*/ "redCallSetProcedureOutput: internal query fail",
    /*[189] =*/ "redCallSetProcedureOutput: user provided malloc returned NULL",
    /*[190] =*/ "redCreateArray: create buffer status",
    /*[191] =*/ "redCreateArray: set handle name status",
    /*[192] =*/ "redMemoryAllocate: allocate memory status",
    /*[193] =*/ "redMemoryAllocate: map memory status",
    /*[194] =*/ "redMemoryAllocate: set handle name status",
    /*[195] =*/ "redMemorySet: bind buffer memory 2 status",
    /*[196] =*/ "redMemorySet: bind image memory 2 status",
    /*[197] =*/ "redMemorySet: bind buffer memory status",
    /*[198] =*/ "redMemorySet: bind image memory status",
    /*[199] =*/ "redCreateGpuToCpuSignal: create event status",
    /*[200] =*/ "redCreateGpuToCpuSignal: set handle name status",
    /*[201] =*/ "redCreateCpuSignal: create fence status",
    /*[202] =*/ "redCreateCpuSignal: set handle name status",
    /*[203] =*/ "redCpuSignalUnsignal: reset fences status",
    /*[204] =*/ "redMemoryGetBudget: user provided gpu handle is unknown",
    /*[205] =*/ "redMemoryGetBudget: gpu.supportsMemoryGetBudget == 0",
    /*[206] =*/ "redMemoryAllocate: user provided gpu handle is unknown",
    /*[207] =*/ "redMemoryAllocate: user provided malloc returned NULL",
    /*[208] =*/ "redMemoryAllocate: user provided malloc returned NULL",
    /*[209] =*/ "redMemoryAllocate: allocate memory status",
    /*[210] =*/ "",
    /*[211] =*/ "redMemoryAllocate: set handle name status",
    /*[212] =*/ "redMemorySet: user provided gpu handle is unknown",
    /*[213] =*/ "redMemorySet: bind buffer memory status",
    /*[214] =*/ "redMemorySet: bind image memory status",
    /*[215] =*/ "redMemorySet: bind buffer memory 2 status",
    /*[216] =*/ "redMemorySet: bind image memory 2 status",
    /*[217] =*/ "redMemorySet: bind buffer memory status",
    /*[218] =*/ "redMemorySet: bind image memory status",
    /*[219] =*/ "redMemoryMap: map memory status",
    /*[220] =*/ "redMemoryNonCoherentFlush: flush mapped memory ranges status",
    /*[221] =*/ "redMemoryNonCoherentFlush: flush mapped memory ranges status",
    /*[222] =*/ "redMemoryNonCoherentFlush: flush mapped memory ranges status",
    /*[223] =*/ "redMemoryNonCoherentInvalidate: invalidate mapped memory ranges status",
    /*[224] =*/ "redMemoryNonCoherentInvalidate: invalidate mapped memory ranges status",
    /*[225] =*/ "redMemoryNonCoherentInvalidate: invalidate mapped memory ranges status",
    /*[226] =*/ "redStructsMemoryAllocate: create descriptor pool status",
    /*[227] =*/ "redStructsMemoryAllocate: set handle name status",
    /*[228] =*/ "redStructsMemorySuballocateStructs: allocate descriptor sets status",
    /*[229] =*/ "redStructsMemorySuballocateStructs: set handle name status",
    /*[230] =*/ "redStructsMemoryReset: reset descriptor pool status",
    /*[231] =*/ "redStructsMemoryAllocate: create descriptor pool status",
    /*[232] =*/ "redStructsMemoryAllocate: set handle name status",
    /*[233] =*/ "redStructsMemorySuballocateStructs: allocate descriptor sets status",
    /*[234] =*/ "redStructsMemorySuballocateStructs: set handle name status",
    /*[235] =*/ "redCreateContext: user provided malloc returned NULL",
    /*[236] =*/ "redCreateContext: user provided malloc returned NULL",
    /*[237] =*/ "redCreateContext: user provided malloc returned NULL",
    /*[238] =*/ "redCreateContext: enumerate instance layer properties status",
    /*[239] =*/ "redCreateContext: enumerate instance layer properties status",
    /*[240] =*/ "redCreateContext: requested layer lunarg standard validation is not found",
    /*[241] =*/ "redCreateContext: requested layer khronos validation is not found",
    /*[242] =*/ "redCreateContext: enumerate instance extension properties status",
    /*[243] =*/ "redCreateContext: enumerate instance extension properties status",
    /*[244] =*/ "redCreateContext: requested extension debug utils is not found",
    /*[245] =*/ "redCreateContext: create instance status",
    /*[246] =*/ "redCreateContext: create debug utils messenger status",
    /*[247] =*/ "redCreateContext: enumerate physical devices status",
    /*[248] =*/ "redCreateContext: enumerate physical devices status",
    /*[249] =*/ "redCreateContext: enumerate device extension properties status",
    /*[250] =*/ "redCreateContext: enumerate device extension properties status",
    /*[251] =*/ "redCreateContext: required extension maintenance 1 is not found for a gpu",
    /*[252] =*/ "redCreateContext: required extension maintenance 2 is not found for a gpu",
    /*[253] =*/ "redCreateContext: required extension maintenance 3 is not found for a gpu",
    /*[254] =*/ "redCreateContext: create device status",
    /*[255] =*/ "redCreateContext: create debug array status",
    /*[256] =*/ "redCreateContext: create debug array status",
    /*[257] =*/ "redCreateContext: create debug array status",
    /*[258] =*/ "redCreateContext: create debug array status",
    /*[259] =*/ "redCreateArray: user provided gpu handle is unknown",
    /*[260] =*/ "redCreateArray: create buffer status",
    /*[261] =*/ "redCreateArray: set handle name status",
    /*[262] =*/ "redCreateImage: user provided gpu handle is unknown",
    /*[263] =*/ "redCreateImage: create image status",
    /*[264] =*/ "redCreateImage: set handle name status",
    /*[265] =*/ "redCreateSampler: create sampler status",
    /*[266] =*/ "redCreateSampler: set handle name status",
    /*[267] =*/ "redCreateTexture: create image view status",
    /*[268] =*/ "redCreateTexture: set handle name status",
    /*[269] =*/ "redCreateGpuCode: create shader module status",
    /*[270] =*/ "redCreateGpuCode: set handle name status",
    /*[271] =*/ "redCreateOutputDeclaration: user provided gpu handle is unknown",
    /*[272] =*/ "redCreateOutputDeclaration: create render pass 2 status",
    /*[273] =*/ "redCreateOutputDeclaration: create render pass status",
    /*[274] =*/ "redCreateOutputDeclaration: set handle name status",
    /*[275] =*/ "redCreateStructDeclaration: user provided gpu handle is unknown",
    /*[276] =*/ "redCreateStructDeclaration: for a gpu with RedGpuInfoOptionalInfoProcedureParametersHandles::supportsProcedureParametersHandles == 0 and RedGpuInfoOptionalInfoProcedureParametersHandles::maxProcedureParametersHandlesCount == 8, requested a struct declaration with procedureParametersHandlesDeclaration == 1 and structDeclarationMembersCount > 8",
    /*[277] =*/ "redCreateStructDeclaration: create descriptor set layout status",
    /*[278] =*/ "redCreateStructDeclaration: set handle name status",
    /*[279] =*/ "redCreateStructDeclaration: user provided malloc returned NULL",
    /*[280] =*/ "redCreateProcedureParameters: user provided gpu handle is unknown",
    /*[281] =*/ "redCreateProcedureParameters: the count of all structs declarations is > 8",
    /*[282] =*/ "redCreateProcedureParameters: create pipeline layout status",
    /*[283] =*/ "redCreateProcedureParameters: set handle name status",
    /*[284] =*/ "redCreateProcedureParameters: user provided malloc returned NULL",
    /*[285] =*/ "redCreateProcedureParameters: internal query fail",
    /*[286] =*/ "redCreateProcedureCache: create pipeline cache status",
    /*[287] =*/ "redCreateProcedureCache: set handle name status",
    /*[288] =*/ "redCreateProcedure: user provided gpu handle is unknown",
    /*[289] =*/ "redCreateProcedure: create graphics pipelines status",
    /*[290] =*/ "redCreateProcedure: set handle name status",
    /*[291] =*/ "redCreateProcedureCompute: create compute pipelines status",
    /*[292] =*/ "redCreateProcedureCompute: set handle name status",
    /*[293] =*/ "redCreateOutput: user provided gpu handle is unknown",
    /*[294] =*/ "redCreateOutput: create framebuffer status",
    /*[295] =*/ "redCreateOutput: set handle name status",
    /*[296] =*/ "redCreateCpuSignal: create fence status",
    /*[297] =*/ "redCreateCpuSignal: set handle name status",
    /*[298] =*/ "redCreateGpuSignal: create semaphore status",
    /*[299] =*/ "redCreateGpuSignal: set handle name status",
    /*[300] =*/ "redCreateGpuToCpuSignal: create event status",
    /*[301] =*/ "redCreateGpuToCpuSignal: set handle name status",
    /*[302] =*/ "redCreateCalls: user provided gpu handle is unknown",
    /*[303] =*/ "redCreateCalls: create command pool status",
    /*[304] =*/ "redCreateCalls: allocate command buffers status",
    /*[305] =*/ "redCreateCalls: set handle name status",
    /*[306] =*/ "redCreateCalls: set handle name status",
    /*[307] =*/ "redCreateCalls: user provided malloc returned NULL",
    /*[308] =*/ "redCreateCallsReusable: user provided gpu handle is unknown",
    /*[309] =*/ "redCreateCallsReusable: create command pool status",
    /*[310] =*/ "redCreateCallsReusable: allocate command buffers status",
    /*[311] =*/ "redCreateCallsReusable: set handle name status",
    /*[312] =*/ "redCreateCallsReusable: set handle name status",
    /*[313] =*/ "redCreateCallsReusable: user provided malloc returned NULL",
    /*[314] =*/ "redDestroyCalls: user provided gpu handle is unknown",
    /*[315] =*/ "redProcedureCacheGetBlob: get pipeline cache data status",
    /*[316] =*/ "redProcedureCacheMergeCaches: merge pipeline caches status",
    /*[317] =*/ "redCpuSignalWait: wait for fences status",
    /*[318] =*/ "redCpuSignalUnsignal: reset fences status",
    /*[319] =*/ "redGpuToCpuSignalUnsignal: reset event status",
    /*[320] =*/ "redCallsSet: reset command pool status",
    /*[321] =*/ "redCallsSet: begin command buffer status",
    /*[322] =*/ "redCallsEnd: user provided gpu handle is unknown",
    /*[323] =*/ "redCallsEnd: end command buffer status",
    /*[324] =*/ "redGetCallProceduresAndAddresses: user provided gpu handle is unknown",
    /*[325] =*/ "redCallSetProcedureOutput: user provided malloc returned NULL",
    /*[326] =*/ "redQueueSubmit: user provided gpu handle is unknown",
    /*[327] =*/ "redQueueSubmit: queue submit status",
    /*[328] =*/ "redQueueSubmit: queue submit status",
    /*[329] =*/ "redDebugArrayGetHandle: user provided gpu handle is unknown",
    /*[330] =*/ "redDebugArrayGetHandle: gpu.debugArrayFeatureIsSupported == 0",
    /*[331] =*/ "redDebugArrayCallPrint: user provided gpu handle is unknown",
    /*[332] =*/ "redDebugArrayCallPrint: gpu.debugArrayFeatureIsSupported == 0",
    /*[333] =*/ "redDebugArrayCallPrint: create xcb surface status",
    /*[334] =*/ "redDebugArrayCallPrint: create xlib surface status",
    /*[335] =*/ "redDebugArrayCallPrint: set handle name status",
    /*[336] =*/ "redCreatePresent: user provided gpu handle is unknown",
    /*[337] =*/ "redCreatePresent: create swapchain status",
    /*[338] =*/ "redCreatePresent: get swapchain images status",
    /*[339] =*/ "redCreatePresent: get swapchain images count != redCreatePresent::imagesCount",
    /*[340] =*/ "redCreatePresent: get swapchain images status",
    /*[341] =*/ "redCreatePresent: create image view status",
    /*[342] =*/ "redCreatePresent: set handle name status",
    /*[343] =*/ "redCreatePresent: set handle name status",
    /*[344] =*/ "redQueueFamilyIndexGetSupportsPresent: user provided gpu handle is unknown",
    /*[345] =*/ "redQueueFamilyIndexGetSupportsPresent: get physical device surface support status",
    /*[346] =*/ "redSurfaceGetPresentFeatures: user provided gpu handle is unknown",
    /*[347] =*/ "redSurfaceGetPresentFeatures: get physical device surface formats status",
    /*[348] =*/ "redSurfaceGetPresentFeatures: get physical device surface formats status",
    /*[349] =*/ "redSurfaceGetPresentFeatures: get physical device surface present modes status",
    /*[350] =*/ "redSurfaceGetPresentFeatures: get physical device surface present modes status",
    /*[351] =*/ "redSurfaceGetCurrentPropertiesAndPresentLimits: user provided gpu handle is unknown",
    /*[352] =*/ "redSurfaceGetCurrentPropertiesAndPresentLimits: get physical device surface capabilities status",
    /*[353] =*/ "redPresentGetImageIndex: acquire next image status",
    /*[354] =*/ "redQueuePresent: queue present status",
    /*[355] =*/ "redDebugArrayGetHandle: redDebugArrayGetHandle is not available, link against redgpu_debug library to use redDebugArrayGetHandle",
    /*[356] =*/ "redDebugArrayCallPrint: redDebugArrayCallPrint is not available, link against redgpu_debug library to use redDebugArrayCallPrint",
    /*[357] =*/ "redCreateSurfaceWin32: create win32 surface status",
    /*[358] =*/ "redCreateSurfaceWin32: set handle name status",
    /*[359] =*/ "redCreateSurfaceWin32: procedure is not available",
    /*[360] =*/ "redCreatePresent: user provided gpu handle is unknown",
    /*[361] =*/ "redCreatePresent: user provided queue handle is unknown",
    /*[362] =*/ "redCreateSurfaceXlibOrXcb: procedure is not available",
    /*[363] =*/ "redCreateContext: unsupported GPU driver with the max count of all structs declarations < 8",
    /*[364] =*/ "redCreateContext: create debug array get memory type index status",
    /*[365] =*/ "redQueueSubmit: debug array get fence status",
    /*[366] =*/ "redQueueSubmit: debug array get fence status",
    /*[367] =*/ "redQueueSubmit: debug array cpu signal unsignal status",
    /*[368] =*/ "redCreateContext: user provided malloc returned NULL",
    /*[369] =*/ "redCreateContext: user provided malloc returned NULL",
    /*[370] =*/ "redCreateContext: user provided malloc returned NULL",
    /*[371] =*/ "redCreateContext: RedDriverProceduresAndAddresses::CreateQueryPool == NULL",
    /*[372] =*/ "redCreateContext: RedDriverProceduresAndAddresses::DestroyQueryPool == NULL",
    /*[373] =*/ "redCreateContext: RedDriverProceduresAndAddresses::CmdResetQueryPool == NULL",
    /*[374] =*/ "redCreateContext: RedDriverProceduresAndAddresses::CmdWriteTimestamp == NULL",
    /*[375] =*/ "redCreateContext: RedDriverProceduresAndAddresses::GetQueryPoolResults == NULL",
    /*[376] =*/ "redCreateArrayTimestamp: create query pool status",
    /*[377] =*/ "redCreateArrayTimestamp: set handle name status",
    /*[378] =*/ "redArrayTimestampRead: get query pool results status",
    /*[379] =*/ "redCreateContext: LoadLibraryA(\"C:\\Windows\\System32\\vulkan-1.dll\") returned NULL",
    /*[380] =*/ "redCreateContext: GetProcAddress(vulkan_1_dll, \"vkGetInstanceProcAddr\") returned NULL",
  };
  outStatusErrorDescription512[511] = 0;
  for (unsigned i = 0; i < 511; i += 1) {
    char c = REDGPUStatusCodesInfo[statusIndex][i];
    outStatusErrorDescription512[i] = c;
    if (c == 0) {
      break;
    }
  }
}

static inline void redInternalSetStatus(RedInternalContext ctx, RedHandleGpu gpu, RedStatuses * outStatuses, unsigned statusIndex, VkResult status, RedProcedureId procedureId, const char * optionalFile, int optionalLine, void * optionalUserData) {
  if (status == VK_SUCCESS) {
    return;
  }

#if REDGPU_COMPILE_SWITCH == REDGPU_COMPILE_SWITCH_DEBUG
  if (procedureId == RED_PROCEDURE_ID_redCreateContext) {
    std::lock_guard<std::mutex> __debugPrintGuardAndSerializeMutexScopeGuard(__REDGPU_DEBUG_GLOBAL_3f5f84a81695ef9521b544bdc3d7d73cbb4aac3c18ce99fd19bdfd306996103e_debugPrintGuardAndSerializeMutex);
    if (optionalFile != NULL) {
      REDGPU_PRINTF("[REDGPU DEBUG LIBRARY][RED_PROCEDURE_ID_redCreateContext][%s:%d]: %s (statusCode: %d, context: %p, gpu: %p)\n", optionalFile, optionalLine, REDGPU_PRINT_STATUS((RedStatus)status), statusIndex, REDGPU_PRINT_POINTER(ctx), REDGPU_PRINT_POINTER(gpu));
    } else {
      REDGPU_PRINTF("[REDGPU DEBUG LIBRARY][RED_PROCEDURE_ID_redCreateContext]: %s (statusCode: %d, context: %p, gpu: %p)\n", REDGPU_PRINT_STATUS((RedStatus)status), statusIndex, REDGPU_PRINT_POINTER(ctx), REDGPU_PRINT_POINTER(gpu));
    }
  }
#endif

  if (outStatuses == NULL) {

#if REDGPU_COMPILE_SWITCH == REDGPU_COMPILE_SWITCH_DEBUG
    if (status < 0) {
      if (procedureId != RED_PROCEDURE_ID_redCreateContext) {
        REDGPU_INTERNAL_DEBUG_SEND_ERROR_MESSAGE(ctx, gpu, REDGPU_PRINT_PROCEDURE_ID(procedureId), statusIndex, REDGPU_PRINT_STATUS((RedStatus)status), optionalFile, optionalLine)
      }
    }
#endif

    return;
  }

  if (status < 0) {

#if REDGPU_COMPILE_SWITCH == REDGPU_COMPILE_SWITCH_DEBUG
    if (procedureId != RED_PROCEDURE_ID_redCreateContext) {
#endif

    REDGPU_INTERNAL_DEBUG_SEND_ERROR_MESSAGE(ctx, gpu, REDGPU_PRINT_PROCEDURE_ID(procedureId), statusIndex, REDGPU_PRINT_STATUS((RedStatus)status), optionalFile, optionalLine)

#if REDGPU_COMPILE_SWITCH == REDGPU_COMPILE_SWITCH_DEBUG
    }
#endif

  }

  if (status > 0) {
    if ((VkResult)outStatuses->status == VK_SUCCESS) {
      outStatuses->status            = (RedStatus)status;
      outStatuses->statusCode        = statusIndex;
      outStatuses->statusHresult     = 0;
      outStatuses->statusProcedureId = procedureId;
      outStatuses->statusFile        = optionalFile;
      outStatuses->statusLine        = optionalLine;
    }
  } else {
    if ((VkResult)outStatuses->statusError == VK_SUCCESS) {
      outStatuses->statusError            = (RedStatus)status;
      outStatuses->statusErrorCode        = statusIndex;
      outStatuses->statusErrorHresult     = 0;
      outStatuses->statusErrorProcedureId = procedureId;
      outStatuses->statusErrorFile        = optionalFile;
      outStatuses->statusErrorLine        = optionalLine;
      redInternalSetStatusErrorDescription(statusIndex, outStatuses->statusErrorDescription);
      redInternalMarkStatus((RedStatus)status);
    }
  }
}

static inline RedBool32 redInternalContextExtensionIsEnabled(RedInternalContext ctx, RedInternalContextExtensionBitflag contextExtension) {
  return ((ctx->internal.contextExtensionBitflags & contextExtension) == contextExtension) ? 1 : 0;
}

static inline RedBool32 redInternalGpuExtensionIsEnabled(RedInternalContext ctx, unsigned gpuIndex, RedInternalGpuExtensionBitflag gpuExtension) {
  return ((ctx->internal.gpuExtensionBitflags[gpuIndex] & gpuExtension) == gpuExtension) ? 1 : 0;
}

// Debug internal

#if REDGPU_COMPILE_SWITCH == REDGPU_COMPILE_SWITCH_DEBUG
static inline unsigned redDebugInternalHelperGetMemoryTypeIndex(RedInternalContext ctx, unsigned gpuIndex, RedDebugInternalMemoryType memoryType, unsigned memoryTypesSupported) {
  const unsigned        memoryTypesCount = ctx->gpus[gpuIndex].memoryTypesCount;
  const RedMemoryType * memoryTypes      = ctx->gpus[gpuIndex].memoryTypes;

  unsigned memoryTypeIsSupported[32];
  memoryTypeIsSupported[31] = (memoryTypesSupported & REDGPU_B32(1000,0000,0000,0000,0000,0000,0000,0000)) == 0 ? 0 : 1;
  memoryTypeIsSupported[30] = (memoryTypesSupported & REDGPU_B32(0100,0000,0000,0000,0000,0000,0000,0000)) == 0 ? 0 : 1;
  memoryTypeIsSupported[29] = (memoryTypesSupported & REDGPU_B32(0010,0000,0000,0000,0000,0000,0000,0000)) == 0 ? 0 : 1;
  memoryTypeIsSupported[28] = (memoryTypesSupported & REDGPU_B32(0001,0000,0000,0000,0000,0000,0000,0000)) == 0 ? 0 : 1;
  memoryTypeIsSupported[27] = (memoryTypesSupported & REDGPU_B32(0000,1000,0000,0000,0000,0000,0000,0000)) == 0 ? 0 : 1;
  memoryTypeIsSupported[26] = (memoryTypesSupported & REDGPU_B32(0000,0100,0000,0000,0000,0000,0000,0000)) == 0 ? 0 : 1;
  memoryTypeIsSupported[25] = (memoryTypesSupported & REDGPU_B32(0000,0010,0000,0000,0000,0000,0000,0000)) == 0 ? 0 : 1;
  memoryTypeIsSupported[24] = (memoryTypesSupported & REDGPU_B32(0000,0001,0000,0000,0000,0000,0000,0000)) == 0 ? 0 : 1;
  memoryTypeIsSupported[23] = (memoryTypesSupported & REDGPU_B32(0000,0000,1000,0000,0000,0000,0000,0000)) == 0 ? 0 : 1;
  memoryTypeIsSupported[22] = (memoryTypesSupported & REDGPU_B32(0000,0000,0100,0000,0000,0000,0000,0000)) == 0 ? 0 : 1;
  memoryTypeIsSupported[21] = (memoryTypesSupported & REDGPU_B32(0000,0000,0010,0000,0000,0000,0000,0000)) == 0 ? 0 : 1;
  memoryTypeIsSupported[20] = (memoryTypesSupported & REDGPU_B32(0000,0000,0001,0000,0000,0000,0000,0000)) == 0 ? 0 : 1;
  memoryTypeIsSupported[19] = (memoryTypesSupported & REDGPU_B32(0000,0000,0000,1000,0000,0000,0000,0000)) == 0 ? 0 : 1;
  memoryTypeIsSupported[18] = (memoryTypesSupported & REDGPU_B32(0000,0000,0000,0100,0000,0000,0000,0000)) == 0 ? 0 : 1;
  memoryTypeIsSupported[17] = (memoryTypesSupported & REDGPU_B32(0000,0000,0000,0010,0000,0000,0000,0000)) == 0 ? 0 : 1;
  memoryTypeIsSupported[16] = (memoryTypesSupported & REDGPU_B32(0000,0000,0000,0001,0000,0000,0000,0000)) == 0 ? 0 : 1;
  memoryTypeIsSupported[15] = (memoryTypesSupported & REDGPU_B32(0000,0000,0000,0000,1000,0000,0000,0000)) == 0 ? 0 : 1;
  memoryTypeIsSupported[14] = (memoryTypesSupported & REDGPU_B32(0000,0000,0000,0000,0100,0000,0000,0000)) == 0 ? 0 : 1;
  memoryTypeIsSupported[13] = (memoryTypesSupported & REDGPU_B32(0000,0000,0000,0000,0010,0000,0000,0000)) == 0 ? 0 : 1;
  memoryTypeIsSupported[12] = (memoryTypesSupported & REDGPU_B32(0000,0000,0000,0000,0001,0000,0000,0000)) == 0 ? 0 : 1;
  memoryTypeIsSupported[11] = (memoryTypesSupported & REDGPU_B32(0000,0000,0000,0000,0000,1000,0000,0000)) == 0 ? 0 : 1;
  memoryTypeIsSupported[10] = (memoryTypesSupported & REDGPU_B32(0000,0000,0000,0000,0000,0100,0000,0000)) == 0 ? 0 : 1;
  memoryTypeIsSupported[9]  = (memoryTypesSupported & REDGPU_B32(0000,0000,0000,0000,0000,0010,0000,0000)) == 0 ? 0 : 1;
  memoryTypeIsSupported[8]  = (memoryTypesSupported & REDGPU_B32(0000,0000,0000,0000,0000,0001,0000,0000)) == 0 ? 0 : 1;
  memoryTypeIsSupported[7]  = (memoryTypesSupported & REDGPU_B32(0000,0000,0000,0000,0000,0000,1000,0000)) == 0 ? 0 : 1;
  memoryTypeIsSupported[6]  = (memoryTypesSupported & REDGPU_B32(0000,0000,0000,0000,0000,0000,0100,0000)) == 0 ? 0 : 1;
  memoryTypeIsSupported[5]  = (memoryTypesSupported & REDGPU_B32(0000,0000,0000,0000,0000,0000,0010,0000)) == 0 ? 0 : 1;
  memoryTypeIsSupported[4]  = (memoryTypesSupported & REDGPU_B32(0000,0000,0000,0000,0000,0000,0001,0000)) == 0 ? 0 : 1;
  memoryTypeIsSupported[3]  = (memoryTypesSupported & REDGPU_B32(0000,0000,0000,0000,0000,0000,0000,1000)) == 0 ? 0 : 1;
  memoryTypeIsSupported[2]  = (memoryTypesSupported & REDGPU_B32(0000,0000,0000,0000,0000,0000,0000,0100)) == 0 ? 0 : 1;
  memoryTypeIsSupported[1]  = (memoryTypesSupported & REDGPU_B32(0000,0000,0000,0000,0000,0000,0000,0010)) == 0 ? 0 : 1;
  memoryTypeIsSupported[0]  = (memoryTypesSupported & REDGPU_B32(0000,0000,0000,0000,0000,0000,0000,0001)) == 0 ? 0 : 1;

  if (memoryType == RED_DEBUG_INTERNAL_MEMORY_TYPE_VRAM) {
    for (unsigned i = 0; i < memoryTypesCount; i += 1) {
      const RedMemoryType * type = &memoryTypes[i];
      if (type->isGpuVram == 1 && memoryTypeIsSupported[i] == 1) {
        return i;
      }
    }
  } else if (memoryType == RED_DEBUG_INTERNAL_MEMORY_TYPE_UPLOAD) {
    for (unsigned i = 0; i < memoryTypesCount; i += 1) {
      const RedMemoryType * type = &memoryTypes[i];
      if (type->isCpuMappable == 1 &&
          type->isCpuCoherent == 1 &&
          type->isCpuCached   == 0 && memoryTypeIsSupported[i] == 1)
      {
        return i;
      }
    }
    for (unsigned i = 0; i < memoryTypesCount; i += 1) {
      const RedMemoryType * type = &memoryTypes[i];
      if (type->isCpuMappable == 1 &&
          type->isCpuCoherent == 1 && memoryTypeIsSupported[i] == 1)
      {
        return i;
      }
    }
  } else if (memoryType == RED_DEBUG_INTERNAL_MEMORY_TYPE_READBACK) {
    for (unsigned i = 0; i < memoryTypesCount; i += 1) {
      const RedMemoryType * type = &memoryTypes[i];
      if (type->isCpuMappable == 1 &&
          type->isCpuCoherent == 1 &&
          type->isCpuCached   == 1 && memoryTypeIsSupported[i] == 1)
      {
        return i;
      }
    }
  }

  return -1;
}

static inline void redDebugInternalCreateArray(RedInternalContext ctx, RedHandleGpu gpu, unsigned gpuIndex, const char * handleName, RedArrayType type, uint64_t bytesCount, unsigned initialQueueFamilyIndex, RedBool32 dedicate, RedArray * outArray, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData) {
  const RedProcedureId procedureId = RED_PROCEDURE_ID_redCreateArray;

  if (type == RED_ARRAY_TYPE_ARRAY_RO) {
    type = RED_ARRAY_TYPE_ARRAY_RW;
  }

  RedInternalInstanceProcedureAddresses * vk = ctx->internal.instanceProcedureAddresses;

  if (redInternalGpuExtensionIsEnabled(ctx, gpuIndex, RED_INTERNAL_GPU_EXTENSION_BITFLAG_DEDICATE_MEMORY_NV))
  {
  } else {
    dedicate = 0;
  }

  struct VkDedicatedAllocationBufferCreateInfoNV {
    unsigned sType;
    void *   pNext;
    VkBool32 dedicatedAllocation;
  } dedicateInfo = {
    1000026001, // VK_STRUCTURE_TYPE_DEDICATED_ALLOCATION_BUFFER_CREATE_INFO_NV
    NULL,
    VK_TRUE
  };

  VkBufferCreateInfo info;
  info.sType                   = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
  info.pNext                   = dedicate == 1 ? &dedicateInfo : NULL;
  info.flags                   = 0;
  info.size                    = bytesCount;
  info.usage                   = (VkBufferUsageFlags)type;
  if (initialQueueFamilyIndex == (unsigned)-1) {
    info.sharingMode           = VK_SHARING_MODE_CONCURRENT;
    info.queueFamilyIndexCount = ctx->internal.cold->gpus[gpuIndex].vkQueueFamilyIndexesCount;
    info.pQueueFamilyIndices   = ctx->internal.cold->gpus[gpuIndex].vkQueueFamilyIndexes;
  } else {
    info.sharingMode           = VK_SHARING_MODE_EXCLUSIVE;
    info.queueFamilyIndexCount = 1;
    info.pQueueFamilyIndices   = &initialQueueFamilyIndex;
  }
  redInternalSetStatus(ctx, gpu, outStatuses, 190, vk->vkCreateBuffer((VkDevice)gpu, &info, NULL, (VkBuffer *)&outArray->handle), procedureId, optionalFile, optionalLine, optionalUserData);

  VkMemoryRequirements memoryRequirements = {0};
  if (outArray->handle != NULL) {
    vk->vkGetBufferMemoryRequirements((VkDevice)gpu, (VkBuffer)outArray->handle, &memoryRequirements);
  }
  outArray->memoryBytesCount     = memoryRequirements.size;
  outArray->memoryBytesAlignment = memoryRequirements.alignment;
  outArray->memoryTypesSupported = memoryRequirements.memoryTypeBits;

  if (ctx->internal.debugUtilsMessengerCallback != NULL && handleName != NULL) {
    if (outArray->handle != NULL) {
      VkDebugUtilsObjectNameInfoEXT info;
      info.sType        = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT;
      info.pNext        = NULL;
      info.objectType   = (VkObjectType)RED_HANDLE_TYPE_ARRAY;
      info.objectHandle = (uint64_t)outArray->handle;
      info.pObjectName  = handleName;
      redInternalSetStatus(ctx, gpu, outStatuses, 191, vk->vkSetDebugUtilsObjectNameEXT((VkDevice)gpu, &info), procedureId, optionalFile, optionalLine, optionalUserData);
    }
  }
}

static inline void redDebugInternalMemoryAllocate(RedInternalContext ctx, RedHandleGpu gpu, unsigned gpuIndex, const char * handleName, uint64_t bytesCount, unsigned memoryTypeIndex, RedBool32 mapMemory, RedHandleArray dedicateToArray, RedHandleImage dedicateToImage, RedMemory * outMemory, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData, RedProcedureId procedureId) {
  RedInternalInstanceProcedureAddresses * vk = ctx->internal.instanceProcedureAddresses;

  RedHandleMemory handle  = NULL;
  void *          pointer = NULL;

  RedBool32 isCpuMappable = ctx->gpus[gpuIndex].memoryTypes[memoryTypeIndex].isCpuMappable;

  RedBool32 dedicate = 0;
  if (redInternalGpuExtensionIsEnabled(ctx, gpuIndex, RED_INTERNAL_GPU_EXTENSION_BITFLAG_DEDICATE_MEMORY_NV)) {
    if (dedicateToArray != NULL || dedicateToImage != NULL) {
      dedicate = 1;
    }
  }

  struct VkDedicatedAllocationMemoryAllocateInfoNV {
    unsigned sType;
    void *   pNext;
    VkImage  image;
    VkBuffer buffer;
  } dedicateInfo = {
    1000026002, // VK_STRUCTURE_TYPE_DEDICATED_ALLOCATION_MEMORY_ALLOCATE_INFO_NV
    NULL,
    (VkImage)dedicateToImage,
    (VkBuffer)dedicateToArray
  };

  VkMemoryAllocateInfo info;
  info.sType           = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
  info.pNext           = dedicate == 1 ? &dedicateInfo : NULL;
  info.allocationSize  = bytesCount;
  info.memoryTypeIndex = memoryTypeIndex;
  redInternalSetStatus(ctx, gpu, outStatuses, 192, vk->vkAllocateMemory((VkDevice)gpu, &info, NULL, (VkDeviceMemory *)&handle), procedureId, optionalFile, optionalLine, optionalUserData);

  if (handle != NULL && mapMemory == 1 && isCpuMappable == 1) {
    redInternalSetStatus(ctx, gpu, outStatuses, 193, vk->vkMapMemory((VkDevice)gpu, (VkDeviceMemory)handle, 0, (VkDeviceSize)-1, 0, &pointer), procedureId, optionalFile, optionalLine, optionalUserData);
  }

  if (ctx->internal.debugUtilsMessengerCallback != NULL && handleName != NULL) {
    if (handle != NULL) {
      VkDebugUtilsObjectNameInfoEXT info;
      info.sType        = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT;
      info.pNext        = NULL;
      info.objectType   = (VkObjectType)RED_HANDLE_TYPE_MEMORY;
      info.objectHandle = (uint64_t)handle;
      info.pObjectName  = handleName;
      redInternalSetStatus(ctx, gpu, outStatuses, 194, vk->vkSetDebugUtilsObjectNameEXT((VkDevice)gpu, &info), procedureId, optionalFile, optionalLine, optionalUserData);
    }
  }

  outMemory->handle  = handle;
  outMemory->pointer = pointer;
}

static inline void redDebugInternalMemorySet(RedInternalContext ctx, RedHandleGpu gpu, unsigned gpuIndex, unsigned memoryArraysCount, const RedMemoryArray * memoryArrays, unsigned memoryImagesCount, const RedMemoryImage * memoryImages, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData) {
  const RedProcedureId procedureId = RED_PROCEDURE_ID_redMemorySet;

  RedInternalInstanceProcedureAddresses * vk = ctx->internal.instanceProcedureAddresses;

  if (redInternalGpuExtensionIsEnabled(ctx, gpuIndex, RED_INTERNAL_GPU_EXTENSION_BITFLAG_BATCH_MEMORY_SET)) {
    if (memoryArraysCount > 0) {
      redInternalSetStatus(ctx, gpu, outStatuses, 195, vk->vkBindBufferMemory2KHR((VkDevice)gpu, memoryArraysCount, memoryArrays), procedureId, optionalFile, optionalLine, optionalUserData);
    }
    if (memoryImagesCount > 0) {
      redInternalSetStatus(ctx, gpu, outStatuses, 196, vk->vkBindImageMemory2KHR( (VkDevice)gpu, memoryImagesCount, memoryImages), procedureId, optionalFile, optionalLine, optionalUserData);
    }
  } else {
    for (unsigned i = 0; i < memoryArraysCount; i += 1) {
      redInternalSetStatus(ctx, gpu, outStatuses, 197, vk->vkBindBufferMemory((VkDevice)gpu, (VkBuffer)memoryArrays[i].array, (VkDeviceMemory)memoryArrays[i].memory, memoryArrays[i].memoryBytesFirst), procedureId, optionalFile, optionalLine, optionalUserData);
    }
    for (unsigned i = 0; i < memoryImagesCount; i += 1) {
      redInternalSetStatus(ctx, gpu, outStatuses, 198, vk->vkBindImageMemory( (VkDevice)gpu,  (VkImage)memoryImages[i].image, (VkDeviceMemory)memoryImages[i].memory, memoryImages[i].memoryBytesFirst), procedureId, optionalFile, optionalLine, optionalUserData);
    }
  }
}

static inline RedDebugInternalArray redDebugInternalCreateDebugArray(RedInternalContext ctx, RedHandleGpu gpu, unsigned gpuIndex, const char * handleName, RedArrayType type, uint64_t bytesCount, unsigned initialQueueFamilyIndex, RedBool32 dedicate, RedDebugInternalMemoryType memoryType, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData) {
  RedDebugInternalArray out = {0};

  redDebugInternalCreateArray(ctx, gpu, gpuIndex, handleName, type, bytesCount, initialQueueFamilyIndex, dedicate, &out.array, outStatuses, optionalFile, optionalLine, optionalUserData);
  if (outStatuses != NULL) {
    if (outStatuses[0].statusError != RED_STATUS_SUCCESS) {
      return out;
    }
  }

  const unsigned memoryTypeIndex = redDebugInternalHelperGetMemoryTypeIndex(ctx, gpuIndex, memoryType, out.array.memoryTypesSupported);
  if (memoryTypeIndex == (unsigned)-1) {
    if (outStatuses != NULL) {
      if (outStatuses[0].statusError == RED_STATUS_SUCCESS) {
        outStatuses[0].statusError            = RED_STATUS_ERROR_INITIALIZATION_FAILED;
        outStatuses[0].statusErrorCode        = 364;
        outStatuses[0].statusErrorProcedureId = RED_PROCEDURE_ID_redMemoryAllocate;
      }
    }
    return out;
  }

  redDebugInternalMemoryAllocate(ctx, gpu, gpuIndex, handleName, out.array.memoryBytesCount, memoryTypeIndex, 1, dedicate == 1 ? out.array.handle : NULL, NULL, &out.memory, outStatuses, optionalFile, optionalLine, optionalUserData, RED_PROCEDURE_ID_redMemoryAllocateMappable);
  if (outStatuses != NULL) {
    if (outStatuses[0].statusError != RED_STATUS_SUCCESS) {
      return out;
    }
  }

  RedMemoryArray memoryArray   = {0};
  memoryArray.setTo1000157000  = 1000157000;
  memoryArray.setTo0           = 0;
  memoryArray.array            = out.array.handle;
  memoryArray.memory           = out.memory.handle;
  memoryArray.memoryBytesFirst = 0;
  redDebugInternalMemorySet(ctx, gpu, gpuIndex, 1, &memoryArray, 0, NULL, outStatuses, optionalFile, optionalLine, optionalUserData);

  return out;
}

static inline void redDebugInternalCreateEvent(RedInternalContext ctx, RedHandleGpu gpu, const char * handleName, VkEvent * outEvent, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData) {
  const RedProcedureId procedureId = RED_PROCEDURE_ID_redCreateGpuToCpuSignal;

  RedInternalInstanceProcedureAddresses * vk = ctx->internal.instanceProcedureAddresses;

  VkEventCreateInfo eventInfo;
  eventInfo.sType = VK_STRUCTURE_TYPE_EVENT_CREATE_INFO;
  eventInfo.pNext = NULL;
  eventInfo.flags = 0;
  redInternalSetStatus(ctx, gpu, outStatuses, 199, vk->vkCreateEvent((VkDevice)gpu, &eventInfo, NULL, outEvent), procedureId, optionalFile, optionalLine, optionalUserData);

  if (ctx->internal.debugUtilsMessengerCallback != NULL && handleName != NULL) {
    if (outEvent[0] != NULL) {
      VkDebugUtilsObjectNameInfoEXT info;
      info.sType        = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT;
      info.pNext        = NULL;
      info.objectType   = (VkObjectType)RED_HANDLE_TYPE_GPU_TO_CPU_SIGNAL;
      info.objectHandle = (uint64_t)outEvent[0];
      info.pObjectName  = handleName;
      redInternalSetStatus(ctx, gpu, outStatuses, 200, vk->vkSetDebugUtilsObjectNameEXT((VkDevice)gpu, &info), procedureId, optionalFile, optionalLine, optionalUserData);
    }
  }
}

static inline void redDebugInternalCreateCpuSignal(RedInternalContext ctx, RedHandleGpu gpu, const char * handleName, RedBool32 createSignaled, RedHandleCpuSignal * outCpuSignal, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData) {
  const RedProcedureId procedureId = RED_PROCEDURE_ID_redCreateCpuSignal;

  RedInternalInstanceProcedureAddresses * vk = ctx->internal.instanceProcedureAddresses;

  VkFenceCreateInfo info;
  info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
  info.pNext = NULL;
  info.flags = createSignaled == 1 ? VK_FENCE_CREATE_SIGNALED_BIT : 0;
  redInternalSetStatus(ctx, gpu, outStatuses, 201, vk->vkCreateFence((VkDevice)gpu, &info, NULL, (VkFence *)outCpuSignal), procedureId, optionalFile, optionalLine, optionalUserData);

  if (ctx->internal.debugUtilsMessengerCallback != NULL && handleName != NULL) {
    if (outCpuSignal[0] != NULL) {
      VkDebugUtilsObjectNameInfoEXT info;
      info.sType        = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT;
      info.pNext        = NULL;
      info.objectType   = (VkObjectType)RED_HANDLE_TYPE_CPU_SIGNAL;
      info.objectHandle = (uint64_t)outCpuSignal[0];
      info.pObjectName  = handleName;
      redInternalSetStatus(ctx, gpu, outStatuses, 202, vk->vkSetDebugUtilsObjectNameEXT((VkDevice)gpu, &info), procedureId, optionalFile, optionalLine, optionalUserData);
    }
  }
}

static inline void redDebugInternalCpuSignalUnsignal(RedInternalContext ctx, RedHandleGpu gpu, unsigned cpuSignalsCount, const RedHandleCpuSignal * cpuSignals, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData) {
  const RedProcedureId procedureId = RED_PROCEDURE_ID_redCpuSignalUnsignal;

  RedInternalInstanceProcedureAddresses * vk = ctx->internal.instanceProcedureAddresses;

  redInternalSetStatus(ctx, gpu, outStatuses, 203, vk->vkResetFences((VkDevice)gpu, cpuSignalsCount, (const VkFence *)cpuSignals), procedureId, optionalFile, optionalLine, optionalUserData);
}

static inline void redDebugInternalDestroyArray(RedInternalContext ctx, RedHandleGpu gpu, RedHandleArray array) {
//  const RedProcedureId procedureId = RED_PROCEDURE_ID_redDestroyArray;

  RedInternalInstanceProcedureAddresses * vk = ctx->internal.instanceProcedureAddresses;

  vk->vkDestroyBuffer((VkDevice)gpu, (VkBuffer)array, NULL);
}

static inline void redDebugInternalMemoryFree(RedInternalContext ctx, RedHandleGpu gpu, RedHandleMemory memory) {
//  const RedProcedureId procedureId = RED_PROCEDURE_ID_redMemoryFree;

  RedInternalInstanceProcedureAddresses * vk = ctx->internal.instanceProcedureAddresses;

  vk->vkFreeMemory((VkDevice)gpu, (VkDeviceMemory)memory, NULL);
}

static inline void redDebugInternalDestroyEvent(RedInternalContext ctx, RedHandleGpu gpu, VkEvent event) {
//  const RedProcedureId procedureId = RED_PROCEDURE_ID_redDestroyGpuToCpuSignal;

  RedInternalInstanceProcedureAddresses * vk = ctx->internal.instanceProcedureAddresses;

  vk->vkDestroyEvent((VkDevice)gpu, event, NULL);
}

static inline void redDebugInternalDestroyCpuSignal(RedInternalContext ctx, RedHandleGpu gpu, RedHandleCpuSignal cpuSignal) {
//  const RedProcedureId procedureId = RED_PROCEDURE_ID_redDestroyCpuSignal;

  RedInternalInstanceProcedureAddresses * vk = ctx->internal.instanceProcedureAddresses;

  vk->vkDestroyFence((VkDevice)gpu, (VkFence)cpuSignal, NULL);
}
#endif

#define REDGPU_CREATE_CONTEXT_ARRAY_MALLOC(gpu, status_index, structure_pointer, structure_count, structure_type, allocation_code) \
  if (structure_count > 0) { \
    structure_pointer = (structure_type *)REDGPU_INTERNAL_ADAPTIVE_MALLOC(structure_count * sizeof(structure_pointer[0]), allocation_code); \
    if (structure_pointer == NULL) { \
      redInlineDestroyContext((RedContext)ctx, optionalFile, optionalLine, optionalUserData); \
      ctx = NULL; \
      redInternalSetStatus(ctx, gpu, outStatuses, status_index, (VkResult)RED_STATUS_ERROR_OUT_OF_CPU_MEMORY, procedureId, optionalFile, optionalLine, optionalUserData); \
      outContext[0] = (RedContext)ctx; \
      return; \
    } \
  }

#define REDGPU_CREATE_CONTEXT_DESTROY_CTX_THEN_SET_CTX_TO_NULL_THEN_SET_STATUS_ERROR_THEN_GOTO_EXIT(gpu, status_index, status_error) \
  do { \
    redInlineDestroyContext((RedContext)ctx, optionalFile, optionalLine, optionalUserData); \
    ctx = NULL; \
    redInternalSetStatus(ctx, gpu, outStatuses, status_index, (VkResult)status_error, procedureId, optionalFile, optionalLine, optionalUserData); \
    outContext[0] = (RedContext)ctx; \
    return; \
  } while (0)

#define REDGPU_ARRAY_MEMSET(structure_pointer, structure_count) \
  if (structure_count > 0) { \
    redInternalMemoryClear((void *)structure_pointer, structure_count * sizeof(structure_pointer[0])); \
  }

typedef unsigned RedVkAccessStageBitflags;
typedef enum RedVkAccessStageBitflag {
  RED_VK_ACCESS_STAGE_BITFLAG_COPY                 = REDGPU_B32(0000,0000,0000,0000,0001,0000,0000,0000),
  RED_VK_ACCESS_STAGE_BITFLAG_COMPUTE              = REDGPU_B32(0000,0000,0000,0000,0000,1000,0000,0000),
  RED_VK_ACCESS_STAGE_BITFLAG_INDEX                = REDGPU_B32(0000,0000,0000,0000,0000,0000,0000,0100),
  RED_VK_ACCESS_STAGE_BITFLAG_VERTEX               = REDGPU_B32(0000,0000,0000,0000,0000,0000,0000,1000),
  RED_VK_ACCESS_STAGE_BITFLAG_FRAGMENT             = REDGPU_B32(0000,0000,0000,0000,0000,0000,1000,0000),
  RED_VK_ACCESS_STAGE_BITFLAG_OUTPUT_DEPTH_STENCIL = REDGPU_B32(0000,0000,0000,0000,0000,0011,0000,0000),
  RED_VK_ACCESS_STAGE_BITFLAG_OUTPUT_COLOR         = REDGPU_B32(0000,0000,0000,0000,0000,0100,0000,0000),
  RED_VK_ACCESS_STAGE_BITFLAG_RESOLVE              = REDGPU_B32(0000,0000,0000,0000,0000,0100,0000,0000),
  RED_VK_ACCESS_STAGE_BITFLAG_CPU                  = REDGPU_B32(0000,0000,0000,0000,0100,0000,0000,0000),
} RedVkAccessStageBitflag;

typedef unsigned RedVkAccessBitflags;
typedef enum RedVkAccessBitflag {
  RED_VK_ACCESS_BITFLAG_COPY_R                     = REDGPU_B32(0000,0000,0000,0000,0000,1000,0000,0000),
  RED_VK_ACCESS_BITFLAG_COPY_W                     = REDGPU_B32(0000,0000,0000,0000,0001,0000,0000,0000),
  RED_VK_ACCESS_BITFLAG_INDEX_R                    = REDGPU_B32(0000,0000,0000,0000,0000,0000,0000,0010),
  RED_VK_ACCESS_BITFLAG_STRUCT_ARRAY_RO_CONSTANT_R = REDGPU_B32(0000,0000,0000,0000,0000,0000,0000,1000),
  RED_VK_ACCESS_BITFLAG_STRUCT_RESOURCE_R          = REDGPU_B32(0000,0000,0000,0000,0000,0000,0010,0000),
  RED_VK_ACCESS_BITFLAG_STRUCT_RESOURCE_W          = REDGPU_B32(0000,0000,0000,0000,0000,0000,0100,0000),
  RED_VK_ACCESS_BITFLAG_OUTPUT_DEPTH_STENCIL       = REDGPU_B32(0000,0000,0000,0000,0000,0110,0000,0000),
  RED_VK_ACCESS_BITFLAG_OUTPUT_COLOR               = REDGPU_B32(0000,0000,0000,0000,0000,0001,1000,0000),
  RED_VK_ACCESS_BITFLAG_RESOLVE_SOURCE_R           = REDGPU_B32(0000,0000,0000,0000,0000,0001,1000,0000),
  RED_VK_ACCESS_BITFLAG_RESOLVE_TARGET_W           = REDGPU_B32(0000,0000,0000,0000,0000,0001,0000,0000),
  RED_VK_ACCESS_BITFLAG_CPU                        = REDGPU_B32(0000,0000,0000,0000,0110,0000,0000,0000),
} RedVkAccessBitflag;

typedef struct RedVkUsageArray {
  unsigned             setTo44;
  size_t               setTo0;
  RedVkAccessBitflags  oldAccesses;
  RedVkAccessBitflags  newAccesses;
  unsigned             queueFamilyIndexSource;
  unsigned             queueFamilyIndexTarget;
  RedHandleArray       array;
  uint64_t             arrayBytesFirst;
  uint64_t             arrayBytesCount;
} RedVkUsageArray;

typedef struct RedVkUsageImageParts {
  RedImagePartBitflags allParts;
  unsigned             levelsFirst;
  unsigned             levelsCount;
  unsigned             layersFirst;
  unsigned             layersCount;
} RedVkUsageImageParts;

typedef struct RedVkUsageImage {
  unsigned             setTo45;
  size_t               setTo0;
  RedVkAccessBitflags  oldAccesses;
  RedVkAccessBitflags  newAccesses;
  RedState             oldState;
  RedState             newState;
  unsigned             queueFamilyIndexSource;
  unsigned             queueFamilyIndexTarget;
  RedHandleImage       image;
  RedVkUsageImageParts imageParts;
} RedVkUsageImage;

// Memory

static inline void redInlineMemoryGetBudget(RedContext context, RedHandleGpu gpu, RedMemoryBudget * outMemoryBudget, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData) {
  const RedProcedureId procedureId = RED_PROCEDURE_ID_redMemoryGetBudget;

  RedInternalContext ctx = (RedInternalContext)context;
  RedInternalInstanceProcedureAddresses * vk = ctx->internal.instanceProcedureAddresses;

  unsigned gpuIndex = (unsigned)-1;
  for (unsigned i = 0; i < ctx->internal.hotGpuHandlesCount; i += 1) {
    if (ctx->internal.hotGpuHandles[i] == gpu) {
      gpuIndex = i;
      break;
    }
  }

  if (gpuIndex == (unsigned)-1) {
    redInternalSetStatus(ctx, gpu, outStatuses, 204, (VkResult)RED_STATUS_ERROR_INITIALIZATION_FAILED, procedureId, optionalFile, optionalLine, optionalUserData);
    return;
  }

  if (redInternalGpuExtensionIsEnabled(ctx, gpuIndex, RED_INTERNAL_GPU_EXTENSION_BITFLAG_MEMORY_GET_BUDGET))
  {
  } else {
    redInternalSetStatus(ctx, gpu, outStatuses, 205, (VkResult)RED_STATUS_ERROR_FEATURE_IS_NOT_FOUND, procedureId, optionalFile, optionalLine, optionalUserData);
    return;
  }

  RedHandleGpuDevice gpuDevice = context->gpus[gpuIndex].gpuDevice;

  VkPhysicalDeviceMemoryProperties2 physicalDeviceMemoryProperties2;
  physicalDeviceMemoryProperties2.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MEMORY_PROPERTIES_2;
  physicalDeviceMemoryProperties2.pNext = outMemoryBudget;
  // physicalDeviceMemoryProperties2.memoryProperties
  vk->vkGetPhysicalDeviceMemoryProperties2KHR((VkPhysicalDevice)gpuDevice, &physicalDeviceMemoryProperties2);
}

static inline void redInlineMemoryAllocate(RedContext context, RedHandleGpu gpu, const char * handleName, uint64_t bytesCount, unsigned memoryTypeIndex, RedHandleArray dedicateToArray, RedHandleImage dedicateToImage, RedHandleMemory * outMemory, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData, RedProcedureId procedureId) {
  RedInternalContext ctx = (RedInternalContext)context;
  RedInternalInstanceProcedureAddresses * vk = ctx->internal.instanceProcedureAddresses;

  RedInternalVirtualHandleMemory * handleVirtual = NULL;
  RedHandleMemory                  handle        = NULL;

  unsigned gpuIndex = (unsigned)-1;
  for (unsigned i = 0; i < ctx->internal.hotGpuHandlesCount; i += 1) {
    if (ctx->internal.hotGpuHandles[i] == gpu) {
      gpuIndex = i;
      break;
    }
  }

  if (gpuIndex == (unsigned)-1) {
    redInternalSetStatus(ctx, gpu, outStatuses, 206, (VkResult)RED_STATUS_ERROR_INITIALIZATION_FAILED, procedureId, optionalFile, optionalLine, optionalUserData);
    outMemory[0] = handle;
    return;
  }

  RedBool32 isCpuMappable = context->gpus[gpuIndex].memoryTypes[memoryTypeIndex].isCpuMappable;

  if (redInternalContextExtensionIsEnabled(ctx, RED_INTERNAL_CONTEXT_EXTENSION_BITFLAG_TREAT_ALL_MAPPABLE_MEMORY_AS_NON_COHERENT)) {
    if (isCpuMappable == 1) {
      struct RedInternalVirtualHandleMemoryAllocationMappable {
        RedInternalVirtualHandleMemory            handleVirtual;
        RedInternalVirtualHandleMemoryMappingInfo mappingInfo;
      };

      struct RedInternalVirtualHandleMemoryAllocationMappable * packed = (struct RedInternalVirtualHandleMemoryAllocationMappable *)REDGPU_INTERNAL_ADAPTIVE_MALLOC(sizeof(struct RedInternalVirtualHandleMemoryAllocationMappable) + (size_t)bytesCount, 47);
      if (packed == NULL) {
        redInternalSetStatus(ctx, gpu, outStatuses, 207, (VkResult)RED_STATUS_ERROR_OUT_OF_CPU_MEMORY, procedureId, optionalFile, optionalLine, optionalUserData);
        outMemory[0] = handle;
        return;
      }
      redInternalMemoryClear(packed, sizeof(struct RedInternalVirtualHandleMemoryAllocationMappable) + (size_t)bytesCount);

      handleVirtual = &packed->handleVirtual;

      handleVirtual->mappingInfo = &packed->mappingInfo;
      handleVirtual->allocation  = (void *)packed;

      unsigned char * pointerToStagingData = (unsigned char *)(void *)packed;
      pointerToStagingData += sizeof(struct RedInternalVirtualHandleMemoryAllocationMappable);

      handleVirtual->mappingInfo->stagingPointer      = pointerToStagingData;
      handleVirtual->mappingInfo->memoryIsCpuCoherent = context->gpus[gpuIndex].memoryTypes[memoryTypeIndex].isCpuCoherent;
    } else {
      struct RedInternalVirtualHandleMemoryAllocationNonMappable {
        RedInternalVirtualHandleMemory handleVirtual;
      };

      struct RedInternalVirtualHandleMemoryAllocationNonMappable * packed = (struct RedInternalVirtualHandleMemoryAllocationNonMappable *)REDGPU_INTERNAL_ADAPTIVE_MALLOC(sizeof(struct RedInternalVirtualHandleMemoryAllocationNonMappable), 48);
      if (packed == NULL) {
        redInternalSetStatus(ctx, gpu, outStatuses, 208, (VkResult)RED_STATUS_ERROR_OUT_OF_CPU_MEMORY, procedureId, optionalFile, optionalLine, optionalUserData);
        outMemory[0] = handle;
        return;
      }
      redInternalMemoryClear(packed, sizeof(struct RedInternalVirtualHandleMemoryAllocationNonMappable));

      handleVirtual = &packed->handleVirtual;

      handleVirtual->allocation = (void *)packed;
    }
  }

  RedBool32 dedicate = 0;
  if (redInternalGpuExtensionIsEnabled(ctx, gpuIndex, RED_INTERNAL_GPU_EXTENSION_BITFLAG_DEDICATE_MEMORY_NV)) {
    if (dedicateToArray != NULL || dedicateToImage != NULL) {
      dedicate = 1;
    }
  }

  struct VkDedicatedAllocationMemoryAllocateInfoNV {
    unsigned sType;
    void *   pNext;
    VkImage  image;
    VkBuffer buffer;
  } dedicateInfo = {
    1000026002, // VK_STRUCTURE_TYPE_DEDICATED_ALLOCATION_MEMORY_ALLOCATE_INFO_NV
    NULL,
    (VkImage)dedicateToImage,
    (VkBuffer)dedicateToArray
  };

  VkMemoryAllocateInfo info;
  info.sType           = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
  info.pNext           = dedicate == 1 ? &dedicateInfo : NULL;
  info.allocationSize  = bytesCount;
  info.memoryTypeIndex = memoryTypeIndex;
  redInternalSetStatus(ctx, gpu, outStatuses, 209, vk->vkAllocateMemory((VkDevice)gpu, &info, NULL, (VkDeviceMemory *)&handle), procedureId, optionalFile, optionalLine, optionalUserData);

  if (ctx->internal.debugUtilsMessengerCallback != NULL && handleName != NULL) {
    if (handle != NULL) {
      VkDebugUtilsObjectNameInfoEXT info;
      info.sType        = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT;
      info.pNext        = NULL;
      info.objectType   = (VkObjectType)RED_HANDLE_TYPE_MEMORY;
      info.objectHandle = (uint64_t)handle;
      info.pObjectName  = handleName;
      redInternalSetStatus(ctx, gpu, outStatuses, 211, vk->vkSetDebugUtilsObjectNameEXT((VkDevice)gpu, &info), procedureId, optionalFile, optionalLine, optionalUserData);
    }
  }

  if (redInternalContextExtensionIsEnabled(ctx, RED_INTERNAL_CONTEXT_EXTENSION_BITFLAG_TREAT_ALL_MAPPABLE_MEMORY_AS_NON_COHERENT)) {
    handleVirtual->handleActual = handle;
    handle = (RedHandleMemory)(void *)handleVirtual;
  }

  outMemory[0] = handle;
}

static inline void redInlineMemoryFree(RedContext context, RedHandleGpu gpu, RedHandleMemory memory, const char * optionalFile, int optionalLine, void * optionalUserData) {
  const RedProcedureId procedureId = RED_PROCEDURE_ID_redMemoryFree;

  RedInternalContext ctx = (RedInternalContext)context;
  RedInternalInstanceProcedureAddresses * vk = ctx->internal.instanceProcedureAddresses;

  RedInternalVirtualHandleMemory * handleVirtual = (RedInternalVirtualHandleMemory *)(void *)memory;
  RedHandleMemory                  handle        = memory;

  if (redInternalContextExtensionIsEnabled(ctx, RED_INTERNAL_CONTEXT_EXTENSION_BITFLAG_TREAT_ALL_MAPPABLE_MEMORY_AS_NON_COHERENT)) {
    handle = handleVirtual->handleActual;
  }

  vk->vkFreeMemory((VkDevice)gpu, (VkDeviceMemory)handle, NULL);

  if (redInternalContextExtensionIsEnabled(ctx, RED_INTERNAL_CONTEXT_EXTENSION_BITFLAG_TREAT_ALL_MAPPABLE_MEMORY_AS_NON_COHERENT)) {
    REDGPU_INTERNAL_ADAPTIVE_FREE(handleVirtual->allocation, 49);
  }
}

static inline void redInlineMemorySet(RedContext context, RedHandleGpu gpu, unsigned memoryArraysCount, const RedMemoryArray * memoryArrays, unsigned memoryImagesCount, const RedMemoryImage * memoryImages, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData) {
  const RedProcedureId procedureId = RED_PROCEDURE_ID_redMemorySet;

  RedInternalContext ctx = (RedInternalContext)context;
  RedInternalInstanceProcedureAddresses * vk = ctx->internal.instanceProcedureAddresses;

  unsigned gpuIndex = (unsigned)-1;
  for (unsigned i = 0; i < ctx->internal.hotGpuHandlesCount; i += 1) {
    if (ctx->internal.hotGpuHandles[i] == gpu) {
      gpuIndex = i;
      break;
    }
  }

  if (gpuIndex == (unsigned)-1) {
    redInternalSetStatus(ctx, gpu, outStatuses, 212, (VkResult)RED_STATUS_ERROR_INITIALIZATION_FAILED, procedureId, optionalFile, optionalLine, optionalUserData);
    goto exit;
  }

  if (redInternalContextExtensionIsEnabled(ctx, RED_INTERNAL_CONTEXT_EXTENSION_BITFLAG_TREAT_ALL_MAPPABLE_MEMORY_AS_NON_COHERENT)) {
    for (unsigned i = 0; i < memoryArraysCount; i += 1) {
      RedInternalVirtualHandleMemory * handleVirtual = (RedInternalVirtualHandleMemory *)(void *)memoryArrays[i].memory;
      redInternalSetStatus(ctx, gpu, outStatuses, 213, vk->vkBindBufferMemory((VkDevice)gpu, (VkBuffer)memoryArrays[i].array, (VkDeviceMemory)handleVirtual->handleActual, memoryArrays[i].memoryBytesFirst), procedureId, optionalFile, optionalLine, optionalUserData);
    }
    for (unsigned i = 0; i < memoryImagesCount; i += 1) {
      RedInternalVirtualHandleMemory * handleVirtual = (RedInternalVirtualHandleMemory *)(void *)memoryImages[i].memory;
      redInternalSetStatus(ctx, gpu, outStatuses, 214, vk->vkBindImageMemory( (VkDevice)gpu,  (VkImage)memoryImages[i].image, (VkDeviceMemory)handleVirtual->handleActual, memoryImages[i].memoryBytesFirst), procedureId, optionalFile, optionalLine, optionalUserData);
    }
  } else {
    if (redInternalGpuExtensionIsEnabled(ctx, gpuIndex, RED_INTERNAL_GPU_EXTENSION_BITFLAG_BATCH_MEMORY_SET)) {
      if (memoryArraysCount > 0) {
        redInternalSetStatus(ctx, gpu, outStatuses, 215, vk->vkBindBufferMemory2KHR((VkDevice)gpu, memoryArraysCount, memoryArrays), procedureId, optionalFile, optionalLine, optionalUserData);
      }
      if (memoryImagesCount > 0) {
        redInternalSetStatus(ctx, gpu, outStatuses, 216, vk->vkBindImageMemory2KHR( (VkDevice)gpu, memoryImagesCount, memoryImages), procedureId, optionalFile, optionalLine, optionalUserData);
      }
    } else {
      for (unsigned i = 0; i < memoryArraysCount; i += 1) {
        redInternalSetStatus(ctx, gpu, outStatuses, 217, vk->vkBindBufferMemory((VkDevice)gpu, (VkBuffer)memoryArrays[i].array, (VkDeviceMemory)memoryArrays[i].memory, memoryArrays[i].memoryBytesFirst), procedureId, optionalFile, optionalLine, optionalUserData);
      }
      for (unsigned i = 0; i < memoryImagesCount; i += 1) {
        redInternalSetStatus(ctx, gpu, outStatuses, 218, vk->vkBindImageMemory( (VkDevice)gpu,  (VkImage)memoryImages[i].image, (VkDeviceMemory)memoryImages[i].memory, memoryImages[i].memoryBytesFirst), procedureId, optionalFile, optionalLine, optionalUserData);
      }
    }
  }

exit:;

}

static inline void redInlineMemoryMap(RedContext context, RedHandleGpu gpu, RedHandleMemory memory, uint64_t memoryBytesFirst, uint64_t memoryBytesCount, void ** outPointer, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData) {
  const RedProcedureId procedureId = RED_PROCEDURE_ID_redMemoryMap;

  RedInternalContext ctx = (RedInternalContext)context;
  RedInternalInstanceProcedureAddresses * vk = ctx->internal.instanceProcedureAddresses;

  RedInternalVirtualHandleMemory * handleVirtual = (RedInternalVirtualHandleMemory *)(void *)memory;
  RedHandleMemory                  handle        = memory;

  if (redInternalContextExtensionIsEnabled(ctx, RED_INTERNAL_CONTEXT_EXTENSION_BITFLAG_TREAT_ALL_MAPPABLE_MEMORY_AS_NON_COHERENT)) {
    handle = handleVirtual->handleActual;
  }

  void * pointer = NULL;
  redInternalSetStatus(ctx, gpu, outStatuses, 219, vk->vkMapMemory((VkDevice)gpu, (VkDeviceMemory)handle, memoryBytesFirst, memoryBytesCount, 0, &pointer), procedureId, optionalFile, optionalLine, optionalUserData);

  if (redInternalContextExtensionIsEnabled(ctx, RED_INTERNAL_CONTEXT_EXTENSION_BITFLAG_TREAT_ALL_MAPPABLE_MEMORY_AS_NON_COHERENT)) {
    handleVirtual->mappingInfo->mappedPointerBytesFirst = memoryBytesFirst;
    handleVirtual->mappingInfo->mappedPointer           = (unsigned char *)pointer;
    pointer = &handleVirtual->mappingInfo->stagingPointer[memoryBytesFirst];
  }

  outPointer[0] = pointer;
}

static inline void redInlineMemoryUnmap(RedContext context, RedHandleGpu gpu, RedHandleMemory memory, const char * optionalFile, int optionalLine, void * optionalUserData) {
//  const RedProcedureId procedureId = RED_PROCEDURE_ID_redMemoryUnmap;

  RedInternalContext ctx = (RedInternalContext)context;
  RedInternalInstanceProcedureAddresses * vk = ctx->internal.instanceProcedureAddresses;

  RedInternalVirtualHandleMemory * handleVirtual = (RedInternalVirtualHandleMemory *)(void *)memory;
  RedHandleMemory                  handle        = memory;

  if (redInternalContextExtensionIsEnabled(ctx, RED_INTERNAL_CONTEXT_EXTENSION_BITFLAG_TREAT_ALL_MAPPABLE_MEMORY_AS_NON_COHERENT)) {
    handle = handleVirtual->handleActual;
  }

  vk->vkUnmapMemory((VkDevice)gpu, (VkDeviceMemory)handle);

  if (redInternalContextExtensionIsEnabled(ctx, RED_INTERNAL_CONTEXT_EXTENSION_BITFLAG_TREAT_ALL_MAPPABLE_MEMORY_AS_NON_COHERENT)) {
    handleVirtual->mappingInfo->mappedPointerBytesFirst = 0;
    handleVirtual->mappingInfo->mappedPointer           = NULL;
  }
}

static inline void redInlineMemoryNonCoherentFlush(RedContext context, RedHandleGpu gpu, unsigned memoryRangesCount, const RedMappableMemoryRange * memoryRanges, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData) {
  const RedProcedureId procedureId = RED_PROCEDURE_ID_redMemoryNonCoherentFlush;

  RedInternalContext ctx = (RedInternalContext)context;
  RedInternalInstanceProcedureAddresses * vk = ctx->internal.instanceProcedureAddresses;

  if (redInternalContextExtensionIsEnabled(ctx, RED_INTERNAL_CONTEXT_EXTENSION_BITFLAG_TREAT_ALL_MAPPABLE_MEMORY_AS_NON_COHERENT)) {
    unsigned nonCoherentStreak = 0;
    for (unsigned i = 0; i < memoryRangesCount; i += 1) {
      RedInternalVirtualHandleMemory * handleVirtual = (RedInternalVirtualHandleMemory *)(void *)memoryRanges[i].mappableMemory;
      if (handleVirtual->mappingInfo->memoryIsCpuCoherent == 0) {
        nonCoherentStreak += 1;
      } else {
        break;
      }
    }
    for (unsigned i = 0; i < memoryRangesCount; i += 1) {
      RedInternalVirtualHandleMemory * handleVirtual = (RedInternalVirtualHandleMemory *)(void *)memoryRanges[i].mappableMemory;
      uint64_t mappedBytesFirst = memoryRanges[i].mappableMemoryRangeBytesFirst - handleVirtual->mappingInfo->mappedPointerBytesFirst;
      uint64_t bytesFirst       = memoryRanges[i].mappableMemoryRangeBytesFirst;
      uint64_t bytesCount       = memoryRanges[i].mappableMemoryRangeBytesCount;
      redInternalMemoryCopy(&handleVirtual->mappingInfo->mappedPointer[mappedBytesFirst], &handleVirtual->mappingInfo->stagingPointer[bytesFirst], bytesCount);
      if (handleVirtual->mappingInfo->memoryIsCpuCoherent == 0 && i >= nonCoherentStreak) {
        redInternalSetStatus(ctx, gpu, outStatuses, 220, vk->vkFlushMappedMemoryRanges((VkDevice)gpu, 1, (const VkMappedMemoryRange *)&memoryRanges[i]), procedureId, optionalFile, optionalLine, optionalUserData);
      }
    }
    if (nonCoherentStreak > 0) {
      redInternalSetStatus(ctx, gpu, outStatuses, 221, vk->vkFlushMappedMemoryRanges((VkDevice)gpu, nonCoherentStreak, (const VkMappedMemoryRange *)memoryRanges), procedureId, optionalFile, optionalLine, optionalUserData);
    }
  } else {
    redInternalSetStatus(ctx, gpu, outStatuses, 222, vk->vkFlushMappedMemoryRanges((VkDevice)gpu, memoryRangesCount, (const VkMappedMemoryRange *)memoryRanges), procedureId, optionalFile, optionalLine, optionalUserData);
  }
}

static inline void redInlineMemoryNonCoherentInvalidate(RedContext context, RedHandleGpu gpu, unsigned memoryRangesCount, const RedMappableMemoryRange * memoryRanges, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData) {
  const RedProcedureId procedureId = RED_PROCEDURE_ID_redMemoryNonCoherentInvalidate;

  RedInternalContext ctx = (RedInternalContext)context;
  RedInternalInstanceProcedureAddresses * vk = ctx->internal.instanceProcedureAddresses;

  if (redInternalContextExtensionIsEnabled(ctx, RED_INTERNAL_CONTEXT_EXTENSION_BITFLAG_TREAT_ALL_MAPPABLE_MEMORY_AS_NON_COHERENT)) {
    unsigned nonCoherentStreak = 0;
    for (unsigned i = 0; i < memoryRangesCount; i += 1) {
      RedInternalVirtualHandleMemory * handleVirtual = (RedInternalVirtualHandleMemory *)(void *)memoryRanges[i].mappableMemory;
      if (handleVirtual->mappingInfo->memoryIsCpuCoherent == 0) {
        nonCoherentStreak += 1;
      } else {
        break;
      }
    }
    if (nonCoherentStreak > 0) {
      redInternalSetStatus(ctx, gpu, outStatuses, 223, vk->vkInvalidateMappedMemoryRanges((VkDevice)gpu, nonCoherentStreak, (const VkMappedMemoryRange *)memoryRanges), procedureId, optionalFile, optionalLine, optionalUserData);
    }
    for (unsigned i = 0; i < memoryRangesCount; i += 1) {
      RedInternalVirtualHandleMemory * handleVirtual = (RedInternalVirtualHandleMemory *)(void *)memoryRanges[i].mappableMemory;
      uint64_t mappedBytesFirst = memoryRanges[i].mappableMemoryRangeBytesFirst - handleVirtual->mappingInfo->mappedPointerBytesFirst;
      uint64_t bytesFirst       = memoryRanges[i].mappableMemoryRangeBytesFirst;
      uint64_t bytesCount       = memoryRanges[i].mappableMemoryRangeBytesCount;
      if (handleVirtual->mappingInfo->memoryIsCpuCoherent == 0 && i >= nonCoherentStreak) {
        redInternalSetStatus(ctx, gpu, outStatuses, 224, vk->vkInvalidateMappedMemoryRanges((VkDevice)gpu, 1, (const VkMappedMemoryRange *)&memoryRanges[i]), procedureId, optionalFile, optionalLine, optionalUserData);
      }
      redInternalMemoryCopy(&handleVirtual->mappingInfo->stagingPointer[bytesFirst], &handleVirtual->mappingInfo->mappedPointer[mappedBytesFirst], bytesCount);
    }
  } else {
    redInternalSetStatus(ctx, gpu, outStatuses, 225, vk->vkInvalidateMappedMemoryRanges((VkDevice)gpu, memoryRangesCount, (const VkMappedMemoryRange *)memoryRanges), procedureId, optionalFile, optionalLine, optionalUserData);
  }
}

// Struct

static inline void redInlineStructsMemoryAllocate(RedContext context, RedHandleGpu gpu, const char * handleName, unsigned maxStructsCount, unsigned maxStructsMembersOfTypeArrayROCount, unsigned maxStructsMembersOfTypeArrayRWCount, unsigned maxStructsMembersOfTypeSamplerCount, unsigned maxStructsMembersOfTypeTextureROCount, unsigned maxStructsMembersOfTypeTextureRWCount, RedHandleStructsMemory * outStructsMemory, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData, RedProcedureId procedureId) {
  RedInternalContext ctx = (RedInternalContext)context;
  RedInternalInstanceProcedureAddresses * vk = ctx->internal.instanceProcedureAddresses;

  unsigned             structMemberTypeCountsCount = 0;
  VkDescriptorPoolSize structMemberTypeCounts[5];
  if (maxStructsMembersOfTypeArrayROCount > 0) {
    structMemberTypeCounts[structMemberTypeCountsCount].type            = (VkDescriptorType)RED_STRUCT_MEMBER_TYPE_ARRAY_RO_CONSTANT;
    structMemberTypeCounts[structMemberTypeCountsCount].descriptorCount = maxStructsMembersOfTypeArrayROCount;
    structMemberTypeCountsCount += 1;
  }
  if (maxStructsMembersOfTypeArrayRWCount > 0) {
    structMemberTypeCounts[structMemberTypeCountsCount].type            = (VkDescriptorType)RED_STRUCT_MEMBER_TYPE_ARRAY_RO_RW;
    structMemberTypeCounts[structMemberTypeCountsCount].descriptorCount = maxStructsMembersOfTypeArrayRWCount;
    structMemberTypeCountsCount += 1;
  }
  if (maxStructsMembersOfTypeSamplerCount > 0) {
    structMemberTypeCounts[structMemberTypeCountsCount].type            = (VkDescriptorType)RED_STRUCT_MEMBER_TYPE_SAMPLER;
    structMemberTypeCounts[structMemberTypeCountsCount].descriptorCount = maxStructsMembersOfTypeSamplerCount;
    structMemberTypeCountsCount += 1;
  }
  if (maxStructsMembersOfTypeTextureROCount > 0) {
    structMemberTypeCounts[structMemberTypeCountsCount].type            = (VkDescriptorType)RED_STRUCT_MEMBER_TYPE_TEXTURE_RO;
    structMemberTypeCounts[structMemberTypeCountsCount].descriptorCount = maxStructsMembersOfTypeTextureROCount;
    structMemberTypeCountsCount += 1;
  }
  if (maxStructsMembersOfTypeTextureRWCount > 0) {
    structMemberTypeCounts[structMemberTypeCountsCount].type            = (VkDescriptorType)RED_STRUCT_MEMBER_TYPE_TEXTURE_RW;
    structMemberTypeCounts[structMemberTypeCountsCount].descriptorCount = maxStructsMembersOfTypeTextureRWCount;
    structMemberTypeCountsCount += 1;
  }

  VkDescriptorPoolCreateInfo info;
  info.sType         = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
  info.pNext         = NULL;
  info.flags         = 0;
  info.maxSets       = maxStructsCount;
  info.poolSizeCount = structMemberTypeCountsCount;
  info.pPoolSizes    = structMemberTypeCounts;
  redInternalSetStatus(ctx, gpu, outStatuses, 226, vk->vkCreateDescriptorPool((VkDevice)gpu, &info, NULL, (VkDescriptorPool *)outStructsMemory), procedureId, optionalFile, optionalLine, optionalUserData);

  if (ctx->internal.debugUtilsMessengerCallback != NULL && handleName != NULL) {
    if (outStructsMemory[0] != NULL) {
      VkDebugUtilsObjectNameInfoEXT info;
      info.sType        = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT;
      info.pNext        = NULL;
      info.objectType   = (VkObjectType)RED_HANDLE_TYPE_STRUCTS_MEMORY;
      info.objectHandle = (uint64_t)outStructsMemory[0];
      info.pObjectName  = handleName;
      redInternalSetStatus(ctx, gpu, outStatuses, 227, vk->vkSetDebugUtilsObjectNameEXT((VkDevice)gpu, &info), procedureId, optionalFile, optionalLine, optionalUserData);
    }
  }
}

static inline void redInlineStructsMemorySuballocateStructs(RedContext context, RedHandleGpu gpu, const char ** handleNames, RedHandleStructsMemory structsMemory, unsigned structsDeclarationsCount, const RedHandleStructDeclaration * structsDeclarations, RedHandleStruct * outStructs, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData) {
  const RedProcedureId procedureId = RED_PROCEDURE_ID_redStructsMemorySuballocateStructs;

  RedInternalContext ctx = (RedInternalContext)context;
  RedInternalInstanceProcedureAddresses * vk = ctx->internal.instanceProcedureAddresses;

  VkDescriptorSetAllocateInfo info;
  info.sType              = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
  info.pNext              = NULL;
  info.descriptorPool     = (VkDescriptorPool)structsMemory;
  info.descriptorSetCount = structsDeclarationsCount;
  info.pSetLayouts        = (const VkDescriptorSetLayout *)structsDeclarations;
  redInternalSetStatus(ctx, gpu, outStatuses, 228, vk->vkAllocateDescriptorSets((VkDevice)gpu, &info, (VkDescriptorSet *)outStructs), procedureId, optionalFile, optionalLine, optionalUserData);

  if (ctx->internal.debugUtilsMessengerCallback != NULL && handleNames != NULL) {
    for (unsigned i = 0; i < structsDeclarationsCount; i += 1) {
      if (outStructs[i] != NULL && handleNames[i] != NULL) {
        VkDebugUtilsObjectNameInfoEXT info;
        info.sType        = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT;
        info.pNext        = NULL;
        info.objectType   = (VkObjectType)RED_HANDLE_TYPE_STRUCT;
        info.objectHandle = (uint64_t)outStructs[i];
        info.pObjectName  = handleNames[i];
        redInternalSetStatus(ctx, gpu, outStatuses, 229, vk->vkSetDebugUtilsObjectNameEXT((VkDevice)gpu, &info), procedureId, optionalFile, optionalLine, optionalUserData);
      }
    }
  }
}

static inline void redInlineStructsMemoryReset(RedContext context, RedHandleGpu gpu, RedHandleStructsMemory structsMemory, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData) {
  const RedProcedureId procedureId = RED_PROCEDURE_ID_redStructsMemoryReset;

  RedInternalContext ctx = (RedInternalContext)context;
  RedInternalInstanceProcedureAddresses * vk = ctx->internal.instanceProcedureAddresses;

  redInternalSetStatus(ctx, gpu, outStatuses, 230, vk->vkResetDescriptorPool((VkDevice)gpu, (VkDescriptorPool)structsMemory, 0), procedureId, optionalFile, optionalLine, optionalUserData);
}

static inline void redInlineStructsMemoryFree(RedContext context, RedHandleGpu gpu, RedHandleStructsMemory structsMemory, const char * optionalFile, int optionalLine, void * optionalUserData) {
//  const RedProcedureId procedureId = RED_PROCEDURE_ID_redStructsMemoryFree;

  RedInternalContext ctx = (RedInternalContext)context;
  RedInternalInstanceProcedureAddresses * vk = ctx->internal.instanceProcedureAddresses;

  vk->vkDestroyDescriptorPool((VkDevice)gpu, (VkDescriptorPool)structsMemory, NULL);
}

static inline void redInlineStructsSet(RedContext context, RedHandleGpu gpu, unsigned structsMembersCount, const RedStructMember * structsMembers, const char * optionalFile, int optionalLine, void * optionalUserData) {
//  const RedProcedureId procedureId = RED_PROCEDURE_ID_redStructsSet;

  RedInternalContext ctx = (RedInternalContext)context;
  RedInternalInstanceProcedureAddresses * vk = ctx->internal.instanceProcedureAddresses;

  vk->vkUpdateDescriptorSets((VkDevice)gpu, structsMembersCount, (const VkWriteDescriptorSet *)structsMembers, 0, NULL);
}

// Create

#if defined(REDGPU_OS_WINDOWS)
void * __stdcall LoadLibraryA(const char * lpLibFileName);
void * __stdcall GetProcAddress(void * hModule, const char * lpProcName);
#elif defined(REDGPU_OS_ANDROID)
// NOTE(Constantine): Nothing, intentionally.
#else
PFN_vkVoidFunction vkGetInstanceProcAddr(VkInstance instance, const char * name);
#endif

#if REDGPU_COMPILE_SWITCH == REDGPU_COMPILE_SWITCH_DEBUG
static inline RedBool32 redDebugLibraryInternalFormatIsValid(unsigned formatIndex) {
  if      (formatIndex == (unsigned)RED_FORMAT_R_8_UINT_TO_FLOAT_0_1)                          { return 1; }
  else if (formatIndex == (unsigned)RED_FORMAT_R_8_UINT)                                       { return 1; }
  else if (formatIndex == (unsigned)RED_FORMAT_R_8_SINT)                                       { return 1; }
  else if (formatIndex == (unsigned)RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1)                 { return 1; }
  else if (formatIndex == (unsigned)RED_FORMAT_RGBA_8_8_8_8_UINT_TO_FLOAT_0_1_GAMMA_CORRECTED) { return 1; }
  else if (formatIndex == (unsigned)RED_FORMAT_RGBA_8_8_8_8_UINT)                              { return 1; }
  else if (formatIndex == (unsigned)RED_FORMAT_RGBA_8_8_8_8_SINT)                              { return 1; }
  else if (formatIndex == (unsigned)RED_FORMAT_PRESENT_BGRA_8_8_8_8_UINT_TO_FLOAT_0_1)         { return 1; }
  else if (formatIndex == (unsigned)RED_FORMAT_R_16_UINT)                                      { return 1; }
  else if (formatIndex == (unsigned)RED_FORMAT_R_16_SINT)                                      { return 1; }
  else if (formatIndex == (unsigned)RED_FORMAT_R_16_FLOAT)                                     { return 1; }
  else if (formatIndex == (unsigned)RED_FORMAT_RGBA_16_16_16_16_UINT)                          { return 1; }
  else if (formatIndex == (unsigned)RED_FORMAT_RGBA_16_16_16_16_SINT)                          { return 1; }
  else if (formatIndex == (unsigned)RED_FORMAT_RGBA_16_16_16_16_FLOAT)                         { return 1; }
  else if (formatIndex == (unsigned)RED_FORMAT_R_32_UINT)                                      { return 1; }
  else if (formatIndex == (unsigned)RED_FORMAT_R_32_SINT)                                      { return 1; }
  else if (formatIndex == (unsigned)RED_FORMAT_R_32_FLOAT)                                     { return 1; }
  else if (formatIndex == (unsigned)RED_FORMAT_RGBA_32_32_32_32_UINT)                          { return 1; }
  else if (formatIndex == (unsigned)RED_FORMAT_RGBA_32_32_32_32_SINT)                          { return 1; }
  else if (formatIndex == (unsigned)RED_FORMAT_RGBA_32_32_32_32_FLOAT)                         { return 1; }
  else if (formatIndex == (unsigned)RED_FORMAT_DEPTH_16_UINT_TO_FLOAT_0_1)                     { return 1; }
  else if (formatIndex == (unsigned)RED_FORMAT_DEPTH_32_FLOAT)                                 { return 1; }
  else if (formatIndex == (unsigned)RED_FORMAT_DEPTH_24_UINT_TO_FLOAT_0_1_STENCIL_8_UINT)      { return 1; }
  else if (formatIndex == (unsigned)RED_FORMAT_DEPTH_32_FLOAT_STENCIL_8_UINT)                  { return 1; }
  return 0;
}
#endif

#if REDGPU_COMPILE_SWITCH == REDGPU_COMPILE_SWITCH_NVTX
RedBool32 REDGPU_CALLING_CONVENTION redNvLibraryCallback(RedDebugCallbackSeverity severity, RedDebugCallbackTypeBitflags types, const RedDebugCallbackData * data, RedContext context) {
  return 0;
}
#endif

#if REDGPU_COMPILE_SWITCH == REDGPU_COMPILE_SWITCH_DEBUG
RedBool32 REDGPU_CALLING_CONVENTION redDebugLibraryCallback(RedDebugCallbackSeverity severity, RedDebugCallbackTypeBitflags types, const RedDebugCallbackData * data, RedContext context) {
  std::lock_guard<std::mutex> __debugPrintGuardAndSerializeMutexScopeGuard(__REDGPU_DEBUG_GLOBAL_3f5f84a81695ef9521b544bdc3d7d73cbb4aac3c18ce99fd19bdfd306996103e_debugPrintGuardAndSerializeMutex);
  RedInternalContext ctx = (RedInternalContext)context;
  const unsigned statusCode    = data->_2;
  const char *   messageIdName = data->messageIdName == NULL ? "" : data->messageIdName;
  const char *   message       = data->message       == NULL ? "" : data->message;
  if (ctx->internal.cold->debugArrayPrintWasEverCalled == 1 && strcmp(messageIdName, "VUID-vkQueuePresentKHR-pWaitSemaphores-03268") == 0) {
    return 0;
  }
  REDGPU_PRINTF("[REDGPU DEBUG LIBRARY][%s]: %s (statusCode: %d)\n", messageIdName, message, statusCode);
  for (unsigned i = 0; i < data->handleInfosCount; i += 1) {
    REDGPU_PRINTF("[REDGPU DEBUG LIBRARY][%s]: handleInfos[%d].handleType: %s\n", messageIdName, i, REDGPU_PRINT_HANDLE_TYPE(data->handleInfos[i].handleType));
    REDGPU_PRINTF("[REDGPU DEBUG LIBRARY][%s]: handleInfos[%d].handle:     %p\n", messageIdName, i, REDGPU_PRINT_POINTER(data->handleInfos[i].handle));
    REDGPU_PRINTF("[REDGPU DEBUG LIBRARY][%s]: handleInfos[%d].handleName: \"%s\"\n", messageIdName, i, data->handleInfos[i].handleName == NULL ? "" : data->handleInfos[i].handleName);
  }
  REDGPU_FFLUSH(stdout);
  return 0;
}
#endif

static inline void redInlineDestroyContext(RedContext context, const char * optionalFile, int optionalLine, void * optionalUserData) {
  const RedProcedureId procedureId = RED_PROCEDURE_ID_redDestroyContext;

  if (context == NULL) {
    return;
  }

  RedInternalContext ctx = (RedInternalContext)context;

  RedInternalContextColdData            * cold = ctx->internal.cold;
  RedInternalInstanceProcedureAddresses * vk   = ctx->internal.instanceProcedureAddresses;

  const unsigned gpusCount = ctx->gpusCount;

  RedTypeProcedureFree       customFree       = cold->customFree;
  RedTypeProcedureFreeTagged customFreeTagged = cold->customFreeTagged;

#if REDGPU_COMPILE_SWITCH == REDGPU_COMPILE_SWITCH_DEBUG
  for (unsigned i = 0; i < gpusCount; i += 1) {
    RedInternalGpuInfo * iGpuInfo = &cold->gpus[i];
    RedHandleGpu         gpu      = ctx->internal.hotGpuHandles[i];

    if (iGpuInfo->debugArrayFeatureIsSupported == 1) {
      if (iGpuInfo->debugArray.array.handle != NULL) {
        redDebugInternalDestroyArray(ctx, gpu, iGpuInfo->debugArray.array.handle);
      }
      if (iGpuInfo->debugArray.memory.handle != NULL) {
        redDebugInternalMemoryFree(ctx, gpu, iGpuInfo->debugArray.memory.handle);
      }
      if (iGpuInfo->debugArrayGpuToCpuEvent != NULL) {
        redDebugInternalDestroyEvent(ctx, gpu, iGpuInfo->debugArrayGpuToCpuEvent);
      }
      if (iGpuInfo->debugArrayCpuToGpuEvent != NULL) {
        redDebugInternalDestroyEvent(ctx, gpu, iGpuInfo->debugArrayCpuToGpuEvent);
      }
      if (iGpuInfo->debugArrayCpuSignal != NULL) {
        redDebugInternalDestroyCpuSignal(ctx, gpu, iGpuInfo->debugArrayCpuSignal);
      }
      RedDebugInternalArray defaults = {0};
      iGpuInfo->debugArray              = defaults;
      iGpuInfo->debugArrayGpuToCpuEvent = NULL;
      iGpuInfo->debugArrayCpuToGpuEvent = NULL;
      iGpuInfo->debugArrayCpuSignal     = NULL;
    }
  }
#endif

  for (unsigned i = 0; i < gpusCount; i += 1) {
    VkDevice gpu = (VkDevice)ctx->internal.hotGpuHandles[i];

    if (gpu != NULL) {
      vk->vkDestroyDevice(gpu, NULL);
    }
  }

#if defined(REDGPU_OS_ANDROID)
  if (ctx->internal.debugUtilsMessengerCallback != NULL && cold->vkDebugReportCallback != NULL) {
    vk->vkDestroyDebugReportCallbackEXT((VkInstance)ctx->contextHandle, cold->vkDebugReportCallback, NULL);
  }
#else
  if (ctx->internal.debugUtilsMessengerCallback != NULL && cold->vkDebugUtilsMessenger != NULL) {
    vk->vkDestroyDebugUtilsMessengerEXT((VkInstance)ctx->contextHandle, cold->vkDebugUtilsMessenger, NULL);
  }
#endif

  if ((VkInstance)ctx->contextHandle != NULL) {
    vk->vkDestroyInstance((VkInstance)ctx->contextHandle, NULL);
  }

  for (unsigned i = 0; i < gpusCount; i += 1) {
    RedGpuInfo * eGpuInfo = &ctx->gpus[i];

    struct SomePreviousOptionalInfo {
      unsigned extension;
      void *   next;
    };
    struct SomePreviousOptionalInfo * optionalInfo = (struct SomePreviousOptionalInfo *)eGpuInfo->optionalInfo;

    while (optionalInfo != NULL) {
      void * next = optionalInfo->next;
      if (optionalInfo->extension == RED_GPU_INFO_OPTIONAL_INFO_ARRAY_TIMESTAMP) {
        RedGpuInfoOptionalInfoArrayTimestamp * info = (RedGpuInfoOptionalInfoArrayTimestamp *)optionalInfo;
        REDGPU_INTERNAL_ADAPTIVE_FREE_STABLE(info->queuesPrecisionBits64BitTicksCount, 103);
      }
      REDGPU_INTERNAL_ADAPTIVE_FREE_STABLE(optionalInfo, 51);
      optionalInfo = (struct SomePreviousOptionalInfo *)next;
    }
  }

  for (unsigned i = 0; i < gpusCount; i += 1) {
    RedGpuInfo * eGpuInfo = &ctx->gpus[i];

    REDGPU_INTERNAL_ADAPTIVE_FREE_STABLE(eGpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered, 52);
    REDGPU_INTERNAL_ADAPTIVE_FREE_STABLE(eGpuInfo->imageFormatsLimitsImageDimensions3D, 53);
    REDGPU_INTERNAL_ADAPTIVE_FREE_STABLE(eGpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered, 54);
    REDGPU_INTERNAL_ADAPTIVE_FREE_STABLE(eGpuInfo->imageFormatsLimitsImageDimensions2DMultisample, 55);
    REDGPU_INTERNAL_ADAPTIVE_FREE_STABLE(eGpuInfo->imageFormatsLimitsImageDimensions2D, 56);
    REDGPU_INTERNAL_ADAPTIVE_FREE_STABLE(eGpuInfo->imageFormatsLimitsImageDimensions1D, 57);
    REDGPU_INTERNAL_ADAPTIVE_FREE_STABLE(eGpuInfo->imageFormatsFeatures, 58);
    REDGPU_INTERNAL_ADAPTIVE_FREE_STABLE(eGpuInfo->memoryHeaps, 59);
    REDGPU_INTERNAL_ADAPTIVE_FREE_STABLE(eGpuInfo->memoryTypes, 60);
  }

  for (unsigned i = 0; i < gpusCount; i += 1) {
    RedGpuInfo * eGpuInfo = &ctx->gpus[i];

    REDGPU_INTERNAL_ADAPTIVE_FREE_STABLE(eGpuInfo->queuesCanCompute, 61);
    REDGPU_INTERNAL_ADAPTIVE_FREE_STABLE(eGpuInfo->queuesCanDraw, 62);
    REDGPU_INTERNAL_ADAPTIVE_FREE_STABLE(eGpuInfo->queuesCanCopy, 63);
  }

  for (unsigned i = 0; i < gpusCount; i += 1) {
    RedGpuInfo * eGpuInfo = &ctx->gpus[i];

    REDGPU_INTERNAL_ADAPTIVE_FREE_STABLE(eGpuInfo->queuesCopyLimits, 64);
    REDGPU_INTERNAL_ADAPTIVE_FREE_STABLE(eGpuInfo->queuesFamilyIndex, 65);
    REDGPU_INTERNAL_ADAPTIVE_FREE_STABLE(eGpuInfo->queues, 66);
  }

  for (unsigned i = 0; i < gpusCount; i += 1) {
    RedInternalGpuInfo * iGpuInfo = &cold->gpus[i];

    for (unsigned j = 0; j < iGpuInfo->queueFamilyDataCount; j += 1) {
      RedInternalQueueFamilyData * queueFamilyData = &iGpuInfo->queueFamilyData[j];

      REDGPU_INTERNAL_ADAPTIVE_FREE_STABLE(queueFamilyData->queues, 67);
    }
  }

  for (unsigned i = 0; i < gpusCount; i += 1) {
    RedInternalGpuInfo * iGpuInfo = &cold->gpus[i];

    REDGPU_INTERNAL_ADAPTIVE_FREE_STABLE(iGpuInfo->vkDeviceQueueCreateInfo, 68);
  }

  for (unsigned i = 0; i < gpusCount; i += 1) {
    RedInternalGpuInfo * iGpuInfo = &cold->gpus[i];

    for (unsigned j = 0; j < iGpuInfo->queueFamilyDataCount; j += 1) {
      RedInternalQueueFamilyData * queueFamilyData = &iGpuInfo->queueFamilyData[j];

      REDGPU_INTERNAL_ADAPTIVE_FREE_STABLE(queueFamilyData->queuesPriorities, 69);
    }

    REDGPU_INTERNAL_ADAPTIVE_FREE_STABLE(iGpuInfo->queueFamilyData, 70);
  }

  for (unsigned i = 0; i < gpusCount; i += 1) {
    RedInternalGpuInfo * iGpuInfo = &cold->gpus[i];

    REDGPU_INTERNAL_ADAPTIVE_FREE_STABLE(iGpuInfo->vkQueueFamilyIndexes, 71);
  }

  for (unsigned i = 0; i < gpusCount; i += 1) {
    RedInternalGpuInfo * iGpuInfo = &cold->gpus[i];

    REDGPU_INTERNAL_ADAPTIVE_FREE_STABLE(iGpuInfo->vkQueueFamilyProperties, 72);
  }

  for (unsigned i = 0; i < gpusCount; i += 1) {
    RedInternalGpuInfo * iGpuInfo = &cold->gpus[i];

    REDGPU_INTERNAL_ADAPTIVE_FREE_STABLE(iGpuInfo->vkDeviceExtensionProperties, 73);
  }

  REDGPU_INTERNAL_ADAPTIVE_FREE_STABLE(cold->gpus, 74);
  REDGPU_INTERNAL_ADAPTIVE_FREE_STABLE(ctx->internal.gpuExtensionBitflags, 75);
  REDGPU_INTERNAL_ADAPTIVE_FREE_STABLE(ctx->internal.hotGpuHandles, 76);
  REDGPU_INTERNAL_ADAPTIVE_FREE_STABLE(ctx->internal.gpuProcedureAddresses, 77);
  REDGPU_INTERNAL_ADAPTIVE_FREE_STABLE(ctx->gpusStatuses, 78);
  REDGPU_INTERNAL_ADAPTIVE_FREE_STABLE(ctx->gpus, 79);
  REDGPU_INTERNAL_ADAPTIVE_FREE_STABLE(cold->vkPhysicalDevices, 80);
  REDGPU_INTERNAL_ADAPTIVE_FREE_STABLE(cold->vkInstanceExtensionProperties, 81);
  REDGPU_INTERNAL_ADAPTIVE_FREE_STABLE(cold->vkInstanceLayerProperties, 82);

  //REDGPU_INTERNAL_ADAPTIVE_FREE_STABLE(ctx->internal.pphi, 83);
  REDGPU_INTERNAL_ADAPTIVE_FREE_STABLE(ctx->internal.instanceProcedureAddresses, 84);
  REDGPU_INTERNAL_ADAPTIVE_FREE_STABLE(ctx->internal.cold, 85);
  REDGPU_INTERNAL_ADAPTIVE_FREE_STABLE(ctx, 86);
}

#ifdef __clang__
static inline __attribute__((always_inline))
#else
static inline
#endif
void redInlineCreateContext(RedTypeProcedureMalloc customMalloc, RedTypeProcedureFree customFree, RedTypeProcedureMallocTagged customMallocTagged, RedTypeProcedureFreeTagged customFreeTagged, RedTypeProcedureDebugCallback debugCallback, RedSdkVersion sdkVersion, unsigned sdkExtensionsCount, const unsigned * sdkExtensions, const char * optionalProgramName, unsigned optionalProgramVersion, const char * optionalEngineName, unsigned optionalEngineVersion, const void * optionalSettings, RedContext * outContext, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData) {
  const RedProcedureId procedureId = RED_PROCEDURE_ID_redCreateContext;

#if REDGPU_COMPILE_SWITCH == REDGPU_COMPILE_SWITCH_DEBUG
  RedBool32   __redgpuDebugStatusesWereNotSetByTheUser = 0;
  RedStatuses __redgpuDebugStatuses = {0};
  __redgpuDebugStatuses.statusProcedureId      = procedureId;
  __redgpuDebugStatuses.statusFile             = optionalFile;
  __redgpuDebugStatuses.statusLine             = optionalLine;
  __redgpuDebugStatuses.statusErrorProcedureId = procedureId;
  __redgpuDebugStatuses.statusErrorFile        = optionalFile;
  __redgpuDebugStatuses.statusErrorLine        = optionalLine;
  if (outStatuses == NULL) {
    __redgpuDebugStatusesWereNotSetByTheUser = 1;
    outStatuses = &__redgpuDebugStatuses;
  }
#endif

#ifdef REDGPU_OS_LINUX
#if REDGPU_COMPILE_SWITCH == REDGPU_COMPILE_SWITCH_DEBUG
  if (0) {
    setenv("VK_LAYER_PATH", "/home/linuxbrew/RedGpuSDK/sdk/1.1.106.0/x86_64/etc/explicit_layer.d/", 0);
    dlopen("/home/linuxbrew/RedGpuSDK/sdk/1.1.106.0/x86_64/lib/libVkLayer_khronos_validation.so", RTLD_LAZY);
  } else if (sdkVersion == RED_SDK_VERSION_1_0_135) {
    setenv("VK_LAYER_PATH", "/home/linuxbrew/RedGpuSDK/sdk/1.2.135.0/x86_64/etc/vulkan/explicit_layer.d/", 0);
    dlopen("/home/linuxbrew/RedGpuSDK/sdk/1.2.135.0/x86_64/lib/libVkLayer_khronos_validation.so", RTLD_LAZY);
  }
#endif
#endif

#if REDGPU_COMPILE_SWITCH == REDGPU_COMPILE_SWITCH_NVTX
  debugCallback = redNvLibraryCallback;
#endif

#if REDGPU_COMPILE_SWITCH == REDGPU_COMPILE_SWITCH_DEBUG && !defined(REDGPU_OS_ANDROID)
  debugCallback = redDebugLibraryCallback;
#endif

  RedBool32  settings0SkipCheckingContextLayers        = 0;
  RedBool32  settings0SkipCheckingContextExtensions    = 0;
  RedBool32  settings0GpusExposeOnlyOneQueue           = 0;
  RedBool32  settingsManagedOutputEnable               = 0;
  RedBool32  settingsContextFromVkPassed               = 0;
  uint64_t   settingsContextFromVkInstance             = 0;
  unsigned   settingsContextFromVkPhysicalDevicesCount = 0;
  uint64_t * settingsContextFromVkPhysicalDevices      = 0;
  uint64_t * settingsContextFromVkDevices              = 0;
  void *     settingsContextFromVkGetInstanceProcAddr  = 0;
  RedBool32  settings1ExposeOnlyOneGpu                 = 0;

  {
    void * settingsToIterate = (void *)optionalSettings;
    while (settingsToIterate != NULL) {
      const RedContextOptionalSettingsIterator * settings = (const RedContextOptionalSettingsIterator *)settingsToIterate;
      if (settings->settings == RED_CONTEXT_OPTIONAL_SETTINGS_0) {
        const RedContextOptionalSettings0 * s = (const RedContextOptionalSettings0 *)settingsToIterate;
        settings0SkipCheckingContextLayers        = s->skipCheckingContextLayers;
        settings0SkipCheckingContextExtensions    = s->skipCheckingContextExtensions;
        settings0GpusExposeOnlyOneQueue           = s->gpusExposeOnlyOneQueue;
      } else if (settings->settings == RED_CONTEXT_OPTIONAL_SETTINGS_CONTEXT_FROM_VK) {
        const RedContextOptionalSettingsContextFromVk * s = (const RedContextOptionalSettingsContextFromVk *)settingsToIterate;
        settingsContextFromVkInstance             = s->instance;
        settingsContextFromVkPhysicalDevicesCount = s->physicalDevicesCount;
        settingsContextFromVkPhysicalDevices      = s->physicalDevices;
        settingsContextFromVkDevices              = s->devices;
        settingsContextFromVkPassed               = 1;
      } else if (settings->settings == RED_CONTEXT_OPTIONAL_SETTINGS_CONTEXT_FROM_VK_GET_INSTANCE_PROC_ADDR) {
        const RedContextOptionalSettingsContextFromVkGetInstanceProcAddr * s = (const RedContextOptionalSettingsContextFromVkGetInstanceProcAddr *)settingsToIterate;
        settingsContextFromVkGetInstanceProcAddr  = s->getInstanceProcAddr;
      } else if (settings->settings == RED_CONTEXT_OPTIONAL_SETTINGS_CREATE_CONTEXT_PERFORMANCE) {
        const RedContextOptionalSettingsCreateContextPerformance * s = (const RedContextOptionalSettingsCreateContextPerformance *)settingsToIterate;
        settings1ExposeOnlyOneGpu                 = s->exposeOnlyOneGpu;
      }
      void * nextSettingsToIterate = (void *)settings->next;
      settingsToIterate = nextSettingsToIterate;
    }
  }

#if REDGPU_COMPILE_SWITCH == REDGPU_COMPILE_SWITCH_DEBUG
  {
    std::lock_guard<std::mutex> __debugPrintGuardAndSerializeMutexScopeGuard(__REDGPU_DEBUG_GLOBAL_3f5f84a81695ef9521b544bdc3d7d73cbb4aac3c18ce99fd19bdfd306996103e_debugPrintGuardAndSerializeMutex);
    if (optionalFile != NULL) {
      REDGPU_PRINTF("[REDGPU DEBUG LIBRARY][RED_PROCEDURE_ID_redCreateContext][%s:%d]: RED_CONTEXT_OPTIONAL_SETTINGS: optionalProgramName:    \"%s\"\n", optionalFile, optionalLine, optionalProgramName == NULL ? "" : optionalProgramName);
      REDGPU_PRINTF("[REDGPU DEBUG LIBRARY][RED_PROCEDURE_ID_redCreateContext][%s:%d]: RED_CONTEXT_OPTIONAL_SETTINGS: optionalProgramVersion: %d\n",     optionalFile, optionalLine, optionalProgramVersion);
      REDGPU_PRINTF("[REDGPU DEBUG LIBRARY][RED_PROCEDURE_ID_redCreateContext][%s:%d]: RED_CONTEXT_OPTIONAL_SETTINGS: optionalEngineName:     \"%s\"\n", optionalFile, optionalLine, optionalEngineName  == NULL ? "" : optionalEngineName);
      REDGPU_PRINTF("[REDGPU DEBUG LIBRARY][RED_PROCEDURE_ID_redCreateContext][%s:%d]: RED_CONTEXT_OPTIONAL_SETTINGS: optionalEngineVersion:  %d\n",     optionalFile, optionalLine, optionalEngineVersion);
      REDGPU_PRINTF("[REDGPU DEBUG LIBRARY][RED_PROCEDURE_ID_redCreateContext][%s:%d]: RED_CONTEXT_OPTIONAL_SETTINGS_0: skipCheckingContextLayers:     %d\n", optionalFile, optionalLine, settings0SkipCheckingContextLayers);
      REDGPU_PRINTF("[REDGPU DEBUG LIBRARY][RED_PROCEDURE_ID_redCreateContext][%s:%d]: RED_CONTEXT_OPTIONAL_SETTINGS_0: skipCheckingContextExtensions: %d\n", optionalFile, optionalLine, settings0SkipCheckingContextExtensions);
      REDGPU_PRINTF("[REDGPU DEBUG LIBRARY][RED_PROCEDURE_ID_redCreateContext][%s:%d]: RED_CONTEXT_OPTIONAL_SETTINGS_0: gpusExposeOnlyOneQueue:        %d\n", optionalFile, optionalLine, settings0GpusExposeOnlyOneQueue);
    } else {
      REDGPU_PRINTF("[REDGPU DEBUG LIBRARY][RED_PROCEDURE_ID_redCreateContext]: RED_CONTEXT_OPTIONAL_SETTINGS: optionalProgramName:    \"%s\"\n", optionalProgramName == NULL ? "" : optionalProgramName);
      REDGPU_PRINTF("[REDGPU DEBUG LIBRARY][RED_PROCEDURE_ID_redCreateContext]: RED_CONTEXT_OPTIONAL_SETTINGS: optionalProgramVersion: %d\n",     optionalProgramVersion);
      REDGPU_PRINTF("[REDGPU DEBUG LIBRARY][RED_PROCEDURE_ID_redCreateContext]: RED_CONTEXT_OPTIONAL_SETTINGS: optionalEngineName:     \"%s\"\n", optionalEngineName  == NULL ? "" : optionalEngineName);
      REDGPU_PRINTF("[REDGPU DEBUG LIBRARY][RED_PROCEDURE_ID_redCreateContext]: RED_CONTEXT_OPTIONAL_SETTINGS: optionalEngineVersion:  %d\n",     optionalEngineVersion);
      REDGPU_PRINTF("[REDGPU DEBUG LIBRARY][RED_PROCEDURE_ID_redCreateContext]: RED_CONTEXT_OPTIONAL_SETTINGS_0: skipCheckingContextLayers:     %d\n", settings0SkipCheckingContextLayers);
      REDGPU_PRINTF("[REDGPU DEBUG LIBRARY][RED_PROCEDURE_ID_redCreateContext]: RED_CONTEXT_OPTIONAL_SETTINGS_0: skipCheckingContextExtensions: %d\n", settings0SkipCheckingContextExtensions);
      REDGPU_PRINTF("[REDGPU DEBUG LIBRARY][RED_PROCEDURE_ID_redCreateContext]: RED_CONTEXT_OPTIONAL_SETTINGS_0: gpusExposeOnlyOneQueue:        %d\n", settings0GpusExposeOnlyOneQueue);
    }
  }
#endif

#if REDGPU_COMPILE_SWITCH == REDGPU_COMPILE_SWITCH_DEBUG
  settings0SkipCheckingContextLayers     = 0;
  settings0SkipCheckingContextExtensions = 0;
#endif

  RedInternalContext ctx = (RedInternalContext)REDGPU_INTERNAL_ADAPTIVE_MALLOC_STABLE(sizeof(ctx[0]), 1);
  if (ctx == NULL) {
    redInternalSetStatus(ctx, NULL, outStatuses, 235, (VkResult)RED_STATUS_ERROR_OUT_OF_CPU_MEMORY, procedureId, optionalFile, optionalLine, optionalUserData);
    outContext[0] = (RedContext)ctx;
    return;
  }
  redInternalMemoryClear(ctx, sizeof(ctx[0]));

  ctx->internal.cold = (RedInternalContextColdData *)REDGPU_INTERNAL_ADAPTIVE_MALLOC_STABLE(sizeof(ctx->internal.cold[0]), 2);
  if (ctx->internal.cold == NULL) {
    REDGPU_INTERNAL_ADAPTIVE_FREE_STABLE(ctx, 87);
    ctx = NULL;
    redInternalSetStatus(ctx, NULL, outStatuses, 236, (VkResult)RED_STATUS_ERROR_OUT_OF_CPU_MEMORY, procedureId, optionalFile, optionalLine, optionalUserData);
    outContext[0] = (RedContext)ctx;
    return;
  }
  redInternalMemoryClear(ctx->internal.cold, sizeof(ctx->internal.cold[0]));

  ctx->internal.instanceProcedureAddresses = (RedInternalInstanceProcedureAddresses *)REDGPU_INTERNAL_ADAPTIVE_MALLOC_STABLE(sizeof(ctx->internal.instanceProcedureAddresses[0]), 3);
  if (ctx->internal.instanceProcedureAddresses == NULL) {
    REDGPU_INTERNAL_ADAPTIVE_FREE_STABLE(ctx->internal.cold, 88);
    REDGPU_INTERNAL_ADAPTIVE_FREE_STABLE(ctx, 89);
    ctx = NULL;
    redInternalSetStatus(ctx, NULL, outStatuses, 237, (VkResult)RED_STATUS_ERROR_OUT_OF_CPU_MEMORY, procedureId, optionalFile, optionalLine, optionalUserData);
    outContext[0] = (RedContext)ctx;
    return;
  }
  redInternalMemoryClear(ctx->internal.instanceProcedureAddresses, sizeof(ctx->internal.instanceProcedureAddresses[0]));

  RedInternalContextColdData            * cold = ctx->internal.cold;
  RedInternalInstanceProcedureAddresses * vk   = ctx->internal.instanceProcedureAddresses;

  cold->customMalloc       = customMalloc;
  cold->customFree         = customFree;
  cold->customMallocTagged = customMallocTagged;
  cold->customFreeTagged   = customFreeTagged;

#if defined(REDGPU_OS_WINDOWS)
  void * vulkan_1_dll = LoadLibraryA("C:\\Windows\\System32\\vulkan-1.dll");

  if (vulkan_1_dll == NULL)
  {
    REDGPU_CREATE_CONTEXT_DESTROY_CTX_THEN_SET_CTX_TO_NULL_THEN_SET_STATUS_ERROR_THEN_GOTO_EXIT(NULL, 379, RED_STATUS_ERROR_INITIALIZATION_FAILED);
  }

  PFN_vkGetInstanceProcAddr vkGetInstanceProcAddr = (PFN_vkGetInstanceProcAddr)GetProcAddress(vulkan_1_dll, "vkGetInstanceProcAddr");

  if (vkGetInstanceProcAddr == NULL)
  {
    REDGPU_CREATE_CONTEXT_DESTROY_CTX_THEN_SET_CTX_TO_NULL_THEN_SET_STATUS_ERROR_THEN_GOTO_EXIT(NULL, 380, RED_STATUS_ERROR_INITIALIZATION_FAILED);
  }
#endif

#if defined(REDGPU_OS_ANDROID)
  PFN_vkGetInstanceProcAddr redInternalGetInstanceProcAddr = 0;
#else
  PFN_vkGetInstanceProcAddr redInternalGetInstanceProcAddr = vkGetInstanceProcAddr;
#endif
  if (settingsContextFromVkGetInstanceProcAddr != NULL) {
    redInternalGetInstanceProcAddr = (PFN_vkGetInstanceProcAddr)settingsContextFromVkGetInstanceProcAddr;
  }

  vk->vkEnumerateInstanceLayerProperties     = (PFN_vkEnumerateInstanceLayerProperties)redInternalGetInstanceProcAddr(NULL, "vkEnumerateInstanceLayerProperties");
  vk->vkEnumerateInstanceExtensionProperties = (PFN_vkEnumerateInstanceExtensionProperties)redInternalGetInstanceProcAddr(NULL, "vkEnumerateInstanceExtensionProperties");
  vk->vkCreateInstance                       = (PFN_vkCreateInstance)redInternalGetInstanceProcAddr(NULL, "vkCreateInstance");
  if (vk->vkEnumerateInstanceLayerProperties     == NULL ||
      vk->vkEnumerateInstanceExtensionProperties == NULL ||
      vk->vkCreateInstance                       == NULL)
  {
    REDGPU_CREATE_CONTEXT_DESTROY_CTX_THEN_SET_CTX_TO_NULL_THEN_SET_STATUS_ERROR_THEN_GOTO_EXIT(NULL, 10, RED_STATUS_ERROR_INITIALIZATION_FAILED);
  }

#if REDGPU_COMPILE_SWITCH == REDGPU_COMPILE_SWITCH_NVTX
  RedBool32 requestedDebugMode        = 1;
  RedBool32 requestedValidationLayers = 0;
#else
  RedBool32 requestedDebugMode        = debugCallback == NULL ? 0 : 1;
  RedBool32 requestedValidationLayers = debugCallback == NULL ? 0 : 1;
#endif

  if (settings0SkipCheckingContextLayers == 0) {
  redInternalSetStatus(ctx, NULL, outStatuses, 238, vk->vkEnumerateInstanceLayerProperties(&cold->vkInstanceLayerPropertiesCount, NULL), procedureId, optionalFile, optionalLine, optionalUserData);
  REDGPU_CREATE_CONTEXT_ARRAY_MALLOC(NULL, 11, cold->vkInstanceLayerProperties, cold->vkInstanceLayerPropertiesCount, VkLayerProperties, 4);
  redInternalSetStatus(ctx, NULL, outStatuses, 239, vk->vkEnumerateInstanceLayerProperties(&cold->vkInstanceLayerPropertiesCount, cold->vkInstanceLayerProperties), procedureId, optionalFile, optionalLine, optionalUserData);

#if REDGPU_COMPILE_SWITCH == REDGPU_COMPILE_SWITCH_DEBUG
  {
    std::lock_guard<std::mutex> __debugPrintGuardAndSerializeMutexScopeGuard(__REDGPU_DEBUG_GLOBAL_3f5f84a81695ef9521b544bdc3d7d73cbb4aac3c18ce99fd19bdfd306996103e_debugPrintGuardAndSerializeMutex);
    for (unsigned i = 0; i < ctx->internal.cold->vkInstanceLayerPropertiesCount; i += 1) {
      VkLayerProperties * layerProperties = &ctx->internal.cold->vkInstanceLayerProperties[i];
      if (optionalFile != NULL) {
        REDGPU_PRINTF("[REDGPU DEBUG LIBRARY][RED_PROCEDURE_ID_redCreateContext][%s:%d]: Available context layer: %s\n", optionalFile, optionalLine, layerProperties->layerName);
      } else {
        REDGPU_PRINTF("[REDGPU DEBUG LIBRARY][RED_PROCEDURE_ID_redCreateContext]: Available context layer: %s\n", layerProperties->layerName);
      }
    }
  }
#endif

  if (requestedValidationLayers == 1) {
    if (0) {
      if (redInternalFindInstanceLayer(ctx, "VK_LAYER_LUNARG_standard_validation", procedureId, 1, optionalFile, optionalLine) == 0) {
        redInternalSetStatus(ctx, NULL, outStatuses, 240, (VkResult)RED_STATUS_ERROR_LAYER_IS_NOT_FOUND, procedureId, optionalFile, optionalLine, optionalUserData);
        requestedValidationLayers = 0;
      }
    } else if (sdkVersion == RED_SDK_VERSION_1_0_135) {
      if (redInternalFindInstanceLayer(ctx, "VK_LAYER_KHRONOS_validation", procedureId, 1, optionalFile, optionalLine) == 0) {
        redInternalSetStatus(ctx, NULL, outStatuses, 241, (VkResult)RED_STATUS_ERROR_LAYER_IS_NOT_FOUND, procedureId, optionalFile, optionalLine, optionalUserData);
        requestedValidationLayers = 0;
      }
    }
  }
  }

  RedBool32 requestedExtensions[16] = {0};
  for (unsigned i = 0; i < sdkExtensionsCount; i += 1) {
    unsigned ext = (unsigned)sdkExtensions[i];
    if (ext < 16) {
      requestedExtensions[ext] = 1;
    }
  }

  if (requestedExtensions[RED_SDK_EXTENSION_TREAT_ALL_MAPPABLE_MEMORY_AS_NON_COHERENT] == 1) {
    ctx->internal.contextExtensionBitflags |= RED_INTERNAL_CONTEXT_EXTENSION_BITFLAG_TREAT_ALL_MAPPABLE_MEMORY_AS_NON_COHERENT;
  }

  if (settingsManagedOutputEnable == 1) {
    ctx->internal.contextExtensionBitflags |= RED_INTERNAL_CONTEXT_EXTENSION_BITFLAG_MANAGED_OUTPUT;
  }

  if (settings0SkipCheckingContextExtensions == 0) {
    redInternalSetStatus(ctx, NULL, outStatuses, 242, vk->vkEnumerateInstanceExtensionProperties(NULL, &cold->vkInstanceExtensionPropertiesCount, NULL), procedureId, optionalFile, optionalLine, optionalUserData);
    REDGPU_CREATE_CONTEXT_ARRAY_MALLOC(NULL, 12, cold->vkInstanceExtensionProperties, cold->vkInstanceExtensionPropertiesCount, VkExtensionProperties, 5);
    redInternalSetStatus(ctx, NULL, outStatuses, 243, vk->vkEnumerateInstanceExtensionProperties(NULL, &cold->vkInstanceExtensionPropertiesCount, cold->vkInstanceExtensionProperties), procedureId, optionalFile, optionalLine, optionalUserData);

#if REDGPU_COMPILE_SWITCH == REDGPU_COMPILE_SWITCH_DEBUG
    {
      std::lock_guard<std::mutex> __debugPrintGuardAndSerializeMutexScopeGuard(__REDGPU_DEBUG_GLOBAL_3f5f84a81695ef9521b544bdc3d7d73cbb4aac3c18ce99fd19bdfd306996103e_debugPrintGuardAndSerializeMutex);
      for (unsigned i = 0; i < ctx->internal.cold->vkInstanceExtensionPropertiesCount; i += 1) {
        struct VkExtensionProperties * extensionProperties = &ctx->internal.cold->vkInstanceExtensionProperties[i];
        if (optionalFile != NULL) {
          REDGPU_PRINTF("[REDGPU DEBUG LIBRARY][RED_PROCEDURE_ID_redCreateContext][%s:%d]: Available context extension: %s\n", optionalFile, optionalLine, extensionProperties->extensionName);
        } else {
          REDGPU_PRINTF("[REDGPU DEBUG LIBRARY][RED_PROCEDURE_ID_redCreateContext]: Available context extension: %s\n", extensionProperties->extensionName);
        }
      }
    }
#endif

    if (requestedDebugMode == 1) {
#if defined(REDGPU_OS_ANDROID)
      if (redInternalFindInstanceExtension(ctx, "VK_EXT_debug_report", procedureId, 1, optionalFile, optionalLine) == 0) {
#else
      if (redInternalFindInstanceExtension(ctx, "VK_EXT_debug_utils", procedureId, 1, optionalFile, optionalLine) == 0) {
#endif
        redInternalSetStatus(ctx, NULL, outStatuses, 244, (VkResult)RED_STATUS_ERROR_EXTENSION_IS_NOT_FOUND, procedureId, optionalFile, optionalLine, optionalUserData);
        requestedDebugMode = 0;
      }
    }
    if (redInternalFindInstanceExtension(ctx, "VK_KHR_get_physical_device_properties2", procedureId, 1, optionalFile, optionalLine) == 0) {
      REDGPU_CREATE_CONTEXT_DESTROY_CTX_THEN_SET_CTX_TO_NULL_THEN_SET_STATUS_ERROR_THEN_GOTO_EXIT(NULL, 13, RED_STATUS_ERROR_EXTENSION_IS_NOT_FOUND);
    }
#if defined(REDGPU_OS_ANDROID)
#else
    if (requestedExtensions[RED_SDK_EXTENSION_WSI_WIN32] == 1 ||
        requestedExtensions[RED_SDK_EXTENSION_WSI_XLIB]  == 1 ||
        requestedExtensions[RED_SDK_EXTENSION_WSI_XCB]   == 1)
#endif
    {
      if (redInternalFindInstanceExtension(ctx, "VK_KHR_surface", procedureId, 1, optionalFile, optionalLine) == 0) {
        REDGPU_CREATE_CONTEXT_DESTROY_CTX_THEN_SET_CTX_TO_NULL_THEN_SET_STATUS_ERROR_THEN_GOTO_EXIT(NULL, 14, RED_STATUS_ERROR_EXTENSION_IS_NOT_FOUND);
      }
    }
    if (requestedExtensions[RED_SDK_EXTENSION_WSI_WIN32] == 1) {
      if (redInternalFindInstanceExtension(ctx, "VK_KHR_win32_surface", procedureId, 1, optionalFile, optionalLine) == 0) {
        REDGPU_CREATE_CONTEXT_DESTROY_CTX_THEN_SET_CTX_TO_NULL_THEN_SET_STATUS_ERROR_THEN_GOTO_EXIT(NULL, 15, RED_STATUS_ERROR_EXTENSION_IS_NOT_FOUND);
      }
    }
    if (requestedExtensions[RED_SDK_EXTENSION_WSI_XLIB] == 1) {
      if (redInternalFindInstanceExtension(ctx, "VK_KHR_xlib_surface", procedureId, 1, optionalFile, optionalLine) == 0) {
        REDGPU_CREATE_CONTEXT_DESTROY_CTX_THEN_SET_CTX_TO_NULL_THEN_SET_STATUS_ERROR_THEN_GOTO_EXIT(NULL, 16, RED_STATUS_ERROR_EXTENSION_IS_NOT_FOUND);
      }
    }
    if (requestedExtensions[RED_SDK_EXTENSION_WSI_XCB] == 1) {
      if (redInternalFindInstanceExtension(ctx, "VK_KHR_xcb_surface", procedureId, 1, optionalFile, optionalLine) == 0) {
        REDGPU_CREATE_CONTEXT_DESTROY_CTX_THEN_SET_CTX_TO_NULL_THEN_SET_STATUS_ERROR_THEN_GOTO_EXIT(NULL, 17, RED_STATUS_ERROR_EXTENSION_IS_NOT_FOUND);
      }
    }
  }

  unsigned enableLayersCount = 0;
  const char * enableLayers[1];
  if (requestedValidationLayers == 1) {
    if (0) {
      enableLayers[enableLayersCount] = "VK_LAYER_LUNARG_standard_validation";
      enableLayersCount += 1;
    } else if (sdkVersion == RED_SDK_VERSION_1_0_135) {
      enableLayers[enableLayersCount] = "VK_LAYER_KHRONOS_validation";
      enableLayersCount += 1;
    }
  }
  unsigned enableExtensionsCount = 0;
  const char * enableExtensions[7];
  if (requestedDebugMode == 1) {
#if defined(REDGPU_OS_ANDROID)
    enableExtensions[enableExtensionsCount] = "VK_EXT_debug_report";
#else
    enableExtensions[enableExtensionsCount] = "VK_EXT_debug_utils";
#endif
    enableExtensionsCount += 1;
  }
  enableExtensions[enableExtensionsCount] = "VK_KHR_get_physical_device_properties2";
  enableExtensionsCount += 1;
#if defined(REDGPU_OS_ANDROID)
#else
  if (requestedExtensions[RED_SDK_EXTENSION_WSI_WIN32] == 1 ||
      requestedExtensions[RED_SDK_EXTENSION_WSI_XLIB]  == 1 ||
      requestedExtensions[RED_SDK_EXTENSION_WSI_XCB]   == 1)
#endif
  {
    enableExtensions[enableExtensionsCount] = "VK_KHR_surface";
    enableExtensionsCount += 1;
  }
  if (requestedExtensions[RED_SDK_EXTENSION_WSI_WIN32] == 1) {
    enableExtensions[enableExtensionsCount] = "VK_KHR_win32_surface";
    enableExtensionsCount += 1;
  }
  if (requestedExtensions[RED_SDK_EXTENSION_WSI_XLIB] == 1) {
    enableExtensions[enableExtensionsCount] = "VK_KHR_xlib_surface";
    enableExtensionsCount += 1;
  }
  if (requestedExtensions[RED_SDK_EXTENSION_WSI_XCB] == 1) {
    enableExtensions[enableExtensionsCount] = "VK_KHR_xcb_surface";
    enableExtensionsCount += 1;
  }
#if defined(REDGPU_OS_ANDROID)
  {
    enableExtensions[enableExtensionsCount] = "VK_KHR_android_surface";
    enableExtensionsCount += 1;
  }
#endif

  VkApplicationInfo applicationInfo;
  applicationInfo.sType              = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  applicationInfo.pNext              = NULL;
  applicationInfo.pApplicationName   = optionalProgramName;
  applicationInfo.applicationVersion = optionalProgramVersion;
  applicationInfo.pEngineName        = optionalEngineName;
  applicationInfo.engineVersion      = optionalEngineVersion;
  if (requestedExtensions[RED_SDK_EXTENSION_RAY_TRACING] == 1) {
  applicationInfo.apiVersion         = VK_MAKE_VERSION(1, 2, 0);
  } else {
  applicationInfo.apiVersion         = VK_MAKE_VERSION(1, 0, 0);
  }
  VkInstanceCreateInfo instanceInfo;
  instanceInfo.sType                   = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  instanceInfo.pNext                   = NULL;
  instanceInfo.flags                   = 0;
  instanceInfo.pApplicationInfo        = &applicationInfo;
  instanceInfo.enabledLayerCount       = enableLayersCount;
  instanceInfo.ppEnabledLayerNames     = enableLayers;
  instanceInfo.enabledExtensionCount   = enableExtensionsCount;
  instanceInfo.ppEnabledExtensionNames = enableExtensions;

#if REDGPU_COMPILE_SWITCH == REDGPU_COMPILE_SWITCH_NVTX || REDGPU_COMPILE_SWITCH == REDGPU_COMPILE_SWITCH_GPUVIS
  {
  redInternalMarkSet("vkCreateInstance");
#endif

  if (settingsContextFromVkPassed == 1) {
    ctx->contextHandle = (RedHandleContext)settingsContextFromVkInstance;
  } else {
    redInternalSetStatus(ctx, NULL, outStatuses, 245, vk->vkCreateInstance(&instanceInfo, NULL, (VkInstance *)&ctx->contextHandle), procedureId, optionalFile, optionalLine, optionalUserData);
  }

#if REDGPU_COMPILE_SWITCH == REDGPU_COMPILE_SWITCH_NVTX || REDGPU_COMPILE_SWITCH == REDGPU_COMPILE_SWITCH_GPUVIS
  redInternalMarkEnd();
  }
#endif

  if (ctx->contextHandle == NULL) {
    REDGPU_CREATE_CONTEXT_DESTROY_CTX_THEN_SET_CTX_TO_NULL_THEN_SET_STATUS_ERROR_THEN_GOTO_EXIT(NULL, 18, RED_STATUS_ERROR_INITIALIZATION_FAILED);
  }

  vk->vkGetDeviceProcAddr                            = (PFN_vkGetDeviceProcAddr)redInternalGetInstanceProcAddr((VkInstance)ctx->contextHandle, "vkGetDeviceProcAddr");
#if defined(REDGPU_OS_ANDROID)
  vk->vkCreateDebugReportCallbackEXT                 = (PFN_vkCreateDebugReportCallbackEXT)redInternalGetInstanceProcAddr((VkInstance)ctx->contextHandle, "vkCreateDebugReportCallbackEXT");
#else
  vk->vkCreateDebugUtilsMessengerEXT                 = (PFN_vkCreateDebugUtilsMessengerEXT)redInternalGetInstanceProcAddr((VkInstance)ctx->contextHandle, "vkCreateDebugUtilsMessengerEXT");
#endif
  vk->vkEnumeratePhysicalDevices                     = (PFN_vkEnumeratePhysicalDevices)redInternalGetInstanceProcAddr((VkInstance)ctx->contextHandle, "vkEnumeratePhysicalDevices");
  vk->vkEnumerateDeviceExtensionProperties           = (PFN_vkEnumerateDeviceExtensionProperties)redInternalGetInstanceProcAddr((VkInstance)ctx->contextHandle, "vkEnumerateDeviceExtensionProperties");
  vk->vkGetPhysicalDeviceMemoryProperties2KHR        = (PFN_vkGetPhysicalDeviceMemoryProperties2KHR)redInternalGetInstanceProcAddr((VkInstance)ctx->contextHandle, "vkGetPhysicalDeviceMemoryProperties2KHR");
  vk->vkGetPhysicalDeviceFeatures2KHR                = (PFN_vkGetPhysicalDeviceFeatures2KHR)redInternalGetInstanceProcAddr((VkInstance)ctx->contextHandle, "vkGetPhysicalDeviceFeatures2KHR");
  vk->vkGetPhysicalDeviceProperties2KHR              = (PFN_vkGetPhysicalDeviceProperties2KHR)redInternalGetInstanceProcAddr((VkInstance)ctx->contextHandle, "vkGetPhysicalDeviceProperties2KHR");
  vk->vkGetPhysicalDeviceQueueFamilyProperties       = (PFN_vkGetPhysicalDeviceQueueFamilyProperties)redInternalGetInstanceProcAddr((VkInstance)ctx->contextHandle, "vkGetPhysicalDeviceQueueFamilyProperties");
  vk->vkCreateDevice                                 = (PFN_vkCreateDevice)redInternalGetInstanceProcAddr((VkInstance)ctx->contextHandle, "vkCreateDevice");
  vk->vkGetDeviceQueue                               = (PFN_vkGetDeviceQueue)redInternalGetInstanceProcAddr((VkInstance)ctx->contextHandle, "vkGetDeviceQueue");
  vk->vkGetPhysicalDeviceFormatProperties            = (PFN_vkGetPhysicalDeviceFormatProperties)redInternalGetInstanceProcAddr((VkInstance)ctx->contextHandle, "vkGetPhysicalDeviceFormatProperties");
  vk->vkDestroyDevice                                = (PFN_vkDestroyDevice)redInternalGetInstanceProcAddr((VkInstance)ctx->contextHandle, "vkDestroyDevice");
#if defined(REDGPU_OS_ANDROID)
  vk->vkDestroyDebugReportCallbackEXT                = (PFN_vkDestroyDebugReportCallbackEXT)redInternalGetInstanceProcAddr((VkInstance)ctx->contextHandle, "vkDestroyDebugReportCallbackEXT");
#else
  vk->vkDestroyDebugUtilsMessengerEXT                = (PFN_vkDestroyDebugUtilsMessengerEXT)redInternalGetInstanceProcAddr((VkInstance)ctx->contextHandle, "vkDestroyDebugUtilsMessengerEXT");
#endif
  vk->vkDestroyInstance                              = (PFN_vkDestroyInstance)redInternalGetInstanceProcAddr((VkInstance)ctx->contextHandle, "vkDestroyInstance");
#if defined(REDGPU_OS_ANDROID)
  vk->vkSetDebugUtilsObjectNameEXT                   = redInternalAndroidVkSetDebugUtilsObjectNameEXT;
#else
  vk->vkSetDebugUtilsObjectNameEXT                   = (PFN_vkSetDebugUtilsObjectNameEXT)redInternalGetInstanceProcAddr((VkInstance)ctx->contextHandle, "vkSetDebugUtilsObjectNameEXT");
#endif
  vk->vkGetPhysicalDeviceImageFormatProperties       = (PFN_vkGetPhysicalDeviceImageFormatProperties)redInternalGetInstanceProcAddr((VkInstance)ctx->contextHandle, "vkGetPhysicalDeviceImageFormatProperties");
  vk->vkGetBufferMemoryRequirements                  = (PFN_vkGetBufferMemoryRequirements)redInternalGetInstanceProcAddr((VkInstance)ctx->contextHandle, "vkGetBufferMemoryRequirements");
  vk->vkGetImageMemoryRequirements                   = (PFN_vkGetImageMemoryRequirements)redInternalGetInstanceProcAddr((VkInstance)ctx->contextHandle, "vkGetImageMemoryRequirements");
  vk->vkAllocateMemory                               = (PFN_vkAllocateMemory)redInternalGetInstanceProcAddr((VkInstance)ctx->contextHandle, "vkAllocateMemory");
  vk->vkFreeMemory                                   = (PFN_vkFreeMemory)redInternalGetInstanceProcAddr((VkInstance)ctx->contextHandle, "vkFreeMemory");
  vk->vkMapMemory                                    = (PFN_vkMapMemory)redInternalGetInstanceProcAddr((VkInstance)ctx->contextHandle, "vkMapMemory");
  vk->vkUnmapMemory                                  = (PFN_vkUnmapMemory)redInternalGetInstanceProcAddr((VkInstance)ctx->contextHandle, "vkUnmapMemory");
  vk->vkBindBufferMemory                             = (PFN_vkBindBufferMemory)redInternalGetInstanceProcAddr((VkInstance)ctx->contextHandle, "vkBindBufferMemory");
  vk->vkBindImageMemory                              = (PFN_vkBindImageMemory)redInternalGetInstanceProcAddr((VkInstance)ctx->contextHandle, "vkBindImageMemory");
  vk->vkFlushMappedMemoryRanges                      = (PFN_vkFlushMappedMemoryRanges)redInternalGetInstanceProcAddr((VkInstance)ctx->contextHandle, "vkFlushMappedMemoryRanges");
  vk->vkInvalidateMappedMemoryRanges                 = (PFN_vkInvalidateMappedMemoryRanges)redInternalGetInstanceProcAddr((VkInstance)ctx->contextHandle, "vkInvalidateMappedMemoryRanges");
  vk->vkCreateBuffer                                 = (PFN_vkCreateBuffer)redInternalGetInstanceProcAddr((VkInstance)ctx->contextHandle, "vkCreateBuffer");
  vk->vkDestroyBuffer                                = (PFN_vkDestroyBuffer)redInternalGetInstanceProcAddr((VkInstance)ctx->contextHandle, "vkDestroyBuffer");
  vk->vkCreateImage                                  = (PFN_vkCreateImage)redInternalGetInstanceProcAddr((VkInstance)ctx->contextHandle, "vkCreateImage");
  vk->vkDestroyImage                                 = (PFN_vkDestroyImage)redInternalGetInstanceProcAddr((VkInstance)ctx->contextHandle, "vkDestroyImage");
  vk->vkCreateSampler                                = (PFN_vkCreateSampler)redInternalGetInstanceProcAddr((VkInstance)ctx->contextHandle, "vkCreateSampler");
  vk->vkDestroySampler                               = (PFN_vkDestroySampler)redInternalGetInstanceProcAddr((VkInstance)ctx->contextHandle, "vkDestroySampler");
  vk->vkCreateImageView                              = (PFN_vkCreateImageView)redInternalGetInstanceProcAddr((VkInstance)ctx->contextHandle, "vkCreateImageView");
  vk->vkDestroyImageView                             = (PFN_vkDestroyImageView)redInternalGetInstanceProcAddr((VkInstance)ctx->contextHandle, "vkDestroyImageView");
  vk->vkCreateShaderModule                           = (PFN_vkCreateShaderModule)redInternalGetInstanceProcAddr((VkInstance)ctx->contextHandle, "vkCreateShaderModule");
  vk->vkDestroyShaderModule                          = (PFN_vkDestroyShaderModule)redInternalGetInstanceProcAddr((VkInstance)ctx->contextHandle, "vkDestroyShaderModule");
  vk->vkCreateGraphicsPipelines                      = (PFN_vkCreateGraphicsPipelines)redInternalGetInstanceProcAddr((VkInstance)ctx->contextHandle, "vkCreateGraphicsPipelines");
  vk->vkCreateComputePipelines                       = (PFN_vkCreateComputePipelines)redInternalGetInstanceProcAddr((VkInstance)ctx->contextHandle, "vkCreateComputePipelines");
  vk->vkDestroyPipeline                              = (PFN_vkDestroyPipeline)redInternalGetInstanceProcAddr((VkInstance)ctx->contextHandle, "vkDestroyPipeline");
  vk->vkCreatePipelineCache                          = (PFN_vkCreatePipelineCache)redInternalGetInstanceProcAddr((VkInstance)ctx->contextHandle, "vkCreatePipelineCache");
  vk->vkDestroyPipelineCache                         = (PFN_vkDestroyPipelineCache)redInternalGetInstanceProcAddr((VkInstance)ctx->contextHandle, "vkDestroyPipelineCache");
  vk->vkGetPipelineCacheData                         = (PFN_vkGetPipelineCacheData)redInternalGetInstanceProcAddr((VkInstance)ctx->contextHandle, "vkGetPipelineCacheData");
  vk->vkMergePipelineCaches                          = (PFN_vkMergePipelineCaches)redInternalGetInstanceProcAddr((VkInstance)ctx->contextHandle, "vkMergePipelineCaches");
  vk->vkCreatePipelineLayout                         = (PFN_vkCreatePipelineLayout)redInternalGetInstanceProcAddr((VkInstance)ctx->contextHandle, "vkCreatePipelineLayout");
  vk->vkDestroyPipelineLayout                        = (PFN_vkDestroyPipelineLayout)redInternalGetInstanceProcAddr((VkInstance)ctx->contextHandle, "vkDestroyPipelineLayout");
  vk->vkCreateDescriptorSetLayout                    = (PFN_vkCreateDescriptorSetLayout)redInternalGetInstanceProcAddr((VkInstance)ctx->contextHandle, "vkCreateDescriptorSetLayout");
  vk->vkDestroyDescriptorSetLayout                   = (PFN_vkDestroyDescriptorSetLayout)redInternalGetInstanceProcAddr((VkInstance)ctx->contextHandle, "vkDestroyDescriptorSetLayout");
  vk->vkCreateDescriptorPool                         = (PFN_vkCreateDescriptorPool)redInternalGetInstanceProcAddr((VkInstance)ctx->contextHandle, "vkCreateDescriptorPool");
  vk->vkAllocateDescriptorSets                       = (PFN_vkAllocateDescriptorSets)redInternalGetInstanceProcAddr((VkInstance)ctx->contextHandle, "vkAllocateDescriptorSets");
  vk->vkResetDescriptorPool                          = (PFN_vkResetDescriptorPool)redInternalGetInstanceProcAddr((VkInstance)ctx->contextHandle, "vkResetDescriptorPool");
  vk->vkDestroyDescriptorPool                        = (PFN_vkDestroyDescriptorPool)redInternalGetInstanceProcAddr((VkInstance)ctx->contextHandle, "vkDestroyDescriptorPool");
  vk->vkUpdateDescriptorSets                         = (PFN_vkUpdateDescriptorSets)redInternalGetInstanceProcAddr((VkInstance)ctx->contextHandle, "vkUpdateDescriptorSets");
  vk->vkCreateRenderPass                             = (PFN_vkCreateRenderPass)redInternalGetInstanceProcAddr((VkInstance)ctx->contextHandle, "vkCreateRenderPass");
  vk->vkDestroyRenderPass                            = (PFN_vkDestroyRenderPass)redInternalGetInstanceProcAddr((VkInstance)ctx->contextHandle, "vkDestroyRenderPass");
  vk->vkCreateFramebuffer                            = (PFN_vkCreateFramebuffer)redInternalGetInstanceProcAddr((VkInstance)ctx->contextHandle, "vkCreateFramebuffer");
  vk->vkDestroyFramebuffer                           = (PFN_vkDestroyFramebuffer)redInternalGetInstanceProcAddr((VkInstance)ctx->contextHandle, "vkDestroyFramebuffer");
  vk->vkCreateFence                                  = (PFN_vkCreateFence)redInternalGetInstanceProcAddr((VkInstance)ctx->contextHandle, "vkCreateFence");
  vk->vkDestroyFence                                 = (PFN_vkDestroyFence)redInternalGetInstanceProcAddr((VkInstance)ctx->contextHandle, "vkDestroyFence");
  vk->vkGetFenceStatus                               = (PFN_vkGetFenceStatus)redInternalGetInstanceProcAddr((VkInstance)ctx->contextHandle, "vkGetFenceStatus");
  vk->vkWaitForFences                                = (PFN_vkWaitForFences)redInternalGetInstanceProcAddr((VkInstance)ctx->contextHandle, "vkWaitForFences");
  vk->vkResetFences                                  = (PFN_vkResetFences)redInternalGetInstanceProcAddr((VkInstance)ctx->contextHandle, "vkResetFences");
  vk->vkCreateSemaphore                              = (PFN_vkCreateSemaphore)redInternalGetInstanceProcAddr((VkInstance)ctx->contextHandle, "vkCreateSemaphore");
  vk->vkDestroySemaphore                             = (PFN_vkDestroySemaphore)redInternalGetInstanceProcAddr((VkInstance)ctx->contextHandle, "vkDestroySemaphore");
  vk->vkCreateEvent                                  = (PFN_vkCreateEvent)redInternalGetInstanceProcAddr((VkInstance)ctx->contextHandle, "vkCreateEvent");
  vk->vkDestroyEvent                                 = (PFN_vkDestroyEvent)redInternalGetInstanceProcAddr((VkInstance)ctx->contextHandle, "vkDestroyEvent");
  vk->vkGetEventStatus                               = (PFN_vkGetEventStatus)redInternalGetInstanceProcAddr((VkInstance)ctx->contextHandle, "vkGetEventStatus");
  vk->vkResetEvent                                   = (PFN_vkResetEvent)redInternalGetInstanceProcAddr((VkInstance)ctx->contextHandle, "vkResetEvent");
  vk->vkCreateCommandPool                            = (PFN_vkCreateCommandPool)redInternalGetInstanceProcAddr((VkInstance)ctx->contextHandle, "vkCreateCommandPool");
  vk->vkAllocateCommandBuffers                       = (PFN_vkAllocateCommandBuffers)redInternalGetInstanceProcAddr((VkInstance)ctx->contextHandle, "vkAllocateCommandBuffers");
  vk->vkDestroyCommandPool                           = (PFN_vkDestroyCommandPool)redInternalGetInstanceProcAddr((VkInstance)ctx->contextHandle, "vkDestroyCommandPool");
  vk->vkResetCommandPool                             = (PFN_vkResetCommandPool)redInternalGetInstanceProcAddr((VkInstance)ctx->contextHandle, "vkResetCommandPool");
  vk->vkBeginCommandBuffer                           = (PFN_vkBeginCommandBuffer)redInternalGetInstanceProcAddr((VkInstance)ctx->contextHandle, "vkBeginCommandBuffer");
  vk->vkEndCommandBuffer                             = (PFN_vkEndCommandBuffer)redInternalGetInstanceProcAddr((VkInstance)ctx->contextHandle, "vkEndCommandBuffer");
  vk->vkCmdInsertDebugUtilsLabelEXT                  = (PFN_vkCmdInsertDebugUtilsLabelEXT)redInternalGetInstanceProcAddr((VkInstance)ctx->contextHandle, "vkCmdInsertDebugUtilsLabelEXT");
  vk->vkCmdBeginDebugUtilsLabelEXT                   = (PFN_vkCmdBeginDebugUtilsLabelEXT)redInternalGetInstanceProcAddr((VkInstance)ctx->contextHandle, "vkCmdBeginDebugUtilsLabelEXT");
  vk->vkCmdEndDebugUtilsLabelEXT                     = (PFN_vkCmdEndDebugUtilsLabelEXT)redInternalGetInstanceProcAddr((VkInstance)ctx->contextHandle, "vkCmdEndDebugUtilsLabelEXT");
  vk->vkQueueSubmit                                  = (PFN_vkQueueSubmit)redInternalGetInstanceProcAddr((VkInstance)ctx->contextHandle, "vkQueueSubmit");
  vk->vkQueueWaitIdle                                = (PFN_vkQueueWaitIdle)redInternalGetInstanceProcAddr((VkInstance)ctx->contextHandle, "vkQueueWaitIdle");
#if defined(VK_USE_PLATFORM_WIN32_KHR)
  vk->vkCreateWin32SurfaceKHR                        = (PFN_vkCreateWin32SurfaceKHR)redInternalGetInstanceProcAddr((VkInstance)ctx->contextHandle, "vkCreateWin32SurfaceKHR");
  vk->vkGetPhysicalDeviceWin32PresentationSupportKHR = (PFN_vkGetPhysicalDeviceWin32PresentationSupportKHR)redInternalGetInstanceProcAddr((VkInstance)ctx->contextHandle, "vkGetPhysicalDeviceWin32PresentationSupportKHR");
#endif
#if defined(VK_USE_PLATFORM_XLIB_KHR) && defined(VK_USE_PLATFORM_XCB_KHR)
  vk->vkCreateXcbSurfaceKHR                          = (PFN_vkCreateXcbSurfaceKHR)redInternalGetInstanceProcAddr((VkInstance)ctx->contextHandle, "vkCreateXcbSurfaceKHR");
  vk->vkGetPhysicalDeviceXcbPresentationSupportKHR   = (PFN_vkGetPhysicalDeviceXcbPresentationSupportKHR)redInternalGetInstanceProcAddr((VkInstance)ctx->contextHandle, "vkGetPhysicalDeviceXcbPresentationSupportKHR");
  vk->vkCreateXlibSurfaceKHR                         = (PFN_vkCreateXlibSurfaceKHR)redInternalGetInstanceProcAddr((VkInstance)ctx->contextHandle, "vkCreateXlibSurfaceKHR");
  vk->vkGetPhysicalDeviceXlibPresentationSupportKHR  = (PFN_vkGetPhysicalDeviceXlibPresentationSupportKHR)redInternalGetInstanceProcAddr((VkInstance)ctx->contextHandle, "vkGetPhysicalDeviceXlibPresentationSupportKHR");
#endif
#if defined(REDGPU_OS_ANDROID)
  redInternalCreateAndroidSurface                    = (int (*)(void *, const void *, const void *, void **))redInternalGetInstanceProcAddr((VkInstance)ctx->contextHandle, "vkCreateAndroidSurfaceKHR");
#endif
  vk->vkDestroySurfaceKHR                            = (PFN_vkDestroySurfaceKHR)redInternalGetInstanceProcAddr((VkInstance)ctx->contextHandle, "vkDestroySurfaceKHR");
  vk->vkCreateSwapchainKHR                           = (PFN_vkCreateSwapchainKHR)redInternalGetInstanceProcAddr((VkInstance)ctx->contextHandle, "vkCreateSwapchainKHR");
  vk->vkGetSwapchainImagesKHR                        = (PFN_vkGetSwapchainImagesKHR)redInternalGetInstanceProcAddr((VkInstance)ctx->contextHandle, "vkGetSwapchainImagesKHR");
  vk->vkDestroySwapchainKHR                          = (PFN_vkDestroySwapchainKHR)redInternalGetInstanceProcAddr((VkInstance)ctx->contextHandle, "vkDestroySwapchainKHR");
  vk->vkGetPhysicalDeviceSurfaceSupportKHR           = (PFN_vkGetPhysicalDeviceSurfaceSupportKHR)redInternalGetInstanceProcAddr((VkInstance)ctx->contextHandle, "vkGetPhysicalDeviceSurfaceSupportKHR");
  vk->vkGetPhysicalDeviceSurfaceFormatsKHR           = (PFN_vkGetPhysicalDeviceSurfaceFormatsKHR)redInternalGetInstanceProcAddr((VkInstance)ctx->contextHandle, "vkGetPhysicalDeviceSurfaceFormatsKHR");
  vk->vkGetPhysicalDeviceSurfaceCapabilitiesKHR      = (PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR)redInternalGetInstanceProcAddr((VkInstance)ctx->contextHandle, "vkGetPhysicalDeviceSurfaceCapabilitiesKHR");
  vk->vkGetPhysicalDeviceSurfacePresentModesKHR      = (PFN_vkGetPhysicalDeviceSurfacePresentModesKHR)redInternalGetInstanceProcAddr((VkInstance)ctx->contextHandle, "vkGetPhysicalDeviceSurfacePresentModesKHR");
  vk->vkAcquireNextImageKHR                          = (PFN_vkAcquireNextImageKHR)redInternalGetInstanceProcAddr((VkInstance)ctx->contextHandle, "vkAcquireNextImageKHR");
  vk->vkQueuePresentKHR                              = (PFN_vkQueuePresentKHR)redInternalGetInstanceProcAddr((VkInstance)ctx->contextHandle, "vkQueuePresentKHR");
  vk->vkCreateRenderPass2KHR                         = (PFN_vkCreateRenderPass2KHR)redInternalGetInstanceProcAddr((VkInstance)ctx->contextHandle, "vkCreateRenderPass2KHR");
  vk->vkSubmitDebugUtilsMessageEXT                   = (PFN_vkSubmitDebugUtilsMessageEXT)redInternalGetInstanceProcAddr((VkInstance)ctx->contextHandle, "vkSubmitDebugUtilsMessageEXT");
  vk->vkBindBufferMemory2KHR                         = (PFN_vkBindBufferMemory2KHR)redInternalGetInstanceProcAddr((VkInstance)ctx->contextHandle, "vkBindBufferMemory2KHR");
  vk->vkBindImageMemory2KHR                          = (PFN_vkBindImageMemory2KHR)redInternalGetInstanceProcAddr((VkInstance)ctx->contextHandle, "vkBindImageMemory2KHR");
#if REDGPU_COMPILE_SWITCH == REDGPU_COMPILE_SWITCH_DEBUG
  vk->vkSetEvent                                     = (PFN_vkSetEvent)redInternalGetInstanceProcAddr((VkInstance)ctx->contextHandle, "vkSetEvent");
  vk->vkCmdWaitEvents                                = (PFN_vkCmdWaitEvents)redInternalGetInstanceProcAddr((VkInstance)ctx->contextHandle, "vkCmdWaitEvents");
  vk->vkCmdResetEvent                                = (PFN_vkCmdResetEvent)redInternalGetInstanceProcAddr((VkInstance)ctx->contextHandle, "vkCmdResetEvent");
#endif
  vk->vkCreateQueryPool                              = (PFN_vkCreateQueryPool)redInternalGetInstanceProcAddr((VkInstance)ctx->contextHandle, "vkCreateQueryPool");
  vk->vkDestroyQueryPool                             = (PFN_vkDestroyQueryPool)redInternalGetInstanceProcAddr((VkInstance)ctx->contextHandle, "vkDestroyQueryPool");
  vk->vkCmdResetQueryPool                            = (PFN_vkCmdResetQueryPool)redInternalGetInstanceProcAddr((VkInstance)ctx->contextHandle, "vkCmdResetQueryPool");
  vk->vkCmdWriteTimestamp                            = (PFN_vkCmdWriteTimestamp)redInternalGetInstanceProcAddr((VkInstance)ctx->contextHandle, "vkCmdWriteTimestamp");
  vk->vkGetQueryPoolResults                          = (PFN_vkGetQueryPoolResults)redInternalGetInstanceProcAddr((VkInstance)ctx->contextHandle, "vkGetQueryPoolResults");

#if REDGPU_COMPILE_SWITCH == REDGPU_COMPILE_SWITCH_DEBUG
  if (vk->vkGetDeviceProcAddr                            == NULL) { REDGPU_INTERNAL_DEBUG_SEND_ERROR_MESSAGE(ctx, NULL, REDGPU_PRINT_PROCEDURE_ID(procedureId), 19, "RedDriverProceduresAndAddresses::GetDeviceProcAddr == NULL", optionalFile, optionalLine) }
#if defined(REDGPU_OS_ANDROID)
  if (vk->vkCreateDebugReportCallbackEXT                 == NULL) { REDGPU_INTERNAL_DEBUG_SEND_ERROR_MESSAGE(ctx, NULL, REDGPU_PRINT_PROCEDURE_ID(procedureId), 20, "RedDriverProceduresAndAddresses::CreateDebugReportCallback == NULL", optionalFile, optionalLine) }
#else
  if (vk->vkCreateDebugUtilsMessengerEXT                 == NULL) { REDGPU_INTERNAL_DEBUG_SEND_ERROR_MESSAGE(ctx, NULL, REDGPU_PRINT_PROCEDURE_ID(procedureId), 20, "RedDriverProceduresAndAddresses::CreateDebugUtilsMessenger == NULL", optionalFile, optionalLine) }
#endif
  if (vk->vkEnumeratePhysicalDevices                     == NULL) { REDGPU_INTERNAL_DEBUG_SEND_ERROR_MESSAGE(ctx, NULL, REDGPU_PRINT_PROCEDURE_ID(procedureId), 21, "RedDriverProceduresAndAddresses::EnumeratePhysicalDevices == NULL", optionalFile, optionalLine) }
  if (vk->vkEnumerateDeviceExtensionProperties           == NULL) { REDGPU_INTERNAL_DEBUG_SEND_ERROR_MESSAGE(ctx, NULL, REDGPU_PRINT_PROCEDURE_ID(procedureId), 22, "RedDriverProceduresAndAddresses::EnumerateDeviceExtensionProperties == NULL", optionalFile, optionalLine) }
  if (vk->vkGetPhysicalDeviceMemoryProperties2KHR        == NULL) { REDGPU_INTERNAL_DEBUG_SEND_ERROR_MESSAGE(ctx, NULL, REDGPU_PRINT_PROCEDURE_ID(procedureId), 23, "RedDriverProceduresAndAddresses::GetPhysicalDeviceMemoryProperties == NULL", optionalFile, optionalLine) }
  if (vk->vkGetPhysicalDeviceFeatures2KHR                == NULL) { REDGPU_INTERNAL_DEBUG_SEND_ERROR_MESSAGE(ctx, NULL, REDGPU_PRINT_PROCEDURE_ID(procedureId), 24, "RedDriverProceduresAndAddresses::GetPhysicalDeviceFeatures == NULL", optionalFile, optionalLine) }
  if (vk->vkGetPhysicalDeviceProperties2KHR              == NULL) { REDGPU_INTERNAL_DEBUG_SEND_ERROR_MESSAGE(ctx, NULL, REDGPU_PRINT_PROCEDURE_ID(procedureId), 25, "RedDriverProceduresAndAddresses::GetPhysicalDeviceProperties == NULL", optionalFile, optionalLine) }
  if (vk->vkGetPhysicalDeviceQueueFamilyProperties       == NULL) { REDGPU_INTERNAL_DEBUG_SEND_ERROR_MESSAGE(ctx, NULL, REDGPU_PRINT_PROCEDURE_ID(procedureId), 26, "RedDriverProceduresAndAddresses::GetPhysicalDeviceQueueFamilyProperties == NULL", optionalFile, optionalLine) }
  if (vk->vkCreateDevice                                 == NULL) { REDGPU_INTERNAL_DEBUG_SEND_ERROR_MESSAGE(ctx, NULL, REDGPU_PRINT_PROCEDURE_ID(procedureId), 27, "RedDriverProceduresAndAddresses::CreateDevice == NULL", optionalFile, optionalLine) }
  if (vk->vkGetDeviceQueue                               == NULL) { REDGPU_INTERNAL_DEBUG_SEND_ERROR_MESSAGE(ctx, NULL, REDGPU_PRINT_PROCEDURE_ID(procedureId), 28, "RedDriverProceduresAndAddresses::GetDeviceQueue == NULL", optionalFile, optionalLine) }
  if (vk->vkGetPhysicalDeviceFormatProperties            == NULL) { REDGPU_INTERNAL_DEBUG_SEND_ERROR_MESSAGE(ctx, NULL, REDGPU_PRINT_PROCEDURE_ID(procedureId), 29, "RedDriverProceduresAndAddresses::GetPhysicalDeviceFormatProperties == NULL", optionalFile, optionalLine) }
  if (vk->vkDestroyDevice                                == NULL) { REDGPU_INTERNAL_DEBUG_SEND_ERROR_MESSAGE(ctx, NULL, REDGPU_PRINT_PROCEDURE_ID(procedureId), 30, "RedDriverProceduresAndAddresses::DestroyDevice == NULL", optionalFile, optionalLine) }
#if defined(REDGPU_OS_ANDROID)
  if (vk->vkDestroyDebugReportCallbackEXT                == NULL) { REDGPU_INTERNAL_DEBUG_SEND_ERROR_MESSAGE(ctx, NULL, REDGPU_PRINT_PROCEDURE_ID(procedureId), 31, "RedDriverProceduresAndAddresses::DestroyDebugReportCallback == NULL", optionalFile, optionalLine) }
#else
  if (vk->vkDestroyDebugUtilsMessengerEXT                == NULL) { REDGPU_INTERNAL_DEBUG_SEND_ERROR_MESSAGE(ctx, NULL, REDGPU_PRINT_PROCEDURE_ID(procedureId), 31, "RedDriverProceduresAndAddresses::DestroyDebugUtilsMessenger == NULL", optionalFile, optionalLine) }
#endif
  if (vk->vkDestroyInstance                              == NULL) { REDGPU_INTERNAL_DEBUG_SEND_ERROR_MESSAGE(ctx, NULL, REDGPU_PRINT_PROCEDURE_ID(procedureId), 32, "RedDriverProceduresAndAddresses::DestroyInstance == NULL", optionalFile, optionalLine) }
  if (vk->vkSetDebugUtilsObjectNameEXT                   == NULL) { REDGPU_INTERNAL_DEBUG_SEND_ERROR_MESSAGE(ctx, NULL, REDGPU_PRINT_PROCEDURE_ID(procedureId), 33, "RedDriverProceduresAndAddresses::SetDebugUtilsObjectName == NULL", optionalFile, optionalLine) }
  if (vk->vkGetPhysicalDeviceImageFormatProperties       == NULL) { REDGPU_INTERNAL_DEBUG_SEND_ERROR_MESSAGE(ctx, NULL, REDGPU_PRINT_PROCEDURE_ID(procedureId), 34, "RedDriverProceduresAndAddresses::GetPhysicalDeviceImageFormatProperties == NULL", optionalFile, optionalLine) }
  if (vk->vkGetBufferMemoryRequirements                  == NULL) { REDGPU_INTERNAL_DEBUG_SEND_ERROR_MESSAGE(ctx, NULL, REDGPU_PRINT_PROCEDURE_ID(procedureId), 35, "RedDriverProceduresAndAddresses::GetBufferMemoryRequirements == NULL", optionalFile, optionalLine) }
  if (vk->vkGetImageMemoryRequirements                   == NULL) { REDGPU_INTERNAL_DEBUG_SEND_ERROR_MESSAGE(ctx, NULL, REDGPU_PRINT_PROCEDURE_ID(procedureId), 36, "RedDriverProceduresAndAddresses::GetImageMemoryRequirements == NULL", optionalFile, optionalLine) }
  if (vk->vkAllocateMemory                               == NULL) { REDGPU_INTERNAL_DEBUG_SEND_ERROR_MESSAGE(ctx, NULL, REDGPU_PRINT_PROCEDURE_ID(procedureId), 37, "RedDriverProceduresAndAddresses::AllocateMemory == NULL", optionalFile, optionalLine) }
  if (vk->vkFreeMemory                                   == NULL) { REDGPU_INTERNAL_DEBUG_SEND_ERROR_MESSAGE(ctx, NULL, REDGPU_PRINT_PROCEDURE_ID(procedureId), 38, "RedDriverProceduresAndAddresses::FreeMemory == NULL", optionalFile, optionalLine) }
  if (vk->vkMapMemory                                    == NULL) { REDGPU_INTERNAL_DEBUG_SEND_ERROR_MESSAGE(ctx, NULL, REDGPU_PRINT_PROCEDURE_ID(procedureId), 39, "RedDriverProceduresAndAddresses::MapMemory == NULL", optionalFile, optionalLine) }
  if (vk->vkUnmapMemory                                  == NULL) { REDGPU_INTERNAL_DEBUG_SEND_ERROR_MESSAGE(ctx, NULL, REDGPU_PRINT_PROCEDURE_ID(procedureId), 40, "RedDriverProceduresAndAddresses::UnmapMemory == NULL", optionalFile, optionalLine) }
  if (vk->vkBindBufferMemory                             == NULL) { REDGPU_INTERNAL_DEBUG_SEND_ERROR_MESSAGE(ctx, NULL, REDGPU_PRINT_PROCEDURE_ID(procedureId), 41, "RedDriverProceduresAndAddresses::BindBufferMemory == NULL", optionalFile, optionalLine) }
  if (vk->vkBindImageMemory                              == NULL) { REDGPU_INTERNAL_DEBUG_SEND_ERROR_MESSAGE(ctx, NULL, REDGPU_PRINT_PROCEDURE_ID(procedureId), 42, "RedDriverProceduresAndAddresses::BindImageMemory == NULL", optionalFile, optionalLine) }
  if (vk->vkFlushMappedMemoryRanges                      == NULL) { REDGPU_INTERNAL_DEBUG_SEND_ERROR_MESSAGE(ctx, NULL, REDGPU_PRINT_PROCEDURE_ID(procedureId), 43, "RedDriverProceduresAndAddresses::FlushMappedMemoryRanges == NULL", optionalFile, optionalLine) }
  if (vk->vkInvalidateMappedMemoryRanges                 == NULL) { REDGPU_INTERNAL_DEBUG_SEND_ERROR_MESSAGE(ctx, NULL, REDGPU_PRINT_PROCEDURE_ID(procedureId), 44, "RedDriverProceduresAndAddresses::InvalidateMappedMemoryRanges == NULL", optionalFile, optionalLine) }
  if (vk->vkCreateBuffer                                 == NULL) { REDGPU_INTERNAL_DEBUG_SEND_ERROR_MESSAGE(ctx, NULL, REDGPU_PRINT_PROCEDURE_ID(procedureId), 45, "RedDriverProceduresAndAddresses::CreateBuffer == NULL", optionalFile, optionalLine) }
  if (vk->vkDestroyBuffer                                == NULL) { REDGPU_INTERNAL_DEBUG_SEND_ERROR_MESSAGE(ctx, NULL, REDGPU_PRINT_PROCEDURE_ID(procedureId), 46, "RedDriverProceduresAndAddresses::DestroyBuffer == NULL", optionalFile, optionalLine) }
  if (vk->vkCreateImage                                  == NULL) { REDGPU_INTERNAL_DEBUG_SEND_ERROR_MESSAGE(ctx, NULL, REDGPU_PRINT_PROCEDURE_ID(procedureId), 47, "RedDriverProceduresAndAddresses::CreateImage == NULL", optionalFile, optionalLine) }
  if (vk->vkDestroyImage                                 == NULL) { REDGPU_INTERNAL_DEBUG_SEND_ERROR_MESSAGE(ctx, NULL, REDGPU_PRINT_PROCEDURE_ID(procedureId), 48, "RedDriverProceduresAndAddresses::DestroyImage == NULL", optionalFile, optionalLine) }
  if (vk->vkCreateSampler                                == NULL) { REDGPU_INTERNAL_DEBUG_SEND_ERROR_MESSAGE(ctx, NULL, REDGPU_PRINT_PROCEDURE_ID(procedureId), 49, "RedDriverProceduresAndAddresses::CreateSampler == NULL", optionalFile, optionalLine) }
  if (vk->vkDestroySampler                               == NULL) { REDGPU_INTERNAL_DEBUG_SEND_ERROR_MESSAGE(ctx, NULL, REDGPU_PRINT_PROCEDURE_ID(procedureId), 50, "RedDriverProceduresAndAddresses::DestroySampler == NULL", optionalFile, optionalLine) }
  if (vk->vkCreateImageView                              == NULL) { REDGPU_INTERNAL_DEBUG_SEND_ERROR_MESSAGE(ctx, NULL, REDGPU_PRINT_PROCEDURE_ID(procedureId), 51, "RedDriverProceduresAndAddresses::CreateImageView == NULL", optionalFile, optionalLine) }
  if (vk->vkDestroyImageView                             == NULL) { REDGPU_INTERNAL_DEBUG_SEND_ERROR_MESSAGE(ctx, NULL, REDGPU_PRINT_PROCEDURE_ID(procedureId), 52, "RedDriverProceduresAndAddresses::DestroyImageView == NULL", optionalFile, optionalLine) }
  if (vk->vkCreateShaderModule                           == NULL) { REDGPU_INTERNAL_DEBUG_SEND_ERROR_MESSAGE(ctx, NULL, REDGPU_PRINT_PROCEDURE_ID(procedureId), 53, "RedDriverProceduresAndAddresses::CreateShaderModule == NULL", optionalFile, optionalLine) }
  if (vk->vkDestroyShaderModule                          == NULL) { REDGPU_INTERNAL_DEBUG_SEND_ERROR_MESSAGE(ctx, NULL, REDGPU_PRINT_PROCEDURE_ID(procedureId), 54, "RedDriverProceduresAndAddresses::DestroyShaderModule == NULL", optionalFile, optionalLine) }
  if (vk->vkCreateGraphicsPipelines                      == NULL) { REDGPU_INTERNAL_DEBUG_SEND_ERROR_MESSAGE(ctx, NULL, REDGPU_PRINT_PROCEDURE_ID(procedureId), 55, "RedDriverProceduresAndAddresses::CreateGraphicsPipelines == NULL", optionalFile, optionalLine) }
  if (vk->vkCreateComputePipelines                       == NULL) { REDGPU_INTERNAL_DEBUG_SEND_ERROR_MESSAGE(ctx, NULL, REDGPU_PRINT_PROCEDURE_ID(procedureId), 56, "RedDriverProceduresAndAddresses::CreateComputePipelines == NULL", optionalFile, optionalLine) }
  if (vk->vkDestroyPipeline                              == NULL) { REDGPU_INTERNAL_DEBUG_SEND_ERROR_MESSAGE(ctx, NULL, REDGPU_PRINT_PROCEDURE_ID(procedureId), 57, "RedDriverProceduresAndAddresses::DestroyPipeline == NULL", optionalFile, optionalLine) }
  if (vk->vkCreatePipelineCache                          == NULL) { REDGPU_INTERNAL_DEBUG_SEND_ERROR_MESSAGE(ctx, NULL, REDGPU_PRINT_PROCEDURE_ID(procedureId), 58, "RedDriverProceduresAndAddresses::CreatePipelineCache == NULL", optionalFile, optionalLine) }
  if (vk->vkDestroyPipelineCache                         == NULL) { REDGPU_INTERNAL_DEBUG_SEND_ERROR_MESSAGE(ctx, NULL, REDGPU_PRINT_PROCEDURE_ID(procedureId), 59, "RedDriverProceduresAndAddresses::DestroyPipelineCache == NULL", optionalFile, optionalLine) }
  if (vk->vkGetPipelineCacheData                         == NULL) { REDGPU_INTERNAL_DEBUG_SEND_ERROR_MESSAGE(ctx, NULL, REDGPU_PRINT_PROCEDURE_ID(procedureId), 60, "RedDriverProceduresAndAddresses::GetPipelineCacheData == NULL", optionalFile, optionalLine) }
  if (vk->vkMergePipelineCaches                          == NULL) { REDGPU_INTERNAL_DEBUG_SEND_ERROR_MESSAGE(ctx, NULL, REDGPU_PRINT_PROCEDURE_ID(procedureId), 61, "RedDriverProceduresAndAddresses::MergePipelineCaches == NULL", optionalFile, optionalLine) }
  if (vk->vkCreatePipelineLayout                         == NULL) { REDGPU_INTERNAL_DEBUG_SEND_ERROR_MESSAGE(ctx, NULL, REDGPU_PRINT_PROCEDURE_ID(procedureId), 62, "RedDriverProceduresAndAddresses::CreatePipelineLayout == NULL", optionalFile, optionalLine) }
  if (vk->vkDestroyPipelineLayout                        == NULL) { REDGPU_INTERNAL_DEBUG_SEND_ERROR_MESSAGE(ctx, NULL, REDGPU_PRINT_PROCEDURE_ID(procedureId), 63, "RedDriverProceduresAndAddresses::DestroyPipelineLayout == NULL", optionalFile, optionalLine) }
  if (vk->vkCreateDescriptorSetLayout                    == NULL) { REDGPU_INTERNAL_DEBUG_SEND_ERROR_MESSAGE(ctx, NULL, REDGPU_PRINT_PROCEDURE_ID(procedureId), 64, "RedDriverProceduresAndAddresses::CreateDescriptorSetLayout == NULL", optionalFile, optionalLine) }
  if (vk->vkDestroyDescriptorSetLayout                   == NULL) { REDGPU_INTERNAL_DEBUG_SEND_ERROR_MESSAGE(ctx, NULL, REDGPU_PRINT_PROCEDURE_ID(procedureId), 65, "RedDriverProceduresAndAddresses::DestroyDescriptorSetLayout == NULL", optionalFile, optionalLine) }
  if (vk->vkCreateDescriptorPool                         == NULL) { REDGPU_INTERNAL_DEBUG_SEND_ERROR_MESSAGE(ctx, NULL, REDGPU_PRINT_PROCEDURE_ID(procedureId), 66, "RedDriverProceduresAndAddresses::CreateDescriptorPool == NULL", optionalFile, optionalLine) }
  if (vk->vkAllocateDescriptorSets                       == NULL) { REDGPU_INTERNAL_DEBUG_SEND_ERROR_MESSAGE(ctx, NULL, REDGPU_PRINT_PROCEDURE_ID(procedureId), 67, "RedDriverProceduresAndAddresses::AllocateDescriptorSets == NULL", optionalFile, optionalLine) }
  if (vk->vkResetDescriptorPool                          == NULL) { REDGPU_INTERNAL_DEBUG_SEND_ERROR_MESSAGE(ctx, NULL, REDGPU_PRINT_PROCEDURE_ID(procedureId), 68, "RedDriverProceduresAndAddresses::ResetDescriptorPool == NULL", optionalFile, optionalLine) }
  if (vk->vkDestroyDescriptorPool                        == NULL) { REDGPU_INTERNAL_DEBUG_SEND_ERROR_MESSAGE(ctx, NULL, REDGPU_PRINT_PROCEDURE_ID(procedureId), 69, "RedDriverProceduresAndAddresses::DestroyDescriptorPool == NULL", optionalFile, optionalLine) }
  if (vk->vkUpdateDescriptorSets                         == NULL) { REDGPU_INTERNAL_DEBUG_SEND_ERROR_MESSAGE(ctx, NULL, REDGPU_PRINT_PROCEDURE_ID(procedureId), 70, "RedDriverProceduresAndAddresses::UpdateDescriptorSets == NULL", optionalFile, optionalLine) }
  if (vk->vkCreateRenderPass                             == NULL) { REDGPU_INTERNAL_DEBUG_SEND_ERROR_MESSAGE(ctx, NULL, REDGPU_PRINT_PROCEDURE_ID(procedureId), 71, "RedDriverProceduresAndAddresses::CreateRenderPass == NULL", optionalFile, optionalLine) }
  if (vk->vkDestroyRenderPass                            == NULL) { REDGPU_INTERNAL_DEBUG_SEND_ERROR_MESSAGE(ctx, NULL, REDGPU_PRINT_PROCEDURE_ID(procedureId), 72, "RedDriverProceduresAndAddresses::DestroyRenderPass == NULL", optionalFile, optionalLine) }
  if (vk->vkCreateFramebuffer                            == NULL) { REDGPU_INTERNAL_DEBUG_SEND_ERROR_MESSAGE(ctx, NULL, REDGPU_PRINT_PROCEDURE_ID(procedureId), 73, "RedDriverProceduresAndAddresses::CreateFramebuffer == NULL", optionalFile, optionalLine) }
  if (vk->vkDestroyFramebuffer                           == NULL) { REDGPU_INTERNAL_DEBUG_SEND_ERROR_MESSAGE(ctx, NULL, REDGPU_PRINT_PROCEDURE_ID(procedureId), 74, "RedDriverProceduresAndAddresses::DestroyFramebuffer == NULL", optionalFile, optionalLine) }
  if (vk->vkCreateFence                                  == NULL) { REDGPU_INTERNAL_DEBUG_SEND_ERROR_MESSAGE(ctx, NULL, REDGPU_PRINT_PROCEDURE_ID(procedureId), 75, "RedDriverProceduresAndAddresses::CreateFence == NULL", optionalFile, optionalLine) }
  if (vk->vkDestroyFence                                 == NULL) { REDGPU_INTERNAL_DEBUG_SEND_ERROR_MESSAGE(ctx, NULL, REDGPU_PRINT_PROCEDURE_ID(procedureId), 76, "RedDriverProceduresAndAddresses::DestroyFence == NULL", optionalFile, optionalLine) }
  if (vk->vkGetFenceStatus                               == NULL) { REDGPU_INTERNAL_DEBUG_SEND_ERROR_MESSAGE(ctx, NULL, REDGPU_PRINT_PROCEDURE_ID(procedureId), 77, "RedDriverProceduresAndAddresses::GetFenceStatus == NULL", optionalFile, optionalLine) }
  if (vk->vkWaitForFences                                == NULL) { REDGPU_INTERNAL_DEBUG_SEND_ERROR_MESSAGE(ctx, NULL, REDGPU_PRINT_PROCEDURE_ID(procedureId), 78, "RedDriverProceduresAndAddresses::WaitForFences == NULL", optionalFile, optionalLine) }
  if (vk->vkResetFences                                  == NULL) { REDGPU_INTERNAL_DEBUG_SEND_ERROR_MESSAGE(ctx, NULL, REDGPU_PRINT_PROCEDURE_ID(procedureId), 79, "RedDriverProceduresAndAddresses::ResetFences == NULL", optionalFile, optionalLine) }
  if (vk->vkCreateSemaphore                              == NULL) { REDGPU_INTERNAL_DEBUG_SEND_ERROR_MESSAGE(ctx, NULL, REDGPU_PRINT_PROCEDURE_ID(procedureId), 80, "RedDriverProceduresAndAddresses::CreateSemaphore == NULL", optionalFile, optionalLine) }
  if (vk->vkDestroySemaphore                             == NULL) { REDGPU_INTERNAL_DEBUG_SEND_ERROR_MESSAGE(ctx, NULL, REDGPU_PRINT_PROCEDURE_ID(procedureId), 81, "RedDriverProceduresAndAddresses::DestroySemaphore == NULL", optionalFile, optionalLine) }
  if (vk->vkCreateEvent                                  == NULL) { REDGPU_INTERNAL_DEBUG_SEND_ERROR_MESSAGE(ctx, NULL, REDGPU_PRINT_PROCEDURE_ID(procedureId), 82, "RedDriverProceduresAndAddresses::CreateEvent == NULL", optionalFile, optionalLine) }
  if (vk->vkDestroyEvent                                 == NULL) { REDGPU_INTERNAL_DEBUG_SEND_ERROR_MESSAGE(ctx, NULL, REDGPU_PRINT_PROCEDURE_ID(procedureId), 83, "RedDriverProceduresAndAddresses::DestroyEvent == NULL", optionalFile, optionalLine) }
  if (vk->vkGetEventStatus                               == NULL) { REDGPU_INTERNAL_DEBUG_SEND_ERROR_MESSAGE(ctx, NULL, REDGPU_PRINT_PROCEDURE_ID(procedureId), 84, "RedDriverProceduresAndAddresses::GetEventStatus == NULL", optionalFile, optionalLine) }
  if (vk->vkResetEvent                                   == NULL) { REDGPU_INTERNAL_DEBUG_SEND_ERROR_MESSAGE(ctx, NULL, REDGPU_PRINT_PROCEDURE_ID(procedureId), 85, "RedDriverProceduresAndAddresses::ResetEvent == NULL", optionalFile, optionalLine) }
  if (vk->vkCreateCommandPool                            == NULL) { REDGPU_INTERNAL_DEBUG_SEND_ERROR_MESSAGE(ctx, NULL, REDGPU_PRINT_PROCEDURE_ID(procedureId), 86, "RedDriverProceduresAndAddresses::CreateCommandPool == NULL", optionalFile, optionalLine) }
  if (vk->vkAllocateCommandBuffers                       == NULL) { REDGPU_INTERNAL_DEBUG_SEND_ERROR_MESSAGE(ctx, NULL, REDGPU_PRINT_PROCEDURE_ID(procedureId), 87, "RedDriverProceduresAndAddresses::AllocateCommandBuffers == NULL", optionalFile, optionalLine) }
  if (vk->vkDestroyCommandPool                           == NULL) { REDGPU_INTERNAL_DEBUG_SEND_ERROR_MESSAGE(ctx, NULL, REDGPU_PRINT_PROCEDURE_ID(procedureId), 88, "RedDriverProceduresAndAddresses::DestroyCommandPool == NULL", optionalFile, optionalLine) }
  if (vk->vkResetCommandPool                             == NULL) { REDGPU_INTERNAL_DEBUG_SEND_ERROR_MESSAGE(ctx, NULL, REDGPU_PRINT_PROCEDURE_ID(procedureId), 89, "RedDriverProceduresAndAddresses::ResetCommandPool == NULL", optionalFile, optionalLine) }
  if (vk->vkBeginCommandBuffer                           == NULL) { REDGPU_INTERNAL_DEBUG_SEND_ERROR_MESSAGE(ctx, NULL, REDGPU_PRINT_PROCEDURE_ID(procedureId), 90, "RedDriverProceduresAndAddresses::BeginCommandBuffer == NULL", optionalFile, optionalLine) }
  if (vk->vkEndCommandBuffer                             == NULL) { REDGPU_INTERNAL_DEBUG_SEND_ERROR_MESSAGE(ctx, NULL, REDGPU_PRINT_PROCEDURE_ID(procedureId), 91, "RedDriverProceduresAndAddresses::EndCommandBuffer == NULL", optionalFile, optionalLine) }
  if (vk->vkCmdInsertDebugUtilsLabelEXT                  == NULL) { REDGPU_INTERNAL_DEBUG_SEND_ERROR_MESSAGE(ctx, NULL, REDGPU_PRINT_PROCEDURE_ID(procedureId), 92, "RedDriverProceduresAndAddresses::CmdInsertDebugUtilsLabel == NULL", optionalFile, optionalLine) }
  if (vk->vkCmdBeginDebugUtilsLabelEXT                   == NULL) { REDGPU_INTERNAL_DEBUG_SEND_ERROR_MESSAGE(ctx, NULL, REDGPU_PRINT_PROCEDURE_ID(procedureId), 93, "RedDriverProceduresAndAddresses::CmdBeginDebugUtilsLabel == NULL", optionalFile, optionalLine) }
  if (vk->vkCmdEndDebugUtilsLabelEXT                     == NULL) { REDGPU_INTERNAL_DEBUG_SEND_ERROR_MESSAGE(ctx, NULL, REDGPU_PRINT_PROCEDURE_ID(procedureId), 94, "RedDriverProceduresAndAddresses::CmdEndDebugUtilsLabel == NULL", optionalFile, optionalLine) }
  if (vk->vkQueueSubmit                                  == NULL) { REDGPU_INTERNAL_DEBUG_SEND_ERROR_MESSAGE(ctx, NULL, REDGPU_PRINT_PROCEDURE_ID(procedureId), 95, "RedDriverProceduresAndAddresses::QueueSubmit == NULL", optionalFile, optionalLine) }
  if (vk->vkQueueWaitIdle                                == NULL) { REDGPU_INTERNAL_DEBUG_SEND_ERROR_MESSAGE(ctx, NULL, REDGPU_PRINT_PROCEDURE_ID(procedureId), 96, "RedDriverProceduresAndAddresses::QueueWaitIdle == NULL", optionalFile, optionalLine) }
#if defined(VK_USE_PLATFORM_WIN32_KHR)
  if (vk->vkCreateWin32SurfaceKHR                        == NULL) { REDGPU_INTERNAL_DEBUG_SEND_ERROR_MESSAGE(ctx, NULL, REDGPU_PRINT_PROCEDURE_ID(procedureId), 97, "RedDriverProceduresAndAddresses::CreateWin32Surface == NULL", optionalFile, optionalLine) }
  if (vk->vkGetPhysicalDeviceWin32PresentationSupportKHR == NULL) { REDGPU_INTERNAL_DEBUG_SEND_ERROR_MESSAGE(ctx, NULL, REDGPU_PRINT_PROCEDURE_ID(procedureId), 98, "RedDriverProceduresAndAddresses::GetPhysicalDeviceWin32PresentationSupport == NULL", optionalFile, optionalLine) }
#endif
#if defined(VK_USE_PLATFORM_XLIB_KHR) && defined(VK_USE_PLATFORM_XCB_KHR)
  if (vk->vkCreateXcbSurfaceKHR                          == NULL) { REDGPU_INTERNAL_DEBUG_SEND_ERROR_MESSAGE(ctx, NULL, REDGPU_PRINT_PROCEDURE_ID(procedureId), 99, "RedDriverProceduresAndAddresses::CreateXcbSurface == NULL", optionalFile, optionalLine) }
  if (vk->vkGetPhysicalDeviceXcbPresentationSupportKHR   == NULL) { REDGPU_INTERNAL_DEBUG_SEND_ERROR_MESSAGE(ctx, NULL, REDGPU_PRINT_PROCEDURE_ID(procedureId), 100, "RedDriverProceduresAndAddresses::GetPhysicalDeviceXcbPresentationSupport == NULL", optionalFile, optionalLine) }
  if (vk->vkCreateXlibSurfaceKHR                         == NULL) { REDGPU_INTERNAL_DEBUG_SEND_ERROR_MESSAGE(ctx, NULL, REDGPU_PRINT_PROCEDURE_ID(procedureId), 101, "RedDriverProceduresAndAddresses::CreateXlibSurface == NULL", optionalFile, optionalLine) }
  if (vk->vkGetPhysicalDeviceXlibPresentationSupportKHR  == NULL) { REDGPU_INTERNAL_DEBUG_SEND_ERROR_MESSAGE(ctx, NULL, REDGPU_PRINT_PROCEDURE_ID(procedureId), 102, "RedDriverProceduresAndAddresses::GetPhysicalDeviceXlibPresentationSupport == NULL", optionalFile, optionalLine) }
#endif
  if (vk->vkDestroySurfaceKHR                            == NULL) { REDGPU_INTERNAL_DEBUG_SEND_ERROR_MESSAGE(ctx, NULL, REDGPU_PRINT_PROCEDURE_ID(procedureId), 103, "RedDriverProceduresAndAddresses::DestroySurface == NULL", optionalFile, optionalLine) }
  if (vk->vkCreateSwapchainKHR                           == NULL) { REDGPU_INTERNAL_DEBUG_SEND_ERROR_MESSAGE(ctx, NULL, REDGPU_PRINT_PROCEDURE_ID(procedureId), 104, "RedDriverProceduresAndAddresses::CreateSwapchain == NULL", optionalFile, optionalLine) }
  if (vk->vkGetSwapchainImagesKHR                        == NULL) { REDGPU_INTERNAL_DEBUG_SEND_ERROR_MESSAGE(ctx, NULL, REDGPU_PRINT_PROCEDURE_ID(procedureId), 105, "RedDriverProceduresAndAddresses::GetSwapchainImages == NULL", optionalFile, optionalLine) }
  if (vk->vkDestroySwapchainKHR                          == NULL) { REDGPU_INTERNAL_DEBUG_SEND_ERROR_MESSAGE(ctx, NULL, REDGPU_PRINT_PROCEDURE_ID(procedureId), 106, "RedDriverProceduresAndAddresses::DestroySwapchain == NULL", optionalFile, optionalLine) }
  if (vk->vkGetPhysicalDeviceSurfaceSupportKHR           == NULL) { REDGPU_INTERNAL_DEBUG_SEND_ERROR_MESSAGE(ctx, NULL, REDGPU_PRINT_PROCEDURE_ID(procedureId), 107, "RedDriverProceduresAndAddresses::GetPhysicalDeviceSurfaceSupport == NULL", optionalFile, optionalLine) }
  if (vk->vkGetPhysicalDeviceSurfaceFormatsKHR           == NULL) { REDGPU_INTERNAL_DEBUG_SEND_ERROR_MESSAGE(ctx, NULL, REDGPU_PRINT_PROCEDURE_ID(procedureId), 108, "RedDriverProceduresAndAddresses::GetPhysicalDeviceSurfaceFormats == NULL", optionalFile, optionalLine) }
  if (vk->vkGetPhysicalDeviceSurfaceCapabilitiesKHR      == NULL) { REDGPU_INTERNAL_DEBUG_SEND_ERROR_MESSAGE(ctx, NULL, REDGPU_PRINT_PROCEDURE_ID(procedureId), 109, "RedDriverProceduresAndAddresses::GetPhysicalDeviceSurfaceCapabilities == NULL", optionalFile, optionalLine) }
  if (vk->vkGetPhysicalDeviceSurfacePresentModesKHR      == NULL) { REDGPU_INTERNAL_DEBUG_SEND_ERROR_MESSAGE(ctx, NULL, REDGPU_PRINT_PROCEDURE_ID(procedureId), 110, "RedDriverProceduresAndAddresses::GetPhysicalDeviceSurfacePresentModes == NULL", optionalFile, optionalLine) }
  if (vk->vkAcquireNextImageKHR                          == NULL) { REDGPU_INTERNAL_DEBUG_SEND_ERROR_MESSAGE(ctx, NULL, REDGPU_PRINT_PROCEDURE_ID(procedureId), 111, "RedDriverProceduresAndAddresses::AcquireNextImage == NULL", optionalFile, optionalLine) }
  if (vk->vkQueuePresentKHR                              == NULL) { REDGPU_INTERNAL_DEBUG_SEND_ERROR_MESSAGE(ctx, NULL, REDGPU_PRINT_PROCEDURE_ID(procedureId), 112, "RedDriverProceduresAndAddresses::QueuePresent == NULL", optionalFile, optionalLine) }
  if (vk->vkCreateRenderPass2KHR                         == NULL) { REDGPU_INTERNAL_DEBUG_SEND_ERROR_MESSAGE(ctx, NULL, REDGPU_PRINT_PROCEDURE_ID(procedureId), 113, "RedDriverProceduresAndAddresses::CreateRenderPass2 == NULL", optionalFile, optionalLine) }
  if (vk->vkSubmitDebugUtilsMessageEXT                   == NULL) { REDGPU_INTERNAL_DEBUG_SEND_ERROR_MESSAGE(ctx, NULL, REDGPU_PRINT_PROCEDURE_ID(procedureId), 114, "RedDriverProceduresAndAddresses::SubmitDebugUtilsMessage == NULL", optionalFile, optionalLine) }
  if (vk->vkBindBufferMemory2KHR                         == NULL) { REDGPU_INTERNAL_DEBUG_SEND_ERROR_MESSAGE(ctx, NULL, REDGPU_PRINT_PROCEDURE_ID(procedureId), 115, "RedDriverProceduresAndAddresses::BindBufferMemory2 == NULL", optionalFile, optionalLine) }
  if (vk->vkBindImageMemory2KHR                          == NULL) { REDGPU_INTERNAL_DEBUG_SEND_ERROR_MESSAGE(ctx, NULL, REDGPU_PRINT_PROCEDURE_ID(procedureId), 116, "RedDriverProceduresAndAddresses::BindImageMemory2 == NULL", optionalFile, optionalLine) }
  if (vk->vkSetEvent                                     == NULL) { REDGPU_INTERNAL_DEBUG_SEND_ERROR_MESSAGE(ctx, NULL, REDGPU_PRINT_PROCEDURE_ID(procedureId), 117, "RedDriverProceduresAndAddresses::SetEvent == NULL", optionalFile, optionalLine) }
  if (vk->vkCmdWaitEvents                                == NULL) { REDGPU_INTERNAL_DEBUG_SEND_ERROR_MESSAGE(ctx, NULL, REDGPU_PRINT_PROCEDURE_ID(procedureId), 118, "RedDriverProceduresAndAddresses::CmdWaitEvents == NULL", optionalFile, optionalLine) }
  if (vk->vkCmdResetEvent                                == NULL) { REDGPU_INTERNAL_DEBUG_SEND_ERROR_MESSAGE(ctx, NULL, REDGPU_PRINT_PROCEDURE_ID(procedureId), 119, "RedDriverProceduresAndAddresses::CmdResetEvent == NULL", optionalFile, optionalLine) }
  if (vk->vkCreateQueryPool                              == NULL) { REDGPU_INTERNAL_DEBUG_SEND_ERROR_MESSAGE(ctx, NULL, REDGPU_PRINT_PROCEDURE_ID(procedureId), 371, "RedDriverProceduresAndAddresses::CreateQueryPool == NULL", optionalFile, optionalLine) }
  if (vk->vkDestroyQueryPool                             == NULL) { REDGPU_INTERNAL_DEBUG_SEND_ERROR_MESSAGE(ctx, NULL, REDGPU_PRINT_PROCEDURE_ID(procedureId), 372, "RedDriverProceduresAndAddresses::DestroyQueryPool == NULL", optionalFile, optionalLine) }
  if (vk->vkCmdResetQueryPool                            == NULL) { REDGPU_INTERNAL_DEBUG_SEND_ERROR_MESSAGE(ctx, NULL, REDGPU_PRINT_PROCEDURE_ID(procedureId), 373, "RedDriverProceduresAndAddresses::CmdResetQueryPool == NULL", optionalFile, optionalLine) }
  if (vk->vkCmdWriteTimestamp                            == NULL) { REDGPU_INTERNAL_DEBUG_SEND_ERROR_MESSAGE(ctx, NULL, REDGPU_PRINT_PROCEDURE_ID(procedureId), 374, "RedDriverProceduresAndAddresses::CmdWriteTimestamp == NULL", optionalFile, optionalLine) }
  if (vk->vkGetQueryPoolResults                          == NULL) { REDGPU_INTERNAL_DEBUG_SEND_ERROR_MESSAGE(ctx, NULL, REDGPU_PRINT_PROCEDURE_ID(procedureId), 375, "RedDriverProceduresAndAddresses::GetQueryPoolResults == NULL", optionalFile, optionalLine) }
#endif

#if defined(REDGPU_OS_ANDROID)
  if (requestedDebugMode == 1) {
    VkDebugReportCallbackCreateInfoEXT info;
    info.sType       = (VkStructureType)VK_STRUCTURE_TYPE_DEBUG_REPORT_CREATE_INFO_EXT;
    info.pNext       = NULL;
    info.flags       = 0;
    info.pfnCallback = (void *)debugCallback;
    info.pUserData   = (void *)ctx;
    redInternalSetStatus(ctx, NULL, outStatuses, 246, vk->vkCreateDebugReportCallbackEXT((VkInstance)ctx->contextHandle, &info, NULL, &cold->vkDebugReportCallback), procedureId, optionalFile, optionalLine, optionalUserData);
  }
  ctx->internal.debugUtilsMessengerCallback = (void *)debugCallback; // NOTE(Constantine): On Android, pass debugCallback to redCreateContext to enable debug mode.
#else
  if (requestedDebugMode == 1) {
    VkDebugUtilsMessengerCreateInfoEXT info;
    info.sType           = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    info.pNext           = NULL;
    info.flags           = 0;
    info.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                           VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    info.messageType     = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT     |
                           VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT;
                        // VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT
    info.pfnUserCallback = (PFN_vkDebugUtilsMessengerCallbackEXT)debugCallback;
    info.pUserData       = (void *)ctx;
    redInternalSetStatus(ctx, NULL, outStatuses, 246, vk->vkCreateDebugUtilsMessengerEXT((VkInstance)ctx->contextHandle, &info, NULL, &cold->vkDebugUtilsMessenger), procedureId, optionalFile, optionalLine, optionalUserData);
  }
  ctx->internal.debugUtilsMessengerCallback = (PFN_vkDebugUtilsMessengerCallbackEXT)debugCallback;
#endif

  redInternalSetStatus(ctx, NULL, outStatuses, 247, vk->vkEnumeratePhysicalDevices((VkInstance)ctx->contextHandle, &cold->vkPhysicalDevicesCount, NULL), procedureId, optionalFile, optionalLine, optionalUserData);
#if REDGPU_COMPILE_SWITCH == REDGPU_COMPILE_SWITCH_DEBUG
  {
    std::lock_guard<std::mutex> __debugPrintGuardAndSerializeMutexScopeGuard(__REDGPU_DEBUG_GLOBAL_3f5f84a81695ef9521b544bdc3d7d73cbb4aac3c18ce99fd19bdfd306996103e_debugPrintGuardAndSerializeMutex);
    if (optionalFile != NULL) {
      REDGPU_PRINTF("[REDGPU DEBUG LIBRARY][RED_PROCEDURE_ID_redCreateContext][%s:%d]: Available GPUs count: %d\n", optionalFile, optionalLine, cold->vkPhysicalDevicesCount);
    } else {
      REDGPU_PRINTF("[REDGPU DEBUG LIBRARY][RED_PROCEDURE_ID_redCreateContext]: Available GPUs count: %d\n", cold->vkPhysicalDevicesCount);
    }
  }
#endif
  if (cold->vkPhysicalDevicesCount > 0 && settings1ExposeOnlyOneGpu == 1) {
    cold->vkPhysicalDevicesCount = 1;
  }
  REDGPU_CREATE_CONTEXT_ARRAY_MALLOC(NULL, 120, cold->vkPhysicalDevices, cold->vkPhysicalDevicesCount, VkPhysicalDevice, 6);
  if (settingsContextFromVkPassed == 1) {
    for (unsigned i = 0; i < cold->vkPhysicalDevicesCount; i += 1) {
      cold->vkPhysicalDevices[i] = (VkPhysicalDevice)settingsContextFromVkPhysicalDevices[i];
    }
  } else {
    redInternalSetStatus(ctx, NULL, outStatuses, 248, vk->vkEnumeratePhysicalDevices((VkInstance)ctx->contextHandle, &cold->vkPhysicalDevicesCount, cold->vkPhysicalDevices), procedureId, optionalFile, optionalLine, optionalUserData);
  }

  ctx->gpusCount = cold->vkPhysicalDevicesCount;
  REDGPU_CREATE_CONTEXT_ARRAY_MALLOC(NULL, 121, ctx->gpus, ctx->gpusCount, RedGpuInfo, 7);
  REDGPU_ARRAY_MEMSET(ctx->gpus, ctx->gpusCount);

  REDGPU_CREATE_CONTEXT_ARRAY_MALLOC(NULL, 122, ctx->gpusStatuses, ctx->gpusCount, RedStatuses, 8);
  REDGPU_ARRAY_MEMSET(ctx->gpusStatuses, ctx->gpusCount);
  for (unsigned i = 0; i < ctx->gpusCount; i += 1) {
    ctx->gpusStatuses[i].statusProcedureId      = procedureId;
    ctx->gpusStatuses[i].statusFile             = optionalFile;
    ctx->gpusStatuses[i].statusLine             = optionalLine;
    ctx->gpusStatuses[i].statusErrorProcedureId = procedureId;
    ctx->gpusStatuses[i].statusErrorFile        = optionalFile;
    ctx->gpusStatuses[i].statusErrorLine        = optionalLine;
  }

  ctx->internal.gpuProcedureAddressesCount = cold->vkPhysicalDevicesCount;
  REDGPU_CREATE_CONTEXT_ARRAY_MALLOC(NULL, 123, ctx->internal.gpuProcedureAddresses, ctx->internal.gpuProcedureAddressesCount, RedInternalGpuProcedureAddresses, 9);
  REDGPU_ARRAY_MEMSET(ctx->internal.gpuProcedureAddresses, ctx->internal.gpuProcedureAddressesCount);

  ctx->internal.hotGpuHandlesCount = cold->vkPhysicalDevicesCount;
  REDGPU_CREATE_CONTEXT_ARRAY_MALLOC(NULL, 124, ctx->internal.hotGpuHandles, ctx->internal.hotGpuHandlesCount, RedHandleGpu, 10);
  REDGPU_ARRAY_MEMSET(ctx->internal.hotGpuHandles, ctx->internal.hotGpuHandlesCount);

  ctx->internal.gpuExtensionBitflagsCount = cold->vkPhysicalDevicesCount;
  REDGPU_CREATE_CONTEXT_ARRAY_MALLOC(NULL, 125, ctx->internal.gpuExtensionBitflags, ctx->internal.gpuExtensionBitflagsCount, unsigned, 11);
  REDGPU_ARRAY_MEMSET(ctx->internal.gpuExtensionBitflags, ctx->internal.gpuExtensionBitflagsCount);

  cold->gpusCount = cold->vkPhysicalDevicesCount;
  REDGPU_CREATE_CONTEXT_ARRAY_MALLOC(NULL, 126, cold->gpus, cold->gpusCount, RedInternalGpuInfo, 12);
  REDGPU_ARRAY_MEMSET(cold->gpus, cold->gpusCount);

  const unsigned gpusCount = ctx->gpusCount;

  for (unsigned i = 0; i < gpusCount; i += 1) {
    RedGpuInfo * eGpuInfo = &ctx->gpus[i];

    eGpuInfo->gpuDevice = (RedHandleGpuDevice)cold->vkPhysicalDevices[i];
  }

  for (unsigned i = 0; i < gpusCount; i += 1) {
    VkPhysicalDevice     device   =  cold->vkPhysicalDevices[i];
    RedInternalGpuInfo * iGpuInfo = &cold->gpus[i];

    redInternalSetStatus(ctx, NULL, &ctx->gpusStatuses[i], 249, vk->vkEnumerateDeviceExtensionProperties(device, NULL, &iGpuInfo->vkDeviceExtensionPropertiesCount, NULL), procedureId, optionalFile, optionalLine, optionalUserData);
    REDGPU_CREATE_CONTEXT_ARRAY_MALLOC(NULL, 127, iGpuInfo->vkDeviceExtensionProperties, iGpuInfo->vkDeviceExtensionPropertiesCount, VkExtensionProperties, 13);
    redInternalSetStatus(ctx, NULL, &ctx->gpusStatuses[i], 250, vk->vkEnumerateDeviceExtensionProperties(device, NULL, &iGpuInfo->vkDeviceExtensionPropertiesCount, iGpuInfo->vkDeviceExtensionProperties), procedureId, optionalFile, optionalLine, optionalUserData);
  }

#if REDGPU_COMPILE_SWITCH == REDGPU_COMPILE_SWITCH_DEBUG
  {
    std::lock_guard<std::mutex> __debugPrintGuardAndSerializeMutexScopeGuard(__REDGPU_DEBUG_GLOBAL_3f5f84a81695ef9521b544bdc3d7d73cbb4aac3c18ce99fd19bdfd306996103e_debugPrintGuardAndSerializeMutex);
    for (unsigned i = 0; i < gpusCount; i += 1) {
      RedInternalGpuInfo * iGpuInfo = &cold->gpus[i];

      for (unsigned j = 0; j < iGpuInfo->vkDeviceExtensionPropertiesCount; j += 1) {
        VkExtensionProperties * extensionProperties = &iGpuInfo->vkDeviceExtensionProperties[j];
        if (optionalFile != NULL) {
          REDGPU_PRINTF("[REDGPU DEBUG LIBRARY][RED_PROCEDURE_ID_redCreateContext][%s:%d]: Available GPU[%d] extension: %s\n", optionalFile, optionalLine, i, extensionProperties->extensionName);
        } else {
          REDGPU_PRINTF("[REDGPU DEBUG LIBRARY][RED_PROCEDURE_ID_redCreateContext]: Available GPU[%d] extension: %s\n", i, extensionProperties->extensionName);
        }
      }
    }
  }
#endif

  for (unsigned i = 0; i < gpusCount; i += 1) {
    RedInternalGpuInfo * iGpuInfo = &cold->gpus[i];

    if (redInternalFindDeviceExtension(iGpuInfo, "VK_KHR_maintenance1", procedureId, 1, optionalFile, optionalLine) == 0) {
      redInternalSetStatus(ctx, NULL, &ctx->gpusStatuses[i], 251, (VkResult)RED_STATUS_ERROR_EXTENSION_IS_NOT_FOUND, procedureId, optionalFile, optionalLine, optionalUserData);
    }
#if defined(REDGPU_OS_ANDROID)
#else
    if (redInternalFindDeviceExtension(iGpuInfo, "VK_KHR_maintenance2", procedureId, 1, optionalFile, optionalLine) == 0) {
      redInternalSetStatus(ctx, NULL, &ctx->gpusStatuses[i], 252, (VkResult)RED_STATUS_ERROR_EXTENSION_IS_NOT_FOUND, procedureId, optionalFile, optionalLine, optionalUserData);
    }
#endif
#if defined(_WIN32) && !defined(_WIN64)
    // NOTE(Constantine): Nothing, intentionally.
#elif defined(REDGPU_OS_ANDROID)
    // NOTE(Constantine): Nothing, intentionally.
#else
    if (redInternalFindDeviceExtension(iGpuInfo, "VK_KHR_maintenance3", procedureId, 1, optionalFile, optionalLine) == 0) {
      redInternalSetStatus(ctx, NULL, &ctx->gpusStatuses[i], 253, (VkResult)RED_STATUS_ERROR_EXTENSION_IS_NOT_FOUND, procedureId, optionalFile, optionalLine, optionalUserData);
    }
#endif
  }

  for (unsigned i = 0; i < gpusCount; i += 1) {
    if (ctx->gpusStatuses[i].statusError != RED_STATUS_SUCCESS) {
      continue;
    }

    RedGpuInfo         * eGpuInfo = &ctx->gpus[i];
    RedInternalGpuInfo * iGpuInfo = &cold->gpus[i];

    eGpuInfo->supportsWsi = redInternalFindDeviceExtension(iGpuInfo, "VK_KHR_swapchain", procedureId, 0, optionalFile, optionalLine);
  }

  for (unsigned i = 0; i < gpusCount; i += 1) {
    if (ctx->gpusStatuses[i].statusError != RED_STATUS_SUCCESS) {
      continue;
    }

    struct VkConformanceVersionKHR {
      unsigned char versionMajor;
      unsigned char versionMinor;
      unsigned char versionSubminor;
      unsigned char versionPatch;
    };

    struct VkPhysicalDeviceDriverPropertiesKHR {
      unsigned                       sType;
      void *                         pNext;
      RedDriverId                    driverID;
      char                           driverName[256];
      char                           driverInfo[256];
      struct VkConformanceVersionKHR conformanceVersion;
    };

    struct VkPhysicalDeviceDepthStencilResolvePropertiesKHR {
      unsigned sType;
      void *   pNext;
      unsigned supportedDepthResolveModes;
      unsigned supportedStencilResolveModes;
      VkBool32 independentResolveNone;
      VkBool32 independentResolve;
    };

    struct VkPhysicalDevicePushDescriptorPropertiesKHR {
      unsigned sType;
      void *   pNext;
      uint32_t maxPushDescriptors;
    };

    struct VkPhysicalDeviceConservativeRasterizationPropertiesEXT {
      unsigned sType;
      void *   pNext;
      float    primitiveOverestimationSize;
      float    maxExtraPrimitiveOverestimationSize;
      float    extraPrimitiveOverestimationSizeGranularity;
      VkBool32 primitiveUnderestimation;
      VkBool32 conservativePointAndLineRasterization;
      VkBool32 degenerateTrianglesRasterized;
      VkBool32 degenerateLinesRasterized;
      VkBool32 fullyCoveredFragmentShaderInputVariable;
      VkBool32 conservativeRasterizationPostDepthCoverage;
    };

    RedGpuInfo         * eGpuInfo = &ctx->gpus[i];
    RedInternalGpuInfo * iGpuInfo = &cold->gpus[i];

    VkPhysicalDevice device = cold->vkPhysicalDevices[i];

    VkPhysicalDeviceMemoryProperties2 physicalDeviceMemoryProperties2;
    physicalDeviceMemoryProperties2.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MEMORY_PROPERTIES_2;
    physicalDeviceMemoryProperties2.pNext = NULL;
    // physicalDeviceMemoryProperties2.memoryProperties
    vk->vkGetPhysicalDeviceMemoryProperties2KHR(device, &physicalDeviceMemoryProperties2);

    iGpuInfo->vkPhysicalDeviceMemoryProperties = physicalDeviceMemoryProperties2.memoryProperties;

    void * featuresToRequestNext = NULL;

    VkPhysicalDeviceVulkanMemoryModelFeaturesKHR vulkanMemoryModelFeatures;
    vulkanMemoryModelFeatures.sType                                         = 1000211000; // VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_MEMORY_MODEL_FEATURES_KHR
    vulkanMemoryModelFeatures.pNext                                         = NULL;
    vulkanMemoryModelFeatures.vulkanMemoryModel                             = 0;
    vulkanMemoryModelFeatures.vulkanMemoryModelDeviceScope                  = 0;
    vulkanMemoryModelFeatures.vulkanMemoryModelAvailabilityVisibilityChains = 0;

    RedBool32 wasRequestedAndWasFoundExtensionVulkanMemoryModelFeatures = (requestedExtensions[RED_SDK_EXTENSION_FORMAL_MEMORY_MODEL] == 1) && (redInternalFindDeviceExtension(iGpuInfo, "VK_KHR_vulkan_memory_model", procedureId, 0, optionalFile, optionalLine) == 1);

    if (wasRequestedAndWasFoundExtensionVulkanMemoryModelFeatures == 1) {
      vulkanMemoryModelFeatures.pNext = featuresToRequestNext;
      featuresToRequestNext = &vulkanMemoryModelFeatures;
    }

    VkPhysicalDeviceFeatures2 physicalDeviceFeatures2;
    physicalDeviceFeatures2.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2;
    physicalDeviceFeatures2.pNext = featuresToRequestNext;
    // physicalDeviceFeatures2.features
    vk->vkGetPhysicalDeviceFeatures2KHR(device, &physicalDeviceFeatures2);

    iGpuInfo->vkPhysicalDeviceFeatures                  = physicalDeviceFeatures2.features;
    iGpuInfo->vkPhysicalDeviceVulkanMemoryModelFeatures = vulkanMemoryModelFeatures;

    void * optionalInfoNext = NULL;

    if (requestedExtensions[RED_SDK_EXTENSION_COMPUTING_LANGUAGE_FEATURE_LEVEL_1] == 1) {
      RedBool32 wasRequestedAndWasFoundExtensionComputingLanguage = requestedExtensions[RED_SDK_EXTENSION_COMPUTING_LANGUAGE_FEATURE_LEVEL_1];
      {
        if (redInternalFindDeviceExtension(iGpuInfo, "VK_KHR_storage_buffer_storage_class", procedureId, 0, optionalFile, optionalLine) == 0) {
          wasRequestedAndWasFoundExtensionComputingLanguage = 0;
        }
        if (redInternalFindDeviceExtension(iGpuInfo, "VK_KHR_variable_pointers", procedureId, 0, optionalFile, optionalLine) == 0) {
          wasRequestedAndWasFoundExtensionComputingLanguage = 0;
        }
      }

      RedGpuInfoOptionalInfoComputingLanguageFeatureLevel1 * info = NULL;
      REDGPU_CREATE_CONTEXT_ARRAY_MALLOC(NULL, 368, info, 1, RedGpuInfoOptionalInfoComputingLanguageFeatureLevel1, 100);
      info->optionalInfo                           = RED_GPU_INFO_OPTIONAL_INFO_COMPUTING_LANGUAGE_FEATURE_LEVEL_1;
      info->next                                   = optionalInfoNext;
      info->supportsComputingLanguageFeatureLevel1 = wasRequestedAndWasFoundExtensionComputingLanguage;
      optionalInfoNext = info;
    }

    if (requestedExtensions[RED_SDK_EXTENSION_RAY_TRACING] == 1) {
      VkPhysicalDeviceProperties2 deviceProperties2;
      deviceProperties2.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2;
      deviceProperties2.pNext = NULL;
      // deviceProperties2.properties
      vk->vkGetPhysicalDeviceProperties2KHR(device, &deviceProperties2);

      RedBool32 wasRequestedAndWasFoundExtensionRayTracing = requestedExtensions[RED_SDK_EXTENSION_RAY_TRACING];
      {
        const unsigned vendorIDAmd    = 0x1002;
        const unsigned vendorIDNvidia = 0x10de;
        const unsigned vendorIDIntel  = 0x8086;
        if (deviceProperties2.properties.vendorID == vendorIDAmd) {
          if (redInternalFindDeviceExtension(iGpuInfo, "VK_AMD_shader_ballot", procedureId, 0, optionalFile, optionalLine) == 0) {
            wasRequestedAndWasFoundExtensionRayTracing = 0;
          }
        } else if (deviceProperties2.properties.vendorID == vendorIDNvidia) {
          if (redInternalFindDeviceExtension(iGpuInfo, "VK_NV_shader_subgroup_partitioned", procedureId, 0, optionalFile, optionalLine) == 0) {
            wasRequestedAndWasFoundExtensionRayTracing = 0;
          }
        } else {
          // NOTE(Constantine): Ray tracing on Intel GPUs is disabled for now.
          wasRequestedAndWasFoundExtensionRayTracing = 0;
        }
        if (redInternalFindDeviceExtension(iGpuInfo, "VK_EXT_shader_subgroup_ballot", procedureId, 0, optionalFile, optionalLine) == 0) {
          wasRequestedAndWasFoundExtensionRayTracing = 0;
        }
        if (redInternalFindDeviceExtension(iGpuInfo, "VK_EXT_descriptor_indexing", procedureId, 0, optionalFile, optionalLine) == 0) {
          wasRequestedAndWasFoundExtensionRayTracing = 0;
        }
      }

      RedGpuInfoOptionalInfoRayTracing * info = NULL;
      REDGPU_CREATE_CONTEXT_ARRAY_MALLOC(NULL, 128, info, 1, RedGpuInfoOptionalInfoRayTracing, 14);
      info->optionalInfo       = RED_GPU_INFO_OPTIONAL_INFO_RAY_TRACING;
      info->next               = optionalInfoNext;
      info->supportsRayTracing = wasRequestedAndWasFoundExtensionRayTracing;
      optionalInfoNext = info;
    }

    {
      RedGpuInfoOptionalInfoAdditionalInfo0 * info = NULL;
      REDGPU_CREATE_CONTEXT_ARRAY_MALLOC(NULL, 129, info, 1, RedGpuInfoOptionalInfoAdditionalInfo0, 15);
      info->optionalInfo                                                                               = RED_GPU_INFO_OPTIONAL_INFO_ADDITIONAL_INFO_0;
      info->next                                                                                       = optionalInfoNext;
      info->supportsArraysImagesUsageBeforeMemorySet                                                   = 0;
      info->supportsInitialQueueFamilyIndexSetToMaxValueForSimultaneousQueueAccessOfArrays             = 1;
      info->supportsInitialQueueFamilyIndexSetToMaxValueForSimultaneousQueueAccessOfImages             = 1;
      info->supportsInitialQueueFamilyIndexSetToMaxValueForSimultaneousQueueAccessOfImagesMultisample  = 0;
      info->supportsInitialQueueFamilyIndexSetToMaxValueForSimultaneousQueueAccessOfImagesDepthStencil = 0;
      optionalInfoNext = info;
    }

    if (requestedExtensions[RED_SDK_EXTENSION_FORMAL_MEMORY_MODEL] == 1) {
      if (wasRequestedAndWasFoundExtensionVulkanMemoryModelFeatures == 1) {
        RedGpuInfoOptionalInfoFormalMemoryModel * info = NULL;
        REDGPU_CREATE_CONTEXT_ARRAY_MALLOC(NULL, 130, info, 1, RedGpuInfoOptionalInfoFormalMemoryModel, 16);
        info->optionalInfo                                             = RED_GPU_INFO_OPTIONAL_INFO_FORMAL_MEMORY_MODEL;
        info->next                                                     = optionalInfoNext;
        info->supportsFormalMemoryModel                                = vulkanMemoryModelFeatures.vulkanMemoryModel            == 1;
        info->supportsFormalMemoryModelGpuScopeSynchronization         = vulkanMemoryModelFeatures.vulkanMemoryModelDeviceScope == 1;
        info->supportsFormalMemoryModelAvailabilityAndVisibilityChains = vulkanMemoryModelFeatures.vulkanMemoryModelAvailabilityVisibilityChains == 1;
        optionalInfoNext = info;
      } else {
        RedGpuInfoOptionalInfoFormalMemoryModel * info = NULL;
        REDGPU_CREATE_CONTEXT_ARRAY_MALLOC(NULL, 131, info, 1, RedGpuInfoOptionalInfoFormalMemoryModel, 17);
        info->optionalInfo                                             = RED_GPU_INFO_OPTIONAL_INFO_FORMAL_MEMORY_MODEL;
        info->next                                                     = optionalInfoNext;
        info->supportsFormalMemoryModel                                = 0;
        info->supportsFormalMemoryModelGpuScopeSynchronization         = 0;
        info->supportsFormalMemoryModelAvailabilityAndVisibilityChains = 0;
        optionalInfoNext = info;
      }
    }

    void * propertiesToRequestNext = NULL;

    struct VkPhysicalDeviceDriverPropertiesKHR                    driverProperties                    = {0};
    struct VkPhysicalDeviceDepthStencilResolvePropertiesKHR       depthStencilResolveProperties       = {0};
    struct VkPhysicalDevicePushDescriptorPropertiesKHR            pushDescriptorProperties            = {0};
    struct VkPhysicalDeviceConservativeRasterizationPropertiesEXT conservativeRasterizationProperties = {0};
    VkPhysicalDeviceMaintenance3Properties                        maintenance3Properties              = {0};

    RedBool32 wasRequestedAndWasFoundExtensionDriverProperties           = (requestedExtensions[RED_SDK_EXTENSION_DRIVER_PROPERTIES]            == 1) && (redInternalFindDeviceExtension(iGpuInfo, "VK_KHR_driver_properties",          procedureId, 0, optionalFile, optionalLine) == 1);
    RedBool32 wasRequestedAndWasFoundExtensionDepthStencilResolve        = (requestedExtensions[RED_SDK_EXTENSION_RESOLVE_DEPTH_STENCIL]        == 1) && (redInternalFindDeviceExtension(iGpuInfo, "VK_KHR_depth_stencil_resolve",      procedureId, 0, optionalFile, optionalLine) == 1);
    RedBool32 wasRequestedAndWasFoundExtensionProcedureParametersHandles = (requestedExtensions[RED_SDK_EXTENSION_PROCEDURE_PARAMETERS_HANDLES] == 1) && (redInternalFindDeviceExtension(iGpuInfo, "VK_KHR_push_descriptor",            procedureId, 0, optionalFile, optionalLine) == 1);
    RedBool32 wasRequestedAndWasFoundExtensionRasterizationMode          = (requestedExtensions[RED_SDK_EXTENSION_RASTERIZATION_MODE]           == 1) && (redInternalFindDeviceExtension(iGpuInfo, "VK_EXT_conservative_rasterization", procedureId, 0, optionalFile, optionalLine) == 1);

    if (wasRequestedAndWasFoundExtensionDriverProperties == 1) {
      driverProperties.sType = 1000196000; // VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DRIVER_PROPERTIES_KHR
      driverProperties.pNext = propertiesToRequestNext;
      // driverProperties.driverID
      // driverProperties.driverName[256]
      // driverProperties.driverInfo[256]
      // driverProperties.conformanceVersion
      propertiesToRequestNext = &driverProperties;
    }

    if (wasRequestedAndWasFoundExtensionDepthStencilResolve == 1) {
      depthStencilResolveProperties.sType = 1000199000; // VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DEPTH_STENCIL_RESOLVE_PROPERTIES_KHR
      depthStencilResolveProperties.pNext = propertiesToRequestNext;
      // depthStencilResolveProperties.supportedDepthResolveModes
      // depthStencilResolveProperties.supportedStencilResolveModes
      // depthStencilResolveProperties.independentResolveNone
      // depthStencilResolveProperties.independentResolve
      propertiesToRequestNext = &depthStencilResolveProperties;
    }

    if (wasRequestedAndWasFoundExtensionProcedureParametersHandles == 1) {
      pushDescriptorProperties.sType = 1000080000; // VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PUSH_DESCRIPTOR_PROPERTIES_KHR
      pushDescriptorProperties.pNext = propertiesToRequestNext;
      // pushDescriptorProperties.maxPushDescriptors
      propertiesToRequestNext = &pushDescriptorProperties;
    }

    if (wasRequestedAndWasFoundExtensionRasterizationMode == 1) {
      conservativeRasterizationProperties.sType = 1000101000; // VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_CONSERVATIVE_RASTERIZATION_PROPERTIES_EXT
      conservativeRasterizationProperties.pNext = propertiesToRequestNext;
      // conservativeRasterizationProperties.primitiveOverestimationSize
      // conservativeRasterizationProperties.maxExtraPrimitiveOverestimationSize
      // conservativeRasterizationProperties.extraPrimitiveOverestimationSizeGranularity
      // conservativeRasterizationProperties.primitiveUnderestimation
      // conservativeRasterizationProperties.conservativePointAndLineRasterization
      // conservativeRasterizationProperties.degenerateTrianglesRasterized
      // conservativeRasterizationProperties.degenerateLinesRasterized
      // conservativeRasterizationProperties.fullyCoveredFragmentShaderInputVariable
      // conservativeRasterizationProperties.conservativeRasterizationPostDepthCoverage
      propertiesToRequestNext = &conservativeRasterizationProperties;
    }

#if defined(_WIN32) && !defined(_WIN64)
    // NOTE(Constantine): Nothing, intentionally.
#elif defined(REDGPU_OS_ANDROID)
    // NOTE(Constantine): Nothing, intentionally.
#else
    maintenance3Properties.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MAINTENANCE_3_PROPERTIES;
    maintenance3Properties.pNext = propertiesToRequestNext;
    // maintenance3Properties.maxPerSetDescriptors
    // maintenance3Properties.maxMemoryAllocationSize
    propertiesToRequestNext = &maintenance3Properties;
#endif

    VkPhysicalDeviceProperties2 deviceProperties2;
    deviceProperties2.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2;
    deviceProperties2.pNext = propertiesToRequestNext;
    // deviceProperties2.properties
    vk->vkGetPhysicalDeviceProperties2KHR(device, &deviceProperties2);

    iGpuInfo->vkPhysicalDeviceProperties  = deviceProperties2.properties;
#if (defined(_WIN32) && !defined(_WIN64)) || defined(REDGPU_OS_ANDROID)
    eGpuInfo->maxMemoryAllocateBytesCount = 1073741824;
    eGpuInfo->maxStructSlotsCount         = 1024;
#else
    eGpuInfo->maxMemoryAllocateBytesCount = maintenance3Properties.maxMemoryAllocationSize;
    eGpuInfo->maxStructSlotsCount         = maintenance3Properties.maxPerSetDescriptors;
#endif

    if (wasRequestedAndWasFoundExtensionDriverProperties == 1) {
      RedGpuInfoOptionalInfoDriverProperties * info = NULL;
      REDGPU_CREATE_CONTEXT_ARRAY_MALLOC(NULL, 132, info, 1, RedGpuInfoOptionalInfoDriverProperties, 18);
      info->optionalInfo = RED_GPU_INFO_OPTIONAL_INFO_DRIVER_PROPERTIES;
      info->next         = optionalInfoNext;
      info->id           = driverProperties.driverID;
      for (unsigned i = 0; i < 256; i += 1) {
        info->name[i]    = driverProperties.driverName[i];
      }
      for (unsigned i = 0; i < 256; i += 1) {
        info->info[i]    = driverProperties.driverInfo[i];
      }
      info->compliantWithConformanceTestSuiteVersionMajor    = driverProperties.conformanceVersion.versionMajor;
      info->compliantWithConformanceTestSuiteVersionMinor    = driverProperties.conformanceVersion.versionMinor;
      info->compliantWithConformanceTestSuiteVersionSubminor = driverProperties.conformanceVersion.versionSubminor;
      info->compliantWithConformanceTestSuiteVersionPatch    = driverProperties.conformanceVersion.versionPatch;
      optionalInfoNext = info;
    }

    if (requestedExtensions[RED_SDK_EXTENSION_RESOLVE_DEPTH_STENCIL] == 1) {
      if (wasRequestedAndWasFoundExtensionDepthStencilResolve == 1) {
        RedGpuInfoOptionalInfoResolveDepthStencil * info = NULL;
        REDGPU_CREATE_CONTEXT_ARRAY_MALLOC(NULL, 133, info, 1, RedGpuInfoOptionalInfoResolveDepthStencil, 19);
        info->optionalInfo                              = RED_GPU_INFO_OPTIONAL_INFO_RESOLVE_DEPTH_STENCIL;
        info->next                                      = optionalInfoNext;
        info->supportsResolveDepthStencil               = 1;
        info->supportsResolveModeDepthSampleIndexZero   = (depthStencilResolveProperties.supportedDepthResolveModes   & RED_RESOLVE_MODE_SAMPLE_INDEX_ZERO) == RED_RESOLVE_MODE_SAMPLE_INDEX_ZERO;
        info->supportsResolveModeDepthAverage           = (depthStencilResolveProperties.supportedDepthResolveModes   & RED_RESOLVE_MODE_AVERAGE)           == RED_RESOLVE_MODE_AVERAGE;
        info->supportsResolveModeDepthMin               = (depthStencilResolveProperties.supportedDepthResolveModes   & RED_RESOLVE_MODE_MIN)               == RED_RESOLVE_MODE_MIN;
        info->supportsResolveModeDepthMax               = (depthStencilResolveProperties.supportedDepthResolveModes   & RED_RESOLVE_MODE_MAX)               == RED_RESOLVE_MODE_MAX;
        info->supportsResolveModeStencilSampleIndexZero = (depthStencilResolveProperties.supportedStencilResolveModes & RED_RESOLVE_MODE_SAMPLE_INDEX_ZERO) == RED_RESOLVE_MODE_SAMPLE_INDEX_ZERO;
        info->supportsResolveModeStencilAverage         = (depthStencilResolveProperties.supportedStencilResolveModes & RED_RESOLVE_MODE_AVERAGE)           == RED_RESOLVE_MODE_AVERAGE;
        info->supportsResolveModeStencilMin             = (depthStencilResolveProperties.supportedStencilResolveModes & RED_RESOLVE_MODE_MIN)               == RED_RESOLVE_MODE_MIN;
        info->supportsResolveModeStencilMax             = (depthStencilResolveProperties.supportedStencilResolveModes & RED_RESOLVE_MODE_MAX)               == RED_RESOLVE_MODE_MAX;
        info->supportsResolveIndependentNone            = depthStencilResolveProperties.independentResolveNone == 1;
        info->supportsResolveIndependent                = depthStencilResolveProperties.independentResolve     == 1;
        optionalInfoNext = info;
      } else {
        RedGpuInfoOptionalInfoResolveDepthStencil * info = NULL;
        REDGPU_CREATE_CONTEXT_ARRAY_MALLOC(NULL, 134, info, 1, RedGpuInfoOptionalInfoResolveDepthStencil, 20);
        info->optionalInfo                              = RED_GPU_INFO_OPTIONAL_INFO_RESOLVE_DEPTH_STENCIL;
        info->next                                      = optionalInfoNext;
        info->supportsResolveDepthStencil               = 0;
        info->supportsResolveModeDepthSampleIndexZero   = 0;
        info->supportsResolveModeDepthAverage           = 0;
        info->supportsResolveModeDepthMin               = 0;
        info->supportsResolveModeDepthMax               = 0;
        info->supportsResolveModeStencilSampleIndexZero = 0;
        info->supportsResolveModeStencilAverage         = 0;
        info->supportsResolveModeStencilMin             = 0;
        info->supportsResolveModeStencilMax             = 0;
        info->supportsResolveIndependentNone            = 0;
        info->supportsResolveIndependent                = 0;
        optionalInfoNext = info;
      }
    }

    if (requestedExtensions[RED_SDK_EXTENSION_PROCEDURE_PARAMETERS_HANDLES] == 1) {
      if (wasRequestedAndWasFoundExtensionProcedureParametersHandles == 1) {
        RedGpuInfoOptionalInfoProcedureParametersHandles * info = NULL;
        REDGPU_CREATE_CONTEXT_ARRAY_MALLOC(NULL, 135, info, 1, RedGpuInfoOptionalInfoProcedureParametersHandles, 21);
        info->optionalInfo                       = RED_GPU_INFO_OPTIONAL_INFO_PROCEDURE_PARAMETERS_HANDLES;
        info->next                               = optionalInfoNext;
        info->supportsProcedureParametersHandles = 1;
        info->maxProcedureParametersHandlesCount = pushDescriptorProperties.maxPushDescriptors;
        optionalInfoNext = info;
      } else {
        RedGpuInfoOptionalInfoProcedureParametersHandles * info = NULL;
        REDGPU_CREATE_CONTEXT_ARRAY_MALLOC(NULL, 136, info, 1, RedGpuInfoOptionalInfoProcedureParametersHandles, 22);
        info->optionalInfo                       = RED_GPU_INFO_OPTIONAL_INFO_PROCEDURE_PARAMETERS_HANDLES;
        info->next                               = optionalInfoNext;
        info->supportsProcedureParametersHandles = 0;
        info->maxProcedureParametersHandlesCount = 8;
        optionalInfoNext = info;
      }
    }

    if (requestedExtensions[RED_SDK_EXTENSION_RASTERIZATION_MODE] == 1) {
      if (wasRequestedAndWasFoundExtensionRasterizationMode == 1) {
        RedGpuInfoOptionalInfoRasterizationMode * info = NULL;
        REDGPU_CREATE_CONTEXT_ARRAY_MALLOC(NULL, 137, info, 1, RedGpuInfoOptionalInfoRasterizationMode, 23);
        info->optionalInfo                                  = RED_GPU_INFO_OPTIONAL_INFO_RASTERIZATION_MODE;
        info->next                                          = optionalInfoNext;
        info->supportsRasterizationModeOverestimate         = 1;
        info->supportsRasterizationModeUnderestimate        = conservativeRasterizationProperties.primitiveUnderestimation                == 1;
        info->supportsGuaranteedZeroAreaBackFacingTriangles = conservativeRasterizationProperties.degenerateTrianglesRasterized           == 1;
        info->supportsGpuCodeFullyCoveredEXT                = conservativeRasterizationProperties.fullyCoveredFragmentShaderInputVariable == 1;
        optionalInfoNext = info;
      } else {
        RedGpuInfoOptionalInfoRasterizationMode * info = NULL;
        REDGPU_CREATE_CONTEXT_ARRAY_MALLOC(NULL, 138, info, 1, RedGpuInfoOptionalInfoRasterizationMode, 24);
        info->optionalInfo                                  = RED_GPU_INFO_OPTIONAL_INFO_RASTERIZATION_MODE;
        info->next                                          = optionalInfoNext;
        info->supportsRasterizationModeOverestimate         = 0;
        info->supportsRasterizationModeUnderestimate        = 0;
        info->supportsGuaranteedZeroAreaBackFacingTriangles = 0;
        info->supportsGpuCodeFullyCoveredEXT                = 0;
        optionalInfoNext = info;
      }
    }

    eGpuInfo->optionalInfo = optionalInfoNext;
  }

  for (unsigned i = 0; i < gpusCount; i += 1) {
    if (ctx->gpusStatuses[i].statusError != RED_STATUS_SUCCESS) {
      continue;
    }

    VkPhysicalDevice     device   =  cold->vkPhysicalDevices[i];
    RedInternalGpuInfo * iGpuInfo = &cold->gpus[i];

    vk->vkGetPhysicalDeviceQueueFamilyProperties(device, &iGpuInfo->vkQueueFamilyPropertiesCount, NULL);
    if (settings0GpusExposeOnlyOneQueue == 1) {
      iGpuInfo->vkQueueFamilyPropertiesCount = 1;
    }
    REDGPU_CREATE_CONTEXT_ARRAY_MALLOC(NULL, 139, iGpuInfo->vkQueueFamilyProperties, iGpuInfo->vkQueueFamilyPropertiesCount, VkQueueFamilyProperties, 25);
    vk->vkGetPhysicalDeviceQueueFamilyProperties(device, &iGpuInfo->vkQueueFamilyPropertiesCount, iGpuInfo->vkQueueFamilyProperties);
    if (settings0GpusExposeOnlyOneQueue == 1) {
      iGpuInfo->vkQueueFamilyProperties[0].queueCount = 1;
    }
  }

  for (unsigned i = 0; i < gpusCount; i += 1) {
    if (ctx->gpusStatuses[i].statusError != RED_STATUS_SUCCESS) {
      continue;
    }

    RedInternalGpuInfo * iGpuInfo = &cold->gpus[i];

    iGpuInfo->vkQueueFamilyIndexesCount = iGpuInfo->vkQueueFamilyPropertiesCount;
    REDGPU_CREATE_CONTEXT_ARRAY_MALLOC(NULL, 140, iGpuInfo->vkQueueFamilyIndexes, iGpuInfo->vkQueueFamilyIndexesCount, unsigned, 26);
    for (unsigned j = 0; j < iGpuInfo->vkQueueFamilyIndexesCount; j += 1) {
      iGpuInfo->vkQueueFamilyIndexes[j] = j;
    }
  }

  for (unsigned i = 0; i < gpusCount; i += 1) {
    if (ctx->gpusStatuses[i].statusError != RED_STATUS_SUCCESS) {
      continue;
    }

    RedInternalGpuInfo * iGpuInfo = &cold->gpus[i];

    iGpuInfo->queueFamilyDataCount = iGpuInfo->vkQueueFamilyPropertiesCount;
    REDGPU_CREATE_CONTEXT_ARRAY_MALLOC(NULL, 141, iGpuInfo->queueFamilyData, iGpuInfo->queueFamilyDataCount, RedInternalQueueFamilyData, 27);
    REDGPU_ARRAY_MEMSET(iGpuInfo->queueFamilyData, iGpuInfo->queueFamilyDataCount);

    for (unsigned j = 0; j < iGpuInfo->queueFamilyDataCount; j += 1) {
      VkQueueFamilyProperties    * queueFamilyProperties = &iGpuInfo->vkQueueFamilyProperties[j];
      RedInternalQueueFamilyData * queueFamilyData       = &iGpuInfo->queueFamilyData[j];

      queueFamilyData->queuesPrioritiesCount = queueFamilyProperties->queueCount;
      REDGPU_CREATE_CONTEXT_ARRAY_MALLOC(NULL, 142, queueFamilyData->queuesPriorities, queueFamilyData->queuesPrioritiesCount, float, 28);
      for (unsigned k = 0; k < queueFamilyData->queuesPrioritiesCount; k += 1) {
        queueFamilyData->queuesPriorities[k] = 1.f;
      }
    }
  }

  for (unsigned i = 0; i < gpusCount; i += 1) {
    if (ctx->gpusStatuses[i].statusError != RED_STATUS_SUCCESS) {
      continue;
    }

    RedInternalGpuInfo * iGpuInfo = &cold->gpus[i];

    iGpuInfo->vkDeviceQueueCreateInfoCount = iGpuInfo->vkQueueFamilyPropertiesCount;
    REDGPU_CREATE_CONTEXT_ARRAY_MALLOC(NULL, 143, iGpuInfo->vkDeviceQueueCreateInfo, iGpuInfo->vkDeviceQueueCreateInfoCount, VkDeviceQueueCreateInfo, 29);
    for (unsigned j = 0; j < iGpuInfo->vkDeviceQueueCreateInfoCount; j += 1) {
      RedInternalQueueFamilyData * queueFamilyData = &iGpuInfo->queueFamilyData[j];

      VkDeviceQueueCreateInfo info;
      info.sType            = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
      info.pNext            = NULL;
      info.flags            = 0;
      info.queueFamilyIndex = j;
      info.queueCount       = queueFamilyData->queuesPrioritiesCount;
      info.pQueuePriorities = queueFamilyData->queuesPriorities;
      iGpuInfo->vkDeviceQueueCreateInfo[j] = info;
    }
  }

  for (unsigned i = 0; i < gpusCount; i += 1) {
    if (ctx->gpusStatuses[i].statusError != RED_STATUS_SUCCESS) {
      continue;
    }

    RedGpuInfo         * eGpuInfo = &ctx->gpus[i];
    RedInternalGpuInfo * iGpuInfo = &cold->gpus[i];

    VkPhysicalDevice device = cold->vkPhysicalDevices[i];

#if defined(REDGPU_OS_ANDROID)
    RedBool32 wasRequestedAndWasFoundExtensionWsi = eGpuInfo->supportsWsi == 1;
#else
    RedBool32 wasRequestedAndWasFoundExtensionWsi = (requestedExtensions[RED_SDK_EXTENSION_WSI_WIN32] == 1 ||
                                                     requestedExtensions[RED_SDK_EXTENSION_WSI_XLIB]  == 1 ||
                                                     requestedExtensions[RED_SDK_EXTENSION_WSI_XCB]   == 1)
                                                     && eGpuInfo->supportsWsi == 1;
#endif
    RedBool32 wasRequestedAndWasFoundExtensionMemoryBudget               = redInternalFindDeviceExtension(iGpuInfo, "VK_EXT_memory_budget", procedureId, 0, optionalFile, optionalLine) == 1;
    RedBool32 wasRequestedAndWasFoundExtensionDriverProperties           = (requestedExtensions[RED_SDK_EXTENSION_DRIVER_PROPERTIES]            == 1) && (redInternalFindDeviceExtension(iGpuInfo, "VK_KHR_driver_properties",          procedureId, 0, optionalFile, optionalLine) == 1);
    RedBool32 wasRequestedAndWasFoundExtensionDepthStencilResolve        = (requestedExtensions[RED_SDK_EXTENSION_RESOLVE_DEPTH_STENCIL]        == 1) && (redInternalFindDeviceExtension(iGpuInfo, "VK_KHR_depth_stencil_resolve",      procedureId, 0, optionalFile, optionalLine) == 1);
    RedBool32 wasRequestedAndWasFoundExtensionMultiview                  = (requestedExtensions[RED_SDK_EXTENSION_RESOLVE_DEPTH_STENCIL]        == 1) && (redInternalFindDeviceExtension(iGpuInfo, "VK_KHR_multiview",                  procedureId, 0, optionalFile, optionalLine) == 1);
    RedBool32 wasRequestedAndWasFoundExtensionDedicateMemory             = (requestedExtensions[RED_SDK_EXTENSION_DEDICATE_MEMORY]              == 1) && (redInternalFindDeviceExtension(iGpuInfo, "VK_NV_dedicated_allocation",        procedureId, 0, optionalFile, optionalLine) == 1);
    RedBool32 wasRequestedAndWasFoundExtensionProcedureParametersHandles = (requestedExtensions[RED_SDK_EXTENSION_PROCEDURE_PARAMETERS_HANDLES] == 1) && (redInternalFindDeviceExtension(iGpuInfo, "VK_KHR_push_descriptor",            procedureId, 0, optionalFile, optionalLine) == 1);
    RedBool32 wasRequestedButNotFoundExtensionProcedureParametersHandles = (requestedExtensions[RED_SDK_EXTENSION_PROCEDURE_PARAMETERS_HANDLES] == 1) && (redInternalFindDeviceExtension(iGpuInfo, "VK_KHR_push_descriptor",            procedureId, 0, optionalFile, optionalLine) == 0);
    RedBool32 wasRequestedAndWasFoundExtensionBatchMemorySet             = (requestedExtensions[RED_SDK_EXTENSION_BATCH_MEMORY_SET]             == 1) && (redInternalFindDeviceExtension(iGpuInfo, "VK_KHR_bind_memory2",               procedureId, 0, optionalFile, optionalLine) == 1);
    RedBool32 wasRequestedAndWasFoundExtensionRasterizationMode          = (requestedExtensions[RED_SDK_EXTENSION_RASTERIZATION_MODE]           == 1) && (redInternalFindDeviceExtension(iGpuInfo, "VK_EXT_conservative_rasterization", procedureId, 0, optionalFile, optionalLine) == 1);
    RedBool32 wasRequestedAndWasFoundExtensionFormalMemoryModel          = (requestedExtensions[RED_SDK_EXTENSION_FORMAL_MEMORY_MODEL]          == 1) && (redInternalFindDeviceExtension(iGpuInfo, "VK_KHR_vulkan_memory_model",        procedureId, 0, optionalFile, optionalLine) == 1);
    RedBool32 wasRequestedAndWasFoundExtensionComputingLanguage          = requestedExtensions[RED_SDK_EXTENSION_COMPUTING_LANGUAGE_FEATURE_LEVEL_1];
    {
      if (redInternalFindDeviceExtension(iGpuInfo, "VK_KHR_storage_buffer_storage_class", procedureId, 0, optionalFile, optionalLine) == 0) {
        wasRequestedAndWasFoundExtensionComputingLanguage = 0;
      }
      if (redInternalFindDeviceExtension(iGpuInfo, "VK_KHR_variable_pointers", procedureId, 0, optionalFile, optionalLine) == 0) {
        wasRequestedAndWasFoundExtensionComputingLanguage = 0;
      }
    }
    RedBool32 wasRequestedAndWasFoundExtensionRayTracing                 = requestedExtensions[RED_SDK_EXTENSION_RAY_TRACING];
    {
      const unsigned vendorIDAmd    = 0x1002;
      const unsigned vendorIDNvidia = 0x10de;
      const unsigned vendorIDIntel  = 0x8086;
      if (iGpuInfo->vkPhysicalDeviceProperties.vendorID == vendorIDAmd) {
        if (redInternalFindDeviceExtension(iGpuInfo, "VK_AMD_shader_ballot", procedureId, 0, optionalFile, optionalLine) == 0) {
          wasRequestedAndWasFoundExtensionRayTracing = 0;
        }
      } else if (iGpuInfo->vkPhysicalDeviceProperties.vendorID == vendorIDNvidia) {
        if (redInternalFindDeviceExtension(iGpuInfo, "VK_NV_shader_subgroup_partitioned", procedureId, 0, optionalFile, optionalLine) == 0) {
          wasRequestedAndWasFoundExtensionRayTracing = 0;
        }
      } else {
        // NOTE(Constantine): Ray tracing on Intel GPUs is disabled for now.
        wasRequestedAndWasFoundExtensionRayTracing = 0;
      }
      if (redInternalFindDeviceExtension(iGpuInfo, "VK_EXT_shader_subgroup_ballot", procedureId, 0, optionalFile, optionalLine) == 0) {
        wasRequestedAndWasFoundExtensionRayTracing = 0;
      }
      if (redInternalFindDeviceExtension(iGpuInfo, "VK_EXT_descriptor_indexing", procedureId, 0, optionalFile, optionalLine) == 0) {
        wasRequestedAndWasFoundExtensionRayTracing = 0;
      }
    }

    unsigned deviceExtensionsCount = 0;
    const char * deviceExtensions[19];
    deviceExtensions[deviceExtensionsCount] = "VK_KHR_maintenance1";
    deviceExtensionsCount += 1;
#if defined(REDGPU_OS_ANDROID)
#else
    deviceExtensions[deviceExtensionsCount] = "VK_KHR_maintenance2";
    deviceExtensionsCount += 1;
#endif
#if defined(_WIN32) && !defined(_WIN64)
    // NOTE(Constantine): Nothing, intentionally.
#elif defined(REDGPU_OS_ANDROID)
    // NOTE(Constantine): Nothing, intentionally.
#else
    deviceExtensions[deviceExtensionsCount] = "VK_KHR_maintenance3";
    deviceExtensionsCount += 1;
#endif
    if (wasRequestedAndWasFoundExtensionWsi == 1) {
      deviceExtensions[deviceExtensionsCount] = "VK_KHR_swapchain";
      deviceExtensionsCount += 1;
    }
    if (wasRequestedAndWasFoundExtensionMemoryBudget == 1) {
      ctx->internal.gpuExtensionBitflags[i] |= RED_INTERNAL_GPU_EXTENSION_BITFLAG_MEMORY_GET_BUDGET;
      eGpuInfo->supportsMemoryGetBudget = 1;
      deviceExtensions[deviceExtensionsCount] = "VK_EXT_memory_budget";
      deviceExtensionsCount += 1;
    } else {
      eGpuInfo->supportsMemoryGetBudget = 0;
    }
    if (wasRequestedAndWasFoundExtensionDriverProperties == 1) {
      deviceExtensions[deviceExtensionsCount] = "VK_KHR_driver_properties";
      deviceExtensionsCount += 1;
    }
    if (wasRequestedAndWasFoundExtensionDepthStencilResolve == 1 && wasRequestedAndWasFoundExtensionMultiview == 1) {
      ctx->internal.gpuExtensionBitflags[i] |= RED_INTERNAL_GPU_EXTENSION_BITFLAG_DEPTH_STENCIL_RESOLVE;
      deviceExtensions[deviceExtensionsCount] = "VK_KHR_multiview";
      deviceExtensionsCount += 1;
      deviceExtensions[deviceExtensionsCount] = "VK_KHR_create_renderpass2";
      deviceExtensionsCount += 1;
      deviceExtensions[deviceExtensionsCount] = "VK_KHR_depth_stencil_resolve";
      deviceExtensionsCount += 1;
    } else if (wasRequestedAndWasFoundExtensionDepthStencilResolve == 1 && wasRequestedAndWasFoundExtensionMultiview == 0) {
      ctx->internal.gpuExtensionBitflags[i] |= RED_INTERNAL_GPU_EXTENSION_BITFLAG_DEPTH_STENCIL_RESOLVE;
      deviceExtensions[deviceExtensionsCount] = "VK_KHR_create_renderpass2";
      deviceExtensionsCount += 1;
      deviceExtensions[deviceExtensionsCount] = "VK_KHR_depth_stencil_resolve";
      deviceExtensionsCount += 1;
    }
    if (wasRequestedAndWasFoundExtensionDedicateMemory == 1) {
      ctx->internal.gpuExtensionBitflags[i] |= RED_INTERNAL_GPU_EXTENSION_BITFLAG_DEDICATE_MEMORY_NV;
      deviceExtensions[deviceExtensionsCount] = "VK_NV_dedicated_allocation";
      deviceExtensionsCount += 1;
    }
    if (wasRequestedAndWasFoundExtensionProcedureParametersHandles == 1) {
      ctx->internal.gpuExtensionBitflags[i] |= RED_INTERNAL_GPU_EXTENSION_BITFLAG_PROCEDURE_PARAMETERS_HANDLES;
      deviceExtensions[deviceExtensionsCount] = "VK_KHR_push_descriptor";
      deviceExtensionsCount += 1;
    }
    if (wasRequestedButNotFoundExtensionProcedureParametersHandles == 1) {
      // TODO(Constantine)
    }
    if (wasRequestedAndWasFoundExtensionBatchMemorySet == 1) {
      ctx->internal.gpuExtensionBitflags[i] |= RED_INTERNAL_GPU_EXTENSION_BITFLAG_BATCH_MEMORY_SET;
      deviceExtensions[deviceExtensionsCount] = "VK_KHR_bind_memory2";
      deviceExtensionsCount += 1;
    }
    if (wasRequestedAndWasFoundExtensionRasterizationMode == 1) {
      ctx->internal.gpuExtensionBitflags[i] |= RED_INTERNAL_GPU_EXTENSION_BITFLAG_RASTERIZATION_MODE;
      deviceExtensions[deviceExtensionsCount] = "VK_EXT_conservative_rasterization";
      deviceExtensionsCount += 1;
    }
    if (wasRequestedAndWasFoundExtensionFormalMemoryModel == 1) {
      ctx->internal.gpuExtensionBitflags[i] |= RED_INTERNAL_GPU_EXTENSION_BITFLAG_FORMAL_MEMORY_MODEL;
      deviceExtensions[deviceExtensionsCount] = "VK_KHR_vulkan_memory_model";
      deviceExtensionsCount += 1;
    }
    if (wasRequestedAndWasFoundExtensionRayTracing == 1) {
      const unsigned vendorIDAmd    = 0x1002;
      const unsigned vendorIDNvidia = 0x10de;
      const unsigned vendorIDIntel  = 0x8086;
      ctx->internal.gpuExtensionBitflags[i] |= RED_INTERNAL_GPU_EXTENSION_BITFLAG_RAY_TRACING;
      if (iGpuInfo->vkPhysicalDeviceProperties.vendorID == vendorIDAmd) {
        deviceExtensions[deviceExtensionsCount] = "VK_AMD_shader_ballot";
        deviceExtensionsCount += 1;
      } else if (iGpuInfo->vkPhysicalDeviceProperties.vendorID == vendorIDNvidia) {
        deviceExtensions[deviceExtensionsCount] = "VK_NV_shader_subgroup_partitioned";
        deviceExtensionsCount += 1;
      }
      deviceExtensions[deviceExtensionsCount] = "VK_EXT_shader_subgroup_ballot";
      deviceExtensionsCount += 1;
      deviceExtensions[deviceExtensionsCount] = "VK_EXT_descriptor_indexing";
      deviceExtensionsCount += 1;
    }
    if (wasRequestedAndWasFoundExtensionComputingLanguage == 1) {
      deviceExtensions[deviceExtensionsCount] = "VK_KHR_storage_buffer_storage_class";
      deviceExtensionsCount += 1;
      deviceExtensions[deviceExtensionsCount] = "VK_KHR_variable_pointers";
      deviceExtensionsCount += 1;
    }

    VkPhysicalDeviceFeatures deviceFeaturesToEnable = iGpuInfo->vkPhysicalDeviceFeatures;
    deviceFeaturesToEnable.robustBufferAccess = 0;
    //                     fullDrawIndexUint32
    //                     imageCubeArray
    //                     independentBlend
    deviceFeaturesToEnable.geometryShader = 0;
    deviceFeaturesToEnable.tessellationShader = 0;
    //                     sampleRateShading
    //                     dualSrcBlend
    //                     logicOp
    deviceFeaturesToEnable.multiDrawIndirect = 0;
    deviceFeaturesToEnable.drawIndirectFirstInstance = 0;
    //                     depthClamp
    //                     depthBiasClamp
    deviceFeaturesToEnable.fillModeNonSolid = 0;
    //                     depthBounds
    deviceFeaturesToEnable.wideLines = 0;
    deviceFeaturesToEnable.largePoints = 0;
    //                     alphaToOne
    deviceFeaturesToEnable.multiViewport = 0;
    //                     samplerAnisotropy
    deviceFeaturesToEnable.textureCompressionETC2 = 0;
    deviceFeaturesToEnable.textureCompressionASTC_LDR = 0;
    deviceFeaturesToEnable.textureCompressionBC = 0;
    deviceFeaturesToEnable.occlusionQueryPrecise = 0;
    deviceFeaturesToEnable.pipelineStatisticsQuery = 0;
    //                     vertexPipelineStoresAndAtomics
    //                     fragmentStoresAndAtomics
    deviceFeaturesToEnable.shaderTessellationAndGeometryPointSize = 0;
    //                     shaderImageGatherExtended
    //                     shaderStorageImageExtendedFormats
    //                     shaderStorageImageMultisample
    //                     shaderStorageImageReadWithoutFormat
    //                     shaderStorageImageWriteWithoutFormat
    //                     shaderUniformBufferArrayDynamicIndexing
    //                     shaderSampledImageArrayDynamicIndexing
    //                     shaderStorageBufferArrayDynamicIndexing
    //                     shaderStorageImageArrayDynamicIndexing
    //                     shaderClipDistance
    //                     shaderCullDistance
    //                     shaderFloat64
    //                     shaderInt64
    deviceFeaturesToEnable.shaderInt16 = 0;
    deviceFeaturesToEnable.shaderResourceResidency = 0;
    //                     shaderResourceMinLod
    deviceFeaturesToEnable.sparseBinding = 0;
    deviceFeaturesToEnable.sparseResidencyBuffer = 0;
    deviceFeaturesToEnable.sparseResidencyImage2D = 0;
    deviceFeaturesToEnable.sparseResidencyImage3D = 0;
    deviceFeaturesToEnable.sparseResidency2Samples = 0;
    deviceFeaturesToEnable.sparseResidency4Samples = 0;
    deviceFeaturesToEnable.sparseResidency8Samples = 0;
    deviceFeaturesToEnable.sparseResidency16Samples = 0;
    deviceFeaturesToEnable.sparseResidencyAliased = 0;
    //                     variableMultisampleRate
    deviceFeaturesToEnable.inheritedQueries = 0;

    void * featuresToEnableNext = NULL;

    typedef struct VkPhysicalDeviceDescriptorIndexingFeaturesEXT {
      unsigned  sType;
      void *    pNext;
      RedBool32 shaderInputAttachmentArrayDynamicIndexing;
      RedBool32 shaderUniformTexelBufferArrayDynamicIndexing;
      RedBool32 shaderStorageTexelBufferArrayDynamicIndexing;
      RedBool32 shaderUniformBufferArrayNonUniformIndexing;
      RedBool32 shaderSampledImageArrayNonUniformIndexing;
      RedBool32 shaderStorageBufferArrayNonUniformIndexing;
      RedBool32 shaderStorageImageArrayNonUniformIndexing;
      RedBool32 shaderInputAttachmentArrayNonUniformIndexing;
      RedBool32 shaderUniformTexelBufferArrayNonUniformIndexing;
      RedBool32 shaderStorageTexelBufferArrayNonUniformIndexing;
      RedBool32 descriptorBindingUniformBufferUpdateAfterBind;
      RedBool32 descriptorBindingSampledImageUpdateAfterBind;
      RedBool32 descriptorBindingStorageImageUpdateAfterBind;
      RedBool32 descriptorBindingStorageBufferUpdateAfterBind;
      RedBool32 descriptorBindingUniformTexelBufferUpdateAfterBind;
      RedBool32 descriptorBindingStorageTexelBufferUpdateAfterBind;
      RedBool32 descriptorBindingUpdateUnusedWhilePending;
      RedBool32 descriptorBindingPartiallyBound;
      RedBool32 descriptorBindingVariableDescriptorCount;
      RedBool32 runtimeDescriptorArray;
    } VkPhysicalDeviceDescriptorIndexingFeaturesEXT;
    typedef struct VkPhysicalDeviceVariablePointersFeaturesKHR {
      unsigned  sType;
      void *    pNext;
      RedBool32 variablePointersStorageBuffer;
      RedBool32 variablePointers;
    } VkPhysicalDeviceVariablePointersFeaturesKHR;
    VkPhysicalDeviceDescriptorIndexingFeaturesEXT descriptorIndexingFeatures = {0};
    VkPhysicalDeviceVariablePointersFeaturesKHR   variablePointersFeatures   = {0};
    VkPhysicalDeviceVulkanMemoryModelFeaturesKHR  vulkanMemoryModelFeatures  = iGpuInfo->vkPhysicalDeviceVulkanMemoryModelFeatures;

    if (wasRequestedAndWasFoundExtensionRayTracing == 1) {
      descriptorIndexingFeatures.sType                                      = 1000161001;           // VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DESCRIPTOR_INDEXING_FEATURES
      descriptorIndexingFeatures.pNext                                      = featuresToEnableNext;
      descriptorIndexingFeatures.shaderStorageBufferArrayNonUniformIndexing = 1;
      descriptorIndexingFeatures.runtimeDescriptorArray                     = 1;
      descriptorIndexingFeatures.descriptorBindingVariableDescriptorCount   = 1;
      featuresToEnableNext = &descriptorIndexingFeatures;
    }

    if (wasRequestedAndWasFoundExtensionComputingLanguage == 1) {
      variablePointersFeatures.sType                         = 1000120000;           // VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VARIABLE_POINTERS_FEATURES_KHR
      variablePointersFeatures.pNext                         = featuresToEnableNext;
      variablePointersFeatures.variablePointersStorageBuffer = 1;
      variablePointersFeatures.variablePointers              = 1;
      featuresToEnableNext = &variablePointersFeatures;
    }

    if (wasRequestedAndWasFoundExtensionFormalMemoryModel == 1) {
      vulkanMemoryModelFeatures.pNext = featuresToEnableNext;
      featuresToEnableNext = &vulkanMemoryModelFeatures;
    }

    VkDeviceCreateInfo info;
    info.sType                   = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    info.pNext                   = featuresToEnableNext;
    info.flags                   = 0;
    info.queueCreateInfoCount    = iGpuInfo->vkDeviceQueueCreateInfoCount;
    info.pQueueCreateInfos       = iGpuInfo->vkDeviceQueueCreateInfo;
    info.enabledLayerCount       = 0;    // NOTE(Constantine): Device layers are deprecated.
    info.ppEnabledLayerNames     = NULL; // NOTE(Constantine): Device layers are deprecated.
    info.enabledExtensionCount   = deviceExtensionsCount;
    info.ppEnabledExtensionNames = deviceExtensions;
    info.pEnabledFeatures        = &deviceFeaturesToEnable;

#if REDGPU_COMPILE_SWITCH == REDGPU_COMPILE_SWITCH_NVTX || REDGPU_COMPILE_SWITCH == REDGPU_COMPILE_SWITCH_GPUVIS
    {
    redInternalMarkSet("vkCreateDevice");
#endif

    if (settingsContextFromVkPassed == 1) {
      eGpuInfo->gpu = (RedHandleGpu)settingsContextFromVkDevices[i];
    } else {
#if REDGPU_COMPILE_SWITCH == REDGPU_COMPILE_SWITCH_NVTX
      const unsigned vendorIDAmd    = 0x1002;
      const unsigned vendorIDNvidia = 0x10de;
      const unsigned vendorIDIntel  = 0x8086;
      if (iGpuInfo->vkPhysicalDeviceProperties.vendorID == vendorIDNvidia) {
#endif
      redInternalSetStatus(ctx, NULL, &ctx->gpusStatuses[i], 254, vk->vkCreateDevice(device, &info, NULL, (VkDevice *)&eGpuInfo->gpu), procedureId, optionalFile, optionalLine, optionalUserData);
#if REDGPU_COMPILE_SWITCH == REDGPU_COMPILE_SWITCH_NVTX
      } else {
        eGpuInfo->gpu = NULL;
        ctx->gpusStatuses[i].statusError = RED_STATUS_ERROR_INCOMPATIBLE_DRIVER;
      }
#endif
    }

#if REDGPU_COMPILE_SWITCH == REDGPU_COMPILE_SWITCH_NVTX || REDGPU_COMPILE_SWITCH == REDGPU_COMPILE_SWITCH_GPUVIS
    redInternalMarkEnd();
    }
#endif
  }

  for (unsigned i = 0; i < gpusCount; i += 1) {
    if (ctx->gpusStatuses[i].statusError != RED_STATUS_SUCCESS) {
      continue;
    }

    RedGpuInfo * eGpuInfo = &ctx->gpus[i];

    ctx->internal.hotGpuHandles[i] = eGpuInfo->gpu;
  }

  for (unsigned i = 0; i < gpusCount; i += 1) {
    if (ctx->gpusStatuses[i].statusError != RED_STATUS_SUCCESS) {
      continue;
    }

    VkDevice gpu = (VkDevice)ctx->internal.hotGpuHandles[i];

    ctx->internal.gpuProcedureAddresses[i].redCallGpuToCpuSignalSignal                                = (void *)vk->vkGetDeviceProcAddr(gpu, "vkCmdSetEvent");
    ctx->internal.gpuProcedureAddresses[i].redCallCopyArrayToArray                                    = (void *)vk->vkGetDeviceProcAddr(gpu, "vkCmdCopyBuffer");
    ctx->internal.gpuProcedureAddresses[i].redCallCopyImageToImage                                    = (void *)vk->vkGetDeviceProcAddr(gpu, "vkCmdCopyImage");
    ctx->internal.gpuProcedureAddresses[i].redCallCopyArrayToImage                                    = (void *)vk->vkGetDeviceProcAddr(gpu, "vkCmdCopyBufferToImage");
    ctx->internal.gpuProcedureAddresses[i].redCallCopyImageToArray                                    = (void *)vk->vkGetDeviceProcAddr(gpu, "vkCmdCopyImageToBuffer");
    ctx->internal.gpuProcedureAddresses[i].redCallProcedure                                           = (void *)vk->vkGetDeviceProcAddr(gpu, "vkCmdDraw");
    ctx->internal.gpuProcedureAddresses[i].redCallProcedureIndexed                                    = (void *)vk->vkGetDeviceProcAddr(gpu, "vkCmdDrawIndexed");
    ctx->internal.gpuProcedureAddresses[i].redCallProcedureCompute                                    = (void *)vk->vkGetDeviceProcAddr(gpu, "vkCmdDispatch");
    ctx->internal.gpuProcedureAddresses[i].redCallSetProcedure                                        = (void *)vk->vkGetDeviceProcAddr(gpu, "vkCmdBindPipeline");
    ctx->internal.gpuProcedureAddresses[i].redCallSetProcedureIndices                                 = (void *)vk->vkGetDeviceProcAddr(gpu, "vkCmdBindIndexBuffer");
    ctx->internal.gpuProcedureAddresses[i].redCallSetProcedureParametersStructs                       = (void *)vk->vkGetDeviceProcAddr(gpu, "vkCmdBindDescriptorSets");
    ctx->internal.gpuProcedureAddresses[i].redCallSetProcedureParametersHandles                       = (void *)vk->vkGetDeviceProcAddr(gpu, "vkCmdPushDescriptorSetKHR");
    ctx->internal.gpuProcedureAddresses[i].redCallSetProcedureParametersVariables                     = (void *)vk->vkGetDeviceProcAddr(gpu, "vkCmdPushConstants");
    ctx->internal.gpuProcedureAddresses[i]._0                                                         = NULL;
    ctx->internal.gpuProcedureAddresses[i]._1                                                         = NULL;
    ctx->internal.gpuProcedureAddresses[i].redCallSetDynamicDepthBias                                 = (void *)vk->vkGetDeviceProcAddr(gpu, "vkCmdSetDepthBias");
    ctx->internal.gpuProcedureAddresses[i].redCallSetDynamicDepthBounds                               = (void *)vk->vkGetDeviceProcAddr(gpu, "vkCmdSetDepthBounds");
    ctx->internal.gpuProcedureAddresses[i].redCallSetDynamicStencilCompareMask                        = (void *)vk->vkGetDeviceProcAddr(gpu, "vkCmdSetStencilCompareMask");
    ctx->internal.gpuProcedureAddresses[i].redCallSetDynamicStencilWriteMask                          = (void *)vk->vkGetDeviceProcAddr(gpu, "vkCmdSetStencilWriteMask");
    ctx->internal.gpuProcedureAddresses[i].redCallSetDynamicStencilReference                          = (void *)vk->vkGetDeviceProcAddr(gpu, "vkCmdSetStencilReference");
    ctx->internal.gpuProcedureAddresses[i].redCallSetDynamicBlendConstants                            = (void *)vk->vkGetDeviceProcAddr(gpu, "vkCmdSetBlendConstants");
    ctx->internal.gpuProcedureAddresses[i].redCallSetDynamicViewport                                  = (void *)vk->vkGetDeviceProcAddr(gpu, "vkCmdSetViewport");
    ctx->internal.gpuProcedureAddresses[i].redCallSetDynamicScissor                                   = (void *)vk->vkGetDeviceProcAddr(gpu, "vkCmdSetScissor");
    ctx->internal.gpuProcedureAddresses[i].redCallSetProcedureOutputVirtual.redCallSetProcedureOutput = (void *)vk->vkGetDeviceProcAddr(gpu, "vkCmdBeginRenderPass");
    ctx->internal.gpuProcedureAddresses[i].redCallSetProcedureOutputVirtual.context                   = ctx;
    ctx->internal.gpuProcedureAddresses[i].redCallSetProcedureOutputVirtual.gpuIndex                  = i;
    ctx->internal.gpuProcedureAddresses[i].redCallEndProcedureOutput                                  = (void *)vk->vkGetDeviceProcAddr(gpu, "vkCmdEndRenderPass");
    ctx->internal.gpuProcedureAddresses[i].redCallUsageOrderBarrier                                   = (void *)vk->vkGetDeviceProcAddr(gpu, "vkCmdPipelineBarrier");

#if REDGPU_COMPILE_SWITCH == REDGPU_COMPILE_SWITCH_DEBUG
    if (ctx->internal.gpuProcedureAddresses[i].redCallGpuToCpuSignalSignal                                == NULL) { REDGPU_INTERNAL_DEBUG_SEND_ERROR_MESSAGE(ctx, gpu, REDGPU_PRINT_PROCEDURE_ID(procedureId), 145, "RedCallProceduresAndAddresses::redCallGpuToCpuSignalSignal == NULL", optionalFile, optionalLine) }
    if (ctx->internal.gpuProcedureAddresses[i].redCallCopyArrayToArray                                    == NULL) { REDGPU_INTERNAL_DEBUG_SEND_ERROR_MESSAGE(ctx, gpu, REDGPU_PRINT_PROCEDURE_ID(procedureId), 146, "RedCallProceduresAndAddresses::redCallCopyArrayToArray == NULL", optionalFile, optionalLine) }
    if (ctx->internal.gpuProcedureAddresses[i].redCallCopyImageToImage                                    == NULL) { REDGPU_INTERNAL_DEBUG_SEND_ERROR_MESSAGE(ctx, gpu, REDGPU_PRINT_PROCEDURE_ID(procedureId), 147, "RedCallProceduresAndAddresses::redCallCopyImageToImage == NULL", optionalFile, optionalLine) }
    if (ctx->internal.gpuProcedureAddresses[i].redCallCopyArrayToImage                                    == NULL) { REDGPU_INTERNAL_DEBUG_SEND_ERROR_MESSAGE(ctx, gpu, REDGPU_PRINT_PROCEDURE_ID(procedureId), 148, "RedCallProceduresAndAddresses::redCallCopyArrayToImage == NULL", optionalFile, optionalLine) }
    if (ctx->internal.gpuProcedureAddresses[i].redCallCopyImageToArray                                    == NULL) { REDGPU_INTERNAL_DEBUG_SEND_ERROR_MESSAGE(ctx, gpu, REDGPU_PRINT_PROCEDURE_ID(procedureId), 149, "RedCallProceduresAndAddresses::redCallCopyImageToArray == NULL", optionalFile, optionalLine) }
    if (ctx->internal.gpuProcedureAddresses[i].redCallProcedure                                           == NULL) { REDGPU_INTERNAL_DEBUG_SEND_ERROR_MESSAGE(ctx, gpu, REDGPU_PRINT_PROCEDURE_ID(procedureId), 150, "RedCallProceduresAndAddresses::redCallProcedure == NULL", optionalFile, optionalLine) }
    if (ctx->internal.gpuProcedureAddresses[i].redCallProcedureIndexed                                    == NULL) { REDGPU_INTERNAL_DEBUG_SEND_ERROR_MESSAGE(ctx, gpu, REDGPU_PRINT_PROCEDURE_ID(procedureId), 151, "RedCallProceduresAndAddresses::redCallProcedureIndexed == NULL", optionalFile, optionalLine) }
    if (ctx->internal.gpuProcedureAddresses[i].redCallProcedureCompute                                    == NULL) { REDGPU_INTERNAL_DEBUG_SEND_ERROR_MESSAGE(ctx, gpu, REDGPU_PRINT_PROCEDURE_ID(procedureId), 152, "RedCallProceduresAndAddresses::redCallProcedureCompute == NULL", optionalFile, optionalLine) }
    if (ctx->internal.gpuProcedureAddresses[i].redCallSetProcedure                                        == NULL) { REDGPU_INTERNAL_DEBUG_SEND_ERROR_MESSAGE(ctx, gpu, REDGPU_PRINT_PROCEDURE_ID(procedureId), 153, "RedCallProceduresAndAddresses::redCallSetProcedure == NULL", optionalFile, optionalLine) }
    if (ctx->internal.gpuProcedureAddresses[i].redCallSetProcedureIndices                                 == NULL) { REDGPU_INTERNAL_DEBUG_SEND_ERROR_MESSAGE(ctx, gpu, REDGPU_PRINT_PROCEDURE_ID(procedureId), 154, "RedCallProceduresAndAddresses::redCallSetProcedureIndices == NULL", optionalFile, optionalLine) }
    if (ctx->internal.gpuProcedureAddresses[i].redCallSetProcedureParametersStructs                       == NULL) { REDGPU_INTERNAL_DEBUG_SEND_ERROR_MESSAGE(ctx, gpu, REDGPU_PRINT_PROCEDURE_ID(procedureId), 157, "RedCallProceduresAndAddresses::redCallSetProcedureParametersStructs == NULL", optionalFile, optionalLine) }
    if (redInternalGpuExtensionIsEnabled(ctx, i, RED_INTERNAL_GPU_EXTENSION_BITFLAG_PROCEDURE_PARAMETERS_HANDLES)) {
      if (ctx->internal.gpuProcedureAddresses[i].redCallSetProcedureParametersHandles                     == NULL) { REDGPU_INTERNAL_DEBUG_SEND_ERROR_MESSAGE(ctx, gpu, REDGPU_PRINT_PROCEDURE_ID(procedureId), 158, "RedCallProceduresAndAddresses::redCallSetProcedureParametersHandles == NULL", optionalFile, optionalLine) }
    }
    if (ctx->internal.gpuProcedureAddresses[i].redCallSetProcedureParametersVariables                     == NULL) { REDGPU_INTERNAL_DEBUG_SEND_ERROR_MESSAGE(ctx, gpu, REDGPU_PRINT_PROCEDURE_ID(procedureId), 159, "RedCallProceduresAndAddresses::redCallSetProcedureParametersVariables == NULL", optionalFile, optionalLine) }
    if (ctx->internal.gpuProcedureAddresses[i].redCallSetDynamicDepthBias                                 == NULL) { REDGPU_INTERNAL_DEBUG_SEND_ERROR_MESSAGE(ctx, gpu, REDGPU_PRINT_PROCEDURE_ID(procedureId), 160, "RedCallProceduresAndAddresses::redCallSetDynamicDepthBias == NULL", optionalFile, optionalLine) }
    if (ctx->internal.gpuProcedureAddresses[i].redCallSetDynamicDepthBounds                               == NULL) { REDGPU_INTERNAL_DEBUG_SEND_ERROR_MESSAGE(ctx, gpu, REDGPU_PRINT_PROCEDURE_ID(procedureId), 161, "RedCallProceduresAndAddresses::redCallSetDynamicDepthBounds == NULL", optionalFile, optionalLine) }
    if (ctx->internal.gpuProcedureAddresses[i].redCallSetDynamicStencilCompareMask                        == NULL) { REDGPU_INTERNAL_DEBUG_SEND_ERROR_MESSAGE(ctx, gpu, REDGPU_PRINT_PROCEDURE_ID(procedureId), 162, "RedCallProceduresAndAddresses::redCallSetDynamicStencilCompareMask == NULL", optionalFile, optionalLine) }
    if (ctx->internal.gpuProcedureAddresses[i].redCallSetDynamicStencilWriteMask                          == NULL) { REDGPU_INTERNAL_DEBUG_SEND_ERROR_MESSAGE(ctx, gpu, REDGPU_PRINT_PROCEDURE_ID(procedureId), 163, "RedCallProceduresAndAddresses::redCallSetDynamicStencilWriteMask == NULL", optionalFile, optionalLine) }
    if (ctx->internal.gpuProcedureAddresses[i].redCallSetDynamicStencilReference                          == NULL) { REDGPU_INTERNAL_DEBUG_SEND_ERROR_MESSAGE(ctx, gpu, REDGPU_PRINT_PROCEDURE_ID(procedureId), 164, "RedCallProceduresAndAddresses::redCallSetDynamicStencilReference == NULL", optionalFile, optionalLine) }
    if (ctx->internal.gpuProcedureAddresses[i].redCallSetDynamicBlendConstants                            == NULL) { REDGPU_INTERNAL_DEBUG_SEND_ERROR_MESSAGE(ctx, gpu, REDGPU_PRINT_PROCEDURE_ID(procedureId), 165, "RedCallProceduresAndAddresses::redCallSetDynamicBlendConstants == NULL", optionalFile, optionalLine) }
    if (ctx->internal.gpuProcedureAddresses[i].redCallSetDynamicViewport                                  == NULL) { REDGPU_INTERNAL_DEBUG_SEND_ERROR_MESSAGE(ctx, gpu, REDGPU_PRINT_PROCEDURE_ID(procedureId), 166, "RedCallProceduresAndAddresses::redCallSetDynamicViewport == NULL", optionalFile, optionalLine) }
    if (ctx->internal.gpuProcedureAddresses[i].redCallSetDynamicScissor                                   == NULL) { REDGPU_INTERNAL_DEBUG_SEND_ERROR_MESSAGE(ctx, gpu, REDGPU_PRINT_PROCEDURE_ID(procedureId), 167, "RedCallProceduresAndAddresses::redCallSetDynamicScissor == NULL", optionalFile, optionalLine) }
    if (ctx->internal.gpuProcedureAddresses[i].redCallSetProcedureOutputVirtual.redCallSetProcedureOutput == NULL) { REDGPU_INTERNAL_DEBUG_SEND_ERROR_MESSAGE(ctx, gpu, REDGPU_PRINT_PROCEDURE_ID(procedureId), 168, "RedCallProceduresAndAddresses::redCallSetProcedureOutput == NULL", optionalFile, optionalLine) }
    if (ctx->internal.gpuProcedureAddresses[i].redCallEndProcedureOutput                                  == NULL) { REDGPU_INTERNAL_DEBUG_SEND_ERROR_MESSAGE(ctx, gpu, REDGPU_PRINT_PROCEDURE_ID(procedureId), 169, "RedCallProceduresAndAddresses::redCallEndProcedureOutput == NULL", optionalFile, optionalLine) }
    if (ctx->internal.gpuProcedureAddresses[i].redCallUsageOrderBarrier                                   == NULL) { REDGPU_INTERNAL_DEBUG_SEND_ERROR_MESSAGE(ctx, gpu, REDGPU_PRINT_PROCEDURE_ID(procedureId), 170, "RedCallProceduresAndAddresses::redCallUsageOrderBarrier == NULL", optionalFile, optionalLine) }
#endif
  }

  for (unsigned i = 0; i < gpusCount; i += 1) {
    if (ctx->gpusStatuses[i].statusError != RED_STATUS_SUCCESS) {
      continue;
    }

    VkDevice             gpu      = (VkDevice)ctx->internal.hotGpuHandles[i];
    RedInternalGpuInfo * iGpuInfo = &cold->gpus[i];

    for (unsigned j = 0; j < iGpuInfo->queueFamilyDataCount; j += 1) {
      VkQueueFamilyProperties    * queueFamilyProperties = &iGpuInfo->vkQueueFamilyProperties[j];
      RedInternalQueueFamilyData * queueFamilyData       = &iGpuInfo->queueFamilyData[j];

      queueFamilyData->queuesCount = queueFamilyProperties->queueCount;
      REDGPU_CREATE_CONTEXT_ARRAY_MALLOC((RedHandleGpu)gpu, 171, queueFamilyData->queues, queueFamilyData->queuesCount, VkQueue, 31);
      for (unsigned k = 0; k < queueFamilyData->queuesCount; k += 1) {
        vk->vkGetDeviceQueue(gpu, j, k, &queueFamilyData->queues[k]);
      }
    }
  }

  for (unsigned i = 0; i < gpusCount; i += 1) {
    if (ctx->gpusStatuses[i].statusError != RED_STATUS_SUCCESS) {
      continue;
    }

    RedInternalTypeGpuInfo * eGpuInfo = (RedInternalTypeGpuInfo*)&ctx->gpus[i];
    RedInternalGpuInfo     * iGpuInfo = &cold->gpus[i];

    eGpuInfo->queuesDescription = NULL;

    eGpuInfo->queuesCount = 0;
    for (unsigned j = 0; j < iGpuInfo->queueFamilyDataCount; j += 1) {
      RedInternalQueueFamilyData * queueFamilyData = &iGpuInfo->queueFamilyData[j];

      eGpuInfo->queuesCount += queueFamilyData->queuesCount;
    }
    REDGPU_CREATE_CONTEXT_ARRAY_MALLOC(eGpuInfo->gpu, 172, eGpuInfo->queues,            eGpuInfo->queuesCount, RedHandleQueue, 32);
    REDGPU_CREATE_CONTEXT_ARRAY_MALLOC(eGpuInfo->gpu, 173, eGpuInfo->queuesFamilyIndex, eGpuInfo->queuesCount, unsigned, 33);
    REDGPU_CREATE_CONTEXT_ARRAY_MALLOC(eGpuInfo->gpu, 174, eGpuInfo->queuesCopyLimits,  eGpuInfo->queuesCount, RedQueueCopyLimits, 34);
    for (unsigned j = 0, k_offset = 0; j < iGpuInfo->queueFamilyDataCount; j += 1) {
      VkQueueFamilyProperties    * queueFamilyProperties = &iGpuInfo->vkQueueFamilyProperties[j];
      RedInternalQueueFamilyData * queueFamilyData       = &iGpuInfo->queueFamilyData[j];

      for (unsigned k = 0; k < queueFamilyData->queuesCount; k += 1) {
        eGpuInfo->queues[k_offset + k] = (RedHandleQueue)queueFamilyData->queues[k];
        eGpuInfo->queuesFamilyIndex[k_offset + k] = j;
        eGpuInfo->queuesCopyLimits[k_offset + k].copyBlockTexelsCountWidth  = queueFamilyProperties->minImageTransferGranularity.width;
        eGpuInfo->queuesCopyLimits[k_offset + k].copyBlockTexelsCountHeight = queueFamilyProperties->minImageTransferGranularity.height;
        eGpuInfo->queuesCopyLimits[k_offset + k].copyBlockTexelsCountDepth  = queueFamilyProperties->minImageTransferGranularity.depth;
      }
      k_offset += queueFamilyData->queuesCount;
    }
  }

  if (requestedExtensions[RED_SDK_EXTENSION_ARRAY_TIMESTAMP] == 1) {
    for (unsigned i = 0; i < gpusCount; i += 1) {
      if (ctx->gpusStatuses[i].statusError != RED_STATUS_SUCCESS) {
        continue;
      }

      RedGpuInfo         * eGpuInfo = &ctx->gpus[i];
      RedInternalGpuInfo * iGpuInfo = &cold->gpus[i];

      RedInternalTypeGpuInfoOptionalInfoArrayTimestamp * info = NULL;
      REDGPU_CREATE_CONTEXT_ARRAY_MALLOC(NULL, 369, info, 1, RedInternalTypeGpuInfoOptionalInfoArrayTimestamp, 101);
      info->optionalInfo                       = RED_GPU_INFO_OPTIONAL_INFO_ARRAY_TIMESTAMP;
      info->next                               = eGpuInfo->optionalInfo;
      info->supportsArrayTimestamp             = 0;
      info->queuesPrecisionBits64BitTicksCount = 0;
      info->nanosecondsPerTick                 = 0;
      eGpuInfo->optionalInfo                   = info;
      REDGPU_CREATE_CONTEXT_ARRAY_MALLOC(NULL, 370, info->queuesPrecisionBits64BitTicksCount, eGpuInfo->queuesCount, unsigned, 102);
      for (unsigned j = 0, k_offset = 0; j < iGpuInfo->queueFamilyDataCount; j += 1) {
        VkQueueFamilyProperties    * queueFamilyProperties = &iGpuInfo->vkQueueFamilyProperties[j];
        RedInternalQueueFamilyData * queueFamilyData       = &iGpuInfo->queueFamilyData[j];

        for (unsigned k = 0; k < queueFamilyData->queuesCount; k += 1) {
          info->queuesPrecisionBits64BitTicksCount[k_offset + k] = queueFamilyProperties->timestampValidBits;
        }
        k_offset += queueFamilyData->queuesCount;
      }
      for (unsigned j = 0; j < eGpuInfo->queuesCount; j += 1) {
        if (info->queuesPrecisionBits64BitTicksCount[j] != 0) {
          info->supportsArrayTimestamp = 1;
          break;
        }
      }
      info->nanosecondsPerTick = iGpuInfo->vkPhysicalDeviceProperties.limits.timestampPeriod;
    }
  }

  for (unsigned i = 0; i < gpusCount; i += 1) {
    if (ctx->gpusStatuses[i].statusError != RED_STATUS_SUCCESS) {
      continue;
    }

    RedInternalTypeGpuInfo * eGpuInfo = (RedInternalTypeGpuInfo *)&ctx->gpus[i];
    RedInternalGpuInfo *     iGpuInfo = &cold->gpus[i];

    REDGPU_CREATE_CONTEXT_ARRAY_MALLOC(eGpuInfo->gpu, 175, eGpuInfo->queuesCanCopy,    eGpuInfo->queuesCount, unsigned, 35);
    REDGPU_CREATE_CONTEXT_ARRAY_MALLOC(eGpuInfo->gpu, 176, eGpuInfo->queuesCanDraw,    eGpuInfo->queuesCount, unsigned, 36);
    REDGPU_CREATE_CONTEXT_ARRAY_MALLOC(eGpuInfo->gpu, 177, eGpuInfo->queuesCanCompute, eGpuInfo->queuesCount, unsigned, 37);

    redInternalSetCapableQueueBools(iGpuInfo, VK_QUEUE_TRANSFER_BIT, eGpuInfo->queuesCanCopy);
    redInternalSetCapableQueueBools(iGpuInfo, VK_QUEUE_GRAPHICS_BIT, eGpuInfo->queuesCanDraw);
    redInternalSetCapableQueueBools(iGpuInfo, VK_QUEUE_COMPUTE_BIT,  eGpuInfo->queuesCanCompute);
  }

  for (unsigned j = 0; j < gpusCount; j += 1) {
    if (ctx->gpusStatuses[j].statusError != RED_STATUS_SUCCESS) {
      continue;
    }

    RedInternalTypeGpuInfo * eGpuInfo = (RedInternalTypeGpuInfo *)&ctx->gpus[j];
    RedInternalGpuInfo *     iGpuInfo = &cold->gpus[j];

    VkPhysicalDevice device = cold->vkPhysicalDevices[j];

    REDGPU_CREATE_CONTEXT_ARRAY_MALLOC(eGpuInfo->gpu, 178, eGpuInfo->memoryTypes, 32, RedMemoryType, 38);
    REDGPU_CREATE_CONTEXT_ARRAY_MALLOC(eGpuInfo->gpu, 179, eGpuInfo->memoryHeaps, 16, RedMemoryHeap, 39);
    REDGPU_CREATE_CONTEXT_ARRAY_MALLOC(eGpuInfo->gpu, 180, eGpuInfo->imageFormatsFeatures, 131, RedImageFormatFeatures, 40);
    REDGPU_CREATE_CONTEXT_ARRAY_MALLOC(eGpuInfo->gpu, 181, eGpuInfo->imageFormatsLimitsImageDimensions1D, 131, RedImageFormatLimits, 41);
    REDGPU_CREATE_CONTEXT_ARRAY_MALLOC(eGpuInfo->gpu, 182, eGpuInfo->imageFormatsLimitsImageDimensions2D, 131, RedImageFormatLimits, 42);
    REDGPU_CREATE_CONTEXT_ARRAY_MALLOC(eGpuInfo->gpu, 183, eGpuInfo->imageFormatsLimitsImageDimensions2DMultisample, 131, RedImageFormatLimits, 43);
    REDGPU_CREATE_CONTEXT_ARRAY_MALLOC(eGpuInfo->gpu, 184, eGpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered, 131, RedImageFormatLimits, 44);
    REDGPU_CREATE_CONTEXT_ARRAY_MALLOC(eGpuInfo->gpu, 185, eGpuInfo->imageFormatsLimitsImageDimensions3D, 131, RedImageFormatLimits, 45);
    REDGPU_CREATE_CONTEXT_ARRAY_MALLOC(eGpuInfo->gpu, 186, eGpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered, 131, RedImageFormatLimits, 46);

    for (unsigned i = 0; i < VK_MAX_PHYSICAL_DEVICE_NAME_SIZE; i += 1) {
      eGpuInfo->gpuName[i] = iGpuInfo->vkPhysicalDeviceProperties.deviceName[i];
    }
    eGpuInfo->gpuType          = (RedGpuType)iGpuInfo->vkPhysicalDeviceProperties.deviceType;
    eGpuInfo->gpuVendorId      = iGpuInfo->vkPhysicalDeviceProperties.vendorID;
    eGpuInfo->gpuDeviceId      = iGpuInfo->vkPhysicalDeviceProperties.deviceID;
    eGpuInfo->gpuDriverVersion = iGpuInfo->vkPhysicalDeviceProperties.driverVersion;
    for (unsigned i = 0; i < VK_UUID_SIZE; i += 1) {
      eGpuInfo->gpuProcedureCacheUuid[i] = iGpuInfo->vkPhysicalDeviceProperties.pipelineCacheUUID[i];
    }
    eGpuInfo->memoryTypesCount = iGpuInfo->vkPhysicalDeviceMemoryProperties.memoryTypeCount;
    for (unsigned i = 0; i < VK_MAX_MEMORY_TYPES; i += 1) {
      eGpuInfo->memoryTypes[i].memoryHeapIndex = iGpuInfo->vkPhysicalDeviceMemoryProperties.memoryTypes[i].heapIndex;
      eGpuInfo->memoryTypes[i].isGpuVram       = (iGpuInfo->vkPhysicalDeviceMemoryProperties.memoryTypes[i].propertyFlags & VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT) == VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
      eGpuInfo->memoryTypes[i].isCpuMappable   = (iGpuInfo->vkPhysicalDeviceMemoryProperties.memoryTypes[i].propertyFlags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT) == VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;
      eGpuInfo->memoryTypes[i].isCpuCoherent   = (iGpuInfo->vkPhysicalDeviceMemoryProperties.memoryTypes[i].propertyFlags & VK_MEMORY_PROPERTY_HOST_COHERENT_BIT) == VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
      eGpuInfo->memoryTypes[i].isCpuCached     = (iGpuInfo->vkPhysicalDeviceMemoryProperties.memoryTypes[i].propertyFlags & VK_MEMORY_PROPERTY_HOST_CACHED_BIT) == VK_MEMORY_PROPERTY_HOST_CACHED_BIT;
    }
    eGpuInfo->memoryTypesDescription = NULL;
    eGpuInfo->memoryHeapsCount = iGpuInfo->vkPhysicalDeviceMemoryProperties.memoryHeapCount;
    for (unsigned i = 0; i < VK_MAX_MEMORY_HEAPS; i += 1) {
      eGpuInfo->memoryHeaps[i].memoryBytesCount = iGpuInfo->vkPhysicalDeviceMemoryProperties.memoryHeaps[i].size;
      eGpuInfo->memoryHeaps[i].isGpuVram        = (iGpuInfo->vkPhysicalDeviceMemoryProperties.memoryHeaps[i].flags & VK_MEMORY_HEAP_DEVICE_LOCAL_BIT) == VK_MEMORY_HEAP_DEVICE_LOCAL_BIT;
    }
    eGpuInfo->memoryHeapsDescription = NULL;

    eGpuInfo->maxImageDimensions1D = iGpuInfo->vkPhysicalDeviceProperties.limits.maxImageDimension1D;
    eGpuInfo->maxImageDimensions2D = iGpuInfo->vkPhysicalDeviceProperties.limits.maxImageDimension2D;
    eGpuInfo->maxImageDimensions3D = iGpuInfo->vkPhysicalDeviceProperties.limits.maxImageDimension3D;
    eGpuInfo->maxImageDimensionsCube = iGpuInfo->vkPhysicalDeviceProperties.limits.maxImageDimensionCube;
    eGpuInfo->maxImageLayersCount = iGpuInfo->vkPhysicalDeviceProperties.limits.maxImageArrayLayers;
    //        maxTexelBufferElements
    eGpuInfo->maxArrayROCStructMemberRangeBytesCount = (uint64_t)iGpuInfo->vkPhysicalDeviceProperties.limits.maxUniformBufferRange;
    eGpuInfo->maxArrayRORWStructMemberRangeBytesCount = (uint64_t)iGpuInfo->vkPhysicalDeviceProperties.limits.maxStorageBufferRange;
    eGpuInfo->maxProcedureParametersVariablesBytesCount = iGpuInfo->vkPhysicalDeviceProperties.limits.maxPushConstantsSize;
    eGpuInfo->maxMemoryAllocateCount = iGpuInfo->vkPhysicalDeviceProperties.limits.maxMemoryAllocationCount;
    eGpuInfo->maxCreateSamplerCount = iGpuInfo->vkPhysicalDeviceProperties.limits.maxSamplerAllocationCount;
    eGpuInfo->minMemoryPageSeparationArrayImageBytesCount = iGpuInfo->vkPhysicalDeviceProperties.limits.bufferImageGranularity;
    //        sparseAddressSpaceSize
    //        maxBoundDescriptorSets
    eGpuInfo->maxStructPerStageSamplersCount = iGpuInfo->vkPhysicalDeviceProperties.limits.maxPerStageDescriptorSamplers;
    eGpuInfo->maxStructPerStageArrayROCsCount = iGpuInfo->vkPhysicalDeviceProperties.limits.maxPerStageDescriptorUniformBuffers;
    eGpuInfo->maxStructPerStageArrayRORWsCount = iGpuInfo->vkPhysicalDeviceProperties.limits.maxPerStageDescriptorStorageBuffers;
    eGpuInfo->maxStructPerStageTextureROsCount = iGpuInfo->vkPhysicalDeviceProperties.limits.maxPerStageDescriptorSampledImages;
    eGpuInfo->maxStructPerStageTextureRWsCount = iGpuInfo->vkPhysicalDeviceProperties.limits.maxPerStageDescriptorStorageImages;
    //        maxPerStageDescriptorInputAttachments
    eGpuInfo->maxStructPerStageAllResourcesCount = iGpuInfo->vkPhysicalDeviceProperties.limits.maxPerStageResources;
    eGpuInfo->maxStructSamplersCount = iGpuInfo->vkPhysicalDeviceProperties.limits.maxDescriptorSetSamplers;
    eGpuInfo->maxStructArrayROCsCount = iGpuInfo->vkPhysicalDeviceProperties.limits.maxDescriptorSetUniformBuffers;
    //        maxDescriptorSetUniformBuffersDynamic
    eGpuInfo->maxStructArrayRORWsCount = iGpuInfo->vkPhysicalDeviceProperties.limits.maxDescriptorSetStorageBuffers;
    //        maxDescriptorSetStorageBuffersDynamic
    eGpuInfo->maxStructTextureROsCount = iGpuInfo->vkPhysicalDeviceProperties.limits.maxDescriptorSetSampledImages;
    eGpuInfo->maxStructTextureRWsCount = iGpuInfo->vkPhysicalDeviceProperties.limits.maxDescriptorSetStorageImages;
    //        maxDescriptorSetInputAttachments
    //        maxVertexInputAttributes
    //        maxVertexInputBindings
    //        maxVertexInputAttributeOffset
    //        maxVertexInputBindingStride
    eGpuInfo->maxVertexOutputLocationsCount = iGpuInfo->vkPhysicalDeviceProperties.limits.maxVertexOutputComponents;
    //        maxTessellationGenerationLevel
    //        maxTessellationPatchSize
    //        maxTessellationControlPerVertexInputComponents
    //        maxTessellationControlPerVertexOutputComponents
    //        maxTessellationControlPerPatchOutputComponents
    //        maxTessellationControlTotalOutputComponents
    //        maxTessellationEvaluationInputComponents
    //        maxTessellationEvaluationOutputComponents
    //        maxGeometryShaderInvocations
    //        maxGeometryInputComponents
    //        maxGeometryOutputComponents
    //        maxGeometryOutputVertices
    //        maxGeometryTotalOutputComponents
    eGpuInfo->maxFragmentInputLocationsCount = iGpuInfo->vkPhysicalDeviceProperties.limits.maxFragmentInputComponents;
    eGpuInfo->maxFragmentOutputColorsCount = iGpuInfo->vkPhysicalDeviceProperties.limits.maxFragmentOutputAttachments;
    eGpuInfo->maxFragmentOutputColorsCountBlendDualSource = iGpuInfo->vkPhysicalDeviceProperties.limits.maxFragmentDualSrcAttachments;
    eGpuInfo->maxFragmentOutputResourcesCount = iGpuInfo->vkPhysicalDeviceProperties.limits.maxFragmentCombinedOutputResources;
    eGpuInfo->maxComputeSharedMemoryBytesCount = iGpuInfo->vkPhysicalDeviceProperties.limits.maxComputeSharedMemorySize;
    eGpuInfo->maxComputeWorkgroupsCount[0] = iGpuInfo->vkPhysicalDeviceProperties.limits.maxComputeWorkGroupCount[0];
    eGpuInfo->maxComputeWorkgroupsCount[1] = iGpuInfo->vkPhysicalDeviceProperties.limits.maxComputeWorkGroupCount[1];
    eGpuInfo->maxComputeWorkgroupsCount[2] = iGpuInfo->vkPhysicalDeviceProperties.limits.maxComputeWorkGroupCount[2];
    eGpuInfo->maxComputeWorkgroupInvocationsCount = iGpuInfo->vkPhysicalDeviceProperties.limits.maxComputeWorkGroupInvocations;
    eGpuInfo->maxComputeWorkgroupDimensions[0] = iGpuInfo->vkPhysicalDeviceProperties.limits.maxComputeWorkGroupSize[0];
    eGpuInfo->maxComputeWorkgroupDimensions[1] = iGpuInfo->vkPhysicalDeviceProperties.limits.maxComputeWorkGroupSize[1];
    eGpuInfo->maxComputeWorkgroupDimensions[2] = iGpuInfo->vkPhysicalDeviceProperties.limits.maxComputeWorkGroupSize[2];
    eGpuInfo->precisionBitsViewportCoordinates = iGpuInfo->vkPhysicalDeviceProperties.limits.subPixelPrecisionBits;
    eGpuInfo->precisionBitsSamplerFilteringMagMin = iGpuInfo->vkPhysicalDeviceProperties.limits.subTexelPrecisionBits;
    eGpuInfo->precisionBitsSamplerFilteringMip = iGpuInfo->vkPhysicalDeviceProperties.limits.mipmapPrecisionBits;
    eGpuInfo->maxArrayIndexUint32Value = iGpuInfo->vkPhysicalDeviceProperties.limits.maxDrawIndexedIndexValue;
    //        maxDrawIndirectCount
    eGpuInfo->maxSamplerMipLodBias = iGpuInfo->vkPhysicalDeviceProperties.limits.maxSamplerLodBias;
    eGpuInfo->maxSamplerAnisotropy = iGpuInfo->vkPhysicalDeviceProperties.limits.maxSamplerAnisotropy;
    //        maxViewports
    eGpuInfo->maxViewportDimensions[0] = iGpuInfo->vkPhysicalDeviceProperties.limits.maxViewportDimensions[0];
    eGpuInfo->maxViewportDimensions[1] = iGpuInfo->vkPhysicalDeviceProperties.limits.maxViewportDimensions[1];
    eGpuInfo->minViewportBoundsRange = iGpuInfo->vkPhysicalDeviceProperties.limits.viewportBoundsRange[0];
    eGpuInfo->maxViewportBoundsRange = iGpuInfo->vkPhysicalDeviceProperties.limits.viewportBoundsRange[1];
    eGpuInfo->precisionBitsViewportBounds = iGpuInfo->vkPhysicalDeviceProperties.limits.viewportSubPixelBits;
    eGpuInfo->minMemoryAllocateBytesAlignment = iGpuInfo->vkPhysicalDeviceProperties.limits.minMemoryMapAlignment;
    //        minTexelBufferOffsetAlignment
    eGpuInfo->minArrayROCStructMemberRangeBytesAlignment = iGpuInfo->vkPhysicalDeviceProperties.limits.minUniformBufferOffsetAlignment;
    eGpuInfo->minArrayRORWStructMemberRangeBytesAlignment = iGpuInfo->vkPhysicalDeviceProperties.limits.minStorageBufferOffsetAlignment;
    eGpuInfo->minImageSampleImageFetchOffset = iGpuInfo->vkPhysicalDeviceProperties.limits.minTexelOffset;
    eGpuInfo->maxImageSampleImageFetchOffset = iGpuInfo->vkPhysicalDeviceProperties.limits.maxTexelOffset;
    eGpuInfo->minImageGatherOffset = iGpuInfo->vkPhysicalDeviceProperties.limits.minTexelGatherOffset;
    eGpuInfo->maxImageGatherOffset = iGpuInfo->vkPhysicalDeviceProperties.limits.maxTexelGatherOffset;
    eGpuInfo->minInterpolateAtOffset = iGpuInfo->vkPhysicalDeviceProperties.limits.minInterpolationOffset;
    eGpuInfo->maxInterpolateAtOffset = iGpuInfo->vkPhysicalDeviceProperties.limits.maxInterpolationOffset;
    eGpuInfo->precisionBitsInterpolateAtOffset = iGpuInfo->vkPhysicalDeviceProperties.limits.subPixelInterpolationOffsetBits;
    eGpuInfo->maxOutputWidth = iGpuInfo->vkPhysicalDeviceProperties.limits.maxFramebufferWidth;
    eGpuInfo->maxOutputHeight = iGpuInfo->vkPhysicalDeviceProperties.limits.maxFramebufferHeight;
    //        maxFramebufferLayers
    eGpuInfo->supportedMultisampleCountsForOutputColor = iGpuInfo->vkPhysicalDeviceProperties.limits.framebufferColorSampleCounts;
    eGpuInfo->supportedMultisampleCountsForOutputDepth = iGpuInfo->vkPhysicalDeviceProperties.limits.framebufferDepthSampleCounts;
    eGpuInfo->supportedMultisampleCountsForOutputStencil = iGpuInfo->vkPhysicalDeviceProperties.limits.framebufferStencilSampleCounts;
    eGpuInfo->supportedMultisampleCountsForEmptyOutput = iGpuInfo->vkPhysicalDeviceProperties.limits.framebufferNoAttachmentsSampleCounts;
    eGpuInfo->maxOutputColorsCount = iGpuInfo->vkPhysicalDeviceProperties.limits.maxColorAttachments;
    //        sampledImageColorSampleCounts
    //        sampledImageIntegerSampleCounts
    //        sampledImageDepthSampleCounts
    //        sampledImageStencilSampleCounts
    //        storageImageSampleCounts
    //        maxSampleMaskWords
    //        timestampComputeAndGraphics
    //        timestampPeriod
    eGpuInfo->maxPerStageClipDistances = iGpuInfo->vkPhysicalDeviceProperties.limits.maxClipDistances;
    eGpuInfo->maxPerStageCullDistances = iGpuInfo->vkPhysicalDeviceProperties.limits.maxCullDistances;
    eGpuInfo->maxPerStageClipAndCullDistances = iGpuInfo->vkPhysicalDeviceProperties.limits.maxCombinedClipAndCullDistances;
    //        discreteQueuePriorities
    //        pointSizeRange[0]
    //        pointSizeRange[1]
    //        lineWidthRange[0]
    //        lineWidthRange[1]
    //        pointSizeGranularity
    //        lineWidthGranularity
    //        strictLines
    eGpuInfo->supportsMultisampleStandardSampleLocations = iGpuInfo->vkPhysicalDeviceProperties.limits.standardSampleLocations;
    eGpuInfo->optimalCopyArrayImageRangeArrayBytesFirstBytesAlignment = iGpuInfo->vkPhysicalDeviceProperties.limits.optimalBufferCopyOffsetAlignment;
    eGpuInfo->optimalCopyArrayImageRangeArrayTexelsCountToNextRowBytesAlignment = iGpuInfo->vkPhysicalDeviceProperties.limits.optimalBufferCopyRowPitchAlignment;
    eGpuInfo->minMemoryNonCoherentBlockBytesCount = iGpuInfo->vkPhysicalDeviceProperties.limits.nonCoherentAtomSize;

    //        residencyStandard2DBlockShape
    //        residencyStandard2DMultisampleBlockShape
    //        residencyStandard3DBlockShape
    //        residencyAlignedMipSize
    //        residencyNonResidentStrict

    // GPU features
    eGpuInfo->supportsProcedureStateRasterizationDepthBiasDynamic = 1;
    eGpuInfo->supportsProcedureStateDepthTestBoundsTestDynamic = 1;
    eGpuInfo->supportsProcedureStateStencilTestFrontAndBackDynamicCompareMask = 1;
    eGpuInfo->supportsProcedureStateStencilTestFrontAndBackDynamicWriteMask = 1;
    //        robustBufferAccess
    eGpuInfo->supportsFullArrayIndexUint32Value = iGpuInfo->vkPhysicalDeviceFeatures.fullDrawIndexUint32;
    eGpuInfo->supportsTextureDimensionsCubeLayered = iGpuInfo->vkPhysicalDeviceFeatures.imageCubeArray;
    eGpuInfo->supportsProcedureStateOutputColorsBlendVaryingPerColor = iGpuInfo->vkPhysicalDeviceFeatures.independentBlend;
    //        geometryShader
    //        tessellationShader
    eGpuInfo->supportsProcedureStateMultisampleSampleShading = iGpuInfo->vkPhysicalDeviceFeatures.sampleRateShading;
    eGpuInfo->supportsProcedureStateOutputColorsBlendDualSource = iGpuInfo->vkPhysicalDeviceFeatures.dualSrcBlend;
    eGpuInfo->supportsProcedureStateBlendLogicOp = iGpuInfo->vkPhysicalDeviceFeatures.logicOp;
    //        multiDrawIndirect
    //        drawIndirectFirstInstance
    eGpuInfo->supportsProcedureStateRasterizationDepthClamp = iGpuInfo->vkPhysicalDeviceFeatures.depthClamp;
    eGpuInfo->supportsProcedureStateRasterizationDepthBiasClamp = iGpuInfo->vkPhysicalDeviceFeatures.depthBiasClamp;
    //        fillModeNonSolid
    eGpuInfo->supportsProcedureStateDepthTestBoundsTest = iGpuInfo->vkPhysicalDeviceFeatures.depthBounds;
    //        wideLines
    //        largePoints
    eGpuInfo->supportsProcedureStateMultisampleAlphaToOne = iGpuInfo->vkPhysicalDeviceFeatures.alphaToOne;
    //        multiViewport
    eGpuInfo->supportsSamplerAnisotropy = iGpuInfo->vkPhysicalDeviceFeatures.samplerAnisotropy;
    //        textureCompressionETC2
    //        textureCompressionASTCLDR
    //        textureCompressionBC
    //        occlusionQueryPrecise
    //        pipelineStatisticsQuery
    eGpuInfo->supportsGpuCodeWritesAndAtomicsInStageVertex = iGpuInfo->vkPhysicalDeviceFeatures.vertexPipelineStoresAndAtomics;
    eGpuInfo->supportsGpuCodeWritesAndAtomicsInStageFragment = iGpuInfo->vkPhysicalDeviceFeatures.fragmentStoresAndAtomics;
    //        shaderTessellationAndGeometryPointSize
    eGpuInfo->supportsGpuCodeImageGatherExtended = iGpuInfo->vkPhysicalDeviceFeatures.shaderImageGatherExtended;
    eGpuInfo->supportsGpuCodeTextureRWExtendedFormats = iGpuInfo->vkPhysicalDeviceFeatures.shaderStorageImageExtendedFormats;
    eGpuInfo->supportsGpuCodeTextureRWMultisample = iGpuInfo->vkPhysicalDeviceFeatures.shaderStorageImageMultisample;
    eGpuInfo->supportsGpuCodeTextureRWReadWithoutFormat = iGpuInfo->vkPhysicalDeviceFeatures.shaderStorageImageReadWithoutFormat;
    eGpuInfo->supportsGpuCodeTextureRWWriteWithoutFormat = iGpuInfo->vkPhysicalDeviceFeatures.shaderStorageImageWriteWithoutFormat;
    eGpuInfo->supportsGpuCodeDynamicallyIndexableArraysOfArrayROCs = iGpuInfo->vkPhysicalDeviceFeatures.shaderUniformBufferArrayDynamicIndexing;
    eGpuInfo->supportsGpuCodeDynamicallyIndexableArraysOfArrayRORWs = iGpuInfo->vkPhysicalDeviceFeatures.shaderStorageBufferArrayDynamicIndexing;
    eGpuInfo->supportsGpuCodeDynamicallyIndexableArraysOfSamplers = iGpuInfo->vkPhysicalDeviceFeatures.shaderSampledImageArrayDynamicIndexing;
    eGpuInfo->supportsGpuCodeDynamicallyIndexableArraysOfTextureROs = iGpuInfo->vkPhysicalDeviceFeatures.shaderSampledImageArrayDynamicIndexing;
    eGpuInfo->supportsGpuCodeDynamicallyIndexableArraysOfTextureRWs = iGpuInfo->vkPhysicalDeviceFeatures.shaderStorageImageArrayDynamicIndexing;
    eGpuInfo->supportsGpuCodeClipDistance = iGpuInfo->vkPhysicalDeviceFeatures.shaderClipDistance;
    eGpuInfo->supportsGpuCodeCullDistance = iGpuInfo->vkPhysicalDeviceFeatures.shaderCullDistance;
    eGpuInfo->supportsGpuCodeFloat64 = iGpuInfo->vkPhysicalDeviceFeatures.shaderFloat64;
    eGpuInfo->supportsGpuCodeInt64 = iGpuInfo->vkPhysicalDeviceFeatures.shaderInt64;
    //        shaderInt16
    //        shaderResourceResidency
    eGpuInfo->supportsGpuCodeMinLod = iGpuInfo->vkPhysicalDeviceFeatures.shaderResourceMinLod;
    //        sparseBinding
    //        sparseResidencyBuffer
    //        sparseResidencyImage2D
    //        sparseResidencyImage3D
    //        sparseResidency2Samples
    //        sparseResidency4Samples
    //        sparseResidency8Samples
    //        sparseResidency16Samples
    //        sparseResidencyAliased
    eGpuInfo->supportsMultisampleEmptyOutputVariableMultisampleCount = iGpuInfo->vkPhysicalDeviceFeatures.variableMultisampleRate;
    //        inheritedQueries

    {
      struct RedFormatProperties {
        unsigned featuresImageTilingLinear;
        unsigned featuresImageTilingSwizzled;
        unsigned featuresBuffer;
      } formatProperties = {0};

      for (unsigned i = 0; i < 131; i += 1) {
        if (i != 0) {
          vk->vkGetPhysicalDeviceFormatProperties(device, (VkFormat)i, (VkFormatProperties *)&formatProperties);
        }
        eGpuInfo->imageFormatsFeatures[i].supportsCopyR                    = ((formatProperties.featuresImageTilingSwizzled & VK_FORMAT_FEATURE_TRANSFER_SRC_BIT_KHR)            == VK_FORMAT_FEATURE_TRANSFER_SRC_BIT_KHR)            ? 1 : 0;
        eGpuInfo->imageFormatsFeatures[i].supportsCopyW                    = ((formatProperties.featuresImageTilingSwizzled & VK_FORMAT_FEATURE_TRANSFER_DST_BIT_KHR)            == VK_FORMAT_FEATURE_TRANSFER_DST_BIT_KHR)            ? 1 : 0;
        eGpuInfo->imageFormatsFeatures[i].supportsTextureRO                = ((formatProperties.featuresImageTilingSwizzled & VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT)               == VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT)               ? 1 : 0;
        eGpuInfo->imageFormatsFeatures[i].supportsTextureRW                = ((formatProperties.featuresImageTilingSwizzled & VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT)               == VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT)               ? 1 : 0;
        eGpuInfo->imageFormatsFeatures[i].supportsOutputDepthStencil       = ((formatProperties.featuresImageTilingSwizzled & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT)    == VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT)    ? 1 : 0;
        eGpuInfo->imageFormatsFeatures[i].supportsOutputColor              = ((formatProperties.featuresImageTilingSwizzled & VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT)            == VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT)            ? 1 : 0;
        eGpuInfo->imageFormatsFeatures[i].supportsTextureROFilteringLinear = ((formatProperties.featuresImageTilingSwizzled & VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT) == VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT) ? 1 : 0;
        eGpuInfo->imageFormatsFeatures[i].supportsTextureRWAtomics         = ((formatProperties.featuresImageTilingSwizzled & VK_FORMAT_FEATURE_STORAGE_IMAGE_ATOMIC_BIT)        == VK_FORMAT_FEATURE_STORAGE_IMAGE_ATOMIC_BIT)        ? 1 : 0;
        eGpuInfo->imageFormatsFeatures[i].supportsOutputColorBlend         = ((formatProperties.featuresImageTilingSwizzled & VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT)      == VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT)      ? 1 : 0;
      }

      RedImageFormatLimits imageFormatLimitsDefaults = {0};

      for (unsigned i = 0; i < 131; i += 1) {
        if (i == 0) {
          eGpuInfo->imageFormatsLimitsImageDimensions1D[i]                                        = imageFormatLimitsDefaults;
          eGpuInfo->imageFormatsLimitsImageDimensions2D[i]                                        = imageFormatLimitsDefaults;
          eGpuInfo->imageFormatsLimitsImageDimensions2DMultisample[i]                             = imageFormatLimitsDefaults;
          eGpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[i] = imageFormatLimitsDefaults;
          eGpuInfo->imageFormatsLimitsImageDimensions3D[i]                                        = imageFormatLimitsDefaults;
          eGpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[i]     = imageFormatLimitsDefaults;

          eGpuInfo->imageFormatsLimitsImageDimensions1D[i].status                                        = RED_STATUS_ERROR_FORMAT_IS_NOT_SUPPORTED;
          eGpuInfo->imageFormatsLimitsImageDimensions2D[i].status                                        = RED_STATUS_ERROR_FORMAT_IS_NOT_SUPPORTED;
          eGpuInfo->imageFormatsLimitsImageDimensions2DMultisample[i].status                             = RED_STATUS_ERROR_FORMAT_IS_NOT_SUPPORTED;
          eGpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[i].status = RED_STATUS_ERROR_FORMAT_IS_NOT_SUPPORTED;
          eGpuInfo->imageFormatsLimitsImageDimensions3D[i].status                                        = RED_STATUS_ERROR_FORMAT_IS_NOT_SUPPORTED;
          eGpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[i].status     = RED_STATUS_ERROR_FORMAT_IS_NOT_SUPPORTED;
          continue;
        }

        VkResult result1 = redInternalImageGetLimits(ctx, j, device, RED_IMAGE_DIMENSIONS_1D,                                               (RedFormat)i, 0, &eGpuInfo->imageFormatsLimitsImageDimensions1D[i]);
        VkResult result2 = redInternalImageGetLimits(ctx, j, device, RED_IMAGE_DIMENSIONS_2D,                                               (RedFormat)i, 0, &eGpuInfo->imageFormatsLimitsImageDimensions2D[i]);
        VkResult result3 = redInternalImageGetLimits(ctx, j, device, RED_IMAGE_DIMENSIONS_2D,                                               (RedFormat)i, 1, &eGpuInfo->imageFormatsLimitsImageDimensions2DMultisample[i]);
        VkResult result4 = redInternalImageGetLimits(ctx, j, device, RED_IMAGE_DIMENSIONS_2D_WITH_TEXTURE_DIMENSIONS_CUBE_AND_CUBE_LAYERED, (RedFormat)i, 0, &eGpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[i]);
        VkResult result5 = redInternalImageGetLimits(ctx, j, device, RED_IMAGE_DIMENSIONS_3D,                                               (RedFormat)i, 0, &eGpuInfo->imageFormatsLimitsImageDimensions3D[i]);
        VkResult result6 = redInternalImageGetLimits(ctx, j, device, RED_IMAGE_DIMENSIONS_3D_WITH_TEXTURE_DIMENSIONS_2D_AND_2D_LAYERED,     (RedFormat)i, 0, &eGpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[i]);

        eGpuInfo->imageFormatsLimitsImageDimensions1D[i].status                                        = RED_STATUS_SUCCESS;
        eGpuInfo->imageFormatsLimitsImageDimensions2D[i].status                                        = RED_STATUS_SUCCESS;
        eGpuInfo->imageFormatsLimitsImageDimensions2DMultisample[i].status                             = RED_STATUS_SUCCESS;
        eGpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[i].status = RED_STATUS_SUCCESS;
        eGpuInfo->imageFormatsLimitsImageDimensions3D[i].status                                        = RED_STATUS_SUCCESS;
        eGpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[i].status     = RED_STATUS_SUCCESS;

        if (result1 != (VkResult)RED_STATUS_SUCCESS) { eGpuInfo->imageFormatsLimitsImageDimensions1D[i]                                        = imageFormatLimitsDefaults; eGpuInfo->imageFormatsLimitsImageDimensions1D[i].status                                        = (RedStatus)result1; }
        if (result2 != (VkResult)RED_STATUS_SUCCESS) { eGpuInfo->imageFormatsLimitsImageDimensions2D[i]                                        = imageFormatLimitsDefaults; eGpuInfo->imageFormatsLimitsImageDimensions2D[i].status                                        = (RedStatus)result2; }
        if (result3 != (VkResult)RED_STATUS_SUCCESS) { eGpuInfo->imageFormatsLimitsImageDimensions2DMultisample[i]                             = imageFormatLimitsDefaults; eGpuInfo->imageFormatsLimitsImageDimensions2DMultisample[i].status                             = (RedStatus)result3; }
        if (result4 != (VkResult)RED_STATUS_SUCCESS) { eGpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[i] = imageFormatLimitsDefaults; eGpuInfo->imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[i].status = (RedStatus)result4; }
        if (result5 != (VkResult)RED_STATUS_SUCCESS) { eGpuInfo->imageFormatsLimitsImageDimensions3D[i]                                        = imageFormatLimitsDefaults; eGpuInfo->imageFormatsLimitsImageDimensions3D[i].status                                        = (RedStatus)result5; }
        if (result6 != (VkResult)RED_STATUS_SUCCESS) { eGpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[i]     = imageFormatLimitsDefaults; eGpuInfo->imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[i].status     = (RedStatus)result6; }
      }
    }
  }

#if REDGPU_COMPILE_SWITCH == REDGPU_COMPILE_SWITCH_DEBUG
  {
    const char * debugArrayHandleName = NULL;
    {
      void * settingsToIterate = (void *)optionalSettings;
      while (settingsToIterate != NULL) {
        const RedContextOptionalSettingsIterator * settings = (const RedContextOptionalSettingsIterator *)settingsToIterate;
        if (settings->settings == RED_CONTEXT_OPTIONAL_SETTINGS_DEBUG_ARRAY) {
          const RedContextOptionalSettingsDebugArray * settingsDebugArray = (const RedContextOptionalSettingsDebugArray *)settingsToIterate;
          debugArrayHandleName                = settingsDebugArray->debugArrayHandleName;
          cold->debugArrayStructsCount        = settingsDebugArray->debugArrayStructsCount;
          cold->debugArrayStructsFloat4sCount = settingsDebugArray->debugArrayStructsFloat4sCount;
          cold->debugArrayPrintCallback       = settingsDebugArray->printCallback;
        }
        void * nextSettingsToIterate = (void *)settings->next;
        settingsToIterate = nextSettingsToIterate;
      }
    }
    if (cold->debugArrayStructsCount > 0 && cold->debugArrayStructsFloat4sCount > 0) {
      for (unsigned i = 0; i < gpusCount; i += 1) {
        if (ctx->gpusStatuses[i].statusError != RED_STATUS_SUCCESS) {
          continue;
        }

        RedGpuInfo         * eGpuInfo = &ctx->gpus[i];
        RedInternalGpuInfo * iGpuInfo = &cold->gpus[i];

        RedStatuses statuses = {0};

        const uint64_t debugArrayBytesCount = cold->debugArrayStructsCount * cold->debugArrayStructsFloat4sCount * 4 * sizeof(float);
        iGpuInfo->debugArray = redDebugInternalCreateDebugArray(ctx, eGpuInfo->gpu, i, debugArrayHandleName, RED_ARRAY_TYPE_ARRAY_RW, debugArrayBytesCount, -1, 0, RED_DEBUG_INTERNAL_MEMORY_TYPE_UPLOAD, &statuses, optionalFile, optionalLine, optionalUserData);
        if (statuses.statusError != RED_STATUS_SUCCESS) {
          redInternalSetStatus(ctx, eGpuInfo->gpu, &ctx->gpusStatuses[i], 255, (VkResult)RED_STATUS_ERROR_FEATURE_IS_NOT_FOUND, procedureId, optionalFile, optionalLine, optionalUserData);
          if (iGpuInfo->debugArray.array.handle != NULL) {
            redDebugInternalDestroyArray(ctx, eGpuInfo->gpu, iGpuInfo->debugArray.array.handle);
          }
          if (iGpuInfo->debugArray.memory.handle != NULL) {
            redDebugInternalMemoryFree(ctx, eGpuInfo->gpu, iGpuInfo->debugArray.memory.handle);
          }
          RedDebugInternalArray defaults = {0};
          iGpuInfo->debugArray = defaults;
          continue;
        }

        redDebugInternalCreateEvent(ctx, eGpuInfo->gpu, debugArrayHandleName, &iGpuInfo->debugArrayGpuToCpuEvent, &statuses, optionalFile, optionalLine, optionalUserData);
        if (statuses.statusError != RED_STATUS_SUCCESS) {
          redInternalSetStatus(ctx, eGpuInfo->gpu, &ctx->gpusStatuses[i], 256, (VkResult)RED_STATUS_ERROR_FEATURE_IS_NOT_FOUND, procedureId, optionalFile, optionalLine, optionalUserData);
          redDebugInternalDestroyArray(ctx, eGpuInfo->gpu, iGpuInfo->debugArray.array.handle);
          redDebugInternalMemoryFree(ctx, eGpuInfo->gpu, iGpuInfo->debugArray.memory.handle);
          RedDebugInternalArray defaults = {0};
          iGpuInfo->debugArray = defaults;
          continue;
        }

        redDebugInternalCreateEvent(ctx, eGpuInfo->gpu, debugArrayHandleName, &iGpuInfo->debugArrayCpuToGpuEvent, &statuses, optionalFile, optionalLine, optionalUserData);
        if (statuses.statusError != RED_STATUS_SUCCESS) {
          redInternalSetStatus(ctx, eGpuInfo->gpu, &ctx->gpusStatuses[i], 257, (VkResult)RED_STATUS_ERROR_FEATURE_IS_NOT_FOUND, procedureId, optionalFile, optionalLine, optionalUserData);
          redDebugInternalDestroyArray(ctx, eGpuInfo->gpu, iGpuInfo->debugArray.array.handle);
          redDebugInternalMemoryFree(ctx, eGpuInfo->gpu, iGpuInfo->debugArray.memory.handle);
          RedDebugInternalArray defaults = {0};
          iGpuInfo->debugArray = defaults;
          redDebugInternalDestroyEvent(ctx, eGpuInfo->gpu, iGpuInfo->debugArrayGpuToCpuEvent);
          iGpuInfo->debugArrayGpuToCpuEvent = NULL;
          continue;
        }

        redDebugInternalCreateCpuSignal(ctx, eGpuInfo->gpu, debugArrayHandleName, 0, &iGpuInfo->debugArrayCpuSignal, &statuses, optionalFile, optionalLine, optionalUserData);
        if (statuses.statusError != RED_STATUS_SUCCESS) {
          redInternalSetStatus(ctx, eGpuInfo->gpu, &ctx->gpusStatuses[i], 258, (VkResult)RED_STATUS_ERROR_FEATURE_IS_NOT_FOUND, procedureId, optionalFile, optionalLine, optionalUserData);
          redDebugInternalDestroyArray(ctx, eGpuInfo->gpu, iGpuInfo->debugArray.array.handle);
          redDebugInternalMemoryFree(ctx, eGpuInfo->gpu, iGpuInfo->debugArray.memory.handle);
          RedDebugInternalArray defaults = {0};
          iGpuInfo->debugArray = defaults;
          redDebugInternalDestroyEvent(ctx, eGpuInfo->gpu, iGpuInfo->debugArrayGpuToCpuEvent);
          iGpuInfo->debugArrayGpuToCpuEvent = NULL;
          redDebugInternalDestroyEvent(ctx, eGpuInfo->gpu, iGpuInfo->debugArrayCpuToGpuEvent);
          iGpuInfo->debugArrayCpuToGpuEvent = NULL;
          continue;
        }

        iGpuInfo->debugArrayFeatureIsSupported = 1;
      }
    }
  }
#endif

  for (unsigned j = 0; j < gpusCount; j += 1) {
    if (ctx->gpusStatuses[j].statusError != RED_STATUS_SUCCESS) {
      continue;
    }

    RedGpuInfo         * eGpuInfo = &ctx->gpus[j];
    RedInternalGpuInfo * iGpuInfo = &cold->gpus[j];

    if (iGpuInfo->vkPhysicalDeviceProperties.limits.maxBoundDescriptorSets < 8) {
      redInternalSetStatus(ctx, eGpuInfo->gpu, &ctx->gpusStatuses[j], 363, (VkResult)RED_STATUS_ERROR_INCOMPATIBLE_DRIVER, procedureId, optionalFile, optionalLine, optionalUserData);
    }
  }

  outContext[0] = (RedContext)ctx;

#if REDGPU_COMPILE_SWITCH == REDGPU_COMPILE_SWITCH_DEBUG
  {
    std::lock_guard<std::mutex> __debugPrintGuardAndSerializeMutexScopeGuard(__REDGPU_DEBUG_GLOBAL_3f5f84a81695ef9521b544bdc3d7d73cbb4aac3c18ce99fd19bdfd306996103e_debugPrintGuardAndSerializeMutex);

    if (ctx == NULL) {
      if (optionalFile != NULL) {
        REDGPU_PRINTF("[REDGPU DEBUG LIBRARY][RED_PROCEDURE_ID_redCreateContext][%s:%d]: %s\n", optionalFile, optionalLine, "Returned context is NULL.");
      } else {
        REDGPU_PRINTF("[REDGPU DEBUG LIBRARY][RED_PROCEDURE_ID_redCreateContext]: %s\n", "Returned context is NULL.");
      }
    } else {
      RedContext context = (RedContext)ctx;
      if (optionalFile != NULL) {
        REDGPU_PRINTF("[REDGPU DEBUG LIBRARY][RED_PROCEDURE_ID_redCreateContext][%s:%d]: context = %p;\n", optionalFile, optionalLine, REDGPU_PRINT_POINTER(context));
      } else {
        REDGPU_PRINTF("[REDGPU DEBUG LIBRARY][RED_PROCEDURE_ID_redCreateContext]: context = %p;\n", REDGPU_PRINT_POINTER(context));
      }
      REDGPU_PRINTF("context.gpusCount = %d;\n", context->gpusCount);
      if (context->gpusCount == 0) {
      REDGPU_PRINTF("context.gpus = NULL;\n");
      }
      for (unsigned gpuIndex = 0; gpuIndex < context->gpusCount; gpuIndex += 1) {
        REDGPU_PRINTF("context.gpus[%d].gpu = %p;\n",                                             gpuIndex, REDGPU_PRINT_POINTER(context->gpus[gpuIndex].gpu));
        REDGPU_PRINTF("context.gpus[%d].gpuDevice = %p;\n",                                       gpuIndex, REDGPU_PRINT_POINTER(context->gpus[gpuIndex].gpuDevice));
        REDGPU_PRINTF("context.gpus[%d].gpuName = \"%s\";\n",                                     gpuIndex, context->gpus[gpuIndex].gpuName[0] == 0 ? "" : context->gpus[gpuIndex].gpuName);
        REDGPU_PRINTF("context.gpus[%d].gpuType = %s;\n",                                         gpuIndex, REDGPU_PRINT_GPU_TYPE(context->gpus[gpuIndex].gpuType));
        REDGPU_PRINTF("context.gpus[%d].gpuVendorId = %d;\n",                                     gpuIndex, context->gpus[gpuIndex].gpuVendorId);
        REDGPU_PRINTF("context.gpus[%d].gpuDeviceId = %d;\n",                                     gpuIndex, context->gpus[gpuIndex].gpuDeviceId);
        REDGPU_PRINTF("context.gpus[%d].gpuDriverVersion = %d;\n",                                gpuIndex, context->gpus[gpuIndex].gpuDriverVersion);
        REDGPU_PRINTF("context.gpus[%d].gpuProcedureCacheUuid = {%d",                             gpuIndex, (int)context->gpus[gpuIndex].gpuProcedureCacheUuid[0]);
        for (unsigned i = 1; i < 16; i += 1) {
          REDGPU_PRINTF(", %d",                                                                             (int)context->gpus[gpuIndex].gpuProcedureCacheUuid[i]);
        }
        REDGPU_PRINTF("};\n");
        REDGPU_PRINTF("context.gpus[%d].memoryTypesCount = %d;\n",                                gpuIndex, context->gpus[gpuIndex].memoryTypesCount);
        if (context->gpus[gpuIndex].memoryTypesCount == 0) {
        REDGPU_PRINTF("context.gpus[%d].memoryTypes = NULL;\n",                                   gpuIndex);
        }
        for (unsigned i = 0; i < context->gpus[gpuIndex].memoryTypesCount; i += 1) {
        REDGPU_PRINTF("context.gpus[%d].memoryTypes[%d].memoryHeapIndex = %d;\n",                 gpuIndex, i, context->gpus[gpuIndex].memoryTypes[i].memoryHeapIndex);
        REDGPU_PRINTF("context.gpus[%d].memoryTypes[%d].isGpuVram = %d;\n",                       gpuIndex, i, context->gpus[gpuIndex].memoryTypes[i].isGpuVram);
        REDGPU_PRINTF("context.gpus[%d].memoryTypes[%d].isCpuMappable = %d;\n",                   gpuIndex, i, context->gpus[gpuIndex].memoryTypes[i].isCpuMappable);
        REDGPU_PRINTF("context.gpus[%d].memoryTypes[%d].isCpuCoherent = %d;\n",                   gpuIndex, i, context->gpus[gpuIndex].memoryTypes[i].isCpuCoherent);
        REDGPU_PRINTF("context.gpus[%d].memoryTypes[%d].isCpuCached = %d;\n",                     gpuIndex, i, context->gpus[gpuIndex].memoryTypes[i].isCpuCached);
        }
        REDGPU_PRINTF("context.gpus[%d].memoryTypesDescription = NULL;\n",                        gpuIndex);
        REDGPU_PRINTF("context.gpus[%d].memoryHeapsCount = %d;\n",                                gpuIndex, context->gpus[gpuIndex].memoryHeapsCount);
        if (context->gpus[gpuIndex].memoryHeapsCount == 0) {
        REDGPU_PRINTF("context.gpus[%d].memoryHeaps = NULL;\n",                                   gpuIndex);
        }
        for (unsigned i = 0; i < context->gpus[gpuIndex].memoryHeapsCount; i += 1) {
        REDGPU_PRINTF("context.gpus[%d].memoryHeaps[%d].memoryBytesCount = %zu;\n",               gpuIndex, i, (uint64_t)context->gpus[gpuIndex].memoryHeaps[i].memoryBytesCount);
        REDGPU_PRINTF("context.gpus[%d].memoryHeaps[%d].isGpuVram = %d;\n",                       gpuIndex, i, context->gpus[gpuIndex].memoryHeaps[i].isGpuVram);
        }
        REDGPU_PRINTF("context.gpus[%d].memoryHeapsDescription = NULL;\n",                        gpuIndex);
        REDGPU_PRINTF("context.gpus[%d].queuesCount = %d;\n",                                     gpuIndex, context->gpus[gpuIndex].queuesCount);
        if (context->gpus[gpuIndex].queuesCount == 0) {
        REDGPU_PRINTF("context.gpus[%d].queues = NULL;\n",                                        gpuIndex);
        REDGPU_PRINTF("context.gpus[%d].queuesFamilyIndex = NULL;\n",                             gpuIndex);
        REDGPU_PRINTF("context.gpus[%d].queuesCanCopy = NULL;\n",                                 gpuIndex);
        REDGPU_PRINTF("context.gpus[%d].queuesCanDraw = NULL;\n",                                 gpuIndex);
        REDGPU_PRINTF("context.gpus[%d].queuesCanCompute = NULL;\n",                              gpuIndex);
        REDGPU_PRINTF("context.gpus[%d].queuesCopyLimits = NULL;\n",                              gpuIndex);
        }
        for (unsigned i = 0; i < context->gpus[gpuIndex].queuesCount; i += 1) {
        REDGPU_PRINTF("context.gpus[%d].queues[%d] = %p;\n",                                      gpuIndex, i, REDGPU_PRINT_POINTER(context->gpus[gpuIndex].queues[i]));
        }
        REDGPU_PRINTF("context.gpus[%d].queuesDescription = NULL;\n",                             gpuIndex);
        for (unsigned i = 0; i < context->gpus[gpuIndex].queuesCount; i += 1) {
        REDGPU_PRINTF("context.gpus[%d].queuesFamilyIndex[%d] = %d;\n",                           gpuIndex, i, context->gpus[gpuIndex].queuesFamilyIndex[i]);
        }
        for (unsigned i = 0; i < context->gpus[gpuIndex].queuesCount; i += 1) {
        REDGPU_PRINTF("context.gpus[%d].queuesCanCopy[%d] = %d;\n",                               gpuIndex, i, context->gpus[gpuIndex].queuesCanCopy[i]);
        }
        for (unsigned i = 0; i < context->gpus[gpuIndex].queuesCount; i += 1) {
        REDGPU_PRINTF("context.gpus[%d].queuesCanDraw[%d] = %d;\n",                               gpuIndex, i, context->gpus[gpuIndex].queuesCanDraw[i]);
        }
        for (unsigned i = 0; i < context->gpus[gpuIndex].queuesCount; i += 1) {
        REDGPU_PRINTF("context.gpus[%d].queuesCanCompute[%d] = %d;\n",                            gpuIndex, i, context->gpus[gpuIndex].queuesCanCompute[i]);
        }
        for (unsigned i = 0; i < context->gpus[gpuIndex].queuesCount; i += 1) {
        REDGPU_PRINTF("context.gpus[%d].queuesCopyLimits[%d].copyBlockTexelsCountWidth = %d;\n",  gpuIndex, i, context->gpus[gpuIndex].queuesCopyLimits[i].copyBlockTexelsCountWidth);
        REDGPU_PRINTF("context.gpus[%d].queuesCopyLimits[%d].copyBlockTexelsCountHeight = %d;\n", gpuIndex, i, context->gpus[gpuIndex].queuesCopyLimits[i].copyBlockTexelsCountHeight);
        REDGPU_PRINTF("context.gpus[%d].queuesCopyLimits[%d].copyBlockTexelsCountDepth = %d;\n",  gpuIndex, i, context->gpus[gpuIndex].queuesCopyLimits[i].copyBlockTexelsCountDepth);
        }
        REDGPU_PRINTF("context.gpus[%d].maxMemoryAllocateCount = %d;\n",                          gpuIndex, context->gpus[gpuIndex].maxMemoryAllocateCount);
        REDGPU_PRINTF("context.gpus[%d].minMemoryAllocateBytesAlignment = %zu;\n",                gpuIndex, (uint64_t)context->gpus[gpuIndex].minMemoryAllocateBytesAlignment);
        REDGPU_PRINTF("context.gpus[%d].maxMemoryAllocateBytesCount = %zu;\n",                    gpuIndex, (uint64_t)context->gpus[gpuIndex].maxMemoryAllocateBytesCount);
        REDGPU_PRINTF("context.gpus[%d].minMemoryNonCoherentBlockBytesCount = %zu;\n",            gpuIndex, (uint64_t)context->gpus[gpuIndex].minMemoryNonCoherentBlockBytesCount);
        REDGPU_PRINTF("context.gpus[%d].maxCreateSamplerCount = %d;\n",                           gpuIndex, context->gpus[gpuIndex].maxCreateSamplerCount);
        REDGPU_PRINTF("context.gpus[%d].minMemoryPageSeparationArrayImageBytesCount = %zu;\n",    gpuIndex, (uint64_t)context->gpus[gpuIndex].minMemoryPageSeparationArrayImageBytesCount);
        REDGPU_PRINTF("context.gpus[%d].minArrayROCStructMemberRangeBytesAlignment = %zu;\n",     gpuIndex, (uint64_t)context->gpus[gpuIndex].minArrayROCStructMemberRangeBytesAlignment);
        REDGPU_PRINTF("context.gpus[%d].maxArrayROCStructMemberRangeBytesCount = %zu;\n",         gpuIndex, (uint64_t)context->gpus[gpuIndex].maxArrayROCStructMemberRangeBytesCount);
        REDGPU_PRINTF("context.gpus[%d].minArrayRORWStructMemberRangeBytesAlignment = %zu;\n",    gpuIndex, (uint64_t)context->gpus[gpuIndex].minArrayRORWStructMemberRangeBytesAlignment);
        REDGPU_PRINTF("context.gpus[%d].maxArrayRORWStructMemberRangeBytesCount = %zu;\n",        gpuIndex, (uint64_t)context->gpus[gpuIndex].maxArrayRORWStructMemberRangeBytesCount);
        REDGPU_PRINTF("context.gpus[%d].maxArrayIndexUint32Value = %d;\n",                        gpuIndex, context->gpus[gpuIndex].maxArrayIndexUint32Value);
        REDGPU_PRINTF("context.gpus[%d].maxImageDimensions1D = %d;\n",                            gpuIndex, context->gpus[gpuIndex].maxImageDimensions1D);
        REDGPU_PRINTF("context.gpus[%d].maxImageDimensions2D = %d;\n",                            gpuIndex, context->gpus[gpuIndex].maxImageDimensions2D);
        REDGPU_PRINTF("context.gpus[%d].maxImageDimensions3D = %d;\n",                            gpuIndex, context->gpus[gpuIndex].maxImageDimensions3D);
        REDGPU_PRINTF("context.gpus[%d].maxImageDimensionsCube = %d;\n",                          gpuIndex, context->gpus[gpuIndex].maxImageDimensionsCube);
        REDGPU_PRINTF("context.gpus[%d].maxImageLayersCount = %d;\n",                             gpuIndex, context->gpus[gpuIndex].maxImageLayersCount);
        REDGPU_PRINTF("context.gpus[%d].maxSamplerMipLodBias = %.9g;\n",                          gpuIndex, context->gpus[gpuIndex].maxSamplerMipLodBias);
        REDGPU_PRINTF("context.gpus[%d].maxSamplerAnisotropy = %.9g;\n",                          gpuIndex, context->gpus[gpuIndex].maxSamplerAnisotropy);
        REDGPU_PRINTF("context.gpus[%d].precisionBitsSamplerFilteringMagMin = %d;\n",             gpuIndex, context->gpus[gpuIndex].precisionBitsSamplerFilteringMagMin);
        REDGPU_PRINTF("context.gpus[%d].precisionBitsSamplerFilteringMip = %d;\n",                gpuIndex, context->gpus[gpuIndex].precisionBitsSamplerFilteringMip);
        REDGPU_PRINTF("context.gpus[%d].maxOutputWidth = %d;\n",                                  gpuIndex, context->gpus[gpuIndex].maxOutputWidth);
        REDGPU_PRINTF("context.gpus[%d].maxOutputHeight = %d;\n",                                 gpuIndex, context->gpus[gpuIndex].maxOutputHeight);
        REDGPU_PRINTF("context.gpus[%d].maxOutputColorsCount = %d;\n",                            gpuIndex, context->gpus[gpuIndex].maxOutputColorsCount);
        REDGPU_PRINTF("context.gpus[%d].supportedMultisampleCountsForOutputDepth = %s | %s | %s | %s | %s;\n",   gpuIndex, REDGPU_PRINT_MULTISAMPLE_COUNT_BITFLAGS(context->gpus[gpuIndex].supportedMultisampleCountsForOutputDepth));
        REDGPU_PRINTF("context.gpus[%d].supportedMultisampleCountsForOutputStencil = %s | %s | %s | %s | %s;\n", gpuIndex, REDGPU_PRINT_MULTISAMPLE_COUNT_BITFLAGS(context->gpus[gpuIndex].supportedMultisampleCountsForOutputStencil));
        REDGPU_PRINTF("context.gpus[%d].supportedMultisampleCountsForOutputColor = %s | %s | %s | %s | %s;\n",   gpuIndex, REDGPU_PRINT_MULTISAMPLE_COUNT_BITFLAGS(context->gpus[gpuIndex].supportedMultisampleCountsForOutputColor));
        REDGPU_PRINTF("context.gpus[%d].supportedMultisampleCountsForEmptyOutput = %s | %s | %s | %s | %s;\n",   gpuIndex, REDGPU_PRINT_MULTISAMPLE_COUNT_BITFLAGS(context->gpus[gpuIndex].supportedMultisampleCountsForEmptyOutput));
        REDGPU_PRINTF("context.gpus[%d].maxStructSlotsCount = %d;\n",                             gpuIndex, context->gpus[gpuIndex].maxStructSlotsCount);
        REDGPU_PRINTF("context.gpus[%d].maxStructPerStageArrayROCsCount = %d;\n",                 gpuIndex, context->gpus[gpuIndex].maxStructPerStageArrayROCsCount);
        REDGPU_PRINTF("context.gpus[%d].maxStructPerStageArrayRORWsCount = %d;\n",                gpuIndex, context->gpus[gpuIndex].maxStructPerStageArrayRORWsCount);
        REDGPU_PRINTF("context.gpus[%d].maxStructPerStageSamplersCount = %d;\n",                  gpuIndex, context->gpus[gpuIndex].maxStructPerStageSamplersCount);
        REDGPU_PRINTF("context.gpus[%d].maxStructPerStageTextureROsCount = %d;\n",                gpuIndex, context->gpus[gpuIndex].maxStructPerStageTextureROsCount);
        REDGPU_PRINTF("context.gpus[%d].maxStructPerStageTextureRWsCount = %d;\n",                gpuIndex, context->gpus[gpuIndex].maxStructPerStageTextureRWsCount);
        REDGPU_PRINTF("context.gpus[%d].maxStructPerStageAllResourcesCount = %d;\n",              gpuIndex, context->gpus[gpuIndex].maxStructPerStageAllResourcesCount);
        REDGPU_PRINTF("context.gpus[%d].maxStructArrayROCsCount = %d;\n",                         gpuIndex, context->gpus[gpuIndex].maxStructArrayROCsCount);
        REDGPU_PRINTF("context.gpus[%d].maxStructArrayRORWsCount = %d;\n",                        gpuIndex, context->gpus[gpuIndex].maxStructArrayRORWsCount);
        REDGPU_PRINTF("context.gpus[%d].maxStructSamplersCount = %d;\n",                          gpuIndex, context->gpus[gpuIndex].maxStructSamplersCount);
        REDGPU_PRINTF("context.gpus[%d].maxStructTextureROsCount = %d;\n",                        gpuIndex, context->gpus[gpuIndex].maxStructTextureROsCount);
        REDGPU_PRINTF("context.gpus[%d].maxStructTextureRWsCount = %d;\n",                        gpuIndex, context->gpus[gpuIndex].maxStructTextureRWsCount);
        REDGPU_PRINTF("context.gpus[%d].maxProcedureParametersVariablesBytesCount = %d;\n",       gpuIndex, context->gpus[gpuIndex].maxProcedureParametersVariablesBytesCount);
        REDGPU_PRINTF("context.gpus[%d].maxViewportDimensions[0] = %d;\n",                        gpuIndex, context->gpus[gpuIndex].maxViewportDimensions[0]);
        REDGPU_PRINTF("context.gpus[%d].maxViewportDimensions[1] = %d;\n",                        gpuIndex, context->gpus[gpuIndex].maxViewportDimensions[1]);
        REDGPU_PRINTF("context.gpus[%d].minViewportBoundsRange = %.9g;\n",                        gpuIndex, context->gpus[gpuIndex].minViewportBoundsRange);
        REDGPU_PRINTF("context.gpus[%d].maxViewportBoundsRange = %.9g;\n",                        gpuIndex, context->gpus[gpuIndex].maxViewportBoundsRange);
        REDGPU_PRINTF("context.gpus[%d].precisionBitsViewportCoordinates = %d;\n",                gpuIndex, context->gpus[gpuIndex].precisionBitsViewportCoordinates);
        REDGPU_PRINTF("context.gpus[%d].precisionBitsViewportBounds = %d;\n",                     gpuIndex, context->gpus[gpuIndex].precisionBitsViewportBounds);
        REDGPU_PRINTF("context.gpus[%d].maxPerStageClipDistances = %d;\n",                        gpuIndex, context->gpus[gpuIndex].maxPerStageClipDistances);
        REDGPU_PRINTF("context.gpus[%d].maxPerStageCullDistances = %d;\n",                        gpuIndex, context->gpus[gpuIndex].maxPerStageCullDistances);
        REDGPU_PRINTF("context.gpus[%d].maxPerStageClipAndCullDistances = %d;\n",                 gpuIndex, context->gpus[gpuIndex].maxPerStageClipAndCullDistances);
        REDGPU_PRINTF("context.gpus[%d].maxVertexOutputLocationsCount = %d;\n",                   gpuIndex, context->gpus[gpuIndex].maxVertexOutputLocationsCount);
        REDGPU_PRINTF("context.gpus[%d].maxFragmentInputLocationsCount = %d;\n",                  gpuIndex, context->gpus[gpuIndex].maxFragmentInputLocationsCount);
        REDGPU_PRINTF("context.gpus[%d].maxFragmentOutputColorsCount = %d;\n",                    gpuIndex, context->gpus[gpuIndex].maxFragmentOutputColorsCount);
        REDGPU_PRINTF("context.gpus[%d].maxFragmentOutputColorsCountBlendDualSource = %d;\n",     gpuIndex, context->gpus[gpuIndex].maxFragmentOutputColorsCountBlendDualSource);
        REDGPU_PRINTF("context.gpus[%d].maxFragmentOutputResourcesCount = %d;\n",                 gpuIndex, context->gpus[gpuIndex].maxFragmentOutputResourcesCount);
        REDGPU_PRINTF("context.gpus[%d].maxComputeSharedMemoryBytesCount = %d;\n",                gpuIndex, context->gpus[gpuIndex].maxComputeSharedMemoryBytesCount);
        REDGPU_PRINTF("context.gpus[%d].maxComputeWorkgroupsCount[0] = %d;\n",                    gpuIndex, context->gpus[gpuIndex].maxComputeWorkgroupsCount[0]);
        REDGPU_PRINTF("context.gpus[%d].maxComputeWorkgroupsCount[1] = %d;\n",                    gpuIndex, context->gpus[gpuIndex].maxComputeWorkgroupsCount[1]);
        REDGPU_PRINTF("context.gpus[%d].maxComputeWorkgroupsCount[2] = %d;\n",                    gpuIndex, context->gpus[gpuIndex].maxComputeWorkgroupsCount[2]);
        REDGPU_PRINTF("context.gpus[%d].maxComputeWorkgroupInvocationsCount = %d;\n",             gpuIndex, context->gpus[gpuIndex].maxComputeWorkgroupInvocationsCount);
        REDGPU_PRINTF("context.gpus[%d].maxComputeWorkgroupDimensions[0] = %d;\n",                gpuIndex, context->gpus[gpuIndex].maxComputeWorkgroupDimensions[0]);
        REDGPU_PRINTF("context.gpus[%d].maxComputeWorkgroupDimensions[1] = %d;\n",                gpuIndex, context->gpus[gpuIndex].maxComputeWorkgroupDimensions[1]);
        REDGPU_PRINTF("context.gpus[%d].maxComputeWorkgroupDimensions[2] = %d;\n",                gpuIndex, context->gpus[gpuIndex].maxComputeWorkgroupDimensions[2]);
        REDGPU_PRINTF("context.gpus[%d].minImageSampleImageFetchOffset = %d;\n",                  gpuIndex, context->gpus[gpuIndex].minImageSampleImageFetchOffset);
        REDGPU_PRINTF("context.gpus[%d].maxImageSampleImageFetchOffset = %d;\n",                  gpuIndex, context->gpus[gpuIndex].maxImageSampleImageFetchOffset);
        REDGPU_PRINTF("context.gpus[%d].minImageGatherOffset = %d;\n",                            gpuIndex, context->gpus[gpuIndex].minImageGatherOffset);
        REDGPU_PRINTF("context.gpus[%d].maxImageGatherOffset = %d;\n",                            gpuIndex, context->gpus[gpuIndex].maxImageGatherOffset);
        REDGPU_PRINTF("context.gpus[%d].minInterpolateAtOffset = %.9g;\n",                        gpuIndex, context->gpus[gpuIndex].minInterpolateAtOffset);
        REDGPU_PRINTF("context.gpus[%d].maxInterpolateAtOffset = %.9g;\n",                        gpuIndex, context->gpus[gpuIndex].maxInterpolateAtOffset);
        REDGPU_PRINTF("context.gpus[%d].precisionBitsInterpolateAtOffset = %d;\n",                gpuIndex, context->gpus[gpuIndex].precisionBitsInterpolateAtOffset);
        REDGPU_PRINTF("context.gpus[%d].optimalCopyArrayImageRangeArrayBytesFirstBytesAlignment = %zu;\n",           gpuIndex, (uint64_t)context->gpus[gpuIndex].optimalCopyArrayImageRangeArrayBytesFirstBytesAlignment);
        REDGPU_PRINTF("context.gpus[%d].optimalCopyArrayImageRangeArrayTexelsCountToNextRowBytesAlignment = %zu;\n", gpuIndex, (uint64_t)context->gpus[gpuIndex].optimalCopyArrayImageRangeArrayTexelsCountToNextRowBytesAlignment);
        REDGPU_PRINTF("context.gpus[%d].supportsWsi = %d;\n",                                                        gpuIndex, context->gpus[gpuIndex].supportsWsi);
        REDGPU_PRINTF("context.gpus[%d].supportsMemoryGetBudget = %d;\n",                                            gpuIndex, context->gpus[gpuIndex].supportsMemoryGetBudget);
        REDGPU_PRINTF("context.gpus[%d].supportsFullArrayIndexUint32Value = %d;\n",                                  gpuIndex, context->gpus[gpuIndex].supportsFullArrayIndexUint32Value);
        REDGPU_PRINTF("context.gpus[%d].supportsSamplerAnisotropy = %d;\n",                                          gpuIndex, context->gpus[gpuIndex].supportsSamplerAnisotropy);
        REDGPU_PRINTF("context.gpus[%d].supportsTextureDimensionsCubeLayered = %d;\n",                               gpuIndex, context->gpus[gpuIndex].supportsTextureDimensionsCubeLayered);
        REDGPU_PRINTF("context.gpus[%d].supportsProcedureStateRasterizationDepthClamp = %d;\n",                      gpuIndex, context->gpus[gpuIndex].supportsProcedureStateRasterizationDepthClamp);
        REDGPU_PRINTF("context.gpus[%d].supportsProcedureStateRasterizationDepthBiasDynamic = %d;\n",                gpuIndex, context->gpus[gpuIndex].supportsProcedureStateRasterizationDepthBiasDynamic);
        REDGPU_PRINTF("context.gpus[%d].supportsProcedureStateRasterizationDepthBiasClamp = %d;\n",                  gpuIndex, context->gpus[gpuIndex].supportsProcedureStateRasterizationDepthBiasClamp);
        REDGPU_PRINTF("context.gpus[%d].supportsProcedureStateMultisampleSampleShading = %d;\n",                     gpuIndex, context->gpus[gpuIndex].supportsProcedureStateMultisampleSampleShading);
        REDGPU_PRINTF("context.gpus[%d].supportsProcedureStateMultisampleAlphaToOne = %d;\n",                        gpuIndex, context->gpus[gpuIndex].supportsProcedureStateMultisampleAlphaToOne);
        REDGPU_PRINTF("context.gpus[%d].supportsProcedureStateDepthTestBoundsTest = %d;\n",                          gpuIndex, context->gpus[gpuIndex].supportsProcedureStateDepthTestBoundsTest);
        REDGPU_PRINTF("context.gpus[%d].supportsProcedureStateDepthTestBoundsTestDynamic = %d;\n",                   gpuIndex, context->gpus[gpuIndex].supportsProcedureStateDepthTestBoundsTestDynamic);
        REDGPU_PRINTF("context.gpus[%d].supportsProcedureStateStencilTestFrontAndBackDynamicCompareMask = %d;\n",    gpuIndex, context->gpus[gpuIndex].supportsProcedureStateStencilTestFrontAndBackDynamicCompareMask);
        REDGPU_PRINTF("context.gpus[%d].supportsProcedureStateStencilTestFrontAndBackDynamicWriteMask = %d;\n",      gpuIndex, context->gpus[gpuIndex].supportsProcedureStateStencilTestFrontAndBackDynamicWriteMask);
        REDGPU_PRINTF("context.gpus[%d].supportsProcedureStateBlendLogicOp = %d;\n",                                 gpuIndex, context->gpus[gpuIndex].supportsProcedureStateBlendLogicOp);
        REDGPU_PRINTF("context.gpus[%d].supportsProcedureStateOutputColorsBlendVaryingPerColor = %d;\n",             gpuIndex, context->gpus[gpuIndex].supportsProcedureStateOutputColorsBlendVaryingPerColor);
        REDGPU_PRINTF("context.gpus[%d].supportsProcedureStateOutputColorsBlendDualSource = %d;\n",                  gpuIndex, context->gpus[gpuIndex].supportsProcedureStateOutputColorsBlendDualSource);
        REDGPU_PRINTF("context.gpus[%d].supportsMultisampleEmptyOutputVariableMultisampleCount = %d;\n",             gpuIndex, context->gpus[gpuIndex].supportsMultisampleEmptyOutputVariableMultisampleCount);
        REDGPU_PRINTF("context.gpus[%d].supportsMultisampleStandardSampleLocations = %d;\n",                         gpuIndex, context->gpus[gpuIndex].supportsMultisampleStandardSampleLocations);
        REDGPU_PRINTF("context.gpus[%d].supportsGpuCodeWritesAndAtomicsInStageVertex = %d;\n",                       gpuIndex, context->gpus[gpuIndex].supportsGpuCodeWritesAndAtomicsInStageVertex);
        REDGPU_PRINTF("context.gpus[%d].supportsGpuCodeWritesAndAtomicsInStageFragment = %d;\n",                     gpuIndex, context->gpus[gpuIndex].supportsGpuCodeWritesAndAtomicsInStageFragment);
        REDGPU_PRINTF("context.gpus[%d].supportsGpuCodeImageGatherExtended = %d;\n",                                 gpuIndex, context->gpus[gpuIndex].supportsGpuCodeImageGatherExtended);
        REDGPU_PRINTF("context.gpus[%d].supportsGpuCodeTextureRWExtendedFormats = %d;\n",                            gpuIndex, context->gpus[gpuIndex].supportsGpuCodeTextureRWExtendedFormats);
        REDGPU_PRINTF("context.gpus[%d].supportsGpuCodeTextureRWMultisample = %d;\n",                                gpuIndex, context->gpus[gpuIndex].supportsGpuCodeTextureRWMultisample);
        REDGPU_PRINTF("context.gpus[%d].supportsGpuCodeTextureRWReadWithoutFormat = %d;\n",                          gpuIndex, context->gpus[gpuIndex].supportsGpuCodeTextureRWReadWithoutFormat);
        REDGPU_PRINTF("context.gpus[%d].supportsGpuCodeTextureRWWriteWithoutFormat = %d;\n",                         gpuIndex, context->gpus[gpuIndex].supportsGpuCodeTextureRWWriteWithoutFormat);
        REDGPU_PRINTF("context.gpus[%d].supportsGpuCodeDynamicallyIndexableArraysOfArrayROCs = %d;\n",               gpuIndex, context->gpus[gpuIndex].supportsGpuCodeDynamicallyIndexableArraysOfArrayROCs);
        REDGPU_PRINTF("context.gpus[%d].supportsGpuCodeDynamicallyIndexableArraysOfArrayRORWs = %d;\n",              gpuIndex, context->gpus[gpuIndex].supportsGpuCodeDynamicallyIndexableArraysOfArrayRORWs);
        REDGPU_PRINTF("context.gpus[%d].supportsGpuCodeDynamicallyIndexableArraysOfSamplers = %d;\n",                gpuIndex, context->gpus[gpuIndex].supportsGpuCodeDynamicallyIndexableArraysOfSamplers);
        REDGPU_PRINTF("context.gpus[%d].supportsGpuCodeDynamicallyIndexableArraysOfTextureROs = %d;\n",              gpuIndex, context->gpus[gpuIndex].supportsGpuCodeDynamicallyIndexableArraysOfTextureROs);
        REDGPU_PRINTF("context.gpus[%d].supportsGpuCodeDynamicallyIndexableArraysOfTextureRWs = %d;\n",              gpuIndex, context->gpus[gpuIndex].supportsGpuCodeDynamicallyIndexableArraysOfTextureRWs);
        REDGPU_PRINTF("context.gpus[%d].supportsGpuCodeClipDistance = %d;\n",                                        gpuIndex, context->gpus[gpuIndex].supportsGpuCodeClipDistance);
        REDGPU_PRINTF("context.gpus[%d].supportsGpuCodeCullDistance = %d;\n",                                        gpuIndex, context->gpus[gpuIndex].supportsGpuCodeCullDistance);
        REDGPU_PRINTF("context.gpus[%d].supportsGpuCodeInt64 = %d;\n",                                               gpuIndex, context->gpus[gpuIndex].supportsGpuCodeInt64);
        REDGPU_PRINTF("context.gpus[%d].supportsGpuCodeFloat64 = %d;\n",                                             gpuIndex, context->gpus[gpuIndex].supportsGpuCodeFloat64);
        REDGPU_PRINTF("context.gpus[%d].supportsGpuCodeMinLod = %d;\n",                                              gpuIndex, context->gpus[gpuIndex].supportsGpuCodeMinLod);
        if (context->gpus[gpuIndex].imageFormatsLimitsImageDimensions1D != NULL) {
        for (unsigned i = 0; i < 131; i += 1) {
        if (redDebugLibraryInternalFormatIsValid(i) == 0) { continue; }
        REDGPU_PRINTF("context.gpus[%d].imageFormatsLimitsImageDimensions1D[%s].maxDimensions.maxWidth = %d;\n",                         gpuIndex, REDGPU_PRINT_FORMAT((RedFormat)i), context->gpus[gpuIndex].imageFormatsLimitsImageDimensions1D[i].maxDimensions.maxWidth);
        REDGPU_PRINTF("context.gpus[%d].imageFormatsLimitsImageDimensions1D[%s].maxDimensions.maxHeight = %d;\n",                        gpuIndex, REDGPU_PRINT_FORMAT((RedFormat)i), context->gpus[gpuIndex].imageFormatsLimitsImageDimensions1D[i].maxDimensions.maxHeight);
        REDGPU_PRINTF("context.gpus[%d].imageFormatsLimitsImageDimensions1D[%s].maxDimensions.maxDepth = %d;\n",                         gpuIndex, REDGPU_PRINT_FORMAT((RedFormat)i), context->gpus[gpuIndex].imageFormatsLimitsImageDimensions1D[i].maxDimensions.maxDepth);
        REDGPU_PRINTF("context.gpus[%d].imageFormatsLimitsImageDimensions1D[%s].maxLevelsCount = %d;\n",                                 gpuIndex, REDGPU_PRINT_FORMAT((RedFormat)i), context->gpus[gpuIndex].imageFormatsLimitsImageDimensions1D[i].maxLevelsCount);
        REDGPU_PRINTF("context.gpus[%d].imageFormatsLimitsImageDimensions1D[%s].maxLayersCount = %d;\n",                                 gpuIndex, REDGPU_PRINT_FORMAT((RedFormat)i), context->gpus[gpuIndex].imageFormatsLimitsImageDimensions1D[i].maxLayersCount);
        REDGPU_PRINTF("context.gpus[%d].imageFormatsLimitsImageDimensions1D[%s].supportedMultisampleCounts = %s | %s | %s | %s | %s;\n", gpuIndex, REDGPU_PRINT_FORMAT((RedFormat)i), REDGPU_PRINT_MULTISAMPLE_COUNT_BITFLAGS(context->gpus[gpuIndex].imageFormatsLimitsImageDimensions1D[i].supportedMultisampleCounts));
        REDGPU_PRINTF("context.gpus[%d].imageFormatsLimitsImageDimensions1D[%s].maxBytesCount = %zu;\n",                                 gpuIndex, REDGPU_PRINT_FORMAT((RedFormat)i), (uint64_t)context->gpus[gpuIndex].imageFormatsLimitsImageDimensions1D[i].maxBytesCount);
        REDGPU_PRINTF("context.gpus[%d].imageFormatsLimitsImageDimensions1D[%s].status = %s;\n",                                         gpuIndex, REDGPU_PRINT_FORMAT((RedFormat)i), REDGPU_PRINT_STATUS(context->gpus[gpuIndex].imageFormatsLimitsImageDimensions1D[i].status));
        }
        } else {
        REDGPU_PRINTF("context.gpus[%d].imageFormatsLimitsImageDimensions1D = NULL;\n",                                                  gpuIndex);
        }
        if (context->gpus[gpuIndex].imageFormatsLimitsImageDimensions2D != NULL) {
        for (unsigned i = 0; i < 131; i += 1) {
        if (redDebugLibraryInternalFormatIsValid(i) == 0) { continue; }
        REDGPU_PRINTF("context.gpus[%d].imageFormatsLimitsImageDimensions2D[%s].maxDimensions.maxWidth = %d;\n",                         gpuIndex, REDGPU_PRINT_FORMAT((RedFormat)i), context->gpus[gpuIndex].imageFormatsLimitsImageDimensions2D[i].maxDimensions.maxWidth);
        REDGPU_PRINTF("context.gpus[%d].imageFormatsLimitsImageDimensions2D[%s].maxDimensions.maxHeight = %d;\n",                        gpuIndex, REDGPU_PRINT_FORMAT((RedFormat)i), context->gpus[gpuIndex].imageFormatsLimitsImageDimensions2D[i].maxDimensions.maxHeight);
        REDGPU_PRINTF("context.gpus[%d].imageFormatsLimitsImageDimensions2D[%s].maxDimensions.maxDepth = %d;\n",                         gpuIndex, REDGPU_PRINT_FORMAT((RedFormat)i), context->gpus[gpuIndex].imageFormatsLimitsImageDimensions2D[i].maxDimensions.maxDepth);
        REDGPU_PRINTF("context.gpus[%d].imageFormatsLimitsImageDimensions2D[%s].maxLevelsCount = %d;\n",                                 gpuIndex, REDGPU_PRINT_FORMAT((RedFormat)i), context->gpus[gpuIndex].imageFormatsLimitsImageDimensions2D[i].maxLevelsCount);
        REDGPU_PRINTF("context.gpus[%d].imageFormatsLimitsImageDimensions2D[%s].maxLayersCount = %d;\n",                                 gpuIndex, REDGPU_PRINT_FORMAT((RedFormat)i), context->gpus[gpuIndex].imageFormatsLimitsImageDimensions2D[i].maxLayersCount);
        REDGPU_PRINTF("context.gpus[%d].imageFormatsLimitsImageDimensions2D[%s].supportedMultisampleCounts = %s | %s | %s | %s | %s;\n", gpuIndex, REDGPU_PRINT_FORMAT((RedFormat)i), REDGPU_PRINT_MULTISAMPLE_COUNT_BITFLAGS(context->gpus[gpuIndex].imageFormatsLimitsImageDimensions2D[i].supportedMultisampleCounts));
        REDGPU_PRINTF("context.gpus[%d].imageFormatsLimitsImageDimensions2D[%s].maxBytesCount = %zu;\n",                                 gpuIndex, REDGPU_PRINT_FORMAT((RedFormat)i), (uint64_t)context->gpus[gpuIndex].imageFormatsLimitsImageDimensions2D[i].maxBytesCount);
        REDGPU_PRINTF("context.gpus[%d].imageFormatsLimitsImageDimensions2D[%s].status = %s;\n",                                         gpuIndex, REDGPU_PRINT_FORMAT((RedFormat)i), REDGPU_PRINT_STATUS(context->gpus[gpuIndex].imageFormatsLimitsImageDimensions2D[i].status));
        }
        } else {
        REDGPU_PRINTF("context.gpus[%d].imageFormatsLimitsImageDimensions2D = NULL;\n",                                                  gpuIndex);
        }
        if (context->gpus[gpuIndex].imageFormatsLimitsImageDimensions2DMultisample != NULL) {
        for (unsigned i = 0; i < 131; i += 1) {
        if (redDebugLibraryInternalFormatIsValid(i) == 0) { continue; }
        REDGPU_PRINTF("context.gpus[%d].imageFormatsLimitsImageDimensions2DMultisample[%s].maxDimensions.maxWidth = %d;\n",                         gpuIndex, REDGPU_PRINT_FORMAT((RedFormat)i), context->gpus[gpuIndex].imageFormatsLimitsImageDimensions2DMultisample[i].maxDimensions.maxWidth);
        REDGPU_PRINTF("context.gpus[%d].imageFormatsLimitsImageDimensions2DMultisample[%s].maxDimensions.maxHeight = %d;\n",                        gpuIndex, REDGPU_PRINT_FORMAT((RedFormat)i), context->gpus[gpuIndex].imageFormatsLimitsImageDimensions2DMultisample[i].maxDimensions.maxHeight);
        REDGPU_PRINTF("context.gpus[%d].imageFormatsLimitsImageDimensions2DMultisample[%s].maxDimensions.maxDepth = %d;\n",                         gpuIndex, REDGPU_PRINT_FORMAT((RedFormat)i), context->gpus[gpuIndex].imageFormatsLimitsImageDimensions2DMultisample[i].maxDimensions.maxDepth);
        REDGPU_PRINTF("context.gpus[%d].imageFormatsLimitsImageDimensions2DMultisample[%s].maxLevelsCount = %d;\n",                                 gpuIndex, REDGPU_PRINT_FORMAT((RedFormat)i), context->gpus[gpuIndex].imageFormatsLimitsImageDimensions2DMultisample[i].maxLevelsCount);
        REDGPU_PRINTF("context.gpus[%d].imageFormatsLimitsImageDimensions2DMultisample[%s].maxLayersCount = %d;\n",                                 gpuIndex, REDGPU_PRINT_FORMAT((RedFormat)i), context->gpus[gpuIndex].imageFormatsLimitsImageDimensions2DMultisample[i].maxLayersCount);
        REDGPU_PRINTF("context.gpus[%d].imageFormatsLimitsImageDimensions2DMultisample[%s].supportedMultisampleCounts = %s | %s | %s | %s | %s;\n", gpuIndex, REDGPU_PRINT_FORMAT((RedFormat)i), REDGPU_PRINT_MULTISAMPLE_COUNT_BITFLAGS(context->gpus[gpuIndex].imageFormatsLimitsImageDimensions2DMultisample[i].supportedMultisampleCounts));
        REDGPU_PRINTF("context.gpus[%d].imageFormatsLimitsImageDimensions2DMultisample[%s].maxBytesCount = %zu;\n",                                 gpuIndex, REDGPU_PRINT_FORMAT((RedFormat)i), (uint64_t)context->gpus[gpuIndex].imageFormatsLimitsImageDimensions2DMultisample[i].maxBytesCount);
        REDGPU_PRINTF("context.gpus[%d].imageFormatsLimitsImageDimensions2DMultisample[%s].status = %s;\n",                                         gpuIndex, REDGPU_PRINT_FORMAT((RedFormat)i), REDGPU_PRINT_STATUS(context->gpus[gpuIndex].imageFormatsLimitsImageDimensions2DMultisample[i].status));
        }
        } else {
        REDGPU_PRINTF("context.gpus[%d].imageFormatsLimitsImageDimensions2DMultisample = NULL;\n",                                                  gpuIndex);
        }
        if (context->gpus[gpuIndex].imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered != NULL) {
        for (unsigned i = 0; i < 131; i += 1) {
        if (redDebugLibraryInternalFormatIsValid(i) == 0) { continue; }
        REDGPU_PRINTF("context.gpus[%d].imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[%s].maxDimensions.maxWidth = %d;\n",                         gpuIndex, REDGPU_PRINT_FORMAT((RedFormat)i), context->gpus[gpuIndex].imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[i].maxDimensions.maxWidth);
        REDGPU_PRINTF("context.gpus[%d].imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[%s].maxDimensions.maxHeight = %d;\n",                        gpuIndex, REDGPU_PRINT_FORMAT((RedFormat)i), context->gpus[gpuIndex].imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[i].maxDimensions.maxHeight);
        REDGPU_PRINTF("context.gpus[%d].imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[%s].maxDimensions.maxDepth = %d;\n",                         gpuIndex, REDGPU_PRINT_FORMAT((RedFormat)i), context->gpus[gpuIndex].imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[i].maxDimensions.maxDepth);
        REDGPU_PRINTF("context.gpus[%d].imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[%s].maxLevelsCount = %d;\n",                                 gpuIndex, REDGPU_PRINT_FORMAT((RedFormat)i), context->gpus[gpuIndex].imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[i].maxLevelsCount);
        REDGPU_PRINTF("context.gpus[%d].imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[%s].maxLayersCount = %d;\n",                                 gpuIndex, REDGPU_PRINT_FORMAT((RedFormat)i), context->gpus[gpuIndex].imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[i].maxLayersCount);
        REDGPU_PRINTF("context.gpus[%d].imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[%s].supportedMultisampleCounts = %s | %s | %s | %s | %s;\n", gpuIndex, REDGPU_PRINT_FORMAT((RedFormat)i), REDGPU_PRINT_MULTISAMPLE_COUNT_BITFLAGS(context->gpus[gpuIndex].imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[i].supportedMultisampleCounts));
        REDGPU_PRINTF("context.gpus[%d].imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[%s].maxBytesCount = %zu;\n",                                 gpuIndex, REDGPU_PRINT_FORMAT((RedFormat)i), (uint64_t)context->gpus[gpuIndex].imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[i].maxBytesCount);
        REDGPU_PRINTF("context.gpus[%d].imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[%s].status = %s;\n",                                         gpuIndex, REDGPU_PRINT_FORMAT((RedFormat)i), REDGPU_PRINT_STATUS(context->gpus[gpuIndex].imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered[i].status));
        }
        } else {
        REDGPU_PRINTF("context.gpus[%d].imageFormatsLimitsImageDimensions2DWithTextureDimensionsCubeAndCubeLayered = NULL;\n",                                                  gpuIndex);
        }
        if (context->gpus[gpuIndex].imageFormatsLimitsImageDimensions3D != NULL) {
        for (unsigned i = 0; i < 131; i += 1) {
        if (redDebugLibraryInternalFormatIsValid(i) == 0) { continue; }
        REDGPU_PRINTF("context.gpus[%d].imageFormatsLimitsImageDimensions3D[%s].maxDimensions.maxWidth = %d;\n",                         gpuIndex, REDGPU_PRINT_FORMAT((RedFormat)i), context->gpus[gpuIndex].imageFormatsLimitsImageDimensions3D[i].maxDimensions.maxWidth);
        REDGPU_PRINTF("context.gpus[%d].imageFormatsLimitsImageDimensions3D[%s].maxDimensions.maxHeight = %d;\n",                        gpuIndex, REDGPU_PRINT_FORMAT((RedFormat)i), context->gpus[gpuIndex].imageFormatsLimitsImageDimensions3D[i].maxDimensions.maxHeight);
        REDGPU_PRINTF("context.gpus[%d].imageFormatsLimitsImageDimensions3D[%s].maxDimensions.maxDepth = %d;\n",                         gpuIndex, REDGPU_PRINT_FORMAT((RedFormat)i), context->gpus[gpuIndex].imageFormatsLimitsImageDimensions3D[i].maxDimensions.maxDepth);
        REDGPU_PRINTF("context.gpus[%d].imageFormatsLimitsImageDimensions3D[%s].maxLevelsCount = %d;\n",                                 gpuIndex, REDGPU_PRINT_FORMAT((RedFormat)i), context->gpus[gpuIndex].imageFormatsLimitsImageDimensions3D[i].maxLevelsCount);
        REDGPU_PRINTF("context.gpus[%d].imageFormatsLimitsImageDimensions3D[%s].maxLayersCount = %d;\n",                                 gpuIndex, REDGPU_PRINT_FORMAT((RedFormat)i), context->gpus[gpuIndex].imageFormatsLimitsImageDimensions3D[i].maxLayersCount);
        REDGPU_PRINTF("context.gpus[%d].imageFormatsLimitsImageDimensions3D[%s].supportedMultisampleCounts = %s | %s | %s | %s | %s;\n", gpuIndex, REDGPU_PRINT_FORMAT((RedFormat)i), REDGPU_PRINT_MULTISAMPLE_COUNT_BITFLAGS(context->gpus[gpuIndex].imageFormatsLimitsImageDimensions3D[i].supportedMultisampleCounts));
        REDGPU_PRINTF("context.gpus[%d].imageFormatsLimitsImageDimensions3D[%s].maxBytesCount = %zu;\n",                                 gpuIndex, REDGPU_PRINT_FORMAT((RedFormat)i), (uint64_t)context->gpus[gpuIndex].imageFormatsLimitsImageDimensions3D[i].maxBytesCount);
        REDGPU_PRINTF("context.gpus[%d].imageFormatsLimitsImageDimensions3D[%s].status = %s;\n",                                         gpuIndex, REDGPU_PRINT_FORMAT((RedFormat)i), REDGPU_PRINT_STATUS(context->gpus[gpuIndex].imageFormatsLimitsImageDimensions3D[i].status));
        }
        } else {
        REDGPU_PRINTF("context.gpus[%d].imageFormatsLimitsImageDimensions3D = NULL;\n",                                                  gpuIndex);
        }
        if (context->gpus[gpuIndex].imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered != NULL) {
        for (unsigned i = 0; i < 131; i += 1) {
        if (redDebugLibraryInternalFormatIsValid(i) == 0) { continue; }
        REDGPU_PRINTF("context.gpus[%d].imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[%s].maxDimensions.maxWidth = %d;\n",                         gpuIndex, REDGPU_PRINT_FORMAT((RedFormat)i), context->gpus[gpuIndex].imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[i].maxDimensions.maxWidth);
        REDGPU_PRINTF("context.gpus[%d].imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[%s].maxDimensions.maxHeight = %d;\n",                        gpuIndex, REDGPU_PRINT_FORMAT((RedFormat)i), context->gpus[gpuIndex].imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[i].maxDimensions.maxHeight);
        REDGPU_PRINTF("context.gpus[%d].imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[%s].maxDimensions.maxDepth = %d;\n",                         gpuIndex, REDGPU_PRINT_FORMAT((RedFormat)i), context->gpus[gpuIndex].imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[i].maxDimensions.maxDepth);
        REDGPU_PRINTF("context.gpus[%d].imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[%s].maxLevelsCount = %d;\n",                                 gpuIndex, REDGPU_PRINT_FORMAT((RedFormat)i), context->gpus[gpuIndex].imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[i].maxLevelsCount);
        REDGPU_PRINTF("context.gpus[%d].imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[%s].maxLayersCount = %d;\n",                                 gpuIndex, REDGPU_PRINT_FORMAT((RedFormat)i), context->gpus[gpuIndex].imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[i].maxLayersCount);
        REDGPU_PRINTF("context.gpus[%d].imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[%s].supportedMultisampleCounts = %s | %s | %s | %s | %s;\n", gpuIndex, REDGPU_PRINT_FORMAT((RedFormat)i), REDGPU_PRINT_MULTISAMPLE_COUNT_BITFLAGS(context->gpus[gpuIndex].imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[i].supportedMultisampleCounts));
        REDGPU_PRINTF("context.gpus[%d].imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[%s].maxBytesCount = %zu;\n",                                 gpuIndex, REDGPU_PRINT_FORMAT((RedFormat)i), (uint64_t)context->gpus[gpuIndex].imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[i].maxBytesCount);
        REDGPU_PRINTF("context.gpus[%d].imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[%s].status = %s;\n",                                         gpuIndex, REDGPU_PRINT_FORMAT((RedFormat)i), REDGPU_PRINT_STATUS(context->gpus[gpuIndex].imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered[i].status));
        }
        } else {
        REDGPU_PRINTF("context.gpus[%d].imageFormatsLimitsImageDimensions3DWithTextureDimensions2DAnd2DLayered = NULL;\n",                                                  gpuIndex);
        }
        if (context->gpus[gpuIndex].imageFormatsFeatures != NULL) {
        for (unsigned i = 0; i < 131; i += 1) {
        if (redDebugLibraryInternalFormatIsValid(i) == 0) { continue; }
        REDGPU_PRINTF("context.gpus[%d].imageFormatsFeatures[%s].supportsCopyR = %d;\n",                    gpuIndex, REDGPU_PRINT_FORMAT((RedFormat)i), context->gpus[gpuIndex].imageFormatsFeatures[i].supportsCopyR);
        REDGPU_PRINTF("context.gpus[%d].imageFormatsFeatures[%s].supportsCopyW = %d;\n",                    gpuIndex, REDGPU_PRINT_FORMAT((RedFormat)i), context->gpus[gpuIndex].imageFormatsFeatures[i].supportsCopyW);
        REDGPU_PRINTF("context.gpus[%d].imageFormatsFeatures[%s].supportsTextureRO = %d;\n",                gpuIndex, REDGPU_PRINT_FORMAT((RedFormat)i), context->gpus[gpuIndex].imageFormatsFeatures[i].supportsTextureRO);
        REDGPU_PRINTF("context.gpus[%d].imageFormatsFeatures[%s].supportsTextureRW = %d;\n",                gpuIndex, REDGPU_PRINT_FORMAT((RedFormat)i), context->gpus[gpuIndex].imageFormatsFeatures[i].supportsTextureRW);
        REDGPU_PRINTF("context.gpus[%d].imageFormatsFeatures[%s].supportsOutputDepthStencil = %d;\n",       gpuIndex, REDGPU_PRINT_FORMAT((RedFormat)i), context->gpus[gpuIndex].imageFormatsFeatures[i].supportsOutputDepthStencil);
        REDGPU_PRINTF("context.gpus[%d].imageFormatsFeatures[%s].supportsOutputColor = %d;\n",              gpuIndex, REDGPU_PRINT_FORMAT((RedFormat)i), context->gpus[gpuIndex].imageFormatsFeatures[i].supportsOutputColor);
        REDGPU_PRINTF("context.gpus[%d].imageFormatsFeatures[%s].supportsTextureROFilteringLinear = %d;\n", gpuIndex, REDGPU_PRINT_FORMAT((RedFormat)i), context->gpus[gpuIndex].imageFormatsFeatures[i].supportsTextureROFilteringLinear);
        REDGPU_PRINTF("context.gpus[%d].imageFormatsFeatures[%s].supportsTextureRWAtomics = %d;\n",         gpuIndex, REDGPU_PRINT_FORMAT((RedFormat)i), context->gpus[gpuIndex].imageFormatsFeatures[i].supportsTextureRWAtomics);
        REDGPU_PRINTF("context.gpus[%d].imageFormatsFeatures[%s].supportsOutputColorBlend = %d;\n",         gpuIndex, REDGPU_PRINT_FORMAT((RedFormat)i), context->gpus[gpuIndex].imageFormatsFeatures[i].supportsOutputColorBlend);
        }
        } else {
        REDGPU_PRINTF("context.gpus[%d].imageFormatsFeatures = NULL;\n",                                    gpuIndex);
        }
        const RedGpuInfoOptionalInfoIterator * optionalInfo = (const RedGpuInfoOptionalInfoIterator *)context->gpus[gpuIndex].optionalInfo;
        if (optionalInfo == NULL) {
        REDGPU_PRINTF("context.gpus[%d].optionalInfo = NULL;\n",                                            gpuIndex);
        }
        while (optionalInfo != NULL) {
          if (optionalInfo->optionalInfo == RED_GPU_INFO_OPTIONAL_INFO_DRIVER_PROPERTIES) {
            REDGPU_PRINTF("context.gpus[%d].optionalInfo.optionalInfo = RED_GPU_INFO_OPTIONAL_INFO_DRIVER_PROPERTIES;\n", gpuIndex);
            REDGPU_PRINTF("context.gpus[%d].optionalInfo.next;\n",                                                  gpuIndex);
            REDGPU_PRINTF("context.gpus[%d].optionalInfo.id = %s;\n",                                               gpuIndex, REDGPU_PRINT_DRIVER_ID(((const RedGpuInfoOptionalInfoDriverProperties *)optionalInfo)->id));
            REDGPU_PRINTF("context.gpus[%d].optionalInfo.name = \"%s\";\n",                                         gpuIndex, ((const RedGpuInfoOptionalInfoDriverProperties *)optionalInfo)->name[0] == 0 ? "" : ((const RedGpuInfoOptionalInfoDriverProperties *)optionalInfo)->name);
            REDGPU_PRINTF("context.gpus[%d].optionalInfo.info = \"%s\";\n",                                         gpuIndex, ((const RedGpuInfoOptionalInfoDriverProperties *)optionalInfo)->info[0] == 0 ? "" : ((const RedGpuInfoOptionalInfoDriverProperties *)optionalInfo)->info);
            REDGPU_PRINTF("context.gpus[%d].optionalInfo.compliantWithConformanceTestSuiteVersionMajor = %d;\n",    gpuIndex, ((const RedGpuInfoOptionalInfoDriverProperties *)optionalInfo)->compliantWithConformanceTestSuiteVersionMajor);
            REDGPU_PRINTF("context.gpus[%d].optionalInfo.compliantWithConformanceTestSuiteVersionMinor = %d;\n",    gpuIndex, ((const RedGpuInfoOptionalInfoDriverProperties *)optionalInfo)->compliantWithConformanceTestSuiteVersionMinor);
            REDGPU_PRINTF("context.gpus[%d].optionalInfo.compliantWithConformanceTestSuiteVersionSubminor = %d;\n", gpuIndex, ((const RedGpuInfoOptionalInfoDriverProperties *)optionalInfo)->compliantWithConformanceTestSuiteVersionSubminor);
            REDGPU_PRINTF("context.gpus[%d].optionalInfo.compliantWithConformanceTestSuiteVersionPatch = %d;\n",    gpuIndex, ((const RedGpuInfoOptionalInfoDriverProperties *)optionalInfo)->compliantWithConformanceTestSuiteVersionPatch);
          } else if (optionalInfo->optionalInfo == RED_GPU_INFO_OPTIONAL_INFO_RESOLVE_DEPTH_STENCIL) {
            REDGPU_PRINTF("context.gpus[%d].optionalInfo.optionalInfo = RED_GPU_INFO_OPTIONAL_INFO_RESOLVE_DEPTH_STENCIL;\n", gpuIndex);
            REDGPU_PRINTF("context.gpus[%d].optionalInfo.next;\n",                                                  gpuIndex);
            REDGPU_PRINTF("context.gpus[%d].optionalInfo.supportsResolveDepthStencil = %d;\n",                      gpuIndex, ((const RedGpuInfoOptionalInfoResolveDepthStencil *)optionalInfo)->supportsResolveDepthStencil);
            REDGPU_PRINTF("context.gpus[%d].optionalInfo.supportsResolveModeDepthSampleIndexZero = %d;\n",          gpuIndex, ((const RedGpuInfoOptionalInfoResolveDepthStencil *)optionalInfo)->supportsResolveModeDepthSampleIndexZero);
            REDGPU_PRINTF("context.gpus[%d].optionalInfo.supportsResolveModeDepthAverage = %d;\n",                  gpuIndex, ((const RedGpuInfoOptionalInfoResolveDepthStencil *)optionalInfo)->supportsResolveModeDepthAverage);
            REDGPU_PRINTF("context.gpus[%d].optionalInfo.supportsResolveModeDepthMin = %d;\n",                      gpuIndex, ((const RedGpuInfoOptionalInfoResolveDepthStencil *)optionalInfo)->supportsResolveModeDepthMin);
            REDGPU_PRINTF("context.gpus[%d].optionalInfo.supportsResolveModeDepthMax = %d;\n",                      gpuIndex, ((const RedGpuInfoOptionalInfoResolveDepthStencil *)optionalInfo)->supportsResolveModeDepthMax);
            REDGPU_PRINTF("context.gpus[%d].optionalInfo.supportsResolveModeStencilSampleIndexZero = %d;\n",        gpuIndex, ((const RedGpuInfoOptionalInfoResolveDepthStencil *)optionalInfo)->supportsResolveModeStencilSampleIndexZero);
            REDGPU_PRINTF("context.gpus[%d].optionalInfo.supportsResolveModeStencilAverage = %d;\n",                gpuIndex, ((const RedGpuInfoOptionalInfoResolveDepthStencil *)optionalInfo)->supportsResolveModeStencilAverage);
            REDGPU_PRINTF("context.gpus[%d].optionalInfo.supportsResolveModeStencilMin = %d;\n",                    gpuIndex, ((const RedGpuInfoOptionalInfoResolveDepthStencil *)optionalInfo)->supportsResolveModeStencilMin);
            REDGPU_PRINTF("context.gpus[%d].optionalInfo.supportsResolveModeStencilMax = %d;\n",                    gpuIndex, ((const RedGpuInfoOptionalInfoResolveDepthStencil *)optionalInfo)->supportsResolveModeStencilMax);
            REDGPU_PRINTF("context.gpus[%d].optionalInfo.supportsResolveIndependentNone = %d;\n",                   gpuIndex, ((const RedGpuInfoOptionalInfoResolveDepthStencil *)optionalInfo)->supportsResolveIndependentNone);
            REDGPU_PRINTF("context.gpus[%d].optionalInfo.supportsResolveIndependent = %d;\n",                       gpuIndex, ((const RedGpuInfoOptionalInfoResolveDepthStencil *)optionalInfo)->supportsResolveIndependent);
          } else if (optionalInfo->optionalInfo == RED_GPU_INFO_OPTIONAL_INFO_PROCEDURE_PARAMETERS_HANDLES) {
            REDGPU_PRINTF("context.gpus[%d].optionalInfo.optionalInfo = RED_GPU_INFO_OPTIONAL_INFO_PROCEDURE_PARAMETERS_HANDLES;\n", gpuIndex);
            REDGPU_PRINTF("context.gpus[%d].optionalInfo.next;\n",                                                  gpuIndex);
            REDGPU_PRINTF("context.gpus[%d].optionalInfo.supportsProcedureParametersHandles = %d;\n",               gpuIndex, ((const RedGpuInfoOptionalInfoProcedureParametersHandles *)optionalInfo)->supportsProcedureParametersHandles);
            REDGPU_PRINTF("context.gpus[%d].optionalInfo.maxProcedureParametersHandlesCount = %d;\n",               gpuIndex, ((const RedGpuInfoOptionalInfoProcedureParametersHandles *)optionalInfo)->maxProcedureParametersHandlesCount);
          } else if (optionalInfo->optionalInfo == RED_GPU_INFO_OPTIONAL_INFO_RASTERIZATION_MODE) {
            REDGPU_PRINTF("context.gpus[%d].optionalInfo.optionalInfo = RED_GPU_INFO_OPTIONAL_INFO_RASTERIZATION_MODE;\n", gpuIndex);
            REDGPU_PRINTF("context.gpus[%d].optionalInfo.next;\n",                                                  gpuIndex);
            REDGPU_PRINTF("context.gpus[%d].optionalInfo.supportsRasterizationModeOverestimate = %d;\n",            gpuIndex, ((const RedGpuInfoOptionalInfoRasterizationMode *)optionalInfo)->supportsRasterizationModeOverestimate);
            REDGPU_PRINTF("context.gpus[%d].optionalInfo.supportsRasterizationModeUnderestimate = %d;\n",           gpuIndex, ((const RedGpuInfoOptionalInfoRasterizationMode *)optionalInfo)->supportsRasterizationModeUnderestimate);
            REDGPU_PRINTF("context.gpus[%d].optionalInfo.supportsGuaranteedZeroAreaBackFacingTriangles = %d;\n",    gpuIndex, ((const RedGpuInfoOptionalInfoRasterizationMode *)optionalInfo)->supportsGuaranteedZeroAreaBackFacingTriangles);
            REDGPU_PRINTF("context.gpus[%d].optionalInfo.supportsGpuCodeFullyCoveredEXT = %d;\n",                   gpuIndex, ((const RedGpuInfoOptionalInfoRasterizationMode *)optionalInfo)->supportsGpuCodeFullyCoveredEXT);
          } else if (optionalInfo->optionalInfo == RED_GPU_INFO_OPTIONAL_INFO_FORMAL_MEMORY_MODEL) {
            REDGPU_PRINTF("context.gpus[%d].optionalInfo.optionalInfo = RED_GPU_INFO_OPTIONAL_INFO_FORMAL_MEMORY_MODEL;\n", gpuIndex);
            REDGPU_PRINTF("context.gpus[%d].optionalInfo.next;\n",                                                          gpuIndex);
            REDGPU_PRINTF("context.gpus[%d].optionalInfo.supportsFormalMemoryModel = %d;\n",                                gpuIndex, ((const RedGpuInfoOptionalInfoFormalMemoryModel *)optionalInfo)->supportsFormalMemoryModel);
            REDGPU_PRINTF("context.gpus[%d].optionalInfo.supportsFormalMemoryModelGpuScopeSynchronization = %d;\n",         gpuIndex, ((const RedGpuInfoOptionalInfoFormalMemoryModel *)optionalInfo)->supportsFormalMemoryModelGpuScopeSynchronization);
            REDGPU_PRINTF("context.gpus[%d].optionalInfo.supportsFormalMemoryModelAvailabilityAndVisibilityChains = %d;\n", gpuIndex, ((const RedGpuInfoOptionalInfoFormalMemoryModel *)optionalInfo)->supportsFormalMemoryModelAvailabilityAndVisibilityChains);
          } else if (optionalInfo->optionalInfo == RED_GPU_INFO_OPTIONAL_INFO_ADDITIONAL_INFO_0) {
            REDGPU_PRINTF("context.gpus[%d].optionalInfo.optionalInfo = RED_GPU_INFO_OPTIONAL_INFO_ADDITIONAL_INFO_0;\n",                                     gpuIndex);
            REDGPU_PRINTF("context.gpus[%d].optionalInfo.next;\n",                                                                                            gpuIndex);
            REDGPU_PRINTF("context.gpus[%d].optionalInfo.supportsArraysImagesUsageBeforeMemorySet = %d;\n",                                                   gpuIndex, ((const RedGpuInfoOptionalInfoAdditionalInfo0 *)optionalInfo)->supportsArraysImagesUsageBeforeMemorySet);
            REDGPU_PRINTF("context.gpus[%d].optionalInfo.supportsInitialQueueFamilyIndexSetToMaxValueForSimultaneousQueueAccessOfArrays = %d;\n",             gpuIndex, ((const RedGpuInfoOptionalInfoAdditionalInfo0 *)optionalInfo)->supportsInitialQueueFamilyIndexSetToMaxValueForSimultaneousQueueAccessOfArrays);
            REDGPU_PRINTF("context.gpus[%d].optionalInfo.supportsInitialQueueFamilyIndexSetToMaxValueForSimultaneousQueueAccessOfImages = %d;\n",             gpuIndex, ((const RedGpuInfoOptionalInfoAdditionalInfo0 *)optionalInfo)->supportsInitialQueueFamilyIndexSetToMaxValueForSimultaneousQueueAccessOfImages);
            REDGPU_PRINTF("context.gpus[%d].optionalInfo.supportsInitialQueueFamilyIndexSetToMaxValueForSimultaneousQueueAccessOfImagesMultisample = %d;\n",  gpuIndex, ((const RedGpuInfoOptionalInfoAdditionalInfo0 *)optionalInfo)->supportsInitialQueueFamilyIndexSetToMaxValueForSimultaneousQueueAccessOfImagesMultisample);
            REDGPU_PRINTF("context.gpus[%d].optionalInfo.supportsInitialQueueFamilyIndexSetToMaxValueForSimultaneousQueueAccessOfImagesDepthStencil = %d;\n", gpuIndex, ((const RedGpuInfoOptionalInfoAdditionalInfo0 *)optionalInfo)->supportsInitialQueueFamilyIndexSetToMaxValueForSimultaneousQueueAccessOfImagesDepthStencil);
          } else if (optionalInfo->optionalInfo == RED_GPU_INFO_OPTIONAL_INFO_RAY_TRACING) {
            REDGPU_PRINTF("context.gpus[%d].optionalInfo.optionalInfo = RED_GPU_INFO_OPTIONAL_INFO_RAY_TRACING;\n", gpuIndex);
            REDGPU_PRINTF("context.gpus[%d].optionalInfo.next;\n",                    gpuIndex);
            REDGPU_PRINTF("context.gpus[%d].optionalInfo.supportsRayTracing = %d;\n", gpuIndex, ((const RedGpuInfoOptionalInfoRayTracing *)optionalInfo)->supportsRayTracing);
          } else if (optionalInfo->optionalInfo == RED_GPU_INFO_OPTIONAL_INFO_COMPUTING_LANGUAGE_FEATURE_LEVEL_1) {
            REDGPU_PRINTF("context.gpus[%d].optionalInfo.optionalInfo = RED_GPU_INFO_OPTIONAL_INFO_COMPUTING_LANGUAGE_FEATURE_LEVEL_1;\n", gpuIndex);
            REDGPU_PRINTF("context.gpus[%d].optionalInfo.next;\n",                                        gpuIndex);
            REDGPU_PRINTF("context.gpus[%d].optionalInfo.supportsComputingLanguageFeatureLevel1 = %d;\n", gpuIndex, ((const RedGpuInfoOptionalInfoComputingLanguageFeatureLevel1 *)optionalInfo)->supportsComputingLanguageFeatureLevel1);
          } else if (optionalInfo->optionalInfo == RED_GPU_INFO_OPTIONAL_INFO_ARRAY_TIMESTAMP) {
            REDGPU_PRINTF("context.gpus[%d].optionalInfo.optionalInfo = RED_GPU_INFO_OPTIONAL_INFO_ARRAY_TIMESTAMP;\n", gpuIndex);
            REDGPU_PRINTF("context.gpus[%d].optionalInfo.next;\n",                                        gpuIndex);
            REDGPU_PRINTF("context.gpus[%d].optionalInfo.supportsArrayTimestamp = %d;\n",                 gpuIndex, ((const RedGpuInfoOptionalInfoArrayTimestamp *)optionalInfo)->supportsArrayTimestamp);
            for (unsigned i = 0; i < context->gpus[gpuIndex].queuesCount; i += 1) {
            REDGPU_PRINTF("context.gpus[%d].optionalInfo.queuesPrecisionBits64BitTicksCount[%d] = %d;\n", gpuIndex, i, ((const RedGpuInfoOptionalInfoArrayTimestamp *)optionalInfo)->queuesPrecisionBits64BitTicksCount[i]);
            }
            REDGPU_PRINTF("context.gpus[%d].optionalInfo.nanosecondsPerTick = %.9g;\n",                   gpuIndex, ((const RedGpuInfoOptionalInfoArrayTimestamp *)optionalInfo)->nanosecondsPerTick);
          }
          const RedGpuInfoOptionalInfoIterator * nextOptionalInfo = (const RedGpuInfoOptionalInfoIterator *)optionalInfo->next;
          optionalInfo = nextOptionalInfo;
        }
      }
      if (context->gpusCount == 0) {
        REDGPU_PRINTF("context.gpusStatuses = NULL;\n");
      }
      for (unsigned gpuIndex = 0; gpuIndex < context->gpusCount; gpuIndex += 1) {
        REDGPU_PRINTF("context.gpusStatuses[%d].status = %s;\n",                     gpuIndex, REDGPU_PRINT_STATUS(context->gpusStatuses[gpuIndex].status));
        REDGPU_PRINTF("context.gpusStatuses[%d].statusCode = %d;\n",                 gpuIndex, context->gpusStatuses[gpuIndex].statusCode);
        REDGPU_PRINTF("context.gpusStatuses[%d].statusHresult = %d;\n",              gpuIndex, context->gpusStatuses[gpuIndex].statusHresult);
        REDGPU_PRINTF("context.gpusStatuses[%d].statusProcedureId = %s;\n",          gpuIndex, REDGPU_PRINT_PROCEDURE_ID(context->gpusStatuses[gpuIndex].statusProcedureId));
        REDGPU_PRINTF("context.gpusStatuses[%d].statusFile = %s%s%s;\n",             gpuIndex, context->gpusStatuses[gpuIndex].statusFile == NULL ? "" : "\"", context->gpusStatuses[gpuIndex].statusFile, context->gpusStatuses[gpuIndex].statusFile == NULL ? "" : "\"");
        REDGPU_PRINTF("context.gpusStatuses[%d].statusLine = %d;\n",                 gpuIndex, context->gpusStatuses[gpuIndex].statusLine);
        REDGPU_PRINTF("context.gpusStatuses[%d].statusError = %s;\n",                gpuIndex, REDGPU_PRINT_STATUS(context->gpusStatuses[gpuIndex].statusError));
        REDGPU_PRINTF("context.gpusStatuses[%d].statusErrorCode = %d;\n",            gpuIndex, context->gpusStatuses[gpuIndex].statusErrorCode);
        REDGPU_PRINTF("context.gpusStatuses[%d].statusErrorHresult = %d;\n",         gpuIndex, context->gpusStatuses[gpuIndex].statusErrorHresult);
        REDGPU_PRINTF("context.gpusStatuses[%d].statusErrorProcedureId = %s;\n",     gpuIndex, REDGPU_PRINT_PROCEDURE_ID(context->gpusStatuses[gpuIndex].statusErrorProcedureId));
        REDGPU_PRINTF("context.gpusStatuses[%d].statusErrorFile = %s%s%s;\n",        gpuIndex, context->gpusStatuses[gpuIndex].statusErrorFile == NULL ? "" : "\"", context->gpusStatuses[gpuIndex].statusErrorFile, context->gpusStatuses[gpuIndex].statusErrorFile == NULL ? "" : "\"");
        REDGPU_PRINTF("context.gpusStatuses[%d].statusErrorLine = %d;\n",            gpuIndex, context->gpusStatuses[gpuIndex].statusErrorLine);
        REDGPU_PRINTF("context.gpusStatuses[%d].statusErrorDescription = \"%s\";\n", gpuIndex, context->gpusStatuses[gpuIndex].statusErrorDescription);
      }
      REDGPU_PRINTF("context.handle = %p;\n", REDGPU_PRINT_POINTER(context->handle));
      REDGPU_PRINTF("context.userData = %p;\n", REDGPU_PRINT_POINTER(context->userData));
    }

#define REDGPU_PRINT_STATUSES_POINTER(x)\
  #x".status                 = %s;\n"\
  #x".statusCode             = %d;\n"\
  #x".statusHresult          = %d;\n"\
  #x".statusProcedureId      = %s;\n"\
  #x".statusFile             = %s%s%s;\n"\
  #x".statusLine             = %d;\n"\
  #x".statusError            = %s;\n"\
  #x".statusErrorCode        = %d;\n"\
  #x".statusErrorHresult     = %d;\n"\
  #x".statusErrorProcedureId = %s;\n"\
  #x".statusErrorFile        = %s%s%s;\n"\
  #x".statusErrorLine        = %d;\n"\
  #x".statusErrorDescription = \"%s\";\n"\
  ,\
  REDGPU_PRINT_STATUS((x)->status),\
  (x)->statusCode,\
  (x)->statusHresult,\
  REDGPU_PRINT_PROCEDURE_ID((x)->statusProcedureId),\
  (x)->statusFile == 0 ? "" : "\"",\
  (x)->statusFile,\
  (x)->statusFile == 0 ? "" : "\"",\
  (x)->statusLine,\
  REDGPU_PRINT_STATUS((x)->statusError),\
  (x)->statusErrorCode,\
  (x)->statusErrorHresult,\
  REDGPU_PRINT_PROCEDURE_ID((x)->statusErrorProcedureId),\
  (x)->statusErrorFile == 0 ? "" : "\"",\
  (x)->statusErrorFile,\
  (x)->statusErrorFile == 0 ? "" : "\"",\
  (x)->statusErrorLine,\
  (x)->statusErrorDescription

    if (__redgpuDebugStatusesWereNotSetByTheUser == 1) {
      if (optionalFile != NULL) {
        REDGPU_PRINTF("[REDGPU DEBUG LIBRARY][RED_PROCEDURE_ID_redCreateContext][%s:%d]: %s\n", optionalFile, optionalLine, "Returned outStatuses (not requested by the user):");
      } else {
        REDGPU_PRINTF("[REDGPU DEBUG LIBRARY][RED_PROCEDURE_ID_redCreateContext]: %s\n", "Returned outStatuses (not requested by the user):");
      }
    } else {
      if (optionalFile != NULL) {
        REDGPU_PRINTF("[REDGPU DEBUG LIBRARY][RED_PROCEDURE_ID_redCreateContext][%s:%d]: %s\n", optionalFile, optionalLine, "Returned outStatuses:");
      } else {
        REDGPU_PRINTF("[REDGPU DEBUG LIBRARY][RED_PROCEDURE_ID_redCreateContext]: %s\n", "Returned outStatuses:");
      }
    }
    if (outStatuses != NULL) {
      if ((VkResult)outStatuses->status == VK_SUCCESS) {
        outStatuses->statusProcedureId = procedureId;
        outStatuses->statusFile        = optionalFile;
        outStatuses->statusLine        = optionalLine;
      }
      if ((VkResult)outStatuses->statusError == VK_SUCCESS) {
        outStatuses->statusErrorProcedureId = procedureId;
        outStatuses->statusErrorFile        = optionalFile;
        outStatuses->statusErrorLine        = optionalLine;
      }
    }
    REDGPU_PRINTF(REDGPU_PRINT_STATUSES_POINTER(outStatuses));

#undef REDGPU_PRINT_STATUSES_POINTER

    REDGPU_FFLUSH(stdout);
  }
#endif
}

static inline void redInlineCreateArray(RedContext context, RedHandleGpu gpu, const char * handleName, RedArrayType type, uint64_t bytesCount, unsigned initialQueueFamilyIndex, RedBool32 dedicate, RedArray * outArray, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData) {
  const RedProcedureId procedureId = RED_PROCEDURE_ID_redCreateArray;

  if (type == RED_ARRAY_TYPE_ARRAY_RO) {
    type = RED_ARRAY_TYPE_ARRAY_RW;
  }

  RedInternalContext ctx = (RedInternalContext)context;
  RedInternalInstanceProcedureAddresses * vk = ctx->internal.instanceProcedureAddresses;

  unsigned gpuIndex = (unsigned)-1;
  for (unsigned i = 0; i < ctx->internal.hotGpuHandlesCount; i += 1) {
    if (ctx->internal.hotGpuHandles[i] == gpu) {
      gpuIndex = i;
      break;
    }
  }

  if (gpuIndex == (unsigned)-1) {
    redInternalSetStatus(ctx, gpu, outStatuses, 259, (VkResult)RED_STATUS_ERROR_INITIALIZATION_FAILED, procedureId, optionalFile, optionalLine, optionalUserData);
    return;
  }

  if (redInternalGpuExtensionIsEnabled(ctx, gpuIndex, RED_INTERNAL_GPU_EXTENSION_BITFLAG_DEDICATE_MEMORY_NV))
  {
  } else {
    dedicate = 0;
  }

  struct VkDedicatedAllocationBufferCreateInfoNV {
    unsigned sType;
    void *   pNext;
    VkBool32 dedicatedAllocation;
  } dedicateInfo = {
    1000026001, // VK_STRUCTURE_TYPE_DEDICATED_ALLOCATION_BUFFER_CREATE_INFO_NV
    NULL,
    VK_TRUE
  };

  VkBufferCreateInfo info;
  info.sType                   = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
  info.pNext                   = dedicate == 1 ? &dedicateInfo : NULL;
  info.flags                   = 0;
  info.size                    = bytesCount;
  info.usage                   = (VkBufferUsageFlags)type;
  if (initialQueueFamilyIndex == (unsigned)-1) {
    info.sharingMode           = VK_SHARING_MODE_CONCURRENT;
    info.queueFamilyIndexCount = ctx->internal.cold->gpus[gpuIndex].vkQueueFamilyIndexesCount;
    info.pQueueFamilyIndices   = ctx->internal.cold->gpus[gpuIndex].vkQueueFamilyIndexes;
  } else {
    info.sharingMode           = VK_SHARING_MODE_EXCLUSIVE;
    info.queueFamilyIndexCount = 1;
    info.pQueueFamilyIndices   = &initialQueueFamilyIndex;
  }
  redInternalSetStatus(ctx, gpu, outStatuses, 260, vk->vkCreateBuffer((VkDevice)gpu, &info, NULL, (VkBuffer *)&outArray->handle), procedureId, optionalFile, optionalLine, optionalUserData);

  VkMemoryRequirements memoryRequirements = {0};
  if (outArray->handle != NULL) {
    vk->vkGetBufferMemoryRequirements((VkDevice)gpu, (VkBuffer)outArray->handle, &memoryRequirements);
  }
  outArray->memoryBytesCount     = memoryRequirements.size;
  outArray->memoryBytesAlignment = memoryRequirements.alignment;
  outArray->memoryTypesSupported = memoryRequirements.memoryTypeBits;

  if (ctx->internal.debugUtilsMessengerCallback != NULL && handleName != NULL) {
    if (outArray->handle != NULL) {
      VkDebugUtilsObjectNameInfoEXT info;
      info.sType        = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT;
      info.pNext        = NULL;
      info.objectType   = (VkObjectType)RED_HANDLE_TYPE_ARRAY;
      info.objectHandle = (uint64_t)outArray->handle;
      info.pObjectName  = handleName;
      redInternalSetStatus(ctx, gpu, outStatuses, 261, vk->vkSetDebugUtilsObjectNameEXT((VkDevice)gpu, &info), procedureId, optionalFile, optionalLine, optionalUserData);
    }
  }
}

static inline void redInlineCreateImage(RedContext context, RedHandleGpu gpu, const char * handleName, RedImageDimensions dimensions, RedFormat format, unsigned width, unsigned height, unsigned depth, unsigned levelsCount, unsigned layersCount, RedMultisampleCountBitflag multisampleCount, RedAccessBitflags restrictToAccess, unsigned initialQueueFamilyIndex, RedBool32 dedicate, RedImage * outImage, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData) {
  const RedProcedureId procedureId = RED_PROCEDURE_ID_redCreateImage;

  RedInternalContext ctx = (RedInternalContext)context;
  RedInternalInstanceProcedureAddresses * vk = ctx->internal.instanceProcedureAddresses;

  unsigned gpuIndex = (unsigned)-1;
  for (unsigned i = 0; i < ctx->internal.hotGpuHandlesCount; i += 1) {
    if (ctx->internal.hotGpuHandles[i] == gpu) {
      gpuIndex = i;
      break;
    }
  }

  if (gpuIndex == (unsigned)-1) {
    redInternalSetStatus(ctx, gpu, outStatuses, 262, (VkResult)RED_STATUS_ERROR_INITIALIZATION_FAILED, procedureId, optionalFile, optionalLine, optionalUserData);
    return;
  }

  VkImageUsageFlags imageUsagePossible = redInternalImageGetUsage(ctx, gpuIndex, format, multisampleCount == RED_MULTISAMPLE_COUNT_BITFLAG_1 ? 0 : 1);
  VkImageUsageFlags imageUsage         = 0;
  if (restrictToAccess == 0) {
    imageUsage = REDGPU_B32(1111,1111,1111,1111,1111,1111,1111,1111);
  } else {
    if ((restrictToAccess & RED_ACCESS_BITFLAG_COPY_R)                               == RED_ACCESS_BITFLAG_COPY_R)                               { imageUsage |= VK_IMAGE_USAGE_TRANSFER_SRC_BIT; }
    if ((restrictToAccess & RED_ACCESS_BITFLAG_COPY_W)                               == RED_ACCESS_BITFLAG_COPY_W)                               { imageUsage |= VK_IMAGE_USAGE_TRANSFER_DST_BIT; }
    if ((restrictToAccess & RED_ACCESS_BITFLAG_STRUCT_RESOURCE_NON_FRAGMENT_STAGE_R) == RED_ACCESS_BITFLAG_STRUCT_RESOURCE_NON_FRAGMENT_STAGE_R) { imageUsage |= VK_IMAGE_USAGE_SAMPLED_BIT; }
    if ((restrictToAccess & RED_ACCESS_BITFLAG_STRUCT_RESOURCE_FRAGMENT_STAGE_R)     == RED_ACCESS_BITFLAG_STRUCT_RESOURCE_FRAGMENT_STAGE_R)     { imageUsage |= VK_IMAGE_USAGE_SAMPLED_BIT; }
    if ((restrictToAccess & RED_ACCESS_BITFLAG_STRUCT_RESOURCE_W)                    == RED_ACCESS_BITFLAG_STRUCT_RESOURCE_W)                    { imageUsage |= VK_IMAGE_USAGE_STORAGE_BIT; }
    if ((restrictToAccess & RED_ACCESS_BITFLAG_OUTPUT_DEPTH_R)                       == RED_ACCESS_BITFLAG_OUTPUT_DEPTH_R)                       { imageUsage |= VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT; }
    if ((restrictToAccess & RED_ACCESS_BITFLAG_OUTPUT_DEPTH_RW)                      == RED_ACCESS_BITFLAG_OUTPUT_DEPTH_RW)                      { imageUsage |= VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT; }
    if ((restrictToAccess & RED_ACCESS_BITFLAG_OUTPUT_STENCIL_R)                     == RED_ACCESS_BITFLAG_OUTPUT_STENCIL_R)                     { imageUsage |= VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT; }
    if ((restrictToAccess & RED_ACCESS_BITFLAG_OUTPUT_STENCIL_RW)                    == RED_ACCESS_BITFLAG_OUTPUT_STENCIL_RW)                    { imageUsage |= VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT; }
    if ((restrictToAccess & RED_ACCESS_BITFLAG_OUTPUT_COLOR_W)                       == RED_ACCESS_BITFLAG_OUTPUT_COLOR_W)                       { imageUsage |= VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT; }
  }
  VkImageUsageFlags usage = imageUsage;
  imageUsage = usage & imageUsagePossible;

  VkImageCreateFlags createFlags = 0;
  {
    RedImageDimensions imageDimensions = dimensions;
    if (imageDimensions == RED_IMAGE_DIMENSIONS_2D_WITH_TEXTURE_DIMENSIONS_CUBE_AND_CUBE_LAYERED) {
      dimensions  = RED_IMAGE_DIMENSIONS_2D;
      createFlags = VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT;
    } else if (imageDimensions == RED_IMAGE_DIMENSIONS_3D_WITH_TEXTURE_DIMENSIONS_2D_AND_2D_LAYERED) {
      dimensions  = RED_IMAGE_DIMENSIONS_3D;
      createFlags = VK_IMAGE_CREATE_2D_ARRAY_COMPATIBLE_BIT;
    }
  }

  if (redInternalGpuExtensionIsEnabled(ctx, gpuIndex, RED_INTERNAL_GPU_EXTENSION_BITFLAG_DEDICATE_MEMORY_NV))
  {
  } else {
    dedicate = 0;
  }

  struct VkDedicatedAllocationImageCreateInfoNV {
    unsigned sType;
    void *   pNext;
    VkBool32 dedicatedAllocation;
  } dedicateInfo = {
    1000026000, // VK_STRUCTURE_TYPE_DEDICATED_ALLOCATION_IMAGE_CREATE_INFO_NV
    NULL,
    VK_TRUE
  };

  VkImageCreateInfo info;
  info.sType                   = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
  info.pNext                   = dedicate == 1 ? &dedicateInfo : NULL;
  info.flags                   = createFlags;
  info.imageType               = (VkImageType)dimensions;
  info.format                  = (VkFormat)format;
  info.extent.width            = width;
  info.extent.height           = height;
  info.extent.depth            = depth;
  info.mipLevels               = levelsCount;
  info.arrayLayers             = layersCount;
  info.samples                 = (VkSampleCountFlagBits)multisampleCount;
  info.tiling                  = VK_IMAGE_TILING_OPTIMAL;
  info.usage                   = imageUsage;
  if (initialQueueFamilyIndex == (unsigned)-1) {
    info.sharingMode           = VK_SHARING_MODE_CONCURRENT;
    info.queueFamilyIndexCount = ctx->internal.cold->gpus[gpuIndex].vkQueueFamilyIndexesCount;
    info.pQueueFamilyIndices   = ctx->internal.cold->gpus[gpuIndex].vkQueueFamilyIndexes;
  } else {
    info.sharingMode           = VK_SHARING_MODE_EXCLUSIVE;
    info.queueFamilyIndexCount = 1;
    info.pQueueFamilyIndices   = &initialQueueFamilyIndex;
  }
  info.initialLayout           = VK_IMAGE_LAYOUT_UNDEFINED;
  redInternalSetStatus(ctx, gpu, outStatuses, 263, vk->vkCreateImage((VkDevice)gpu, &info, NULL, (VkImage *)&outImage->handle), procedureId, optionalFile, optionalLine, optionalUserData);

  VkMemoryRequirements memoryRequirements = {0};
  if (outImage->handle != NULL) {
    vk->vkGetImageMemoryRequirements((VkDevice)gpu, (VkImage)outImage->handle, &memoryRequirements);
  }
  outImage->memoryBytesCount     = memoryRequirements.size;
  outImage->memoryBytesAlignment = memoryRequirements.alignment;
  outImage->memoryTypesSupported = memoryRequirements.memoryTypeBits;

  if (ctx->internal.debugUtilsMessengerCallback != NULL && handleName != NULL) {
    if (outImage->handle != NULL) {
      VkDebugUtilsObjectNameInfoEXT info;
      info.sType        = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT;
      info.pNext        = NULL;
      info.objectType   = (VkObjectType)RED_HANDLE_TYPE_IMAGE;
      info.objectHandle = (uint64_t)outImage->handle;
      info.pObjectName  = handleName;
      redInternalSetStatus(ctx, gpu, outStatuses, 264, vk->vkSetDebugUtilsObjectNameEXT((VkDevice)gpu, &info), procedureId, optionalFile, optionalLine, optionalUserData);
    }
  }
}

static inline void redInlineCreateSampler(RedContext context, RedHandleGpu gpu, const char * handleName, RedSamplerFiltering filteringMag, RedSamplerFiltering filteringMin, RedSamplerFilteringMip filteringMip, RedSamplerBehaviorOutsideTextureCoordinate behaviorOutsideTextureCoordinateU, RedSamplerBehaviorOutsideTextureCoordinate behaviorOutsideTextureCoordinateV, RedSamplerBehaviorOutsideTextureCoordinate behaviorOutsideTextureCoordinateW, float mipLodBias, RedBool32 enableAnisotropy, float maxAnisotropy, RedBool32 enableCompare, RedCompareOp compareOp, float minLod, float maxLod, RedBool32 unnormalizedCoordinates, RedHandleSampler * outSampler, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData) {
  const RedProcedureId procedureId = RED_PROCEDURE_ID_redCreateSampler;

  RedInternalContext ctx = (RedInternalContext)context;
  RedInternalInstanceProcedureAddresses * vk = ctx->internal.instanceProcedureAddresses;

  VkSamplerCreateInfo info;
  info.sType                   = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
  info.pNext                   = NULL;
  info.flags                   = 0;
  info.magFilter               = (VkFilter)filteringMag;
  info.minFilter               = (VkFilter)filteringMin;
  info.mipmapMode              = (VkSamplerMipmapMode)filteringMip;
  info.addressModeU            = (VkSamplerAddressMode)behaviorOutsideTextureCoordinateU;
  info.addressModeV            = (VkSamplerAddressMode)behaviorOutsideTextureCoordinateV;
  info.addressModeW            = (VkSamplerAddressMode)behaviorOutsideTextureCoordinateW;
  info.mipLodBias              = mipLodBias;
  info.anisotropyEnable        = (VkBool32)enableAnisotropy;
  info.maxAnisotropy           = maxAnisotropy;
  info.compareEnable           = (VkBool32)enableCompare;
  info.compareOp               = (VkCompareOp)compareOp;
  info.minLod                  = minLod;
  info.maxLod                  = maxLod;
  info.borderColor             = VK_BORDER_COLOR_FLOAT_TRANSPARENT_BLACK;
  info.unnormalizedCoordinates = (VkBool32)unnormalizedCoordinates;
  redInternalSetStatus(ctx, gpu, outStatuses, 265, vk->vkCreateSampler((VkDevice)gpu, &info, NULL, (VkSampler *)outSampler), procedureId, optionalFile, optionalLine, optionalUserData);

  if (ctx->internal.debugUtilsMessengerCallback != NULL && handleName != NULL) {
    if (outSampler[0] != NULL) {
      VkDebugUtilsObjectNameInfoEXT info;
      info.sType        = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT;
      info.pNext        = NULL;
      info.objectType   = (VkObjectType)RED_HANDLE_TYPE_SAMPLER;
      info.objectHandle = (uint64_t)outSampler[0];
      info.pObjectName  = handleName;
      redInternalSetStatus(ctx, gpu, outStatuses, 266, vk->vkSetDebugUtilsObjectNameEXT((VkDevice)gpu, &info), procedureId, optionalFile, optionalLine, optionalUserData);
    }
  }
}

static inline void redInlineCreateTexture(RedContext context, RedHandleGpu gpu, const char * handleName, RedHandleImage image, RedImagePartBitflags parts, RedTextureDimensions dimensions, RedFormat format, unsigned levelsFirst, unsigned levelsCount, unsigned layersFirst, unsigned layersCount, RedHandleTexture * outTexture, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData) {
  const RedProcedureId procedureId = RED_PROCEDURE_ID_redCreateTexture;

  RedInternalContext ctx = (RedInternalContext)context;
  RedInternalInstanceProcedureAddresses * vk = ctx->internal.instanceProcedureAddresses;

  VkImageViewCreateInfo info;
  info.sType                           = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
  info.pNext                           = NULL;
  info.flags                           = 0;
  info.image                           = (VkImage)image;
  info.viewType                        = (VkImageViewType)dimensions;
  info.format                          = (VkFormat)format;
  info.components.r                    = VK_COMPONENT_SWIZZLE_IDENTITY;
  info.components.g                    = VK_COMPONENT_SWIZZLE_IDENTITY;
  info.components.b                    = VK_COMPONENT_SWIZZLE_IDENTITY;
  info.components.a                    = VK_COMPONENT_SWIZZLE_IDENTITY;
  info.subresourceRange.aspectMask     = (VkImageAspectFlags)parts;
  info.subresourceRange.baseMipLevel   = levelsFirst;
  info.subresourceRange.levelCount     = levelsCount;
  info.subresourceRange.baseArrayLayer = layersFirst;
  info.subresourceRange.layerCount     = layersCount;
  redInternalSetStatus(ctx, gpu, outStatuses, 267, vk->vkCreateImageView((VkDevice)gpu, &info, NULL, (VkImageView *)outTexture), procedureId, optionalFile, optionalLine, optionalUserData);

  if (ctx->internal.debugUtilsMessengerCallback != NULL && handleName != NULL) {
    if (outTexture[0] != NULL) {
      VkDebugUtilsObjectNameInfoEXT info;
      info.sType        = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT;
      info.pNext        = NULL;
      info.objectType   = (VkObjectType)RED_HANDLE_TYPE_TEXTURE;
      info.objectHandle = (uint64_t)outTexture[0];
      info.pObjectName  = handleName;
      redInternalSetStatus(ctx, gpu, outStatuses, 268, vk->vkSetDebugUtilsObjectNameEXT((VkDevice)gpu, &info), procedureId, optionalFile, optionalLine, optionalUserData);
    }
  }
}

static inline void redInlineCreateGpuCode(RedContext context, RedHandleGpu gpu, const char * handleName, uint64_t irBytesCount, const void * ir, RedHandleGpuCode * outGpuCode, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData) {
  const RedProcedureId procedureId = RED_PROCEDURE_ID_redCreateGpuCode;

  RedInternalContext ctx = (RedInternalContext)context;
  RedInternalInstanceProcedureAddresses * vk = ctx->internal.instanceProcedureAddresses;

  VkShaderModuleCreateInfo info;
  info.sType    = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
  info.pNext    = NULL;
  info.flags    = 0;
  info.codeSize = (size_t)irBytesCount;
  info.pCode    = (const unsigned *)ir;
  redInternalSetStatus(ctx, gpu, outStatuses, 269, vk->vkCreateShaderModule((VkDevice)gpu, &info, NULL, (VkShaderModule *)outGpuCode), procedureId, optionalFile, optionalLine, optionalUserData);

  if (ctx->internal.debugUtilsMessengerCallback != NULL && handleName != NULL) {
    if (outGpuCode[0] != NULL) {
      VkDebugUtilsObjectNameInfoEXT info;
      info.sType        = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT;
      info.pNext        = NULL;
      info.objectType   = (VkObjectType)RED_HANDLE_TYPE_GPU_CODE;
      info.objectHandle = (uint64_t)outGpuCode[0];
      info.pObjectName  = handleName;
      redInternalSetStatus(ctx, gpu, outStatuses, 270, vk->vkSetDebugUtilsObjectNameEXT((VkDevice)gpu, &info), procedureId, optionalFile, optionalLine, optionalUserData);
    }
  }
}

static inline void redInlineCreateOutputDeclaration(RedContext context, RedHandleGpu gpu, const char * handleName, const RedOutputDeclarationMembers * outputDeclarationMembers, const RedOutputDeclarationMembersResolveSources * outputDeclarationMembersResolveSources, RedBool32 dependencyByRegion, RedBool32 dependencyByRegionAllowUsageOrderBarriers, RedHandleOutputDeclaration * outOutputDeclaration, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData) {
  const RedProcedureId procedureId = RED_PROCEDURE_ID_redCreateOutputDeclaration;

  RedInternalContext ctx = (RedInternalContext)context;
  RedInternalInstanceProcedureAddresses * vk = ctx->internal.instanceProcedureAddresses;

  unsigned gpuIndex = (unsigned)-1;
  for (unsigned i = 0; i < ctx->internal.hotGpuHandlesCount; i += 1) {
    if (ctx->internal.hotGpuHandles[i] == gpu) {
      gpuIndex = i;
      break;
    }
  }

  if (gpuIndex == (unsigned)-1) {
    redInternalSetStatus(ctx, gpu, outStatuses, 271, (VkResult)RED_STATUS_ERROR_INITIALIZATION_FAILED, procedureId, optionalFile, optionalLine, optionalUserData);
    return;
  }

  const unsigned  colorsCount        = outputDeclarationMembers == NULL ? 0 : outputDeclarationMembers->colorsCount;
  const RedBool32 depthStencilEnable = outputDeclarationMembers == NULL ? 0 : outputDeclarationMembers->depthStencilEnable;

  if (redInternalGpuExtensionIsEnabled(ctx, gpuIndex, RED_INTERNAL_GPU_EXTENSION_BITFLAG_DEPTH_STENCIL_RESOLVE)) {
    struct VkAttachmentDescription2KHR {
      unsigned                                   sType;
      void *                                     pNext;
      VkAttachmentDescriptionFlags               flags;
      VkFormat                                   format;
      VkSampleCountFlagBits                      samples;
      VkAttachmentLoadOp                         loadOp;
      VkAttachmentStoreOp                        storeOp;
      VkAttachmentLoadOp                         stencilLoadOp;
      VkAttachmentStoreOp                        stencilStoreOp;
      VkImageLayout                              initialLayout;
      VkImageLayout                              finalLayout;
    };
    struct VkAttachmentReference2KHR {
      unsigned                                   sType;
      void *                                     pNext;
      uint32_t                                   attachment;
      VkImageLayout                              layout;
      VkImageAspectFlags                         aspectMask;
    };
    struct VkSubpassDescription2KHR {
      unsigned                                   sType;
      void *                                     pNext;
      VkSubpassDescriptionFlags                  flags;
      VkPipelineBindPoint                        pipelineBindPoint;
      uint32_t                                   viewMask;
      uint32_t                                   inputAttachmentCount;
      const struct VkAttachmentReference2KHR *   pInputAttachments;
      uint32_t                                   colorAttachmentCount;
      const struct VkAttachmentReference2KHR *   pColorAttachments;
      const struct VkAttachmentReference2KHR *   pResolveAttachments;
      const struct VkAttachmentReference2KHR *   pDepthStencilAttachment;
      uint32_t                                   preserveAttachmentCount;
      const uint32_t *                           pPreserveAttachments;
    };
    struct VkSubpassDependency2KHR {
      unsigned                                   sType;
      void *                                     pNext;
      uint32_t                                   srcSubpass;
      uint32_t                                   dstSubpass;
      VkPipelineStageFlags                       srcStageMask;
      VkPipelineStageFlags                       dstStageMask;
      VkAccessFlags                              srcAccessMask;
      VkAccessFlags                              dstAccessMask;
      VkDependencyFlags                          dependencyFlags;
      int32_t                                    viewOffset;
    };
    struct VkRenderPassCreateInfo2KHR {
      unsigned                                   sType;
      void *                                     pNext;
      VkRenderPassCreateFlags                    flags;
      uint32_t                                   attachmentCount;
      const struct VkAttachmentDescription2KHR * pAttachments;
      uint32_t                                   subpassCount;
      const struct VkSubpassDescription2KHR *    pSubpasses;
      uint32_t                                   dependencyCount;
      const struct VkSubpassDependency2KHR *     pDependencies;
      uint32_t                                   correlatedViewMaskCount;
      const uint32_t *                           pCorrelatedViewMasks;
    };
    struct VkSubpassDescriptionDepthStencilResolveKHR {
      unsigned                                   sType;
      void *                                     pNext;
      RedResolveMode                             depthResolveMode;
      RedResolveMode                             stencilResolveMode;
      const struct VkAttachmentReference2KHR *   pDepthStencilResolveAttachment;
    };
    struct VkAttachmentDescription2KHR attachmentDescriptions[8 + 1 + 8 + 1];
    for (unsigned i = 0; i < colorsCount; i += 1) {
      attachmentDescriptions[i].sType          = 1000109000; // VK_STRUCTURE_TYPE_ATTACHMENT_DESCRIPTION_2_KHR
      attachmentDescriptions[i].pNext          = NULL;
      attachmentDescriptions[i].flags          = outputDeclarationMembers->colorsSharesMemoryWithAnotherMember[i] == 1 ? VK_ATTACHMENT_DESCRIPTION_MAY_ALIAS_BIT : 0;
      attachmentDescriptions[i].format         = (VkFormat)outputDeclarationMembers->colorsFormat[i];
      attachmentDescriptions[i].samples        = (VkSampleCountFlagBits)outputDeclarationMembers->colorsMultisampleCount[i];
      attachmentDescriptions[i].loadOp         = (VkAttachmentLoadOp)outputDeclarationMembers->colorsSetProcedureOutputOp[i];
      attachmentDescriptions[i].storeOp        = (VkAttachmentStoreOp)outputDeclarationMembers->colorsEndProcedureOutputOp[i];
      attachmentDescriptions[i].stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_LOAD;
      attachmentDescriptions[i].stencilStoreOp = VK_ATTACHMENT_STORE_OP_STORE;
      attachmentDescriptions[i].initialLayout  = VK_IMAGE_LAYOUT_GENERAL;
      attachmentDescriptions[i].finalLayout    = VK_IMAGE_LAYOUT_GENERAL;
    }
    if (depthStencilEnable == 1) {
      attachmentDescriptions[colorsCount].sType          = 1000109000; // VK_STRUCTURE_TYPE_ATTACHMENT_DESCRIPTION_2_KHR
      attachmentDescriptions[colorsCount].pNext          = NULL;
      attachmentDescriptions[colorsCount].flags          = outputDeclarationMembers->depthStencilSharesMemoryWithAnotherMember == 1 ? VK_ATTACHMENT_DESCRIPTION_MAY_ALIAS_BIT : 0;
      attachmentDescriptions[colorsCount].format         = (VkFormat)outputDeclarationMembers->depthStencilFormat;
      attachmentDescriptions[colorsCount].samples        = (VkSampleCountFlagBits)outputDeclarationMembers->depthStencilMultisampleCount;
      attachmentDescriptions[colorsCount].loadOp         = (VkAttachmentLoadOp)outputDeclarationMembers->depthStencilDepthSetProcedureOutputOp;
      attachmentDescriptions[colorsCount].storeOp        = (VkAttachmentStoreOp)outputDeclarationMembers->depthStencilDepthEndProcedureOutputOp;
      attachmentDescriptions[colorsCount].stencilLoadOp  = (VkAttachmentLoadOp)outputDeclarationMembers->depthStencilStencilSetProcedureOutputOp;
      attachmentDescriptions[colorsCount].stencilStoreOp = (VkAttachmentStoreOp)outputDeclarationMembers->depthStencilStencilEndProcedureOutputOp;
      attachmentDescriptions[colorsCount].initialLayout  = VK_IMAGE_LAYOUT_GENERAL;
      attachmentDescriptions[colorsCount].finalLayout    = VK_IMAGE_LAYOUT_GENERAL;
    }
    unsigned attachmentDescriptionsCount = colorsCount;
    if (depthStencilEnable == 1) {
      attachmentDescriptionsCount += 1;
    }
    if (outputDeclarationMembersResolveSources != NULL) {
      if (outputDeclarationMembersResolveSources->resolveColors == 1) {
        for (unsigned i = 0; i < colorsCount; i += 1) {
          const unsigned index = attachmentDescriptionsCount + i;
          attachmentDescriptions[index].sType          = 1000109000; // VK_STRUCTURE_TYPE_ATTACHMENT_DESCRIPTION_2_KHR
          attachmentDescriptions[index].pNext          = NULL;
          attachmentDescriptions[index].flags          = outputDeclarationMembers->colorsSharesMemoryWithAnotherMember[i] == 1 ? VK_ATTACHMENT_DESCRIPTION_MAY_ALIAS_BIT : 0;
          attachmentDescriptions[index].format         = (VkFormat)outputDeclarationMembers->colorsFormat[i];
          attachmentDescriptions[index].samples        = VK_SAMPLE_COUNT_1_BIT;
          attachmentDescriptions[index].loadOp         = VK_ATTACHMENT_LOAD_OP_LOAD;
          attachmentDescriptions[index].storeOp        = VK_ATTACHMENT_STORE_OP_STORE;
          attachmentDescriptions[index].stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_LOAD;
          attachmentDescriptions[index].stencilStoreOp = VK_ATTACHMENT_STORE_OP_STORE;
          attachmentDescriptions[index].initialLayout  = VK_IMAGE_LAYOUT_GENERAL;
          attachmentDescriptions[index].finalLayout    = VK_IMAGE_LAYOUT_GENERAL;
        }
      }
      if (outputDeclarationMembersResolveSources->resolveDepthStencil == 1) {
        if (depthStencilEnable == 1) {
          const unsigned index = attachmentDescriptionsCount + (outputDeclarationMembersResolveSources->resolveColors == 1 ? colorsCount : 0);
          attachmentDescriptions[index].sType          = 1000109000; // VK_STRUCTURE_TYPE_ATTACHMENT_DESCRIPTION_2_KHR
          attachmentDescriptions[index].pNext          = NULL;
          attachmentDescriptions[index].flags          = outputDeclarationMembers->depthStencilSharesMemoryWithAnotherMember == 1 ? VK_ATTACHMENT_DESCRIPTION_MAY_ALIAS_BIT : 0;
          attachmentDescriptions[index].format         = (VkFormat)outputDeclarationMembers->depthStencilFormat;
          attachmentDescriptions[index].samples        = VK_SAMPLE_COUNT_1_BIT;
          attachmentDescriptions[index].loadOp         = VK_ATTACHMENT_LOAD_OP_LOAD;
          attachmentDescriptions[index].storeOp        = VK_ATTACHMENT_STORE_OP_STORE;
          attachmentDescriptions[index].stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_LOAD;
          attachmentDescriptions[index].stencilStoreOp = VK_ATTACHMENT_STORE_OP_STORE;
          attachmentDescriptions[index].initialLayout  = VK_IMAGE_LAYOUT_GENERAL;
          attachmentDescriptions[index].finalLayout    = VK_IMAGE_LAYOUT_GENERAL;
        }
      }
    }
    struct VkAttachmentReference2KHR attachmentDescriptionReferencesColors[8];                      // VkAttachmentDescriptionReference
    for (unsigned i = 0; i < colorsCount; i += 1) {
      attachmentDescriptionReferencesColors[i].sType      = 1000109001; // VK_STRUCTURE_TYPE_ATTACHMENT_REFERENCE_2_KHR
      attachmentDescriptionReferencesColors[i].pNext      = NULL;
      attachmentDescriptionReferencesColors[i].attachment = i;                                      // attachmentDescriptionIndex
      attachmentDescriptionReferencesColors[i].layout     = VK_IMAGE_LAYOUT_GENERAL;
      attachmentDescriptionReferencesColors[i].aspectMask = 0;
    }
    struct VkAttachmentReference2KHR attachmentDescriptionReferencesDepthStencil[1];                // VkAttachmentDescriptionReference
    if (depthStencilEnable == 1) {
      attachmentDescriptionReferencesDepthStencil[0].sType      = 1000109001; // VK_STRUCTURE_TYPE_ATTACHMENT_REFERENCE_2_KHR
      attachmentDescriptionReferencesDepthStencil[0].pNext      = NULL;
      attachmentDescriptionReferencesDepthStencil[0].attachment = colorsCount;                      // attachmentDescriptionIndex
      attachmentDescriptionReferencesDepthStencil[0].layout     = VK_IMAGE_LAYOUT_GENERAL;
      attachmentDescriptionReferencesDepthStencil[0].aspectMask = 0;
    }
    struct VkAttachmentReference2KHR attachmentDescriptionReferencesResolveColors[8];               // VkAttachmentDescriptionReference
    for (unsigned i = 0; i < colorsCount; i += 1) {
      attachmentDescriptionReferencesResolveColors[i].sType      = 1000109001; // VK_STRUCTURE_TYPE_ATTACHMENT_REFERENCE_2_KHR
      attachmentDescriptionReferencesResolveColors[i].pNext      = NULL;
      attachmentDescriptionReferencesResolveColors[i].attachment = attachmentDescriptionsCount + i; // attachmentDescriptionIndex
      attachmentDescriptionReferencesResolveColors[i].layout     = VK_IMAGE_LAYOUT_GENERAL;
      attachmentDescriptionReferencesResolveColors[i].aspectMask = 0;
    }
    struct VkAttachmentReference2KHR attachmentDescriptionReferencesResolveDepthStencil[1];         // VkAttachmentDescriptionReference
    if (outputDeclarationMembersResolveSources != NULL) {
      if (depthStencilEnable == 1) {
        attachmentDescriptionReferencesResolveDepthStencil[0].sType      = 1000109001; // VK_STRUCTURE_TYPE_ATTACHMENT_REFERENCE_2_KHR
        attachmentDescriptionReferencesResolveDepthStencil[0].pNext      = NULL;
        attachmentDescriptionReferencesResolveDepthStencil[0].attachment = attachmentDescriptionsCount + (outputDeclarationMembersResolveSources->resolveColors == 1 ? colorsCount : 0); // attachmentDescriptionIndex
        attachmentDescriptionReferencesResolveDepthStencil[0].layout     = VK_IMAGE_LAYOUT_GENERAL;
        attachmentDescriptionReferencesResolveDepthStencil[0].aspectMask = 0;
      }
    }
    RedBool32 resolveColors = 0;
    RedBool32 resolveDepthStencil = 0;
    if (outputDeclarationMembersResolveSources != NULL) {
      if (outputDeclarationMembersResolveSources->resolveColors == 1) {
        resolveColors = 1;
      }
      if (outputDeclarationMembersResolveSources->resolveDepthStencil == 1) {
        if (depthStencilEnable == 1) {
          resolveDepthStencil = 1;
        }
      }
    }
    if (outputDeclarationMembersResolveSources != NULL) {
      if (outputDeclarationMembersResolveSources->resolveColors == 1) {
        attachmentDescriptionsCount += colorsCount;
      }
      if (outputDeclarationMembersResolveSources->resolveDepthStencil == 1) {
        if (depthStencilEnable == 1) {
          attachmentDescriptionsCount += 1;
        }
      }
    }
    struct VkSubpassDescriptionDepthStencilResolveKHR subpassDepthStencilResolve;
    struct VkSubpassDescription2KHR subpass;
    subpass.sType                   = 1000109002; // VK_STRUCTURE_TYPE_SUBPASS_DESCRIPTION_2_KHR
    subpass.pNext                   = NULL;
    subpass.flags                   = 0;
    subpass.pipelineBindPoint       = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.viewMask                = 0;
    subpass.inputAttachmentCount    = 0;
    subpass.pInputAttachments       = NULL;
    subpass.colorAttachmentCount    = colorsCount;
    subpass.pColorAttachments       = colorsCount > 0         ? attachmentDescriptionReferencesColors        : NULL;
    subpass.pResolveAttachments     = resolveColors      == 1 ? attachmentDescriptionReferencesResolveColors : NULL;
    subpass.pDepthStencilAttachment = depthStencilEnable == 1 ? attachmentDescriptionReferencesDepthStencil  : NULL;
    subpass.preserveAttachmentCount = 0;
    subpass.pPreserveAttachments    = NULL;
    if (resolveDepthStencil == 1) {
      subpassDepthStencilResolve.sType                          = 1000199001; // VK_STRUCTURE_TYPE_SUBPASS_DESCRIPTION_DEPTH_STENCIL_RESOLVE_KHR
      subpassDepthStencilResolve.pNext                          = NULL;
      subpassDepthStencilResolve.depthResolveMode               = outputDeclarationMembersResolveSources->resolveModeDepth;
      subpassDepthStencilResolve.stencilResolveMode             = outputDeclarationMembersResolveSources->resolveModeStencil;
      subpassDepthStencilResolve.pDepthStencilResolveAttachment = attachmentDescriptionReferencesResolveDepthStencil;
      subpass.pNext = &subpassDepthStencilResolve;
    }
    struct VkSubpassDependency2KHR selfDependency;
    selfDependency.sType           = 1000109003; // VK_STRUCTURE_TYPE_SUBPASS_DEPENDENCY_2_KHR
    selfDependency.pNext           = NULL;
    selfDependency.srcSubpass      = 0;
    selfDependency.dstSubpass      = 0;
    selfDependency.srcStageMask    = dependencyByRegionAllowUsageOrderBarriers == 1 ? (VkPipelineStageFlags)(RED_VK_ACCESS_STAGE_BITFLAG_OUTPUT_DEPTH_STENCIL | RED_VK_ACCESS_STAGE_BITFLAG_OUTPUT_COLOR) : (VkPipelineStageFlags)(VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT);
    selfDependency.dstStageMask    = dependencyByRegionAllowUsageOrderBarriers == 1 ? (VkPipelineStageFlags)(RED_VK_ACCESS_STAGE_BITFLAG_OUTPUT_DEPTH_STENCIL | RED_VK_ACCESS_STAGE_BITFLAG_OUTPUT_COLOR) : (VkPipelineStageFlags)(VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT);
    selfDependency.srcAccessMask   = dependencyByRegionAllowUsageOrderBarriers == 1 ? (VkAccessFlags)(RED_VK_ACCESS_BITFLAG_OUTPUT_DEPTH_STENCIL | RED_VK_ACCESS_BITFLAG_OUTPUT_COLOR) : 0;
    selfDependency.dstAccessMask   = dependencyByRegionAllowUsageOrderBarriers == 1 ? (VkAccessFlags)(RED_VK_ACCESS_BITFLAG_OUTPUT_DEPTH_STENCIL | RED_VK_ACCESS_BITFLAG_OUTPUT_COLOR) : 0;
    selfDependency.dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;
    selfDependency.viewOffset      = 0;
    struct VkRenderPassCreateInfo2KHR info;
    info.sType                   = 1000109004; // VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO_2_KHR
    info.pNext                   = NULL;
    info.flags                   = 0;
    info.attachmentCount         = attachmentDescriptionsCount;
    info.pAttachments            = attachmentDescriptions;
    info.subpassCount            = 1;
    info.pSubpasses              = &subpass;
    info.dependencyCount         = dependencyByRegion == 1 ? 1 : 0;
    info.pDependencies           = dependencyByRegion == 1 ? &selfDependency : NULL;
    info.correlatedViewMaskCount = 0;
    info.pCorrelatedViewMasks    = NULL;
    redInternalSetStatus(ctx, gpu, outStatuses, 272, vk->vkCreateRenderPass2KHR((VkDevice)gpu, &info, NULL, (VkRenderPass *)outOutputDeclaration), procedureId, optionalFile, optionalLine, optionalUserData);
  } else {
    VkAttachmentDescription attachmentDescriptions[8 + 1 + 8 + 1];
    for (unsigned i = 0; i < colorsCount; i += 1) {
      attachmentDescriptions[i].flags          = outputDeclarationMembers->colorsSharesMemoryWithAnotherMember[i] == 1 ? VK_ATTACHMENT_DESCRIPTION_MAY_ALIAS_BIT : 0;
      attachmentDescriptions[i].format         = (VkFormat)outputDeclarationMembers->colorsFormat[i];
      attachmentDescriptions[i].samples        = (VkSampleCountFlagBits)outputDeclarationMembers->colorsMultisampleCount[i];
      attachmentDescriptions[i].loadOp         = (VkAttachmentLoadOp)outputDeclarationMembers->colorsSetProcedureOutputOp[i];
      attachmentDescriptions[i].storeOp        = (VkAttachmentStoreOp)outputDeclarationMembers->colorsEndProcedureOutputOp[i];
      attachmentDescriptions[i].stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_LOAD;
      attachmentDescriptions[i].stencilStoreOp = VK_ATTACHMENT_STORE_OP_STORE;
      attachmentDescriptions[i].initialLayout  = VK_IMAGE_LAYOUT_GENERAL;
      attachmentDescriptions[i].finalLayout    = VK_IMAGE_LAYOUT_GENERAL;
    }
    if (depthStencilEnable == 1) {
      attachmentDescriptions[colorsCount].flags          = outputDeclarationMembers->depthStencilSharesMemoryWithAnotherMember == 1 ? VK_ATTACHMENT_DESCRIPTION_MAY_ALIAS_BIT : 0;
      attachmentDescriptions[colorsCount].format         = (VkFormat)outputDeclarationMembers->depthStencilFormat;
      attachmentDescriptions[colorsCount].samples        = (VkSampleCountFlagBits)outputDeclarationMembers->depthStencilMultisampleCount;
      attachmentDescriptions[colorsCount].loadOp         = (VkAttachmentLoadOp)outputDeclarationMembers->depthStencilDepthSetProcedureOutputOp;
      attachmentDescriptions[colorsCount].storeOp        = (VkAttachmentStoreOp)outputDeclarationMembers->depthStencilDepthEndProcedureOutputOp;
      attachmentDescriptions[colorsCount].stencilLoadOp  = (VkAttachmentLoadOp)outputDeclarationMembers->depthStencilStencilSetProcedureOutputOp;
      attachmentDescriptions[colorsCount].stencilStoreOp = (VkAttachmentStoreOp)outputDeclarationMembers->depthStencilStencilEndProcedureOutputOp;
      attachmentDescriptions[colorsCount].initialLayout  = VK_IMAGE_LAYOUT_GENERAL;
      attachmentDescriptions[colorsCount].finalLayout    = VK_IMAGE_LAYOUT_GENERAL;
    }
    unsigned attachmentDescriptionsCount = colorsCount;
    if (depthStencilEnable == 1) {
      attachmentDescriptionsCount += 1;
    }
    if (outputDeclarationMembersResolveSources != NULL) {
      if (outputDeclarationMembersResolveSources->resolveColors == 1) {
        for (unsigned i = 0; i < colorsCount; i += 1) {
          const unsigned index = attachmentDescriptionsCount + i;
          attachmentDescriptions[index].flags          = outputDeclarationMembers->colorsSharesMemoryWithAnotherMember[i] == 1 ? VK_ATTACHMENT_DESCRIPTION_MAY_ALIAS_BIT : 0;
          attachmentDescriptions[index].format         = (VkFormat)outputDeclarationMembers->colorsFormat[i];
          attachmentDescriptions[index].samples        = VK_SAMPLE_COUNT_1_BIT;
          attachmentDescriptions[index].loadOp         = VK_ATTACHMENT_LOAD_OP_LOAD;
          attachmentDescriptions[index].storeOp        = VK_ATTACHMENT_STORE_OP_STORE;
          attachmentDescriptions[index].stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_LOAD;
          attachmentDescriptions[index].stencilStoreOp = VK_ATTACHMENT_STORE_OP_STORE;
          attachmentDescriptions[index].initialLayout  = VK_IMAGE_LAYOUT_GENERAL;
          attachmentDescriptions[index].finalLayout    = VK_IMAGE_LAYOUT_GENERAL;
        }
      }
    }
    VkAttachmentReference attachmentDescriptionReferencesColors[8];                                 // VkAttachmentDescriptionReference
    for (unsigned i = 0; i < colorsCount; i += 1) {
      attachmentDescriptionReferencesColors[i].attachment = i;                                      // attachmentDescriptionIndex
      attachmentDescriptionReferencesColors[i].layout     = VK_IMAGE_LAYOUT_GENERAL;
    }
    VkAttachmentReference attachmentDescriptionReferencesDepthStencil[1];                           // VkAttachmentDescriptionReference
    if (depthStencilEnable == 1) {
      attachmentDescriptionReferencesDepthStencil[0].attachment = colorsCount;                      // attachmentDescriptionIndex
      attachmentDescriptionReferencesDepthStencil[0].layout     = VK_IMAGE_LAYOUT_GENERAL;
    }
    VkAttachmentReference attachmentDescriptionReferencesResolveColors[8];                          // VkAttachmentDescriptionReference
    for (unsigned i = 0; i < colorsCount; i += 1) {
      attachmentDescriptionReferencesResolveColors[i].attachment = attachmentDescriptionsCount + i; // attachmentDescriptionIndex
      attachmentDescriptionReferencesResolveColors[i].layout     = VK_IMAGE_LAYOUT_GENERAL;
    }
    RedBool32 resolveColors = 0;
    if (outputDeclarationMembersResolveSources != NULL) {
      if (outputDeclarationMembersResolveSources->resolveColors == 1) {
        resolveColors = 1;
      }
    }
    if (outputDeclarationMembersResolveSources != NULL) {
      if (outputDeclarationMembersResolveSources->resolveColors == 1) {
        attachmentDescriptionsCount += colorsCount;
      }
    }
    VkSubpassDescription subpass;
    subpass.flags                   = 0;
    subpass.pipelineBindPoint       = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.inputAttachmentCount    = 0;
    subpass.pInputAttachments       = NULL;
    subpass.colorAttachmentCount    = colorsCount;
    subpass.pColorAttachments       = colorsCount > 0         ? attachmentDescriptionReferencesColors        : NULL;
    subpass.pResolveAttachments     = resolveColors      == 1 ? attachmentDescriptionReferencesResolveColors : NULL;
    subpass.pDepthStencilAttachment = depthStencilEnable == 1 ? attachmentDescriptionReferencesDepthStencil  : NULL;
    subpass.preserveAttachmentCount = 0;
    subpass.pPreserveAttachments    = NULL;
    VkSubpassDependency selfDependency;
    selfDependency.srcSubpass      = 0;
    selfDependency.dstSubpass      = 0;
    selfDependency.srcStageMask    = dependencyByRegionAllowUsageOrderBarriers == 1 ? (VkPipelineStageFlags)(RED_VK_ACCESS_STAGE_BITFLAG_OUTPUT_DEPTH_STENCIL | RED_VK_ACCESS_STAGE_BITFLAG_OUTPUT_COLOR) : (VkPipelineStageFlags)(VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT);
    selfDependency.dstStageMask    = dependencyByRegionAllowUsageOrderBarriers == 1 ? (VkPipelineStageFlags)(RED_VK_ACCESS_STAGE_BITFLAG_OUTPUT_DEPTH_STENCIL | RED_VK_ACCESS_STAGE_BITFLAG_OUTPUT_COLOR) : (VkPipelineStageFlags)(VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT);
    selfDependency.srcAccessMask   = dependencyByRegionAllowUsageOrderBarriers == 1 ? (VkAccessFlags)(RED_VK_ACCESS_BITFLAG_OUTPUT_DEPTH_STENCIL | RED_VK_ACCESS_BITFLAG_OUTPUT_COLOR) : 0;
    selfDependency.dstAccessMask   = dependencyByRegionAllowUsageOrderBarriers == 1 ? (VkAccessFlags)(RED_VK_ACCESS_BITFLAG_OUTPUT_DEPTH_STENCIL | RED_VK_ACCESS_BITFLAG_OUTPUT_COLOR) : 0;
    selfDependency.dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;
    VkRenderPassCreateInfo info;
    info.sType           = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    info.pNext           = NULL;
    info.flags           = 0;
    info.attachmentCount = attachmentDescriptionsCount;
    info.pAttachments    = attachmentDescriptions;
    info.subpassCount    = 1;
    info.pSubpasses      = &subpass;
    info.dependencyCount = dependencyByRegion == 1 ? 1 : 0;
    info.pDependencies   = dependencyByRegion == 1 ? &selfDependency : NULL;
    redInternalSetStatus(ctx, gpu, outStatuses, 273, vk->vkCreateRenderPass((VkDevice)gpu, &info, NULL, (VkRenderPass *)outOutputDeclaration), procedureId, optionalFile, optionalLine, optionalUserData);
  }

  if (ctx->internal.debugUtilsMessengerCallback != NULL && handleName != NULL) {
    if (outOutputDeclaration[0] != NULL) {
      VkDebugUtilsObjectNameInfoEXT info;
      info.sType        = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT;
      info.pNext        = NULL;
      info.objectType   = (VkObjectType)RED_HANDLE_TYPE_OUTPUT_DECLARATION;
      info.objectHandle = (uint64_t)outOutputDeclaration[0];
      info.pObjectName  = handleName;
      redInternalSetStatus(ctx, gpu, outStatuses, 274, vk->vkSetDebugUtilsObjectNameEXT((VkDevice)gpu, &info), procedureId, optionalFile, optionalLine, optionalUserData);
    }
  }
}

static inline void redInlineCreateStructDeclaration(RedContext context, RedHandleGpu gpu, const char * handleName, unsigned structDeclarationMembersCount, const RedStructDeclarationMember * structDeclarationMembers, RedBool32 procedureParametersHandlesDeclaration, RedHandleStructDeclaration * outStructDeclaration, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData) {
  const RedProcedureId procedureId = RED_PROCEDURE_ID_redCreateStructDeclaration;

  RedInternalContext ctx = (RedInternalContext)context;
  RedInternalInstanceProcedureAddresses * vk = ctx->internal.instanceProcedureAddresses;

  unsigned gpuIndex = (unsigned)-1;
  for (unsigned i = 0; i < ctx->internal.hotGpuHandlesCount; i += 1) {
    if (ctx->internal.hotGpuHandles[i] == gpu) {
      gpuIndex = i;
      break;
    }
  }

  if (gpuIndex == (unsigned)-1) {
    redInternalSetStatus(ctx, gpu, outStatuses, 275, (VkResult)RED_STATUS_ERROR_INITIALIZATION_FAILED, procedureId, optionalFile, optionalLine, optionalUserData);
    return;
  }

  const unsigned  gpuVendorId       = ctx->gpus[gpuIndex].gpuVendorId;
  const RedBool32 useDynamicBuffers = (gpuVendorId == 0x1002 || gpuVendorId == 0x1022) ? 1 : 0;
  unsigned        arrayROsCount     = 0;
  unsigned        arrayRWsCount     = 0;

  VkDescriptorSetLayoutCreateInfo info;
  info.sType        = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
  info.pNext        = NULL;
  info.flags        = procedureParametersHandlesDeclaration == 1 ? 1 : 0;
  info.bindingCount = structDeclarationMembersCount;
  info.pBindings    = (const VkDescriptorSetLayoutBinding *)structDeclarationMembers;
  redInternalSetStatus(ctx, gpu, outStatuses, 277, vk->vkCreateDescriptorSetLayout((VkDevice)gpu, &info, NULL, (VkDescriptorSetLayout *)outStructDeclaration), procedureId, optionalFile, optionalLine, optionalUserData);

  if (ctx->internal.debugUtilsMessengerCallback != NULL && handleName != NULL) {
    if (outStructDeclaration[0] != NULL) {
      VkDebugUtilsObjectNameInfoEXT info;
      info.sType        = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT;
      info.pNext        = NULL;
      info.objectType   = (VkObjectType)RED_HANDLE_TYPE_STRUCT_DECLARATION;
      info.objectHandle = (uint64_t)outStructDeclaration[0];
      info.pObjectName  = handleName;
      redInternalSetStatus(ctx, gpu, outStatuses, 278, vk->vkSetDebugUtilsObjectNameEXT((VkDevice)gpu, &info), procedureId, optionalFile, optionalLine, optionalUserData);
    }
  }
}

static inline void redInlineCreateProcedureParameters(RedContext context, RedHandleGpu gpu, const char * handleName, const RedProcedureParametersDeclaration * procedureParametersDeclaration, RedHandleProcedureParameters * outProcedureParameters, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData) {
  const RedProcedureId procedureId = RED_PROCEDURE_ID_redCreateProcedureParameters;

  RedInternalContext ctx = (RedInternalContext)context;
  RedInternalInstanceProcedureAddresses * vk = ctx->internal.instanceProcedureAddresses;

  unsigned gpuIndex = (unsigned)-1;
  for (unsigned i = 0; i < ctx->internal.hotGpuHandlesCount; i += 1) {
    if (ctx->internal.hotGpuHandles[i] == gpu) {
      gpuIndex = i;
      break;
    }
  }

  if (gpuIndex == (unsigned)-1) {
    redInternalSetStatus(ctx, gpu, outStatuses, 280, (VkResult)RED_STATUS_ERROR_INITIALIZATION_FAILED, procedureId, optionalFile, optionalLine, optionalUserData);
    return;
  }

  {
    unsigned allStructsDeclarationsCount = (procedureParametersDeclaration == NULL ? 0 : procedureParametersDeclaration->structsDeclarationsCount) + (procedureParametersDeclaration == NULL ? 0 : (procedureParametersDeclaration->handlesDeclaration != NULL ? 1 : 0));
    if (procedureParametersDeclaration != NULL) {
      if (procedureParametersDeclaration->handlesDeclaration != NULL) {
        allStructsDeclarationsCount += 1;
      }
    }
    if (allStructsDeclarationsCount > 8) {
      redInternalSetStatus(ctx, gpu, outStatuses, 281, (VkResult)RED_STATUS_ERROR_OUT_OF_GPU_MEMORY, procedureId, optionalFile, optionalLine, optionalUserData);
      return;
    }
  }

  unsigned                   structsDeclarationsCount = procedureParametersDeclaration == NULL ? 0 : procedureParametersDeclaration->structsDeclarationsCount;
  RedHandleStructDeclaration structsDeclarations[8];
  for (unsigned i = 0; i < structsDeclarationsCount; i += 1) {
    structsDeclarations[i] = procedureParametersDeclaration->structsDeclarations[i];
  }
  if (procedureParametersDeclaration != NULL) {
    if (procedureParametersDeclaration->handlesDeclaration != NULL) {
      structsDeclarations[structsDeclarationsCount] = procedureParametersDeclaration->handlesDeclaration;
      structsDeclarationsCount += 1;
    }
  }

  VkPushConstantRange constants = {0};
  if (procedureParametersDeclaration != NULL) {
    constants.stageFlags = (VkShaderStageFlags)procedureParametersDeclaration->variablesVisibleToStages;
    constants.offset     = 0;
    constants.size       = procedureParametersDeclaration->variablesBytesCount;
  }
  VkPipelineLayoutCreateInfo info;
  info.sType                  = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
  info.pNext                  = NULL;
  info.flags                  = 0;
  info.setLayoutCount         = structsDeclarationsCount;
  info.pSetLayouts            = structsDeclarationsCount == 0 ? NULL : (const VkDescriptorSetLayout *)structsDeclarations;
  info.pushConstantRangeCount = procedureParametersDeclaration == NULL ? 0    : (procedureParametersDeclaration->variablesVisibleToStages == 0 ? 0    : 1);
  info.pPushConstantRanges    = procedureParametersDeclaration == NULL ? NULL : (procedureParametersDeclaration->variablesVisibleToStages == 0 ? NULL : &constants);
  redInternalSetStatus(ctx, gpu, outStatuses, 282, vk->vkCreatePipelineLayout((VkDevice)gpu, &info, NULL, (VkPipelineLayout *)outProcedureParameters), procedureId, optionalFile, optionalLine, optionalUserData);

  if (ctx->internal.debugUtilsMessengerCallback != NULL && handleName != NULL) {
    if (outProcedureParameters[0] != NULL) {
      VkDebugUtilsObjectNameInfoEXT info;
      info.sType        = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT;
      info.pNext        = NULL;
      info.objectType   = (VkObjectType)RED_HANDLE_TYPE_PROCEDURE_PARAMETERS;
      info.objectHandle = (uint64_t)outProcedureParameters[0];
      info.pObjectName  = handleName;
      redInternalSetStatus(ctx, gpu, outStatuses, 283, vk->vkSetDebugUtilsObjectNameEXT((VkDevice)gpu, &info), procedureId, optionalFile, optionalLine, optionalUserData);
    }
  }
}

static inline void redInlineCreateProcedureCache(RedContext context, RedHandleGpu gpu, const char * handleName, uint64_t fromBlobBytesCount, const void * fromBlob, RedHandleProcedureCache * outProcedureCache, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData) {
  const RedProcedureId procedureId = RED_PROCEDURE_ID_redCreateProcedureCache;

  RedInternalContext ctx = (RedInternalContext)context;
  RedInternalInstanceProcedureAddresses * vk = ctx->internal.instanceProcedureAddresses;

  VkPipelineCacheCreateInfo info;
  info.sType           = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
  info.pNext           = NULL;
  info.flags           = 0;
  info.initialDataSize = (size_t)fromBlobBytesCount;
  info.pInitialData    = fromBlob;
  redInternalSetStatus(ctx, gpu, outStatuses, 286, vk->vkCreatePipelineCache((VkDevice)gpu, &info, NULL, (VkPipelineCache *)outProcedureCache), procedureId, optionalFile, optionalLine, optionalUserData);

  if (ctx->internal.debugUtilsMessengerCallback != NULL && handleName != NULL) {
    if (outProcedureCache[0] != NULL) {
      VkDebugUtilsObjectNameInfoEXT info;
      info.sType        = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT;
      info.pNext        = NULL;
      info.objectType   = (VkObjectType)RED_HANDLE_TYPE_PROCEDURE_CACHE;
      info.objectHandle = (uint64_t)outProcedureCache[0];
      info.pObjectName  = handleName;
      redInternalSetStatus(ctx, gpu, outStatuses, 287, vk->vkSetDebugUtilsObjectNameEXT((VkDevice)gpu, &info), procedureId, optionalFile, optionalLine, optionalUserData);
    }
  }
}

static inline void redInlineCreateProcedure(RedContext context, RedHandleGpu gpu, const char * handleName, RedHandleProcedureCache procedureCache, RedHandleOutputDeclaration outputDeclaration, RedHandleProcedureParameters procedureParameters, const char * gpuCodeVertexMainProcedureName, RedHandleGpuCode gpuCodeVertex, const char * gpuCodeFragmentMainProcedureName, RedHandleGpuCode gpuCodeFragment, const RedProcedureState * state, const void * stateExtension, RedBool32 deriveBase, RedHandleProcedure deriveFrom, RedHandleProcedure * outProcedure, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData) {
  const RedProcedureId procedureId = RED_PROCEDURE_ID_redCreateProcedure;

  RedInternalContext ctx = (RedInternalContext)context;
  RedInternalInstanceProcedureAddresses * vk = ctx->internal.instanceProcedureAddresses;

  RedHandleProcedureParameters procedureParametersHandle = procedureParameters;

  unsigned gpuIndex = (unsigned)-1;
  for (unsigned i = 0; i < ctx->internal.hotGpuHandlesCount; i += 1) {
    if (ctx->internal.hotGpuHandles[i] == gpu) {
      gpuIndex = i;
      break;
    }
  }

  if (gpuIndex == (unsigned)-1) {
    redInternalSetStatus(ctx, gpu, outStatuses, 288, (VkResult)RED_STATUS_ERROR_INITIALIZATION_FAILED, procedureId, optionalFile, optionalLine, optionalUserData);
    return;
  }

  struct {
    struct {
      unsigned     _;
      void *       __;
      unsigned     ___;
      unsigned     stage;
      RedHandleGpuCode module;
      const char * name;
      void *       specializationInfo;
    }              stages[2];
    struct {
      unsigned     _;
      void *       __;
      unsigned     ___;
      unsigned     vertexBindingDescriptionsCount;
      void *       vertexBindingDescriptions;
      unsigned     vertexAttributeDescriptionsCount;
      void *       vertexAttributeDescriptions;
    }              vertexInputState;
    struct {
      unsigned     _;
      void *       __;
      unsigned     ___;
      unsigned     topology;
      RedBool32    primitiveRestartEnable;
    }              inputAssemblyState;
    struct {
      unsigned     _;
      void *       __;
      unsigned     ___;
      unsigned     viewportsCount;
      void *       viewports;
      unsigned     scissorsCount;
      void *       scissors;
    }              viewportState;
    VkViewport     flippedViewport;
    VkRect2D       scissor;
    struct {
      unsigned     _;
      void *       __;
      unsigned     ___;
      RedBool32    depthClampEnable;
      RedBool32    rasterizerDiscardEnable;
      unsigned     polygonMode;
      unsigned     cullMode;
      unsigned     frontFace;
      RedBool32    depthBiasEnable;
      float        depthBiasConstantFactor;
      float        depthBiasClamp;
      float        depthBiasSlopeFactor;
      float        lineWidth;
    }              rasterizationState;
    struct {
      unsigned     _;
      void *       __;
      unsigned     ___;
      unsigned     rasterizationSamples;
      RedBool32    sampleShadingEnable;
      float        minSampleShading;
      const unsigned * sampleMask;
      RedBool32    alphaToCoverageEnable;
      RedBool32    alphaToOneEnable;
    }              multisampleState;
    struct {
      unsigned     _;
      void *       __;
      unsigned     ___;
      RedBool32    depthTestEnable;
      RedBool32    depthWriteEnable;
      unsigned     depthCompareOp;
      RedBool32    depthBoundsTestEnable;
      RedBool32    stencilTestEnable;
      struct {
        unsigned   frontFailOp;
        unsigned   frontPassOp;
        unsigned   frontDepthFailOp;
        unsigned   frontCompareOp;
        unsigned   frontCompareMask;
        unsigned   frontWriteMask;
        unsigned   frontReference;
      };
      struct {
        unsigned   backFailOp;
        unsigned   backPassOp;
        unsigned   backDepthFailOp;
        unsigned   backCompareOp;
        unsigned   backCompareMask;
        unsigned   backWriteMask;
        unsigned   backReference;
      };
      float        minDepthBounds;
      float        maxDepthBounds;
    }              depthStencilState;
    struct {
      unsigned     _;
      void *       __;
      unsigned     ___;
      RedBool32    logicOpEnable;
      unsigned     logicOp;
      unsigned     attachmentsCount;
      void *       attachments;
      float        blendConstants[4];
    }              colorBlendState;
    VkPipelineColorBlendAttachmentState attachments[8];
    struct {
      unsigned     _;
      void *       __;
      unsigned     ___;
      unsigned     dynamicStatesCount;
      unsigned *   dynamicStates;
    }              dynamicState;
    unsigned       dynamicStates[8];
  } metadata;
  struct {
    unsigned       _;
    void *         __;
    unsigned       ___;
    unsigned       conservativeRasterizationMode;
    float          extraPrimitiveOverestimationSize;
  } conservativeRasterization;
  conservativeRasterization._                                = 1000101001; // VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_CONSERVATIVE_STATE_CREATE_INFO_EXT
  conservativeRasterization.__                               = NULL;
  conservativeRasterization.___                              = 0;
  conservativeRasterization.conservativeRasterizationMode    = RED_RASTERIZATION_MODE_DEFAULT;
  conservativeRasterization.extraPrimitiveOverestimationSize = 0;
  while (stateExtension != NULL) {
    const RedProcedureStateExtensionIterator * stateExtensionIterator = (const RedProcedureStateExtensionIterator *)stateExtension;
    if (stateExtensionIterator->extension == RED_PROCEDURE_STATE_EXTENSION_RASTERIZATION_MODE) {
      const RedProcedureStateExtensionRasterizationMode * state = (const RedProcedureStateExtensionRasterizationMode *)stateExtension;
      conservativeRasterization.conservativeRasterizationMode = state->rasterizationMode;
    }
    const void * nextStateExtension = stateExtensionIterator->next;
    stateExtension = nextStateExtension;
  }
  metadata.flippedViewport.x        = state->viewportStaticX;
  metadata.flippedViewport.y        = state->viewportStaticY + state->viewportStaticHeight;
  metadata.flippedViewport.width    = state->viewportStaticWidth;
  metadata.flippedViewport.height   =-state->viewportStaticHeight;
  metadata.flippedViewport.minDepth = state->viewportStaticDepthMin;
  metadata.flippedViewport.maxDepth = state->viewportStaticDepthMax;
  metadata.scissor.offset.x         = state->scissorStaticX;
  metadata.scissor.offset.y         = state->scissorStaticY;
  metadata.scissor.extent.width     = state->scissorStaticWidth;
  metadata.scissor.extent.height    = state->scissorStaticHeight;
  for (unsigned j = 0; j < state->outputColorsCount; j += 1) {
    metadata.attachments[j].blendEnable         = state->outputColorsBlendEnable[j];
    metadata.attachments[j].srcColorBlendFactor = (VkBlendFactor)state->outputColorsBlendColorFactorSource[j];
    metadata.attachments[j].dstColorBlendFactor = (VkBlendFactor)state->outputColorsBlendColorFactorTarget[j];
    metadata.attachments[j].colorBlendOp        = (VkBlendOp)state->outputColorsBlendColorOp[j];
    metadata.attachments[j].srcAlphaBlendFactor = (VkBlendFactor)state->outputColorsBlendAlphaFactorSource[j];
    metadata.attachments[j].dstAlphaBlendFactor = (VkBlendFactor)state->outputColorsBlendAlphaFactorTarget[j];
    metadata.attachments[j].alphaBlendOp        = (VkBlendOp)state->outputColorsBlendAlphaOp[j];
    metadata.attachments[j].colorWriteMask      = state->outputColorsWriteMask[j];
  }
  unsigned dynamicStatesCount = 0;
  if (state->viewportDynamic                           == 1) { metadata.dynamicStates[dynamicStatesCount] = VK_DYNAMIC_STATE_VIEWPORT;             dynamicStatesCount += 1; }
  if (state->scissorDynamic                            == 1) { metadata.dynamicStates[dynamicStatesCount] = VK_DYNAMIC_STATE_SCISSOR;              dynamicStatesCount += 1; }
  if (state->rasterizationDepthBiasDynamic             == 1) { metadata.dynamicStates[dynamicStatesCount] = VK_DYNAMIC_STATE_DEPTH_BIAS;           dynamicStatesCount += 1; }
  if (state->depthTestBoundsTestDynamic                == 1) { metadata.dynamicStates[dynamicStatesCount] = VK_DYNAMIC_STATE_DEPTH_BOUNDS;         dynamicStatesCount += 1; }
  if (state->stencilTestFrontAndBackDynamicCompareMask == 1) { metadata.dynamicStates[dynamicStatesCount] = VK_DYNAMIC_STATE_STENCIL_COMPARE_MASK; dynamicStatesCount += 1; }
  if (state->stencilTestFrontAndBackDynamicWriteMask   == 1) { metadata.dynamicStates[dynamicStatesCount] = VK_DYNAMIC_STATE_STENCIL_WRITE_MASK;   dynamicStatesCount += 1; }
  if (state->stencilTestFrontAndBackDynamicReference   == 1) { metadata.dynamicStates[dynamicStatesCount] = VK_DYNAMIC_STATE_STENCIL_REFERENCE;    dynamicStatesCount += 1; }
  if (state->blendConstantsDynamic                     == 1) { metadata.dynamicStates[dynamicStatesCount] = VK_DYNAMIC_STATE_BLEND_CONSTANTS;      dynamicStatesCount += 1; }
  metadata.stages[0]._                                       = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
  metadata.stages[0].__                                      = NULL;
  metadata.stages[0].___                                     = 0;
  metadata.stages[0].stage                                   = VK_SHADER_STAGE_VERTEX_BIT;
  metadata.stages[0].module                                  = gpuCodeVertex;
  metadata.stages[0].name                                    = gpuCodeVertexMainProcedureName;
  metadata.stages[0].specializationInfo                      = NULL;
  metadata.stages[1]._                                       = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
  metadata.stages[1].__                                      = NULL;
  metadata.stages[1].___                                     = 0;
  metadata.stages[1].stage                                   = VK_SHADER_STAGE_FRAGMENT_BIT;
  metadata.stages[1].module                                  = gpuCodeFragment;
  metadata.stages[1].name                                    = gpuCodeFragmentMainProcedureName;
  metadata.stages[1].specializationInfo                      = NULL;
  metadata.vertexInputState._                                = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
  metadata.vertexInputState.__                               = NULL;
  metadata.vertexInputState.___                              = 0;
  metadata.vertexInputState.vertexBindingDescriptionsCount   = 0;
  metadata.vertexInputState.vertexBindingDescriptions        = NULL;
  metadata.vertexInputState.vertexAttributeDescriptionsCount = 0;
  metadata.vertexInputState.vertexAttributeDescriptions      = NULL;
  metadata.inputAssemblyState._                              = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
  metadata.inputAssemblyState.__                             = NULL;
  metadata.inputAssemblyState.___                            = 0;
  metadata.inputAssemblyState.topology                       = state->inputAssemblyTopology;
  metadata.inputAssemblyState.primitiveRestartEnable         = state->inputAssemblyPrimitiveRestartEnable;
  metadata.viewportState._                                   = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
  metadata.viewportState.__                                  = NULL;
  metadata.viewportState.___                                 = 0;
  metadata.viewportState.viewportsCount                      = 1;
  metadata.viewportState.viewports                           = &metadata.flippedViewport;
  metadata.viewportState.scissorsCount                       = 1;
  metadata.viewportState.scissors                            = &metadata.scissor;
  const RedBool32 useRasterizationMode = redInternalGpuExtensionIsEnabled(ctx, gpuIndex, RED_INTERNAL_GPU_EXTENSION_BITFLAG_RASTERIZATION_MODE);
  metadata.rasterizationState._                              = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
  metadata.rasterizationState.__                             = useRasterizationMode == 1 ? &conservativeRasterization : NULL;
  metadata.rasterizationState.___                            = 0;
  metadata.rasterizationState.depthClampEnable               = state->rasterizationDepthClampEnable;
  metadata.rasterizationState.rasterizerDiscardEnable        = state->rasterizationDiscardAllPrimitivesEnable;
  metadata.rasterizationState.polygonMode                    = VK_POLYGON_MODE_FILL;
  metadata.rasterizationState.cullMode                       = state->rasterizationCullMode;
  metadata.rasterizationState.frontFace                      = state->rasterizationFrontFace;
  metadata.rasterizationState.depthBiasEnable                = state->rasterizationDepthBiasEnable;
  metadata.rasterizationState.depthBiasConstantFactor        = state->rasterizationDepthBiasStaticConstantFactor;
  metadata.rasterizationState.depthBiasClamp                 = state->rasterizationDepthBiasStaticClamp;
  metadata.rasterizationState.depthBiasSlopeFactor           = state->rasterizationDepthBiasStaticSlopeFactor;
  metadata.rasterizationState.lineWidth                      = 1.f;
  metadata.multisampleState._                                = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
  metadata.multisampleState.__                               = NULL;
  metadata.multisampleState.___                              = 0;
  metadata.multisampleState.rasterizationSamples             = state->multisampleCount;
  metadata.multisampleState.sampleShadingEnable              = state->multisampleSampleShadingEnable;
  metadata.multisampleState.minSampleShading                 = state->multisampleSampleShadingMin;
  metadata.multisampleState.sampleMask                       = state->multisampleSampleMask;
  metadata.multisampleState.alphaToCoverageEnable            = state->multisampleAlphaToCoverageEnable;
  metadata.multisampleState.alphaToOneEnable                 = state->multisampleAlphaToOneEnable;
  metadata.depthStencilState._                               = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
  metadata.depthStencilState.__                              = NULL;
  metadata.depthStencilState.___                             = 0;
  metadata.depthStencilState.depthTestEnable                 = state->depthTestEnable;
  metadata.depthStencilState.depthWriteEnable                = state->depthTestDepthWriteEnable;
  metadata.depthStencilState.depthCompareOp                  = state->depthTestDepthCompareOp;
  metadata.depthStencilState.depthBoundsTestEnable           = state->depthTestBoundsTestEnable;
  metadata.depthStencilState.stencilTestEnable               = state->stencilTestEnable;
  metadata.depthStencilState.frontFailOp                     = state->stencilTestFrontStencilTestFailOp;
  metadata.depthStencilState.frontPassOp                     = state->stencilTestFrontStencilTestPassDepthTestPassOp;
  metadata.depthStencilState.frontDepthFailOp                = state->stencilTestFrontStencilTestPassDepthTestFailOp;
  metadata.depthStencilState.frontCompareOp                  = state->stencilTestFrontCompareOp;
  metadata.depthStencilState.frontCompareMask                = state->stencilTestFrontAndBackStaticCompareMask;
  metadata.depthStencilState.frontWriteMask                  = state->stencilTestFrontAndBackStaticWriteMask;
  metadata.depthStencilState.frontReference                  = state->stencilTestFrontAndBackStaticReference;
  metadata.depthStencilState.backFailOp                      = state->stencilTestBackStencilTestFailOp;
  metadata.depthStencilState.backPassOp                      = state->stencilTestBackStencilTestPassDepthTestPassOp;
  metadata.depthStencilState.backDepthFailOp                 = state->stencilTestBackStencilTestPassDepthTestFailOp;
  metadata.depthStencilState.backCompareOp                   = state->stencilTestBackCompareOp;
  metadata.depthStencilState.backCompareMask                 = state->stencilTestFrontAndBackStaticCompareMask;
  metadata.depthStencilState.backWriteMask                   = state->stencilTestFrontAndBackStaticWriteMask;
  metadata.depthStencilState.backReference                   = state->stencilTestFrontAndBackStaticReference;
  metadata.depthStencilState.minDepthBounds                  = state->depthTestBoundsTestStaticMin;
  metadata.depthStencilState.maxDepthBounds                  = state->depthTestBoundsTestStaticMax;
  metadata.colorBlendState._                                 = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
  metadata.colorBlendState.__                                = NULL;
  metadata.colorBlendState.___                               = 0;
  metadata.colorBlendState.logicOpEnable                     = state->blendLogicOpEnable;
  metadata.colorBlendState.logicOp                           = state->blendLogicOp;
  metadata.colorBlendState.attachmentsCount                  = state->outputColorsCount;
  metadata.colorBlendState.attachments                       = metadata.attachments;
  metadata.colorBlendState.blendConstants[0]                 = state->blendConstantsStatic[0];
  metadata.colorBlendState.blendConstants[1]                 = state->blendConstantsStatic[1];
  metadata.colorBlendState.blendConstants[2]                 = state->blendConstantsStatic[2];
  metadata.colorBlendState.blendConstants[3]                 = state->blendConstantsStatic[3];
  metadata.dynamicState._                                    = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
  metadata.dynamicState.__                                   = NULL;
  metadata.dynamicState.___                                  = 0;
  metadata.dynamicState.dynamicStatesCount                   = dynamicStatesCount;
  metadata.dynamicState.dynamicStates                        = metadata.dynamicStates;
  VkPipelineCreateFlags flags = 0;
  flags |= deriveBase == 1    ? VK_PIPELINE_CREATE_ALLOW_DERIVATIVES_BIT : 0;
  flags |= deriveFrom != NULL ? VK_PIPELINE_CREATE_DERIVATIVE_BIT        : 0;
  VkGraphicsPipelineCreateInfo graphicsPipelineCreateInfo;
  graphicsPipelineCreateInfo.sType               = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
  graphicsPipelineCreateInfo.pNext               = NULL;
  graphicsPipelineCreateInfo.flags               = flags;
  graphicsPipelineCreateInfo.stageCount          = (gpuCodeFragmentMainProcedureName == NULL && gpuCodeFragment == NULL) ? 1 : 2;
  graphicsPipelineCreateInfo.pStages             = (VkPipelineShaderStageCreateInfo *)metadata.stages;
  graphicsPipelineCreateInfo.pVertexInputState   = (VkPipelineVertexInputStateCreateInfo *)&metadata.vertexInputState;
  graphicsPipelineCreateInfo.pInputAssemblyState = (VkPipelineInputAssemblyStateCreateInfo *)&metadata.inputAssemblyState;
  graphicsPipelineCreateInfo.pTessellationState  = NULL;
  graphicsPipelineCreateInfo.pViewportState      = (VkPipelineViewportStateCreateInfo *)&metadata.viewportState;
  graphicsPipelineCreateInfo.pRasterizationState = (VkPipelineRasterizationStateCreateInfo *)&metadata.rasterizationState;
  graphicsPipelineCreateInfo.pMultisampleState   = (VkPipelineMultisampleStateCreateInfo *)&metadata.multisampleState;
  graphicsPipelineCreateInfo.pDepthStencilState  = (VkPipelineDepthStencilStateCreateInfo *)&metadata.depthStencilState;
  graphicsPipelineCreateInfo.pColorBlendState    = (VkPipelineColorBlendStateCreateInfo *)&metadata.colorBlendState;
  graphicsPipelineCreateInfo.pDynamicState       = metadata.dynamicState.dynamicStatesCount == 0 ? NULL : (VkPipelineDynamicStateCreateInfo *)&metadata.dynamicState;
  graphicsPipelineCreateInfo.layout              = (VkPipelineLayout)procedureParametersHandle;
  graphicsPipelineCreateInfo.renderPass          = (VkRenderPass)outputDeclaration;
  graphicsPipelineCreateInfo.subpass             = 0;
  graphicsPipelineCreateInfo.basePipelineHandle  = (VkPipeline)deriveFrom;
  graphicsPipelineCreateInfo.basePipelineIndex   = -1;
  redInternalSetStatus(ctx, gpu, outStatuses, 289, vk->vkCreateGraphicsPipelines((VkDevice)gpu, (VkPipelineCache)procedureCache, 1, &graphicsPipelineCreateInfo, NULL, (VkPipeline *)outProcedure), procedureId, optionalFile, optionalLine, optionalUserData);

  if (ctx->internal.debugUtilsMessengerCallback != NULL && handleName != NULL) {
    if (outProcedure[0] != NULL) {
      VkDebugUtilsObjectNameInfoEXT info;
      info.sType        = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT;
      info.pNext        = NULL;
      info.objectType   = (VkObjectType)RED_HANDLE_TYPE_PROCEDURE;
      info.objectHandle = (uint64_t)outProcedure[0];
      info.pObjectName  = handleName;
      redInternalSetStatus(ctx, gpu, outStatuses, 290, vk->vkSetDebugUtilsObjectNameEXT((VkDevice)gpu, &info), procedureId, optionalFile, optionalLine, optionalUserData);
    }
  }
}

static inline void redInlineCreateProcedureCompute(RedContext context, RedHandleGpu gpu, const char * handleName, RedHandleProcedureCache procedureCache, RedHandleProcedureParameters procedureParameters, const char * gpuCodeMainProcedureName, RedHandleGpuCode gpuCode, const void * stateExtension, RedHandleProcedure * outProcedure, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData, RedProcedureId procedureId) {
  RedInternalContext ctx = (RedInternalContext)context;
  RedInternalInstanceProcedureAddresses * vk = ctx->internal.instanceProcedureAddresses;

  RedHandleProcedureParameters procedureParametersHandle = procedureParameters;

  RedBool32            specializationInfoIsPassed = 0;
  VkSpecializationInfo specializationInfo;
  specializationInfo.mapEntryCount = 0;
  specializationInfo.pMapEntries   = NULL;
  specializationInfo.dataSize      = 0;
  specializationInfo.pData         = NULL;
  while (stateExtension != NULL) {
    const RedProcedureComputingLanguageStateExtensionIterator * stateExtensionIterator = (const RedProcedureComputingLanguageStateExtensionIterator *)stateExtension;
    if (stateExtensionIterator->extension == RED_PROCEDURE_COMPUTING_LANGUAGE_STATE_EXTENSION_SPECIALIZATION_CONSTANTS) {
      const RedProcedureComputingLanguageStateExtensionSpecializationConstants * state = (const RedProcedureComputingLanguageStateExtensionSpecializationConstants *)stateExtension;
      specializationInfo.mapEntryCount = state->specsCount;
      specializationInfo.pMapEntries   = (const VkSpecializationMapEntry *)state->specs;
      specializationInfo.dataSize      = state->specDataBytesCount;
      specializationInfo.pData         = state->specData;
      specializationInfoIsPassed       = 1;
    }
    const void * nextStateExtension = stateExtensionIterator->next;
    stateExtension = nextStateExtension;
  }
  VkPipelineShaderStageCreateInfo stage;
  stage.sType               = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
  stage.pNext               = NULL;
  stage.flags               = 0;
  stage.stage               = VK_SHADER_STAGE_COMPUTE_BIT;
  stage.module              = (VkShaderModule)gpuCode;
  stage.pName               = gpuCodeMainProcedureName;
  stage.pSpecializationInfo = specializationInfoIsPassed == 1 ? &specializationInfo : NULL;
  VkComputePipelineCreateInfo computePipelineCreateInfo;
  computePipelineCreateInfo.sType              = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
  computePipelineCreateInfo.pNext              = NULL;
  computePipelineCreateInfo.flags              = 0;
  computePipelineCreateInfo.stage              = stage;
  computePipelineCreateInfo.layout             = (VkPipelineLayout)procedureParametersHandle;
  computePipelineCreateInfo.basePipelineHandle = NULL;
  computePipelineCreateInfo.basePipelineIndex  = -1;
  redInternalSetStatus(ctx, gpu, outStatuses, 291, vk->vkCreateComputePipelines((VkDevice)gpu, (VkPipelineCache)procedureCache, 1, &computePipelineCreateInfo, NULL, (VkPipeline *)outProcedure), procedureId, optionalFile, optionalLine, optionalUserData);

  if (ctx->internal.debugUtilsMessengerCallback != NULL && handleName != NULL) {
    if (outProcedure[0] != NULL) {
      VkDebugUtilsObjectNameInfoEXT info;
      info.sType        = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT;
      info.pNext        = NULL;
      info.objectType   = (VkObjectType)RED_HANDLE_TYPE_PROCEDURE;
      info.objectHandle = (uint64_t)outProcedure[0];
      info.pObjectName  = handleName;
      redInternalSetStatus(ctx, gpu, outStatuses, 292, vk->vkSetDebugUtilsObjectNameEXT((VkDevice)gpu, &info), procedureId, optionalFile, optionalLine, optionalUserData);
    }
  }
}

#ifdef __clang__
static inline __attribute__((always_inline))
#else
static inline
#endif
void redInlineCreateOutput(RedContext context, RedHandleGpu gpu, const char * handleName, RedHandleOutputDeclaration outputDeclaration, const RedOutputMembers * outputMembers, const RedOutputMembersResolveTargets * outputMembersResolveTargets, unsigned width, unsigned height, RedOutput * outOutput, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData) {
  const RedProcedureId procedureId = RED_PROCEDURE_ID_redCreateOutput;

  RedInternalContext ctx = (RedInternalContext)context;
  RedInternalInstanceProcedureAddresses * vk = ctx->internal.instanceProcedureAddresses;

  unsigned gpuIndex = (unsigned)-1;
  for (unsigned i = 0; i < ctx->internal.hotGpuHandlesCount; i += 1) {
    if (ctx->internal.hotGpuHandles[i] == gpu) {
      gpuIndex = i;
      break;
    }
  }

  if (gpuIndex == (unsigned)-1) {
    redInternalSetStatus(ctx, gpu, outStatuses, 293, (VkResult)RED_STATUS_ERROR_INITIALIZATION_FAILED, procedureId, optionalFile, optionalLine, optionalUserData);
    return;
  }

  const unsigned  colorsCount        = outputMembers == NULL ? 0 : (outputMembers->colorsCount);
  const RedBool32 depthStencilEnable = outputMembers == NULL ? 0 : (outputMembers->depthStencil != NULL ? 1 : 0);

  RedHandleTexture attachments[8 + 1 + 8 + 1];
  for (unsigned i = 0; i < colorsCount; i += 1) {
    attachments[i] = outputMembers->colors[i];
  }
  if (depthStencilEnable == 1) {
    attachments[colorsCount] = outputMembers->depthStencil;
  }
  unsigned attachmentsCount = colorsCount;
  if (depthStencilEnable == 1) {
    attachmentsCount += 1;
  }
  unsigned colorsToResolveCount = 0;
  if (outputMembersResolveTargets != NULL) {
    if (outputMembersResolveTargets->colors[0] != NULL) {
      colorsToResolveCount = colorsCount;
    }
  }
  if (outputMembersResolveTargets != NULL) {
    for (unsigned i = 0; i < colorsToResolveCount; i += 1) {
      attachments[attachmentsCount + i] = outputMembersResolveTargets->colors[i];
    }
    if (depthStencilEnable == 1) {
      attachments[attachmentsCount + colorsToResolveCount] = outputMembersResolveTargets->depthStencil;
    }
  }
  if (outputMembersResolveTargets != NULL) {
    attachmentsCount += colorsToResolveCount;
    if (redInternalGpuExtensionIsEnabled(ctx, gpuIndex, RED_INTERNAL_GPU_EXTENSION_BITFLAG_DEPTH_STENCIL_RESOLVE)) {
      if (outputMembersResolveTargets->depthStencil != NULL) {
        if (depthStencilEnable == 1) {
          attachmentsCount += 1;
        }
      }
    }
  }

  VkFramebufferCreateInfo info;
  info.sType           = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
  info.pNext           = NULL;
  info.flags           = 0;
  info.renderPass      = (VkRenderPass)outputDeclaration;
  info.attachmentCount = attachmentsCount;
  info.pAttachments    = (const VkImageView *)attachments;
  info.width           = width;
  info.height          = height;
  info.layers          = 1;
  redInternalSetStatus(ctx, gpu, outStatuses, 294, vk->vkCreateFramebuffer((VkDevice)gpu, &info, NULL, (VkFramebuffer *)&outOutput->handle), procedureId, optionalFile, optionalLine, optionalUserData);

  outOutput->width              = width;
  outOutput->height             = height;
  outOutput->depthStencilEnable = depthStencilEnable;
  outOutput->colorsCount        = colorsCount;

  if (ctx->internal.debugUtilsMessengerCallback != NULL && handleName != NULL) {
    if (outOutput->handle != NULL) {
      VkDebugUtilsObjectNameInfoEXT info;
      info.sType        = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT;
      info.pNext        = NULL;
      info.objectType   = (VkObjectType)RED_HANDLE_TYPE_OUTPUT;
      info.objectHandle = (uint64_t)outOutput->handle;
      info.pObjectName  = handleName;
      redInternalSetStatus(ctx, gpu, outStatuses, 295, vk->vkSetDebugUtilsObjectNameEXT((VkDevice)gpu, &info), procedureId, optionalFile, optionalLine, optionalUserData);
    }
  }
}

static inline void redInlineCreateCpuSignal(RedContext context, RedHandleGpu gpu, const char * handleName, RedBool32 createSignaled, RedHandleCpuSignal * outCpuSignal, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData) {
  const RedProcedureId procedureId = RED_PROCEDURE_ID_redCreateCpuSignal;

  RedInternalContext ctx = (RedInternalContext)context;
  RedInternalInstanceProcedureAddresses * vk = ctx->internal.instanceProcedureAddresses;

  VkFenceCreateInfo info;
  info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
  info.pNext = NULL;
  info.flags = createSignaled == 1 ? VK_FENCE_CREATE_SIGNALED_BIT : 0;
  redInternalSetStatus(ctx, gpu, outStatuses, 296, vk->vkCreateFence((VkDevice)gpu, &info, NULL, (VkFence *)outCpuSignal), procedureId, optionalFile, optionalLine, optionalUserData);

  if (ctx->internal.debugUtilsMessengerCallback != NULL && handleName != NULL) {
    if (outCpuSignal[0] != NULL) {
      VkDebugUtilsObjectNameInfoEXT info;
      info.sType        = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT;
      info.pNext        = NULL;
      info.objectType   = (VkObjectType)RED_HANDLE_TYPE_CPU_SIGNAL;
      info.objectHandle = (uint64_t)outCpuSignal[0];
      info.pObjectName  = handleName;
      redInternalSetStatus(ctx, gpu, outStatuses, 297, vk->vkSetDebugUtilsObjectNameEXT((VkDevice)gpu, &info), procedureId, optionalFile, optionalLine, optionalUserData);
    }
  }
}

static inline void redInlineCreateGpuSignal(RedContext context, RedHandleGpu gpu, const char * handleName, RedHandleGpuSignal * outGpuSignal, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData) {
  const RedProcedureId procedureId = RED_PROCEDURE_ID_redCreateGpuSignal;

  RedInternalContext ctx = (RedInternalContext)context;
  RedInternalInstanceProcedureAddresses * vk = ctx->internal.instanceProcedureAddresses;

  VkSemaphoreCreateInfo info;
  info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
  info.pNext = NULL;
  info.flags = 0;
  redInternalSetStatus(ctx, gpu, outStatuses, 298, vk->vkCreateSemaphore((VkDevice)gpu, &info, NULL, (VkSemaphore *)outGpuSignal), procedureId, optionalFile, optionalLine, optionalUserData);

  if (ctx->internal.debugUtilsMessengerCallback != NULL && handleName != NULL) {
    if (outGpuSignal[0] != NULL) {
      VkDebugUtilsObjectNameInfoEXT info;
      info.sType        = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT;
      info.pNext        = NULL;
      info.objectType   = (VkObjectType)RED_HANDLE_TYPE_GPU_SIGNAL;
      info.objectHandle = (uint64_t)outGpuSignal[0];
      info.pObjectName  = handleName;
      redInternalSetStatus(ctx, gpu, outStatuses, 299, vk->vkSetDebugUtilsObjectNameEXT((VkDevice)gpu, &info), procedureId, optionalFile, optionalLine, optionalUserData);
    }
  }
}

static inline void redInlineCreateGpuToCpuSignal(RedContext context, RedHandleGpu gpu, const char * handleName, RedHandleGpuToCpuSignal * outGpuToCpuSignal, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData) {
  const RedProcedureId procedureId = RED_PROCEDURE_ID_redCreateGpuToCpuSignal;

  RedInternalContext ctx = (RedInternalContext)context;
  RedInternalInstanceProcedureAddresses * vk = ctx->internal.instanceProcedureAddresses;

  VkEventCreateInfo eventInfo;
  eventInfo.sType = VK_STRUCTURE_TYPE_EVENT_CREATE_INFO;
  eventInfo.pNext = NULL;
  eventInfo.flags = 0;
  redInternalSetStatus(ctx, gpu, outStatuses, 300, vk->vkCreateEvent((VkDevice)gpu, &eventInfo, NULL, (VkEvent *)outGpuToCpuSignal), procedureId, optionalFile, optionalLine, optionalUserData);

  if (ctx->internal.debugUtilsMessengerCallback != NULL && handleName != NULL) {
    if (outGpuToCpuSignal[0] != NULL) {
      VkDebugUtilsObjectNameInfoEXT info;
      info.sType        = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT;
      info.pNext        = NULL;
      info.objectType   = (VkObjectType)RED_HANDLE_TYPE_GPU_TO_CPU_SIGNAL;
      info.objectHandle = (uint64_t)outGpuToCpuSignal[0];
      info.pObjectName  = handleName;
      redInternalSetStatus(ctx, gpu, outStatuses, 301, vk->vkSetDebugUtilsObjectNameEXT((VkDevice)gpu, &info), procedureId, optionalFile, optionalLine, optionalUserData);
    }
  }
}

static inline void redInlineCreateCalls(RedContext context, RedHandleGpu gpu, const char * handleName, unsigned queueFamilyIndex, RedCalls * outCalls, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData) {
  const RedProcedureId procedureId = RED_PROCEDURE_ID_redCreateCalls;

  RedInternalContext ctx = (RedInternalContext)context;
  RedInternalInstanceProcedureAddresses * vk = ctx->internal.instanceProcedureAddresses;

  unsigned gpuIndex = (unsigned)-1;
  for (unsigned i = 0; i < ctx->internal.hotGpuHandlesCount; i += 1) {
    if (ctx->internal.hotGpuHandles[i] == gpu) {
      gpuIndex = i;
      break;
    }
  }

  if (gpuIndex == (unsigned)-1) {
    redInternalSetStatus(ctx, gpu, outStatuses, 302, (VkResult)RED_STATUS_ERROR_INITIALIZATION_FAILED, procedureId, optionalFile, optionalLine, optionalUserData);
    goto exit;
  }

  VkCommandPoolCreateInfo commandPoolInfo;
  commandPoolInfo.sType            = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
  commandPoolInfo.pNext            = NULL;
  commandPoolInfo.flags            = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT;
  commandPoolInfo.queueFamilyIndex = queueFamilyIndex;
  redInternalSetStatus(ctx, gpu, outStatuses, 303, vk->vkCreateCommandPool((VkDevice)gpu, &commandPoolInfo, NULL, (VkCommandPool *)&outCalls->memory), procedureId, optionalFile, optionalLine, optionalUserData);

  if (outCalls->memory == NULL) {
    outCalls->handle = NULL;
  } else {
    VkCommandBufferAllocateInfo commandBufferInfo;
    commandBufferInfo.sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    commandBufferInfo.pNext              = NULL;
    commandBufferInfo.commandPool        = (VkCommandPool)outCalls->memory;
    commandBufferInfo.level              = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    commandBufferInfo.commandBufferCount = 1;
    redInternalSetStatus(ctx, gpu, outStatuses, 304, vk->vkAllocateCommandBuffers((VkDevice)gpu, &commandBufferInfo, (VkCommandBuffer *)&outCalls->handle), procedureId, optionalFile, optionalLine, optionalUserData);
  }

  outCalls->reusable = 0;

  if (ctx->internal.debugUtilsMessengerCallback != NULL && handleName != NULL) {
    if (outCalls->handle != NULL) {
      VkDebugUtilsObjectNameInfoEXT info;
      info.sType        = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT;
      info.pNext        = NULL;
      info.objectType   = (VkObjectType)RED_HANDLE_TYPE_CALLS;
      info.objectHandle = (uint64_t)outCalls->handle;
      info.pObjectName  = handleName;
      redInternalSetStatus(ctx, gpu, outStatuses, 305, vk->vkSetDebugUtilsObjectNameEXT((VkDevice)gpu, &info), procedureId, optionalFile, optionalLine, optionalUserData);
    }
    if (outCalls->memory != NULL) {
      VkDebugUtilsObjectNameInfoEXT info;
      info.sType        = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT;
      info.pNext        = NULL;
      info.objectType   = (VkObjectType)RED_HANDLE_TYPE_CALLS_MEMORY;
      info.objectHandle = (uint64_t)outCalls->memory;
      info.pObjectName  = handleName;
      redInternalSetStatus(ctx, gpu, outStatuses, 306, vk->vkSetDebugUtilsObjectNameEXT((VkDevice)gpu, &info), procedureId, optionalFile, optionalLine, optionalUserData);
    }
  }

exit:;

}

static inline void redInlineCreateCallsReusable(RedContext context, RedHandleGpu gpu, const char * handleName, unsigned queueFamilyIndex, RedCalls * outCalls, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData) {
  const RedProcedureId procedureId = RED_PROCEDURE_ID_redCreateCallsReusable;

  RedInternalContext ctx = (RedInternalContext)context;
  RedInternalInstanceProcedureAddresses * vk = ctx->internal.instanceProcedureAddresses;

  unsigned gpuIndex = (unsigned)-1;
  for (unsigned i = 0; i < ctx->internal.hotGpuHandlesCount; i += 1) {
    if (ctx->internal.hotGpuHandles[i] == gpu) {
      gpuIndex = i;
      break;
    }
  }

  if (gpuIndex == (unsigned)-1) {
    redInternalSetStatus(ctx, gpu, outStatuses, 308, (VkResult)RED_STATUS_ERROR_INITIALIZATION_FAILED, procedureId, optionalFile, optionalLine, optionalUserData);
    goto exit;
  }

  VkCommandPoolCreateInfo commandPoolInfo;
  commandPoolInfo.sType            = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
  commandPoolInfo.pNext            = NULL;
  commandPoolInfo.flags            = 0;
  commandPoolInfo.queueFamilyIndex = queueFamilyIndex;
  redInternalSetStatus(ctx, gpu, outStatuses, 309, vk->vkCreateCommandPool((VkDevice)gpu, &commandPoolInfo, NULL, (VkCommandPool *)&outCalls->memory), procedureId, optionalFile, optionalLine, optionalUserData);

  if (outCalls->memory == NULL) {
    outCalls->handle = NULL;
  } else {
    VkCommandBufferAllocateInfo commandBufferInfo;
    commandBufferInfo.sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    commandBufferInfo.pNext              = NULL;
    commandBufferInfo.commandPool        = (VkCommandPool)outCalls->memory;
    commandBufferInfo.level              = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    commandBufferInfo.commandBufferCount = 1;
    redInternalSetStatus(ctx, gpu, outStatuses, 310, vk->vkAllocateCommandBuffers((VkDevice)gpu, &commandBufferInfo, (VkCommandBuffer *)&outCalls->handle), procedureId, optionalFile, optionalLine, optionalUserData);
  }

  outCalls->reusable = 1;

  if (ctx->internal.debugUtilsMessengerCallback != NULL && handleName != NULL) {
    if (outCalls->handle != NULL) {
      VkDebugUtilsObjectNameInfoEXT info;
      info.sType        = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT;
      info.pNext        = NULL;
      info.objectType   = (VkObjectType)RED_HANDLE_TYPE_CALLS;
      info.objectHandle = (uint64_t)outCalls->handle;
      info.pObjectName  = handleName;
      redInternalSetStatus(ctx, gpu, outStatuses, 311, vk->vkSetDebugUtilsObjectNameEXT((VkDevice)gpu, &info), procedureId, optionalFile, optionalLine, optionalUserData);
    }
    if (outCalls->memory != NULL) {
      VkDebugUtilsObjectNameInfoEXT info;
      info.sType        = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT;
      info.pNext        = NULL;
      info.objectType   = (VkObjectType)RED_HANDLE_TYPE_CALLS_MEMORY;
      info.objectHandle = (uint64_t)outCalls->memory;
      info.pObjectName  = handleName;
      redInternalSetStatus(ctx, gpu, outStatuses, 312, vk->vkSetDebugUtilsObjectNameEXT((VkDevice)gpu, &info), procedureId, optionalFile, optionalLine, optionalUserData);
    }
  }

exit:;

}

// Destroy

static inline void redInlineDestroyArray(RedContext context, RedHandleGpu gpu, RedHandleArray array, const char * optionalFile, int optionalLine, void * optionalUserData) {
//  const RedProcedureId procedureId = RED_PROCEDURE_ID_redDestroyArray;

  RedInternalContext ctx = (RedInternalContext)context;
  RedInternalInstanceProcedureAddresses * vk = ctx->internal.instanceProcedureAddresses;

  vk->vkDestroyBuffer((VkDevice)gpu, (VkBuffer)array, NULL);
}

static inline void redInlineDestroyImage(RedContext context, RedHandleGpu gpu, RedHandleImage image, const char * optionalFile, int optionalLine, void * optionalUserData) {
//  const RedProcedureId procedureId = RED_PROCEDURE_ID_redDestroyImage;

  RedInternalContext ctx = (RedInternalContext)context;
  RedInternalInstanceProcedureAddresses * vk = ctx->internal.instanceProcedureAddresses;

  vk->vkDestroyImage((VkDevice)gpu, (VkImage)image, NULL);
}

static inline void redInlineDestroySampler(RedContext context, RedHandleGpu gpu, RedHandleSampler sampler, const char * optionalFile, int optionalLine, void * optionalUserData) {
//  const RedProcedureId procedureId = RED_PROCEDURE_ID_redDestroySampler;

  RedInternalContext ctx = (RedInternalContext)context;
  RedInternalInstanceProcedureAddresses * vk = ctx->internal.instanceProcedureAddresses;

  vk->vkDestroySampler((VkDevice)gpu, (VkSampler)sampler, NULL);
}

static inline void redInlineDestroyTexture(RedContext context, RedHandleGpu gpu, RedHandleTexture texture, const char * optionalFile, int optionalLine, void * optionalUserData) {
//  const RedProcedureId procedureId = RED_PROCEDURE_ID_redDestroyTexture;

  RedInternalContext ctx = (RedInternalContext)context;
  RedInternalInstanceProcedureAddresses * vk = ctx->internal.instanceProcedureAddresses;

  vk->vkDestroyImageView((VkDevice)gpu, (VkImageView)texture, NULL);
}

static inline void redInlineDestroyGpuCode(RedContext context, RedHandleGpu gpu, RedHandleGpuCode gpuCode, const char * optionalFile, int optionalLine, void * optionalUserData) {
//  const RedProcedureId procedureId = RED_PROCEDURE_ID_redDestroyGpuCode;

  RedInternalContext ctx = (RedInternalContext)context;
  RedInternalInstanceProcedureAddresses * vk = ctx->internal.instanceProcedureAddresses;

  vk->vkDestroyShaderModule((VkDevice)gpu, (VkShaderModule)gpuCode, NULL);
}

static inline void redInlineDestroyOutputDeclaration(RedContext context, RedHandleGpu gpu, RedHandleOutputDeclaration outputDeclaration, const char * optionalFile, int optionalLine, void * optionalUserData) {
//  const RedProcedureId procedureId = RED_PROCEDURE_ID_redDestroyOutputDeclaration;

  RedInternalContext ctx = (RedInternalContext)context;
  RedInternalInstanceProcedureAddresses * vk = ctx->internal.instanceProcedureAddresses;

  vk->vkDestroyRenderPass((VkDevice)gpu, (VkRenderPass)outputDeclaration, NULL);
}

static inline void redInlineDestroyStructDeclaration(RedContext context, RedHandleGpu gpu, RedHandleStructDeclaration structDeclaration, const char * optionalFile, int optionalLine, void * optionalUserData) {
  const RedProcedureId procedureId = RED_PROCEDURE_ID_redDestroyStructDeclaration;

  RedInternalContext ctx = (RedInternalContext)context;
  RedInternalInstanceProcedureAddresses * vk = ctx->internal.instanceProcedureAddresses;

  vk->vkDestroyDescriptorSetLayout((VkDevice)gpu, (VkDescriptorSetLayout)structDeclaration, NULL);
}

static inline void redInlineDestroyProcedureParameters(RedContext context, RedHandleGpu gpu, RedHandleProcedureParameters procedureParameters, const char * optionalFile, int optionalLine, void * optionalUserData) {
  const RedProcedureId procedureId = RED_PROCEDURE_ID_redDestroyProcedureParameters;

  RedInternalContext ctx = (RedInternalContext)context;
  RedInternalInstanceProcedureAddresses * vk = ctx->internal.instanceProcedureAddresses;

  RedHandleProcedureParameters handle = procedureParameters;

  vk->vkDestroyPipelineLayout((VkDevice)gpu, (VkPipelineLayout)handle, NULL);
}

static inline void redInlineDestroyProcedureCache(RedContext context, RedHandleGpu gpu, RedHandleProcedureCache procedureCache, const char * optionalFile, int optionalLine, void * optionalUserData) {
//  const RedProcedureId procedureId = RED_PROCEDURE_ID_redDestroyProcedureCache;

  RedInternalContext ctx = (RedInternalContext)context;
  RedInternalInstanceProcedureAddresses * vk = ctx->internal.instanceProcedureAddresses;

  vk->vkDestroyPipelineCache((VkDevice)gpu, (VkPipelineCache)procedureCache, NULL);
}

static inline void redInlineDestroyProcedure(RedContext context, RedHandleGpu gpu, RedHandleProcedure procedure, const char * optionalFile, int optionalLine, void * optionalUserData) {
//  const RedProcedureId procedureId = RED_PROCEDURE_ID_redDestroyProcedure;

  RedInternalContext ctx = (RedInternalContext)context;
  RedInternalInstanceProcedureAddresses * vk = ctx->internal.instanceProcedureAddresses;

  vk->vkDestroyPipeline((VkDevice)gpu, (VkPipeline)procedure, NULL);
}

static inline void redInlineDestroyOutput(RedContext context, RedHandleGpu gpu, RedHandleOutput output, const char * optionalFile, int optionalLine, void * optionalUserData) {
//  const RedProcedureId procedureId = RED_PROCEDURE_ID_redDestroyOutput;

  RedInternalContext ctx = (RedInternalContext)context;
  RedInternalInstanceProcedureAddresses * vk = ctx->internal.instanceProcedureAddresses;

  vk->vkDestroyFramebuffer((VkDevice)gpu, (VkFramebuffer)output, NULL);
}

static inline void redInlineDestroyCpuSignal(RedContext context, RedHandleGpu gpu, RedHandleCpuSignal cpuSignal, const char * optionalFile, int optionalLine, void * optionalUserData) {
//  const RedProcedureId procedureId = RED_PROCEDURE_ID_redDestroyCpuSignal;

  RedInternalContext ctx = (RedInternalContext)context;
  RedInternalInstanceProcedureAddresses * vk = ctx->internal.instanceProcedureAddresses;

  vk->vkDestroyFence((VkDevice)gpu, (VkFence)cpuSignal, NULL);
}

static inline void redInlineDestroyGpuSignal(RedContext context, RedHandleGpu gpu, RedHandleGpuSignal gpuSignal, const char * optionalFile, int optionalLine, void * optionalUserData) {
//  const RedProcedureId procedureId = RED_PROCEDURE_ID_redDestroyGpuSignal;

  RedInternalContext ctx = (RedInternalContext)context;
  RedInternalInstanceProcedureAddresses * vk = ctx->internal.instanceProcedureAddresses;

  vk->vkDestroySemaphore((VkDevice)gpu, (VkSemaphore)gpuSignal, NULL);
}

static inline void redInlineDestroyGpuToCpuSignal(RedContext context, RedHandleGpu gpu, RedHandleGpuToCpuSignal gpuToCpuSignal, const char * optionalFile, int optionalLine, void * optionalUserData) {
//  const RedProcedureId procedureId = RED_PROCEDURE_ID_redDestroyGpuToCpuSignal;

  RedInternalContext ctx = (RedInternalContext)context;
  RedInternalInstanceProcedureAddresses * vk = ctx->internal.instanceProcedureAddresses;

  vk->vkDestroyEvent((VkDevice)gpu, (VkEvent)gpuToCpuSignal, NULL);
}

static inline void redInlineDestroyCalls(RedContext context, RedHandleGpu gpu, RedHandleCallsMemory callsMemory, const char * optionalFile, int optionalLine, void * optionalUserData) {
  const RedProcedureId procedureId = RED_PROCEDURE_ID_redDestroyCalls;

  RedInternalContext ctx = (RedInternalContext)context;
  RedInternalInstanceProcedureAddresses * vk = ctx->internal.instanceProcedureAddresses;

  unsigned gpuIndex = (unsigned)-1;
  for (unsigned i = 0; i < ctx->internal.hotGpuHandlesCount; i += 1) {
    if (ctx->internal.hotGpuHandles[i] == gpu) {
      gpuIndex = i;
      break;
    }
  }

  if (gpuIndex == (unsigned)-1) {
    redInternalSetStatus(ctx, gpu, NULL, 314, (VkResult)RED_STATUS_ERROR_INITIALIZATION_FAILED, procedureId, optionalFile, optionalLine, optionalUserData);
    return;
  }

  RedHandleCallsMemory handle = callsMemory;

  vk->vkDestroyCommandPool((VkDevice)gpu, (VkCommandPool)handle, NULL);
}

// Procedure cache

static inline void redInlineProcedureCacheGetBlob(RedContext context, RedHandleGpu gpu, RedHandleProcedureCache procedureCache, uint64_t * outBlobBytesCount, void * outBlob, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData) {
  const RedProcedureId procedureId = RED_PROCEDURE_ID_redProcedureCacheGetBlob;

  RedInternalContext ctx = (RedInternalContext)context;
  RedInternalInstanceProcedureAddresses * vk = ctx->internal.instanceProcedureAddresses;

  redInternalSetStatus(ctx, gpu, outStatuses, 315, vk->vkGetPipelineCacheData((VkDevice)gpu, (VkPipelineCache)procedureCache, (size_t *)outBlobBytesCount, outBlob), procedureId, optionalFile, optionalLine, optionalUserData);
}

static inline void redInlineProcedureCacheMergeCaches(RedContext context, RedHandleGpu gpu, unsigned sourceProcedureCachesCount, const RedHandleProcedureCache * sourceProcedureCaches, RedHandleProcedureCache targetProcedureCache, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData) {
  const RedProcedureId procedureId = RED_PROCEDURE_ID_redProcedureCacheMergeCaches;

  RedInternalContext ctx = (RedInternalContext)context;
  RedInternalInstanceProcedureAddresses * vk = ctx->internal.instanceProcedureAddresses;

  redInternalSetStatus(ctx, gpu, outStatuses, 316, vk->vkMergePipelineCaches((VkDevice)gpu, (VkPipelineCache)targetProcedureCache, sourceProcedureCachesCount, (const VkPipelineCache *)sourceProcedureCaches), procedureId, optionalFile, optionalLine, optionalUserData);
}

// CPU signal

static inline void redInlineCpuSignalGetStatus(RedContext context, RedHandleGpu gpu, RedHandleCpuSignal cpuSignal, RedStatus * outStatus, const char * optionalFile, int optionalLine, void * optionalUserData) {
//  const RedProcedureId procedureId = RED_PROCEDURE_ID_redCpuSignalGetStatus;

  RedInternalContext ctx = (RedInternalContext)context;
  RedInternalInstanceProcedureAddresses * vk = ctx->internal.instanceProcedureAddresses;

  outStatus[0] = (RedStatus)vk->vkGetFenceStatus((VkDevice)gpu, (VkFence)cpuSignal);
}

static inline void redInlineCpuSignalWait(RedContext context, RedHandleGpu gpu, unsigned cpuSignalsCount, const RedHandleCpuSignal * cpuSignals, RedBool32 waitAll, uint64_t timeout, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData) {
  const RedProcedureId procedureId = RED_PROCEDURE_ID_redCpuSignalWait;

  RedInternalContext ctx = (RedInternalContext)context;
  RedInternalInstanceProcedureAddresses * vk = ctx->internal.instanceProcedureAddresses;

  redInternalSetStatus(ctx, gpu, outStatuses, 317, vk->vkWaitForFences((VkDevice)gpu, cpuSignalsCount, (const VkFence *)cpuSignals, (VkBool32)waitAll, timeout), procedureId, optionalFile, optionalLine, optionalUserData);
}

static inline void redInlineCpuSignalUnsignal(RedContext context, RedHandleGpu gpu, unsigned cpuSignalsCount, const RedHandleCpuSignal * cpuSignals, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData) {
  const RedProcedureId procedureId = RED_PROCEDURE_ID_redCpuSignalUnsignal;

  RedInternalContext ctx = (RedInternalContext)context;
  RedInternalInstanceProcedureAddresses * vk = ctx->internal.instanceProcedureAddresses;

  redInternalSetStatus(ctx, gpu, outStatuses, 318, vk->vkResetFences((VkDevice)gpu, cpuSignalsCount, (const VkFence *)cpuSignals), procedureId, optionalFile, optionalLine, optionalUserData);
}

// GPU to CPU signal

static inline void redInlineGpuToCpuSignalGetStatus(RedContext context, RedHandleGpu gpu, RedHandleGpuToCpuSignal gpuToCpuSignal, RedStatus * outStatus, const char * optionalFile, int optionalLine, void * optionalUserData) {
//  const RedProcedureId procedureId = RED_PROCEDURE_ID_redGpuToCpuSignalGetStatus;

  RedInternalContext ctx = (RedInternalContext)context;
  RedInternalInstanceProcedureAddresses * vk = ctx->internal.instanceProcedureAddresses;

  outStatus[0] = (RedStatus)vk->vkGetEventStatus((VkDevice)gpu, (VkEvent)gpuToCpuSignal);
}

static inline void redInlineGpuToCpuSignalUnsignal(RedContext context, RedHandleGpu gpu, RedHandleGpuToCpuSignal gpuToCpuSignal, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData) {
  const RedProcedureId procedureId = RED_PROCEDURE_ID_redGpuToCpuSignalUnsignal;

  RedInternalContext ctx = (RedInternalContext)context;
  RedInternalInstanceProcedureAddresses * vk = ctx->internal.instanceProcedureAddresses;

  redInternalSetStatus(ctx, gpu, outStatuses, 319, vk->vkResetEvent((VkDevice)gpu, (VkEvent)gpuToCpuSignal), procedureId, optionalFile, optionalLine, optionalUserData);
}

// Calls

static inline void redInlineCallsReset(RedContext context, RedHandleGpu gpu, RedHandleCallsMemory callsMemory, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData) {
  const RedProcedureId procedureId = RED_PROCEDURE_ID_redCallsSet;

  RedInternalContext ctx = (RedInternalContext)context;
  RedInternalInstanceProcedureAddresses * vk = ctx->internal.instanceProcedureAddresses;

  RedHandleCallsMemory handle = callsMemory;

  redInternalSetStatus(ctx, gpu, outStatuses, 320, vk->vkResetCommandPool((VkDevice)gpu, (VkCommandPool)handle, 0), procedureId, optionalFile, optionalLine, optionalUserData);
}

static inline void redInlineCallsSet(RedContext context, RedHandleGpu gpu, RedHandleCalls calls, RedBool32 callsReusable, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData) {
  const RedProcedureId procedureId = RED_PROCEDURE_ID_redCallsSet;

  RedInternalContext ctx = (RedInternalContext)context;
  RedInternalInstanceProcedureAddresses * vk = ctx->internal.instanceProcedureAddresses;

  VkCommandBufferBeginInfo info;
  info.sType            = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
  info.pNext            = NULL;
  info.flags            = callsReusable == 1 ? 0 : VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
  info.pInheritanceInfo = NULL;
  redInternalSetStatus(ctx, gpu, outStatuses, 321, vk->vkBeginCommandBuffer((VkCommandBuffer)calls, &info), procedureId, optionalFile, optionalLine, optionalUserData);
}

static inline void redInlineCallsEnd(RedContext context, RedHandleGpu gpu, RedHandleCalls calls, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData) {
  const RedProcedureId procedureId = RED_PROCEDURE_ID_redCallsEnd;

  RedInternalContext ctx = (RedInternalContext)context;
  RedInternalInstanceProcedureAddresses * vk = ctx->internal.instanceProcedureAddresses;

  unsigned gpuIndex = (unsigned)-1;
  for (unsigned i = 0; i < ctx->internal.hotGpuHandlesCount; i += 1) {
    if (ctx->internal.hotGpuHandles[i] == gpu) {
      gpuIndex = i;
      break;
    }
  }

  if (gpuIndex == (unsigned)-1) {
    redInternalSetStatus(ctx, gpu, outStatuses, 322, (VkResult)RED_STATUS_ERROR_INITIALIZATION_FAILED, procedureId, optionalFile, optionalLine, optionalUserData);
    goto exit;
  }

exit:;

  redInternalSetStatus(ctx, gpu, outStatuses, 323, vk->vkEndCommandBuffer((VkCommandBuffer)calls), procedureId, optionalFile, optionalLine, optionalUserData);
}

// Calls recording

static inline void redInlineGetCallProceduresAndAddresses(RedContext context, RedHandleGpu gpu, RedCallProceduresAndAddresses * outCallProceduresAndAddresses, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData) {
  const RedProcedureId procedureId = RED_PROCEDURE_ID_redGetCallProceduresAndAddresses;

  RedInternalContext ctx = (RedInternalContext)context;

  unsigned gpuIndex = (unsigned)-1;
  for (unsigned i = 0; i < ctx->internal.hotGpuHandlesCount; i += 1) {
    if (ctx->internal.hotGpuHandles[i] == gpu) {
      gpuIndex = i;
      break;
    }
  }

  if (gpuIndex == (unsigned)-1) {
    redInternalSetStatus(ctx, gpu, outStatuses, 324, (VkResult)RED_STATUS_ERROR_INITIALIZATION_FAILED, procedureId, optionalFile, optionalLine, optionalUserData);
    goto exit;
  }

  {
#if defined(REDGPU_OS_ANDROID)
    const PFN_vkDebugUtilsMessengerCallbackEXT debugUtilsMessenger        =  NULL;
#else
    const PFN_vkDebugUtilsMessengerCallbackEXT debugUtilsMessenger        =  ctx->internal.debugUtilsMessengerCallback;
#endif
    RedInternalInstanceProcedureAddresses *    instanceProcedureAddresses =  ctx->internal.instanceProcedureAddresses;
    const RedInternalGpuProcedureAddresses *   gpuProcedureAddresses      = &ctx->internal.gpuProcedureAddresses[gpuIndex];

    outCallProceduresAndAddresses->redCallGpuToCpuSignalSignal            = (RedTypeProcedureCallGpuToCpuSignalSignal)gpuProcedureAddresses->redCallGpuToCpuSignalSignal;
    outCallProceduresAndAddresses->redCallCopyArrayToArray                = (RedTypeProcedureCallCopyArrayToArray)gpuProcedureAddresses->redCallCopyArrayToArray;
    outCallProceduresAndAddresses->redCallCopyImageToImage                = (RedTypeProcedureCallCopyImageToImage)gpuProcedureAddresses->redCallCopyImageToImage;
    outCallProceduresAndAddresses->redCallCopyArrayToImage                = (RedTypeProcedureCallCopyArrayToImage)gpuProcedureAddresses->redCallCopyArrayToImage;
    outCallProceduresAndAddresses->redCallCopyImageToArray                = (RedTypeProcedureCallCopyImageToArray)gpuProcedureAddresses->redCallCopyImageToArray;
    outCallProceduresAndAddresses->redCallProcedure                       = (RedTypeProcedureCallProcedure)gpuProcedureAddresses->redCallProcedure;
    outCallProceduresAndAddresses->redCallProcedureIndexed                = (RedTypeProcedureCallProcedureIndexed)gpuProcedureAddresses->redCallProcedureIndexed;
    outCallProceduresAndAddresses->redCallProcedureCompute                = (RedTypeProcedureCallProcedureCompute)gpuProcedureAddresses->redCallProcedureCompute;
    outCallProceduresAndAddresses->redCallSetProcedure                    = (RedTypeProcedureCallSetProcedure)gpuProcedureAddresses->redCallSetProcedure;
    outCallProceduresAndAddresses->redCallSetProcedureIndices             = (RedTypeProcedureCallSetProcedureIndices)gpuProcedureAddresses->redCallSetProcedureIndices;
    outCallProceduresAndAddresses->redCallSetProcedureParametersVariables = (RedTypeProcedureCallSetProcedureParametersVariables)gpuProcedureAddresses->redCallSetProcedureParametersVariables;
    outCallProceduresAndAddresses->redCallSetProcedureParametersStructs   = (RedTypeProcedureCallSetProcedureParametersStructs)gpuProcedureAddresses->redCallSetProcedureParametersStructs;
    outCallProceduresAndAddresses->redCallSetProcedureParametersHandles   = (RedTypeProcedureCallSetProcedureParametersHandles)gpuProcedureAddresses->redCallSetProcedureParametersHandles;
    outCallProceduresAndAddresses->redCallSetDynamicDepthBias             = (RedTypeProcedureCallSetDynamicDepthBias)gpuProcedureAddresses->redCallSetDynamicDepthBias;
    outCallProceduresAndAddresses->redCallSetDynamicDepthBounds           = (RedTypeProcedureCallSetDynamicDepthBounds)gpuProcedureAddresses->redCallSetDynamicDepthBounds;
    outCallProceduresAndAddresses->redCallSetDynamicStencilCompareMask    = (RedTypeProcedureCallSetDynamicStencilCompareMask)gpuProcedureAddresses->redCallSetDynamicStencilCompareMask;
    outCallProceduresAndAddresses->redCallSetDynamicStencilWriteMask      = (RedTypeProcedureCallSetDynamicStencilWriteMask)gpuProcedureAddresses->redCallSetDynamicStencilWriteMask;
    outCallProceduresAndAddresses->redCallSetDynamicStencilReference      = (RedTypeProcedureCallSetDynamicStencilReference)gpuProcedureAddresses->redCallSetDynamicStencilReference;
    outCallProceduresAndAddresses->redCallSetDynamicBlendConstants        = (RedTypeProcedureCallSetDynamicBlendConstants)gpuProcedureAddresses->redCallSetDynamicBlendConstants;
    outCallProceduresAndAddresses->redCallSetDynamicViewport              = (RedTypeProcedureAddressCallSetDynamicViewport)gpuProcedureAddresses->redCallSetDynamicViewport;
    outCallProceduresAndAddresses->redCallSetDynamicScissor               = (RedTypeProcedureAddressCallSetDynamicScissor)gpuProcedureAddresses->redCallSetDynamicScissor;
    outCallProceduresAndAddresses->redCallSetStructsMemory                = (RedTypeProcedureAddressCallSetStructsMemory)NULL;
    outCallProceduresAndAddresses->redCallSetProcedureParameters          = (RedTypeProcedureAddressCallSetProcedureParameters)NULL;
    outCallProceduresAndAddresses->redCallSetProcedureOutput              = (RedTypeProcedureAddressCallSetProcedureOutput)&gpuProcedureAddresses->redCallSetProcedureOutputVirtual;
    outCallProceduresAndAddresses->redCallEndProcedureOutput              = (RedTypeProcedureAddressCallEndProcedureOutput)gpuProcedureAddresses->redCallEndProcedureOutput;
    outCallProceduresAndAddresses->redCallUsageAliasOrderBarrier          = (RedTypeProcedureAddressCallUsageAliasOrderBarrier)gpuProcedureAddresses->redCallUsageOrderBarrier;
    outCallProceduresAndAddresses->redCallMark                            = debugUtilsMessenger == NULL ? NULL : (RedTypeProcedureAddressCallMark)instanceProcedureAddresses->vkCmdInsertDebugUtilsLabelEXT;
    outCallProceduresAndAddresses->redCallMarkSet                         = debugUtilsMessenger == NULL ? NULL : (RedTypeProcedureAddressCallMarkSet)instanceProcedureAddresses->vkCmdBeginDebugUtilsLabelEXT;
    outCallProceduresAndAddresses->redCallMarkEnd                         = debugUtilsMessenger == NULL ? NULL : (RedTypeProcedureAddressCallMarkEnd)instanceProcedureAddresses->vkCmdEndDebugUtilsLabelEXT;
  }

exit:;

}

static inline void redInlineCallSetDynamicViewport(RedTypeProcedureAddressCallSetDynamicViewport address, RedHandleCalls calls, float x, float y, float width, float height, float depthMin, float depthMax) {
  VkViewport flippedViewport;
  flippedViewport.x        = x;
  flippedViewport.y        = y + height;
  flippedViewport.width    = width;
  flippedViewport.height   =-height;
  flippedViewport.minDepth = depthMin;
  flippedViewport.maxDepth = depthMax;
  ((PFN_vkCmdSetViewport)((void *)address))((VkCommandBuffer)calls, 0, 1, &flippedViewport);
}

static inline void redInlineCallSetDynamicScissor(RedTypeProcedureAddressCallSetDynamicScissor address, RedHandleCalls calls, int x, int y, unsigned width, unsigned height) {
  VkRect2D scissor;
  scissor.offset.x      = x;
  scissor.offset.y      = y;
  scissor.extent.width  = width;
  scissor.extent.height = height;
  ((PFN_vkCmdSetScissor)((void *)address))((VkCommandBuffer)calls, 0, 1, &scissor);
}

static inline void redInlineCallSetProcedureOutput(RedTypeProcedureAddressCallSetProcedureOutput address, RedHandleCalls calls, RedHandleOutputDeclaration outputDeclaration, RedHandleOutput output, const RedInlineOutput * inlineOutput, unsigned outputWidth, unsigned outputHeight, RedBool32 outputDepthStencilEnable, unsigned outputColorsCount, float depthClearValue, unsigned stencilClearValue, const RedColorsClearValuesFloat * colorsClearValuesFloat, const RedColorsClearValuesSint * colorsClearValuesSint, const RedColorsClearValuesUint * colorsClearValuesUint) {
  const RedProcedureId procedureId = RED_PROCEDURE_ID_UNDEFINED;

  const char * optionalFile     = NULL;
  int          optionalLine     = 0;
  void *       optionalUserData = NULL;

  RedInternalVirtualRedCallSetProcedureOutput * virtualAddress = (RedInternalVirtualRedCallSetProcedureOutput *)address;
  RedInternalContext                            ctx            = (RedInternalContext)virtualAddress->context;
  unsigned                                      gpuIndex       = virtualAddress->gpuIndex;
  RedHandleGpu                                  gpu            = ctx->gpus[gpuIndex].gpu;
  const void *                                  actualAddress  = (void *)virtualAddress->redCallSetProcedureOutput;

  unsigned clearValuesCount = outputColorsCount + (outputDepthStencilEnable == 1 ? 1 : 0);
  VkClearValue clearValues[8 + 1];
  if (colorsClearValuesFloat != NULL) {
    for (unsigned i = 0; i < outputColorsCount; i += 1) {
      VkClearColorValue color;
      color.float32[0] = colorsClearValuesFloat->r[i];
      color.float32[1] = colorsClearValuesFloat->g[i];
      color.float32[2] = colorsClearValuesFloat->b[i];
      color.float32[3] = colorsClearValuesFloat->a[i];
      clearValues[i].color = color;
    }
  } else if (colorsClearValuesSint != NULL) {
    for (unsigned i = 0; i < outputColorsCount; i += 1) {
      VkClearColorValue color;
      color.int32[0] = colorsClearValuesSint->r[i];
      color.int32[1] = colorsClearValuesSint->g[i];
      color.int32[2] = colorsClearValuesSint->b[i];
      color.int32[3] = colorsClearValuesSint->a[i];
      clearValues[i].color = color;
    }
  } else if (colorsClearValuesUint != NULL) {
    for (unsigned i = 0; i < outputColorsCount; i += 1) {
      VkClearColorValue color;
      color.uint32[0] = colorsClearValuesUint->r[i];
      color.uint32[1] = colorsClearValuesUint->g[i];
      color.uint32[2] = colorsClearValuesUint->b[i];
      color.uint32[3] = colorsClearValuesUint->a[i];
      clearValues[i].color = color;
    }
  } else {
    VkClearColorValue color = {0};
    for (unsigned i = 0; i < outputColorsCount; i += 1) {
      clearValues[i].color = color;
    }
  }
  clearValues[outputColorsCount].depthStencil.depth   = depthClearValue;
  clearValues[outputColorsCount].depthStencil.stencil = stencilClearValue;
  struct {
    unsigned                   _0;
    size_t                     _1;
    RedHandleOutputDeclaration _2;
    RedHandleOutput            _3;
    struct {
      struct {
        int                    _4;
        int                    _5;
      };
      struct {
        unsigned               _6;
        unsigned               _7;
      };
    };
    unsigned                   _8;
    VkClearValue *             _9;
  } input;
  input._0 = 43;
  input._1 = 0;
  input._2 = outputDeclaration;
  input._3 = output;
  input._4 = 0;
  input._5 = 0;
  input._6 = outputWidth;
  input._7 = outputHeight;
  input._8 = clearValuesCount;
  input._9 = clearValues;
  ((PFN_vkCmdBeginRenderPass)actualAddress)((VkCommandBuffer)calls, (const VkRenderPassBeginInfo *)&input, VK_SUBPASS_CONTENTS_INLINE);
}

static inline void redInlineCallEndProcedureOutput(RedTypeProcedureAddressCallEndProcedureOutput address, RedHandleCalls calls) {
  ((PFN_vkCmdEndRenderPass)((void *)address))((VkCommandBuffer)calls);
}

static inline void redInlineCallUsageOrderBarrier(RedTypeProcedureAddressCallUsageAliasOrderBarrier address, RedHandleCalls calls, RedVkAccessStageBitflags oldAccessStages, RedVkAccessStageBitflags newAccessStages, unsigned arrayUsagesCount, const RedVkUsageArray * arrayUsages, unsigned imageUsagesCount, const RedVkUsageImage * imageUsages, RedBool32 dependencyByRegion) {
  if (oldAccessStages == 0) { oldAccessStages = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;    }
  if (newAccessStages == 0) { newAccessStages = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT; }
  ((PFN_vkCmdPipelineBarrier)((void *)address))((VkCommandBuffer)calls, (unsigned)oldAccessStages, (unsigned)newAccessStages, (unsigned)dependencyByRegion, 0, NULL, arrayUsagesCount, (const VkBufferMemoryBarrier *)arrayUsages, imageUsagesCount, (const VkImageMemoryBarrier *)imageUsages);
}

static inline void redInlineCallMark(RedTypeProcedureAddressCallMark address, RedHandleCalls calls, const char * mark) {
  if (address == NULL) {
    return;
  }

  VkDebugUtilsLabelEXT info;
  info.sType      = VK_STRUCTURE_TYPE_DEBUG_UTILS_LABEL_EXT;
  info.pNext      = NULL;
  info.pLabelName = mark;
  info.color[0]   = 1.f;
  info.color[1]   = 1.f;
  info.color[2]   = 1.f;
  info.color[3]   = 1.f;
  ((PFN_vkCmdInsertDebugUtilsLabelEXT)((void *)address))((VkCommandBuffer)calls, &info);
}

static inline void redInlineCallMarkSet(RedTypeProcedureAddressCallMarkSet address, RedHandleCalls calls, const char * mark) {
  if (address == NULL) {
    return;
  }

  VkDebugUtilsLabelEXT info;
  info.sType      = VK_STRUCTURE_TYPE_DEBUG_UTILS_LABEL_EXT;
  info.pNext      = NULL;
  info.pLabelName = mark;
  info.color[0]   = 1.f;
  info.color[1]   = 1.f;
  info.color[2]   = 1.f;
  info.color[3]   = 1.f;
  ((PFN_vkCmdBeginDebugUtilsLabelEXT)((void *)address))((VkCommandBuffer)calls, &info);
}

static inline void redInlineCallMarkEnd(RedTypeProcedureAddressCallMarkEnd address, RedHandleCalls calls) {
  if (address == NULL) {
    return;
  }

  ((PFN_vkCmdEndDebugUtilsLabelEXT)((void *)address))((VkCommandBuffer)calls);
}

// Queue

static inline void redInlineQueueSubmit(RedContext context, RedHandleGpu gpu, RedHandleQueue queue, unsigned timelinesCount, const RedGpuTimeline * timelines, RedHandleCpuSignal signalCpuSignal, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData) {
#if REDGPU_COMPILE_SWITCH == REDGPU_COMPILE_SWITCH_DEBUG
  std::lock_guard<std::mutex> __debugArrayQueueSubmitGuardAndSerializeMutexScopeGuard(__REDGPU_DEBUG_GLOBAL_3f5f84a81695ef9521b544bdc3d7d73cbb4aac3c18ce99fd19bdfd306996103e_debugArrayQueueSubmitGuardAndSerializeMutex);

  const RedProcedureId procedureId = RED_PROCEDURE_ID_redQueueSubmit;

  RedInternalContext ctx = (RedInternalContext)context;

  unsigned gpuIndex = (unsigned)-1;
  for (unsigned i = 0; i < ctx->internal.hotGpuHandlesCount; i += 1) {
    if (ctx->internal.hotGpuHandles[i] == gpu) {
      gpuIndex = i;
      break;
    }
  }

  if (gpuIndex == (unsigned)-1) {
    redInternalSetStatus(ctx, gpu, outStatuses, 326, (VkResult)RED_STATUS_ERROR_INITIALIZATION_FAILED, procedureId, optionalFile, optionalLine, optionalUserData);
    return;
  }
#else
  const RedProcedureId procedureId = RED_PROCEDURE_ID_redQueueSubmit;

  RedInternalContext ctx = (RedInternalContext)context;
#endif

  RedInternalInstanceProcedureAddresses * vk = ctx->internal.instanceProcedureAddresses;

#if REDGPU_COMPILE_SWITCH == REDGPU_COMPILE_SWITCH_DEBUG
  RedInternalContextColdData * cold = ctx->internal.cold;

  if (cold->gpus[gpuIndex].debugArrayFeatureIsSupported == 0 || cold->debugArrayPrintWasEverCalled == 0) {
#endif

  redInternalSetStatus(ctx, gpu, outStatuses, 327, vk->vkQueueSubmit((VkQueue)queue, timelinesCount, (const VkSubmitInfo *)timelines, (VkFence)signalCpuSignal), procedureId, optionalFile, optionalLine, optionalUserData);

#if REDGPU_COMPILE_SWITCH == REDGPU_COMPILE_SWITCH_DEBUG
  } else {
    RedStatuses statuses = {0};

    RedHandleCpuSignal cpuSignal = cold->gpus[gpuIndex].debugArrayCpuSignal;

    RedBool32 isUsersCpuSignal = 0;
    for (unsigned i = 0; i < timelinesCount; i += 1) {
      if (signalCpuSignal != NULL) {
        if (i == timelinesCount - 1) {
          cpuSignal = signalCpuSignal;
          isUsersCpuSignal = 1;
        }
      }

      redInternalSetStatus(ctx, gpu, outStatuses, 328, vk->vkQueueSubmit((VkQueue)queue, 1, (const VkSubmitInfo *)&timelines[i], (VkFence)cpuSignal), procedureId, optionalFile, optionalLine, optionalUserData);

      RedStatus cpuSignalStatus = (RedStatus)vk->vkGetFenceStatus((VkDevice)gpu, (VkFence)cpuSignal);
      if (cpuSignalStatus == RED_STATUS_ERROR_GPU_IS_LOST) {
        if (outStatuses != NULL) {
          if (outStatuses[0].statusError == RED_STATUS_SUCCESS) {
            outStatuses[0].statusError            = RED_STATUS_ERROR_GPU_IS_LOST;
            outStatuses[0].statusErrorCode        = 365;
            outStatuses[0].statusErrorProcedureId = procedureId;
          }
        }
        return;
      }
      const RedBool32 hasDebugArrayPrintCallback = cold->debugArrayPrintCallback != NULL;
      float4 * debugArrayData = (float4 *)cold->gpus[gpuIndex].debugArray.memory.pointer;
      const unsigned debugArrayStructsCount        = cold->debugArrayStructsCount;
      const unsigned debugArrayStructsFloat4sCount = cold->debugArrayStructsFloat4sCount;
      while (cpuSignalStatus == RED_STATUS_NOT_READY) {
        if (vk->vkGetEventStatus((VkDevice)gpu, cold->gpus[gpuIndex].debugArrayGpuToCpuEvent) == VK_EVENT_SET) {
          vk->vkResetEvent((VkDevice)gpu, cold->gpus[gpuIndex].debugArrayGpuToCpuEvent);
          if (hasDebugArrayPrintCallback == 1) {
            cold->debugArrayPrintCallback(context, gpu, debugArrayStructsCount, debugArrayStructsFloat4sCount, cold->gpus[gpuIndex].debugArray.memory.pointer);
          } else {
            REDGPU_PRINTF("[REDGPU DEBUG LIBRARY][RED_PROCEDURE_ID_redDebugArrayCallPrint]:\n");
            for (unsigned k = 0; k < debugArrayStructsCount; k += 1) {
              for (unsigned j = 0; j < debugArrayStructsFloat4sCount; j += 1) {
                const float x = debugArrayData[k * debugArrayStructsFloat4sCount + j].x;
                const float y = debugArrayData[k * debugArrayStructsFloat4sCount + j].y;
                const float z = debugArrayData[k * debugArrayStructsFloat4sCount + j].z;
                const float w = debugArrayData[k * debugArrayStructsFloat4sCount + j].w;
                REDGPU_PRINTF("debugArray[%d].float4s[%d]: %.9g, %.9g, %.9g, %.9g\n", k, j, x, y, z, w);
                debugArrayData[k * debugArrayStructsFloat4sCount + j].x = 0;
                debugArrayData[k * debugArrayStructsFloat4sCount + j].y = 0;
                debugArrayData[k * debugArrayStructsFloat4sCount + j].z = 0;
                debugArrayData[k * debugArrayStructsFloat4sCount + j].w = 0;
              }
              REDGPU_PRINTF("\n");
            }
          }
          vk->vkSetEvent((VkDevice)gpu, cold->gpus[gpuIndex].debugArrayCpuToGpuEvent);
        }
        cpuSignalStatus = (RedStatus)vk->vkGetFenceStatus((VkDevice)gpu, (VkFence)cpuSignal);
        if (cpuSignalStatus == RED_STATUS_ERROR_GPU_IS_LOST) {
          if (outStatuses != NULL) {
            if (outStatuses[0].statusError == RED_STATUS_SUCCESS) {
              outStatuses[0].statusError            = RED_STATUS_ERROR_GPU_IS_LOST;
              outStatuses[0].statusErrorCode        = 366;
              outStatuses[0].statusErrorProcedureId = procedureId;
            }
          }
          return;
        }
      }
      REDGPU_FFLUSH(stdout);

      if (isUsersCpuSignal == 0) {
        redDebugInternalCpuSignalUnsignal(ctx, gpu, 1, &cpuSignal, &statuses, optionalFile, optionalLine, optionalUserData);
        if (statuses.statusError != RED_STATUS_SUCCESS) {
          if (outStatuses != NULL) {
            if (outStatuses[0].statusError == RED_STATUS_SUCCESS) {
              outStatuses[0].statusError            = RED_STATUS_ERROR_INITIALIZATION_FAILED;
              outStatuses[0].statusErrorCode        = 367;
              outStatuses[0].statusErrorProcedureId = procedureId;
            }
          }
          return;
        }
      }
    }
  }
#endif
}


// Debug


static inline RedHandleArray redInlineDebugArrayGetHandle(RedContext context, RedHandleGpu gpu, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData) {
  const RedProcedureId procedureId = RED_PROCEDURE_ID_redDebugArrayGetHandle;

  RedInternalContext ctx = (RedInternalContext)context;

  RedHandleArray out = NULL;

#if REDGPU_COMPILE_SWITCH == REDGPU_COMPILE_SWITCH_DEBUG
  unsigned gpuIndex = (unsigned)-1;
  for (unsigned i = 0; i < ctx->internal.hotGpuHandlesCount; i += 1) {
    if (ctx->internal.hotGpuHandles[i] == gpu) {
      gpuIndex = i;
      break;
    }
  }

  if (gpuIndex == (unsigned)-1) {
    redInternalSetStatus(ctx, gpu, outStatuses, 329, (VkResult)RED_STATUS_ERROR_INITIALIZATION_FAILED, procedureId, optionalFile, optionalLine, optionalUserData);
    goto exit;
  }

  if (ctx->internal.cold->gpus[gpuIndex].debugArrayFeatureIsSupported == 0) {
    redInternalSetStatus(ctx, gpu, outStatuses, 330, (VkResult)RED_STATUS_ERROR_FEATURE_IS_NOT_FOUND, procedureId, optionalFile, optionalLine, optionalUserData);
    goto exit;
  }

  out = ctx->internal.cold->gpus[gpuIndex].debugArray.array.handle;
#else
  redInternalSetStatus(ctx, gpu, outStatuses, 355, (VkResult)RED_STATUS_ERROR_FEATURE_IS_NOT_FOUND, procedureId, optionalFile, optionalLine, optionalUserData);
#endif

exit:;

  return out;
}

static inline void redInlineDebugArrayCallPrint(RedContext context, RedHandleGpu gpu, RedHandleCalls calls, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData) {
  const RedProcedureId procedureId = RED_PROCEDURE_ID_redDebugArrayCallPrint;

  RedInternalContext ctx = (RedInternalContext)context;

#if REDGPU_COMPILE_SWITCH == REDGPU_COMPILE_SWITCH_DEBUG
  RedInternalInstanceProcedureAddresses * vk = ctx->internal.instanceProcedureAddresses;

  ctx->internal.cold->debugArrayPrintWasEverCalled = 1;

  unsigned gpuIndex = (unsigned)-1;
  for (unsigned i = 0; i < ctx->internal.hotGpuHandlesCount; i += 1) {
    if (ctx->internal.hotGpuHandles[i] == gpu) {
      gpuIndex = i;
      break;
    }
  }

  if (gpuIndex == (unsigned)-1) {
    redInternalSetStatus(ctx, gpu, outStatuses, 331, (VkResult)RED_STATUS_ERROR_INITIALIZATION_FAILED, procedureId, optionalFile, optionalLine, optionalUserData);
    return;
  }

  if (ctx->internal.cold->gpus[gpuIndex].debugArrayFeatureIsSupported == 0) {
    redInternalSetStatus(ctx, gpu, outStatuses, 332, (VkResult)RED_STATUS_ERROR_FEATURE_IS_NOT_FOUND, procedureId, optionalFile, optionalLine, optionalUserData);
    return;
  }

  const RedHandleArray debugArray              = ctx->internal.cold->gpus[gpuIndex].debugArray.array.handle;
  const VkEvent        debugArrayGpuToCpuEvent = ctx->internal.cold->gpus[gpuIndex].debugArrayGpuToCpuEvent;
  const VkEvent        debugArrayCpuToGpuEvent = ctx->internal.cold->gpus[gpuIndex].debugArrayCpuToGpuEvent;

  const RedTypeProcedureAddressCallUsageAliasOrderBarrier callUsageOrderBarrier    = (RedTypeProcedureAddressCallUsageAliasOrderBarrier)ctx->internal.gpuProcedureAddresses[gpuIndex].redCallUsageOrderBarrier;
  const RedTypeProcedureCallGpuToCpuSignalSignal          callGpuToCpuSignalSignal = (RedTypeProcedureCallGpuToCpuSignalSignal)ctx->internal.gpuProcedureAddresses[gpuIndex].redCallGpuToCpuSignalSignal;

  RedVkUsageArray debugArrayFlush        = {0};
  debugArrayFlush.setTo44                = 44;
  debugArrayFlush.setTo0                 = 0;
  debugArrayFlush.oldAccesses            = RED_VK_ACCESS_BITFLAG_COPY_R | RED_VK_ACCESS_BITFLAG_COPY_W | RED_VK_ACCESS_BITFLAG_STRUCT_RESOURCE_R | RED_VK_ACCESS_BITFLAG_STRUCT_RESOURCE_W;
  debugArrayFlush.newAccesses            = RED_VK_ACCESS_BITFLAG_CPU;
  debugArrayFlush.queueFamilyIndexSource =-1;
  debugArrayFlush.queueFamilyIndexTarget =-1;
  debugArrayFlush.array                  = debugArray;
  debugArrayFlush.arrayBytesFirst        = 0;
  debugArrayFlush.arrayBytesCount        =-1;
  redInlineCallUsageOrderBarrier(callUsageOrderBarrier, calls, RED_VK_ACCESS_STAGE_BITFLAG_COPY | RED_VK_ACCESS_STAGE_BITFLAG_COMPUTE | RED_VK_ACCESS_STAGE_BITFLAG_VERTEX | RED_VK_ACCESS_STAGE_BITFLAG_FRAGMENT, RED_VK_ACCESS_STAGE_BITFLAG_CPU, 1, &debugArrayFlush, 0, NULL, 0);

  callGpuToCpuSignalSignal(calls, (RedHandleGpuToCpuSignal)debugArrayGpuToCpuEvent, VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT);

  vk->vkCmdWaitEvents((VkCommandBuffer)calls, 1, &debugArrayCpuToGpuEvent,
    VK_PIPELINE_STAGE_HOST_BIT,
    VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
    0,
    NULL,
    0,
    NULL,
    0,
    NULL
  );
  vk->vkCmdResetEvent((VkCommandBuffer)calls, debugArrayCpuToGpuEvent, VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT);

  RedVkUsageArray debugArrayInvalidate        = {0};
  debugArrayInvalidate.setTo44                = 44;
  debugArrayInvalidate.setTo0                 = 0;
  debugArrayInvalidate.oldAccesses            = RED_VK_ACCESS_BITFLAG_CPU;
  debugArrayInvalidate.newAccesses            = RED_VK_ACCESS_BITFLAG_COPY_R | RED_VK_ACCESS_BITFLAG_COPY_W | RED_VK_ACCESS_BITFLAG_STRUCT_RESOURCE_R | RED_VK_ACCESS_BITFLAG_STRUCT_RESOURCE_W;
  debugArrayInvalidate.queueFamilyIndexSource =-1;
  debugArrayInvalidate.queueFamilyIndexTarget =-1;
  debugArrayInvalidate.array                  = debugArray;
  debugArrayInvalidate.arrayBytesFirst        = 0;
  debugArrayInvalidate.arrayBytesCount        =-1;
  redInlineCallUsageOrderBarrier(callUsageOrderBarrier, calls, RED_VK_ACCESS_STAGE_BITFLAG_CPU, RED_VK_ACCESS_STAGE_BITFLAG_COPY | RED_VK_ACCESS_STAGE_BITFLAG_COMPUTE | RED_VK_ACCESS_STAGE_BITFLAG_VERTEX | RED_VK_ACCESS_STAGE_BITFLAG_FRAGMENT, 1, &debugArrayInvalidate, 0, NULL, 0);
#else
  redInternalSetStatus(ctx, gpu, outStatuses, 356, (VkResult)RED_STATUS_ERROR_FEATURE_IS_NOT_FOUND, procedureId, optionalFile, optionalLine, optionalUserData);
#endif
}


// WSI


// Create

#if defined(VK_USE_PLATFORM_WIN32_KHR)
static inline void redInlineCreateSurfaceWin32(RedContext context, RedHandleGpu gpu, const char * handleName, const void * win32Hinstance, const void * win32Hwnd, RedHandleSurface * outSurface, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData) {
  const RedProcedureId procedureId = RED_PROCEDURE_ID_redCreateSurfaceWin32;

  RedInternalContext ctx = (RedInternalContext)context;
  RedInternalInstanceProcedureAddresses * vk = ctx->internal.instanceProcedureAddresses;

  VkWin32SurfaceCreateInfoKHR info;
  info.sType     = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
  info.pNext     = NULL;
  info.flags     = 0;
  info.hinstance = (HINSTANCE)win32Hinstance;
  info.hwnd      = (HWND)win32Hwnd;
  redInternalSetStatus(ctx, gpu, outStatuses, 357, vk->vkCreateWin32SurfaceKHR((VkInstance)ctx->contextHandle, &info, NULL, (VkSurfaceKHR *)outSurface), procedureId, optionalFile, optionalLine, optionalUserData);

  if (ctx->internal.debugUtilsMessengerCallback != NULL && handleName != NULL) {
    if (outSurface[0] != NULL) {
      VkDebugUtilsObjectNameInfoEXT info;
      info.sType        = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT;
      info.pNext        = NULL;
      info.objectType   = (VkObjectType)RED_HANDLE_TYPE_SURFACE;
      info.objectHandle = (uint64_t)outSurface[0];
      info.pObjectName  = handleName;
      redInternalSetStatus(ctx, gpu, outStatuses, 358, vk->vkSetDebugUtilsObjectNameEXT((VkDevice)gpu, &info), procedureId, optionalFile, optionalLine, optionalUserData);
    }
  }
}
#endif

#if defined(VK_USE_PLATFORM_XLIB_KHR) && defined(VK_USE_PLATFORM_XCB_KHR)
static inline void redInlineCreateSurfaceXlibOrXcb(RedContext context, RedHandleGpu gpu, const char * handleName, const void * xlibDisplay, unsigned long xlibWindow, const void * xcbConnection, unsigned xcbWindow, RedHandleSurface * outSurface, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData) {
  const RedProcedureId procedureId = RED_PROCEDURE_ID_redCreateSurfaceXlibOrXcb;

  RedInternalContext ctx = (RedInternalContext)context;
  RedInternalInstanceProcedureAddresses * vk = ctx->internal.instanceProcedureAddresses;

  if (xcbConnection != NULL) {
    VkXcbSurfaceCreateInfoKHR info;
    info.sType      = VK_STRUCTURE_TYPE_XCB_SURFACE_CREATE_INFO_KHR;
    info.pNext      = NULL;
    info.flags      = 0;
    info.connection = (xcb_connection_t *)xcbConnection;
    info.window     = (xcb_window_t)xcbWindow;
    redInternalSetStatus(ctx, gpu, outStatuses, 333, vk->vkCreateXcbSurfaceKHR((VkInstance)ctx->contextHandle, &info, NULL, (VkSurfaceKHR *)outSurface), procedureId, optionalFile, optionalLine, optionalUserData);
  } else {
    VkXlibSurfaceCreateInfoKHR info;
    info.sType  = VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR;
    info.pNext  = NULL;
    info.flags  = 0;
    info.dpy    = (Display *)xlibDisplay;
    info.window = (Window)xlibWindow;
    redInternalSetStatus(ctx, gpu, outStatuses, 334, vk->vkCreateXlibSurfaceKHR((VkInstance)ctx->contextHandle, &info, NULL, (VkSurfaceKHR *)outSurface), procedureId, optionalFile, optionalLine, optionalUserData);
  }

  if (ctx->internal.debugUtilsMessengerCallback != NULL && handleName != NULL) {
    if (outSurface[0] != NULL) {
      VkDebugUtilsObjectNameInfoEXT info;
      info.sType        = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT;
      info.pNext        = NULL;
      info.objectType   = (VkObjectType)RED_HANDLE_TYPE_SURFACE;
      info.objectHandle = (uint64_t)outSurface[0];
      info.pObjectName  = handleName;
      redInternalSetStatus(ctx, gpu, outStatuses, 335, vk->vkSetDebugUtilsObjectNameEXT((VkDevice)gpu, &info), procedureId, optionalFile, optionalLine, optionalUserData);
    }
  }
}
#endif

static inline void redInlineSurfaceGetCurrentPropertiesAndPresentLimits(RedContext context, RedHandleGpu gpu, RedHandleSurface surface, RedSurfaceCurrentPropertiesAndPresentLimits * outSurfaceCurrentPropertiesAndPresentLimits, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData) {
  const RedProcedureId procedureId = RED_PROCEDURE_ID_redSurfaceGetCurrentPropertiesAndPresentLimits;

  RedInternalContext ctx = (RedInternalContext)context;
  RedInternalInstanceProcedureAddresses * vk = ctx->internal.instanceProcedureAddresses;

  unsigned gpuIndex = (unsigned)-1;
  for (unsigned i = 0; i < ctx->internal.hotGpuHandlesCount; i += 1) {
    if (ctx->internal.hotGpuHandles[i] == gpu) {
      gpuIndex = i;
      break;
    }
  }

  if (gpuIndex == (unsigned)-1) {
    redInternalSetStatus(ctx, gpu, outStatuses, 351, (VkResult)RED_STATUS_ERROR_INITIALIZATION_FAILED, procedureId, optionalFile, optionalLine, optionalUserData);
    return;
  }

  RedHandleGpuDevice gpuDevice = context->gpus[gpuIndex].gpuDevice;

  struct {
    unsigned                         minImageCount;
    unsigned                         maxImageCount;
    struct {
      unsigned                       currentWidth;
      unsigned                       currentHeight;
    };
    struct {
      unsigned                       minImageWidth;
      unsigned                       minImageHeight;
    };
    struct {
      unsigned                       maxImageWidth;
      unsigned                       maxImageHeight;
    };
    unsigned                         maxImageLayersCount;
    RedSurfaceTransformBitflags      supportedTransforms;
    RedSurfaceTransformBitflag       currentTransform;
    RedSurfaceCompositeAlphaBitflags supportedCompositeAlphas;
    VkImageUsageFlags                supportedImageUsageFlags;
  } surfaceCapabilities;

  redInternalSetStatus(ctx, gpu, outStatuses, 352, vk->vkGetPhysicalDeviceSurfaceCapabilitiesKHR((VkPhysicalDevice)gpuDevice, (VkSurfaceKHR)surface, (VkSurfaceCapabilitiesKHR *)&surfaceCapabilities), procedureId, optionalFile, optionalLine, optionalUserData);

  VkImageUsageFlags supportedImageUsageFlags = surfaceCapabilities.supportedImageUsageFlags;

  outSurfaceCurrentPropertiesAndPresentLimits->currentSurfaceWidth                           = surfaceCapabilities.currentWidth;
  outSurfaceCurrentPropertiesAndPresentLimits->currentSurfaceHeight                          = surfaceCapabilities.currentHeight;
  outSurfaceCurrentPropertiesAndPresentLimits->currentSurfaceTransform                       = surfaceCapabilities.currentTransform;
  outSurfaceCurrentPropertiesAndPresentLimits->minPresentImagesCount                         = surfaceCapabilities.minImageCount;
  outSurfaceCurrentPropertiesAndPresentLimits->maxPresentImagesCount                         = surfaceCapabilities.maxImageCount;
  outSurfaceCurrentPropertiesAndPresentLimits->minPresentImagesWidth                         = surfaceCapabilities.minImageWidth;
  outSurfaceCurrentPropertiesAndPresentLimits->maxPresentImagesWidth                         = surfaceCapabilities.maxImageWidth;
  outSurfaceCurrentPropertiesAndPresentLimits->minPresentImagesHeight                        = surfaceCapabilities.minImageHeight;
  outSurfaceCurrentPropertiesAndPresentLimits->maxPresentImagesHeight                        = surfaceCapabilities.maxImageHeight;
  outSurfaceCurrentPropertiesAndPresentLimits->maxPresentImagesLayersCount                   = surfaceCapabilities.maxImageLayersCount;
  outSurfaceCurrentPropertiesAndPresentLimits->supportsPresentImagesAccessCopyR              = ((supportedImageUsageFlags & VK_IMAGE_USAGE_TRANSFER_SRC_BIT)             == VK_IMAGE_USAGE_TRANSFER_SRC_BIT)             ? 1 : 0;
  outSurfaceCurrentPropertiesAndPresentLimits->supportsPresentImagesAccessCopyW              = ((supportedImageUsageFlags & VK_IMAGE_USAGE_TRANSFER_DST_BIT)             == VK_IMAGE_USAGE_TRANSFER_DST_BIT)             ? 1 : 0;
  outSurfaceCurrentPropertiesAndPresentLimits->supportsPresentImagesAccessTextureRO          = ((supportedImageUsageFlags & VK_IMAGE_USAGE_SAMPLED_BIT)                  == VK_IMAGE_USAGE_SAMPLED_BIT)                  ? 1 : 0;
  outSurfaceCurrentPropertiesAndPresentLimits->supportsPresentImagesAccessTextureRW          = ((supportedImageUsageFlags & VK_IMAGE_USAGE_STORAGE_BIT)                  == VK_IMAGE_USAGE_STORAGE_BIT)                  ? 1 : 0;
  outSurfaceCurrentPropertiesAndPresentLimits->supportsPresentImagesAccessOutputDepthStencil = ((supportedImageUsageFlags & VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT) == VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT) ? 1 : 0;
  outSurfaceCurrentPropertiesAndPresentLimits->supportsPresentImagesAccessOutputColor        = ((supportedImageUsageFlags & VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT)         == VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT)         ? 1 : 0;
  outSurfaceCurrentPropertiesAndPresentLimits->supportedPresentTransforms                    = surfaceCapabilities.supportedTransforms;
  outSurfaceCurrentPropertiesAndPresentLimits->supportedPresentCompositeAlphas               = surfaceCapabilities.supportedCompositeAlphas;
}

static inline void redInlineCreatePresent(RedContext context, RedHandleGpu gpu, const char * handleName, RedHandleSurface surface, unsigned imagesCount, unsigned imagesWidth, unsigned imagesHeight, unsigned imagesLayersCount, RedAccessBitflags imagesAccess, unsigned imagesInitialQueueFamilyIndex, RedSurfaceTransformBitflag transform, RedSurfaceCompositeAlphaBitflag compositeAlpha, RedPresentVsyncMode vsyncMode, RedBool32 clipped, RedHandlePresent oldPresent, RedHandlePresent * outPresent, RedHandleImage * outImages, RedHandleTexture * outTextures, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData) {
  const RedProcedureId procedureId = RED_PROCEDURE_ID_redCreatePresent;

  RedInternalContext ctx = (RedInternalContext)context;
  RedInternalInstanceProcedureAddresses * vk = ctx->internal.instanceProcedureAddresses;

  unsigned gpuIndex = (unsigned)-1;
  for (unsigned i = 0; i < ctx->internal.hotGpuHandlesCount; i += 1) {
    if (ctx->internal.hotGpuHandles[i] == gpu) {
      gpuIndex = i;
      break;
    }
  }

  if (gpuIndex == (unsigned)-1) {
    redInternalSetStatus(ctx, gpu, outStatuses, 336, (VkResult)RED_STATUS_ERROR_INITIALIZATION_FAILED, procedureId, optionalFile, optionalLine, optionalUserData);
    return;
  }

  int isImagesCountSetToMaxValue = imagesCount == 0xFFFFFFFF ? 1 : 0;

  if (isImagesCountSetToMaxValue == 1) {
    RedSurfaceCurrentPropertiesAndPresentLimits surfaceCurrentPropertiesAndPresentLimits = {0};
    redInlineSurfaceGetCurrentPropertiesAndPresentLimits(context, gpu, surface, &surfaceCurrentPropertiesAndPresentLimits, outStatuses, optionalFile, optionalLine, optionalUserData);
    imagesCount = surfaceCurrentPropertiesAndPresentLimits.minPresentImagesCount;
  }

  VkImageUsageFlags imagesUsage = 0;
  if ((imagesAccess & RED_ACCESS_BITFLAG_COPY_R)                               == RED_ACCESS_BITFLAG_COPY_R)                               { imagesUsage |= VK_IMAGE_USAGE_TRANSFER_SRC_BIT; }
  if ((imagesAccess & RED_ACCESS_BITFLAG_COPY_W)                               == RED_ACCESS_BITFLAG_COPY_W)                               { imagesUsage |= VK_IMAGE_USAGE_TRANSFER_DST_BIT; }
  if ((imagesAccess & RED_ACCESS_BITFLAG_STRUCT_RESOURCE_NON_FRAGMENT_STAGE_R) == RED_ACCESS_BITFLAG_STRUCT_RESOURCE_NON_FRAGMENT_STAGE_R) { imagesUsage |= VK_IMAGE_USAGE_SAMPLED_BIT; }
  if ((imagesAccess & RED_ACCESS_BITFLAG_STRUCT_RESOURCE_FRAGMENT_STAGE_R)     == RED_ACCESS_BITFLAG_STRUCT_RESOURCE_FRAGMENT_STAGE_R)     { imagesUsage |= VK_IMAGE_USAGE_SAMPLED_BIT; }
  if ((imagesAccess & RED_ACCESS_BITFLAG_STRUCT_RESOURCE_W)                    == RED_ACCESS_BITFLAG_STRUCT_RESOURCE_W)                    { imagesUsage |= VK_IMAGE_USAGE_STORAGE_BIT; }
  if ((imagesAccess & RED_ACCESS_BITFLAG_OUTPUT_DEPTH_R)                       == RED_ACCESS_BITFLAG_OUTPUT_DEPTH_R)                       { imagesUsage |= VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT; }
  if ((imagesAccess & RED_ACCESS_BITFLAG_OUTPUT_DEPTH_RW)                      == RED_ACCESS_BITFLAG_OUTPUT_DEPTH_RW)                      { imagesUsage |= VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT; }
  if ((imagesAccess & RED_ACCESS_BITFLAG_OUTPUT_STENCIL_R)                     == RED_ACCESS_BITFLAG_OUTPUT_STENCIL_R)                     { imagesUsage |= VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT; }
  if ((imagesAccess & RED_ACCESS_BITFLAG_OUTPUT_STENCIL_RW)                    == RED_ACCESS_BITFLAG_OUTPUT_STENCIL_RW)                    { imagesUsage |= VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT; }
  if ((imagesAccess & RED_ACCESS_BITFLAG_OUTPUT_COLOR_W)                       == RED_ACCESS_BITFLAG_OUTPUT_COLOR_W)                       { imagesUsage |= VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT; }
#if defined(REDGPU_OS_ANDROID)
  imagesUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
#endif

  VkSwapchainCreateInfoKHR info;
  info.sType                   = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
  info.pNext                   = NULL;
  info.flags                   = 0;
  info.surface                 = (VkSurfaceKHR)surface;
  info.minImageCount           = imagesCount;
#if defined(REDGPU_OS_ANDROID)
  info.imageFormat             = (VkFormat)VK_FORMAT_R8G8B8A8_SRGB;
#else
  info.imageFormat             = (VkFormat)RED_FORMAT_PRESENT_BGRA_8_8_8_8_UINT_TO_FLOAT_0_1;
#endif
  info.imageColorSpace         = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
  info.imageExtent.width       = imagesWidth;
  info.imageExtent.height      = imagesHeight;
  info.imageArrayLayers        = imagesLayersCount;
  info.imageUsage              = imagesUsage;
  if (imagesInitialQueueFamilyIndex == (unsigned)-1) {
    info.imageSharingMode      = VK_SHARING_MODE_CONCURRENT;
    info.queueFamilyIndexCount = ctx->internal.cold->gpus[gpuIndex].vkQueueFamilyIndexesCount;
    info.pQueueFamilyIndices   = ctx->internal.cold->gpus[gpuIndex].vkQueueFamilyIndexes;
  } else {
    info.imageSharingMode      = VK_SHARING_MODE_EXCLUSIVE;
    info.queueFamilyIndexCount = 1;
    info.pQueueFamilyIndices   = &imagesInitialQueueFamilyIndex;
  }
  info.preTransform            = (VkSurfaceTransformFlagBitsKHR)transform;
  info.compositeAlpha          = (VkCompositeAlphaFlagBitsKHR)compositeAlpha;
  info.presentMode             = (VkPresentModeKHR)vsyncMode;
  info.clipped                 = (VkBool32)clipped;
  info.oldSwapchain            = (VkSwapchainKHR)oldPresent;
  redInternalSetStatus(ctx, gpu, outStatuses, 337, vk->vkCreateSwapchainKHR((VkDevice)gpu, &info, NULL, (VkSwapchainKHR *)outPresent), procedureId, optionalFile, optionalLine, optionalUserData);

  unsigned returnedImagesCount = 0;
  redInternalSetStatus(ctx, gpu, outStatuses, 338, vk->vkGetSwapchainImagesKHR((VkDevice)gpu, (VkSwapchainKHR)(outPresent[0]), &returnedImagesCount, NULL), procedureId, optionalFile, optionalLine, optionalUserData);
  if (isImagesCountSetToMaxValue == 0 && returnedImagesCount != imagesCount) {
    redInternalSetStatus(ctx, gpu, outStatuses, 339, (VkResult)RED_STATUS_ERROR_INITIALIZATION_FAILED, procedureId, optionalFile, optionalLine, optionalUserData);
    return;
  }
  redInternalSetStatus(ctx, gpu, outStatuses, 340, vk->vkGetSwapchainImagesKHR((VkDevice)gpu, (VkSwapchainKHR)(outPresent[0]), &returnedImagesCount, (VkImage *)outImages), procedureId, optionalFile, optionalLine, optionalUserData);

  if (outTextures != NULL) {
    for (unsigned i = 0; i < returnedImagesCount; i += 1) {
      VkImageViewCreateInfo info;
      info.sType                           = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
      info.pNext                           = NULL;
      info.flags                           = 0;
      info.image                           = (VkImage)outImages[i];
      info.viewType                        = (VkImageViewType)RED_TEXTURE_DIMENSIONS_2D;
#if defined(REDGPU_OS_ANDROID)
      info.format                          = (VkFormat)VK_FORMAT_R8G8B8A8_SRGB;
#else
      info.format                          = (VkFormat)RED_FORMAT_PRESENT_BGRA_8_8_8_8_UINT_TO_FLOAT_0_1;
#endif
      info.components.r                    = VK_COMPONENT_SWIZZLE_IDENTITY;
      info.components.g                    = VK_COMPONENT_SWIZZLE_IDENTITY;
      info.components.b                    = VK_COMPONENT_SWIZZLE_IDENTITY;
      info.components.a                    = VK_COMPONENT_SWIZZLE_IDENTITY;
      info.subresourceRange.aspectMask     = (VkImageAspectFlags)RED_IMAGE_PART_BITFLAG_COLOR;
      info.subresourceRange.baseMipLevel   = 0;
      info.subresourceRange.levelCount     = 1;
      info.subresourceRange.baseArrayLayer = 0;
      info.subresourceRange.layerCount     = 1;
      redInternalSetStatus(ctx, gpu, outStatuses, 341, vk->vkCreateImageView((VkDevice)gpu, &info, NULL, (VkImageView *)&outTextures[i]), procedureId, optionalFile, optionalLine, optionalUserData);

      if (ctx->internal.debugUtilsMessengerCallback != NULL && handleName != NULL) {
        if (outTextures[i] != NULL) {
          VkDebugUtilsObjectNameInfoEXT info;
          info.sType        = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT;
          info.pNext        = NULL;
          info.objectType   = (VkObjectType)RED_HANDLE_TYPE_TEXTURE;
          info.objectHandle = (uint64_t)outTextures[i];
          info.pObjectName  = handleName;
          redInternalSetStatus(ctx, gpu, outStatuses, 342, vk->vkSetDebugUtilsObjectNameEXT((VkDevice)gpu, &info), procedureId, optionalFile, optionalLine, optionalUserData);
        }
      }
    }
  }

  if (ctx->internal.debugUtilsMessengerCallback != NULL && handleName != NULL) {
    if (outPresent[0] != NULL) {
      VkDebugUtilsObjectNameInfoEXT info;
      info.sType        = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT;
      info.pNext        = NULL;
      info.objectType   = (VkObjectType)RED_HANDLE_TYPE_PRESENT;
      info.objectHandle = (uint64_t)outPresent[0];
      info.pObjectName  = handleName;
      redInternalSetStatus(ctx, gpu, outStatuses, 343, vk->vkSetDebugUtilsObjectNameEXT((VkDevice)gpu, &info), procedureId, optionalFile, optionalLine, optionalUserData);
    }
  }
}

// Destroy

static inline void redInlineDestroySurface(RedContext context, RedHandleGpu gpu, RedHandleSurface surface, const char * optionalFile, int optionalLine, void * optionalUserData) {
//  const RedProcedureId procedureId = RED_PROCEDURE_ID_redDestroySurface;

  RedInternalContext ctx = (RedInternalContext)context;
  RedInternalInstanceProcedureAddresses * vk = ctx->internal.instanceProcedureAddresses;

  vk->vkDestroySurfaceKHR((VkInstance)ctx->contextHandle, (VkSurfaceKHR)surface, NULL);
}

static inline void redInlineDestroyPresent(RedContext context, RedHandleGpu gpu, RedHandlePresent present, const char * optionalFile, int optionalLine, void * optionalUserData) {
//  const RedProcedureId procedureId = RED_PROCEDURE_ID_redDestroyPresent;

  RedInternalContext ctx = (RedInternalContext)context;
  RedInternalInstanceProcedureAddresses * vk = ctx->internal.instanceProcedureAddresses;

  vk->vkDestroySwapchainKHR((VkDevice)gpu, (VkSwapchainKHR)present, NULL);
}

// Queue family index

static inline void redInlineQueueFamilyIndexGetSupportsPresent(RedContext context, RedHandleGpu gpu, unsigned queueFamilyIndex, RedQueueFamilyIndexGetSupportsPresentOnWin32 * supportsPresentOnWin32, RedQueueFamilyIndexGetSupportsPresentOnXlib * supportsPresentOnXlib, RedQueueFamilyIndexGetSupportsPresentOnXcb * supportsPresentOnXcb, RedQueueFamilyIndexGetSupportsPresentOnSurface * supportsPresentOnSurface, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData) {
  const RedProcedureId procedureId = RED_PROCEDURE_ID_redQueueFamilyIndexGetSupportsPresent;

  RedInternalContext ctx = (RedInternalContext)context;
  RedInternalInstanceProcedureAddresses * vk = ctx->internal.instanceProcedureAddresses;

  unsigned gpuIndex = (unsigned)-1;
  for (unsigned i = 0; i < ctx->internal.hotGpuHandlesCount; i += 1) {
    if (ctx->internal.hotGpuHandles[i] == gpu) {
      gpuIndex = i;
      break;
    }
  }

  if (gpuIndex == (unsigned)-1) {
    redInternalSetStatus(ctx, gpu, outStatuses, 344, (VkResult)RED_STATUS_ERROR_INITIALIZATION_FAILED, procedureId, optionalFile, optionalLine, optionalUserData);
    return;
  }

  RedHandleGpuDevice gpuDevice = context->gpus[gpuIndex].gpuDevice;

#if defined(VK_USE_PLATFORM_WIN32_KHR)
  if (supportsPresentOnWin32 != NULL) {
    supportsPresentOnWin32->outQueueFamilyIndexSupportsPresentOnWin32 = (RedBool32)vk->vkGetPhysicalDeviceWin32PresentationSupportKHR((VkPhysicalDevice)gpuDevice, queueFamilyIndex);
  }
#else
  if (supportsPresentOnWin32 != NULL) {
    supportsPresentOnWin32->outQueueFamilyIndexSupportsPresentOnWin32 = 0;
  }
#endif

#if defined(VK_USE_PLATFORM_XLIB_KHR) && defined(VK_USE_PLATFORM_XCB_KHR)
  if (supportsPresentOnXlib != NULL) {
    supportsPresentOnXlib->outQueueFamilyIndexSupportsPresentOnXlib = (RedBool32)vk->vkGetPhysicalDeviceXlibPresentationSupportKHR((VkPhysicalDevice)gpuDevice, queueFamilyIndex, (Display *)supportsPresentOnXlib->display, supportsPresentOnXlib->visualId);
  }

  if (supportsPresentOnXcb != NULL) {
    supportsPresentOnXcb->outQueueFamilyIndexSupportsPresentOnXcb = (RedBool32)vk->vkGetPhysicalDeviceXcbPresentationSupportKHR((VkPhysicalDevice)gpuDevice, queueFamilyIndex, (xcb_connection_t *)supportsPresentOnXcb->connection, supportsPresentOnXcb->visualId);
  }
#else
  if (supportsPresentOnXlib != NULL) {
    supportsPresentOnXlib->outQueueFamilyIndexSupportsPresentOnXlib = 0;
  }

  if (supportsPresentOnXcb != NULL) {
    supportsPresentOnXcb->outQueueFamilyIndexSupportsPresentOnXcb = 0;
  }
#endif

  if (supportsPresentOnSurface != NULL) {
    redInternalSetStatus(ctx, gpu, outStatuses, 345, vk->vkGetPhysicalDeviceSurfaceSupportKHR((VkPhysicalDevice)gpuDevice, queueFamilyIndex, (VkSurfaceKHR)supportsPresentOnSurface->surface, &supportsPresentOnSurface->outQueueFamilyIndexSupportsPresentOnSurface), procedureId, optionalFile, optionalLine, optionalUserData);
  }
}

// Surface

static inline void redInlineSurfaceGetPresentFeatures(RedContext context, RedHandleGpu gpu, RedHandleSurface surface, RedSurfacePresentFeatures * outSurfacePresentFeatures, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData) {
  const RedProcedureId procedureId = RED_PROCEDURE_ID_redSurfaceGetPresentFeatures;

  RedInternalContext ctx = (RedInternalContext)context;
  RedInternalInstanceProcedureAddresses * vk = ctx->internal.instanceProcedureAddresses;

  unsigned gpuIndex = (unsigned)-1;
  for (unsigned i = 0; i < ctx->internal.hotGpuHandlesCount; i += 1) {
    if (ctx->internal.hotGpuHandles[i] == gpu) {
      gpuIndex = i;
      break;
    }
  }

  if (gpuIndex == (unsigned)-1) {
    redInternalSetStatus(ctx, gpu, outStatuses, 346, (VkResult)RED_STATUS_ERROR_INITIALIZATION_FAILED, procedureId, optionalFile, optionalLine, optionalUserData);
    return;
  }

  RedHandleGpuDevice gpuDevice = context->gpus[gpuIndex].gpuDevice;

  unsigned           supportedFormatsCount = 0;
  VkSurfaceFormatKHR supportedFormats[1];
  unsigned           supportedVsyncModesCount = 0;
  VkPresentModeKHR   supportedVsyncModes[4];

  redInternalSetStatus(ctx, gpu, outStatuses, 347, vk->vkGetPhysicalDeviceSurfaceFormatsKHR((VkPhysicalDevice)gpuDevice, (VkSurfaceKHR)surface, &supportedFormatsCount, NULL), procedureId, optionalFile, optionalLine, optionalUserData);
  if (supportedFormatsCount > 0) {
    supportedFormatsCount = 1;
    redInternalSetStatus(ctx, gpu, outStatuses, 348, vk->vkGetPhysicalDeviceSurfaceFormatsKHR((VkPhysicalDevice)gpuDevice, (VkSurfaceKHR)surface, &supportedFormatsCount, (VkSurfaceFormatKHR *)supportedFormats), procedureId, optionalFile, optionalLine, optionalUserData);
  }

  redInternalSetStatus(ctx, gpu, outStatuses, 349, vk->vkGetPhysicalDeviceSurfacePresentModesKHR((VkPhysicalDevice)gpuDevice, (VkSurfaceKHR)surface, &supportedVsyncModesCount, NULL), procedureId, optionalFile, optionalLine, optionalUserData);
  if (supportedVsyncModesCount > 4) {
    supportedVsyncModesCount = 4;
  }
  redInternalSetStatus(ctx, gpu, outStatuses, 350, vk->vkGetPhysicalDeviceSurfacePresentModesKHR((VkPhysicalDevice)gpuDevice, (VkSurfaceKHR)surface, &supportedVsyncModesCount, (VkPresentModeKHR *)supportedVsyncModes), procedureId, optionalFile, optionalLine, optionalUserData);

  outSurfacePresentFeatures->supportsPresentVsyncModeOff       = 0;
  outSurfacePresentFeatures->supportsPresentVsyncModeOn        = 0;
  outSurfacePresentFeatures->supportsPresentVsyncModeOnRelaxed = 0;
  for (unsigned i = 0; i < supportedVsyncModesCount; i += 1) {
    if (supportedVsyncModes[i] == VK_PRESENT_MODE_IMMEDIATE_KHR) {
      outSurfacePresentFeatures->supportsPresentVsyncModeOff = 1;
    } else if (supportedVsyncModes[i] == VK_PRESENT_MODE_FIFO_KHR) {
      outSurfacePresentFeatures->supportsPresentVsyncModeOn = 1;
    } else if (supportedVsyncModes[i] == VK_PRESENT_MODE_FIFO_RELAXED_KHR) {
      outSurfacePresentFeatures->supportsPresentVsyncModeOnRelaxed = 1;
    }
  }
}

// Present

static inline void redInlinePresentGetImageIndex(RedContext context, RedHandleGpu gpu, RedHandlePresent present, RedHandleCpuSignal signalCpuSignal, RedHandleGpuSignal signalGpuSignal, unsigned * outImageIndex, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData) {
  const RedProcedureId procedureId = RED_PROCEDURE_ID_redPresentGetImageIndex;

  RedInternalContext ctx = (RedInternalContext)context;
  RedInternalInstanceProcedureAddresses * vk = ctx->internal.instanceProcedureAddresses;

  redInternalSetStatus(ctx, gpu, outStatuses, 353, vk->vkAcquireNextImageKHR((VkDevice)gpu, (VkSwapchainKHR)present, (uint64_t)-1, (VkSemaphore)signalGpuSignal, (VkFence)signalCpuSignal, outImageIndex), procedureId, optionalFile, optionalLine, optionalUserData);
}

// Queue

static inline void redInlineQueuePresent(RedContext context, RedHandleGpu gpu, RedHandleQueue queue, unsigned waitForAndUnsignalGpuSignalsCount, const RedHandleGpuSignal * waitForAndUnsignalGpuSignals, unsigned presentsCount, const RedHandlePresent * presents, const unsigned * presentsImageIndex, RedStatus * outPresentsStatus, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData) {
  const RedProcedureId procedureId = RED_PROCEDURE_ID_redQueuePresent;

  RedInternalContext ctx = (RedInternalContext)context;
  RedInternalInstanceProcedureAddresses * vk = ctx->internal.instanceProcedureAddresses;

  if (waitForAndUnsignalGpuSignalsCount == 0 && presentsCount == 0) {
    vk->vkQueueWaitIdle((VkQueue)queue);
    return;
  }

  VkPresentInfoKHR info;
  info.sType              = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
  info.pNext              = NULL;
  info.waitSemaphoreCount = waitForAndUnsignalGpuSignalsCount;
  info.pWaitSemaphores    = (const VkSemaphore *)waitForAndUnsignalGpuSignals;
  info.swapchainCount     = presentsCount;
  info.pSwapchains        = (const VkSwapchainKHR *)presents;
  info.pImageIndices      = presentsImageIndex;
  info.pResults           = (VkResult *)outPresentsStatus;
  VkResult result = vk->vkQueuePresentKHR((VkQueue)queue, &info);
  redInternalSetStatus(ctx, gpu, outStatuses, 354, result, procedureId, optionalFile, optionalLine, optionalUserData);
}

// Mark

static inline void redInlineMark(const char * mark, const char * optionalFile, int optionalLine, void * optionalUserData) {
//  const RedProcedureId procedureId = RED_PROCEDURE_ID_redMark;
#if REDGPU_COMPILE_SWITCH == REDGPU_COMPILE_SWITCH_NVTX
  nvtxMarkA(mark);
#endif

#if REDGPU_COMPILE_SWITCH == REDGPU_COMPILE_SWITCH_GPUVIS
  gpuvis_trace_printf(optionalFile == NULL ? "%s%s%s\n" : "%s [%s:%s]\n", mark, optionalFile == NULL ? "" : optionalFile, optionalFile == NULL ? "" : std::to_string(optionalLine).c_str());
#endif
}

static inline void redInlineMarkSet(const char * mark, const char * optionalFile, int optionalLine, void * optionalUserData) {
//  const RedProcedureId procedureId = RED_PROCEDURE_ID_redMarkSet;
#if REDGPU_COMPILE_SWITCH == REDGPU_COMPILE_SWITCH_NVTX
  nvtxRangePushA(mark);
#endif
}

static inline void redInlineMarkEnd(const char * optionalFile, int optionalLine, void * optionalUserData) {
//  const RedProcedureId procedureId = RED_PROCEDURE_ID_redMarkEnd;
#if REDGPU_COMPILE_SWITCH == REDGPU_COMPILE_SWITCH_NVTX
  nvtxRangePop();
#endif
}

#ifdef REDGPU_OS_WINDOWS
#ifdef _WIN64
#pragma comment(linker, "/include:redGpuVisTracingStart")
#else
#pragma comment(linker, "/include:_redGpuVisTracingStart")
#endif
#endif
static inline void redInlineGpuVisTracingStart(const char * optionalFile, int optionalLine, void * optionalUserData) {
#if REDGPU_COMPILE_SWITCH == REDGPU_COMPILE_SWITCH_GPUVIS
  gpuvis_trace_init();
  gpuvis_tracing_on();
  gpuvis_start_tracing(8192);
  gpuvis_tracing_on();
#endif
}

#ifdef REDGPU_OS_WINDOWS
#ifdef _WIN64
#pragma comment(linker, "/include:redGpuVisTracingCapture")
#else
#pragma comment(linker, "/include:_redGpuVisTracingCapture")
#endif
#endif
static inline void redInlineGpuVisTracingCapture(const char * optionalFile, int optionalLine, void * optionalUserData) {
#if REDGPU_COMPILE_SWITCH == REDGPU_COMPILE_SWITCH_GPUVIS
  gpuvis_trigger_capture_and_keep_tracing(NULL, 0);
#endif
}

#ifdef REDGPU_OS_WINDOWS
#ifdef _WIN64
#pragma comment(linker, "/include:redGpuVisTracingStop")
#else
#pragma comment(linker, "/include:_redGpuVisTracingStop")
#endif
#endif
static inline void redInlineGpuVisTracingStop(const char * optionalFile, int optionalLine, void * optionalUserData) {
#if REDGPU_COMPILE_SWITCH == REDGPU_COMPILE_SWITCH_GPUVIS
  gpuvis_stop_tracing();
  gpuvis_trace_shutdown();
#endif
}

static inline void redInlineCreateArrayTimestamp(RedContext context, RedHandleGpu gpu, const char * handleName, unsigned count, RedHandleArrayTimestamp * outArrayTimestamp, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData) {
  const RedProcedureId procedureId = (const RedProcedureId)RED_PROCEDURE_ID_redCreateArrayTimestamp;

  RedInternalContext ctx = (RedInternalContext)context;
  RedInternalInstanceProcedureAddresses * vk = ctx->internal.instanceProcedureAddresses;

  VkQueryPoolCreateInfo info;
  info.sType              = VK_STRUCTURE_TYPE_QUERY_POOL_CREATE_INFO;
  info.pNext              = NULL;
  info.flags              = 0;
  info.queryType          = VK_QUERY_TYPE_TIMESTAMP;
  info.queryCount         = count;
  info.pipelineStatistics = 0;
  redInternalSetStatus(ctx, gpu, outStatuses, 376, vk->vkCreateQueryPool((VkDevice)gpu, &info, NULL, (VkQueryPool *)outArrayTimestamp), procedureId, optionalFile, optionalLine, optionalUserData);

  if (ctx->internal.debugUtilsMessengerCallback != NULL && handleName != NULL) {
    if (outArrayTimestamp[0] != NULL) {
      VkDebugUtilsObjectNameInfoEXT info;
      info.sType        = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT;
      info.pNext        = NULL;
      info.objectType   = (VkObjectType)RED_HANDLE_TYPE_ARRAY_TIMESTAMP;
      info.objectHandle = (uint64_t)outArrayTimestamp[0];
      info.pObjectName  = handleName;
      redInternalSetStatus(ctx, gpu, outStatuses, 377, vk->vkSetDebugUtilsObjectNameEXT((VkDevice)gpu, &info), procedureId, optionalFile, optionalLine, optionalUserData);
    }
  }
}

static inline void redInlineDestroyArrayTimestamp(RedContext context, RedHandleGpu gpu, RedHandleArrayTimestamp arrayTimestamp, const char * optionalFile, int optionalLine, void * optionalUserData) {
//  const RedProcedureId procedureId = (const RedProcedureId)RED_PROCEDURE_ID_redDestroyArrayTimestamp;

  RedInternalContext ctx = (RedInternalContext)context;
  RedInternalInstanceProcedureAddresses * vk = ctx->internal.instanceProcedureAddresses;

  vk->vkDestroyQueryPool((VkDevice)gpu, (VkQueryPool)arrayTimestamp, NULL);
}

static inline void redInlineArrayTimestampRead(RedContext context, RedHandleGpu gpu, RedHandleArrayTimestamp arrayTimestamp, unsigned rangeFirst, unsigned rangeCount, void * out64BitTicksCounts, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData) {
  const RedProcedureId procedureId = (const RedProcedureId)RED_PROCEDURE_ID_redArrayTimestampRead;

  RedInternalContext ctx = (RedInternalContext)context;
  RedInternalInstanceProcedureAddresses * vk = ctx->internal.instanceProcedureAddresses;

  redInternalSetStatus(ctx, gpu, outStatuses, 378, vk->vkGetQueryPoolResults((VkDevice)gpu, (VkQueryPool)arrayTimestamp, rangeFirst, rangeCount, rangeCount * sizeof(uint64_t), out64BitTicksCounts, sizeof(uint64_t), VK_QUERY_RESULT_WAIT_BIT | VK_QUERY_RESULT_64_BIT), procedureId, optionalFile, optionalLine, optionalUserData);
}

REDGPU_DECLSPEC void REDGPU_API redMemoryGetBudget(RedContext context, RedHandleGpu gpu, RedMemoryBudget * outMemoryBudget, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData) {
  const RedProcedureId procedureId = RED_PROCEDURE_ID_redMemoryGetBudget;
  redInternalMarkSet(__FUNCTION__);
  redInlineMemoryGetBudget(context, gpu, outMemoryBudget, outStatuses, optionalFile, optionalLine, optionalUserData);
  REDGPU_INTERNAL_FINALIZE_OUTSTATUSES
  redInternalMarkEnd();
}

REDGPU_DECLSPEC void REDGPU_API redMemoryAllocate(RedContext context, RedHandleGpu gpu, const char * handleName, uint64_t bytesCount, unsigned memoryTypeIndex, RedHandleArray dedicateToArray, RedHandleImage dedicateToImage, unsigned memoryBitflags, RedHandleMemory * outMemory, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData) {
  const RedProcedureId procedureId = RED_PROCEDURE_ID_redMemoryAllocate;
  redInternalMarkSet(__FUNCTION__);
  redInlineMemoryAllocate(context, gpu, handleName, bytesCount, memoryTypeIndex, dedicateToArray, dedicateToImage, outMemory, outStatuses, optionalFile, optionalLine, optionalUserData, procedureId);
  REDGPU_INTERNAL_FINALIZE_OUTSTATUSES
  redInternalMarkEnd();
}

REDGPU_DECLSPEC void REDGPU_API redMemoryAllocateMappable(RedContext context, RedHandleGpu gpu, const char * handleName, RedBool32 dedicate, RedHandleArray array, uint64_t arrayMemoryBytesCount, unsigned memoryTypeIndex, unsigned memoryBitflags, RedHandleMemory * outMemory, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData) {
  const RedProcedureId procedureId = RED_PROCEDURE_ID_redMemoryAllocateMappable;
  redInternalMarkSet(__FUNCTION__);
  redInlineMemoryAllocate(context, gpu, handleName, arrayMemoryBytesCount, memoryTypeIndex, dedicate == 1 ? array : NULL, NULL, outMemory, outStatuses, optionalFile, optionalLine, optionalUserData, procedureId);
  REDGPU_INTERNAL_FINALIZE_OUTSTATUSES
  redInternalMarkEnd();
}

REDGPU_DECLSPEC void REDGPU_API redMemoryFree(RedContext context, RedHandleGpu gpu, RedHandleMemory memory, const char * optionalFile, int optionalLine, void * optionalUserData) {
  redInternalMarkSet(__FUNCTION__);
  redInlineMemoryFree(context, gpu, memory, optionalFile, optionalLine, optionalUserData);
  redInternalMarkEnd();
}

REDGPU_DECLSPEC void REDGPU_API redMemorySet(RedContext context, RedHandleGpu gpu, unsigned memoryArraysCount, const RedMemoryArray * memoryArrays, unsigned memoryImagesCount, const RedMemoryImage * memoryImages, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData) {
  const RedProcedureId procedureId = RED_PROCEDURE_ID_redMemorySet;
  redInternalMarkSet(__FUNCTION__);
  redInlineMemorySet(context, gpu, memoryArraysCount, memoryArrays, memoryImagesCount, memoryImages, outStatuses, optionalFile, optionalLine, optionalUserData);
  REDGPU_INTERNAL_FINALIZE_OUTSTATUSES
  redInternalMarkEnd();
}

REDGPU_DECLSPEC void REDGPU_API redMemoryMap(RedContext context, RedHandleGpu gpu, RedHandleMemory mappableMemory, uint64_t mappableMemoryBytesFirst, uint64_t mappableMemoryBytesCount, void ** outVolatilePointer, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData) {
  const RedProcedureId procedureId = RED_PROCEDURE_ID_redMemoryMap;
  redInternalMarkSet(__FUNCTION__);
  redInlineMemoryMap(context, gpu, mappableMemory, mappableMemoryBytesFirst, mappableMemoryBytesCount, outVolatilePointer, outStatuses, optionalFile, optionalLine, optionalUserData);
  REDGPU_INTERNAL_FINALIZE_OUTSTATUSES
  redInternalMarkEnd();
}

REDGPU_DECLSPEC void REDGPU_API redMemoryUnmap(RedContext context, RedHandleGpu gpu, RedHandleMemory mappableMemory, const char * optionalFile, int optionalLine, void * optionalUserData) {
  redInternalMarkSet(__FUNCTION__);
  redInlineMemoryUnmap(context, gpu, mappableMemory, optionalFile, optionalLine, optionalUserData);
  redInternalMarkEnd();
}

REDGPU_DECLSPEC void REDGPU_API redMemoryNonCoherentFlush(RedContext context, RedHandleGpu gpu, unsigned mappableMemoryRangesCount, const RedMappableMemoryRange * mappableMemoryRanges, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData) {
  const RedProcedureId procedureId = RED_PROCEDURE_ID_redMemoryNonCoherentFlush;
  redInternalMarkSet(__FUNCTION__);
  redInlineMemoryNonCoherentFlush(context, gpu, mappableMemoryRangesCount, mappableMemoryRanges, outStatuses, optionalFile, optionalLine, optionalUserData);
  REDGPU_INTERNAL_FINALIZE_OUTSTATUSES
  redInternalMarkEnd();
}

REDGPU_DECLSPEC void REDGPU_API redMemoryNonCoherentInvalidate(RedContext context, RedHandleGpu gpu, unsigned mappableMemoryRangesCount, const RedMappableMemoryRange * mappableMemoryRanges, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData) {
  const RedProcedureId procedureId = RED_PROCEDURE_ID_redMemoryNonCoherentInvalidate;
  redInternalMarkSet(__FUNCTION__);
  redInlineMemoryNonCoherentInvalidate(context, gpu, mappableMemoryRangesCount, mappableMemoryRanges, outStatuses, optionalFile, optionalLine, optionalUserData);
  REDGPU_INTERNAL_FINALIZE_OUTSTATUSES
  redInternalMarkEnd();
}

REDGPU_DECLSPEC void REDGPU_API redStructsMemoryAllocate(RedContext context, RedHandleGpu gpu, const char * handleName, unsigned maxStructsCount, unsigned maxStructsMembersOfTypeArrayROConstantCount, unsigned maxStructsMembersOfTypeArrayROOrArrayRWCount, unsigned maxStructsMembersOfTypeTextureROCount, unsigned maxStructsMembersOfTypeTextureRWCount, RedHandleStructsMemory * outStructsMemory, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData) {
  const RedProcedureId procedureId = RED_PROCEDURE_ID_redStructsMemoryAllocate;
  redInternalMarkSet(__FUNCTION__);
  redInlineStructsMemoryAllocate(context, gpu, handleName, maxStructsCount, maxStructsMembersOfTypeArrayROConstantCount, maxStructsMembersOfTypeArrayROOrArrayRWCount, 0, maxStructsMembersOfTypeTextureROCount, maxStructsMembersOfTypeTextureRWCount, outStructsMemory, outStatuses, optionalFile, optionalLine, optionalUserData, procedureId);
  REDGPU_INTERNAL_FINALIZE_OUTSTATUSES
  redInternalMarkEnd();
}

REDGPU_DECLSPEC void REDGPU_API redStructsMemoryAllocateSamplers(RedContext context, RedHandleGpu gpu, const char * handleName, unsigned maxStructsCount, unsigned maxStructsMembersOfTypeSamplerCount, RedHandleStructsMemory * outStructsMemory, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData) {
  const RedProcedureId procedureId = RED_PROCEDURE_ID_redStructsMemoryAllocateSamplers;
  redInternalMarkSet(__FUNCTION__);
  redInlineStructsMemoryAllocate(context, gpu, handleName, maxStructsCount, 0, 0, maxStructsMembersOfTypeSamplerCount, 0, 0, outStructsMemory, outStatuses, optionalFile, optionalLine, optionalUserData, procedureId);
  REDGPU_INTERNAL_FINALIZE_OUTSTATUSES
  redInternalMarkEnd();
}

REDGPU_DECLSPEC void REDGPU_API redStructsMemoryAllocateWithInlineSamplers(RedContext context, RedHandleGpu gpu, const char * handleName, unsigned maxStructsCount, unsigned maxStructsMembersOfTypeArrayROConstantCount, unsigned maxStructsMembersOfTypeArrayROOrArrayRWCount, unsigned maxStructsMembersOfTypeTextureROCount, unsigned maxStructsMembersOfTypeTextureRWCount, unsigned maxStructsMembersOfTypeInlineSamplerCount, RedHandleStructsMemory * outStructsMemory, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData) {
  const RedProcedureId procedureId = RED_PROCEDURE_ID_redStructsMemoryAllocateWithInlineSamplers;
  redInternalMarkSet(__FUNCTION__);
  redInlineStructsMemoryAllocate(context, gpu, handleName, maxStructsCount, maxStructsMembersOfTypeArrayROConstantCount, maxStructsMembersOfTypeArrayROOrArrayRWCount, maxStructsMembersOfTypeInlineSamplerCount, maxStructsMembersOfTypeTextureROCount, maxStructsMembersOfTypeTextureRWCount, outStructsMemory, outStatuses, optionalFile, optionalLine, optionalUserData, procedureId);
  REDGPU_INTERNAL_FINALIZE_OUTSTATUSES
  redInternalMarkEnd();
}

REDGPU_DECLSPEC void REDGPU_API redStructsMemoryAllocateSamplersWithInlineSamplers(RedContext context, RedHandleGpu gpu, const char * handleName, unsigned maxStructsCount, unsigned maxStructsMembersOfTypeSamplerCount, unsigned maxStructsMembersOfTypeInlineSamplerCount, RedHandleStructsMemory * outStructsMemory, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData) {
  const RedProcedureId procedureId = RED_PROCEDURE_ID_redStructsMemoryAllocateSamplersWithInlineSamplers;
  redInternalMarkSet(__FUNCTION__);
  redInlineStructsMemoryAllocate(context, gpu, handleName, maxStructsCount, 0, 0, maxStructsMembersOfTypeSamplerCount + maxStructsMembersOfTypeInlineSamplerCount, 0, 0, outStructsMemory, outStatuses, optionalFile, optionalLine, optionalUserData, procedureId);
  REDGPU_INTERNAL_FINALIZE_OUTSTATUSES
  redInternalMarkEnd();
}

REDGPU_DECLSPEC void REDGPU_API redStructsMemorySuballocateStructs(RedContext context, RedHandleGpu gpu, const char ** handleNames, RedHandleStructsMemory structsMemory, unsigned structsDeclarationsCount, const RedHandleStructDeclaration * structsDeclarations, RedHandleStruct * outStructs, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData) {
  const RedProcedureId procedureId = RED_PROCEDURE_ID_redStructsMemorySuballocateStructs;
  redInternalMarkSet(__FUNCTION__);
  redInlineStructsMemorySuballocateStructs(context, gpu, handleNames, structsMemory, structsDeclarationsCount, structsDeclarations, outStructs, outStatuses, optionalFile, optionalLine, optionalUserData);
  REDGPU_INTERNAL_FINALIZE_OUTSTATUSES
  redInternalMarkEnd();
}

REDGPU_DECLSPEC void REDGPU_API redStructsMemoryReset(RedContext context, RedHandleGpu gpu, RedHandleStructsMemory structsMemory, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData) {
  const RedProcedureId procedureId = RED_PROCEDURE_ID_redStructsMemoryReset;
  redInternalMarkSet(__FUNCTION__);
  redInlineStructsMemoryReset(context, gpu, structsMemory, outStatuses, optionalFile, optionalLine, optionalUserData);
  REDGPU_INTERNAL_FINALIZE_OUTSTATUSES
  redInternalMarkEnd();
}

REDGPU_DECLSPEC void REDGPU_API redStructsMemoryFree(RedContext context, RedHandleGpu gpu, RedHandleStructsMemory structsMemory, const char * optionalFile, int optionalLine, void * optionalUserData) {
  redInternalMarkSet(__FUNCTION__);
  redInlineStructsMemoryFree(context, gpu, structsMemory, optionalFile, optionalLine, optionalUserData);
  redInternalMarkEnd();
}

REDGPU_DECLSPEC void REDGPU_API redStructsSet(RedContext context, RedHandleGpu gpu, unsigned structsMembersCount, const RedStructMember * structsMembers, const char * optionalFile, int optionalLine, void * optionalUserData) {
  redInternalMarkSet(__FUNCTION__);
  redInlineStructsSet(context, gpu, structsMembersCount, structsMembers, optionalFile, optionalLine, optionalUserData);
  redInternalMarkEnd();
}

REDGPU_DECLSPEC void REDGPU_API redCreateContext(RedTypeProcedureMalloc customMalloc, RedTypeProcedureFree customFree, RedTypeProcedureMallocTagged optionalMallocTagged, RedTypeProcedureFreeTagged optionalFreeTagged, RedTypeProcedureDebugCallback debugCallback, RedSdkVersion sdkVersion, unsigned sdkExtensionsCount, const unsigned * sdkExtensions, const char * optionalProgramName, unsigned optionalProgramVersion, const char * optionalEngineName, unsigned optionalEngineVersion, const void * optionalSettings, RedContext * outContext, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData) {
  const RedProcedureId procedureId = RED_PROCEDURE_ID_redCreateContext;
  redInternalMarkSet(__FUNCTION__);
  redInlineCreateContext(customMalloc, customFree, optionalMallocTagged, optionalFreeTagged, debugCallback, sdkVersion, sdkExtensionsCount, sdkExtensions, optionalProgramName, optionalProgramVersion, optionalEngineName, optionalEngineVersion, optionalSettings, outContext, outStatuses, optionalFile, optionalLine, optionalUserData);
  REDGPU_INTERNAL_FINALIZE_OUTSTATUSES
  redInternalMarkEnd();
}

REDGPU_DECLSPEC void REDGPU_API redCreateArray(RedContext context, RedHandleGpu gpu, const char * handleName, RedArrayType type, uint64_t bytesCount, uint64_t structuredBufferElementBytesCount, RedAccessBitflags restrictToAccess, unsigned initialQueueFamilyIndex, RedBool32 dedicate, RedArray * outArray, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData) {
  const RedProcedureId procedureId = RED_PROCEDURE_ID_redCreateArray;
  redInternalMarkSet(__FUNCTION__);
  redInlineCreateArray(context, gpu, handleName, type, bytesCount, initialQueueFamilyIndex, dedicate, outArray, outStatuses, optionalFile, optionalLine, optionalUserData);
  REDGPU_INTERNAL_FINALIZE_OUTSTATUSES
  redInternalMarkEnd();
}

REDGPU_DECLSPEC void REDGPU_API redCreateImage(RedContext context, RedHandleGpu gpu, const char * handleName, RedImageDimensions dimensions, RedFormat format, unsigned width, unsigned height, unsigned depth, unsigned levelsCount, unsigned layersCount, RedMultisampleCountBitflag multisampleCount, RedAccessBitflags restrictToAccess, unsigned initialQueueFamilyIndex, RedBool32 dedicate, RedImage * outImage, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData) {
  const RedProcedureId procedureId = RED_PROCEDURE_ID_redCreateImage;
  redInternalMarkSet(__FUNCTION__);
  redInlineCreateImage(context, gpu, handleName, dimensions, format, width, height, depth, levelsCount, layersCount, multisampleCount, restrictToAccess, initialQueueFamilyIndex, dedicate, outImage, outStatuses, optionalFile, optionalLine, optionalUserData);
  REDGPU_INTERNAL_FINALIZE_OUTSTATUSES
  redInternalMarkEnd();
}

REDGPU_DECLSPEC void REDGPU_API redCreateSampler(RedContext context, RedHandleGpu gpu, const char * handleName, RedSamplerFiltering filteringMag, RedSamplerFiltering filteringMin, RedSamplerFilteringMip filteringMip, RedSamplerBehaviorOutsideTextureCoordinate behaviorOutsideTextureCoordinateU, RedSamplerBehaviorOutsideTextureCoordinate behaviorOutsideTextureCoordinateV, RedSamplerBehaviorOutsideTextureCoordinate behaviorOutsideTextureCoordinateW, float mipLodBias, RedBool32 enableAnisotropy, float maxAnisotropy, RedBool32 enableCompare, RedCompareOp compareOp, float minLod, float maxLod, RedHandleSampler * outSampler, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData) {
  const RedProcedureId procedureId = RED_PROCEDURE_ID_redCreateSampler;
  redInternalMarkSet(__FUNCTION__);
  redInlineCreateSampler(context, gpu, handleName, filteringMag, filteringMin, filteringMip, behaviorOutsideTextureCoordinateU, behaviorOutsideTextureCoordinateV, behaviorOutsideTextureCoordinateW, mipLodBias, enableAnisotropy, maxAnisotropy, enableCompare, compareOp, minLod, maxLod, 0, outSampler, outStatuses, optionalFile, optionalLine, optionalUserData);
  REDGPU_INTERNAL_FINALIZE_OUTSTATUSES
  redInternalMarkEnd();
}

REDGPU_DECLSPEC void REDGPU_API redCreateTexture(RedContext context, RedHandleGpu gpu, const char * handleName, RedHandleImage image, RedImagePartBitflags parts, RedTextureDimensions dimensions, RedFormat format, unsigned levelsFirst, unsigned levelsCount, unsigned layersFirst, unsigned layersCount, RedAccessBitflags restrictToAccess, RedHandleTexture * outTexture, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData) {
  const RedProcedureId procedureId = RED_PROCEDURE_ID_redCreateTexture;
  redInternalMarkSet(__FUNCTION__);
  redInlineCreateTexture(context, gpu, handleName, image, parts, dimensions, format, levelsFirst, levelsCount, layersFirst, layersCount, outTexture, outStatuses, optionalFile, optionalLine, optionalUserData);
  REDGPU_INTERNAL_FINALIZE_OUTSTATUSES
  redInternalMarkEnd();
}

REDGPU_DECLSPEC void REDGPU_API redCreateGpuCode(RedContext context, RedHandleGpu gpu, const char * handleName, uint64_t irBytesCount, const void * ir, RedHandleGpuCode * outGpuCode, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData) {
  const RedProcedureId procedureId = RED_PROCEDURE_ID_redCreateGpuCode;
  redInternalMarkSet(__FUNCTION__);
  redInlineCreateGpuCode(context, gpu, handleName, irBytesCount, ir, outGpuCode, outStatuses, optionalFile, optionalLine, optionalUserData);
  REDGPU_INTERNAL_FINALIZE_OUTSTATUSES
  redInternalMarkEnd();
}

REDGPU_DECLSPEC void REDGPU_API redCreateOutputDeclaration(RedContext context, RedHandleGpu gpu, const char * handleName, const RedOutputDeclarationMembers * outputDeclarationMembers, const RedOutputDeclarationMembersResolveSources * outputDeclarationMembersResolveSources, RedBool32 dependencyByRegion, RedBool32 dependencyByRegionAllowUsageAliasOrderBarriers, RedHandleOutputDeclaration * outOutputDeclaration, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData) {
  const RedProcedureId procedureId = RED_PROCEDURE_ID_redCreateOutputDeclaration;
  redInternalMarkSet(__FUNCTION__);
  redInlineCreateOutputDeclaration(context, gpu, handleName, outputDeclarationMembers, outputDeclarationMembersResolveSources, dependencyByRegion, dependencyByRegionAllowUsageAliasOrderBarriers, outOutputDeclaration, outStatuses, optionalFile, optionalLine, optionalUserData);
  REDGPU_INTERNAL_FINALIZE_OUTSTATUSES
  redInternalMarkEnd();
}

REDGPU_DECLSPEC void REDGPU_API redCreateStructDeclaration(RedContext context, RedHandleGpu gpu, const char * handleName, unsigned structDeclarationMembersCount, const RedStructDeclarationMember * structDeclarationMembers, unsigned structDeclarationMembersArrayROCount, const RedStructDeclarationMemberArrayRO * structDeclarationMembersArrayRO, RedBool32 procedureParametersHandlesDeclaration, RedHandleStructDeclaration * outStructDeclaration, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData) {
  const RedProcedureId procedureId = RED_PROCEDURE_ID_redCreateStructDeclaration;
  redInternalMarkSet(__FUNCTION__);
  redInlineCreateStructDeclaration(context, gpu, handleName, structDeclarationMembersCount, structDeclarationMembers, procedureParametersHandlesDeclaration, outStructDeclaration, outStatuses, optionalFile, optionalLine, optionalUserData);
  REDGPU_INTERNAL_FINALIZE_OUTSTATUSES
  redInternalMarkEnd();
}

REDGPU_DECLSPEC void REDGPU_API redCreateProcedureParameters(RedContext context, RedHandleGpu gpu, const char * handleName, const RedProcedureParametersDeclaration * procedureParametersDeclaration, RedHandleProcedureParameters * outProcedureParameters, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData) {
  const RedProcedureId procedureId = RED_PROCEDURE_ID_redCreateProcedureParameters;
  redInternalMarkSet(__FUNCTION__);
  redInlineCreateProcedureParameters(context, gpu, handleName, procedureParametersDeclaration, outProcedureParameters, outStatuses, optionalFile, optionalLine, optionalUserData);
  REDGPU_INTERNAL_FINALIZE_OUTSTATUSES
  redInternalMarkEnd();
}

REDGPU_DECLSPEC void REDGPU_API redCreateProcedureCache(RedContext context, RedHandleGpu gpu, const char * handleName, uint64_t fromBlobBytesCount, const void * fromBlob, RedHandleProcedureCache * outProcedureCache, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData) {
  const RedProcedureId procedureId = RED_PROCEDURE_ID_redCreateProcedureCache;
  redInternalMarkSet(__FUNCTION__);
  redInlineCreateProcedureCache(context, gpu, handleName, fromBlobBytesCount, fromBlob, outProcedureCache, outStatuses, optionalFile, optionalLine, optionalUserData);
  REDGPU_INTERNAL_FINALIZE_OUTSTATUSES
  redInternalMarkEnd();
}

REDGPU_DECLSPEC void REDGPU_API redCreateProcedure(RedContext context, RedHandleGpu gpu, const char * handleName, RedHandleProcedureCache procedureCache, RedHandleOutputDeclaration outputDeclaration, RedHandleProcedureParameters procedureParameters, const char * gpuCodeVertexMainProcedureName, RedHandleGpuCode gpuCodeVertex, const char * gpuCodeFragmentMainProcedureName, RedHandleGpuCode gpuCodeFragment, const RedProcedureState * state, const void * stateExtension, RedBool32 deriveBase, RedHandleProcedure deriveFrom, RedHandleProcedure * outProcedure, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData) {
  const RedProcedureId procedureId = RED_PROCEDURE_ID_redCreateProcedure;
  redInternalMarkSet(__FUNCTION__);
  redInlineCreateProcedure(context, gpu, handleName, procedureCache, outputDeclaration, procedureParameters, gpuCodeVertexMainProcedureName, gpuCodeVertex, gpuCodeFragmentMainProcedureName, gpuCodeFragment, state, stateExtension, deriveBase, deriveFrom, outProcedure, outStatuses, optionalFile, optionalLine, optionalUserData);
  REDGPU_INTERNAL_FINALIZE_OUTSTATUSES
  redInternalMarkEnd();
}

REDGPU_DECLSPEC void REDGPU_API redCreateProcedureCompute(RedContext context, RedHandleGpu gpu, const char * handleName, RedHandleProcedureCache procedureCache, RedHandleProcedureParameters procedureParameters, const char * gpuCodeMainProcedureName, RedHandleGpuCode gpuCode, RedHandleProcedure * outProcedure, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData) {
  const RedProcedureId procedureId = RED_PROCEDURE_ID_redCreateProcedureCompute;
  redInternalMarkSet(__FUNCTION__);
  redInlineCreateProcedureCompute(context, gpu, handleName, procedureCache, procedureParameters, gpuCodeMainProcedureName, gpuCode, NULL, outProcedure, outStatuses, optionalFile, optionalLine, optionalUserData, procedureId);
  REDGPU_INTERNAL_FINALIZE_OUTSTATUSES
  redInternalMarkEnd();
}

REDGPU_DECLSPEC void REDGPU_API redCreateProcedureComputingLanguage(RedContext context, RedHandleGpu gpu, const char * handleName, RedHandleProcedureCache procedureCache, RedHandleProcedureParameters procedureParameters, const char * gpuCodeMainProcedureName, RedHandleGpuCode gpuCode, const void * stateExtension, RedHandleProcedure * outProcedure, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData) {
  const RedProcedureId procedureId = (RedProcedureId)RED_PROCEDURE_ID_redCreateProcedureComputingLanguage;
  redInternalMarkSet(__FUNCTION__);
  redInlineCreateProcedureCompute(context, gpu, handleName, procedureCache, procedureParameters, gpuCodeMainProcedureName, gpuCode, stateExtension, outProcedure, outStatuses, optionalFile, optionalLine, optionalUserData, procedureId);
  REDGPU_INTERNAL_FINALIZE_OUTSTATUSES
  redInternalMarkEnd();
}

REDGPU_DECLSPEC void REDGPU_API redCreateOutput(RedContext context, RedHandleGpu gpu, const char * handleName, RedHandleOutputDeclaration outputDeclaration, const RedOutputMembers * outputMembers, const RedOutputMembersResolveTargets * outputMembersResolveTargets, unsigned width, unsigned height, RedOutput * outOutput, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData) {
  const RedProcedureId procedureId = RED_PROCEDURE_ID_redCreateOutput;
  redInternalMarkSet(__FUNCTION__);
  redInlineCreateOutput(context, gpu, handleName, outputDeclaration, outputMembers, outputMembersResolveTargets, width, height, outOutput, outStatuses, optionalFile, optionalLine, optionalUserData);
  REDGPU_INTERNAL_FINALIZE_OUTSTATUSES
  redInternalMarkEnd();
}

REDGPU_DECLSPEC void REDGPU_API redCreateCpuSignal(RedContext context, RedHandleGpu gpu, const char * handleName, RedBool32 createSignaled, RedHandleCpuSignal * outCpuSignal, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData) {
  const RedProcedureId procedureId = RED_PROCEDURE_ID_redCreateCpuSignal;
  redInternalMarkSet(__FUNCTION__);
  redInlineCreateCpuSignal(context, gpu, handleName, createSignaled, outCpuSignal, outStatuses, optionalFile, optionalLine, optionalUserData);
  REDGPU_INTERNAL_FINALIZE_OUTSTATUSES
  redInternalMarkEnd();
}

REDGPU_DECLSPEC void REDGPU_API redCreateGpuSignal(RedContext context, RedHandleGpu gpu, const char * handleName, RedHandleGpuSignal * outGpuSignal, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData) {
  const RedProcedureId procedureId = RED_PROCEDURE_ID_redCreateGpuSignal;
  redInternalMarkSet(__FUNCTION__);
  redInlineCreateGpuSignal(context, gpu, handleName, outGpuSignal, outStatuses, optionalFile, optionalLine, optionalUserData);
  REDGPU_INTERNAL_FINALIZE_OUTSTATUSES
  redInternalMarkEnd();
}

REDGPU_DECLSPEC void REDGPU_API redCreateGpuToCpuSignal(RedContext context, RedHandleGpu gpu, const char * handleName, RedHandleGpuToCpuSignal * outGpuToCpuSignal, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData) {
  const RedProcedureId procedureId = RED_PROCEDURE_ID_redCreateGpuToCpuSignal;
  redInternalMarkSet(__FUNCTION__);
  redInlineCreateGpuToCpuSignal(context, gpu, handleName, outGpuToCpuSignal, outStatuses, optionalFile, optionalLine, optionalUserData);
  REDGPU_INTERNAL_FINALIZE_OUTSTATUSES
  redInternalMarkEnd();
}

REDGPU_DECLSPEC void REDGPU_API redCreateCalls(RedContext context, RedHandleGpu gpu, const char * handleName, unsigned queueFamilyIndex, RedCalls * outCalls, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData) {
  const RedProcedureId procedureId = RED_PROCEDURE_ID_redCreateCalls;
  redInternalMarkSet(__FUNCTION__);
  redInlineCreateCalls(context, gpu, handleName, queueFamilyIndex, outCalls, outStatuses, optionalFile, optionalLine, optionalUserData);
  REDGPU_INTERNAL_FINALIZE_OUTSTATUSES
  redInternalMarkEnd();
}

REDGPU_DECLSPEC void REDGPU_API redCreateCallsReusable(RedContext context, RedHandleGpu gpu, const char * handleName, unsigned queueFamilyIndex, RedCalls * outCalls, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData) {
  const RedProcedureId procedureId = RED_PROCEDURE_ID_redCreateCallsReusable;
  redInternalMarkSet(__FUNCTION__);
  redInlineCreateCallsReusable(context, gpu, handleName, queueFamilyIndex, outCalls, outStatuses, optionalFile, optionalLine, optionalUserData);
  REDGPU_INTERNAL_FINALIZE_OUTSTATUSES
  redInternalMarkEnd();
}

REDGPU_DECLSPEC void REDGPU_API redDestroyContext(RedContext context, const char * optionalFile, int optionalLine, void * optionalUserData) {
  redInternalMarkSet(__FUNCTION__);
  redInlineDestroyContext(context, optionalFile, optionalLine, optionalUserData);
  redInternalMarkEnd();
}

REDGPU_DECLSPEC void REDGPU_API redDestroyArray(RedContext context, RedHandleGpu gpu, RedHandleArray array, const char * optionalFile, int optionalLine, void * optionalUserData) {
  redInternalMarkSet(__FUNCTION__);
  redInlineDestroyArray(context, gpu, array, optionalFile, optionalLine, optionalUserData);
  redInternalMarkEnd();
}

REDGPU_DECLSPEC void REDGPU_API redDestroyImage(RedContext context, RedHandleGpu gpu, RedHandleImage image, const char * optionalFile, int optionalLine, void * optionalUserData) {
  redInternalMarkSet(__FUNCTION__);
  redInlineDestroyImage(context, gpu, image, optionalFile, optionalLine, optionalUserData);
  redInternalMarkEnd();
}

REDGPU_DECLSPEC void REDGPU_API redDestroySampler(RedContext context, RedHandleGpu gpu, RedHandleSampler sampler, const char * optionalFile, int optionalLine, void * optionalUserData) {
  redInternalMarkSet(__FUNCTION__);
  redInlineDestroySampler(context, gpu, sampler, optionalFile, optionalLine, optionalUserData);
  redInternalMarkEnd();
}

REDGPU_DECLSPEC void REDGPU_API redDestroyTexture(RedContext context, RedHandleGpu gpu, RedHandleTexture texture, const char * optionalFile, int optionalLine, void * optionalUserData) {
  redInternalMarkSet(__FUNCTION__);
  redInlineDestroyTexture(context, gpu, texture, optionalFile, optionalLine, optionalUserData);
  redInternalMarkEnd();
}

REDGPU_DECLSPEC void REDGPU_API redDestroyGpuCode(RedContext context, RedHandleGpu gpu, RedHandleGpuCode gpuCode, const char * optionalFile, int optionalLine, void * optionalUserData) {
  redInternalMarkSet(__FUNCTION__);
  redInlineDestroyGpuCode(context, gpu, gpuCode, optionalFile, optionalLine, optionalUserData);
  redInternalMarkEnd();
}

REDGPU_DECLSPEC void REDGPU_API redDestroyOutputDeclaration(RedContext context, RedHandleGpu gpu, RedHandleOutputDeclaration outputDeclaration, const char * optionalFile, int optionalLine, void * optionalUserData) {
  redInternalMarkSet(__FUNCTION__);
  redInlineDestroyOutputDeclaration(context, gpu, outputDeclaration, optionalFile, optionalLine, optionalUserData);
  redInternalMarkEnd();
}

REDGPU_DECLSPEC void REDGPU_API redDestroyStructDeclaration(RedContext context, RedHandleGpu gpu, RedHandleStructDeclaration structDeclaration, const char * optionalFile, int optionalLine, void * optionalUserData) {
  redInternalMarkSet(__FUNCTION__);
  redInlineDestroyStructDeclaration(context, gpu, structDeclaration, optionalFile, optionalLine, optionalUserData);
  redInternalMarkEnd();
}

REDGPU_DECLSPEC void REDGPU_API redDestroyProcedureParameters(RedContext context, RedHandleGpu gpu, RedHandleProcedureParameters procedureParameters, const char * optionalFile, int optionalLine, void * optionalUserData) {
  redInternalMarkSet(__FUNCTION__);
  redInlineDestroyProcedureParameters(context, gpu, procedureParameters, optionalFile, optionalLine, optionalUserData);
  redInternalMarkEnd();
}

REDGPU_DECLSPEC void REDGPU_API redDestroyProcedureCache(RedContext context, RedHandleGpu gpu, RedHandleProcedureCache procedureCache, const char * optionalFile, int optionalLine, void * optionalUserData) {
  redInternalMarkSet(__FUNCTION__);
  redInlineDestroyProcedureCache(context, gpu, procedureCache, optionalFile, optionalLine, optionalUserData);
  redInternalMarkEnd();
}

REDGPU_DECLSPEC void REDGPU_API redDestroyProcedure(RedContext context, RedHandleGpu gpu, RedHandleProcedure procedure, const char * optionalFile, int optionalLine, void * optionalUserData) {
  redInternalMarkSet(__FUNCTION__);
  redInlineDestroyProcedure(context, gpu, procedure, optionalFile, optionalLine, optionalUserData);
  redInternalMarkEnd();
}

REDGPU_DECLSPEC void REDGPU_API redDestroyOutput(RedContext context, RedHandleGpu gpu, RedHandleOutput output, const char * optionalFile, int optionalLine, void * optionalUserData) {
  redInternalMarkSet(__FUNCTION__);
  redInlineDestroyOutput(context, gpu, output, optionalFile, optionalLine, optionalUserData);
  redInternalMarkEnd();
}

REDGPU_DECLSPEC void REDGPU_API redDestroyCpuSignal(RedContext context, RedHandleGpu gpu, RedHandleCpuSignal cpuSignal, const char * optionalFile, int optionalLine, void * optionalUserData) {
  redInternalMarkSet(__FUNCTION__);
  redInlineDestroyCpuSignal(context, gpu, cpuSignal, optionalFile, optionalLine, optionalUserData);
  redInternalMarkEnd();
}

REDGPU_DECLSPEC void REDGPU_API redDestroyGpuSignal(RedContext context, RedHandleGpu gpu, RedHandleGpuSignal gpuSignal, const char * optionalFile, int optionalLine, void * optionalUserData) {
  redInternalMarkSet(__FUNCTION__);
  redInlineDestroyGpuSignal(context, gpu, gpuSignal, optionalFile, optionalLine, optionalUserData);
  redInternalMarkEnd();
}

REDGPU_DECLSPEC void REDGPU_API redDestroyGpuToCpuSignal(RedContext context, RedHandleGpu gpu, RedHandleGpuToCpuSignal gpuToCpuSignal, const char * optionalFile, int optionalLine, void * optionalUserData) {
  redInternalMarkSet(__FUNCTION__);
  redInlineDestroyGpuToCpuSignal(context, gpu, gpuToCpuSignal, optionalFile, optionalLine, optionalUserData);
  redInternalMarkEnd();
}

REDGPU_DECLSPEC void REDGPU_API redDestroyCalls(RedContext context, RedHandleGpu gpu, RedHandleCalls calls, RedHandleCallsMemory callsMemory, const char * optionalFile, int optionalLine, void * optionalUserData) {
  redInternalMarkSet(__FUNCTION__);
  redInlineDestroyCalls(context, gpu, callsMemory, optionalFile, optionalLine, optionalUserData);
  redInternalMarkEnd();
}

REDGPU_DECLSPEC void REDGPU_API redProcedureCacheGetBlob(RedContext context, RedHandleGpu gpu, RedHandleProcedureCache procedureCache, uint64_t * outBlobBytesCount, void * outBlob, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData) {
  const RedProcedureId procedureId = RED_PROCEDURE_ID_redProcedureCacheGetBlob;
  redInternalMarkSet(__FUNCTION__);
  redInlineProcedureCacheGetBlob(context, gpu, procedureCache, outBlobBytesCount, outBlob, outStatuses, optionalFile, optionalLine, optionalUserData);
  REDGPU_INTERNAL_FINALIZE_OUTSTATUSES
  redInternalMarkEnd();
}

REDGPU_DECLSPEC void REDGPU_API redProcedureCacheMergeCaches(RedContext context, RedHandleGpu gpu, unsigned sourceProcedureCachesCount, const RedHandleProcedureCache * sourceProcedureCaches, RedHandleProcedureCache targetProcedureCache, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData) {
  const RedProcedureId procedureId = RED_PROCEDURE_ID_redProcedureCacheMergeCaches;
  redInternalMarkSet(__FUNCTION__);
  redInlineProcedureCacheMergeCaches(context, gpu, sourceProcedureCachesCount, sourceProcedureCaches, targetProcedureCache, outStatuses, optionalFile, optionalLine, optionalUserData);
  REDGPU_INTERNAL_FINALIZE_OUTSTATUSES
  redInternalMarkEnd();
}

REDGPU_DECLSPEC void REDGPU_API redCpuSignalGetStatus(RedContext context, RedHandleGpu gpu, RedHandleCpuSignal cpuSignal, RedStatus * outStatus, const char * optionalFile, int optionalLine, void * optionalUserData) {
  redInternalMarkSet(__FUNCTION__);
  redInlineCpuSignalGetStatus(context, gpu, cpuSignal, outStatus, optionalFile, optionalLine, optionalUserData);
  redInternalMarkEnd();
}

REDGPU_DECLSPEC void REDGPU_API redCpuSignalWait(RedContext context, RedHandleGpu gpu, unsigned cpuSignalsCount, const RedHandleCpuSignal * cpuSignals, RedBool32 waitAll, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData) {
  const RedProcedureId procedureId = RED_PROCEDURE_ID_redCpuSignalWait;
  redInternalMarkSet(__FUNCTION__);
  redInlineCpuSignalWait(context, gpu, cpuSignalsCount, cpuSignals, waitAll, (uint64_t)-1, outStatuses, optionalFile, optionalLine, optionalUserData);
  REDGPU_INTERNAL_FINALIZE_OUTSTATUSES
  redInternalMarkEnd();
}

REDGPU_DECLSPEC void REDGPU_API redCpuSignalUnsignal(RedContext context, RedHandleGpu gpu, unsigned cpuSignalsCount, const RedHandleCpuSignal * cpuSignals, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData) {
  const RedProcedureId procedureId = RED_PROCEDURE_ID_redCpuSignalUnsignal;
  redInternalMarkSet(__FUNCTION__);
  redInlineCpuSignalUnsignal(context, gpu, cpuSignalsCount, cpuSignals, outStatuses, optionalFile, optionalLine, optionalUserData);
  REDGPU_INTERNAL_FINALIZE_OUTSTATUSES
  redInternalMarkEnd();
}

REDGPU_DECLSPEC void REDGPU_API redGpuToCpuSignalGetStatus(RedContext context, RedHandleGpu gpu, RedHandleGpuToCpuSignal gpuToCpuSignal, RedStatus * outStatus, const char * optionalFile, int optionalLine, void * optionalUserData) {
  redInternalMarkSet(__FUNCTION__);
  redInlineGpuToCpuSignalGetStatus(context, gpu, gpuToCpuSignal, outStatus, optionalFile, optionalLine, optionalUserData);
  redInternalMarkEnd();
}

REDGPU_DECLSPEC void REDGPU_API redGpuToCpuSignalUnsignal(RedContext context, RedHandleGpu gpu, RedHandleGpuToCpuSignal gpuToCpuSignal, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData) {
  const RedProcedureId procedureId = RED_PROCEDURE_ID_redGpuToCpuSignalUnsignal;
  redInternalMarkSet(__FUNCTION__);
  redInlineGpuToCpuSignalUnsignal(context, gpu, gpuToCpuSignal, outStatuses, optionalFile, optionalLine, optionalUserData);
  REDGPU_INTERNAL_FINALIZE_OUTSTATUSES
  redInternalMarkEnd();
}

REDGPU_DECLSPEC void REDGPU_API redCallsSet(RedContext context, RedHandleGpu gpu, RedHandleCalls calls, RedHandleCallsMemory callsMemory, RedBool32 callsReusable, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData) {
  const RedProcedureId procedureId = RED_PROCEDURE_ID_redCallsSet;
  redInternalMarkSet(__FUNCTION__);
  redInlineCallsReset(context, gpu, callsMemory, outStatuses, optionalFile, optionalLine, optionalUserData);
  redInlineCallsSet(context, gpu, calls, callsReusable, outStatuses, optionalFile, optionalLine, optionalUserData);
  REDGPU_INTERNAL_FINALIZE_OUTSTATUSES
  redInternalMarkEnd();
}

REDGPU_DECLSPEC void REDGPU_API redCallsEnd(RedContext context, RedHandleGpu gpu, RedHandleCalls calls, RedHandleCallsMemory callsMemory, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData) {
  const RedProcedureId procedureId = RED_PROCEDURE_ID_redCallsEnd;
  redInternalMarkSet(__FUNCTION__);
  redInlineCallsEnd(context, gpu, calls, outStatuses, optionalFile, optionalLine, optionalUserData);
  REDGPU_INTERNAL_FINALIZE_OUTSTATUSES
  redInternalMarkEnd();
}

REDGPU_DECLSPEC void REDGPU_API redGetCallProceduresAndAddresses(RedContext context, RedHandleGpu gpu, RedCallProceduresAndAddresses * outCallProceduresAndAddresses, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData) {
  const RedProcedureId procedureId = RED_PROCEDURE_ID_redGetCallProceduresAndAddresses;
  redInlineGetCallProceduresAndAddresses(context, gpu, outCallProceduresAndAddresses, outStatuses, optionalFile, optionalLine, optionalUserData);
  REDGPU_INTERNAL_FINALIZE_OUTSTATUSES
}

REDGPU_DECLSPEC void REDGPU_API redCallSetDynamicViewport(RedTypeProcedureAddressCallSetDynamicViewport address, RedHandleCalls calls, float x, float y, float width, float height, float depthMin, float depthMax) {
  redInlineCallSetDynamicViewport(address, calls, x, y, width, height, depthMin, depthMax);
}

REDGPU_DECLSPEC void REDGPU_API redCallSetDynamicScissor(RedTypeProcedureAddressCallSetDynamicScissor address, RedHandleCalls calls, int x, int y, unsigned width, unsigned height) {
  redInlineCallSetDynamicScissor(address, calls, x, y, width, height);
}

REDGPU_DECLSPEC void REDGPU_API redCallSetStructsMemory(RedTypeProcedureAddressCallSetStructsMemory address, RedHandleCalls calls, RedHandleStructsMemory structsMemory, RedHandleStructsMemory structsMemorySamplers) {
  volatile int x = 0;
}

REDGPU_DECLSPEC void REDGPU_API redCallSetProcedureParameters(RedTypeProcedureAddressCallSetProcedureParameters address, RedHandleCalls calls, RedProcedureType procedureType, RedHandleProcedureParameters procedureParameters) {
  volatile int x = 0;
}

REDGPU_DECLSPEC void REDGPU_API redCallSetProcedureOutput(RedTypeProcedureAddressCallSetProcedureOutput     address, RedHandleCalls calls, RedHandleOutputDeclaration outputDeclaration, RedHandleOutput output, const RedInlineOutput * inlineOutput, unsigned outputWidth, unsigned outputHeight, RedBool32 outputDepthStencilEnable, unsigned outputColorsCount, float depthClearValue, unsigned stencilClearValue, const RedColorsClearValuesFloat * colorsClearValuesFloat, const RedColorsClearValuesSint * colorsClearValuesSint, const RedColorsClearValuesUint * colorsClearValuesUint) {
  redInlineCallSetProcedureOutput(address, calls, outputDeclaration, output, inlineOutput, outputWidth, outputHeight, outputDepthStencilEnable, outputColorsCount, depthClearValue, stencilClearValue, colorsClearValuesFloat, colorsClearValuesSint, colorsClearValuesUint);
}

REDGPU_DECLSPEC void REDGPU_API redCallEndProcedureOutput(RedTypeProcedureAddressCallEndProcedureOutput address, RedHandleCalls calls) {
  redInlineCallEndProcedureOutput(address, calls);
}

static inline RedVkAccessStageBitflags redInternalRedAccessStagesToRedVkAccessStages(RedAccessStageBitflags flags) {
  RedVkAccessStageBitflags out = 0;
  if ((flags & RED_ACCESS_STAGE_BITFLAG_COPY)                 == RED_ACCESS_STAGE_BITFLAG_COPY)                 { out |= RED_VK_ACCESS_STAGE_BITFLAG_COPY;                 }
  if ((flags & RED_ACCESS_STAGE_BITFLAG_COMPUTE)              == RED_ACCESS_STAGE_BITFLAG_COMPUTE)              { out |= RED_VK_ACCESS_STAGE_BITFLAG_COMPUTE;              }
  if ((flags & RED_ACCESS_STAGE_BITFLAG_INDEX)                == RED_ACCESS_STAGE_BITFLAG_INDEX)                { out |= RED_VK_ACCESS_STAGE_BITFLAG_INDEX;                }
  if ((flags & RED_ACCESS_STAGE_BITFLAG_VERTEX)               == RED_ACCESS_STAGE_BITFLAG_VERTEX)               { out |= RED_VK_ACCESS_STAGE_BITFLAG_VERTEX;               }
  if ((flags & RED_ACCESS_STAGE_BITFLAG_FRAGMENT)             == RED_ACCESS_STAGE_BITFLAG_FRAGMENT)             { out |= RED_VK_ACCESS_STAGE_BITFLAG_FRAGMENT;             }
  if ((flags & RED_ACCESS_STAGE_BITFLAG_OUTPUT_DEPTH_STENCIL) == RED_ACCESS_STAGE_BITFLAG_OUTPUT_DEPTH_STENCIL) { out |= RED_VK_ACCESS_STAGE_BITFLAG_OUTPUT_DEPTH_STENCIL; }
  if ((flags & RED_ACCESS_STAGE_BITFLAG_OUTPUT_COLOR)         == RED_ACCESS_STAGE_BITFLAG_OUTPUT_COLOR)         { out |= RED_VK_ACCESS_STAGE_BITFLAG_OUTPUT_COLOR;         }
  if ((flags & RED_ACCESS_STAGE_BITFLAG_RESOLVE)              == RED_ACCESS_STAGE_BITFLAG_RESOLVE)              { out |= RED_VK_ACCESS_STAGE_BITFLAG_RESOLVE;              }
  if ((flags & RED_ACCESS_STAGE_BITFLAG_CPU)                  == RED_ACCESS_STAGE_BITFLAG_CPU)                  { out |= RED_VK_ACCESS_STAGE_BITFLAG_CPU;                  }
  return out;
}

static inline RedVkAccessBitflags redInternalRedAccessToRedVkAccess(RedAccessBitflags flags) {
  RedVkAccessBitflags out = 0;
  if ((flags & RED_ACCESS_BITFLAG_COPY_R)                               == RED_ACCESS_BITFLAG_COPY_R)                               { out |= RED_VK_ACCESS_BITFLAG_COPY_R;                     }
  if ((flags & RED_ACCESS_BITFLAG_COPY_W)                               == RED_ACCESS_BITFLAG_COPY_W)                               { out |= RED_VK_ACCESS_BITFLAG_COPY_W;                     }
  if ((flags & RED_ACCESS_BITFLAG_INDEX_R)                              == RED_ACCESS_BITFLAG_INDEX_R)                              { out |= RED_VK_ACCESS_BITFLAG_INDEX_R;                    }
  if ((flags & RED_ACCESS_BITFLAG_STRUCT_ARRAY_RO_CONSTANT_R)           == RED_ACCESS_BITFLAG_STRUCT_ARRAY_RO_CONSTANT_R)           { out |= RED_VK_ACCESS_BITFLAG_STRUCT_ARRAY_RO_CONSTANT_R; }
  if ((flags & RED_ACCESS_BITFLAG_STRUCT_RESOURCE_NON_FRAGMENT_STAGE_R) == RED_ACCESS_BITFLAG_STRUCT_RESOURCE_NON_FRAGMENT_STAGE_R) { out |= RED_VK_ACCESS_BITFLAG_STRUCT_RESOURCE_R;          }
  if ((flags & RED_ACCESS_BITFLAG_STRUCT_RESOURCE_FRAGMENT_STAGE_R)     == RED_ACCESS_BITFLAG_STRUCT_RESOURCE_FRAGMENT_STAGE_R)     { out |= RED_VK_ACCESS_BITFLAG_STRUCT_RESOURCE_R;          }
  if ((flags & RED_ACCESS_BITFLAG_STRUCT_RESOURCE_W)                    == RED_ACCESS_BITFLAG_STRUCT_RESOURCE_W)                    { out |= RED_VK_ACCESS_BITFLAG_STRUCT_RESOURCE_W;          }
  if ((flags & RED_ACCESS_BITFLAG_OUTPUT_DEPTH_R)                       == RED_ACCESS_BITFLAG_OUTPUT_DEPTH_R)                       { out |= RED_VK_ACCESS_BITFLAG_OUTPUT_DEPTH_STENCIL;       }
  if ((flags & RED_ACCESS_BITFLAG_OUTPUT_DEPTH_RW)                      == RED_ACCESS_BITFLAG_OUTPUT_DEPTH_RW)                      { out |= RED_VK_ACCESS_BITFLAG_OUTPUT_DEPTH_STENCIL;       }
  if ((flags & RED_ACCESS_BITFLAG_OUTPUT_STENCIL_R)                     == RED_ACCESS_BITFLAG_OUTPUT_STENCIL_R)                     { out |= RED_VK_ACCESS_BITFLAG_OUTPUT_DEPTH_STENCIL;       }
  if ((flags & RED_ACCESS_BITFLAG_OUTPUT_STENCIL_RW)                    == RED_ACCESS_BITFLAG_OUTPUT_STENCIL_RW)                    { out |= RED_VK_ACCESS_BITFLAG_OUTPUT_DEPTH_STENCIL;       }
  if ((flags & RED_ACCESS_BITFLAG_OUTPUT_COLOR_W)                       == RED_ACCESS_BITFLAG_OUTPUT_COLOR_W)                       { out |= RED_VK_ACCESS_BITFLAG_OUTPUT_COLOR;               }
  if ((flags & RED_ACCESS_BITFLAG_RESOLVE_SOURCE_R)                     == RED_ACCESS_BITFLAG_RESOLVE_SOURCE_R)                     { out |= RED_VK_ACCESS_BITFLAG_RESOLVE_SOURCE_R;           }
  if ((flags & RED_ACCESS_BITFLAG_RESOLVE_TARGET_W)                     == RED_ACCESS_BITFLAG_RESOLVE_TARGET_W)                     { out |= RED_VK_ACCESS_BITFLAG_RESOLVE_TARGET_W;           }
  if ((flags & RED_ACCESS_BITFLAG_CPU_RW)                               == RED_ACCESS_BITFLAG_CPU_RW)                               { out |= RED_VK_ACCESS_BITFLAG_CPU;                        }
  return out;
}

REDGPU_DECLSPEC void REDGPU_API redCallUsageAliasOrderBarrier(RedTypeProcedureAddressCallUsageAliasOrderBarrier address, RedHandleCalls calls, RedContext context, unsigned arrayUsagesCount, const RedUsageArray * arrayUsages, unsigned imageUsagesCount, const RedUsageImage * imageUsages, unsigned aliasesCount, const RedAlias * aliases, unsigned ordersCount, const RedOrder * orders, RedBool32 dependencyByRegion) {
  RedInternalContext ctx = (RedInternalContext)context;

  RedVkUsageArray * barriersArray = NULL;
  RedVkUsageImage * barriersImage = NULL;

  if (arrayUsagesCount > 0) {
    barriersArray = (RedVkUsageArray *)redInternalAdaptiveMalloc(ctx->internal.cold->customMalloc, ctx->internal.cold->customMallocTagged, arrayUsagesCount * sizeof(RedVkUsageArray), RED_PROCEDURE_ID_UNDEFINED, 100, NULL, NULL, 0, NULL);
  }
  if (imageUsagesCount > 0) {
    barriersImage = (RedVkUsageImage *)redInternalAdaptiveMalloc(ctx->internal.cold->customMalloc, ctx->internal.cold->customMallocTagged, imageUsagesCount * sizeof(RedVkUsageImage), RED_PROCEDURE_ID_UNDEFINED, 101, NULL, NULL, 0, NULL);
  }

  RedVkAccessStageBitflags oldAccessStages = 0;
  RedVkAccessStageBitflags newAccessStages = 0;

  for (unsigned i = 0; i < arrayUsagesCount; i += 1) {
    const RedUsageArray usage = arrayUsages[i];
    oldAccessStages |= redInternalRedAccessStagesToRedVkAccessStages(usage.oldAccessStages);
    newAccessStages |= redInternalRedAccessStagesToRedVkAccessStages(usage.newAccessStages);
    barriersArray[i].setTo44                = 44;
    barriersArray[i].setTo0                 = 0;
    barriersArray[i].oldAccesses            = redInternalRedAccessToRedVkAccess(usage.oldAccess);
    barriersArray[i].newAccesses            = redInternalRedAccessToRedVkAccess(usage.newAccess);
    barriersArray[i].queueFamilyIndexSource = usage.queueFamilyIndexSource;
    barriersArray[i].queueFamilyIndexTarget = usage.queueFamilyIndexTarget;
    barriersArray[i].array                  = usage.array;
    barriersArray[i].arrayBytesFirst        = usage.arrayBytesFirst;
    barriersArray[i].arrayBytesCount        = usage.arrayBytesCount;
  }

  for (unsigned i = 0; i < imageUsagesCount; i += 1) {
    const RedUsageImage usage = imageUsages[i];
    oldAccessStages |= redInternalRedAccessStagesToRedVkAccessStages(usage.oldAccessStages);
    newAccessStages |= redInternalRedAccessStagesToRedVkAccessStages(usage.newAccessStages);
    barriersImage[i].setTo45                = 45;
    barriersImage[i].setTo0                 = 0;
    barriersImage[i].oldAccesses            = redInternalRedAccessToRedVkAccess(usage.oldAccess);
    barriersImage[i].newAccesses            = redInternalRedAccessToRedVkAccess(usage.newAccess);
    barriersImage[i].oldState               = usage.oldState;
    barriersImage[i].newState               = usage.newState;
    barriersImage[i].queueFamilyIndexSource = usage.queueFamilyIndexSource;
    barriersImage[i].queueFamilyIndexTarget = usage.queueFamilyIndexTarget;
    barriersImage[i].image                  = usage.image;
    barriersImage[i].imageParts.allParts    = usage.imageAllParts;
    barriersImage[i].imageParts.levelsFirst = usage.imageLevelsFirst;
    barriersImage[i].imageParts.levelsCount = usage.imageLevelsCount;
    barriersImage[i].imageParts.layersFirst = usage.imageLayersFirst;
    barriersImage[i].imageParts.layersCount = usage.imageLayersCount;
  }

  redInlineCallUsageOrderBarrier(address, calls, oldAccessStages, newAccessStages, arrayUsagesCount, barriersArray, imageUsagesCount, barriersImage, dependencyByRegion);

  redInternalAdaptiveFree(ctx->internal.cold->customFree, ctx->internal.cold->customFreeTagged, barriersImage, RED_PROCEDURE_ID_UNDEFINED, 102, NULL, NULL, 0, NULL);
  redInternalAdaptiveFree(ctx->internal.cold->customFree, ctx->internal.cold->customFreeTagged, barriersArray, RED_PROCEDURE_ID_UNDEFINED, 103, NULL, NULL, 0, NULL);
}

REDGPU_DECLSPEC void REDGPU_API redCallMark(RedTypeProcedureAddressCallMark address, RedHandleCalls calls, const char * mark) {
  redInlineCallMark(address, calls, mark);
}

REDGPU_DECLSPEC void REDGPU_API redCallMarkSet(RedTypeProcedureAddressCallMarkSet address, RedHandleCalls calls, const char * mark) {
  redInlineCallMarkSet(address, calls, mark);
}

REDGPU_DECLSPEC void REDGPU_API redCallMarkEnd(RedTypeProcedureAddressCallMarkEnd address, RedHandleCalls calls) {
  redInlineCallMarkEnd(address, calls);
}

REDGPU_DECLSPEC void REDGPU_API redQueueSubmit(RedContext context, RedHandleGpu gpu, RedHandleQueue queue, unsigned timelinesCount, const RedGpuTimeline * timelines, RedHandleCpuSignal signalCpuSignal, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData) {
  const RedProcedureId procedureId = RED_PROCEDURE_ID_redQueueSubmit;
  redInternalMarkSet(__FUNCTION__);
  redInlineQueueSubmit(context, gpu, queue, timelinesCount, timelines, signalCpuSignal, outStatuses, optionalFile, optionalLine, optionalUserData);
  REDGPU_INTERNAL_FINALIZE_OUTSTATUSES
  redInternalMarkEnd();
}

REDGPU_DECLSPEC void REDGPU_API redMark(const char * mark, const char * optionalFile, int optionalLine, void * optionalUserData) {
  redInlineMark(mark, optionalFile, optionalLine, optionalUserData);
}

REDGPU_DECLSPEC void REDGPU_API redMarkSet(const char * mark, const char * optionalFile, int optionalLine, void * optionalUserData) {
  redInlineMarkSet(mark, optionalFile, optionalLine, optionalUserData);
}

REDGPU_DECLSPEC void REDGPU_API redMarkEnd(const char * optionalFile, int optionalLine, void * optionalUserData) {
  redInlineMarkEnd(optionalFile, optionalLine, optionalUserData);
}

REDGPU_DECLSPEC void REDGPU_API redGpuVisTracingStart(const char * optionalFile, int optionalLine, void * optionalUserData) {
  redInternalMarkSet(__FUNCTION__);
  redInlineGpuVisTracingStart(optionalFile, optionalLine, optionalUserData);
  redInternalMarkEnd();
}

REDGPU_DECLSPEC void REDGPU_API redGpuVisTracingCapture(const char * optionalFile, int optionalLine, void * optionalUserData) {
  redInternalMarkSet(__FUNCTION__);
  redInlineGpuVisTracingCapture(optionalFile, optionalLine, optionalUserData);
  redInternalMarkEnd();
}

REDGPU_DECLSPEC void REDGPU_API redGpuVisTracingStop(const char * optionalFile, int optionalLine, void * optionalUserData) {
  redInternalMarkSet(__FUNCTION__);
  redInlineGpuVisTracingStop(optionalFile, optionalLine, optionalUserData);
  redInternalMarkEnd();
}

REDGPU_DECLSPEC void REDGPU_API redCreateSurfaceWin32(RedContext context, RedHandleGpu gpu, const char * handleName, const void * win32Hinstance, const void * win32Hwnd, RedHandleSurface * outSurface, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData) {
  const RedProcedureId procedureId = RED_PROCEDURE_ID_redCreateSurfaceWin32;
  redInternalMarkSet(__FUNCTION__);
  RedInternalContext ctx = (RedInternalContext)context;
#if defined(VK_USE_PLATFORM_WIN32_KHR)
  redInlineCreateSurfaceWin32(context, gpu, handleName, win32Hinstance, win32Hwnd, outSurface, outStatuses, optionalFile, optionalLine, optionalUserData);
#endif
#if defined(VK_USE_PLATFORM_XLIB_KHR) && defined(VK_USE_PLATFORM_XCB_KHR)
  redInternalSetStatus(ctx, gpu, outStatuses, 359, (VkResult)RED_STATUS_ERROR_FEATURE_IS_NOT_FOUND, procedureId, optionalFile, optionalLine, optionalUserData);
#endif
  REDGPU_INTERNAL_FINALIZE_OUTSTATUSES
  redInternalMarkEnd();
}

REDGPU_DECLSPEC void REDGPU_API redCreateSurfaceXlibOrXcb(RedContext context, RedHandleGpu gpu, const char * handleName, const void * xlibDisplay, unsigned long xlibWindow, const void * xcbConnection, unsigned xcbWindow, RedHandleSurface * outSurface, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData) {
  const RedProcedureId procedureId = RED_PROCEDURE_ID_redCreateSurfaceXlibOrXcb;
  redInternalMarkSet(__FUNCTION__);
  RedInternalContext ctx = (RedInternalContext)context;
#if defined(VK_USE_PLATFORM_WIN32_KHR)
  redInternalSetStatus(ctx, gpu, outStatuses, 362, (VkResult)RED_STATUS_ERROR_FEATURE_IS_NOT_FOUND, procedureId, optionalFile, optionalLine, optionalUserData);
#endif
#if defined(VK_USE_PLATFORM_XLIB_KHR) && defined(VK_USE_PLATFORM_XCB_KHR)
  redInlineCreateSurfaceXlibOrXcb(context, gpu, handleName, xlibDisplay, xlibWindow, xcbConnection, xcbWindow, outSurface, outStatuses, optionalFile, optionalLine, optionalUserData);
#endif
  REDGPU_INTERNAL_FINALIZE_OUTSTATUSES
  redInternalMarkEnd();
}

REDGPU_DECLSPEC void REDGPU_API redCreatePresent(RedContext context, RedHandleGpu gpu, RedHandleQueue queue, const char * handleName, RedHandleSurface surface, unsigned imagesCount, unsigned imagesWidth, unsigned imagesHeight, unsigned imagesLayersCount, RedAccessBitflags imagesRestrictToAccess, RedSurfaceTransformBitflag transform, RedSurfaceCompositeAlphaBitflag compositeAlpha, RedPresentVsyncMode vsyncMode, RedBool32 clipped, RedBool32 discardAfterPresent, RedHandlePresent oldPresent, RedHandlePresent * outPresent, RedHandleImage * outImages, RedHandleTexture * outTextures, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData) {
  const RedProcedureId procedureId = RED_PROCEDURE_ID_redCreatePresent;
  redInternalMarkSet(__FUNCTION__);

  RedInternalContext ctx = (RedInternalContext)context;

  {
    unsigned gpuIndex = (unsigned)-1;
    for (unsigned i = 0; i < ctx->internal.hotGpuHandlesCount; i += 1) {
      if (ctx->internal.hotGpuHandles[i] == gpu) {
        gpuIndex = i;
        break;
      }
    }

    if (gpuIndex == (unsigned)-1) {
      redInternalSetStatus(ctx, gpu, outStatuses, 360, (VkResult)RED_STATUS_ERROR_INITIALIZATION_FAILED, procedureId, optionalFile, optionalLine, optionalUserData);
      goto exit;
    }

    unsigned queueIndex = (unsigned)-1;
    for (unsigned i = 0, count = ctx->gpus[gpuIndex].queuesCount; i < count; i += 1) {
      if (ctx->gpus[gpuIndex].queues[i] == queue) {
        queueIndex = i;
        break;
      }
    }

    if (queueIndex == (unsigned)-1) {
      redInternalSetStatus(ctx, gpu, outStatuses, 361, (VkResult)RED_STATUS_ERROR_INITIALIZATION_FAILED, procedureId, optionalFile, optionalLine, optionalUserData);
      goto exit;
    }

    {
      unsigned initialQueueFamilyIndex = ctx->gpus[gpuIndex].queuesFamilyIndex[queueIndex];

      redInlineCreatePresent(context, gpu, handleName, surface, imagesCount, imagesWidth, imagesHeight, imagesLayersCount, imagesRestrictToAccess, initialQueueFamilyIndex, transform, compositeAlpha, vsyncMode, clipped, oldPresent, outPresent, outImages, outTextures, outStatuses, optionalFile, optionalLine, optionalUserData);
    }
  }

exit:;

  REDGPU_INTERNAL_FINALIZE_OUTSTATUSES

  redInternalMarkEnd();
}

REDGPU_DECLSPEC void REDGPU_API redDestroySurface(RedContext context, RedHandleGpu gpu, RedHandleSurface surface, const char * optionalFile, int optionalLine, void * optionalUserData) {
  redInternalMarkSet(__FUNCTION__);
  redInlineDestroySurface(context, gpu, surface, optionalFile, optionalLine, optionalUserData);
  redInternalMarkEnd();
}

REDGPU_DECLSPEC void REDGPU_API redDestroyPresent(RedContext context, RedHandleGpu gpu, RedHandlePresent present, const char * optionalFile, int optionalLine, void * optionalUserData) {
  redInternalMarkSet(__FUNCTION__);
  redInlineDestroyPresent(context, gpu, present, optionalFile, optionalLine, optionalUserData);
  redInternalMarkEnd();
}

REDGPU_DECLSPEC void REDGPU_API redQueueFamilyIndexGetSupportsPresent(RedContext context, RedHandleGpu gpu, unsigned queueFamilyIndex, RedQueueFamilyIndexGetSupportsPresentOnWin32 * supportsPresentOnWin32, RedQueueFamilyIndexGetSupportsPresentOnXlib * supportsPresentOnXlib, RedQueueFamilyIndexGetSupportsPresentOnXcb * supportsPresentOnXcb, RedQueueFamilyIndexGetSupportsPresentOnSurface * supportsPresentOnSurface, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData) {
  const RedProcedureId procedureId = RED_PROCEDURE_ID_redQueueFamilyIndexGetSupportsPresent;
  redInternalMarkSet(__FUNCTION__);
  redInlineQueueFamilyIndexGetSupportsPresent(context, gpu, queueFamilyIndex, supportsPresentOnWin32, supportsPresentOnXlib, supportsPresentOnXcb, supportsPresentOnSurface, outStatuses, optionalFile, optionalLine, optionalUserData);
  REDGPU_INTERNAL_FINALIZE_OUTSTATUSES
  redInternalMarkEnd();
}

REDGPU_DECLSPEC void REDGPU_API redSurfaceGetPresentFeatures(RedContext context, RedHandleGpu gpu, RedHandleSurface surface, RedSurfacePresentFeatures * outSurfacePresentFeatures, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData) {
  const RedProcedureId procedureId = RED_PROCEDURE_ID_redSurfaceGetPresentFeatures;
  redInternalMarkSet(__FUNCTION__);
  redInlineSurfaceGetPresentFeatures(context, gpu, surface, outSurfacePresentFeatures, outStatuses, optionalFile, optionalLine, optionalUserData);
  REDGPU_INTERNAL_FINALIZE_OUTSTATUSES
  redInternalMarkEnd();
}

REDGPU_DECLSPEC void REDGPU_API redSurfaceGetCurrentPropertiesAndPresentLimits(RedContext context, RedHandleGpu gpu, RedHandleSurface surface, RedSurfaceCurrentPropertiesAndPresentLimits * outSurfaceCurrentPropertiesAndPresentLimits, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData) {
  const RedProcedureId procedureId = RED_PROCEDURE_ID_redSurfaceGetCurrentPropertiesAndPresentLimits;
  redInternalMarkSet(__FUNCTION__);
  redInlineSurfaceGetCurrentPropertiesAndPresentLimits(context, gpu, surface, outSurfaceCurrentPropertiesAndPresentLimits, outStatuses, optionalFile, optionalLine, optionalUserData);
  REDGPU_INTERNAL_FINALIZE_OUTSTATUSES
  redInternalMarkEnd();
}

REDGPU_DECLSPEC void REDGPU_API redPresentGetImageIndex(RedContext context, RedHandleGpu gpu, RedHandlePresent present, RedHandleCpuSignal signalCpuSignal, RedHandleGpuSignal signalGpuSignal, unsigned * outImageIndex, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData) {
  const RedProcedureId procedureId = RED_PROCEDURE_ID_redPresentGetImageIndex;
  redInternalMarkSet(__FUNCTION__);
  redInlinePresentGetImageIndex(context, gpu, present, signalCpuSignal, signalGpuSignal, outImageIndex, outStatuses, optionalFile, optionalLine, optionalUserData);
  REDGPU_INTERNAL_FINALIZE_OUTSTATUSES
  redInternalMarkEnd();
}

REDGPU_DECLSPEC void REDGPU_API redQueuePresent(RedContext context, RedHandleGpu gpu, RedHandleQueue queue, unsigned waitForAndUnsignalGpuSignalsCount, const RedHandleGpuSignal * waitForAndUnsignalGpuSignals, unsigned presentsCount, const RedHandlePresent * presents, const unsigned * presentsImageIndex, RedStatus * outPresentsStatus, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData) {
  const RedProcedureId procedureId = RED_PROCEDURE_ID_redQueuePresent;
  redInternalMarkSet(__FUNCTION__);
  redInlineQueuePresent(context, gpu, queue, waitForAndUnsignalGpuSignalsCount, waitForAndUnsignalGpuSignals, presentsCount, presents, presentsImageIndex, outPresentsStatus, outStatuses, optionalFile, optionalLine, optionalUserData);
  REDGPU_INTERNAL_FINALIZE_OUTSTATUSES
  redInternalMarkEnd();
}

REDGPU_DECLSPEC RedHandleArray REDGPU_API redDebugArrayGetHandle(RedContext context, RedHandleGpu gpu, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData) {
  const RedProcedureId procedureId = RED_PROCEDURE_ID_redDebugArrayGetHandle;
  RedHandleArray out = redInlineDebugArrayGetHandle(context, gpu, outStatuses, optionalFile, optionalLine, optionalUserData);
  REDGPU_INTERNAL_FINALIZE_OUTSTATUSES
  return out;
}

REDGPU_DECLSPEC void REDGPU_API redDebugArrayCallPrint(RedContext context, RedHandleGpu gpu, RedHandleCalls calls, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData) {
  const RedProcedureId procedureId = RED_PROCEDURE_ID_redDebugArrayCallPrint;
  redInlineDebugArrayCallPrint(context, gpu, calls, outStatuses, optionalFile, optionalLine, optionalUserData);
  REDGPU_INTERNAL_FINALIZE_OUTSTATUSES
}

REDGPU_DECLSPEC void REDGPU_API redCreateArrayTimestamp(RedContext context, RedHandleGpu gpu, const char * handleName, unsigned count, RedHandleArrayTimestamp * outArrayTimestamp, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData) {
  const RedProcedureId procedureId = (const RedProcedureId)RED_PROCEDURE_ID_redCreateArrayTimestamp;
  redInternalMarkSet(__FUNCTION__);
  redInlineCreateArrayTimestamp(context, gpu, handleName, count, outArrayTimestamp, outStatuses, optionalFile, optionalLine, optionalUserData);
  REDGPU_INTERNAL_FINALIZE_OUTSTATUSES
  redInternalMarkEnd();
}

REDGPU_DECLSPEC void REDGPU_API redDestroyArrayTimestamp(RedContext context, RedHandleGpu gpu, RedHandleArrayTimestamp arrayTimestamp, const char * optionalFile, int optionalLine, void * optionalUserData) {
  redInternalMarkSet(__FUNCTION__);
  redInlineDestroyArrayTimestamp(context, gpu, arrayTimestamp, optionalFile, optionalLine, optionalUserData);
  redInternalMarkEnd();
}

REDGPU_DECLSPEC void REDGPU_API redCallArrayTimestampWrite(RedHandleCalls calls, RedContext context, RedHandleArrayTimestamp arrayTimestamp, unsigned index) {
  RedInternalContext ctx = (RedInternalContext)context;
  RedInternalInstanceProcedureAddresses * vk = ctx->internal.instanceProcedureAddresses;
  vk->vkCmdResetQueryPool((VkCommandBuffer)calls, (VkQueryPool)arrayTimestamp, index, 1);
  vk->vkCmdWriteTimestamp((VkCommandBuffer)calls, VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, (VkQueryPool)arrayTimestamp, index);
}

REDGPU_DECLSPEC void REDGPU_API redArrayTimestampRead(RedContext context, RedHandleGpu gpu, RedHandleArrayTimestamp arrayTimestamp, unsigned rangeFirst, unsigned rangeCount, void * out64BitTicksCounts, RedStatuses * outStatuses, const char * optionalFile, int optionalLine, void * optionalUserData) {
  const RedProcedureId procedureId = (const RedProcedureId)RED_PROCEDURE_ID_redArrayTimestampRead;
  redInternalMarkSet(__FUNCTION__);
  redInlineArrayTimestampRead(context, gpu, arrayTimestamp, rangeFirst, rangeCount, out64BitTicksCounts, outStatuses, optionalFile, optionalLine, optionalUserData);
  REDGPU_INTERNAL_FINALIZE_OUTSTATUSES
  redInternalMarkEnd();
}

#ifdef __cplusplus
} // extern "C" {
#endif

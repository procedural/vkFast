#pragma once

#include "named_parameters.h"

#include <stddef.h> // For size_t
#include <stdint.h> // For uint64_t

#ifdef __cplusplus
extern "C" {
#endif

// C/C++ cross-compatible inline struct literals
#ifdef __cplusplus
  #define REDGPU_32_STRUCT(STRUCT_TYPE, ...) STRUCT_TYPE{__VA_ARGS__}
#else
  #define REDGPU_32_STRUCT(STRUCT_TYPE, ...) (struct STRUCT_TYPE){__VA_ARGS__}
#endif

#define REDGPU_32_BYTES_TO_NEXT_ALIGNMENT_BOUNDARY(CURRENT_BYTES, ALIGNMENT) ( ((ALIGNMENT) - (CURRENT_BYTES) % ((ALIGNMENT) ? (ALIGNMENT) : 1)) % ((ALIGNMENT) ? (ALIGNMENT) : 1) )

#ifndef REDGPU_32_DECLSPEC
#define REDGPU_32_DECLSPEC
#endif

#ifndef REDGPU_32_API
#define REDGPU_32_API
#endif

npf(REDGPU_32_DECLSPEC void * REDGPU_32_API, red32MemorySet,            void *,pointer, int,value, size_t,bytesCount);
npf(REDGPU_32_DECLSPEC void * REDGPU_32_API, red32MemoryCopy,           void *,pointerCopyTo, const void *,pointerCopyFrom, size_t,bytesCount);
npf(REDGPU_32_DECLSPEC void * REDGPU_32_API, red32MemoryCallocAligned,  size_t,bytesCount, size_t,alignment);
npf(REDGPU_32_DECLSPEC void * REDGPU_32_API, red32MemoryCalloc,         size_t,bytesCount);
npf(REDGPU_32_DECLSPEC void * REDGPU_32_API, red32MemoryRealloc,        void *,pointer, size_t,newSizeBytesCount, size_t,oldSizeBytesCount);
npf(REDGPU_32_DECLSPEC void * REDGPU_32_API, red32MemoryReallocAligned, void *,pointer, size_t,newSizeBytesCount, size_t,oldSizeBytesCount, size_t,alignment);
npf(REDGPU_32_DECLSPEC void   REDGPU_32_API, red32MemoryFree,           void *,pointer);
npf(REDGPU_32_DECLSPEC void * REDGPU_32_API, red32GetModuleHandle,      const char *,moduleName);
npf(REDGPU_32_DECLSPEC void * REDGPU_32_API, red32WindowCreate,         const char *,title);
npf(REDGPU_32_DECLSPEC int    REDGPU_32_API, red32WindowDestroy,        void *,windowHandle);
npf(REDGPU_32_DECLSPEC int    REDGPU_32_API, red32WindowLoop,           void *,windowHandle);
npf(REDGPU_32_DECLSPEC void   REDGPU_32_API, red32ConsolePrint,         const char *,string);
npf(REDGPU_32_DECLSPEC void   REDGPU_32_API, red32ConsolePrintError,    const char *,string);
npf(REDGPU_32_DECLSPEC int    REDGPU_32_API, red32FileMap,              const unsigned short *,filepath, void **,outFileDescriptorHandle, void **,outFileMappingHandle, void **,outFileDataPointer);
npf(REDGPU_32_DECLSPEC int    REDGPU_32_API, red32FileUnmap,            void *,fileHandle, void *,fileMappingDescriptorHandle, void *,fileMapping);
npf(REDGPU_32_DECLSPEC void   REDGPU_32_API, red32OutputDebugString,    const char *,string);
npf(REDGPU_32_DECLSPEC void   REDGPU_32_API, red32Exit,                 int,exitCode);

npf(REDGPU_32_DECLSPEC int    REDGPU_32_API, red32IntToChars,           int,value, char *,outChars);
npf(REDGPU_32_DECLSPEC int    REDGPU_32_API, red32UnsignedToChars,      unsigned,value, char *,outChars);
npf(REDGPU_32_DECLSPEC int    REDGPU_32_API, red32Int64ToChars,         int64_t,value, char *,outChars);
npf(REDGPU_32_DECLSPEC int    REDGPU_32_API, red32Uint64ToChars,        uint64_t,value, char *,outChars);
npf(REDGPU_32_DECLSPEC int    REDGPU_32_API, red32FloatToChars,         float,value, char *,outChars);
npf(REDGPU_32_DECLSPEC int    REDGPU_32_API, red32DoubleToChars,        double,value, char *,outChars);

npf(REDGPU_32_DECLSPEC void   REDGPU_32_API, red32BinaryUint8ToCharsNoNullTerm,    uint8_t,value, char *,outCharsNoNullTerm);
npf(REDGPU_32_DECLSPEC void   REDGPU_32_API, red32BinaryUnsignedToCharsNoNullTerm, unsigned,value, char *,outCharsNoNullTerm);
npf(REDGPU_32_DECLSPEC void   REDGPU_32_API, red32BinaryUint64ToCharsNoNullTerm,   uint64_t,value, char *,outCharsNoNullTerm);

npf(REDGPU_32_DECLSPEC void   REDGPU_32_API, red32HexUint8ToCharsNoNullTerm,       uint8_t,value, char *,outCharsNoNullTerm);
npf(REDGPU_32_DECLSPEC void   REDGPU_32_API, red32HexUnsignedToCharsNoNullTerm,    unsigned,value, char *,outCharsNoNullTerm);
npf(REDGPU_32_DECLSPEC void   REDGPU_32_API, red32HexUint64ToCharsNoNullTerm,      uint64_t,value, char *,outCharsNoNullTerm);

#ifdef __cplusplus
}
#endif

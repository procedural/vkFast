#pragma once

#include <stddef.h> // For size_t
#include <stdint.h> // For uint64_t

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
#define REDGPU_32_STRUCT(STRUCT_TYPE, ...) STRUCT_TYPE{__VA_ARGS__}
#else
#define REDGPU_32_STRUCT(STRUCT_TYPE, ...) ((struct STRUCT_TYPE){__VA_ARGS__})
#endif

#define REDGPU_32_BYTES_TO_NEXT_ALIGNMENT_BOUNDARY(CURRENT_BYTES, ALIGNMENT) ( ((ALIGNMENT) - (CURRENT_BYTES) % ((ALIGNMENT) ? (ALIGNMENT) : 1)) % ((ALIGNMENT) ? (ALIGNMENT) : 1) )

// Dynamic array example:
//
// typedef struct MyDynamicArray {
//   MyItem * items;
//   size_t count;     // Do not modify the count value directly, rather use REDGPU_32_DYNAMIC_ARRAY_RESIZE
//   size_t capacity;
//   size_t alignment;
// } MyDynamicArray;
#define REDGPU_32_DYNAMIC_ARRAY_APPEND_PREPARE(ARRAY)\
  do {\
    if (ARRAY.alignment == 0) { ARRAY.alignment = 1; }\
    if (ARRAY.count >= ARRAY.capacity) {\
      if (ARRAY.capacity == 0) {\
        ARRAY.capacity = 256;\
      } else {\
        if (ARRAY.capacity == 1) {\
          ARRAY.capacity = 2;\
        } else {\
          ARRAY.capacity *= 2;\
        }\
      }\
      void ** items = (void **)&ARRAY.items;\
      items[0] = red32MemoryReallocAligned(ARRAY.items, ARRAY.capacity * sizeof(ARRAY.items[0]), ARRAY.count * sizeof(ARRAY.items[0]), ARRAY.alignment);\
    }\
  } while(0)

#define REDGPU_32_DYNAMIC_ARRAY_APPEND(ARRAY, ELEMENT)\
  do {\
    REDGPU_32_DYNAMIC_ARRAY_APPEND_PREPARE(ARRAY);\
    if (ARRAY.items != 0) {\
      ARRAY.items[ARRAY.count++] = ELEMENT;\
    }\
  } while(0)

#define REDGPU_32_DYNAMIC_ARRAY_RESIZE(ARRAY, NEW_SIZE)\
  do {\
    if (ARRAY.alignment == 0) { ARRAY.alignment = 1; }\
    ARRAY.capacity = NEW_SIZE;\
    if (NEW_SIZE == 0) {\
      red32MemoryFree(ARRAY.items);\
      ARRAY.items = 0;\
    } else {\
      void ** items = (void **)&ARRAY.items;\
      items[0] = red32MemoryReallocAligned(ARRAY.items, ARRAY.capacity * sizeof(ARRAY.items[0]), ARRAY.count * sizeof(ARRAY.items[0]), ARRAY.alignment);\
    }\
    if (ARRAY.count > ARRAY.capacity) { ARRAY.count = ARRAY.capacity; }\
  } while(0)

#define REDGPU_32_DYNAMIC_ARRAY_STRING_JOIN(ARRAY_STRING, JOIN_STRING)\
  if ((JOIN_STRING) != 0) {\
    if (ARRAY_STRING.count > 0) {\
      ARRAY_STRING.count -= 1;\
    }\
    for (size_t i = 0; ; i += 1) {\
      REDGPU_32_DYNAMIC_ARRAY_APPEND(ARRAY_STRING, (JOIN_STRING)[i]);\
      if ((JOIN_STRING)[i] == 0) {\
        break;\
      }\
    }\
  }

#define REDGPU_32_DYNAMIC_ARRAY_FREE(ARRAY)\
  do {\
    red32MemoryFree(ARRAY.items);\
    ARRAY.items = 0;\
    ARRAY.count = 0;\
    ARRAY.capacity = 0;\
  } while(0)

#ifndef REDGPU_32_DECLSPEC
#define REDGPU_32_DECLSPEC
#endif

#ifndef REDGPU_32_API
#define REDGPU_32_API
#endif

REDGPU_32_DECLSPEC void *   REDGPU_32_API red32MemorySet           (void * pointer, int value, size_t bytesCount);
REDGPU_32_DECLSPEC void *   REDGPU_32_API red32MemoryCopy          (void * pointerCopyTo, const void * pointerCopyFrom, size_t bytesCount);
REDGPU_32_DECLSPEC void *   REDGPU_32_API red32MemoryCallocAligned (size_t bytesCount, size_t alignment);
REDGPU_32_DECLSPEC void *   REDGPU_32_API red32MemoryCalloc        (size_t bytesCount);
REDGPU_32_DECLSPEC void *   REDGPU_32_API red32MemoryRealloc       (void * pointer, size_t newSizeBytesCount, size_t oldSizeBytesCount);
REDGPU_32_DECLSPEC void *   REDGPU_32_API red32MemoryReallocAligned(void * pointer, size_t newSizeBytesCount, size_t oldSizeBytesCount, size_t alignment);
REDGPU_32_DECLSPEC void     REDGPU_32_API red32MemoryFree          (void * pointer);
REDGPU_32_DECLSPEC void *   REDGPU_32_API red32GetModuleHandle     (const char * moduleName);
REDGPU_32_DECLSPEC void *   REDGPU_32_API red32WindowCreate        (const char * title);
REDGPU_32_DECLSPEC int      REDGPU_32_API red32WindowDestroy       (void * windowHandle);
REDGPU_32_DECLSPEC int      REDGPU_32_API red32WindowLoop          (void * windowHandle);
REDGPU_32_DECLSPEC void     REDGPU_32_API red32ConsolePrint        (const char * string);
REDGPU_32_DECLSPEC void     REDGPU_32_API red32ConsolePrintError   (const char * string);
REDGPU_32_DECLSPEC int      REDGPU_32_API red32FileMap             (const unsigned short * filepath, void ** outFileDescriptorHandle, void ** outFileMappingHandle, void ** outFileDataPointer);
REDGPU_32_DECLSPEC int      REDGPU_32_API red32FileUnmap           (void * fileHandle, void * fileMappingDescriptorHandle, void * fileMapping);
REDGPU_32_DECLSPEC void     REDGPU_32_API red32OutputDebugString   (const char * string);
REDGPU_32_DECLSPEC void     REDGPU_32_API red32Exit                (int exitCode);

REDGPU_32_DECLSPEC int      REDGPU_32_API red32IntToChars          (int value, char * outChars);
REDGPU_32_DECLSPEC int      REDGPU_32_API red32UnsignedToChars     (unsigned value, char * outChars);
REDGPU_32_DECLSPEC int      REDGPU_32_API red32Int64ToChars        (int64_t value, char * outChars);
REDGPU_32_DECLSPEC int      REDGPU_32_API red32Uint64ToChars       (uint64_t value, char * outChars);
REDGPU_32_DECLSPEC int      REDGPU_32_API red32FloatToChars        (float value, char * outChars);
REDGPU_32_DECLSPEC int      REDGPU_32_API red32DoubleToChars       (double value, char * outChars);

REDGPU_32_DECLSPEC void     REDGPU_32_API red32BinaryUint8ToCharsNoNullTerm    (uint8_t value, char * outCharsNoNullTerm);
REDGPU_32_DECLSPEC void     REDGPU_32_API red32BinaryUnsignedToCharsNoNullTerm (unsigned value, char * outCharsNoNullTerm);
REDGPU_32_DECLSPEC void     REDGPU_32_API red32BinaryUint64ToCharsNoNullTerm   (uint64_t value, char * outCharsNoNullTerm);

REDGPU_32_DECLSPEC void     REDGPU_32_API red32HexUint8ToCharsNoNullTerm       (uint8_t value, char * outCharsNoNullTerm);
REDGPU_32_DECLSPEC void     REDGPU_32_API red32HexUnsignedToCharsNoNullTerm    (unsigned value, char * outCharsNoNullTerm);
REDGPU_32_DECLSPEC void     REDGPU_32_API red32HexUint64ToCharsNoNullTerm      (uint64_t value, char * outCharsNoNullTerm);

#ifdef __cplusplus
}
#endif

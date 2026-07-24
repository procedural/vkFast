#pragma once

extern wchar_t const * g_arc_compilerLicense;

#if !defined(ARC_INTERNAL_COMPILER_TESTS) && !defined(ARC_INTERNAL_COMPILER_TESTS_IGNORE)
#error ARC_INTERNAL_COMPILER_TESTS{_IGNORE} undefined
#endif
#if !defined(ARC_INTERNAL_COMPILER_DEBUG) && !defined(ARC_INTERNAL_COMPILER_DEBUG_IGNORE)
#error ARC_INTERNAL_COMPILER_DEBUG{_IGNORE} undefined
#endif

#define ARC_COMPILER_OS_UNDEFINED 0
#define ARC_COMPILER_OS_LINUX     1
#define ARC_COMPILER_OS_WINDOWS   2

#ifndef ARC_COMPILER_OS
  #if defined(__linux__) && !defined(__ANDROID__)
    #define ARC_COMPILER_OS ARC_COMPILER_OS_LINUX
  #endif
  #if defined(_WIN32)
    #define ARC_COMPILER_OS ARC_COMPILER_OS_WINDOWS
  #endif
#endif

#ifndef ARC_COMPILER_OS
  #error ARC_COMPILER_OS was not defined.
#endif

#if ARC_COMPILER_OS == ARC_COMPILER_OS_UNDEFINED
  #error ARC_COMPILER_OS == ARC_COMPILER_OS_UNDEFINED (0)
#endif

#define ARC_INTERNAL_CPP_VERSION_17 201703L

#ifdef _MSVC_LANG
  #define ARC_INTERNAL_CPP_VERSION _MSVC_LANG
#else
  #define ARC_INTERNAL_CPP_VERSION __cplusplus
#endif

#if ARC_INTERNAL_CPP_VERSION >= ARC_INTERNAL_CPP_VERSION_17
  #define arc_static_assert static_assert
#else
  #define arc_static_assert assert
#endif

#if ARC_COMPILER_OS == ARC_COMPILER_OS_WINDOWS
#include <Windows.h>  // For SetConsoleOutputCP
#endif
#if ARC_COMPILER_OS == ARC_COMPILER_OS_LINUX
#include <filesystem> // For std::filesystem::path
#endif
#include <assert.h>   // For assert
#include <stdint.h>   // For uint64_t
#include <stdio.h>    // For wprintf
#include <sstream>    // For std::wostringstream
#include <fstream>    // For std::wifstream
#include <string>     // For std::wstring
#include <vector>     // For std::vector

#define arc_wprintf_cli        wprintf
#define arc_wprintf_fatalError wprintf
#define arc_wprintf_info       wprintf
#define arc_wprintf_debug      wprintf
#define arc_wprintf_verbose    if (arcVerboseIsEnabled == 1) wprintf

typedef unsigned char ArcBool8;

#include "arc_state/arc_state_stage1.inl"
#include "arc_state/arc_state_rawbuild.inl"

typedef struct ArcState {
  ArcStateStage1 stage1;
} ArcState;

void arcStage1(ArcState * state, int ArgsCount, wchar_t * const * const Args);
void arcRawbuild(ArcState * state);
void arcStage2(ArcState * state);

// Common

#include <sys/types.h>
#include <sys/stat.h>

// Handle the naming differences between Windows and Linux/POSIX
#if defined(_WIN32) || defined(_WIN64)
  #define ARC_COMMON_STAT_STRUCT struct _stat
  #define ARC_COMMON_STAT_FUNC _stat
  // Windows defines S_IFDIR and S_IFREG, but lacks the POSIX macros
  #ifndef S_ISDIR
    #define S_ISDIR(mode) (((mode) & _S_IFMT) == _S_IFDIR)
  #endif
  #ifndef S_ISREG
    #define S_ISREG(mode) (((mode) & _S_IFMT) == _S_IFREG)
  #endif
#else
  #include <unistd.h>
  #define ARC_COMMON_STAT_STRUCT struct stat
  #define ARC_COMMON_STAT_FUNC stat
#endif

const char * const arcCommonMallocWcharToChar(const wchar_t * const wstr) {
  // Determine required buffer size (passing NULL as the destination)
  size_t size = wcstombs(NULL, wstr, 0);
  if (size == (size_t)-1) {
    return NULL;
  }

  // Allocate memory for the destination char string (+1 for null-terminator)
  char * str = (char *)calloc(1, size + 1);
  if (str == NULL) {
    return NULL;
  }

  // Perform the conversion
  wcstombs(str, wstr, size + 1);

  return (const char * const)str;
}

// Checks the system path type.
// Returns: 0 if Regular File, 1 if Folder/Directory, -1 if Path Not Found/Error/Other Type
int arcCommonGetSystemPathType(const char * const path) {
  ARC_COMMON_STAT_STRUCT info = {0};

  if (ARC_COMMON_STAT_FUNC(path, &info) != 0) {
    return -1; // Path does not exist or is inaccessible
  }

  if (S_ISDIR(info.st_mode)) {
    return 1; // It's a folder/directory
  } else if (S_ISREG(info.st_mode)) {
    return 0; // It's a regular file
  }

  return -1; // Other type (e.g., pipe, socket, device, etc.)
}

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

typedef unsigned char ArcBool8;

#include "arc_state/arc_state_stage1.inl"
#include "arc_state/arc_state_rawbuild.inl"

typedef struct ArcState {
  ArcStateStage1 stage1;
} ArcState;

void arcStage1(ArcState * state, int ArgsCount, wchar_t * const * const Args);
void arcRawbuild(ArcState * state);
void arcStage2(ArcState * state);

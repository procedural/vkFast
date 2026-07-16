#pragma once

extern wchar_t const * g_tbbtros_compilerLicense;

#if !defined(TBBTROS_STAGE1_INTERNAL_COMPILER_TESTS) && !defined(TBBTROS_STAGE1_INTERNAL_COMPILER_TESTS_IGNORE)
#error
#endif
#if !defined(TBBTROS_STAGE1_INTERNAL_COMPILER_DEBUG) && !defined(TBBTROS_STAGE1_INTERNAL_COMPILER_DEBUG_IGNORE)
#error
#endif

#define internalTargetOsSwitch              1
#define internalTargetOsWindows             2

#ifndef internalTargetOs
  #ifdef _WIN32
    #define internalTargetOs                internalTargetOsWindows
  #endif
#endif

#ifndef internalTargetOs
  #error internalTargetOs was not defined.
#endif

#define internalCpp17                       201703L

#ifdef _MSVC_LANG
  #define internalCpp                       _MSVC_LANG
#else
  #define internalCpp                       __cplusplus
#endif

#if internalCpp >= internalCpp17
  #define internalStaticAssert              static_assert
#else
  #define internalStaticAssert              assert
#endif

#if internalTargetOs == internalTargetOsWindows
#include <Windows.h> // For SetConsoleOutputCP
#endif
#include <assert.h>  // For assert
#include <stdint.h>  // For uint64_t
#include <stdio.h>   // For wprintf
#include <sstream>   // For std::wostringstream
#include <fstream>   // For std::wifstream
#include <string>    // For std::wstring
#include <vector>    // For std::vector

#define internalCommandLineInterfaceWprintf wprintf
#define internalFatalErrorWprintf           wprintf
#define internalInfoWprintf                 wprintf
#define internalDebugWprintf                wprintf

typedef unsigned char TbbtrosBool8;

#include "tbbtros_compiler_common_stage1.h"

typedef struct TbbtrosCompilerState {
  TbbtrosStage1 stage1;
} TbbtrosCompilerState;

void internalCompilerStage1(TbbtrosCompilerState * state, int ArgsCount, wchar_t const * const * Args);
void internalCompilerStage2(TbbtrosCompilerState * state);

#pragma once

#include "arc.h"

#include "arc_cross_stage/HashFNV1a64.h"

// NOTE(Constantine)(Jul 25, 2026):
// All functions are 'static' here in this file,
// because they can be used across all stages multiple times.

static void arc_xs_InitialChecks() {
  arc_static_assert(
    "Fatal internal compiler error: invalid expected output from the internal FNV1a64 hash function." &&
    arc_xs_HashFNV1a64(sizeof("foobar")-1, "foobar") == 0x85944171f73967e8
  );
}

static std::wstring arc_xs_FileRead(std::wstring filepath) {
  std::filesystem::path std_filepath(filepath);
  std::wifstream wif(std_filepath);
  wif.imbue(std::locale("en_US.UTF-8"));
  std::wostringstream ss;
  ss << wif.rdbuf();
  std::wstring s = ss.str();
  return s;
}

static void arc_xs_FileWrite(std::wstring filepath, std::wstring writeString) {
  std::filesystem::path std_filepath(filepath);
  std::wofstream fs(std_filepath, std::wofstream::out);
  fs.imbue(std::locale("en_US.UTF-8"));
  fs << writeString;
  fs.close();
}

static void arc_xs_CompilerCommandIncludeSourceCodeFile(ArcStateStage1 & stage1, std::wstring filepath) {
  std::wstring fileSourceCode = arc_xs_FileRead(filepath);

  if (fileSourceCode.size() > 0) {
    // Do nothing then.
  } else {
    // Skip empty file.
    return;
  }

  {
    wchar_t lastCharacter = fileSourceCode[fileSourceCode.size() - 1];
    if (lastCharacter != L'\n') {
      fileSourceCode += L"\n";
    }
  }

  stage1.sourceCodeWithoutCommentsString += fileSourceCode;
  stage1.filesPath.push_back(filepath);
  stage1.filesSize.push_back(fileSourceCode.size());
  stage1.filesOriginalSourceCodeString.push_back(fileSourceCode);
}

static void arc_xs_CompilerCommandIncludeSourceCodeFileOrFolder(ArcStateStage1 & stage1, std::wstring filepath) {
  std::wstring fileSourceCode = arc_xs_FileRead(filepath);

  if (fileSourceCode.size() > 0) {
    wchar_t lastCharacter = fileSourceCode[fileSourceCode.size() - 1];
    if (lastCharacter != L'\n') {
      fileSourceCode += L"\n";
    }
  }

  stage1.sourceCodeWithoutCommentsString += fileSourceCode;
  stage1.filesPath.push_back(filepath);
  stage1.filesSize.push_back(fileSourceCode.size());
  stage1.filesOriginalSourceCodeString.push_back(fileSourceCode);
}

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

static const char * const arc_xs_MallocWcharToChar(const wchar_t * const wstr) {
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
static int arc_xs_GetSystemPathType(const char * const path) {
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

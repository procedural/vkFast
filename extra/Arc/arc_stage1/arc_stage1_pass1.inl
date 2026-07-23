// NOTE(Constantine): Misc compiler helper functions begin

#if ARC_COMPILER_OS == ARC_COMPILER_OS_WINDOWS
static void arc_s1p1_DebugDataBreakpoint(std::wstring breakpointName) {
  std::wstring text;
  text += L"Breakpoint \"";
  text += breakpointName;
  text += L"\" reached. Press OK to continue.";
  MessageBoxW(NULL, text.c_str(), L"Data breakpoint", MB_OK);
  DebugBreak();
  volatile int _ = 0;
}
#else
static void arc_s1p1_DebugDataBreakpoint(std::wstring breakpointName) {
  const volatile wchar_t * const volatile name = (const volatile wchar_t * const volatile)breakpointName.c_str();
  volatile int trap = 1;
  while (trap) {
    volatile int _ = 0;
  }
}
#endif

#if ARC_INTERNAL_CPP_VERSION >= ARC_INTERNAL_CPP_VERSION_17
constexpr
#endif
static uint64_t arc_s1p1_HashFNV1a64(uint64_t bytesCount, const char * const bytes) {
  uint64_t hash = 0xcbf29ce484222325;
  for (uint64_t i = 0; i < bytesCount; i += 1) {
    hash = (hash ^ (uint64_t)((uint8_t)(bytes[i]))) * 0x100000001b3;
  }
  return hash;
}

static std::wstring arc_s1p1_FileRead(std::wstring filepath) {
#if ARC_COMPILER_OS == ARC_COMPILER_OS_WINDOWS
  std::wifstream wif(filepath);
#else
  std::filesystem::path std_filepath(filepath);
  std::wifstream wif(std_filepath);
#endif
  wif.imbue(std::locale("en_US.UTF-8"));
  std::wostringstream ss;
  ss << wif.rdbuf();
  std::wstring s = ss.str();
  return s;
}

static void arc_s1p1_FileWrite(std::wstring filepath, std::wstring writeString) {
#if ARC_COMPILER_OS == ARC_COMPILER_OS_WINDOWS
  std::wofstream fs(filepath, std::wofstream::out);
#else
  std::filesystem::path std_filepath(filepath);
  std::wofstream fs(std_filepath, std::wofstream::out);
  fs.imbue(std::locale("en_US.UTF-8"));
#endif
  fs << writeString;
  fs.close();
}

static void arc_s1p1_CompilerCommandIncludeSourceCodeFile(ArcStateStage1 & stage1, std::wstring filepath) {
  std::wstring fileSourceCode = arc_s1p1_FileRead(filepath);

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

static void arc_s1p1_CompilerCommandIncludeSourceCodeFileOrFolder(ArcStateStage1 & stage1, std::wstring filepath) {
  std::wstring fileSourceCode = arc_s1p1_FileRead(filepath);

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

static void arc_s1p1_InfoPrintAdditionalInfo(const char * const functionName, const ArcStateStage1 & stage1, const uint64_t * const optionalCursorPosition = NULL) {
  arc_wprintf_info(L"Compiler arguments:");
  for (uint64_t i = 0, count = stage1.wmainArguments.arguments.size(); i < count; i += 1) {
    arc_wprintf_info(L" %ls", stage1.wmainArguments.arguments[i].c_str());

    if (optionalCursorPosition != NULL && stage1.wmainArgumentsParameters.debugPrintSourceCodeUpToAndIncludingCursorPositionIsRequested == 0) {
      if (i == (count - 1)) {
        arc_wprintf_info(L" --debug-print-to-cursor-position %zu", optionalCursorPosition[0]);
      }
    }
  }
  arc_wprintf_info(L"\n");
  arc_wprintf_info(L"Function: %s" L"\n", functionName);
}

static void arc_s1p1_FatalErrorAdditionalCalls() {
#ifdef ARC_INTERNAL_COMPILER_DEBUG
  arc_s1p1_DebugDataBreakpoint(L"Fatal error");
#endif
}

static void arc_s1p1_ProcessWmainArgumentsFatalError(std::wstring error, std::wstring parameterName, const ArcStateStage1 & stage1) {
  arc_s1p1_InfoPrintAdditionalInfo(__FUNCTION__, stage1, NULL);
  arc_wprintf_fatalError(L"\n");
  arc_wprintf_fatalError(error.c_str(), parameterName.c_str());
  arc_wprintf_fatalError(L"\n");
  arc_s1p1_FatalErrorAdditionalCalls();
  exit(1);
}

static const wchar_t * const arc_s1p1_CompilerCommandGetCompilerLicense() {
  return g_arc_compilerLicense;
}

static void arc_s1p1_CompilerCommandDefineMacro(ArcStateStage1 & stage1, std::wstring macroName, int macroValue, ArcBool8 isDefinedByCli) {
  for (size_t i = 0, count = stage1.compilerCommandDefinedMacros.macrosName.size(); i < count; i += 1) {
    std::wstring alreadyDefinedMacroName = stage1.compilerCommandDefinedMacros.macrosName[i];
    
    if (alreadyDefinedMacroName == macroName) {
      arc_s1p1_InfoPrintAdditionalInfo(__FUNCTION__, stage1, NULL);
      arc_wprintf_fatalError(L"\n");
      arc_wprintf_fatalError(L"Fatal compiler command error: attempting to define (%ls) macro name \"%ls\" again when it was already defined previously." L"\n",
        isDefinedByCli == 1 ? L"by command line interface" : L"by a custom compiler command call",
        macroName.c_str()
      );
      arc_wprintf_fatalError(L"\n");
      arc_wprintf_fatalError(L"List of all macro names and their values defined to this point:" L"\n");
      for (size_t i = 0, count = stage1.compilerCommandDefinedMacros.macrosName.size(); i < count; i += 1) {
        arc_wprintf_fatalError(
          L" %lc %ls %d",
          macroName == stage1.compilerCommandDefinedMacros.macrosName[i] ? L'>' : L' ',
          stage1.compilerCommandDefinedMacros.macrosName[i].c_str(),
          stage1.compilerCommandDefinedMacros.macrosValue[i]
        );
        if (stage1.compilerCommandDefinedMacros.macrosIsDefinedByCli[i] == 1) {
          arc_wprintf_fatalError(L" (defined by command line interface)");
        } else {
          arc_wprintf_fatalError(L" (defined by a custom compiler command call)");
        }
        arc_wprintf_fatalError(L"\n");
      }
      arc_wprintf_fatalError(L"\n");
      arc_s1p1_FatalErrorAdditionalCalls();
      exit(1);
    }
  }
  stage1.compilerCommandDefinedMacros.macrosName.push_back(macroName);
  stage1.compilerCommandDefinedMacros.macrosValue.push_back(macroValue);
  stage1.compilerCommandDefinedMacros.macrosIsDefinedByCli.push_back(isDefinedByCli);
}

static void arc_s1p1_ProcessWmainArguments(ArcStateStage1 & stage1) {
  const wchar_t * const availableParameters[] = {
    /*[0]*/ L"--help",
    /*[1]*/ L"--license",
    /*[2]*/ L"--debug-print-to-cursor-position",
    /*[3]*/ L"--define",
  };
  int availableParametersArgumentsCount[] = {
    /*[0]*/ 0,
    /*[1]*/ 0,
    /*[2]*/ 1,
    /*[3]*/ 2,
  };

  for (size_t i = 0, argumentsCount = stage1.wmainArguments.arguments.size(); i < argumentsCount; i += 1) {
    std::wstring * arguments = stage1.wmainArguments.arguments.data();
    std::wstring   argument  = arguments[i];
    if (i == 0 && argumentsCount == 1) {
      goto help;
    }
    if (i == 0) {
      continue;
    }
    if (argument == availableParameters[0]) {
      help:
      const int             parameterIndex     = 0;
      const wchar_t * const parameter          = availableParameters[parameterIndex];
      int                   parameterArgsCount = availableParametersArgumentsCount[parameterIndex];

      arc_wprintf_cli(L"%ls:" L"\n", L"\"Arc\" compiler");
      arc_wprintf_cli(L"\n");
      arc_wprintf_cli(L"Usage: %ls [ parameterName [ parameterValue ... ] ... ] sourceCodeFilepath ..." L"\n", arguments[0].c_str());
      arc_wprintf_cli(L"\n");
      arc_wprintf_cli(L"Parameters:" L"\n");
      for (int parameterIndex = 0, parametersCount = sizeof(availableParameters) / sizeof(availableParameters[0]); parameterIndex < parametersCount; parameterIndex += 1) {
        arc_wprintf_cli(L"  %ls", availableParameters[parameterIndex]);
        for (int parameterArgumentIndex = 0; parameterArgumentIndex < availableParametersArgumentsCount[parameterIndex]; parameterArgumentIndex += 1) {

          if (parameterIndex == 2) {
            if (parameterArgumentIndex == 0) { arc_wprintf_cli(L" <position value>"); }
            continue;
          }

          if (parameterIndex == 3) {
            if (parameterArgumentIndex == 0) { arc_wprintf_cli(L" <macro name>"); }
            if (parameterArgumentIndex == 1) { arc_wprintf_cli(L" <macro value 0 for undefined or 1 for defined>"); }
            continue;
          }

          arc_wprintf_cli(L" <value>");
        }
        arc_wprintf_cli(L"\n");
      }
      exit(0);
      continue;
    }
    if (argument == availableParameters[1]) {
      const int             parameterIndex     = 1;
      const wchar_t * const parameter          = availableParameters[parameterIndex];
      int                   parameterArgsCount = availableParametersArgumentsCount[parameterIndex];

      arc_wprintf_cli(L"%ls" L"\n", arc_s1p1_CompilerCommandGetCompilerLicense());
      exit(0);
      continue;
    }
    if (argument == availableParameters[2]) {
      const int             parameterIndex     = 2;
      const wchar_t * const parameter          = availableParameters[parameterIndex];
      int                   parameterArgsCount = availableParametersArgumentsCount[parameterIndex];

      if (i + parameterArgsCount >= argumentsCount) { arc_s1p1_ProcessWmainArgumentsFatalError(L"Fatal command line interface error: not enough arguments for parameter \"%ls\"." "\n", parameter, stage1); }

      std::wstring argument0 = std::wstring(arguments[i + 1]);

      int position = 0;
      try { position = std::stoi(argument0); } catch(...) { arc_s1p1_ProcessWmainArgumentsFatalError(L"Fatal command line interface error: argument[0] for parameter \"%ls\" is an invalid number." "\n", parameter, stage1); }

      stage1.wmainArgumentsParameters.debugPrintSourceCodeUpToAndIncludingCursorPositionIsRequested = 1;
      stage1.wmainArgumentsParameters.debugPrintSourceCodeUpToAndIncludingCursorPosition            = position;

      continue;
    }
    if (argument == availableParameters[3]) {
      const int             parameterIndex     = 3;
      const wchar_t * const parameter          = availableParameters[parameterIndex];
      int                   parameterArgsCount = availableParametersArgumentsCount[parameterIndex];

      if (i + parameterArgsCount >= argumentsCount) { arc_s1p1_ProcessWmainArgumentsFatalError(L"Fatal command line interface error: not enough arguments for parameter \"%ls\"." "\n", parameter, stage1); }

      std::wstring argument0 = std::wstring(arguments[i + 1]);
      std::wstring argument1 = std::wstring(arguments[i + 2]);

      std::wstring macroName  = argument0;
      int          macroValue = 0;
      try { macroValue = std::stoi(argument1); } catch(...) { arc_s1p1_ProcessWmainArgumentsFatalError(L"Fatal command line interface error: argument[1] for parameter \"%ls\" is an invalid number." "\n", parameter, stage1); }

      if (macroValue == 0 || macroValue == 1) {
        // Check is passed.
      } else {
        arc_s1p1_ProcessWmainArgumentsFatalError(L"Fatal command line interface error: parameter \"%ls\" expects macro value to be set to either 0 for undefined or 1 for defined." "\n", parameter, stage1);
      }

      arc_s1p1_CompilerCommandDefineMacro(stage1, macroName, macroValue, 1);

      continue;
    }

    arc_s1p1_CompilerCommandIncludeSourceCodeFile(stage1, argument);
  }
}

// NOTE(Constantine): Misc compiler helper functions end

// NOTE(Constantine): General compiler functions begin

static wchar_t arc_s1p1_PeekCharacter(const ArcStateStage1 & stage1, uint64_t currentCursorPosition, uint64_t peekOffset) {
  size_t sourceCodeLength = stage1.sourceCodeWithoutCommentsString.size();
  if ((currentCursorPosition + peekOffset) >= sourceCodeLength) {
    return 0;
  }
  wchar_t character = stage1.sourceCodeWithoutCommentsString[currentCursorPosition + peekOffset];
  return character;
}

static void arc_s1p1_SetCharacter(ArcStateStage1 & stage1, uint64_t currentCursorPosition, uint64_t setOffset, wchar_t setCharacter) {
  size_t sourceCodeLength = stage1.sourceCodeWithoutCommentsString.size();
  if ((currentCursorPosition + setOffset) >= sourceCodeLength) {
    return;
  }
  stage1.sourceCodeWithoutCommentsString[currentCursorPosition + setOffset] = setCharacter;
}

static uint64_t arc_s1p1_GetFileCursorStartBasedOnCursorPosition(const ArcStateStage1 & stage1, uint64_t cursorPosition) {
  uint64_t filesizeOffset     = 0;
  uint64_t filesizeOffsetLast = 0;
  for (uint64_t i = 0, count = stage1.filesSize.size(); i < count; i += 1) {
    filesizeOffset += stage1.filesSize[i];
    if (filesizeOffset > cursorPosition) {
      return filesizeOffsetLast;
    }
    filesizeOffsetLast = filesizeOffset;
  }
  arc_s1p1_InfoPrintAdditionalInfo(__FUNCTION__, stage1, &cursorPosition);
  arc_wprintf_fatalError(L"\n");
  arc_wprintf_fatalError(L"Fatal internal compiler error: couldn't find the correct file cursor start based on cursor position value %zu." L"\n", cursorPosition);
  arc_wprintf_fatalError(L"\n");
  arc_s1p1_FatalErrorAdditionalCalls();
  exit(1);
  return -1; // NOTE(Constantine): Unreachable, intentionally.
}

static uint64_t arc_s1p1_GetFileIndexBasedOnCursorPosition(const ArcStateStage1 & stage1, uint64_t cursorPosition) {
  uint64_t filesizeOffset = 0;
  for (uint64_t i = 0, count = stage1.filesSize.size(); i < count; i += 1) {
    filesizeOffset += stage1.filesSize[i];
    if (filesizeOffset > cursorPosition) {
      return i;
    }
  }
  arc_s1p1_InfoPrintAdditionalInfo(__FUNCTION__, stage1, &cursorPosition);
  arc_wprintf_fatalError(L"\n");
  arc_wprintf_fatalError(L"Fatal internal compiler error: couldn't find the correct file index based on cursor position value %zu." L"\n", cursorPosition);
  arc_wprintf_fatalError(L"\n");
  arc_s1p1_FatalErrorAdditionalCalls();
  exit(1);
  return -1; // NOTE(Constantine): Unreachable, intentionally.
}

static std::wstring arc_s1p1_GetFilePathBasedOnCursorPosition(const ArcStateStage1 & stage1, uint64_t cursorPosition) {
  uint64_t fileindex = arc_s1p1_GetFileIndexBasedOnCursorPosition(stage1, cursorPosition);
  return stage1.filesPath[fileindex];
}

static uint64_t arc_s1p1_GetFileLineBasedOnCursorPosition(const ArcStateStage1 & stage1, uint64_t cursorPosition) {
  uint64_t lineIndex   = 0;
  uint64_t cursorStart = arc_s1p1_GetFileCursorStartBasedOnCursorPosition(stage1, cursorPosition);
  for (uint64_t i = cursorStart, count = cursorPosition; i < count; i += 1) {
    wchar_t character1 = arc_s1p1_PeekCharacter(stage1, i, 0);

    if (character1 == L'\n') {
      lineIndex += 1;
    }
  }
  return lineIndex + 1;
}

static uint64_t arc_s1p1_GetLineColumnFromCursorPosition(const ArcStateStage1 & stage1, uint64_t cursorPosition) {
  uint64_t columnIndex = 0;
  for (ptrdiff_t i = (ptrdiff_t)cursorPosition; i >= 0; i -= 1) { // NOTE(Constantine): Casting unsigned uint64_t to signed ptrdiff_t.
    wchar_t character1 = arc_s1p1_PeekCharacter(stage1, i, 0);

    if (character1 == L'\n') {
      if (i != cursorPosition) { // NOTE(Constantine): Handles the case when the cursor is at '\n' character.
        break;
      }
    }

    columnIndex += 1;
  }
  return columnIndex; // NOTE(Constantine): No need to + 1, intentionally.
}

static void arc_s1p1_InfoPrintFile(const ArcStateStage1 & stage1, uint64_t cursorPosition) {
  std::wstring filepath   = arc_s1p1_GetFilePathBasedOnCursorPosition(stage1, cursorPosition);
  uint64_t     fileline   = arc_s1p1_GetFileLineBasedOnCursorPosition(stage1, cursorPosition);
  uint64_t     filecolumn = arc_s1p1_GetLineColumnFromCursorPosition(stage1, cursorPosition);
  arc_wprintf_info(L" --> %ls:%zu:%zu (cursor position: %zu)\n", filepath.c_str(), fileline, filecolumn, cursorPosition);
}

static void arc_s1p1_InfoPrintLine(const ArcStateStage1 & stage1, uint64_t fileIndex, ptrdiff_t lineNumber, ArcBool8 markTheLine) {
  if (lineNumber <= 0) {
    arc_wprintf_info(L"           | \n"); // NOTE(Constantine): Using 7 spaces to print line numbers + 3 spaces for a mark.
    return;
  }

  ArcBool8 lineIsPrinted = 0;
  uint64_t     lineIndex     = 0;
  for (uint64_t i = 0, count = stage1.filesOriginalSourceCodeString[fileIndex].size(); i < count; i += 1) {
    wchar_t character1 = stage1.filesOriginalSourceCodeString[fileIndex][i];
    
    if (lineIndex == (lineNumber - 1)) {
      if (lineIsPrinted == 0) {
        arc_wprintf_info(L" %lc %7zu | ", markTheLine == 1 ? L'>' : L' ', lineNumber); // NOTE(Constantine): Using 7 spaces to print line numbers + 3 spaces for a mark.
      }
      lineIsPrinted = 1;
      arc_wprintf_info(L"%lc", character1);
    }

    if (character1 == L'\n') {
      if (lineIsPrinted == 1) {
        return;
      }
      lineIndex += 1;
      continue;
    }
  }

  if (lineIsPrinted == 1) {
    arc_wprintf_info(L"\n");
  }

  if (lineIsPrinted == 0) {
    arc_wprintf_info(L"           | \n"); // NOTE(Constantine): Using 7 spaces to print line numbers + 3 spaces for a mark.
  }
}

static void arc_s1p1_InfoPrintLinesAtCursorPosition(const ArcStateStage1 & stage1, uint64_t cursorPosition) {
  uint64_t fileindex  = arc_s1p1_GetFileIndexBasedOnCursorPosition(stage1, cursorPosition);
  uint64_t fileline   = arc_s1p1_GetFileLineBasedOnCursorPosition(stage1, cursorPosition);
  uint64_t filecolumn = arc_s1p1_GetLineColumnFromCursorPosition(stage1, cursorPosition);
  arc_s1p1_InfoPrintFile(stage1, cursorPosition);
  arc_s1p1_InfoPrintLine(stage1, fileindex, fileline - 4, 0);
  arc_s1p1_InfoPrintLine(stage1, fileindex, fileline - 3, 0);
  arc_s1p1_InfoPrintLine(stage1, fileindex, fileline - 2, 0);
  arc_s1p1_InfoPrintLine(stage1, fileindex, fileline - 1, 0);
  arc_s1p1_InfoPrintLine(stage1, fileindex, fileline + 0, 1);
  arc_wprintf_info(L"             ");
  for (uint64_t i = 0; i < (filecolumn - 1); i += 1) {
    arc_wprintf_info(L" ");
  }
  arc_wprintf_info(L"^" L"\n");
  arc_s1p1_InfoPrintLine(stage1, fileindex, fileline + 1, 0);
  arc_s1p1_InfoPrintLine(stage1, fileindex, fileline + 2, 0);
  arc_s1p1_InfoPrintLine(stage1, fileindex, fileline + 3, 0);
  arc_s1p1_InfoPrintLine(stage1, fileindex, fileline + 4, 0);
}

// NOTE(Constantine): General compiler functions end

static void arc_s1p1_ReplaceSingleLineCommentWithSpaces(ArcStateStage1 & stage1, uint64_t & i, uint64_t count) {
  // NOTE(Constantine): Commenting everything up to and including the new line character, with \ in mind.
  for (ArcBool8 continueParsingToNextLine = 0; i < count; i += 1) {
    wchar_t character1 = arc_s1p1_PeekCharacter(stage1, i, 0);
    
    if (character1 == L'\\') {
      continueParsingToNextLine = 1;
      arc_s1p1_SetCharacter(stage1, i, 0, L' ');
      continue;
    }
    if (character1 == L'\n' && continueParsingToNextLine == 1) {
      continueParsingToNextLine = 0;
      continue;
    }
    if (character1 == L'\n' && continueParsingToNextLine == 0) {
      break;
    }

    arc_s1p1_SetCharacter(stage1, i, 0, L' ');
  }
}

static void arc_s1p1_ReplaceMultiLineCommentWithSpaces(ArcStateStage1 & stage1, uint64_t & i, uint64_t count) {
  // NOTE(Constantine): Replacing the beginning /* characters with space characters for cases like '/*/'.
  arc_s1p1_SetCharacter(stage1, i, 0, L' ');
  arc_s1p1_SetCharacter(stage1, i, 1, L' ');

  // NOTE(Constantine): Replacing everything up to and including the */ characters with space characters.
  for (; i < count; i += 1) {
    wchar_t character1 = arc_s1p1_PeekCharacter(stage1, i, 0);
    wchar_t character2 = arc_s1p1_PeekCharacter(stage1, i, 1);
    
    if (character1 == L'\n') {
      continue;
    }

    if (character1 == L'*' && character2 == L'/') {
      arc_s1p1_SetCharacter(stage1, i, 0, L' ');
      arc_s1p1_SetCharacter(stage1, i, 1, L' ');
      break;
    }

    arc_s1p1_SetCharacter(stage1, i, 0, L' ');
  }
}

// NOTE(Constantine): External function from Pass 2.
static void arc_s1p2_TokenizerParseMulticharacterString(ArcStateStage1 & stage1, uint64_t & i, uint64_t count, ArcBool8 optionalAppendParsedTokens);

static void arc_s1p1_Stage1Pass1SourceCodeReplaceCommentsWithSpaceCharacters(ArcStateStage1 & stage1) {
  for (uint64_t i = 0, count = stage1.sourceCodeWithoutCommentsString.size(); i < count; i += 1) {
    wchar_t character1 = arc_s1p1_PeekCharacter(stage1, i, 0);
    wchar_t character2 = arc_s1p1_PeekCharacter(stage1, i, 1);

    if (character1 == L'"') {
      arc_s1p2_TokenizerParseMulticharacterString(stage1, i, count, 0);
      continue;
    }

    if (character1 == L'/' && character2 == L'/') {
      arc_s1p1_ReplaceSingleLineCommentWithSpaces(stage1, i, count);
      continue;
    }

    if (character1 == L'/' && character2 == L'*') {
      arc_s1p1_ReplaceMultiLineCommentWithSpaces(stage1, i, count);
      continue;
    }

    if (character1 == L'*' && character2 == L'/') {
      arc_s1p1_InfoPrintAdditionalInfo(__FUNCTION__, stage1, &i);
      arc_wprintf_fatalError(L"\n");
      arc_wprintf_fatalError(L"Fatal error: closing multi-line comment that was not opened." "\n");
      arc_wprintf_fatalError(L"\n");
      arc_s1p1_InfoPrintLinesAtCursorPosition(stage1, i);
      arc_s1p1_FatalErrorAdditionalCalls();
      exit(1);
    }
  }

#if 0 // NOTE(Constantine): Example code:

// https://discord.com/invite/dDynxDH29K
/* https://discord.com/channels/459770346245980161/1319515207915143181/1322285791149555782
*/

const char * const some_string_1 = "// You'd think this is a code comment, but actually this is a part of a string lol";
const char * const some_string_2 = "/* Same */";
const char * const some_string_3 = "I mean"; // this is "not a code string too, lol"

int something = 0; // It's time to be /* bamboozled, lol, not a multiline comment start, */ not a multiline comment end either
/* Same, // not a single line comment
but a multiline one */

/* Let me teach you * /
the importance of space */

/*/*/

/*/**/

/*//*/

///*

/*/
*/

/*
  /*
    Nested
  */
*/
// こんにちは、世界！

// oh \
   right \
   funky "single line" comments :D

#endif
}

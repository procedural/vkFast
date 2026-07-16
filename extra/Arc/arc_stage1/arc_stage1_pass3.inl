static ArcToken arc_s1p3_PeekToken(const ArcStateStage1 & stage1, uint64_t currentTokenIndex, uint64_t peekOffset) {
  size_t tokensCount = stage1.tokenizer.tokens.size();
  if ((currentTokenIndex + peekOffset) >= tokensCount) {
    return {};
  }
  ArcToken token = stage1.tokenizer.tokens[currentTokenIndex + peekOffset];
  return token;
}

static ArcBool8 arc_s1p3_TokenGetEitherBoolean0Or1OtherwiseFatalError(const ArcStateStage1 & stage1, ArcToken token, std::wstring tokenString) {
  // NOTE(Constantine):
  // This is an oversimplification, of course, since the numbers may be written
  // in different formats, for example 0x0 or 0x1, 0b0 or 0b1, 00 or 01, etc.
  // In future when the parser gets more mature this can be parsed more correctly.
  if (tokenString == L"0") {
    return 0;
  }
  if (tokenString == L"1") {
    return 1;
  }
  arc_s1p1_InfoPrintAdditionalInfo(__FUNCTION__, stage1, &token.stringOffset);
  arc_wprintf_fatalError(L"\n");
  arc_wprintf_fatalError(L"Fatal error: encountered a token that is expected to be of value 0 or 1, but is neither 0 nor 1." L"\n");
  arc_wprintf_fatalError(L"\n");
  arc_s1p1_InfoPrintLinesAtCursorPosition(stage1, token.stringOffset);
  arc_s1p1_FatalErrorAdditionalCalls();
  exit(1);
  return -1; // NOTE(Constantine): Unreachable, intentionally.
}

static void arc_s1p3_TokenCheckIfCompilerCommandDefinedMacroExistOtherwiseFatalError(const ArcStateStage1 & stage1, ArcToken token, std::wstring tokenString) {
  for (size_t compilerCommandDefinedMacroIndex = 0, compilerCommandDefinedMacrosCount = stage1.compilerCommandDefinedMacros.macrosName.size(); compilerCommandDefinedMacroIndex < compilerCommandDefinedMacrosCount; compilerCommandDefinedMacroIndex += 1) {
    std::wstring definedMacroName = stage1.compilerCommandDefinedMacros.macrosName[compilerCommandDefinedMacroIndex];
    if (tokenString == definedMacroName) {
      return;
    }
  }
  arc_s1p1_InfoPrintAdditionalInfo(__FUNCTION__, stage1, &token.stringOffset);
  arc_wprintf_fatalError(L"\n");
  arc_wprintf_fatalError(L"Fatal error: encountered a token that is expected to be a defined or undefined macro, but was not found to be set neither by the command line interface nor by a custom compiler command call." L"\n");
  arc_wprintf_fatalError(L"\n");
  arc_wprintf_fatalError(L"Suggested fix: pass '--define %ls [0 or 1]' (without '' quotes and [] brackets, choosing the value) command line interface parameter or make a custom compiler command call to set the macro name and value." L"\n", tokenString.c_str());
  arc_wprintf_fatalError(L"\n");
  arc_s1p1_InfoPrintLinesAtCursorPosition(stage1, token.stringOffset);
  arc_s1p1_FatalErrorAdditionalCalls();
  exit(1);
}

template<typename T>
static size_t arc_s1p3_GetLastIndexElseError(const std::vector<T> & v, const ArcStateStage1 & stage1) {
  if (v.size() == 0) {
    arc_s1p1_InfoPrintAdditionalInfo(__FUNCTION__, stage1, NULL);
    arc_wprintf_fatalError(L"\n");
    arc_wprintf_fatalError(L"Fatal internal compiler error: encountered an attempt to get the last index of an empty vector." L"\n");
    arc_wprintf_fatalError(L"\n");
    arc_s1p1_FatalErrorAdditionalCalls();
    exit(1);
  }
  return v.size() - 1;
}

static void arc_s1p3_Stage1Pass3ConditionalMacroTokenSyntaxChecks(ArcStateStage1 & stage1) {
  std::vector<ArcBool8> openedConditionalMacroMustNowFollowWithEitherElseCasesOrEndifStack;
  std::vector<ArcBool8> openedConditionalMacroAlreadyHasAnElseCaseStack;
  
  for (uint64_t tokenIndex = 0, tokensCount = stage1.tokenizer.tokens.size(); tokenIndex < tokensCount;) {
    ArcToken token1 = arc_s1p3_PeekToken(stage1, tokenIndex, 0);
    ArcToken token2 = arc_s1p3_PeekToken(stage1, tokenIndex, 1);
    ArcToken token3 = arc_s1p3_PeekToken(stage1, tokenIndex, 2);

    std::wstring token1String = arc_s1p2_TokenToStringCanBeEmpty(stage1, token1);
    std::wstring token2String = arc_s1p2_TokenToStringCanBeEmpty(stage1, token2);
    std::wstring token3String = arc_s1p2_TokenToStringCanBeEmpty(stage1, token3);

    if (
      (
        token1String == L"#"  &&
        token2String == L"if"
      )
    )
    {
      openedConditionalMacroMustNowFollowWithEitherElseCasesOrEndifStack.push_back(1); // NOTE(Constantine): The pushed value 1 doesn't matter.
      openedConditionalMacroAlreadyHasAnElseCaseStack.push_back(0);
      arc_s1p3_TokenGetEitherBoolean0Or1OtherwiseFatalError(stage1, token3, token3String);
    }

    if (
      (
        token1String == L"#"      &&
        token2String == L"ifdef"
      )
      ||
      (
        token1String == L"#"      &&
        token2String == L"ifndef"
      )
    )
    {
      openedConditionalMacroMustNowFollowWithEitherElseCasesOrEndifStack.push_back(1);
      openedConditionalMacroAlreadyHasAnElseCaseStack.push_back(0);
      arc_s1p3_TokenCheckIfCompilerCommandDefinedMacroExistOtherwiseFatalError(stage1, token3, token3String);
    }

    if (
      (
        token1String == L"#"  &&
        token2String == L"else"
      )
    )
    {
      if (openedConditionalMacroAlreadyHasAnElseCaseStack.size() == 0) {
        arc_s1p1_InfoPrintAdditionalInfo(__FUNCTION__, stage1, &token1.stringOffset);
        arc_wprintf_fatalError(L"\n");
        arc_wprintf_fatalError(L"Fatal error: a conditional macro was not opened before encountering this conditional macro." L"\n");
        arc_wprintf_fatalError(L"\n");
        arc_s1p1_InfoPrintLinesAtCursorPosition(stage1, token1.stringOffset);
        arc_s1p1_FatalErrorAdditionalCalls();
        exit(1);
      }
      if (openedConditionalMacroAlreadyHasAnElseCaseStack[arc_s1p3_GetLastIndexElseError(openedConditionalMacroAlreadyHasAnElseCaseStack, stage1)] > 0) {
        arc_s1p1_InfoPrintAdditionalInfo(__FUNCTION__, stage1, &token1.stringOffset);
        arc_wprintf_fatalError(L"\n");
        arc_wprintf_fatalError(L"Fatal error: expected #endif, #else was already encountered previously." L"\n");
        arc_wprintf_fatalError(L"\n");
        arc_s1p1_InfoPrintLinesAtCursorPosition(stage1, token1.stringOffset);
        arc_s1p1_FatalErrorAdditionalCalls();
        exit(1);
      }
      openedConditionalMacroAlreadyHasAnElseCaseStack[arc_s1p3_GetLastIndexElseError(openedConditionalMacroAlreadyHasAnElseCaseStack, stage1)] += 1;
    }

    if (
      (
        token1String == L"#"    &&
        token2String == L"elif"
      )
    )
    {
      if (openedConditionalMacroMustNowFollowWithEitherElseCasesOrEndifStack.size() == 0) {
        arc_s1p1_InfoPrintAdditionalInfo(__FUNCTION__, stage1, &token1.stringOffset);
        arc_wprintf_fatalError(L"\n");
        arc_wprintf_fatalError(L"Fatal error: a conditional macro was not opened before encountering this conditional macro." L"\n");
        arc_wprintf_fatalError(L"\n");
        arc_s1p1_InfoPrintLinesAtCursorPosition(stage1, token1.stringOffset);
        arc_s1p1_FatalErrorAdditionalCalls();
        exit(1);
      }
      arc_s1p3_TokenGetEitherBoolean0Or1OtherwiseFatalError(stage1, token3, token3String);
    }

    if (
      (
        token1String == L"#"        &&
        token2String == L"elifdef"
      )
      ||
      (
        token1String == L"#"        &&
        token2String == L"elifndef"
      )
    )
    {
      if (openedConditionalMacroMustNowFollowWithEitherElseCasesOrEndifStack.size() == 0) {
        arc_s1p1_InfoPrintAdditionalInfo(__FUNCTION__, stage1, &token1.stringOffset);
        arc_wprintf_fatalError(L"\n");
        arc_wprintf_fatalError(L"Fatal error: a conditional macro was not opened before encountering this conditional macro." L"\n");
        arc_wprintf_fatalError(L"\n");
        arc_s1p1_InfoPrintLinesAtCursorPosition(stage1, token1.stringOffset);
        arc_s1p1_FatalErrorAdditionalCalls();
        exit(1);
      }
      arc_s1p3_TokenCheckIfCompilerCommandDefinedMacroExistOtherwiseFatalError(stage1, token3, token3String);
    }

    if (
      (
        token1String == L"#"     &&
        token2String == L"endif"
      )
    )
    {
      if (openedConditionalMacroMustNowFollowWithEitherElseCasesOrEndifStack.size() == 0) {
        arc_s1p1_InfoPrintAdditionalInfo(__FUNCTION__, stage1, &token1.stringOffset);
        arc_wprintf_fatalError(L"\n");
        arc_wprintf_fatalError(L"Fatal error: a conditional macro was not opened before encountering this conditional macro." L"\n");
        arc_wprintf_fatalError(L"\n");
        arc_s1p1_InfoPrintLinesAtCursorPosition(stage1, token1.stringOffset);
        arc_s1p1_FatalErrorAdditionalCalls();
        exit(1);
      }
      openedConditionalMacroMustNowFollowWithEitherElseCasesOrEndifStack.pop_back();
      openedConditionalMacroAlreadyHasAnElseCaseStack.pop_back();
    }

    tokenIndex += 1; // Next token.
  }

  if (openedConditionalMacroMustNowFollowWithEitherElseCasesOrEndifStack.size() > 0) {
    size_t lastCursorPosition = stage1.sourceCodeWithoutCommentsString.size() - 1;
    arc_s1p1_InfoPrintAdditionalInfo(__FUNCTION__, stage1, &lastCursorPosition);
    arc_wprintf_fatalError(L"\n");
    arc_wprintf_fatalError(L"Fatal error: a conditional macro was not closed before encountering the end of source code." L"\n");
    arc_wprintf_fatalError(L"\n");
    arc_s1p1_InfoPrintLinesAtCursorPosition(stage1, lastCursorPosition);
    arc_s1p1_FatalErrorAdditionalCalls();
    exit(1);
  }
}

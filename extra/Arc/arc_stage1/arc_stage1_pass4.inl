static ArcBool8 arc_s1p4_TokenCanConditionalMacroPathBeTakenBasedOnIdentifier(ArcToken tokenConditionalMacroStartToken, std::wstring tokenConditionalMacroNameString, ArcToken tokenConditionalMacroIdentifierToken, std::wstring tokenConditionalMacroIdentifierString, const ArcStateStage1 & stage1) {
  if (
    tokenConditionalMacroNameString == L"if"   ||
    tokenConditionalMacroNameString == L"elif"
  )
  {
    ArcBool8 value0or1 = arc_s1p3_TokenGetEitherBoolean0Or1OtherwiseFatalError(stage1, tokenConditionalMacroIdentifierToken, tokenConditionalMacroIdentifierString);
    return value0or1;
  }
  if (
    tokenConditionalMacroNameString == L"ifdef"    ||
    tokenConditionalMacroNameString == L"ifndef"   ||
    tokenConditionalMacroNameString == L"elifdef"  ||
    tokenConditionalMacroNameString == L"elifndef"
  )
  {
    ArcBool8 value0or1 = 0;

    for (size_t compilerCommandDefinedMacroIndex = 0, compilerCommandDefinedMacrosCount = stage1.compilerCommandDefinedMacros.macrosName.size(); compilerCommandDefinedMacroIndex < compilerCommandDefinedMacrosCount; compilerCommandDefinedMacroIndex += 1) {
      std::wstring definedMacroName = stage1.compilerCommandDefinedMacros.macrosName[compilerCommandDefinedMacroIndex];
      if (tokenConditionalMacroIdentifierString == definedMacroName) {
        value0or1 = (ArcBool8)stage1.compilerCommandDefinedMacros.macrosValue[compilerCommandDefinedMacroIndex];
        break;
      }
    }

    if (
      tokenConditionalMacroNameString == L"ifndef"   ||
      tokenConditionalMacroNameString == L"elifndef"
    )
    {
      return !value0or1;
    } else {
      return value0or1;
    }
  }
  arc_s1p1_InfoPrintAdditionalInfo(__FUNCTION__, stage1, &tokenConditionalMacroStartToken.stringOffset);
  arc_wprintf_fatalError(L"\n");
  arc_wprintf_fatalError(L"Fatal internal compiler error: unexpected conditional macro." L"\n");
  arc_wprintf_fatalError(L"\n");
  arc_s1p1_InfoPrintLinesAtCursorPosition(stage1, tokenConditionalMacroStartToken.stringOffset);
  arc_s1p1_FatalErrorAdditionalCalls();
  exit(1);
  return -1; // NOTE(Constantine): Unreachable, intentionally.
}

static void arc_s1p4_RemoveToken(ArcStateStage1 & stage1, uint64_t removeTokenIndex, uint64_t & recalculateTokensCount) {
  size_t tokensCount = stage1.tokenizer.tokens.size();
  if (removeTokenIndex >= tokensCount) {
    arc_s1p1_InfoPrintAdditionalInfo(__FUNCTION__, stage1, NULL);
    arc_wprintf_fatalError(L"\n");
    arc_wprintf_fatalError(L"Fatal internal compiler error: attempting to access out of bounds token from tokens array." L"\n");
    arc_wprintf_fatalError(L"\n");
    arc_s1p1_FatalErrorAdditionalCalls();
    exit(1);
  }
  stage1.tokenizer.tokens.erase(stage1.tokenizer.tokens.begin() + removeTokenIndex);
  recalculateTokensCount = stage1.tokenizer.tokens.size();
}

static void arc_s1p4_Stage1Pass4ConditionalMacroTokenFalseCodePathsElimination(ArcStateStage1 & stage1) {
  std::vector<ArcBool8> parentExecutionMaskStack;
  std::vector<ArcBool8> executionMaskStack;
  std::vector<ArcBool8> pathCanBeTakenMaskStack;
  
  // NOTE(Constantine):
  // Add the initial global parent scope.
  parentExecutionMaskStack.push_back(1);
  executionMaskStack.push_back(1);
  pathCanBeTakenMaskStack.push_back(1);

  for (uint64_t tokenIndex = 0, tokensCount = stage1.tokenizer.tokens.size(); tokenIndex < tokensCount;) {
    ArcToken token1 = arc_s1p3_PeekToken(stage1, tokenIndex, 0);
    ArcToken token2 = arc_s1p3_PeekToken(stage1, tokenIndex, 1);
    ArcToken token3 = arc_s1p3_PeekToken(stage1, tokenIndex, 2);

    std::wstring token1String = arc_s1p2_TokenToStringCanBeEmpty(stage1, token1);
    std::wstring token2String = arc_s1p2_TokenToStringCanBeEmpty(stage1, token2);
    std::wstring token3String = arc_s1p2_TokenToStringCanBeEmpty(stage1, token3);

    if (
      (
        token1String == L"#"        &&
        token2String == L"if"
      )
      ||
      (
        token1String == L"#"        &&
        token2String == L"ifdef"
      )
      ||
      (
        token1String == L"#"        &&
        token2String == L"ifndef"
      )
      ||
      (
        token1String == L"#"        &&
        token2String == L"elif"
      )
      ||
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
      if (
        (
          token1String == L"#"      &&
          token2String == L"if"
        )
        ||
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
        ArcBool8 executionMask = executionMaskStack[arc_s1p3_GetLastIndexElseError(executionMaskStack, stage1)];
        parentExecutionMaskStack.push_back(executionMask);
        executionMaskStack.push_back(1);
        pathCanBeTakenMaskStack.push_back(1);
      }

      if (
        pathCanBeTakenMaskStack[arc_s1p3_GetLastIndexElseError(pathCanBeTakenMaskStack, stage1)] == 1 &&
        arc_s1p4_TokenCanConditionalMacroPathBeTakenBasedOnIdentifier(token1, token2String, token3, token3String, stage1) == 1
      )
      {
        pathCanBeTakenMaskStack[arc_s1p3_GetLastIndexElseError(pathCanBeTakenMaskStack, stage1)] = 0;
        executionMaskStack[arc_s1p3_GetLastIndexElseError(executionMaskStack, stage1)]           = 1;
      } else {
        executionMaskStack[arc_s1p3_GetLastIndexElseError(executionMaskStack, stage1)]           = 0;
      }

      arc_s1p4_RemoveToken(stage1, tokenIndex, tokensCount);
      arc_s1p4_RemoveToken(stage1, tokenIndex, tokensCount);
      arc_s1p4_RemoveToken(stage1, tokenIndex, tokensCount);
      // NOTE(Constantine): Intentionally not incrementing tokenIndex here.
      continue;
    }

    if (
      (
        token1String == L"#"     &&
        token2String == L"else"
      )
    )
    {
      if (pathCanBeTakenMaskStack[arc_s1p3_GetLastIndexElseError(pathCanBeTakenMaskStack, stage1)] == 1) {
        pathCanBeTakenMaskStack[arc_s1p3_GetLastIndexElseError(pathCanBeTakenMaskStack, stage1)] = 0;
        executionMaskStack[arc_s1p3_GetLastIndexElseError(executionMaskStack, stage1)]           = 1;
      } else {
        executionMaskStack[arc_s1p3_GetLastIndexElseError(executionMaskStack, stage1)]           = 0;
      }

      arc_s1p4_RemoveToken(stage1, tokenIndex, tokensCount);
      arc_s1p4_RemoveToken(stage1, tokenIndex, tokensCount);
      // NOTE(Constantine): Intentionally not incrementing tokenIndex here.
      continue;
    }

    if (
      (
        token1String == L"#"     &&
        token2String == L"endif"
      )
    )
    {
      parentExecutionMaskStack.pop_back();
      executionMaskStack.pop_back();
      pathCanBeTakenMaskStack.pop_back();

      arc_s1p4_RemoveToken(stage1, tokenIndex, tokensCount);
      arc_s1p4_RemoveToken(stage1, tokenIndex, tokensCount);
      // NOTE(Constantine): Intentionally not incrementing tokenIndex here.
      continue;
    }

    if (
      parentExecutionMaskStack[arc_s1p3_GetLastIndexElseError(parentExecutionMaskStack, stage1)] == 1 &&
      executionMaskStack[arc_s1p3_GetLastIndexElseError(executionMaskStack, stage1)] == 1
    )
    {
      tokenIndex += 1; // Next token.
      continue;
    } else {
      arc_s1p4_RemoveToken(stage1, tokenIndex, tokensCount);
      // NOTE(Constantine): Intentionally not incrementing tokenIndex here.
      continue;
    }
  }
}

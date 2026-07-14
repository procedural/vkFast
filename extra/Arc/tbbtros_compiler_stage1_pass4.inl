static TbbtrosBool8 internalTokenCanConditionalMacroPathBeTakenBasedOnIdentifier(TbbtrosToken tokenConditionalMacroStartToken, std::wstring tokenConditionalMacroNameString, TbbtrosToken tokenConditionalMacroIdentifierToken, std::wstring tokenConditionalMacroIdentifierString, TbbtrosStage1 const & stage1) {
  if (
    tokenConditionalMacroNameString == L"if"   ||
    tokenConditionalMacroNameString == L"elif"
  )
  {
    TbbtrosBool8 value0or1 = internalTokenGetEitherBoolean0Or1OtherwiseFatalError(stage1, tokenConditionalMacroIdentifierToken, tokenConditionalMacroIdentifierString);
    return value0or1;
  }
  if (
    tokenConditionalMacroNameString == L"ifdef"    ||
    tokenConditionalMacroNameString == L"ifndef"   ||
    tokenConditionalMacroNameString == L"elifdef"  ||
    tokenConditionalMacroNameString == L"elifndef"
  )
  {
    TbbtrosBool8 value0or1 = 0;

    for (size_t compilerCommandDefinedMacroIndex = 0, compilerCommandDefinedMacrosCount = stage1.compilerCommandDefinedMacros.macrosName.size(); compilerCommandDefinedMacroIndex < compilerCommandDefinedMacrosCount; compilerCommandDefinedMacroIndex += 1) {
      std::wstring definedMacroName = stage1.compilerCommandDefinedMacros.macrosName[compilerCommandDefinedMacroIndex];
      if (tokenConditionalMacroIdentifierString == definedMacroName) {
        value0or1 = (TbbtrosBool8)stage1.compilerCommandDefinedMacros.macrosValue[compilerCommandDefinedMacroIndex];
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
  internalInfoPrintAdditionalInfo(__FUNCTIONW__, stage1, &tokenConditionalMacroStartToken.stringOffset);
  internalFatalErrorWprintf(L"\n");
  internalFatalErrorWprintf(L"Fatal internal compiler error: unexpected conditional macro." L"\n");
  internalFatalErrorWprintf(L"\n");
  internalInfoPrintLinesAtCursorPosition(stage1, tokenConditionalMacroStartToken.stringOffset);
  internalFatalErrorAdditionalCalls();
  exit(1);
  return -1; // NOTE(Constantine): Unreachable, intentionally.
}

static void internalRemoveToken(TbbtrosStage1 & stage1, uint64_t removeTokenIndex, uint64_t & recalculateTokensCount) {
  size_t tokensCount = stage1.tokenizer.tokens.size();
  if (removeTokenIndex >= tokensCount) {
    internalInfoPrintAdditionalInfo(__FUNCTIONW__, stage1, NULL);
    internalFatalErrorWprintf(L"\n");
    internalFatalErrorWprintf(L"Fatal internal compiler error: attempting to access out of bounds token from tokens array." L"\n");
    internalFatalErrorWprintf(L"\n");
    internalFatalErrorAdditionalCalls();
    exit(1);
  }
  stage1.tokenizer.tokens.erase(stage1.tokenizer.tokens.begin() + removeTokenIndex);
  recalculateTokensCount = stage1.tokenizer.tokens.size();
}

static void internalStage1Pass4ConditionalMacroTokenFalseCodePathsElimination(TbbtrosStage1 & stage1) {
  std::vector<TbbtrosBool8> parentExecutionMaskStack;
  std::vector<TbbtrosBool8> executionMaskStack;
  std::vector<TbbtrosBool8> pathCanBeTakenMaskStack;
  
  // NOTE(Constantine):
  // Add the initial global parent scope.
  parentExecutionMaskStack.push_back(1);
  executionMaskStack.push_back(1);
  pathCanBeTakenMaskStack.push_back(1);

  for (uint64_t tokenIndex = 0, tokensCount = stage1.tokenizer.tokens.size(); tokenIndex < tokensCount;) {
    TbbtrosToken token1 = internalPeekToken(stage1, tokenIndex, 0);
    TbbtrosToken token2 = internalPeekToken(stage1, tokenIndex, 1);
    TbbtrosToken token3 = internalPeekToken(stage1, tokenIndex, 2);

    std::wstring token1String = internalTokenToStringCanBeEmpty(stage1, token1);
    std::wstring token2String = internalTokenToStringCanBeEmpty(stage1, token2);
    std::wstring token3String = internalTokenToStringCanBeEmpty(stage1, token3);

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
        TbbtrosBool8 executionMask = executionMaskStack[internalGetLastIndexElseError(executionMaskStack, stage1)];
        parentExecutionMaskStack.push_back(executionMask);
        executionMaskStack.push_back(1);
        pathCanBeTakenMaskStack.push_back(1);
      }

      if (
        pathCanBeTakenMaskStack[internalGetLastIndexElseError(pathCanBeTakenMaskStack, stage1)] == 1 &&
        internalTokenCanConditionalMacroPathBeTakenBasedOnIdentifier(token1, token2String, token3, token3String, stage1) == 1
      )
      {
        pathCanBeTakenMaskStack[internalGetLastIndexElseError(pathCanBeTakenMaskStack, stage1)] = 0;
        executionMaskStack[internalGetLastIndexElseError(executionMaskStack, stage1)]           = 1;
      } else {
        executionMaskStack[internalGetLastIndexElseError(executionMaskStack, stage1)]           = 0;
      }

      internalRemoveToken(stage1, tokenIndex, tokensCount);
      internalRemoveToken(stage1, tokenIndex, tokensCount);
      internalRemoveToken(stage1, tokenIndex, tokensCount);
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
      if (pathCanBeTakenMaskStack[internalGetLastIndexElseError(pathCanBeTakenMaskStack, stage1)] == 1) {
        pathCanBeTakenMaskStack[internalGetLastIndexElseError(pathCanBeTakenMaskStack, stage1)] = 0;
        executionMaskStack[internalGetLastIndexElseError(executionMaskStack, stage1)]           = 1;
      } else {
        executionMaskStack[internalGetLastIndexElseError(executionMaskStack, stage1)]           = 0;
      }

      internalRemoveToken(stage1, tokenIndex, tokensCount);
      internalRemoveToken(stage1, tokenIndex, tokensCount);
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

      internalRemoveToken(stage1, tokenIndex, tokensCount);
      internalRemoveToken(stage1, tokenIndex, tokensCount);
      // NOTE(Constantine): Intentionally not incrementing tokenIndex here.
      continue;
    }

    if (
      parentExecutionMaskStack[internalGetLastIndexElseError(parentExecutionMaskStack, stage1)] == 1 &&
      executionMaskStack[internalGetLastIndexElseError(executionMaskStack, stage1)] == 1
    )
    {
      tokenIndex += 1; // Next token.
      continue;
    } else {
      internalRemoveToken(stage1, tokenIndex, tokensCount);
      // NOTE(Constantine): Intentionally not incrementing tokenIndex here.
      continue;
    }
  }
}

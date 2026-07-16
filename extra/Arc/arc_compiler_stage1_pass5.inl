static std::wstring internalInt64ToWstring(int64_t number) {
  return std::to_wstring(number);
}

static std::wstring internalUint64ToWstring(uint64_t number) {
  return std::to_wstring(number);
}

uint64_t internalCursorPositionToLineIndex(TbbtrosStage1 const & stage1, uint64_t cursorPosition) {
  if (cursorPosition >= stage1.sourceCodeWithoutCommentsString.size()) {
    internalInfoPrintAdditionalInfo(__FUNCTIONW__, stage1, NULL);
    internalFatalErrorWprintf(L"\n");
    internalFatalErrorWprintf(L"Fatal internal compiler error: passed cursor position is outside of source code string's bounds." L"\n");
    internalFatalErrorWprintf(L"\n");
    internalFatalErrorAdditionalCalls();
    exit(1);
  }

  uint64_t lineIndex = 0;
  for (uint64_t i = 0, count = cursorPosition + 1; i < count;) {
    wchar_t character1 = internalPeekCharacter(stage1, i, 0);

    if (character1 == L'\n') {
      lineIndex += 1;
    }

    i += 1; // Skipping character.
  }

  return lineIndex;
}

uint64_t internalGetLineIndentation(TbbtrosStage1 const & stage1, uint64_t lineIndex) {
  uint64_t indentation = 0;

  uint64_t i = 0;
  for (uint64_t currentLineIndex = 0, count = stage1.sourceCodeWithoutCommentsString.size(); i < count;) {
    wchar_t character1 = internalPeekCharacter(stage1, i, 0);

    if (currentLineIndex == lineIndex) {
      break;
    }

    if (character1 == L'\n') {
      currentLineIndex += 1;
    }

    i += 1; // Skipping character.
  }

  for (uint64_t count = stage1.sourceCodeWithoutCommentsString.size(); i < count;) {
    wchar_t character1 = internalPeekCharacter(stage1, i, 0);

    if (character1 == L' ') {
      indentation += 1;
      i += 1; // Skipping character.
      continue;
    }

    break;
  }

  return indentation;
}

typedef struct np_0001_saveToFilepath {
  std::wstring v;
} np_0001_saveToFilepath;

static void internalDebugSaveTokensAsSourceCodeString(TbbtrosStage1 const & stage1, np_0001_saveToFilepath saveToFilepath) {
  std::wstring out;

  {
    uint64_t lastLineIndex = 0;
    for (TbbtrosToken token : stage1.tokenizer.tokens) {
      uint64_t currentLineIndex = internalCursorPositionToLineIndex(stage1, token.stringOffset);

      int isNewLine = 0;
      if (currentLineIndex > lastLineIndex) {
        isNewLine = 1;
        uint64_t insertNewLinesCount = currentLineIndex - lastLineIndex;
        for (uint64_t i = 0; i < insertNewLinesCount; i += 1) {
          out += L'\n';
        }
        lastLineIndex = currentLineIndex;
      }

      if (isNewLine == 0) {
        out += L' ';
      } else {
        uint64_t indentation = internalGetLineIndentation(stage1, currentLineIndex);
        for (uint64_t i = 0; i < indentation; i += 1) {
          out += L' ';
        }
      }
      out += internalTokenToString(stage1, token);
    }
  }

  internalFileWrite(saveToFilepath.v, out);
}

static void internalStage1Pass5PrintPragmaMessageAndErrorMacros(TbbtrosStage1 & stage1) {
  for (uint64_t tokenIndex = 0, tokensCount = stage1.tokenizer.tokens.size(); tokenIndex < tokensCount;) {
    TbbtrosToken token1 = internalPeekToken(stage1, tokenIndex, 0);
    TbbtrosToken token2 = internalPeekToken(stage1, tokenIndex, 1);
    TbbtrosToken token3 = internalPeekToken(stage1, tokenIndex, 2);

    std::wstring token1String = internalTokenToStringCanBeEmpty(stage1, token1);
    std::wstring token2String = internalTokenToStringCanBeEmpty(stage1, token2);
    std::wstring token3String = internalTokenToStringCanBeEmpty(stage1, token3);

    if (
      (
        token1String == L"#"       &&
        token2String == L"pragma"  &&
        token3String == L"message"
      )
    )
    {
      internalInfoPrintLinesAtCursorPosition(stage1, token1.stringOffset);
      internalInfoWprintf(L"\n");
      // NOTE(Constantine):
      // Removing tokens '#', 'pragma' and 'message' left by previous passes for this pass.
      internalRemoveToken(stage1, tokenIndex, tokensCount);
      internalRemoveToken(stage1, tokenIndex, tokensCount);
      internalRemoveToken(stage1, tokenIndex, tokensCount);
      // NOTE(Constantine): Intentionally not incrementing tokenIndex here.
      continue;
    }

    if (
      (
        token1String == L"#"     &&
        token2String == L"error"
      )
    )
    {
      internalInfoPrintAdditionalInfo(__FUNCTIONW__, stage1, &token1.stringOffset);
      internalFatalErrorWprintf(L"\n");
      internalFatalErrorWprintf(L"Fatal error: encountered an #error macro." L"\n");
      internalFatalErrorWprintf(L"\n");
      internalInfoPrintLinesAtCursorPosition(stage1, token1.stringOffset);
      internalFatalErrorAdditionalCalls();
      exit(1);
    }

    tokenIndex += 1; // Next token.
  }
}

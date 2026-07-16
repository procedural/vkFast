static std::wstring arc_s1p5_Int64ToWstring(int64_t number) {
  return std::to_wstring(number);
}

static std::wstring arc_s1p5_Uint64ToWstring(uint64_t number) {
  return std::to_wstring(number);
}

uint64_t arc_s1p5_CursorPositionToLineIndex(const ArcStateStage1 & stage1, uint64_t cursorPosition) {
  if (cursorPosition >= stage1.sourceCodeWithoutCommentsString.size()) {
    arc_s1p1_InfoPrintAdditionalInfo(__FUNCTION__, stage1, NULL);
    arc_wprintf_fatalError(L"\n");
    arc_wprintf_fatalError(L"Fatal internal compiler error: passed cursor position is outside of source code string's bounds." L"\n");
    arc_wprintf_fatalError(L"\n");
    arc_s1p1_FatalErrorAdditionalCalls();
    exit(1);
  }

  uint64_t lineIndex = 0;
  for (uint64_t i = 0, count = cursorPosition + 1; i < count;) {
    wchar_t character1 = arc_s1p1_PeekCharacter(stage1, i, 0);

    if (character1 == L'\n') {
      lineIndex += 1;
    }

    i += 1; // Skipping character.
  }

  return lineIndex;
}

uint64_t arc_s1p5_GetLineIndentation(const ArcStateStage1 & stage1, uint64_t lineIndex) {
  uint64_t indentation = 0;

  uint64_t i = 0;
  for (uint64_t currentLineIndex = 0, count = stage1.sourceCodeWithoutCommentsString.size(); i < count;) {
    wchar_t character1 = arc_s1p1_PeekCharacter(stage1, i, 0);

    if (currentLineIndex == lineIndex) {
      break;
    }

    if (character1 == L'\n') {
      currentLineIndex += 1;
    }

    i += 1; // Skipping character.
  }

  for (uint64_t count = stage1.sourceCodeWithoutCommentsString.size(); i < count;) {
    wchar_t character1 = arc_s1p1_PeekCharacter(stage1, i, 0);

    if (character1 == L' ') {
      indentation += 1;
      i += 1; // Skipping character.
      continue;
    }

    break;
  }

  return indentation;
}

#ifdef arc_np_0001
#error arc_np_0001
#endif
#define arc_np_0001
typedef struct arc_np_0001_saveToFilepath {
  std::wstring v;
} arc_np_0001_saveToFilepath;

static void arc_s1p5_DebugSaveTokensAsSourceCodeString(const ArcStateStage1 & stage1, arc_np_0001_saveToFilepath saveToFilepath) {
  std::wstring out;

  {
    uint64_t lastLineIndex = 0;
    for (ArcToken token : stage1.tokenizer.tokens) {
      uint64_t currentLineIndex = arc_s1p5_CursorPositionToLineIndex(stage1, token.stringOffset);

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
        uint64_t indentation = arc_s1p5_GetLineIndentation(stage1, currentLineIndex);
        for (uint64_t i = 0; i < indentation; i += 1) {
          out += L' ';
        }
      }
      out += arc_s1p2_TokenToString(stage1, token);
    }
  }

  arc_s1p1_FileWrite(saveToFilepath.v, out);
}

static void arc_s1p5_Stage1Pass5PrintPragmaMessageAndErrorMacros(ArcStateStage1 & stage1) {
  for (uint64_t tokenIndex = 0, tokensCount = stage1.tokenizer.tokens.size(); tokenIndex < tokensCount;) {
    ArcToken token1 = arc_s1p3_PeekToken(stage1, tokenIndex, 0);
    ArcToken token2 = arc_s1p3_PeekToken(stage1, tokenIndex, 1);
    ArcToken token3 = arc_s1p3_PeekToken(stage1, tokenIndex, 2);

    std::wstring token1String = arc_s1p2_TokenToStringCanBeEmpty(stage1, token1);
    std::wstring token2String = arc_s1p2_TokenToStringCanBeEmpty(stage1, token2);
    std::wstring token3String = arc_s1p2_TokenToStringCanBeEmpty(stage1, token3);

    if (
      (
        token1String == L"#"       &&
        token2String == L"pragma"  &&
        token3String == L"message"
      )
    )
    {
      arc_s1p1_InfoPrintLinesAtCursorPosition(stage1, token1.stringOffset);
      arc_wprintf_info(L"\n");
      // NOTE(Constantine):
      // Removing tokens '#', 'pragma' and 'message' left by previous passes for this pass.
      arc_s1p4_RemoveToken(stage1, tokenIndex, tokensCount);
      arc_s1p4_RemoveToken(stage1, tokenIndex, tokensCount);
      arc_s1p4_RemoveToken(stage1, tokenIndex, tokensCount);
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
      arc_s1p1_InfoPrintAdditionalInfo(__FUNCTION__, stage1, &token1.stringOffset);
      arc_wprintf_fatalError(L"\n");
      arc_wprintf_fatalError(L"Fatal error: encountered an #error macro." L"\n");
      arc_wprintf_fatalError(L"\n");
      arc_s1p1_InfoPrintLinesAtCursorPosition(stage1, token1.stringOffset);
      arc_s1p1_FatalErrorAdditionalCalls();
      exit(1);
    }

    tokenIndex += 1; // Next token.
  }
}

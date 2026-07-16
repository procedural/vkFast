static void arc_s1p2_TokenEatOneWideCharacterAndAdvanceCursorPositionByOne(ArcToken & token, const ArcStateStage1 & stage1, uint64_t & cursorPosition) {
  token.stringLength += 1;
  cursorPosition     += 1;
}

static ArcBool8 arc_s1p2_CharacterIsNumber(wchar_t c) {
  if (
    c == L'0' || c == L'1' || c == L'2' || c == L'3' || c == L'4' || c == L'5' || c == L'6' || c == L'7' || c == L'8' || c == L'9'
  )
  {
    return 1;
  }
  return 0;
}

static ArcBool8 arc_s1p2_CharacterIsAnyAsciiLetter(wchar_t c) {
  if (
    c == L'a' || c == L'b' || c == L'c' || c == L'd' || c == L'e' || c == L'f' || c == L'g' || c == L'h' || c == L'i' || c == L'j' || c == L'k' || c == L'l' || c == L'm' ||
    c == L'n' || c == L'o' || c == L'p' || c == L'q' || c == L'r' || c == L's' || c == L't' || c == L'u' || c == L'v' || c == L'w' || c == L'x' || c == L'y' || c == L'z' ||
    
    c == L'A' || c == L'B' || c == L'C' || c == L'D' || c == L'E' || c == L'F' || c == L'G' || c == L'H' || c == L'I' || c == L'J' || c == L'K' || c == L'L' || c == L'M' ||
    c == L'N' || c == L'O' || c == L'P' || c == L'Q' || c == L'R' || c == L'S' || c == L'T' || c == L'U' || c == L'V' || c == L'W' || c == L'X' || c == L'Y' || c == L'Z'
  )
  {
    return 1;
  }
  return 0;
}

static ArcBool8 arc_s1p2_CharacterIsIdentifierBeginning(wchar_t c) {
  if (
    c == L'a' || c == L'b' || c == L'c' || c == L'd' || c == L'e' || c == L'f' || c == L'g' || c == L'h' || c == L'i' || c == L'j' || c == L'k' || c == L'l' || c == L'm' ||
    c == L'n' || c == L'o' || c == L'p' || c == L'q' || c == L'r' || c == L's' || c == L't' || c == L'u' || c == L'v' || c == L'w' || c == L'x' || c == L'y' || c == L'z' ||
    
    c == L'A' || c == L'B' || c == L'C' || c == L'D' || c == L'E' || c == L'F' || c == L'G' || c == L'H' || c == L'I' || c == L'J' || c == L'K' || c == L'L' || c == L'M' ||
    c == L'N' || c == L'O' || c == L'P' || c == L'Q' || c == L'R' || c == L'S' || c == L'T' || c == L'U' || c == L'V' || c == L'W' || c == L'X' || c == L'Y' || c == L'Z' ||
    
    c == L'_'
  )
  {
    return 1;
  }
  return 0;
}

static ArcBool8 arc_s1p2_CharacterIsIdentifier(wchar_t c) {
  if (
    c == L'0' || c == L'1' || c == L'2' || c == L'3' || c == L'4' || c == L'5' || c == L'6' || c == L'7' || c == L'8' || c == L'9' ||

    c == L'a' || c == L'b' || c == L'c' || c == L'd' || c == L'e' || c == L'f' || c == L'g' || c == L'h' || c == L'i' || c == L'j' || c == L'k' || c == L'l' || c == L'm' ||
    c == L'n' || c == L'o' || c == L'p' || c == L'q' || c == L'r' || c == L's' || c == L't' || c == L'u' || c == L'v' || c == L'w' || c == L'x' || c == L'y' || c == L'z' ||
    
    c == L'A' || c == L'B' || c == L'C' || c == L'D' || c == L'E' || c == L'F' || c == L'G' || c == L'H' || c == L'I' || c == L'J' || c == L'K' || c == L'L' || c == L'M' ||
    c == L'N' || c == L'O' || c == L'P' || c == L'Q' || c == L'R' || c == L'S' || c == L'T' || c == L'U' || c == L'V' || c == L'W' || c == L'X' || c == L'Y' || c == L'Z' ||
    
    c == L'_'
  )
  {
    return 1;
  }
  return 0;
}

static ArcBool8 arc_s1p2_CharacterIsKnownSymbol(wchar_t c) {
  if (
    c == L'!' ||
    c == L'"' ||
    c == L'#' ||
    c == L'%' ||
    c == L'&' ||
    c == L'\'' ||
    c == L'(' ||
    c == L')' ||
    c == L'*' ||
    c == L'+' ||
    c == L',' ||
    c == L'-' ||
    c == L'.' ||
    c == L'/' ||
    c == L':' ||
    c == L';' ||
    c == L'<' ||
    c == L'=' ||
    c == L'>' ||
    c == L'?' ||
    c == L'[' ||
    c == L']' ||
    c == L'^' ||
    c == L'{' ||
    c == L'|' ||
    c == L'}' ||
    c == L'~'
  )
  {
    return 1;
  }
  return 0;
}

static void arc_s1p2_SourceCodeAppendToken(ArcStateStage1 & stage1, const ArcToken & token) {
  if (token.stringLength == 0) {
    arc_s1p1_InfoPrintAdditionalInfo(__FUNCTION__, stage1, &token.stringOffset);
    arc_wprintf_fatalError(L"\n");
    arc_wprintf_fatalError(L"Fatal internal compiler error: encountered a token at offset %zu with its length unexpectedly equal to 0." L"\n", token.stringOffset);
    arc_wprintf_fatalError(L"\n");
    arc_s1p1_InfoPrintLinesAtCursorPosition(stage1, token.stringOffset);
    arc_s1p1_FatalErrorAdditionalCalls();
    exit(1);
  }
  stage1.tokenizer.tokens.push_back(token);
}

static std::wstring arc_s1p2_TokenToStringCanBeEmpty(const ArcStateStage1 & stage1, const ArcToken & token) {
  std::wstring tokenString;
  for (uint64_t i = token.stringOffset; i < (token.stringOffset + token.stringLength); i += 1) {
    tokenString += stage1.sourceCodeWithoutCommentsString[i];
  }
  return tokenString;
}

static std::wstring arc_s1p2_TokenToString(const ArcStateStage1 & stage1, const ArcToken & token) {
  if (token.stringLength == 0) {
    arc_s1p1_InfoPrintAdditionalInfo(__FUNCTION__, stage1, &token.stringOffset);
    arc_wprintf_fatalError(L"\n");
    arc_wprintf_fatalError(L"Fatal internal compiler error: encountered a token at offset %zu with its length unexpectedly equal to 0." L"\n", token.stringOffset);
    arc_wprintf_fatalError(L"\n");
    arc_s1p1_InfoPrintLinesAtCursorPosition(stage1, token.stringOffset);
    arc_s1p1_FatalErrorAdditionalCalls();
    exit(1);
  }
  std::wstring tokenString;
  for (uint64_t i = token.stringOffset; i < (token.stringOffset + token.stringLength); i += 1) {
    tokenString += stage1.sourceCodeWithoutCommentsString[i];
  }
  if (tokenString.empty()) {
    arc_s1p1_InfoPrintAdditionalInfo(__FUNCTION__, stage1, &token.stringOffset);
    arc_wprintf_fatalError(L"\n");
    arc_wprintf_fatalError(L"Fatal internal compiler error: encountered a token at offset %zu that unexpectedly converts to an empty string." L"\n", token.stringOffset);
    arc_wprintf_fatalError(L"\n");
    arc_s1p1_InfoPrintLinesAtCursorPosition(stage1, token.stringOffset);
    arc_s1p1_FatalErrorAdditionalCalls();
    exit(1);
  }
  return tokenString;
}

#ifdef arc_np_0000
#error arc_np_0000
#endif
#define arc_np_0000
typedef struct arc_np_0000_optionalPrintPerLineTokensCount {
  uint64_t v;
} arc_np_0000_optionalPrintPerLineTokensCount;

static void arc_s1p2_WprintfDebugTokenizer(const ArcStateStage1 & stage1, arc_np_0000_optionalPrintPerLineTokensCount optionalPrintPerLineTokensCount) {
  arc_wprintf_debug(L"[DEBUG][%s]\n", __FUNCTION__);
  arc_wprintf_debug(L"std::wstring tokenizerTokens[] = {" L"\n");
  uint64_t tokensPerLineCounter = 0;
  for (uint64_t i = 0, tokensCount = stage1.tokenizer.tokens.size(); i < tokensCount; i += 1) {
    if (i != 0) {
      arc_wprintf_debug(L", ");
    }
    std::wstring tokenString = arc_s1p2_TokenToString(stage1, stage1.tokenizer.tokens[i]);
    if (tokenString == L"\"") {
      arc_wprintf_debug(L"L\"\\%ls\"", tokenString.c_str());
    } else {
      arc_wprintf_debug(L"L\"%ls\"", tokenString.c_str());
    }
    tokensPerLineCounter += 1;
    if (optionalPrintPerLineTokensCount.v > 0) {
      if (tokensPerLineCounter >= optionalPrintPerLineTokensCount.v) {
        arc_wprintf_debug(L"\n");
        tokensPerLineCounter = 0;
      }
    }
  }
  arc_wprintf_debug(L"};\n");
}

static void arc_s1p2_TokenizerParseMulticharacterNumberLiteral(ArcStateStage1 & stage1, uint64_t & i, uint64_t count) {
  ArcToken token = {i, 0};
  for (; i < count;) {
    wchar_t character1 = arc_s1p1_PeekCharacter(stage1, i, 0);

    if (arc_s1p2_CharacterIsNumber(character1)) {
      arc_s1p2_TokenEatOneWideCharacterAndAdvanceCursorPositionByOne(token, stage1, i);
      continue;
    }
    if (character1 == L'-') { // NOTE(Constantine): Examples: -5
      arc_s1p2_TokenEatOneWideCharacterAndAdvanceCursorPositionByOne(token, stage1, i);
      continue;
    }
    if (character1 == L'.') { // NOTE(Constantine): Examples: .5, 0.01
      arc_s1p2_TokenEatOneWideCharacterAndAdvanceCursorPositionByOne(token, stage1, i);
      continue;
    }
    if (character1 == L'\'') { // NOTE(Constantine): Examples (C23): 0xFE'DC'BA'98, 299'792'458, 1.414'213'562
      arc_s1p2_TokenEatOneWideCharacterAndAdvanceCursorPositionByOne(token, stage1, i);
      continue;
    }
    if (arc_s1p2_CharacterIsAnyAsciiLetter(character1)) { // NOTE(Constantine): Examples: 0.5f, 42ULL, 0xcafebabe
      arc_s1p2_TokenEatOneWideCharacterAndAdvanceCursorPositionByOne(token, stage1, i);
      continue;
    }

    arc_s1p2_SourceCodeAppendToken(stage1, token); // NOTE(Constantine): Append the previously accumulated number token.
    break;
  }
}

static void arc_s1p2_TokenizerParseMulticharacterIdentifier(ArcStateStage1 & stage1, uint64_t & i, uint64_t count, ArcToken * optionalOutToken = NULL, ArcBool8 optionalAppendParsedToken = 1) {
  ArcToken token = {i, 0};
  for (; i < count;) {
    wchar_t character1 = arc_s1p1_PeekCharacter(stage1, i, 0);

    if (arc_s1p2_CharacterIsIdentifier(character1)) {
      arc_s1p2_TokenEatOneWideCharacterAndAdvanceCursorPositionByOne(token, stage1, i);
      continue;
    }

    if (optionalOutToken != NULL) {
      optionalOutToken[0] = token;
    }

    if (optionalAppendParsedToken == 1) {
      arc_s1p2_SourceCodeAppendToken(stage1, token); // NOTE(Constantine): Append the previously accumulated identifier token.
    }
    break;
  }
}

static void arc_s1p2_TokenizerParseMulticharacterMacroSkipSpaceCharacters(const ArcStateStage1 & stage1, uint64_t & i, uint64_t count) {
  // NOTE(Constantine): Parsing possible space and new line characters.
  for (ArcBool8 continueParsingToNextLine = 0; i < count;) {
    wchar_t character1 = arc_s1p1_PeekCharacter(stage1, i, 0);

    if (character1 == L' ') {
      i += 1; // Skipping character.
      continue;
    }
    if (character1 == L'\n' && continueParsingToNextLine == 0) {
      arc_s1p1_InfoPrintAdditionalInfo(__FUNCTION__, stage1, &i);
      arc_wprintf_fatalError(L"\n");
      arc_wprintf_fatalError(L"Fatal error: macro parsing encountered an unexpected terminating new line character." L"\n");
      arc_wprintf_fatalError(L"\n");
      arc_s1p1_InfoPrintLinesAtCursorPosition(stage1, i);
      arc_s1p1_FatalErrorAdditionalCalls();
      exit(1);
    }
    if (character1 == L'\n' && continueParsingToNextLine == 1) {
      continueParsingToNextLine = 0;
      i += 1; // Skipping character.
      continue;
    }
    if (character1 == L'\\') {
      continueParsingToNextLine = 1;
      i += 1; // Skipping character.
      continue;
    }

    break;
  }
}

static void arc_s1p2_TokenizerParseMulticharacterMacroSkipSpaceCharactersToTheTerminatingNewLineCharacterOtherwiseFatalError(const ArcStateStage1 & stage1, uint64_t & i, uint64_t count) {
  // NOTE(Constantine): Parsing possible space and new line characters.
  for (ArcBool8 continueParsingToNextLine = 0; i < count;) {
    wchar_t character1 = arc_s1p1_PeekCharacter(stage1, i, 0);

    if (character1 == L' ') {
      i += 1; // Skipping character.
      continue;
    }
    if (character1 == L'\n' && continueParsingToNextLine == 0) {
      // NOTE(Constantine): Intentionally not i += 1; here.
      break;
    }
    if (character1 == L'\n' && continueParsingToNextLine == 1) {
      continueParsingToNextLine = 0;
      i += 1; // Skipping character.
      continue;
    }
    if (character1 == L'\\') {
      continueParsingToNextLine = 1;
      i += 1; // Skipping character.
      continue;
    }

    arc_s1p1_InfoPrintAdditionalInfo(__FUNCTION__, stage1, &i);
    arc_wprintf_fatalError(L"\n");
    arc_wprintf_fatalError(L"Fatal error: macro parsing encountered a character when the macro was expected to finish instead." L"\n");
    arc_wprintf_fatalError(L"\n");
    arc_s1p1_InfoPrintLinesAtCursorPosition(stage1, i);
    arc_s1p1_FatalErrorAdditionalCalls();
    exit(1);
  }
}

static void arc_s1p2_CompilerCheckCharacterIsIdentifierBeginningOtherwiseFatalError(wchar_t character, const char * const functionName, const ArcStateStage1 & stage1, uint64_t & i) {
  if (arc_s1p2_CharacterIsIdentifierBeginning(character) == 0) {
    arc_s1p1_InfoPrintAdditionalInfo(functionName, stage1, &i);
    arc_wprintf_fatalError(L"\n");
    arc_wprintf_fatalError(L"Fatal error: encountered an unsupported character at the beginning of an identifier." L"\n");
    arc_wprintf_fatalError(L"\n");
    arc_s1p1_InfoPrintLinesAtCursorPosition(stage1, i);
    arc_s1p1_FatalErrorAdditionalCalls();
    exit(1);
  }
}

static void arc_s1p2_CheckCompilerSupportsMacroOtherwiseFatalError(const ArcStateStage1 & stage1, uint64_t i, std::wstring tokenMacroStartString, std::wstring tokenMacroNameString, std::wstring tokenMacroIdentifierString) {
  if (
    (
      tokenMacroStartString      == L"#"        &&
      tokenMacroNameString       == L"pragma"   &&
      tokenMacroIdentifierString == L"once"
    )
    ||
    (
      tokenMacroStartString      == L"#"        &&
      tokenMacroNameString       == L"include"
    )
    ||
    (
      tokenMacroStartString      == L"#"        &&
      tokenMacroNameString       == L"define"
    )
    ||
    (
      tokenMacroStartString      == L"#"        &&
      tokenMacroNameString       == L"if"
    )
    ||
    (
      tokenMacroStartString      == L"#"        &&
      tokenMacroNameString       == L"elif"
    )
    ||
    (
      tokenMacroStartString      == L"#"        &&
      tokenMacroNameString       == L"else"
    )
    ||
    (
      tokenMacroStartString      == L"#"        &&
      tokenMacroNameString       == L"endif"
    )
    ||
    (
      tokenMacroStartString      == L"#"        &&
      tokenMacroNameString       == L"ifdef"
    )
    ||
    (
      tokenMacroStartString      == L"#"        &&
      tokenMacroNameString       == L"ifndef"
    )
    ||
    (
      tokenMacroStartString      == L"#"        &&
      tokenMacroNameString       == L"elifdef"
    )
    ||
    (
      tokenMacroStartString      == L"#"        &&
      tokenMacroNameString       == L"elifndef"
    )
    ||
    (
      tokenMacroStartString      == L"#"        &&
      tokenMacroNameString       == L"error"
    )
    ||
    (
      tokenMacroStartString      == L"#"        &&
      tokenMacroNameString       == L"pragma"   &&
      tokenMacroIdentifierString == L"message"
    )
  )
  {
    // These macros are known to the compiler, check is passed.
  }
  else
  {
    arc_s1p1_InfoPrintAdditionalInfo(__FUNCTION__, stage1, &i);
    arc_wprintf_fatalError(L"\n");
    arc_wprintf_fatalError(L"Fatal error: encountered an unknown or unsupported macro." L"\n");
    arc_wprintf_fatalError(L"\n");
    arc_s1p1_InfoPrintLinesAtCursorPosition(stage1, i);
    arc_s1p1_FatalErrorAdditionalCalls();
    exit(1);
  }
}

static void arc_s1p2_TokenizerParseMulticharacterMacro(ArcStateStage1 & stage1, uint64_t & i, uint64_t count) {
  // NOTE(Constantine): Parsing macro start hash character.
  {
    wchar_t character1 = arc_s1p1_PeekCharacter(stage1, i, 0);
    if (character1 != L'#') {
      arc_s1p1_InfoPrintAdditionalInfo(__FUNCTION__, stage1, &i);
      arc_wprintf_fatalError(L"\n");
      arc_wprintf_fatalError(L"Fatal error: macro is expected to start with a # character." L"\n");
      arc_wprintf_fatalError(L"\n");
      arc_s1p1_InfoPrintLinesAtCursorPosition(stage1, i);
      arc_s1p1_FatalErrorAdditionalCalls();
      exit(1);
    }
  }
  ArcToken tokenMacroStart = {i, 0};
  arc_s1p2_TokenEatOneWideCharacterAndAdvanceCursorPositionByOne(tokenMacroStart, stage1, i);

  // NOTE(Constantine): Skip possible space and new line characters.
  arc_s1p2_TokenizerParseMulticharacterMacroSkipSpaceCharacters(stage1, i, count);

  // NOTE(Constantine): Parsing macro name.
  ArcToken tokenMacroName = {i, 0};
  arc_s1p2_CompilerCheckCharacterIsIdentifierBeginningOtherwiseFatalError(arc_s1p1_PeekCharacter(stage1, i, 0), __FUNCTION__, stage1, i);
  arc_s1p2_TokenizerParseMulticharacterIdentifier(stage1, i, count, &tokenMacroName, 0);

  // NOTE(Constantine): Caching token strings.
  std::wstring tokenMacroStartString = arc_s1p2_TokenToString(stage1, tokenMacroStart);
  std::wstring tokenMacroNameString  = arc_s1p2_TokenToString(stage1, tokenMacroName);

  // NOTE(Constantine): Parsing macro identifier.
  ArcToken tokenMacroIdentifier = {i, 0};
  if (
    (
      tokenMacroStartString == L"#"      &&
      tokenMacroNameString  == L"pragma"
    )
    ||
    (
      tokenMacroStartString == L"#"      &&
      tokenMacroNameString  == L"define"
    )
  )
  {
    // NOTE(Constantine): Skip possible space and new line characters.
    arc_s1p2_TokenizerParseMulticharacterMacroSkipSpaceCharacters(stage1, i, count);
    
    // NOTE(Constantine): Advancing the initial offset after skipping possible space and new line characters above.
    tokenMacroIdentifier.stringOffset = i;
    
    // NOTE(Constantine): Parsing macro identifier.
    arc_s1p2_CompilerCheckCharacterIsIdentifierBeginningOtherwiseFatalError(arc_s1p1_PeekCharacter(stage1, i, 0), __FUNCTION__, stage1, i);
    arc_s1p2_TokenizerParseMulticharacterIdentifier(stage1, i, count, &tokenMacroIdentifier, 0);
  }
  // NOTE(Constantine):
  // Parsing conditional macros.
  else if (
    (
      tokenMacroStartString == L"#"        &&
      tokenMacroNameString  == L"if"
    )
    ||
    (
      tokenMacroStartString == L"#"        &&
      tokenMacroNameString  == L"elif"
    )
    ||
    (
      tokenMacroStartString == L"#"        &&
      tokenMacroNameString  == L"ifdef"
    )
    ||
    (
      tokenMacroStartString == L"#"        &&
      tokenMacroNameString  == L"ifndef"
    )
    ||
    (
      tokenMacroStartString == L"#"        &&
      tokenMacroNameString  == L"elifdef"
    )
    ||
    (
      tokenMacroStartString == L"#"        &&
      tokenMacroNameString  == L"elifndef"
    )
  )
  {
    // NOTE(Constantine): Skip possible space and new line characters.
    arc_s1p2_TokenizerParseMulticharacterMacroSkipSpaceCharacters(stage1, i, count);

    // NOTE(Constantine): Advancing the initial offset after skipping possible space and new line characters above.
    tokenMacroIdentifier.stringOffset = i;

    // NOTE(Constantine):
    // Intentionally not calling arc_s1p2_CompilerCheckCharacterIsIdentifierBeginningOtherwiseFatalError
    // since the identifier can be a number such as 0 or 1.
    arc_s1p2_TokenizerParseMulticharacterIdentifier(stage1, i, count, &tokenMacroIdentifier, 0);

    // NOTE(Constantine):
    // Skipping possible space characters to the terminating new line character,
    // otherwise it's not of one of the supported conditional macros, so have to
    // exit the compiler with an error instead. One identifier per conditional
    // macro is a current limitation of the compiler.
    arc_s1p2_TokenizerParseMulticharacterMacroSkipSpaceCharactersToTheTerminatingNewLineCharacterOtherwiseFatalError(stage1, i, count);
  }

  // NOTE(Constantine): Caching token strings.
  std::wstring tokenMacroIdentifierString = arc_s1p2_TokenToStringCanBeEmpty(stage1, tokenMacroIdentifier);

  arc_s1p2_CheckCompilerSupportsMacroOtherwiseFatalError(stage1, tokenMacroStart.stringOffset, tokenMacroStartString, tokenMacroNameString, tokenMacroIdentifierString);

  // NOTE(Constantine): Skipping everything else up to and including the new line character, with \ in mind.
  for (ArcBool8 continueParsingToNextLine = 0; i < count;) {
    wchar_t character1 = arc_s1p1_PeekCharacter(stage1, i, 0);

    if (character1 == L'\\') {
      continueParsingToNextLine = 1;
      i += 1; // Skipping character.
      continue;
    }
    if (character1 == L'\n' && continueParsingToNextLine == 1) {
      continueParsingToNextLine = 0;
      i += 1; // Skipping character.
      continue;
    }
    if (character1 == L'\n' && continueParsingToNextLine == 0) {
      i += 1; // Skipping character.
      break;
    }

    i += 1; // Skipping character.
  }

  if (
    (
      tokenMacroStartString      == L"#"      &&
      tokenMacroNameString       == L"pragma" &&
      tokenMacroIdentifierString == L"once"
    )
    ||
    (
      tokenMacroStartString      == L"#"       &&
      tokenMacroNameString       == L"include"
    )
    ||
    (
      tokenMacroStartString      == L"#"       &&
      tokenMacroNameString       == L"define"
    )
  )
  {
    // These macros are explicitly ignored and not added to the tokenizer's tokens array.
  }
  else
  {
    arc_s1p2_SourceCodeAppendToken(stage1, tokenMacroStart);
    arc_s1p2_SourceCodeAppendToken(stage1, tokenMacroName);
    if (tokenMacroIdentifier.stringLength > 0) {
      arc_s1p2_SourceCodeAppendToken(stage1, tokenMacroIdentifier);
    }
  }
}

static void arc_s1p2_TokenizerParseMulticharacterString(ArcStateStage1 & stage1, uint64_t & i, uint64_t count, ArcBool8 optionalAppendParsedTokens = 1) {
  // NOTE(Constantine): Parsing open string literal character token
  {
    wchar_t character1 = arc_s1p1_PeekCharacter(stage1, i, 0);
    if (character1 != L'\"') {
      arc_s1p1_InfoPrintAdditionalInfo(__FUNCTION__, stage1, &i);
      arc_wprintf_fatalError(L"\n");
      arc_wprintf_fatalError(L"Fatal error: multicharacter string literal parsing expected to start with a \" character." L"\n");
      arc_wprintf_fatalError(L"\n");
      arc_s1p1_InfoPrintLinesAtCursorPosition(stage1, i);
      arc_s1p1_FatalErrorAdditionalCalls();
      exit(1);
    }
  }
  ArcToken tokenOpenStringLiteralCharacter = {i, 0};
  arc_s1p2_TokenEatOneWideCharacterAndAdvanceCursorPositionByOne(tokenOpenStringLiteralCharacter, stage1, i);

  // NOTE(Constantine): Parsing everything up to but not including ", with \ in mind.
  ArcToken tokenStringLiteral = {i, 0};
  for (; i < count;) {
    wchar_t character1 = arc_s1p1_PeekCharacter(stage1, i, 0);
    wchar_t character2 = arc_s1p1_PeekCharacter(stage1, i, 1);

    if (character1 == L'\\') {
      arc_s1p2_TokenEatOneWideCharacterAndAdvanceCursorPositionByOne(tokenStringLiteral, stage1, i); // NOTE(Constantine): The token string will contain \ character.
      arc_s1p2_TokenEatOneWideCharacterAndAdvanceCursorPositionByOne(tokenStringLiteral, stage1, i);
      continue;
    }

    if (character1 == L'"') {
      break;
    }

    if (character1 == 0 || character1 == L'\n') {
      arc_s1p1_InfoPrintAdditionalInfo(__FUNCTION__, stage1, &i);
      arc_wprintf_fatalError(L"\n");
      arc_wprintf_fatalError(L"Fatal error: encountered end of file or a new line terminating character when an opened string literal was not closed yet." L"\n");
      arc_wprintf_fatalError(L"\n");
      arc_s1p1_InfoPrintLinesAtCursorPosition(stage1, i);
      arc_s1p1_FatalErrorAdditionalCalls();
      exit(1);
    }

    arc_s1p2_TokenEatOneWideCharacterAndAdvanceCursorPositionByOne(tokenStringLiteral, stage1, i);
  }

  // NOTE(Constantine): Parsing close string literal character token.
  {
    wchar_t character1 = arc_s1p1_PeekCharacter(stage1, i, 0);
    if (character1 != L'\"') {
      arc_s1p1_InfoPrintAdditionalInfo(__FUNCTION__, stage1, &i);
      arc_wprintf_fatalError(L"\n");
      arc_wprintf_fatalError(L"Fatal error: multicharacter string literal parsing expected to close with a \" character." L"\n");
      arc_wprintf_fatalError(L"\n");
      arc_s1p1_InfoPrintLinesAtCursorPosition(stage1, i);
      arc_s1p1_FatalErrorAdditionalCalls();
      exit(1);
    }
  }
  ArcToken tokenCloseStringLiteralCharacter = {i, 0};
  arc_s1p2_TokenEatOneWideCharacterAndAdvanceCursorPositionByOne(tokenCloseStringLiteralCharacter, stage1, i);

  if (optionalAppendParsedTokens == 1) {
    arc_s1p2_SourceCodeAppendToken(stage1, tokenOpenStringLiteralCharacter);
    arc_s1p2_SourceCodeAppendToken(stage1, tokenStringLiteral);
    arc_s1p2_SourceCodeAppendToken(stage1, tokenCloseStringLiteralCharacter);
  }
}

static void arc_s1p2_TokenizerParseSingleOperator(ArcStateStage1 & stage1, uint64_t & i, uint64_t count) {
  ArcToken token = {i, 0};

  wchar_t character1 = arc_s1p1_PeekCharacter(stage1, i, 0);

  if (arc_s1p2_CharacterIsKnownSymbol(character1)) {
    arc_s1p2_TokenEatOneWideCharacterAndAdvanceCursorPositionByOne(token, stage1, i);
    arc_s1p2_SourceCodeAppendToken(stage1, token);
    return;
  }
}

static void arc_s1p2_TokenizerParseDoubleOperator(ArcStateStage1 & stage1, uint64_t & i, uint64_t count) {
  ArcToken token = {i, 0};

  wchar_t character1 = arc_s1p1_PeekCharacter(stage1, i, 0);
  wchar_t character2 = arc_s1p1_PeekCharacter(stage1, i, 1);

  if (arc_s1p2_CharacterIsKnownSymbol(character1) && arc_s1p2_CharacterIsKnownSymbol(character2)) {
    arc_s1p2_TokenEatOneWideCharacterAndAdvanceCursorPositionByOne(token, stage1, i);
    arc_s1p2_TokenEatOneWideCharacterAndAdvanceCursorPositionByOne(token, stage1, i);
    arc_s1p2_SourceCodeAppendToken(stage1, token);
    return;
  }
}

static void arc_s1p2_Stage1Pass2SourceCodeFillTokenizerStruct(ArcStateStage1 & stage1) {
  for (uint64_t i = 0, count = stage1.sourceCodeWithoutCommentsString.size(); i < count;) {
    wchar_t character1 = arc_s1p1_PeekCharacter(stage1, i, 0);
    wchar_t character2 = arc_s1p1_PeekCharacter(stage1, i, 1);

    if (character1 == L' ' || character1 == L'\n' || character1 == L'\t') { // NOTE(Constantine): Thanks to @kiwec for spotting the missing tab '\t' character handling in gen22! :)
      i += 1; // Skipping character.
      continue;
    }

    if (character1 == L'.' && arc_s1p2_CharacterIsNumber(character2)) { // NOTE(Constantine): Example: .5
      arc_s1p2_TokenizerParseMulticharacterNumberLiteral(stage1, i, count);
      continue;
    }

    if (arc_s1p2_CharacterIsNumber(character1)) {
      arc_s1p2_TokenizerParseMulticharacterNumberLiteral(stage1, i, count);
      continue;
    }

    if (arc_s1p2_CharacterIsIdentifierBeginning(character1)) {
      arc_s1p2_TokenizerParseMulticharacterIdentifier(stage1, i, count);
      continue;
    }

    if (character1 == L'#') {
      arc_s1p2_TokenizerParseMulticharacterMacro(stage1, i, count);
      continue;
    }

    if (character1 == L'"') {
      arc_s1p2_TokenizerParseMulticharacterString(stage1, i, count);
      continue;
    }

    if (character1 == L'-' && character2 == L'-') {
      arc_s1p2_TokenizerParseDoubleOperator(stage1, i, count);
      continue;
    }

    if (arc_s1p2_CharacterIsKnownSymbol(character1)) {
      arc_s1p2_TokenizerParseSingleOperator(stage1, i, count);
      continue;
    }

    arc_s1p1_InfoPrintAdditionalInfo(__FUNCTION__, stage1, &i);
    arc_wprintf_fatalError(L"\n");
    arc_wprintf_fatalError(L"Fatal error: unexpected to the compiler symbol or character." L"\n");
    arc_wprintf_fatalError(L"\n");
    arc_s1p1_InfoPrintLinesAtCursorPosition(stage1, i);
    arc_s1p1_FatalErrorAdditionalCalls();
    exit(1);
  }
}

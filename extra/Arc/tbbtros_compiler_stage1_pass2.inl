static void internalTokenEatOneWideCharacterAndAdvanceCursorPositionByOne(TbbtrosToken & token, TbbtrosStage1 const & stage1, uint64_t & cursorPosition) {
  token.stringLength += 1;
  cursorPosition     += 1;
}

static TbbtrosBool8 internalCharacterIsNumber(wchar_t c) {
  if (
    c == L'0' || c == L'1' || c == L'2' || c == L'3' || c == L'4' || c == L'5' || c == L'6' || c == L'7' || c == L'8' || c == L'9'
  )
  {
    return 1;
  }
  return 0;
}

static TbbtrosBool8 internalCharacterIsAnyAsciiLetter(wchar_t c) {
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

static TbbtrosBool8 internalCharacterIsIdentifierBeginning(wchar_t c) {
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

static TbbtrosBool8 internalCharacterIsIdentifier(wchar_t c) {
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

static TbbtrosBool8 internalCharacterIsKnownSymbol(wchar_t c) {
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

static void internalSourceCodeAppendToken(TbbtrosStage1 & stage1, const TbbtrosToken & token) {
  if (token.stringLength == 0) {
    internalInfoPrintAdditionalInfo(__FUNCTIONW__, stage1, &token.stringOffset);
    internalFatalErrorWprintf(L"\n");
    internalFatalErrorWprintf(L"Fatal internal compiler error: encountered a token at offset %zu with its length unexpectedly equal to 0." L"\n", token.stringOffset);
    internalFatalErrorWprintf(L"\n");
    internalInfoPrintLinesAtCursorPosition(stage1, token.stringOffset);
    internalFatalErrorAdditionalCalls();
    exit(1);
  }
  stage1.tokenizer.tokens.push_back(token);
}

static std::wstring internalTokenToStringCanBeEmpty(TbbtrosStage1 const & stage1, const TbbtrosToken & token) {
  std::wstring tokenString;
  for (uint64_t i = token.stringOffset; i < (token.stringOffset + token.stringLength); i += 1) {
    tokenString += stage1.sourceCodeWithoutCommentsString[i];
  }
  return tokenString;
}

static std::wstring internalTokenToString(TbbtrosStage1 const & stage1, const TbbtrosToken & token) {
  if (token.stringLength == 0) {
    internalInfoPrintAdditionalInfo(__FUNCTIONW__, stage1, &token.stringOffset);
    internalFatalErrorWprintf(L"\n");
    internalFatalErrorWprintf(L"Fatal internal compiler error: encountered a token at offset %zu with its length unexpectedly equal to 0." L"\n", token.stringOffset);
    internalFatalErrorWprintf(L"\n");
    internalInfoPrintLinesAtCursorPosition(stage1, token.stringOffset);
    internalFatalErrorAdditionalCalls();
    exit(1);
  }
  std::wstring tokenString;
  for (uint64_t i = token.stringOffset; i < (token.stringOffset + token.stringLength); i += 1) {
    tokenString += stage1.sourceCodeWithoutCommentsString[i];
  }
  if (tokenString.empty()) {
    internalInfoPrintAdditionalInfo(__FUNCTIONW__, stage1, &token.stringOffset);
    internalFatalErrorWprintf(L"\n");
    internalFatalErrorWprintf(L"Fatal internal compiler error: encountered a token at offset %zu that unexpectedly converts to an empty string." L"\n", token.stringOffset);
    internalFatalErrorWprintf(L"\n");
    internalInfoPrintLinesAtCursorPosition(stage1, token.stringOffset);
    internalFatalErrorAdditionalCalls();
    exit(1);
  }
  return tokenString;
}

typedef struct np_0000_optionalPrintPerLineTokensCount {
  uint64_t v;
} np_0000_optionalPrintPerLineTokensCount;

static void internalDebugWprintfTokenizer(TbbtrosStage1 const & stage1, np_0000_optionalPrintPerLineTokensCount optionalPrintPerLineTokensCount) {
  internalDebugWprintf(L"[DEBUG][%s]\n", __FUNCTIONW__);
  internalDebugWprintf(L"std::wstring tokenizerTokens[] = {" L"\n");
  uint64_t tokensPerLineCounter = 0;
  for (uint64_t i = 0, tokensCount = stage1.tokenizer.tokens.size(); i < tokensCount; i += 1) {
    if (i != 0) {
      internalDebugWprintf(L", ");
    }
    std::wstring tokenString = internalTokenToString(stage1, stage1.tokenizer.tokens[i]);
    if (tokenString == L"\"") {
      internalDebugWprintf(L"L\"\\%s\"", tokenString.c_str());
    } else {
      internalDebugWprintf(L"L\"%s\"", tokenString.c_str());
    }
    tokensPerLineCounter += 1;
    if (optionalPrintPerLineTokensCount.v > 0) {
      if (tokensPerLineCounter >= optionalPrintPerLineTokensCount.v) {
        internalDebugWprintf(L"\n");
        tokensPerLineCounter = 0;
      }
    }
  }
  internalDebugWprintf(L"};\n");
}

static void internalTokenizerParseMulticharacterNumberLiteral(TbbtrosStage1 & stage1, uint64_t & i, uint64_t count) {
  TbbtrosToken token = {i, 0};
  for (; i < count;) {
    wchar_t character1 = internalPeekCharacter(stage1, i, 0);

    if (internalCharacterIsNumber(character1)) {
      internalTokenEatOneWideCharacterAndAdvanceCursorPositionByOne(token, stage1, i);
      continue;
    }
    if (character1 == L'-') { // NOTE(Constantine): Examples: -5
      internalTokenEatOneWideCharacterAndAdvanceCursorPositionByOne(token, stage1, i);
      continue;
    }
    if (character1 == L'.') { // NOTE(Constantine): Examples: .5, 0.01
      internalTokenEatOneWideCharacterAndAdvanceCursorPositionByOne(token, stage1, i);
      continue;
    }
    if (character1 == L'\'') { // NOTE(Constantine): Examples (C23): 0xFE'DC'BA'98, 299'792'458, 1.414'213'562
      internalTokenEatOneWideCharacterAndAdvanceCursorPositionByOne(token, stage1, i);
      continue;
    }
    if (internalCharacterIsAnyAsciiLetter(character1)) { // NOTE(Constantine): Examples: 0.5f, 42ULL, 0xcafebabe
      internalTokenEatOneWideCharacterAndAdvanceCursorPositionByOne(token, stage1, i);
      continue;
    }

    internalSourceCodeAppendToken(stage1, token); // NOTE(Constantine): Append the previously accumulated number token.
    break;
  }
}

static void internalTokenizerParseMulticharacterIdentifier(TbbtrosStage1 & stage1, uint64_t & i, uint64_t count, TbbtrosToken * optionalOutToken = NULL, TbbtrosBool8 optionalAppendParsedToken = 1) {
  TbbtrosToken token = {i, 0};
  for (; i < count;) {
    wchar_t character1 = internalPeekCharacter(stage1, i, 0);

    if (internalCharacterIsIdentifier(character1)) {
      internalTokenEatOneWideCharacterAndAdvanceCursorPositionByOne(token, stage1, i);
      continue;
    }

    if (optionalOutToken != NULL) {
      optionalOutToken[0] = token;
    }

    if (optionalAppendParsedToken == 1) {
      internalSourceCodeAppendToken(stage1, token); // NOTE(Constantine): Append the previously accumulated identifier token.
    }
    break;
  }
}

static void internalTokenizerParseMulticharacterMacroSkipSpaceCharacters(TbbtrosStage1 const & stage1, uint64_t & i, uint64_t count) {
  // NOTE(Constantine): Parsing possible space and new line characters.
  for (TbbtrosBool8 continueParsingToNextLine = 0; i < count;) {
    wchar_t character1 = internalPeekCharacter(stage1, i, 0);

    if (character1 == L' ') {
      i += 1; // Skipping character.
      continue;
    }
    if (character1 == L'\n' && continueParsingToNextLine == 0) {
      internalInfoPrintAdditionalInfo(__FUNCTIONW__, stage1, &i);
      internalFatalErrorWprintf(L"\n");
      internalFatalErrorWprintf(L"Fatal error: macro parsing encountered an unexpected terminating new line character." L"\n");
      internalFatalErrorWprintf(L"\n");
      internalInfoPrintLinesAtCursorPosition(stage1, i);
      internalFatalErrorAdditionalCalls();
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

static void internalTokenizerParseMulticharacterMacroSkipSpaceCharactersToTheTerminatingNewLineCharacterOtherwiseFatalError(TbbtrosStage1 const & stage1, uint64_t & i, uint64_t count) {
  // NOTE(Constantine): Parsing possible space and new line characters.
  for (TbbtrosBool8 continueParsingToNextLine = 0; i < count;) {
    wchar_t character1 = internalPeekCharacter(stage1, i, 0);

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

    internalInfoPrintAdditionalInfo(__FUNCTIONW__, stage1, &i);
    internalFatalErrorWprintf(L"\n");
    internalFatalErrorWprintf(L"Fatal error: macro parsing encountered a character when the macro was expected to finish instead." L"\n");
    internalFatalErrorWprintf(L"\n");
    internalInfoPrintLinesAtCursorPosition(stage1, i);
    internalFatalErrorAdditionalCalls();
    exit(1);
  }
}

static void internalCompilerCheckCharacterIsIdentifierBeginningOtherwiseFatalError(wchar_t character, wchar_t const * functionwName, TbbtrosStage1 const & stage1, uint64_t & i) {
  if (internalCharacterIsIdentifierBeginning(character) == 0) {
    internalInfoPrintAdditionalInfo(functionwName, stage1, &i);
    internalFatalErrorWprintf(L"\n");
    internalFatalErrorWprintf(L"Fatal error: encountered an unsupported character at the beginning of an identifier." L"\n");
    internalFatalErrorWprintf(L"\n");
    internalInfoPrintLinesAtCursorPosition(stage1, i);
    internalFatalErrorAdditionalCalls();
    exit(1);
  }
}

static void internalCheckCompilerSupportsMacroOtherwiseFatalError(TbbtrosStage1 const & stage1, uint64_t i, std::wstring tokenMacroStartString, std::wstring tokenMacroNameString, std::wstring tokenMacroIdentifierString) {
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
    internalInfoPrintAdditionalInfo(__FUNCTIONW__, stage1, &i);
    internalFatalErrorWprintf(L"\n");
    internalFatalErrorWprintf(L"Fatal error: encountered an unknown or unsupported macro." L"\n");
    internalFatalErrorWprintf(L"\n");
    internalInfoPrintLinesAtCursorPosition(stage1, i);
    internalFatalErrorAdditionalCalls();
    exit(1);
  }
}

static void internalTokenizerParseMulticharacterMacro(TbbtrosStage1 & stage1, uint64_t & i, uint64_t count) {
  // NOTE(Constantine): Parsing macro start hash character.
  {
    wchar_t character1 = internalPeekCharacter(stage1, i, 0);
    if (character1 != L'#') {
      internalInfoPrintAdditionalInfo(__FUNCTIONW__, stage1, &i);
      internalFatalErrorWprintf(L"\n");
      internalFatalErrorWprintf(L"Fatal error: macro is expected to start with a # character." L"\n");
      internalFatalErrorWprintf(L"\n");
      internalInfoPrintLinesAtCursorPosition(stage1, i);
      internalFatalErrorAdditionalCalls();
      exit(1);
    }
  }
  TbbtrosToken tokenMacroStart = {i, 0};
  internalTokenEatOneWideCharacterAndAdvanceCursorPositionByOne(tokenMacroStart, stage1, i);

  // NOTE(Constantine): Skip possible space and new line characters.
  internalTokenizerParseMulticharacterMacroSkipSpaceCharacters(stage1, i, count);

  // NOTE(Constantine): Parsing macro name.
  TbbtrosToken tokenMacroName = {i, 0};
  internalCompilerCheckCharacterIsIdentifierBeginningOtherwiseFatalError(internalPeekCharacter(stage1, i, 0), __FUNCTIONW__, stage1, i);
  internalTokenizerParseMulticharacterIdentifier(stage1, i, count, &tokenMacroName, 0);

  // NOTE(Constantine): Caching token strings.
  std::wstring tokenMacroStartString = internalTokenToString(stage1, tokenMacroStart);
  std::wstring tokenMacroNameString  = internalTokenToString(stage1, tokenMacroName);

  // NOTE(Constantine): Parsing macro identifier.
  TbbtrosToken tokenMacroIdentifier = {i, 0};
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
    internalTokenizerParseMulticharacterMacroSkipSpaceCharacters(stage1, i, count);
    
    // NOTE(Constantine): Advancing the initial offset after skipping possible space and new line characters above.
    tokenMacroIdentifier.stringOffset = i;
    
    // NOTE(Constantine): Parsing macro identifier.
    internalCompilerCheckCharacterIsIdentifierBeginningOtherwiseFatalError(internalPeekCharacter(stage1, i, 0), __FUNCTIONW__, stage1, i);
    internalTokenizerParseMulticharacterIdentifier(stage1, i, count, &tokenMacroIdentifier, 0);
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
    internalTokenizerParseMulticharacterMacroSkipSpaceCharacters(stage1, i, count);

    // NOTE(Constantine): Advancing the initial offset after skipping possible space and new line characters above.
    tokenMacroIdentifier.stringOffset = i;

    // NOTE(Constantine):
    // Intentionally not calling internalCompilerCheckCharacterIsIdentifierBeginningOtherwiseFatalError
    // since the identifier can be a number such as 0 or 1.
    internalTokenizerParseMulticharacterIdentifier(stage1, i, count, &tokenMacroIdentifier, 0);

    // NOTE(Constantine):
    // Skipping possible space characters to the terminating new line character,
    // otherwise it's not of one of the supported conditional macros, so have to
    // exit the compiler with an error instead. One identifier per conditional
    // macro is a current limitation of the compiler.
    internalTokenizerParseMulticharacterMacroSkipSpaceCharactersToTheTerminatingNewLineCharacterOtherwiseFatalError(stage1, i, count);
  }

  // NOTE(Constantine): Caching token strings.
  std::wstring tokenMacroIdentifierString = internalTokenToStringCanBeEmpty(stage1, tokenMacroIdentifier);

  internalCheckCompilerSupportsMacroOtherwiseFatalError(stage1, tokenMacroStart.stringOffset, tokenMacroStartString, tokenMacroNameString, tokenMacroIdentifierString);

  // NOTE(Constantine): Skipping everything else up to and including the new line character, with \ in mind.
  for (TbbtrosBool8 continueParsingToNextLine = 0; i < count;) {
    wchar_t character1 = internalPeekCharacter(stage1, i, 0);

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
    internalSourceCodeAppendToken(stage1, tokenMacroStart);
    internalSourceCodeAppendToken(stage1, tokenMacroName);
    if (tokenMacroIdentifier.stringLength > 0) {
      internalSourceCodeAppendToken(stage1, tokenMacroIdentifier);
    }
  }
}

static void internalTokenizerParseMulticharacterString(TbbtrosStage1 & stage1, uint64_t & i, uint64_t count, TbbtrosBool8 optionalAppendParsedTokens = 1) {
  // NOTE(Constantine): Parsing open string literal character token
  {
    wchar_t character1 = internalPeekCharacter(stage1, i, 0);
    if (character1 != L'\"') {
      internalInfoPrintAdditionalInfo(__FUNCTIONW__, stage1, &i);
      internalFatalErrorWprintf(L"\n");
      internalFatalErrorWprintf(L"Fatal error: multicharacter string literal parsing expected to start with a \" character." L"\n");
      internalFatalErrorWprintf(L"\n");
      internalInfoPrintLinesAtCursorPosition(stage1, i);
      internalFatalErrorAdditionalCalls();
      exit(1);
    }
  }
  TbbtrosToken tokenOpenStringLiteralCharacter = {i, 0};
  internalTokenEatOneWideCharacterAndAdvanceCursorPositionByOne(tokenOpenStringLiteralCharacter, stage1, i);

  // NOTE(Constantine): Parsing everything up to but not including ", with \ in mind.
  TbbtrosToken tokenStringLiteral = {i, 0};
  for (; i < count;) {
    wchar_t character1 = internalPeekCharacter(stage1, i, 0);
    wchar_t character2 = internalPeekCharacter(stage1, i, 1);

    if (character1 == L'\\') {
      internalTokenEatOneWideCharacterAndAdvanceCursorPositionByOne(tokenStringLiteral, stage1, i); // NOTE(Constantine): The token string will contain \ character.
      internalTokenEatOneWideCharacterAndAdvanceCursorPositionByOne(tokenStringLiteral, stage1, i);
      continue;
    }

    if (character1 == L'"') {
      break;
    }

    if (character1 == 0 || character1 == L'\n') {
      internalInfoPrintAdditionalInfo(__FUNCTIONW__, stage1, &i);
      internalFatalErrorWprintf(L"\n");
      internalFatalErrorWprintf(L"Fatal error: encountered end of file or a new line terminating character when an opened string literal was not closed yet." L"\n");
      internalFatalErrorWprintf(L"\n");
      internalInfoPrintLinesAtCursorPosition(stage1, i);
      internalFatalErrorAdditionalCalls();
      exit(1);
    }

    internalTokenEatOneWideCharacterAndAdvanceCursorPositionByOne(tokenStringLiteral, stage1, i);
  }

  // NOTE(Constantine): Parsing close string literal character token.
  {
    wchar_t character1 = internalPeekCharacter(stage1, i, 0);
    if (character1 != L'\"') {
      internalInfoPrintAdditionalInfo(__FUNCTIONW__, stage1, &i);
      internalFatalErrorWprintf(L"\n");
      internalFatalErrorWprintf(L"Fatal error: multicharacter string literal parsing expected to close with a \" character." L"\n");
      internalFatalErrorWprintf(L"\n");
      internalInfoPrintLinesAtCursorPosition(stage1, i);
      internalFatalErrorAdditionalCalls();
      exit(1);
    }
  }
  TbbtrosToken tokenCloseStringLiteralCharacter = {i, 0};
  internalTokenEatOneWideCharacterAndAdvanceCursorPositionByOne(tokenCloseStringLiteralCharacter, stage1, i);

  if (optionalAppendParsedTokens == 1) {
    internalSourceCodeAppendToken(stage1, tokenOpenStringLiteralCharacter);
    internalSourceCodeAppendToken(stage1, tokenStringLiteral);
    internalSourceCodeAppendToken(stage1, tokenCloseStringLiteralCharacter);
  }
}

static void internalTokenizerParseSingleOperator(TbbtrosStage1 & stage1, uint64_t & i, uint64_t count) {
  TbbtrosToken token = {i, 0};

  wchar_t character1 = internalPeekCharacter(stage1, i, 0);

  if (internalCharacterIsKnownSymbol(character1)) {
    internalTokenEatOneWideCharacterAndAdvanceCursorPositionByOne(token, stage1, i);
    internalSourceCodeAppendToken(stage1, token);
    return;
  }
}

static void internalTokenizerParseDoubleOperator(TbbtrosStage1 & stage1, uint64_t & i, uint64_t count) {
  TbbtrosToken token = {i, 0};

  wchar_t character1 = internalPeekCharacter(stage1, i, 0);
  wchar_t character2 = internalPeekCharacter(stage1, i, 1);

  if (internalCharacterIsKnownSymbol(character1) && internalCharacterIsKnownSymbol(character2)) {
    internalTokenEatOneWideCharacterAndAdvanceCursorPositionByOne(token, stage1, i);
    internalTokenEatOneWideCharacterAndAdvanceCursorPositionByOne(token, stage1, i);
    internalSourceCodeAppendToken(stage1, token);
    return;
  }
}

static void internalStage1Pass2SourceCodeFillTokenizerStruct(TbbtrosStage1 & stage1) {
  for (uint64_t i = 0, count = stage1.sourceCodeWithoutCommentsString.size(); i < count;) {
    wchar_t character1 = internalPeekCharacter(stage1, i, 0);
    wchar_t character2 = internalPeekCharacter(stage1, i, 1);

    if (character1 == L' ' || character1 == L'\n' || character1 == L'\t') { // NOTE(Constantine): Thanks to @kiwec for spotting the missing tab '\t' character handling in gen22! :)
      i += 1; // Skipping character.
      continue;
    }

    if (character1 == L'.' && internalCharacterIsNumber(character2)) { // NOTE(Constantine): Example: .5
      internalTokenizerParseMulticharacterNumberLiteral(stage1, i, count);
      continue;
    }

    if (internalCharacterIsNumber(character1)) {
      internalTokenizerParseMulticharacterNumberLiteral(stage1, i, count);
      continue;
    }

    if (internalCharacterIsIdentifierBeginning(character1)) {
      internalTokenizerParseMulticharacterIdentifier(stage1, i, count);
      continue;
    }

    if (character1 == L'#') {
      internalTokenizerParseMulticharacterMacro(stage1, i, count);
      continue;
    }

    if (character1 == L'"') {
      internalTokenizerParseMulticharacterString(stage1, i, count);
      continue;
    }

    if (character1 == L'-' && character2 == L'-') {
      internalTokenizerParseDoubleOperator(stage1, i, count);
      continue;
    }

    if (internalCharacterIsKnownSymbol(character1)) {
      internalTokenizerParseSingleOperator(stage1, i, count);
      continue;
    }

    internalInfoPrintAdditionalInfo(__FUNCTIONW__, stage1, &i);
    internalFatalErrorWprintf(L"\n");
    internalFatalErrorWprintf(L"Fatal error: unexpected to the compiler symbol or character." L"\n");
    internalFatalErrorWprintf(L"\n");
    internalInfoPrintLinesAtCursorPosition(stage1, i);
    internalFatalErrorAdditionalCalls();
    exit(1);
  }
}

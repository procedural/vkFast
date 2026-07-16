#include "tbbtros_compiler_common.h"

#include "tbbtros_compiler_stage1_pass1.inl" // NOTE(Constantine): Pass 1. Comments remover.
#include "tbbtros_compiler_stage1_pass2.inl" // NOTE(Constantine): Pass 2. Fills TbbtrosStage1::tokenizer struct.
#include "tbbtros_compiler_stage1_pass3.inl" // NOTE(Constantine): Pass 3. Checks conditional macro tokens for correct syntax.
#include "tbbtros_compiler_stage1_pass4.inl" // NOTE(Constantine): Pass 4. Eliminates false code paths of conditional macro tokens.
#include "tbbtros_compiler_stage1_pass5.inl" // NOTE(Constantine): Pass 5. Prints and removes encountered '#pragma message' and '#error' macro tokens.

void internalCompilerStage1(TbbtrosCompilerState * state, int ArgsCount, wchar_t const * const * Args) {
  TbbtrosStage1 * const stage1 = &state->stage1;
  
  internalStaticAssert(
    "Fatal internal compiler error: invalid expected output from the internal FNV1a64 hash function." &&
    internalHashFNV1a64(sizeof("foobar")-1, "foobar") == 0x85944171f73967e8
  );

  setlocale(LC_ALL, "en_US.UTF-8");
#if internalTargetOs == internalTargetOsWindows
  SetConsoleOutputCP(65001);
#endif

  for (int i = 0; i < ArgsCount; i += 1) {
    stage1->wmainArguments.arguments.push_back(std::wstring(Args[i]));
  }

#ifdef TBBTROS_STAGE1_INTERNAL_COMPILER_TESTS
  {
    stage1->wmainArguments.arguments.push_back(L"C:/Users/Constantine/Desktop/main.tbbtros");
  }
#endif

  internalProcessWmainArguments(stage1[0]);

#ifdef TBBTROS_STAGE1_INTERNAL_COMPILER_DEBUG
  {
    internalInfoPrintAdditionalInfo(__FUNCTIONW__, stage1[0], &stage1->wmainArgumentsParameters.debugPrintSourceCodeUpToAndIncludingCursorPosition);
    internalInfoWprintf(L"\n");
  }
#endif

  if (stage1->wmainArgumentsParameters.debugPrintSourceCodeUpToAndIncludingCursorPositionIsRequested == 1)
  {
    uint64_t i = 0;
    for (; i <= stage1->wmainArgumentsParameters.debugPrintSourceCodeUpToAndIncludingCursorPosition; i += 1) {
      internalCommandLineInterfaceWprintf(L"%c", stage1->sourceCodeWithoutCommentsString[i]);
    }
    i -= 1; // NOTE(Constantine): At its end, the for loop increments i, then fails the check by being bigger by 1 than the compared value, then exits the loop, so we need to decrement it back here.
    internalCommandLineInterfaceWprintf(L"\n");
    uint64_t column = internalGetLineColumnFromCursorPosition(stage1[0], i);
    for (uint64_t j = 0; j < (column - 1); j += 1) {
      internalInfoWprintf(L" ");
    }
    internalInfoWprintf(L"^" L"\n");
    exit(0);
  }

  internalStage1Pass1SourceCodeReplaceCommentsWithSpaceCharacters(stage1[0]);
  internalStage1Pass2SourceCodeFillTokenizerStruct(stage1[0]);
  internalStage1Pass3ConditionalMacroTokenSyntaxChecks(stage1[0]);
  internalStage1Pass4ConditionalMacroTokenFalseCodePathsElimination(stage1[0]);
  internalStage1Pass5PrintPragmaMessageAndErrorMacros(stage1[0]);

#ifdef TBBTROS_STAGE1_INTERNAL_COMPILER_DEBUG
  internalDebugWprintfTokenizer(stage1[0], np_0000_optionalPrintPerLineTokensCount{1});
  internalDebugSaveTokensAsSourceCodeString(stage1[0], np_0001_saveToFilepath{L"stage1_debug_print_tokens_as_code.tbbtros"});
#endif
}

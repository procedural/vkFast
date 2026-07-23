#include "arc.h"

#include "arc_stage1/arc_stage1_pass1.inl" // NOTE(Constantine): Pass 1. Comments remover.
#include "arc_stage1/arc_stage1_pass2.inl" // NOTE(Constantine): Pass 2. Fills ArcStateStage1::tokenizer struct.
#include "arc_stage1/arc_stage1_pass3.inl" // NOTE(Constantine): Pass 3. Checks conditional macro tokens for correct syntax.
#include "arc_stage1/arc_stage1_pass4.inl" // NOTE(Constantine): Pass 4. Eliminates false code paths of conditional macro tokens.
#include "arc_stage1/arc_stage1_pass5.inl" // NOTE(Constantine): Pass 5. Prints and removes encountered '#pragma message' and '#error' macro tokens.

void arcStage1(ArcState * state, int ArgsCount, wchar_t * const * const Args) {
  ArcStateStage1 * const stage1 = &state->stage1;
  
  arc_static_assert(
    "Fatal internal compiler error: invalid expected output from the internal FNV1a64 hash function." &&
    arc_s1p1_HashFNV1a64(sizeof("foobar")-1, "foobar") == 0x85944171f73967e8
  );

  for (int i = 0; i < ArgsCount; i += 1) {
    stage1->wmainArguments.arguments.push_back(std::wstring(Args[i]));
  }

#ifdef ARC_INTERNAL_COMPILER_TESTS
  {
    #if ARC_COMPILER_OS == ARC_COMPILER_OS_LINUX
    stage1->wmainArguments.arguments.push_back(L"/home/constantine/Desktop/main.arc");
    #else
    #error
    #endif
  }
#endif

  arc_s1p1_ProcessWmainArguments(stage1[0]);

#ifdef ARC_INTERNAL_COMPILER_DEBUG
  {
    arc_s1p1_InfoPrintAdditionalInfo(__FUNCTION__, stage1[0], &stage1->wmainArgumentsParameters.debugPrintSourceCodeUpToAndIncludingCursorPosition);
    arc_wprintf_info(L"\n");
  }
#endif

  if (stage1->wmainArgumentsParameters.debugPrintSourceCodeUpToAndIncludingCursorPositionIsRequested == 1)
  {
    uint64_t i = 0;
    for (; i <= stage1->wmainArgumentsParameters.debugPrintSourceCodeUpToAndIncludingCursorPosition; i += 1) {
      arc_wprintf_cli(L"%lc", stage1->sourceCodeWithoutCommentsString[i]);
    }
    i -= 1; // NOTE(Constantine): At its end, the for loop increments i, then fails the check by being bigger by 1 than the compared value, then exits the loop, so we need to decrement it back here.
    arc_wprintf_cli(L"\n");
    uint64_t column = arc_s1p1_GetLineColumnFromCursorPosition(stage1[0], i);
    for (uint64_t j = 0; j < (column - 1); j += 1) {
      arc_wprintf_info(L" ");
    }
    arc_wprintf_info(L"^" L"\n");
    exit(0);
  }

  arc_s1p1_Stage1Pass1SourceCodeReplaceCommentsWithSpaceCharacters(stage1[0]);
  arc_s1p2_Stage1Pass2SourceCodeFillTokenizerStruct(stage1[0]);
  if (stage1->wmainArgumentsParameters.rawbuildIsEnabled == 0) {
    arc_s1p3_Stage1Pass3ConditionalMacroTokenSyntaxChecks(stage1[0]);
    arc_s1p4_Stage1Pass4ConditionalMacroTokenFalseCodePathsElimination(stage1[0]);
    arc_s1p5_Stage1Pass5PrintPragmaMessageAndErrorMacros(stage1[0]);
  }

#ifdef ARC_INTERNAL_COMPILER_DEBUG
  arc_s1p2_WprintfDebugTokenizer(stage1[0], arc_np_0000_optionalPrintPerLineTokensCount{1});
  arc_s1p5_DebugSaveTokensAsSourceCodeString(stage1[0], arc_np_0001_saveToFilepath{L"arc_stage1_debug_print_tokens_as_code.h"});
#endif
}

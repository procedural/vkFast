typedef struct ArcWmainArguments {
  std::vector<std::wstring> arguments;
} ArcWmainArguments;

typedef struct ArcWmainArgumentsParameters {
  ArcBool8 debugPrintSourceCodeUpToAndIncludingCursorPositionIsRequested;
  uint64_t debugPrintSourceCodeUpToAndIncludingCursorPosition;
} ArcWmainArgumentsParameters;

typedef struct ArcCompilerCommandDefinedMacros {
  std::vector<std::wstring> macrosName;
  std::vector<int>          macrosValue;
  std::vector<ArcBool8>     macrosIsDefinedByCli; // cli: command line interface
} ArcCompilerCommandDefinedMacros;

typedef struct ArcToken {
  uint64_t stringOffset; // Cursor position offset into ArcSourceCode::string.
  uint64_t stringLength; // Token's string length.
} ArcToken;

typedef struct ArcTokenizer {
  std::vector<ArcToken> tokens;
} ArcTokenizer;

typedef struct ArcStateStage1 {
  std::wstring                    sourceCodeWithoutCommentsString;
  std::vector<std::wstring>       filesPath;
  std::vector<uint64_t>           filesSize;
  std::vector<std::wstring>       filesOriginalSourceCodeString;
  ArcWmainArguments               wmainArguments;
  ArcWmainArgumentsParameters     wmainArgumentsParameters;
  ArcCompilerCommandDefinedMacros compilerCommandDefinedMacros;
  ArcTokenizer                    tokenizer;
} ArcStateStage1;

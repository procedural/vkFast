typedef struct TbbtrosWmainArguments {
  std::vector<std::wstring> arguments;
} TbbtrosWmainArguments;

typedef struct TbbtrosWmainArgumentsParameters {
  TbbtrosBool8 debugPrintSourceCodeUpToAndIncludingCursorPositionIsRequested;
  uint64_t     debugPrintSourceCodeUpToAndIncludingCursorPosition;
} TbbtrosWmainArgumentsParameters;

typedef struct TbbtrosCompilerCommandDefinedMacros {
  std::vector<std::wstring> macrosName;
  std::vector<int>          macrosValue;
  std::vector<TbbtrosBool8> macrosIsDefinedByCli; // cli: command line interface
} TbbtrosCompilerCommandDefinedMacros;

typedef struct TbbtrosToken {
  uint64_t stringOffset; // Cursor position offset into TbbtrosSourceCode::string.
  uint64_t stringLength; // Token's string length.
} TbbtrosToken;

typedef struct TbbtrosTokenizer {
  std::vector<TbbtrosToken> tokens;
} TbbtrosTokenizer;

typedef struct TbbtrosStage1 {
  std::wstring                        sourceCodeWithoutCommentsString;
  std::vector<std::wstring>           filesPath;
  std::vector<uint64_t>               filesSize;
  std::vector<std::wstring>           filesOriginalSourceCodeString;
  TbbtrosWmainArguments               wmainArguments;
  TbbtrosWmainArgumentsParameters     wmainArgumentsParameters;
  TbbtrosCompilerCommandDefinedMacros compilerCommandDefinedMacros;
  TbbtrosTokenizer                    tokenizer;
} TbbtrosStage1;

wchar_t const * g_arc_compilerLicense = L""
L"\n"
L"The source code for \"Arc\" compiler." L"\n"
L"Copyright(C) 2024-2026 Constantine Tarasenkov (iamvfx@gmail.com). All rights reserved." L"\n"
L"\n"
L"Licensed under the Apache License, Version 2.0 (the \"License\");" L"\n"
L"you may not use this file except in compliance with the License." L"\n"
L"You may obtain a copy of the License at" L"\n"
L"\n"
L"    http://www.apache.org/licenses/LICENSE-2.0" L"\n"
L"\n"
L"Unless required by applicable law or agreed to in writing, software" L"\n"
L"distributed under the License is distributed on an \"AS IS\" BASIS," L"\n"
L"WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied." L"\n"
L"See the License for the specific language governing permissions and" L"\n"
L"limitations under the License." L"\n"
;

#include "tbbtros_compiler_common.h"

int wmain(int ArgsCount, wchar_t const * const * Args) {
  TbbtrosCompilerState state = {};
  internalCompilerStage1(&state, ArgsCount, Args);
  internalCompilerStage2(&state);
}

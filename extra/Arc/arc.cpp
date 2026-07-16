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

#include "arc.h"

int wmain(int ArgsCount, wchar_t * const * const Args) {
  #if ARC_COMPILER_OS == ARC_COMPILER_OS_WINDOWS
  setlocale(LC_ALL, "en_US.UTF-8");
  SetConsoleOutputCP(65001);
  #endif

  ArcState state = {};
  arcStage1(&state, ArgsCount, Args);
  arcStage2(&state);

  return 0;
}

#if ARC_COMPILER_OS != ARC_COMPILER_OS_WINDOWS
int main(int ArgsCount, char * const * const Args) {
  setlocale(LC_ALL, "en_US.UTF-8");

  // Allocate array for wide characters
  wchar_t ** Argsw = (wchar_t **)malloc(ArgsCount * sizeof(wchar_t *));

  for (int i = 0; i < ArgsCount; i += 1) {
    // Calculate size needed
    size_t size = mbstowcs(NULL, Args[i], 0) + 1;
    Argsw[i] = (wchar_t *)calloc(1, size * sizeof(wchar_t));
    // Convert UTF-8 char * to wchar_t *
    mbstowcs(Argsw[i], Args[i], size);
  }

  int result = wmain(ArgsCount, Argsw);

  // Free allocated memory
  for (int i = 0; i < ArgsCount; i += 1) {
    free(Argsw[i]);
  }
  free(Argsw);

  return result;
}
#endif

#include "arc_stage1.inl"
#include "arc_stage2.inl"

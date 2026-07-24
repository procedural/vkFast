#include "arc.h"

void arcRawbuild(ArcState * state) {
  ArcStateStage1 * const stage1 = &state->stage1;

  const ArcBool8 arcVerboseIsEnabled = stage1->wmainArgumentsParameters.verboseIsEnabled; // Needed for arc_wprintf_verbose

  if (stage1->wmainArgumentsParameters.rawbuildIsEnabled == 0) {
    arc_wprintf_verbose(L"[--verbose][arc_rawbuild.inl] Arc rawbuild is not enabled, skipping arcRawbuild() stage." "\n");
    return;
  }

  {
    arc_wprintf_verbose(L"[--verbose][arc_rawbuild.inl] stage1->filesPath array printing:" "\n");
    for (uint64_t i = 0, count = stage1->filesPath.size(); i < count; i += 1) {
      arc_wprintf_verbose(L"[--verbose][arc_rawbuild.inl] [%d]: \"%ls\"" "\n", i, stage1->filesPath[i].c_str());
    }
    arc_wprintf_verbose(L"[--verbose][arc_rawbuild.inl] stage1->filesSize array printing:" "\n");
    for (uint64_t i = 0, count = stage1->filesSize.size(); i < count; i += 1) {
      arc_wprintf_verbose(L"[--verbose][arc_rawbuild.inl] [%d]: %zu" "\n", i, stage1->filesSize[i]);
    }
  }
}

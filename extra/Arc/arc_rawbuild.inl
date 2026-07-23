#include "arc.h"

void arcRawbuild(ArcState * state) {
  ArcStateStage1 * const stage1 = &state->stage1;

  if (stage1->wmainArgumentsParameters.verboseIsEnabled == 1) {
    arc_wprintf_info(L"[--verbose][arc_rawbuild.inl] stage1->filesPath array printing:" "\n");
    for (uint64_t i = 0, count = stage1->filesPath.size(); i < count; i += 1) {
      arc_wprintf_info(L"[--verbose][arc_rawbuild.inl] [%d]: \"%ls\"" "\n", i, stage1->filesPath[i].c_str());
    }
    arc_wprintf_info(L"[--verbose][arc_rawbuild.inl] stage1->filesSize array printing:" "\n");
    for (uint64_t i = 0, count = stage1->filesSize.size(); i < count; i += 1) {
      arc_wprintf_info(L"[--verbose][arc_rawbuild.inl] [%d]: %zu" "\n", i, stage1->filesSize[i]);
    }
  }
}

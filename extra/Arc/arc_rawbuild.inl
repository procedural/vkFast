#include "arc.h"

void arcRawbuild(ArcState * state) {
  ArcStateStage1 * const stage1 = &state->stage1;

  const ArcBool8 arcVerboseIsEnabled = stage1->wmainArgumentsParameters.verboseIsEnabled; // Needed for arc_wprintf_verbose

  if (stage1->wmainArgumentsParameters.rawbuildIsEnabled == 0) {
    arc_wprintf_verbose(L"[--verbose][arc_rawbuild.inl] Arc rawbuild is not enabled, skipping arcRawbuild() stage." "\n");
    return;
  }

  // Checks
  {
    ArcBool8 pathCanNotBeRead   = 0;
    ArcBool8 folderWasRequested = 0;
    {
      arc_wprintf_verbose(L"[--verbose][arc_rawbuild.inl] stage1->filesPath array printing:" "\n");
      for (uint64_t i = 0, count = stage1->filesPath.size(); i < count; i += 1) {
        arc_wprintf_verbose(L"[--verbose][arc_rawbuild.inl] [%d]: \"%ls\"" "\n", i, stage1->filesPath[i].c_str());
      }
      arc_wprintf_verbose(L"[--verbose][arc_rawbuild.inl] stage1->filesSize array printing:" "\n");
      for (uint64_t i = 0, count = stage1->filesSize.size(); i < count; i += 1) {
        arc_wprintf_verbose(L"[--verbose][arc_rawbuild.inl] [%d]: %zu" "\n", i, stage1->filesSize[i]);
      }
      arc_wprintf_verbose(L"[--verbose][arc_rawbuild.inl] stage1->filesPath is file (0) or folder (1) or other/error/notfound (-1):" "\n");
      for (uint64_t i = 0, count = stage1->filesPath.size(); i < count; i += 1) {
        // To free
        const char * const cpath = arcCommonMallocWcharToChar(stage1->filesPath[i].c_str());
        int type = arcCommonGetSystemPathType(cpath);
        arc_wprintf_verbose(L"[--verbose][arc_rawbuild.inl] [%d]: %d" "\n", i, type);
        free((void *)cpath);

        if (type == -1) {
          pathCanNotBeRead = 1;
        }
        if (type == 1) {
          folderWasRequested = 1;
        }
      }
    }

    if (pathCanNotBeRead == 1) {
      arc_wprintf_fatalError(L"\n");
      arc_wprintf_fatalError(L"[rawbuild] Fatal error: a path that was provided is not a file or a folder. See --verbose log for details." "\n");
      arc_wprintf_fatalError(L"\n");
      arc_s1p1_FatalErrorAdditionalCalls();
      exit(1);
    }

    if (folderWasRequested == 1 && state->rawbuild.lookInFoldersForFileExtensions.empty()) {
      arc_wprintf_fatalError(L"\n");
      arc_wprintf_fatalError(L"[rawbuild] Fatal error: a folder search was requested, but no file extensions to look for were provided. See --verbose log for details." "\n");
      arc_wprintf_fatalError(L"\n");
      arc_s1p1_FatalErrorAdditionalCalls();
      exit(1);
    }
  }
}

#pragma once

#include "arc.h"

#include "arc_cross_stage/HashFNV1a64.h"

// NOTE(Constantine)(Jul 25, 2026):
// All functions are 'static inline' here in this file,
// because they can be used across all stages multiple times.

static inline void arcCrossStageInitialChecks() {
  arc_static_assert(
    "Fatal internal compiler error: invalid expected output from the internal FNV1a64 hash function." &&
    arc_xs_HashFNV1a64(sizeof("foobar")-1, "foobar") == 0x85944171f73967e8
  );
}

#if ARC_INTERNAL_CPP_VERSION >= ARC_INTERNAL_CPP_VERSION_17
constexpr
#endif
static uint64_t arc_xs_HashFNV1a64(uint64_t bytesCount, const char * const bytes) {
  uint64_t hash = 0xcbf29ce484222325;
  for (uint64_t i = 0; i < bytesCount; i += 1) {
    hash = (hash ^ (uint64_t)((uint8_t)(bytes[i]))) * 0x100000001b3;
  }
  return hash;
}

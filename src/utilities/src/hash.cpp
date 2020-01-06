#include <utilities/hash.hpp>

std::size_t CombineHashes(std::size_t hash1, std::size_t hash2) {
  // Same as boost's 32-bit `hash_combine` implementation, but with a 64-bit
  // magic number.
  return hash1 ^ (hash2 + 0x9e3779b97f4a7c15 + (hash1 << 6) + (hash1 >> 2));
}

std::size_t Hash(int64_t x) {
  // Snippet from the MurmurHash3 hash function.
  x ^= x >> 33;
  x *= 0xff51afd7ed558ccd;
  x ^= x >> 33;
  x *= 0xc4ceb9fe1a85ec53;
  x ^= x >> 33;
  return x;
}

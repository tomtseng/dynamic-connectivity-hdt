#pragma once

#include <cstdint>
#include <utility>

std::size_t CombineHashes(std::size_t h1, std::size_t h2);

// `std::hash<int64_t>{}()` is usually the identity function. This hash function
// is quick but is not the identity function.
std::size_t Hash(int64_t x);

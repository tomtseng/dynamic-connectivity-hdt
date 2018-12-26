#pragma once

#include <utility>

std::size_t CombineHashes(std::size_t h1, std::size_t h2);
// `std::hash<std::size_t>{}()` is usually the identity function. This hash
// function is quick but is not the identity function.
std::size_t HashInt(std::size_t x);

// For use in hash containers. For instance:
//   std::unordered_map<
//     std::pair<std::size_t, std::size_t>, std::string, IntPairHasher>
//     int_to_string_map;
//
// This hash struct was written because it was found that boost's integer pair
// hash `boost::hash<pair<., .>>` gives a low range of values and did not
// perform as well as expected in hash containers where the keys had small
// integer values.
struct IntPairHasher {
  std::size_t operator()(const std::pair<std::size_t, std::size_t>& p) const;
};

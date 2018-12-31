// Utilities that allow printing of standard C++ classes.
#pragma once

#include <ostream>
#include <utility>
#include <vector>

template <typename T1, typename T2>
std::ostream& operator<<(std::ostream& out, const std::pair<T1, T2> p) {
  out << '(' << p.first << ", " << p.second << ')';
  return out;
}

template <typename T>
std::ostream& operator<<(std::ostream& out, const std::vector<T> v) {
  out << '(';
  for (auto elem : v) {
    out << elem << ", ";
  }
  out << ')';
  return out;
}

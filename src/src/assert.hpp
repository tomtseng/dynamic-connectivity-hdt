#pragma once

#ifdef DEBUG
#include <cstdlib>
#include <iostream>
#include <sstream>

// If boolean `cond` is false, prints `msg` to stderr and terminates the
// program.
#define ASSERT_MSG(cond, msg) \
do { \
  if (!(cond)) { \
    std::ostringstream str; \
    str << msg; \
    std::cerr << str.str(); \
    std::abort(); \
  } \
} while (false)
#else
#define ASSERT_WITH_MSG(cond, msg) do {} while (false)
#endif  // ifdef DEBUG

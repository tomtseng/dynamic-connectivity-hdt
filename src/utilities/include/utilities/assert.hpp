#pragma once

#include <cstdlib>
#include <iostream>
#include <sstream>

#include <boost/stacktrace.hpp>

// If boolean `cond` is false, prints `msg` to stderr and terminates the
// program.
#define ASSERT_MSG_ALWAYS(cond, msg) \
do { \
  if (!(cond)) { \
    std::ostringstream str; \
    str << msg; \
    std::cerr << "ASSERTION FAILED: " << str.str() << '\n' \
      << "Stack trace:\n" \
      << boost::stacktrace::stacktrace() << '\n'; \
    std::abort(); \
  } \
} while (false)

#ifndef NDEBUG
#define ASSERT_MSG(cond, msg) ASSERT_MSG_ALWAYS(cond, msg)
#else
#define ASSERT_MSG(cond, msg) do {} while (false)
#endif  // ifndef NDEBUG

add_library(lib_assert STATIC
  src/assert.cpp
)
target_include_directories(lib_assert PRIVATE
  include
)
target_link_libraries(lib_assert PRIVATE
  # These two are for boost to print stack traces.
  backtrace
  dl
)

add_library(lib_hash STATIC
  src/hash.cpp
)
target_include_directories(lib_hash PRIVATE
  include
)

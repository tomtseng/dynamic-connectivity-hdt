add_library(lib_hash STATIC
  src/hash.cpp
)
target_include_directories(lib_hash PRIVATE
  include
)

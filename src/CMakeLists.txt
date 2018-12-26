include_directories(include)
include_directories(src)

add_library(lib_sequence STATIC
  src/sequence.cpp
)

add_subdirectory(test)

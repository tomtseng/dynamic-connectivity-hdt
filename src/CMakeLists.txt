include_directories(include)

add_library(lib_sequence STATIC
  src/sequence.cpp
)

add_subdirectory(test)

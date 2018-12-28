add_library(lib_sequence STATIC
  src/sequence.cpp
)
target_include_directories(lib_sequence PRIVATE
  include
  ${CMAKE_SOURCE_DIR}/src/utilities/include
)

add_subdirectory(test)

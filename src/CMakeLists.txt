include_directories(include)
include_directories(src)

add_library(lib_dynamic_trees STATIC
  src/dynamic_trees.cpp
)
target_link_libraries(lib_dynamic_trees
  lib_sequence
  lib_utilities
)
add_library(lib_sequence STATIC
  src/sequence.cpp
)
add_library(lib_utilities STATIC
  src/utilities.cpp
)

add_subdirectory(test)

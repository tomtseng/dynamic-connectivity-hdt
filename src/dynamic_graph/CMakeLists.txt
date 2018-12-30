add_library(lib_dynamic_connectivity STATIC
  src/dynamic_connectivity.cpp
)
target_link_libraries(lib_dynamic_connectivity
  lib_dynamic_forest
)
target_include_directories(lib_dynamic_connectivity PRIVATE
  include
)
target_include_directories(lib_dynamic_connectivity PUBLIC
  src
)

add_library(lib_dynamic_forest STATIC
  src/dynamic_forest.cpp
)
target_link_libraries(lib_dynamic_forest
  lib_hash
  lib_sequence
)
target_include_directories(lib_dynamic_forest PUBLIC
  src
  ${CMAKE_SOURCE_DIR}/src/utilities/include
)

add_library(lib_sequence STATIC
  src/sequence.cpp
)
target_include_directories(lib_sequence PRIVATE
  src
  ${CMAKE_SOURCE_DIR}/src/utilities/include
)

add_subdirectory(test)

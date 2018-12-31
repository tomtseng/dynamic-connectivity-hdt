add_library(lib_dynamic_connectivity STATIC
  src/dynamic_connectivity.cpp
)
target_link_libraries(lib_dynamic_connectivity
  lib_assert
  lib_dynamic_forest
  lib_graph
  lib_hash
)
target_include_directories(lib_dynamic_connectivity PUBLIC
  ${CMAKE_SOURCE_DIR}/src/utilities/include
  include
  src
)

add_library(lib_dynamic_forest STATIC
  src/dynamic_forest.cpp
)
target_link_libraries(lib_dynamic_forest
  lib_assert
  lib_graph
  lib_sequence
)
target_include_directories(lib_dynamic_forest PRIVATE
  ${CMAKE_SOURCE_DIR}/src/utilities/include
)
target_include_directories(lib_dynamic_forest PUBLIC
  include
  src
)

add_library(lib_graph STATIC
  src/graph.cpp
)
target_link_libraries(lib_graph
  lib_assert
  lib_hash
)
target_include_directories(lib_graph PRIVATE
  include
  ${CMAKE_SOURCE_DIR}/src/utilities/include
)

add_library(lib_sequence STATIC
  src/sequence.cpp
)
target_link_libraries(lib_sequence
  lib_assert
)
target_include_directories(lib_sequence PRIVATE
  src
  ${CMAKE_SOURCE_DIR}/src/utilities/include
)

add_subdirectory(benchmark)
add_subdirectory(test)

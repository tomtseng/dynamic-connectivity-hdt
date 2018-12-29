add_library(lib_dynamic_forest STATIC
  src/dynamic_forest.cpp
)
target_link_libraries(lib_dynamic_forest
  lib_hash
  lib_sequence
)
target_include_directories(lib_dynamic_forest PUBLIC
  ${CMAKE_SOURCE_DIR}/src/utilities/include
)
target_include_directories(lib_dynamic_forest PRIVATE
  include
  src
)

add_library(lib_sequence STATIC
  src/sequence.cpp
)
target_include_directories(lib_sequence PRIVATE
  include
  src
  ${CMAKE_SOURCE_DIR}/src/utilities/include
)

add_subdirectory(test)

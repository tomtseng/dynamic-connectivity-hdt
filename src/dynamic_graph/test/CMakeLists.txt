include (GoogleTest)

add_executable(test_dynamic_connectivity
  test_dynamic_connectivity.cpp
)
target_include_directories(test_dynamic_connectivity PRIVATE
  ../include
)
target_link_libraries(test_dynamic_connectivity
  gtest_main
  lib_dynamic_connectivity
)
gtest_discover_tests(test_dynamic_connectivity)

add_executable(test_dynamic_forest
  test_dynamic_forest.cpp
)
target_include_directories(test_dynamic_forest PRIVATE
  ../src
)
target_link_libraries(test_dynamic_forest
  gmock
  gtest_main
  lib_dynamic_forest
)
gtest_discover_tests(test_dynamic_forest)

add_executable(test_sequence
  test_sequence.cpp
)
target_include_directories(test_sequence PRIVATE
  ../src
)
target_link_libraries(test_sequence
  gmock
  gtest_main
  lib_sequence
)
gtest_discover_tests(test_sequence)

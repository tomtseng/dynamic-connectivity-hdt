include (GoogleTest)

add_executable(test_dynamic_trees
  test_dynamic_trees.cpp
)
target_link_libraries(test_dynamic_trees
  gtest_main
  lib_dynamic_trees
)
gtest_discover_tests(test_dynamic_trees)
add_executable(test_sequence
  test_sequence.cpp
)
target_link_libraries(test_sequence
  gtest_main
  lib_sequence
)
gtest_discover_tests(test_sequence)

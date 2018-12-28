include (GoogleTest)

add_executable(test_dynamic_forest
  test_dynamic_forest.cpp
)
target_include_directories(test_dynamic_forest PRIVATE
  ../src
)
target_link_libraries(test_dynamic_forest
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
  gtest_main
  lib_sequence
)
gtest_discover_tests(test_sequence)

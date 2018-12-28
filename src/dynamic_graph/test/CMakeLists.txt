include (GoogleTest)

add_executable(test_dynamic_forest
  test_dynamic_forest.cpp
)
target_include_directories(test_dynamic_forest PRIVATE
  ../include
)
target_link_libraries(test_dynamic_forest
  gtest_main
  lib_dynamic_forest
)
gtest_discover_tests(test_dynamic_forest)

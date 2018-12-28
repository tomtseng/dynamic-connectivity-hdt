include (GoogleTest)

add_executable(test_sequence
  test_sequence.cpp
)
target_include_directories(test_sequence PRIVATE
  ../include
)
target_link_libraries(test_sequence
  gtest_main
  lib_sequence
)
gtest_discover_tests(test_sequence)

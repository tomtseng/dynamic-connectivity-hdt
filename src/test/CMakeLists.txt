include (GoogleTest)

add_executable(test_treap
  test_treap.cpp
)

target_link_libraries(test_treap
  gtest_main
  lib_treap
)

gtest_discover_tests(test_treap)

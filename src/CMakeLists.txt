include_directories(include)

add_library(lib_treap STATIC
  src/treap.cpp
)

add_subdirectory(test)

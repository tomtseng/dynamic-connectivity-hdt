cmake_minimum_required(VERSION 2.8.12.2)
project(dynamic_connectivity)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include_directories(include)

add_library(dynamic_connectivity STATIC
    src/treap.cpp
)

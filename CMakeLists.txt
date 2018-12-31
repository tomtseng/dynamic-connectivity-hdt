cmake_minimum_required(VERSION 3.13.2)
project(dynamic_connectivity)

## Google Test installation {{{

configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()

# Add googletest directly to our build. This defines
# the gtest and gtest_main targets.
add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/googletest-src
                 ${CMAKE_CURRENT_BINARY_DIR}/googletest-build
                 EXCLUDE_FROM_ALL)

## end Google Test installation }}}

## Doxygen {{{

find_package(Doxygen)
if (NOT DOXYGEN_FOUND)
  message(WARNING
      "doxygen not found - doxygen documentation will not be available.")
endif()

set(docs_directory "${CMAKE_SOURCE_DIR}/docs/")

add_custom_target(docs
  COMMENT "--- Generating documentation ---"
  COMMAND cd ${docs_directory} && doxygen
)

## end Doxygen}}}

set(CMAKE_CXX_COMPILER "/usr/bin/g++-7")
set(GCC_COMPILE_FLAGS "\
  -std=c++17 \
  \
  -DBOOST_STACKTRACE_USE_BACKTRACE \
  \
  -Wall \
  -Wdouble-promotion \
  -Wduplicated-branches \
  -Wduplicated-cond \
  -Wextra \
  -Wlogical-op \
  -Wnull-dereference \
  -Wrestrict \
  -Wshadow \
  -Wold-style-cast \
  -Wuseless-cast \
")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_COMPILE_FLAGS}")

enable_testing()

# Custom target for running ctest with more helpful output.
add_custom_target(check
  COMMENT "--- Running tests ---"
  COMMAND GTEST_COLOR=1 ${CMAKE_CTEST_COMMAND} --output-on-failure
)

add_subdirectory(src)

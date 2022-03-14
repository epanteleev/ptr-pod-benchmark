# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required(VERSION 3.11)

project("Benchmarks")

set(CMAKE_CXX_FLAGS "-Wall -Wextra -O3 -save-temps")

add_subdirectory(src)
# Include sub-projects.
add_subdirectory(bench)

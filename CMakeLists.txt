cmake_minimum_required(VERSION 3.11)

project("Benchmarks")

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "-Wall -Wextra -save-temps")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O5")

add_subdirectory(src)
add_subdirectory(test)

add_subdirectory(bench)
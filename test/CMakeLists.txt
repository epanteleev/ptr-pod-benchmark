cmake_minimum_required(VERSION 3.11)


include(../cmake/GoogleTest.cmake)

set(CMAKE_CXX_FLAGS "-Wall -Wextra -save-temps -Wno-strict-aliasing")
add_executable(testHashMap testHashMap.cpp)
target_link_libraries(testHashMap gtest gtest_main lib)
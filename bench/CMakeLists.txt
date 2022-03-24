cmake_minimum_required(VERSION 3.11)

include(../cmake/GoogleBenchmark.cmake)

add_executable(Benchmarks benchRef.cpp benchPod.cpp)
target_link_libraries(Benchmarks benchmark::benchmark lib)

if ("${CMAKE_SYSTEM_NAME}" MATCHES "Windows")
    target_link_libraries(Benchmarks Shlwapi)
endif ()
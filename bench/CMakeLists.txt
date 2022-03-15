cmake_minimum_required(VERSION 3.11)

# Include Google Benchmark
include(../cmake/GoogleBenchmark.cmake)

# Add source to this project's executable.
add_executable(Benchmarks benchRef.cpp benchPod.cpp)
# Link Google Benchmark to the project
target_link_libraries(Benchmarks benchmark::benchmark lib)

# Link Shlwapi to the project
if ("${CMAKE_SYSTEM_NAME}" MATCHES "Windows")
    target_link_libraries(Benchmarks Shlwapi)
endif ()
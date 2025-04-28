cmake_minimum_required(VERSION 3.10)

project(AllocatorAnalyzer)

set(CMAKE_CXX_STANDARD 23)
add_executable(allocator allocator.cpp)
add_executable(analyzer analyzer.cpp)
add_executable(main main.cpp)

# Add the test
enable_testing()
add_test(NAME PipelineTest
    COMMAND main "--allocator=$<TARGET_FILE:allocator> 50 1000" "--analyzer=$<TARGET_FILE:analyzer>")

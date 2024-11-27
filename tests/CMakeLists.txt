cmake_minimum_required(VERSION 3.30)

# Test Harness uses C++
project(PowerTaskTests LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Get Test Harness
find_package(CppUTest REQUIRED)

add_executable(run_tests main.cpp)

target_link_libraries(run_tests PRIVATE CppUTest PowerTask)
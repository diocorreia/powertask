cmake_minimum_required(VERSION 3.30)

# Enable c++ (required by the test harness)
project(PowerTaskTests LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Get test harness
find_package(CppUTest REQUIRED)

# Add project header files
include_directories(${CMAKE_SOURCE_DIR}/include)

# Add tests
add_subdirectory(scheduler)

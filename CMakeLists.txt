cmake_minimum_required(VERSION 3.30)

project(
    PowerTask
    VERSION 0.0.1
    LANGUAGES C
)

add_subdirectory(tests)

add_library(PowerTask STATIC src/scheduler.c)

target_include_directories(PowerTask PUBLIC include)

option(ENABLE_COVERAGE "Enable code coverage" OFF)

if(ENABLE_COVERAGE)
target_compile_options(PowerTask PRIVATE -coverage)
target_link_libraries(PowerTask PRIVATE gcov)
endif()

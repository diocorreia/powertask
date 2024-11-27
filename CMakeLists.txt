cmake_minimum_required(VERSION 3.30)

project(
    PowerTask
    VERSION 0.0.1
    LANGUAGES C
)

add_subdirectory(tests)

add_library(PowerTask STATIC src/scheduler.c)

target_include_directories(PowerTask PUBLIC include)
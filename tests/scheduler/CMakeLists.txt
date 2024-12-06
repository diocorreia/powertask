add_executable(test_scheduler 
    ${CMAKE_SOURCE_DIR}/tests/RunAllTests.cpp
    src/scheduler.cpp
    src/mocks.cpp
)

if(ENABLE_COVERAGE)
target_compile_options(test_scheduler PRIVATE -coverage)
endif()

target_link_libraries(test_scheduler CppUTest CppUTestExt PowerTask)

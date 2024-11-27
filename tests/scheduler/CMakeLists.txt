add_executable(test_scheduler 
    ${CMAKE_SOURCE_DIR}/tests/RunAllTests.cpp
    src/scheduler.cpp
    src/mocks.cpp
)

target_link_libraries(test_scheduler CppUTest CppUTestExt PowerTask)

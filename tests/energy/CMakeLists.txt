add_executable(test_energy 
    ${CMAKE_SOURCE_DIR}/tests/RunAllTests.cpp
    src/energy.cpp
)

if(ENABLE_COVERAGE)
target_compile_options(test_energy PRIVATE -coverage)
endif()

target_link_libraries(test_energy CppUTest CppUTestExt PowerTask)

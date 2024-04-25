cmake_minimum_required(VERSION 3.10)
project(tests)

# Collect tests
file(GLOB TESTS_FILES ${CMAKE_CURRENT_SOURCE_DIR}/unit/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/../src/cppSocketLib.cpp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage")

# Ensure these paths are correct
include_directories(${PROJECT_SOURCE_DIR}/../src)
link_directories(${PROJECT_SOURCE_DIR}/../src)

if(RUN_COVERAGE)
    message("Coverage enabled")

    # Busca las herramientas necesarias para la cobertura de código
    find_program(LCOV_PATH lcov)
    if(NOT LCOV_PATH)
        message(FATAL_ERROR "lcov not found! Aborting...")
    endif()

    find_program(GCOV_PATH gcov)
    if(NOT GCOV_PATH)
        message(FATAL_ERROR "gcov not found! Aborting...")
    endif()

    find_program(GENHTML_PATH genhtml)
    if(NOT GENHTML_PATH)
        message(FATAL_ERROR "genhtml not found! Aborting...")
    endif()
endif()

# Create the test executable
add_executable(tests ${TESTS_FILES}) # Include TESTS_FILES variable
target_link_libraries(tests gtest gtest_main) # Link with Google Test

# Add test
add_test(NAME test_${PROJECT_NAME} COMMAND tests) # Use the target name 'tests' for the command

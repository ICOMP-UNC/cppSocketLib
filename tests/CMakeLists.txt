cmake_minimum_required(VERSION 3.10)
project(unit_tests)

# Collect tests
file(GLOB TESTS_FILES ${CMAKE_CURRENT_SOURCE_DIR}/unit/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/../src/*.cpp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -fprofile-arcs -ftest-coverage")

# Ensure these paths are correct
include_directories(${PROJECT_SOURCE_DIR}/../src)

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
add_executable(${PROJECT_NAME} ${TESTS_FILES}) # Include TESTS_FILES variable
target_link_libraries(${PROJECT_NAME}
    debug gtest
    debug gmock
    debug gtest_main
    debug gmock_main
    optimized gtest
    optimized gmock
    optimized gtest_main
    optimized gmock_main
)

# Add test
add_test(NAME ${PROJECT_NAME} COMMAND ${PROJECT_NAME})

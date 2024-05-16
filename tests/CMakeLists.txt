cmake_minimum_required(VERSION 3.10)
project(unit_tests)

# Collect tests
file(GLOB TESTS_FILES ${CMAKE_CURRENT_SOURCE_DIR}/unit/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/../src/*.cpp)

set(CMAKE_CXX_FLAGS_DEBUG "-g -fprofile-arcs -ftest-coverage --coverage -fsanitize=address,leak,undefined")

# Ensure these paths are correct
include_directories(${PROJECT_SOURCE_DIR}/../src)

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

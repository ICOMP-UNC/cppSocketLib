cmake_minimum_required(VERSION 3.10)
project(tests)

# Collect tests
file(GLOB TESTS_FILES ${CMAKE_CURRENT_SOURCE_DIR}/unit/*.cpp)

# Ensure these paths are correct
include_directories(${PROJECT_SOURCE_DIR}/../src)
link_directories(${PROJECT_SOURCE_DIR}/../src)

# Create the test executable
add_executable(tests ${TESTS_FILES}) # Include TESTS_FILES variable
target_link_libraries(tests gtest gtest_main) # Link with Google Test

# Add test
add_test(NAME test_${PROJECT_NAME} COMMAND tests) # Use the target name 'tests' for the command

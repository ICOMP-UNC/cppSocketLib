# Request the minimum version of CMake, in case of lower version throws an error.
cmake_minimum_required(VERSION 3.25 FATAL_ERROR)
cmake_policy(SET CMP0148 NEW)
# Define your project and specify the use of C++.
project(
  "FirstProjectInCMake"
  VERSION 1.0.0
  DESCRIPTION "CMake C++ Project Template for Operating Systems II"
  LANGUAGES CXX  # Change from C to C++
)

# Enable testing with CTest which is integrated with GTest
include(CTest)
enable_testing()

# Define the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include the `include` directory, where the headers are located
include_directories(include)

# To automatically download external dependencies, use `FetchContent`
include(FetchContent)

# Configure FetchContent to download Google Test
set(GTEST_GIT_URL "https://github.com/google/googletest.git")

FetchContent_Declare(
  googletest
  GIT_REPOSITORY ${GTEST_GIT_URL}
  GIT_TAG release-1.10.0  # Optionally pin to a stable release
)

FetchContent_MakeAvailable(googletest)

# Add the `src` directory, where the C++ source files are located
file(GLOB_RECURSE SOURCES "src/*.cpp")  # Change from *.c to *.cpp

# Create the executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Setup Google Test

if((RUN_TESTS EQUAL 1 OR RUN_COVERAGE EQUAL 1) AND BUILD_TESTING)  # This is set by enable_testing()
  add_subdirectory(tests)

  # Link Google Test to the test executable
  add_executable(unit_tests ${TEST_SOURCES})
  target_link_libraries(unit_tests gtest_main gmock_main)
  target_include_directories(unit_tests PUBLIC ${gtest_SOURCE_DIR}/include ${gmock_SOURCE_DIR}/include)

  # Add the tests to be run by CTest
  include(GoogleTest)
  gtest_discover_tests(unit_tests)
endif()

# Request the minimum version of CMake, in case of lower version throws an error.
cmake_minimum_required(VERSION 3.25 FATAL_ERROR)
# Define your project and specify the use of C++.
project(
  "SocketWrapper"
  VERSION 1.0.0
  DESCRIPTION "Socket Wrapper for c++"
  LANGUAGES CXX
)

# Enable testing with CTest which is integrated with GTest
include(CTest)
enable_testing()

# Define the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-error=maybe-uninitialized")

# Include the `include` directory, where the headers are located
include_directories(include)

# To automatically download external dependencies, use `FetchContent`
include(FetchContent)

# Configure FetchContent to download Google Test
set(GTEST_GIT_URL "https://github.com/google/googletest.git")

# Enable debug
set(FETCHCONTENT_QUIET OFF)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY ${GTEST_GIT_URL}
  GIT_TAG v1.13.0  # Optionally pin to a stable release
)

FetchContent_MakeAvailable(googletest)


# Add the `src` directory, where the C++ source files are located
file(GLOB_RECURSE SOURCES "src/*.cpp")  # Change from *.c to *.cpp

# Create the executable
add_library(${PROJECT_NAME} ${SOURCES})

# Setup Google Test
if(RUN_TESTS EQUAL 1 OR RUN_COVERAGE EQUAL 1)  # This is set by enable_testing()
  add_subdirectory(tests)
endif()

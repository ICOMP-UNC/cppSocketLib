# Environment configuration

set(CTEST_PROJECT_NAME "EchoesOfSurvival")
set(CTEST_SUBMIT_URL 
    "http://localhost:8080/submit.php?project=${CTEST_PROJECT_NAME}")
set(CTEST_USE_LAUNCHERS YES)

# General settings
site_name(CTEST_SITE)
set(CTEST_BUILD_NAME "linux-x86_64-gcc")
set(CTEST_SOURCE_DIRECTORY "${CTEST_SCRIPT_DIRECTORY}")
message(STATUS "CTEST_SOURCE_DIRECTORY: ${CTEST_SOURCE_DIRECTORY}")
set(CTEST_BINARY_DIRECTORY "${CTEST_SCRIPT_DIRECTORY}/build")
message(STATUS "CTEST_BINARY_DIRECTORY: ${CTEST_BINARY_DIRECTORY}/../build")

# Build settings
set(CTEST_CMAKE_GENERATOR Ninja)
set(CTEST_CONFIGURATION_TYPE Debug)
set(configureOpts
    "-DRUN_COVERAGE=1"
    )

# Coverage settings
set(CTEST_COVERAGE_COMMAND gcov)

#Memcheck
find_program(CTEST_MEMORYCHECK_COMMAND valgrind)
set(CTEST_MEMORYCHECK_COMMAND_OPTIONS "--trace-children=yes --leak-check=full")

# Step 1 - Clean previous pipeline run
ctest_empty_binary_directory(${CTEST_BINARY_DIRECTORY})

# Step 2 - Configure
ctest_start(Experimental)
ctest_configure(OPTIONS "${configureOpts}")
ctest_submit(PARTS Start Configure)

# Step 3 - Build
ctest_build(PARALLEL_LEVEL 16)
ctest_submit(PARTS Build)

# Step 4 - Run unit tests
ctest_test(PARALLEL_LEVEL 16)
ctest_submit(PARTS Test)

# Step 5 - Mem leak
ctest_memcheck()
ctest_submit(PARTS Memcheck)

# Step 5 - Collect coverage information
ctest_coverage()
ctest_submit(PARTS Coverage Done)

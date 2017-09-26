cmake_minimum_required(VERSION 2.4)

# Settings:
set(CTEST_DASHBOARD_ROOT                "/Users/davidcoplowe/software/xsLLhFitter/cmake/CMAKE/Darwin/cmake-3.5.0/Tests/CTestTest")
set(CTEST_SITE                          "davidcoplowe.nat.physics.ox.ac.uk")
set(CTEST_BUILD_NAME                    "CTestTest-Darwin-g++-Scheduler")

set(CTEST_SOURCE_DIRECTORY              "/Users/davidcoplowe/software/xsLLhFitter/cmake/CMAKE/Darwin/cmake-3.5.0/Tests/CTestTestScheduler")
set(CTEST_BINARY_DIRECTORY              "/Users/davidcoplowe/software/xsLLhFitter/cmake/CMAKE/Darwin/cmake-3.5.0/Tests/CTestTestScheduler")
set(CTEST_CVS_COMMAND                   "/opt/local/bin/cvs")
set(CTEST_CMAKE_GENERATOR               "Unix Makefiles")
set(CTEST_CMAKE_GENERATOR_PLATFORM      "")
set(CTEST_CMAKE_GENERATOR_TOOLSET       "")
set(CTEST_BUILD_CONFIGURATION           "$ENV{CMAKE_CONFIG_TYPE}")
set(CTEST_COVERAGE_COMMAND              "/usr/bin/gcov")
set(CTEST_NOTES_FILES                   "${CTEST_SCRIPT_DIRECTORY}/${CTEST_SCRIPT_NAME}")

#CTEST_EMPTY_BINARY_DIRECTORY(${CTEST_BINARY_DIRECTORY})

# Remove old cost data file if it exists
if(EXISTS "${CTEST_BINARY_DIRECTORY}/Testing/Temporary/CTestCostData.txt")
  file(REMOVE "${CTEST_BINARY_DIRECTORY}/Testing/Temporary/CTestCostData.txt")
endif()

CTEST_START(Experimental)
CTEST_CONFIGURE(BUILD "${CTEST_BINARY_DIRECTORY}" RETURN_VALUE res)
CTEST_BUILD(BUILD "${CTEST_BINARY_DIRECTORY}" RETURN_VALUE res)
CTEST_TEST(BUILD "${CTEST_BINARY_DIRECTORY}" RETURN_VALUE res PARALLEL_LEVEL 5)
# Run test set a second time to make sure they run in reverse order
CTEST_TEST(BUILD "${CTEST_BINARY_DIRECTORY}" RETURN_VALUE res PARALLEL_LEVEL 5)

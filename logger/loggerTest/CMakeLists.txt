# CMakeList.txt : CMake project for gTest, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.14)

set(
	LOGGER_TEST_HEADER
	loggerTest.h
)

set(
	LOGGER_TEST_SOURCE
	gTest.cpp
	loggerTest.cpp
)

# Add source to this project's executable.
add_executable (loggerTest ${LOGGER_TEST_HEADER} ${LOGGER_TEST_SOURCE})

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET loggerTest PROPERTY CXX_STANDARD 20)
endif()

include(FetchContent)
FetchContent_Declare(
  googletest
  # Specify the commit you depend on and update it regularly.
  URL https://github.com/google/googletest/archive/e2239ee6043f73722e7aa812a459f54a28552929.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Now simply link against gtest or gtest_main as needed. Eg
target_link_libraries(loggerTest gtest_main logger)

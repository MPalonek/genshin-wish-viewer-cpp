# CMakeList.txt : CMake project for gTest, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.14)

set(
	DATABASE_TEST_HEADER
	utilityTest.h
)

set(
	DATABASE_TEST_SOURCE
	gTest.cpp
	utilityTest.cpp
	databaseTest.cpp
)

# Add source to this project's executable.
add_executable (databaseTest ${DATABASE_TEST_HEADER} ${DATABASE_TEST_SOURCE})

if (CMAKE_VERSION VERSION_GREATER 3.12)
	set_property(TARGET databaseTest PROPERTY CXX_STANDARD 20)
endif()

# Now simply link against gtest or gtest_main as needed. Eg
target_link_libraries(databaseTest gtest_main db)

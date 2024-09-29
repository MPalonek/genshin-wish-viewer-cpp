# CMakeList.txt : CMake project for gTest, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.14)

set(
	IMPORTER_TEST_HEADER
)

set(
	IMPORTER_TEST_SOURCE
	gTest.cpp
	importerTest.cpp
)

# Add source to this project's executable.
add_executable (importerTest ${IMPORTER_TEST_HEADER} ${IMPORTER_TEST_SOURCE})

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET importerTest PROPERTY CXX_STANDARD 20)
endif()

# Custom step to copy images to test executable directory
set(TEST_IMG_DIR "${CMAKE_SOURCE_DIR}/importer/importerTest/img")

# Get the output directory based on the current configuration (Debug, Release, etc.)
set(OUTPUT_DIR "$<TARGET_FILE_DIR:importerTest>/img")
set(OUTPUT_DIR2 "$<TARGET_FILE_DIR:importerTest>/../img") # specifically for VisualStudio debugger path

# Add a custom command to copy the test images directory to the output folder
add_custom_command(
    TARGET importerTest
    PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
		"${TEST_IMG_DIR}" "${OUTPUT_DIR}"
	COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${TEST_IMG_DIR}" "${OUTPUT_DIR2}"
    COMMENT "Copying img directory to output..."
)

# Now simply link against gtest or gtest_main as needed. Eg
target_link_libraries(importerTest gtest_main importer logger)

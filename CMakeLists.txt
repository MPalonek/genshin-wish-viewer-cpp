# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)
cmake_policy(SET CMP0091 NEW) # for MSVC_RUNTIME_LIBRARY
project ("genshin-wish-viewer")

# gTest
#if(BUILD_TESTING)
#  include(AddGoogleTest)
#  enable_testing ()  
#endif()
message("Genshin Wish Viewer...")
message("Building configuration: ${CMAKE_BUILD_TYPE}")
message("CMAKE_SOURCE_DIR: ${CMAKE_SOURCE_DIR}")
message("CMAKE_BINARY_DIR: ${CMAKE_BINARY_DIR}")

if(WIN32)
	# Set the runtime library for the Release configuration
	set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded")
	# Set the runtime library for the Debug configuration
	set(CMAKE_MSVC_RUNTIME_LIBRARY_DEBUG "MultiThreadedDebug")
endif()

# Include sub-projects.
add_subdirectory ("logger")
#add_subdirectory ("database")
add_subdirectory ("importer")
#add_subdirectory ("viewer")

# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)
cmake_policy(SET CMP0091 NEW) # for MSVC_RUNTIME_LIBRARY
cmake_policy(SET CMP0135 NEW) # for FetchConent DOWNLOAD_EXTRACT_TIMESTAMP option
project ("genshin-wish-viewer")

message("Genshin Wish Viewer...")
message("Building configuration: ${CMAKE_BUILD_TYPE}")
message("CMAKE_SOURCE_DIR: ${CMAKE_SOURCE_DIR}")
message("CMAKE_BINARY_DIR: ${CMAKE_BINARY_DIR}")

# gTest
if(BUILD_TESTING)
	message("Build Testing enabled")
	enable_testing()
	
	include(FetchContent)
	FetchContent_Declare(
	  googletest
	  # Specify the commit you depend on and update it regularly.
	  URL https://github.com/google/googletest/archive/e2239ee6043f73722e7aa812a459f54a28552929.zip
	)
	# For Windows: Prevent overriding the parent project's compiler/linker settings
	set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
	FetchContent_MakeAvailable(googletest)

else()
	message("Build Testing disabled")
endif()

if(WIN32)
	# # Set the runtime library for the Release configuration
	# set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded")
	# # Set the runtime library for the Debug configuration
	# set(CMAKE_MSVC_RUNTIME_LIBRARY_DEBUG "MultiThreadedDebug")
endif()

# Set default build type to Release if none is specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Default build type" FORCE)
endif()

# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MD")  # For multi-threaded DLL



# Include sub-projects.
add_subdirectory ("logger")
add_subdirectory ("database")
add_subdirectory ("importer")
#add_subdirectory ("viewer")

# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

project ("genshin-wish-viewer")

# gTest
#if(BUILD_TESTING)
#  include(AddGoogleTest)
#  enable_testing ()  
#endif()

# Include sub-projects.
add_subdirectory ("database")
#add_subdirectory ("importer")
#add_subdirectory ("viewer")

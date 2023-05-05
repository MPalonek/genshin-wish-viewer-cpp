# https://gist.github.com/UnaNancyOwen/5061d8c966178b753447e8a9f9ac8cf1

cmake_minimum_required (VERSION 3.15)
cmake_policy(SET CMP0091 NEW) # for MSVC_RUNTIME_LIBRARY

project ("importer")

set(
	IMPORTER_HEADER
	src/importer.h
)

set(
	IMPORTER_SOURCE
	src/importer.cpp
)

set( OpenCV_STATIC ON )
find_package( OpenCV CONFIG REQUIRED )
find_package( Tesseract CONFIG REQUIRED )


if(MSVC)
    add_compile_options(
        $<$<CONFIG:>:/MT> #---------|
        $<$<CONFIG:Debug>:/MTd> #---|-- Statically link the runtime libraries
        $<$<CONFIG:Release>:/MT> #--|
    )
endif()



#add_library(importer ${IMPORTER_HEADER} ${IMPORTER_SOURCE})
add_executable(importer ${IMPORTER_HEADER} ${IMPORTER_SOURCE})

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET importer PROPERTY CXX_STANDARD 20)
endif()

# this doesnt work...
set_property(TARGET importer PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

include_directories( ${OpenCV_INCLUDE_DIRS} )
link_directories( ${OpenCV_LIB_DIR} )


#if (BUILD_TESTING)
#	add_subdirectory (importerTest)
#endif()

target_link_libraries( importer ${OpenCV_LIBS} Tesseract::libtesseract )
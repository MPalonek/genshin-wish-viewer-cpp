# https://gist.github.com/UnaNancyOwen/5061d8c966178b753447e8a9f9ac8cf1

cmake_minimum_required (VERSION 3.15)

project ("importer")

set( IMPORTER_HEADER
	src/importer.h
)

set( IMPORTER_SOURCE
	src/importer.cpp
)

set( OpenCV_STATIC ON )
find_package( OpenCV CONFIG REQUIRED )
find_package( Tesseract CONFIG REQUIRED )

#add_library(importer ${IMPORTER_HEADER} ${IMPORTER_SOURCE})
add_executable(importer ${IMPORTER_HEADER} ${IMPORTER_SOURCE})

target_include_directories(importer PRIVATE "${CMAKE_SOURCE_DIR}")

set(LOGGER_LIBRARY_DIR "${CMAKE_BINARY_DIR}/logger")  # Path to the logger library directory
link_directories(${LOGGER_LIBRARY_DIR})

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET importer PROPERTY CXX_STANDARD 20)
endif()

include_directories( ${OpenCV_INCLUDE_DIRS} )
link_directories( ${OpenCV_LIB_DIR} )

#if (BUILD_TESTING)
#	add_subdirectory (importerTest)
#endif()

target_link_libraries( importer ${OpenCV_LIBS} Tesseract::libtesseract logger )
# https://gist.github.com/UnaNancyOwen/5061d8c966178b753447e8a9f9ac8cf1

cmake_minimum_required (VERSION 3.15)

project ("importer")

set( IMPORTER_HEADER
    src/importer.h
    src/importerItem.h
)

set( IMPORTER_SOURCE
    src/importer.cpp
    src/importerItem.cpp
)

if (UNIX)
	find_library( Tesseract_LIBRARY NAMES tesseract )
	find_path( Tesseract_INCLUDE_DIR NAMES tesseract/baseapi.h )

	if (Tesseract_LIBRARY AND Tesseract_INCLUDE_DIR)
		set(Tesseract_FOUND TRUE)
	endif()
	if (Tesseract_FOUND)
		message(STATUS "Found Tesseract library: ${Tesseract_LIBRARY}")
		message(STATUS "Found Tesseract include directory: ${Tesseract_INCLUDE_DIR}")
	else()
		message(FATAL_ERROR "Tesseract library not found")
	endif()
else() # WINDOWS
	message(STATUS "CMAKE_TOOLCHAIN_FILE: ${CMAKE_TOOLCHAIN_FILE}")
	message(STATUS "CMAKE_PREFIX_PATH: ${CMAKE_PREFIX_PATH}")
	find_package( Tesseract CONFIG REQUIRED )

	if (Tesseract_FOUND)
		include_directories(${Tesseract_INCLUDE_DIRS}) # This should include Leptonica
	else()
		message(FATAL_ERROR "Tesseract not found!")
	endif()
endif()

add_library(importer ${IMPORTER_HEADER} ${IMPORTER_SOURCE})
#add_executable(importer ${IMPORTER_HEADER} ${IMPORTER_SOURCE})

target_include_directories(importer PUBLIC "${CMAKE_SOURCE_DIR}")

set(LOGGER_LIBRARY_DIR "${CMAKE_BINARY_DIR}/logger")  # Path to the logger library directory
link_directories(${LOGGER_LIBRARY_DIR})

if (CMAKE_VERSION VERSION_GREATER 3.12)
	set_property(TARGET importer PROPERTY CXX_STANDARD 20)
endif()

if (BUILD_TESTING)
	add_subdirectory (importerTest)
endif()

if (UNIX)
	target_link_libraries( importer ${Tesseract_LIBRARY} lept logger )
else() # WINDOWS
	target_link_libraries( importer Tesseract::libtesseract logger )
endif()

cmake_minimum_required (VERSION 3.8)

project ("logger")

set( LOGGER_HEADER
	src/logger.h
)

set( LOGGER_SOURCE
	src/logger.cpp
)

add_library(logger ${LOGGER_HEADER} ${LOGGER_SOURCE})
#add_executable(logger ${LOGGER_HEADER} ${LOGGER_SOURCE})
# https://stackoverflow.com/questions/43862273/cmake-c-include-relative-to-base-directory
#target_include_directories(logger PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET logger PROPERTY CXX_STANDARD 20)
endif()

#if (BUILD_TESTING)
#	add_subdirectory (loggerTest)
#endif()

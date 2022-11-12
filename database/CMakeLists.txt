cmake_minimum_required (VERSION 3.8)

project ("database")

set(
	DATABASE_HEADER
	src/sqlite3.h
	src/db.h
)

set(
	DATABASE_SOURCE
	src/sqlite3.c
	src/db.cpp
)

add_library(db ${DATABASE_HEADER} ${DATABASE_SOURCE})
#add_executable(db ${DATABASE_HEADER} ${DATABASE_SOURCE})

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET db PROPERTY CXX_STANDARD 20)
endif()

#if (BUILD_TESTING)
	add_subdirectory (databaseTest)
#endif()

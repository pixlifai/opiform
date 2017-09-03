CMAKE_MINIMUM_REQUIRED(VERSION 3.1)

PROJECT(opiform)

SET_PROPERTY(GLOBAL PROPERTY USE_FOLDERS ON)

SET(CMAKE_DEBUG_POSTFIX "D")

# Warn in the case of in-source build
if("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_CURRENT_BINARY_DIR}")
  message(WARNING "The source directory is the same as binary directory.")
endif()

#set install directory
set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/../install CACHE STRING
	  "Redirect install dir to default" FORCE)
set(PROJECT_INSTALL_INCLUDE_DIR ${CMAKE_INSTALL_PREFIX}/include)

include(cmake/opiform.cmake)

add_subdirectory(opiform)
add_subdirectory(tests)
add_subdirectory(demo)
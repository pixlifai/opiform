CMAKE_MINIMUM_REQUIRED(VERSION 3.1)

PROJECT(OpinionFormation)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

SET(CMAKE_DEBUG_POSTFIX "D")

# Warn in the case of in-source build
if("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_CURRENT_BINARY_DIR}")
  message(WARNING "The source directory is the same as binary directory.")
endif()

#set install directory
set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/../install CACHE STRING
	  "Redirect install dir to default" FORCE)
set(PROJECT_INSTALL_INCLUDE_DIR ${CMAKE_INSTALL_PREFIX}/include)


FILE(GLOB_RECURSE files RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}/opiform"
    "${CMAKE_CURRENT_SOURCE_DIR}/opiform/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/opiform/*.cpp"
)

foreach(FILE ${files}) 
  get_filename_component(PARENT_DIR "${FILE}" PATH)

  # skip src or include and changes /'s to \\'s
  string(REGEX REPLACE "(\\./)?(src|include)/?" "" GROUP "${PARENT_DIR}")
  string(REPLACE "/" "\\" GROUP "${GROUP}")

  # group into "Source Files" and "Header Files"
  if ("${FILE}" MATCHES ".*\\.cpp")
    set(GROUP "Source Files\\${GROUP}")
  elseif("${FILE}" MATCHES ".*\\.h")
    set(GROUP "Header Files\\${GROUP}")
  endif()

  source_group(${GROUP} FILES ${FILE})
endforeach()


add_subdirectory(opiform)
add_subdirectory(tests)
add_subdirectory(demo)
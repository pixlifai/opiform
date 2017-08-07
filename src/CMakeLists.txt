CMAKE_MINIMUM_REQUIRED(VERSION 3.1)

PROJECT(opiform)

SET(CMAKE_DEBUG_POSTFIX "D")

# Warn in the case of in-source build
if("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_CURRENT_BINARY_DIR}")
  message(WARNING "The source directory is the same as binary directory.")
endif()

#set install directory
set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/../install CACHE STRING
	  "Redirect install dir to default" FORCE)
set(PROJECT_INSTALL_INCLUDE_DIR ${CMAKE_INSTALL_PREFIX}/include)


SET ( src
	Utils/Writer.h Utils/Writer.cpp
	Utils/OpinionFormationModels.h Utils/OpinionFormationModels.cpp
	Utils/DecisionMaking.h Utils/DecisionMaking.cpp
	Agent/AgentBase.h Agent/AgentBase.cpp
	Agent/AgentStubborn.h Agent/AgentStubborn.cpp
	Agent/AgentInconsistent.h Agent/AgentInconsistent.cpp
	Network/NetworkAbstract.h Network/NetworkAbstract.cpp
	Network/NetworkBA.h Network/NetworkBA.cpp
	Network/NetworkER.h Network/NetworkER.cpp
	Network/NetworkFC.h Network/NetworkFC.cpp
	Network/NetworkSW.h Network/NetworkSW.cpp
	Network/NetworkCM.h Network/NetworkCM.cpp
	Game/Game.h Game/Game.cpp
)

FILE(GLOB_RECURSE files RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

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

  source_group("${GROUP}" FILES "${FILE}")
endforeach()

ADD_EXECUTABLE(AgentBased
	${src}
	Main/main.cpp
)
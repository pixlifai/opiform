PROJECT(demo)

INCLUDE_DIRECTORIES(
	${CMAKE_SOURCE_DIR}/opiform
)

ADD_EXECUTABLE(demo
	main.cpp
)

TARGET_LINK_LIBRARIES(demo
	opiform
)

SET_PROPERTY(TARGET demo PROPERTY FOLDER "Executable")
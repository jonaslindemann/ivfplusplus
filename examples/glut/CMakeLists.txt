project(ivfglutapp)

cmake_minimum_required(VERSION 2.8)

set( LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib )
set( EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin )

if(WIN32)
	include_directories(${PROJECT_SOURCE_DIR}/../../include)
	link_directories(${PROJECT_SOURCE_DIR}/../../lib)
else(WIN32)
	include_directories(${PROJECT_SOURCE_DIR}/../../include)
	link_directories(${PROJECT_SOURCE_DIR}/../../lib)
endif(WIN32)

file(GLOB SOURCE_FILES *.cpp)
file(GLOB INCLUDE_FILES *.h)
file(GLOB EXTRA_FILES ${PROJECT_SOURCE_DIR}/../../src/ivfglut/*.cpp)

message(${EXTRA_FILES})

if (APPLE)
	set( EXT_LIBS "-framework carbon" "-framework cocoa" "-framework applicationservices" "-framework agl" "-framework opengl" "-framework GLUT" ivfmath ivf ivfwidget) 
endif (APPLE)

add_executable(ivfglutapp ${INCLUDE_FILES} ${SOURCE_FILES} ${EXTRA_FILES})
target_link_libraries(ivfglutapp ${EXT_LIBS})



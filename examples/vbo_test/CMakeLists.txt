cmake_minimum_required(VERSION 3.10)

project(vbo_test)

# Find required packages
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

# Include directories
include_directories(${ivf_SOURCE_DIR}/include)
include_directories(${OPENGL_INCLUDE_DIR})
include_directories(${GLUT_INCLUDE_DIR})

# Add executable
add_executable(vbo_test vbo_test.cpp)

# Link libraries
target_link_libraries(vbo_test 
    gleivf glad
    ${OPENGL_LIBRARIES}
    ${GLUT_LIBRARIES}
)

# Set output properties
set_target_properties(vbo_test PROPERTIES
    DEBUG_POSTFIX "d"
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# Install
install(TARGETS vbo_test DESTINATION bin)

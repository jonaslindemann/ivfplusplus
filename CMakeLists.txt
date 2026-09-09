cmake_minimum_required(VERSION 3.20)

set(_vcpkg_toolchain_already_set FALSE)
if(DEFINED CMAKE_TOOLCHAIN_FILE)
        set(_vcpkg_toolchain_already_set TRUE)
endif()

if(NOT _vcpkg_toolchain_already_set OR CMAKE_TOOLCHAIN_FILE MATCHES ".*[\\/]Microsoft Visual Studio[\\/].*[\\/]VC[\\/]vcpkg[\\/].*")
        set(_vcpkg_candidate_paths
                "${CMAKE_SOURCE_DIR}/vcpkg"
                "${CMAKE_SOURCE_DIR}/../vcpkg"
                "C:/vcpkg"
        )

        if(DEFINED ENV{VCPKG_ROOT})
                list(APPEND _vcpkg_candidate_paths "$ENV{VCPKG_ROOT}")
        endif() 

        if(DEFINED ENV{VCPKG_INSTALLATION_ROOT})
                list(APPEND _vcpkg_candidate_paths "$ENV{VCPKG_INSTALLATION_ROOT}")
        endif()

        foreach(_vcpkg_root IN LISTS _vcpkg_candidate_paths)
                if(EXISTS "${_vcpkg_root}/scripts/buildsystems/vcpkg.cmake")
                        set(CMAKE_TOOLCHAIN_FILE "${_vcpkg_root}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file" FORCE)
                        message(STATUS "Using vcpkg toolchain: ${CMAKE_TOOLCHAIN_FILE}")
                        break()
                endif()
        endforeach()
endif()

project(ivf)

option(IVF_UI "Build user interface integration libraries." ON) 
option(IVF_UI_FLTK "Build user interface integration libraries." ON)
option(IVF_UI_WIN32 "Build user interface integration libraries." OFF)
option(IVF_UI_GLFW "Build the GLFW integration library." ON)
option(IVF_IMAGE "Build Ivf++ image support." ON)
option(IVF_FONT "Build Ivf++ with font support." OFF)
option(IVF_EXAMPLES "Build Ivf++ examples." ON)
option(IVF_EXAMPLES_GENERIC "Build Ivf++ generic examples" ON)
option(IVF_EXAMPLES_GLE "Build Ivf++ GLE examples" ON)
option(IVF_EXAMPLES_FLTK "Build Ivf++ FLTK examples" ON)
option(IVF_EXAMPLES_WIN32 "Build Ivf++ WIN32 examples" OFF)
option(IVF_EXAMPLES_ASSIMP "Build Ivf++ assimp examples" OFF)
option(IVF_DEBUG "Build Ivf++ with debug information." OFF)
option(IVF_SHARED "Build Ivf++ as shared libraries." OFF)

find_package(PNG REQUIRED)
find_package(JPEG REQUIRED)
find_package(ZLIB REQUIRED)

set(FLTK_SKIP_FLUID TRUE) 
find_package(FLTK CONFIG REQUIRED)

find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)

# GLU is used directly -- gluPerspective, gluLookAt and gluPickMatrix in the
# camera, the quadrics in Cone and Sphere, gluBuild2DMipmaps in Texture, and the
# tessellator throughout the vendored gle library. Nothing here ever linked it
# explicitly; it built only where some other package happened to drag glu32 in,
# and failed with a page of unresolved externals where none did.
#
# It is attached to the ivf and gleivf targets below, so anything linking them
# picks it up. Phases 2-7 replace the GLU calls one by one; this can go when the
# last of them does.

if(TARGET OpenGL::GLU)
	set(IVF_GLU_LIBRARY OpenGL::GLU)
elseif(OPENGL_glu_LIBRARY)
	set(IVF_GLU_LIBRARY ${OPENGL_glu_LIBRARY})
elseif(WIN32)
	set(IVF_GLU_LIBRARY glu32)
else()
	set(IVF_GLU_LIBRARY "")
	message(WARNING "GLU not found -- linking will fail until the remaining GLU calls are replaced.")
endif()

message(STATUS "Ivf++ GLU library: ${IVF_GLU_LIBRARY}")
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)

if(IVF_EXAMPLES_ASSIMP)
find_package(assimp CONFIG REQUIRED)
endif()

set( LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib )
set( EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin )

include_directories( ${PROJECT_SOURCE_DIR}/include/vc ${PROJECT_SOURCE_DIR}/include )
link_directories(${PROJECT_SOURCE_DIR}/lib )

if(WIN32)
	message( "Ivf++ windows build" )
else(WIN32)
	if(APPLE)
		message("Ivf++ Max OS X build")
	else(APPLE)
		message( "Ivf++ linux build")
	endif(APPLE)
endif(WIN32)

if(IVF_SHARED)
	message( "Building shared libraries" )
else(IVF_SHARED)
	message( "Building static libraries." )
endif(IVF_SHARED)

macro(IVFAPP_SETUP)

	file(GLOB SOURCE_FILES *.cpp)
	file(GLOB INCLUDE_FILES *.h) 	

	add_executable(${IVFAPP_NAME} ${SOURCE_FILES} ${INCLUDE_FILES})
	target_link_libraries(${IVFAPP_NAME} PRIVATE 
		${IVFAPP_LIBS}
		PNG::PNG JPEG::JPEG ZLIB::ZLIB glfw GLEW::GLEW OpenGL::GL ${IVF_GLU_LIBRARY}
		glm::glm)
	install(TARGETS ${IVFAPP_NAME} DESTINATION bin)

endmacro(IVFAPP_SETUP)

macro(IVFCONAPP_SETUP)
        if(WIN32)
                include_directories(${PROJECT_SOURCE_DIR}/include)
        else()
                include_directories(${PROJECT_SOURCE_DIR}/include ${FREETYPE_INCLUDE})
        endif()

        file(GLOB SOURCE_FILES *.cpp)
        file(GLOB INCLUDE_FILES *.h)

        set(EXT_LIBS "")
        set(EXT_DEBUG_LIBS "")

        if(IVF_SHARED)
                if(WIN32)
                        add_definitions(-DIVF_DLL -DFL_DLL)
                endif()
        else()
                if(WIN32)
                        set(EXT_DEBUG_LIBS ${FLTK_BASE_LIBRARY} ${FLTK_GL_LIBRARY} ${GDIPLUS_LIBRARY})
                        set(EXT_LIBS ${FLTK_BASE_LIBRARY} ${FLTK_GL_LIBRARY} ${GDIPLUS_LIBRARY})
                else()
                        set(EXT_LIBS)
                endif()
        endif()

        add_executable(${IVFAPP_NAME} ${SOURCE_FILES} ${INCLUDE_FILES})
        target_link_libraries(${IVFAPP_NAME} ${IVFAPP_LIBS} ${EXT_LIBS})
        install(TARGETS ${IVFAPP_NAME} DESTINATION bin)

endmacro(IVFCONAPP_SETUP)

if (IVF_EXAMPLES)
    add_subdirectory(src)
    add_subdirectory(include)
    add_subdirectory(examples)
    add_subdirectory(utils)
else()
    add_subdirectory(src)
    add_subdirectory(include)
    add_subdirectory(utils)
endif()

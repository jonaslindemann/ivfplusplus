cmake_minimum_required(VERSION 3.20)

# Auto-detect vcpkg if not already configured
# Prefers user-installed vcpkg over Visual Studio's built-in vcpkg
if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    if(DEFINED ENV{VCPKG_ROOT})
        set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
        message(STATUS "Using vcpkg from VCPKG_ROOT environment variable: $ENV{VCPKG_ROOT}")
    elseif(EXISTS "c:/vcpkg/scripts/buildsystems/vcpkg.cmake")
        set(CMAKE_TOOLCHAIN_FILE "c:/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
        message(STATUS "Using vcpkg from default Windows location: c:/vcpkg")
    elseif(EXISTS "/home/${USER}/vcpkg/scripts/buildsystems/vcpkg.cmake")
        set(CMAKE_TOOLCHAIN_FILE "/home/${USER}/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
        message(STATUS "Using vcpkg from default Linux location: /home/${USER}/vcpkg")
    elseif(EXISTS "$ENV{HOME}/vcpkg/scripts/buildsystems/vcpkg.cmake")
        set(CMAKE_TOOLCHAIN_FILE "$ENV{HOME}/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
        message(STATUS "Using vcpkg from home directory: $ENV{HOME}/vcpkg")
    else()
        message(WARNING "vcpkg not found in standard locations. CMake will search for packages using system defaults.")
        message(WARNING "For best results, install vcpkg or set VCPKG_ROOT environment variable.")
    endif()
endif()

project(ivf)

option(IVF_UI "Build user interface integration libraries." ON) 
option(IVF_UI_FLTK "Build user interface integration libraries." ON)
option(IVF_UI_WIN32 "Build user interface integration libraries." OFF)
option(IVF_UI_GLUT "Build user interface integration libraries." ON)
option(IVF_IMAGE "Build Ivf++ image support." ON)
option(IVF_FONT "Build Ivf++ with font support." OFF)
option(IVF_EXAMPLES "Build Ivf++ examples." ON)
option(IVF_EXAMPLES_GENERIC "Build Ivf++ generic examples" ON)
option(IVF_EXAMPLES_GLE "Build Ivf++ GLE examples" ON)
option(IVF_EXAMPLES_FLTK "Build Ivf++ FLTK examples" ON)
option(IVF_EXAMPLES_WIN32 "Build Ivf++ WIN32 examples" OFF)
option(IVF_EXAMPLES_FONTS "Build Ivf++ font support examples" OFF)
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
find_package(FreeGLUT CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)

if(IVF_EXAMPLES_ASSIMP)
find_package(assimp CONFIG REQUIRED)
endif()

set( LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib )
set( EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin )

include_directories( ${PROJECT_SOURCE_DIR}/include/vc ${PROJECT_SOURCE_DIR}/include )
link_directories(${PROJECT_SOURCE_DIR}/lib )

if(WIN32)
	message( "-- Ivf++ windows build" )
else(WIN32)
	if(APPLE)
		message("-- Ivf++ Max OS X build")
	else(APPLE)
		message("-- Ivf++ linux build")
	endif(APPLE)
endif(WIN32)

if(IVF_SHARED)
	message( "-- Building shared libraries" )
else(IVF_SHARED)
	message( "-- Building static libraries." )
endif(IVF_SHARED)

macro(IVFAPP_SETUP)

	file(GLOB SOURCE_FILES *.cpp)
	file(GLOB INCLUDE_FILES *.h) 	

	add_executable(${IVFAPP_NAME} ${SOURCE_FILES} ${INCLUDE_FILES})
	target_link_libraries(${IVFAPP_NAME} PRIVATE 
		${IVFAPP_LIBS}
		PNG::PNG JPEG::JPEG ZLIB::ZLIB glfw GLEW::GLEW OpenGL::GL FreeGLUT::freeglut)
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

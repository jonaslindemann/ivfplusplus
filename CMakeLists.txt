cmake_minimum_required(VERSION 3.20)

# Auto-detect vcpkg toolchain file, preferring standalone installations over VS bundled vcpkg
# Check if Visual Studio's bundled vcpkg is being used and override with local installations
set(_VS_VCPKG_DETECTED FALSE)
if(DEFINED CMAKE_TOOLCHAIN_FILE)
    string(FIND "${CMAKE_TOOLCHAIN_FILE}" "Microsoft Visual Studio" _VS_VCPKG_POS)
    if(NOT _VS_VCPKG_POS EQUAL -1)
        set(_VS_VCPKG_DETECTED TRUE)
        message(STATUS "Visual Studio bundled vcpkg detected, checking for local installations...")
    endif()
endif() 

if(NOT DEFINED CMAKE_TOOLCHAIN_FILE OR _VS_VCPKG_DETECTED)
    # Check for VCPKG_ROOT environment variable first
    if(DEFINED ENV{VCPKG_ROOT})
        set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" 
            CACHE STRING "Vcpkg toolchain file" FORCE)
        message(STATUS "Using vcpkg from VCPKG_ROOT: $ENV{VCPKG_ROOT}")
    # Try common vcpkg installation locations
    elseif(WIN32)
        if(EXISTS "c:/vcpkg/scripts/buildsystems/vcpkg.cmake")
            set(CMAKE_TOOLCHAIN_FILE "c:/vcpkg/scripts/buildsystems/vcpkg.cmake" 
                CACHE STRING "Vcpkg toolchain file" FORCE)
            message(STATUS "Found and using vcpkg at: c:/vcpkg")
        elseif(EXISTS "e:/vcpkg/scripts/buildsystems/vcpkg.cmake")
            set(CMAKE_TOOLCHAIN_FILE "e:/vcpkg/scripts/buildsystems/vcpkg.cmake" 
                CACHE STRING "Vcpkg toolchain file" FORCE)
            message(STATUS "Found and using vcpkg at: e:/vcpkg")
        else()
            message(FATAL_ERROR "vcpkg not found. Please install vcpkg at c:/vcpkg or e:/vcpkg, or set VCPKG_ROOT environment variable.\n"
                    "To install vcpkg:\n"
                    "  git clone https://github.com/Microsoft/vcpkg.git c:\\vcpkg\n"
                    "  cd c:\\vcpkg\n"
                    "  .\\bootstrap-vcpkg.bat\n"
                    "  set VCPKG_ROOT=c:\\vcpkg")
        endif()
    elseif(UNIX AND NOT APPLE)
        if(EXISTS "$ENV{HOME}/vcpkg/scripts/buildsystems/vcpkg.cmake")
            set(CMAKE_TOOLCHAIN_FILE "$ENV{HOME}/vcpkg/scripts/buildsystems/vcpkg.cmake" 
                CACHE STRING "Vcpkg toolchain file" FORCE)
            message(STATUS "Found and using vcpkg at: $ENV{HOME}/vcpkg")
        elseif(EXISTS "/usr/local/vcpkg/scripts/buildsystems/vcpkg.cmake")
            set(CMAKE_TOOLCHAIN_FILE "/usr/local/vcpkg/scripts/buildsystems/vcpkg.cmake" 
                CACHE STRING "Vcpkg toolchain file" FORCE)
            message(STATUS "Found and using vcpkg at: /usr/local/vcpkg")
        else()
            message(FATAL_ERROR "vcpkg not found. Please install vcpkg at ~/vcpkg or /usr/local/vcpkg, or set VCPKG_ROOT environment variable.")
        endif()
    elseif(APPLE)
        if(EXISTS "$ENV{HOME}/vcpkg/scripts/buildsystems/vcpkg.cmake")
            set(CMAKE_TOOLCHAIN_FILE "$ENV{HOME}/vcpkg/scripts/buildsystems/vcpkg.cmake" 
                CACHE STRING "Vcpkg toolchain file" FORCE)
            message(STATUS "Found and using vcpkg at: $ENV{HOME}/vcpkg")
        elseif(EXISTS "/usr/local/vcpkg/scripts/buildsystems/vcpkg.cmake")
            set(CMAKE_TOOLCHAIN_FILE "/usr/local/vcpkg/scripts/buildsystems/vcpkg.cmake" 
                CACHE STRING "Vcpkg toolchain file" FORCE)
            message(STATUS "Found and using vcpkg at: /usr/local/vcpkg")
        else()
            message(FATAL_ERROR "vcpkg not found. Please install vcpkg at ~/vcpkg or /usr/local/vcpkg, or set VCPKG_ROOT environment variable.")
        endif()
    endif()
endif()

# Share vcpkg installed packages across all build directories to save disk space
# This puts vcpkg_installed/ in the project root instead of each build directory
set(VCPKG_INSTALLED_DIR "${CMAKE_SOURCE_DIR}/vcpkg_installed" CACHE PATH "vcpkg installed directory")

project(ivf)

option(IVF_UI "Build user interface integration libraries." ON) 
option(IVF_UI_FLTK "Build user interface integration libraries." OFF)
option(IVF_UI_WIN32 "Build user interface integration libraries." OFF)
option(IVF_UI_GLUT "Build user interface integration libraries." ON)
option(IVF_IMAGE "Build Ivf++ image support." ON)
option(IVF_FONT "Build Ivf++ with font support." OFF)
option(IVF_EXAMPLES "Build Ivf++ examples." ON)
option(IVF_EXAMPLES_GENERIC "Build Ivf++ generic examples" ON)
option(IVF_EXAMPLES_GLE "Build Ivf++ GLE examples" ON)
option(IVF_EXAMPLES_FLTK "Build Ivf++ FLTK examples" OFF)
option(IVF_EXAMPLES_WIN32 "Build Ivf++ WIN32 examples" OFF)
option(IVF_EXAMPLES_FONTS "Build Ivf++ font support examples" OFF)
option(IVF_EXAMPLES_ASSIMP "Build Ivf++ assimp examples" OFF)
option(IVF_DEBUG "Build Ivf++ with debug information." OFF)
option(IVF_SHARED "Build Ivf++ as shared libraries." OFF)

# Enable automatic debug postfix for all targets
set(CMAKE_DEBUG_POSTFIX d)

find_package(PNG REQUIRED)
find_package(JPEG REQUIRED)
find_package(ZLIB REQUIRED)

if(IVF_UI_FLTK OR IVF_EXAMPLES_FLTK)
    set(FLTK_SKIP_FLUID TRUE) 
    find_package(FLTK CONFIG REQUIRED)
endif()

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

if( CMAKE_BUILD_TYPE STREQUAL "Debug" )
    set(IVF_DEBUG on)
else( CMAKE_BUILD_TYPE STREQUAL "Debug" )
	set(IVF_DEBUG off)
endif( CMAKE_BUILD_TYPE STREQUAL "Debug" )

if(WIN32)
	message( "Ivf++ windows build" )
else(WIN32)
	if(APPLE)
		message("Ivf++ Max OS X build")
	else(APPLE)
		message( "Ivf++ linux build")
	endif(APPLE)
endif(WIN32)

if(IVF_DEBUG)
	message( "Building with debug information." )
else(IVF_DEBUG)
	message( "Building optimised version without debug information." )
endif(IVF_DEBUG)

if(IVF_SHARED)
	message( "Building shared libraries" )
else(IVF_SHARED)
	message( "Building static libraries." )
endif(IVF_SHARED)

if(WIN32)
    if(IVF_DEBUG)
		string(REPLACE "/zi" "/z7" CMAKE_CXX_FLAGS_DEBUG ${CMAKE_CXX_FLAGS_DEBUG})
		string(REPLACE "/zi" "/z7" CMAKE_C_FLAGS_DEBUG ${CMAKE_C_FLAGS_DEBUG})
    endif(IVF_DEBUG)
endif(WIN32)

macro(IVFAPP_SETUP)

	file(GLOB SOURCE_FILES *.cpp)
	file(GLOB INCLUDE_FILES *.h) 	

        if(IVF_DEBUG)
                add_executable(${IVFAPP_NAME}d ${SOURCE_FILES} ${INCLUDE_FILES})
                target_link_libraries(${IVFAPP_NAME}d PRIVATE ${IVFAPP_DEBUG_LIBS} PRIVATE PNG::PNG JPEG::JPEG ZLIB::ZLIB glfw GLEW::GLEW OpenGL::GL FreeGLUT::freeglut glfw)
                install_targets(/bin ${IVFAPP_NAME}d)
        else(IVF_DEBUG)
                add_executable(${IVFAPP_NAME} ${SOURCE_FILES} ${INCLUDE_FILES})
                target_link_libraries(${IVFAPP_NAME} PRIVATE ${IVFAPP_LIBS} PNG::PNG JPEG::JPEG ZLIB::ZLIB glfw GLEW::GLEW OpenGL::GL FreeGLUT::freeglut glfw)
                install_targets(/bin ${IVFAPP_NAME})
        endif(IVF_DEBUG)		

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

        if(WIN32)
                if(IVF_DEBUG)
                        add_executable(${IVFAPP_NAME}d ${SOURCE_FILES} ${INCLUDE_FILES})
                        target_link_libraries( ${IVFAPP_NAME}d ${IVFAPP_DEBUG_LIBS} ${EXT_DEBUG_LIBS})
                        install_targets( /bin ${IVFAPP_NAME}d )
                else()
                        add_executable(${IVFAPP_NAME} ${SOURCE_FILES} ${INCLUDE_FILES})
                        target_link_libraries( ${IVFAPP_NAME} ${IVFAPP_LIBS} ${EXT_LIBS})
                        install_targets( /bin ${IVFAPP_NAME} )
                endif()
        else()
                add_executable(${IVFAPP_NAME} ${SOURCE_FILES})
                target_link_libraries(${IVFAPP_NAME} ${IVFAPP_LIBS} ${EXT_LIBS})
                install_targets( /bin ${IVFAPP_NAME} )
        endif()

endmacro(IVFCONAPP_SETUP)

add_subdirectory(src)
add_subdirectory(include)

# Create unified ivfplusplus interface library target
# This bundles all ivf libraries and dependencies for easier linking
add_library(ivfplusplus INTERFACE)
add_library(ivfplusplus::ivfplusplus ALIAS ivfplusplus)

target_link_libraries(ivfplusplus INTERFACE 
    ivf 
    ivfmath 
    ivfctl 
    ivffile 
    ivf3dui 
    ivfwidget 
    ivfgle 
    ivfglut 
    ivffltk
    glad 
    PNG::PNG 
    JPEG::JPEG 
    ZLIB::ZLIB 
    glfw 
    GLEW::GLEW 
    OpenGL::GL 
    FreeGLUT::freeglut
)

if(IVF_IMAGE)
    target_link_libraries(ivfplusplus INTERFACE ivfimage ivfext)
endif()

target_include_directories(ivfplusplus INTERFACE 
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/include/vc
)

if (IVF_EXAMPLES)
    add_subdirectory(examples)
endif()

add_subdirectory(utils)

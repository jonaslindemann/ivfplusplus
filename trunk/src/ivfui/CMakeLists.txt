INCLUDE_DIRECTORIES(${ivf_SOURCE_DIR}/include)
FILE(GLOB SOURCE_FILES *.cpp)
FILE(GLOB INCLUDE_FILES ${ivf_SOURCE_DIR}/include/ivfui/*.h)

IF(BUILD_SHARED_LIBS)
	IF(WIN32)
		ADD_DEFINITIONS(-DFL_DLL -DIVF_DLL -DIVFUI_LIB)
	ENDIF(WIN32)
ENDIF(BUILD_SHARED_LIBS)

IF(WIN32)
	IF(IVF_DEBUG)
		ADD_LIBRARY(ivfuid ${INCLUDE_FILES} ${SOURCE_FILES})
		TARGET_LINK_LIBRARIES( ivfuid ivfd ivffltkd wsock32 comctl32 )
		INSTALL_TARGETS( /lib ivfuid )
	ELSE(IVF_DEBUG)
		ADD_LIBRARY(ivfui ${INCLUDE_FILES} ${SOURCE_FILES})
		TARGET_LINK_LIBRARIES( ivfui ivf ivffltk wsock32 comctl32 )
		INSTALL_TARGETS( /lib ivfui )
	ENDIF(IVF_DEBUG)
ELSE(WIN32)
	ADD_LIBRARY(ivfui ${SOURCE_FILES})

        IF(APPLE)
                TARGET_LINK_LIBRARIES( ivfui ivf ivffltk "-framework Carbon" "-framework Cocoa" "-framework ApplicationServices" "-framework AGL" "-framework OpenGL" gleivf )
        ELSE(APPLE)
                TARGET_LINK_LIBRARIES( ivfui ivf ivffltk  GL GLU gleivf )
        ENDIF(APPLE)

	INSTALL_TARGETS( /lib ivfui )
ENDIF(WIN32)



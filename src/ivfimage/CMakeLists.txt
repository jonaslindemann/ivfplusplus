INCLUDE_DIRECTORIES(${ivf_SOURCE_DIR}/include)
FILE(GLOB SOURCE_FILES *.cpp)
FILE(GLOB INCLUDE_FILES ${ivf_SOURCE_DIR}/include/ivfimage/*.h)

IF(BUILD_SHARED_LIBS)
	ADD_DEFINITIONS(-DIVF_DLL -DIVFIMAGE_LIB)
ENDIF(BUILD_SHARED_LIBS)

IF(WIN32)
	IF(IVF_DEBUG)
		ADD_LIBRARY(ivfimaged ${INCLUDE_FILES} ${SOURCE_FILES})
		TARGET_LINK_LIBRARIES( ivfimaged ivfd )
		INSTALL_TARGETS( /lib ivfimaged )
	ELSE(IVF_DEBUG)
		ADD_LIBRARY(ivfimage ${INCLUDE_FILES} ${SOURCE_FILES})
		TARGET_LINK_LIBRARIES( ivfimage ivf )
		INSTALL_TARGETS( /lib ivfimage )
	ENDIF(IVF_DEBUG)
ELSE(WIN32)
	ADD_LIBRARY(ivfimage ${SOURCE_FILES})
	TARGET_LINK_LIBRARIES( ivfimage )
	INSTALL_TARGETS( /lib ivfimage )
ENDIF(WIN32)



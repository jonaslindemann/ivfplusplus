INCLUDE_DIRECTORIES(${ivf_SOURCE_DIR}/include)
FILE(GLOB SOURCE_FILES *.cpp)
FILE(GLOB INCLUDE_FILES ${ivf_SOURCE_DIR}/include/ivfctl/*.h)

IF(BUILD_SHARED_LIBS)
	ADD_DEFINITIONS(-DIVF_DLL -DIVFCTL_LIB)
ENDIF(BUILD_SHARED_LIBS)

IF(WIN32)
	IF(IVF_DEBUG)
		ADD_LIBRARY(ivfctld ${INCLUDE_FILES} ${SOURCE_FILES})
		TARGET_LINK_LIBRARIES( ivfctld ivfd )
		INSTALL_TARGETS(/lib ivfctld)
	ELSE(IVF_DEBUG)
		ADD_LIBRARY(ivfctl ${INCLUDE_FILES} ${SOURCE_FILES})
		TARGET_LINK_LIBRARIES( ivfctl ivf )
		INSTALL_TARGETS(/lib ivfctl)
	ENDIF(IVF_DEBUG)
ELSE(WIN32)
	ADD_LIBRARY(ivfctl ${SOURCE_FILES})
	TARGET_LINK_LIBRARIES( ivfctl ivf )
	INSTALL_TARGETS(/lib ivfctl)
ENDIF(WIN32)


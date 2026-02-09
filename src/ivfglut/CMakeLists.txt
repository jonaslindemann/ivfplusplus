include_directories(${ivf_SOURCE_DIR}/include)
file(GLOB SOURCE_FILES *.cpp)
file(GLOB INCLUDE_FILES ${ivf_SOURCE_DIR}/include/ivfglut/*.h)

if(IVF_SHARED)
  if(WIN32)
    add_definitions(-DIVF_DLL)
  endif(WIN32)
endif(IVF_SHARED)

add_library(ivfglut ${INCLUDE_FILES} ${SOURCE_FILES})
target_link_libraries(ivfglut ivf ivfwidget FreeGLUT::freeglut)
set_target_properties(ivfglut PROPERTIES DEBUG_POSTFIX "d")
install(TARGETS ivfglut DESTINATION lib)

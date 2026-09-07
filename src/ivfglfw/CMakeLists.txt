include_directories(${ivf_SOURCE_DIR}/include)
file(GLOB SOURCE_FILES *.cpp)
file(GLOB INCLUDE_FILES ${ivf_SOURCE_DIR}/include/ivfglfw/*.h)

if(IVF_SHARED)
  if(WIN32)
    add_definitions(-DIVF_DLL)
  endif(WIN32)
endif(IVF_SHARED)

add_library(ivfglfw ${INCLUDE_FILES} ${SOURCE_FILES})
target_link_libraries(ivfglfw ivf ivfwidget glfw)
set_target_properties(ivfglfw PROPERTIES DEBUG_POSTFIX "d")
install(TARGETS ivfglfw DESTINATION lib)

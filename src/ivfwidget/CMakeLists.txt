include_directories(${ivf_SOURCE_DIR}/include)
file(GLOB SOURCE_FILES *.cpp)
file(GLOB INCLUDE_FILES ${ivf_SOURCE_DIR}/include/ivfwidget/*.h)

if(BUILD_SHARED_LIBS)
  add_definitions(-DIVF_DLL -DIVFWIDGET_LIB)
endif(BUILD_SHARED_LIBS)

add_library(ivfwidget ${INCLUDE_FILES} ${SOURCE_FILES})
target_link_libraries(ivfwidget ivf ivf3dui)
set_target_properties(ivfwidget PROPERTIES DEBUG_POSTFIX "d")
install(TARGETS ivfwidget DESTINATION lib)

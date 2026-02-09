include_directories(${ivf_SOURCE_DIR}/include)
file(GLOB SOURCE_FILES *.cpp)
file(GLOB INCLUDE_FILES ${ivf_SOURCE_DIR}/include/ivf3dui/*.h)

if(BUILD_SHARED_LIBS)
  add_definitions(-DIVF_DLL -DIVF3DUI_LIB)
endif(BUILD_SHARED_LIBS)

add_library(ivf3dui ${INCLUDE_FILES} ${SOURCE_FILES})
target_link_libraries(ivf3dui ivf ivfmath gleivf)
set_target_properties(ivf3dui PROPERTIES DEBUG_POSTFIX "d")
install(TARGETS ivf3dui DESTINATION lib)

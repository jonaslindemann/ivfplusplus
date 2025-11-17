include_directories(${ivf_SOURCE_DIR}/include)
file(GLOB SOURCE_FILES *.cpp)
file(GLOB INCLUDE_FILES ${ivf_SOURCE_DIR}/include/ivfctl/*.h)

if(BUILD_SHARED_LIBS)
  add_definitions(-DIVF_DLL -DIVFCTL_LIB)
endif(BUILD_SHARED_LIBS)

add_library(ivfctl ${INCLUDE_FILES} ${SOURCE_FILES})
target_link_libraries(ivfctl ivf)
set_target_properties(ivfctl PROPERTIES DEBUG_POSTFIX "d")
install(TARGETS ivfctl DESTINATION lib)

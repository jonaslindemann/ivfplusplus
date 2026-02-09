include_directories(${ivf_SOURCE_DIR}/include)
file(GLOB SOURCE_FILES *.cpp)
file(GLOB INCLUDE_FILES ${ivf_SOURCE_DIR}/include/ivffile/*.h)

if(BUILD_SHARED_LIBS)
  add_definitions(-DIVF_DLL -DIVFFILE_LIB)
endif(BUILD_SHARED_LIBS)

add_library(ivffile ${INCLUDE_FILES} ${SOURCE_FILES})
target_link_libraries(ivffile ivf ivfimage)
set_target_properties(ivffile PROPERTIES DEBUG_POSTFIX "d")
install(TARGETS ivffile DESTINATION lib)

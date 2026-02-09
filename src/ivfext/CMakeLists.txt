include_directories(${ivf_SOURCE_DIR}/include)
file(GLOB SOURCE_FILES *.cpp)
file(GLOB INCLUDE_FILES ${ivf_SOURCE_DIR}/include/ivfext/*.h)

if(BUILD_SHARED_LIBS)
  add_definitions(-DIVF_DLL -DIVFEXT_LIB)
endif(BUILD_SHARED_LIBS)

add_library(ivfext ${INCLUDE_FILES} ${SOURCE_FILES})
target_link_libraries(ivfext ivf ivfimage)
set_target_properties(ivfext PROPERTIES DEBUG_POSTFIX "d")
install(TARGETS ivfext DESTINATION lib)

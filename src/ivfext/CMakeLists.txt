include_directories(${ivf_SOURCE_DIR}/include)
file(GLOB SOURCE_FILES *.cpp)
file(GLOB INCLUDE_FILES ${ivf_SOURCE_DIR}/include/ivfext/*.h)

if(BUILD_SHARED_LIBS)
  add_definitions(-DIVF_DLL -DIVFEXT_LIB)
endif(BUILD_SHARED_LIBS)

if(IVF_DEBUG)
  add_library(ivfextd ${INCLUDE_FILES} ${SOURCE_FILES})
  target_link_libraries(ivfextd ivfd ivfimaged)
  install_targets(/lib ivfextd)
else()
  add_library(ivfext ${INCLUDE_FILES} ${SOURCE_FILES})
  target_link_libraries(ivfext ivf ivfimage)
  install_targets(/lib ivfext)
endif()

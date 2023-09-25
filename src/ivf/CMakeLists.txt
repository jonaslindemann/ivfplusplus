include_directories(${ivf_SOURCE_DIR}/include)
file(GLOB SOURCE_FILES *.cpp)
file(GLOB INCLUDE_FILES ${ivf_SOURCE_DIR}/include/ivf/*.h)

if(BUILD_SHARED_LIBS)
  add_definitions(-DIVF_DLL -DIVF_LIB)
endif(BUILD_SHARED_LIBS)

if(IVF_DEBUG)
  add_library(ivfd ${INCLUDE_FILES} ${SOURCE_FILES})
  target_link_libraries(ivfd ivfmathd gleivfd)
  install_targets(/lib ivfd)
else()
  add_library(ivf ${INCLUDE_FILES} ${SOURCE_FILES})
  target_link_libraries(ivf ivfmath gleivf)
  install_targets(/lib ivf)
endif()

install_files(/include/ivf ../../include/ivf/*.h)

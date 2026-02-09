include_directories(${ivf_SOURCE_DIR}/include)
file(GLOB SOURCE_FILES *.cpp)
file(GLOB INCLUDE_FILES ${ivf_SOURCE_DIR}/include/ivf/*.h)

if(BUILD_SHARED_LIBS)
  add_definitions(-DIVF_DLL -DIVF_LIB)
endif(BUILD_SHARED_LIBS)

add_library(ivf ${INCLUDE_FILES} ${SOURCE_FILES})
target_link_libraries(ivf ivfmath gleivf)
set_target_properties(ivf PROPERTIES DEBUG_POSTFIX "d")
install(TARGETS ivf DESTINATION lib)

install(FILES ${INCLUDE_FILES} DESTINATION include/ivf)

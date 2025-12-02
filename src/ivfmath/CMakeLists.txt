include_directories(${ivf_SOURCE_DIR}/include)
file(GLOB SOURCE_FILES *.cpp)
file(GLOB INCLUDE_FILES ${ivf_SOURCE_DIR}/include/ivfmath/*.h)

add_library(ivfmath ${INCLUDE_FILES} ${SOURCE_FILES})
set_target_properties(ivfmath PROPERTIES
  DEBUG_POSTFIX "d"
  OUTPUT_NAME "ivfmath")
install(TARGETS ivfmath DESTINATION lib)

include_directories(${ivf_SOURCE_DIR}/include)
file(GLOB SOURCE_FILES *.c)
file(GLOB INCLUDE_FILES ${ivf_SOURCE_DIR}/include/GL/*.h)

add_library(gleivf ${INCLUDE_FILES} ${SOURCE_FILES})
set_target_properties(gleivf PROPERTIES
  DEBUG_POSTFIX "d"
  OUTPUT_NAME "gleivf")
install(TARGETS gleivf DESTINATION lib)

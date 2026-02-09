include_directories(${ivf_SOURCE_DIR}/include)
file(GLOB SOURCE_FILES *.c)
file(GLOB INCLUDE_FILES ${ivf_SOURCE_DIR}/include/glad/*.h)

add_library(glad ${INCLUDE_FILES} ${SOURCE_FILES})
set_target_properties(glad PROPERTIES
  DEBUG_POSTFIX "d"
  OUTPUT_NAME "glad")
install(TARGETS glad DESTINATION lib)

message("INSTALL GLAD files...")
install(FILES ${INCLUDE_FILES} DESTINATION include/glad)

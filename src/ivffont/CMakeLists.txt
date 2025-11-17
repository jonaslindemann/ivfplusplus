if(WIN32)
  include_directories(${ivf_SOURCE_DIR}/include)
else(WIN32)
  include_directories(${ivf_SOURCE_DIR}/include ${FREETYPE_INCLUDE}
                      ${FREETYPE_CONFIG})
  link_directories(${FREETYPE_LIBS})
endif(WIN32)

file(GLOB SOURCE_FILES *.cpp)
file(GLOB INCLUDE_FILES ${ivf_SOURCE_DIR}/include/ivffont/*.h)

add_library(ivffont ${INCLUDE_FILES} ${SOURCE_FILES})
target_link_libraries(ivffont ivf $<$<CONFIG:Debug>:${FONT_DEBUG_LIBS}> $<$<NOT:$<CONFIG:Debug>>:${FONT_LIBS}>)
set_target_properties(ivffont PROPERTIES DEBUG_POSTFIX "d")
install(TARGETS ivffont DESTINATION lib)

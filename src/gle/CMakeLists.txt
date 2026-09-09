include_directories(${ivf_SOURCE_DIR}/include)
file(GLOB SOURCE_FILES CONFIGURE_DEPENDS *.c)
file(GLOB INCLUDE_FILES ${ivf_SOURCE_DIR}/include/GL/*.h )

add_library(gleivf ${INCLUDE_FILES} ${SOURCE_FILES})
target_link_libraries(gleivf OpenGL::GL ${IVF_GLU_LIBRARY})
set_target_properties(gleivf PROPERTIES
  DEBUG_POSTFIX "d"
  OUTPUT_NAME "gleivf")

# Automatic texture coordinate generation costs a callback test per emitted
# vertex and normal, paid whether or not the feature is in use. Turn this off
# when nothing calls Extrusion::setTextureMode() with a non-zero mode.
# See src/gle/config.h.
option(GLE_AUTO_TEXTURE "Enable GLE automatic texture coordinate generation" ON)

if(NOT GLE_AUTO_TEXTURE)
  target_compile_definitions(gleivf PRIVATE IVF_GLE_NO_AUTO_TEXTURE)
endif()

install(TARGETS gleivf DESTINATION lib)

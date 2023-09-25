include_directories(${ivf_SOURCE_DIR}/include)
file(GLOB SOURCE_FILES *.c)
file(GLOB INCLUDE_FILES ${ivf_SOURCE_DIR}/include/glad/*.h)

if(IVF_DEBUG)
  add_library(gladd ${INCLUDE_FILES} ${SOURCE_FILES})
  install_targets(/lib gladd)
else()
  add_library(glad ${INCLUDE_FILES} ${SOURCE_FILES})
  install_targets(/lib glad)
endif()

message("INSTALL GLAD files...")
install_files(/include/glad ${ivf_SOURCE_DIR}/include/glad/*.h)

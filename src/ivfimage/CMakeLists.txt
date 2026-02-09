include_directories(${ivf_SOURCE_DIR}/include)
file(GLOB SOURCE_FILES *.cpp)
file(GLOB INCLUDE_FILES ${ivf_SOURCE_DIR}/include/ivfimage/*.h)

add_library(ivfimage ${INCLUDE_FILES} ${SOURCE_FILES})
target_link_libraries(ivfimage ivf PNG::PNG JPEG::JPEG ZLIB::ZLIB)
set_target_properties(ivfimage PROPERTIES DEBUG_POSTFIX "d")
install(TARGETS ivfimage DESTINATION lib)

include_directories(${ivf_SOURCE_DIR}/include)
file(GLOB SOURCE_FILES *.cpp)
file(GLOB INCLUDE_FILES ${ivf_SOURCE_DIR}/include/ivfui/*.h)

if(BUILD_SHARED_LIBS)
  if(WIN32)
    add_definitions(-DFL_DLL -DIVF_DLL -DIVFUI_LIB)
  endif(WIN32)
endif(BUILD_SHARED_LIBS)

add_library(ivfui ${INCLUDE_FILES} ${SOURCE_FILES})
target_link_libraries(ivfui ivf ivffltk wsock32 comctl32)
set_target_properties(ivfui PROPERTIES DEBUG_POSTFIX "d")
install(TARGETS ivfui DESTINATION lib)

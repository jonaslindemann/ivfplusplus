include_directories(${ivf_SOURCE_DIR}/include)
file(GLOB SOURCE_FILES *.cpp)
file(GLOB INCLUDE_FILES ${ivf_SOURCE_DIR}/include/ivffltk/*.h)

if(BUILD_SHARED_LIBS)
  if(WIN32)
    add_definitions(-DFL_DLL -DIVF_DLL -DIVFFLTK_LIB)
  endif(WIN32)
  set(FLTKLIBS_DEBUG fltkdlld)
  set(FLTKLIBS_RELEASE fltkdll)
else(BUILD_SHARED_LIBS)
  set(IVFLIBS_DEBUG fltkd fltkgld)
  set(IVFLIBS_RELEASE fltk fltkgl)
endif(BUILD_SHARED_LIBS)

if(IVF_DEBUG)
  add_library(ivffltkd ${INCLUDE_FILES} ${SOURCE_FILES})
  target_link_libraries(
    ivffltkd
    ivfd
    ivfwidgetd
    fltk
    fltk_gl
    fltk_forms
    fltk_images)
  install_targets(/lib ivffltkd)
else(IVF_DEBUG)
  add_library(ivffltk ${INCLUDE_FILES} ${SOURCE_FILES})
  target_link_libraries(
    ivffltk
    ivf
    ivfwidget
    fltk
    fltk_gl
    fltk_forms
    fltk_images)
  install_targets(/lib ivffltk)
endif(IVF_DEBUG)

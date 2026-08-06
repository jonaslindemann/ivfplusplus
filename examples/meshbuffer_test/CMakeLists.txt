file(GLOB SOURCE_FILES *.cpp)
file(GLOB INCLUDE_FILES *.h)

SET(IVFAPP_NAME meshbuffer_test)
SET(IVFAPP_LIBS ivf ivfmath glad)
SET(IVFAPP_DEBUG_LIBS ivfd ivfmathd gladd)

IVFAPP_SETUP()

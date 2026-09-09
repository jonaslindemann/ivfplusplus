file(GLOB SOURCE_FILES *.cpp)
file(GLOB INCLUDE_FILES *.h)

SET(IVFAPP_NAME profile_test)
SET(IVFAPP_LIBS ivf ivfmath ivfgle ivfimage gleivf glad)
SET(IVFAPP_DEBUG_LIBS ivfd ivfmathd ivfgled ivfimaged gleivfd gladd)

IVFAPP_SETUP()

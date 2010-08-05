file(GLOB SOURCE_FILES *.cpp)
file(GLOB INCLUDE_FILES *.h) 	


SET(IVFAPP_NAME cube)
SET(IVFAPP_LIBS ivf ivfui)
SET(IVFAPP_DEBUG_LIBS ivfd ivfuid)

IVFAPP_SETUP()


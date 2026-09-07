file(GLOB SOURCE_FILES *.cpp)
file(GLOB INCLUDE_FILES *.h) 	


SET(IVFAPP_NAME cube)
SET(IVFAPP_LIBS ivf ivfglfw glad)
SET(IVFAPP_DEBUG_LIBS ivfd ivfglfwd gladd)

IVFAPP_SETUP()


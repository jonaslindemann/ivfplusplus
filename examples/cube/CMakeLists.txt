file(GLOB SOURCE_FILES *.cpp)
file(GLOB INCLUDE_FILES *.h) 	


SET(IVFAPP_NAME cube)
SET(IVFAPP_LIBS ivf ivfglut glad)
SET(IVFAPP_DEBUG_LIBS ivfd ivfglutd gladd)

IVFAPP_SETUP()


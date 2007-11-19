IF(IVFWIN32)
	SET(IVFAPP_NAME fibreviewc )
	SET(IVFAPP_LIBS ivf ivfwin32 ivffile )
	SET(IVFAPP_DEBUG_LIBS ivfd ivfwin32d ivffiled )

	IVFAPP_SETUP()
ENDIF(IVFWIN32)

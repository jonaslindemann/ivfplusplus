IF(NOT IVFWIN32)
	FILE(GLOB INCLUDE_FILES *.h)
	INSTALL_FILES(/include/ivffont FILES ${INCLUDE_FILES})
ENDIF(NOT IVFWIN32)


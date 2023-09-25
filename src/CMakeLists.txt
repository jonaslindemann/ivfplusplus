add_subdirectory(glad)
add_subdirectory(gle)
add_subdirectory(ivfmath)
add_subdirectory(ivf)
add_subdirectory(ivffile)
add_subdirectory(ivfctl)
add_subdirectory(ivf3dui)
add_subdirectory(ivfwidget)

if (IVF_UI)

	if (IVF_UI_FLTK)
		add_subdirectory(ivffltk)
	endif (IVF_UI_FLTK)
	
	if (IVF_UI_WIN32)
		add_subdirectory(ivfwin32)
	endif (IVF_UI_WIN32)

	if (IVF_UI_GLUT)
		add_subdirectory(ivfglut)
	endif (IVF_UI_GLUT)

endif(IVF_UI)

add_subdirectory(ivfgle)

if (IVF_IMAGE)
	add_subdirectory(ivfimage)
	add_subdirectory(ivfext)
endif(IVF_IMAGE)

if (IVF_FONT)
	add_subdirectory(ivffont)
endif(IVF_FONT)
set(IVF_DIRS ivfmath ivf ivffile ivfctl ivf3dui ivfwidget)

if (IVF_UI)

	if (IVF_UI_FLTK)
		set(IVF_DIRS ${IVF_DIRS} ivffltk)
	endif (IVF_UI_FLTK)
	
	if (IVF_UI_WIN32)
		set(IVF_DIRS ${IVF_DIRS} ivfwin32)
	endif (IVF_UI_WIN32)

	if (IVF_UI_GLUT)
		set(IVF_DIRS ${IVF_DIRS} ivfglut)
	endif (IVF_UI_GLUT)

endif(IVF_UI)

if (IVF_GLE)
	set(IVF_DIRS ${IVF_DIRS} ivfgle)
endif(IVF_GLE)

if (IVF_IMAGE)
	set(IVF_DIRS ${IVF_DIRS} ivfimage ivfext)
endif(IVF_IMAGE)

if (IVF_FONT)
	set(IVF_DIRS ${IVF_DIRS} ivffont)
endif(IVF_FONT)

subdirs(${IVF_DIRS})
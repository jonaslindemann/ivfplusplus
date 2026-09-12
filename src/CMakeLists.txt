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

	if (IVF_UI_GLFW)
		add_subdirectory(ivfglfw)
	endif (IVF_UI_GLFW)

endif(IVF_UI)

add_subdirectory(ivfgle)

if (IVF_IMAGE)
	add_subdirectory(ivfimage)
	add_subdirectory(ivfext)
endif(IVF_IMAGE)

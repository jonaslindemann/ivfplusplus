if (IVF_EXAMPLES)

	if (IVF_EXAMPLES_GENERIC)
	
		add_subdirectory(cube)
		add_subdirectory(handlers)
		add_subdirectory(ac3dreader)
		add_subdirectory(actionctl)
		add_subdirectory(advgeom)
		add_subdirectory(culling)
		add_subdirectory(controllers)
		add_subdirectory(fly)
		add_subdirectory(lighting)
		add_subdirectory(lod)
		add_subdirectory(manip)
		add_subdirectory(math)
		add_subdirectory(overlay)
		add_subdirectory(placement)
		add_subdirectory(robot)
		add_subdirectory(robot2)
		add_subdirectory(robot_events)
		add_subdirectory(selection)
		add_subdirectory(bmfonts)
		add_subdirectory(texture)
		add_subdirectory(viewports)
		add_subdirectory(imui)
		add_subdirectory(objframe2)
		add_subdirectory(fibrescope)
		add_subdirectory(gleextrusion)
	
	endif (IVF_EXAMPLES_GENERIC)
	
	if (IVF_EXAMPLES_ASSIMP)
	
		add_subdirectory(assimp)
		
	endif (IVF_EXAMPLES_ASSIMP)
	
	if (IVF_EXAMPLES_FONTS)
	
		add_subdirectory(fonts)
		
	endif (IVF_EXAMPLES_FONTS)

endif (IVF_EXAMPLES)

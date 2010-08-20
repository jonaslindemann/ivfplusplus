if (IVF_EXAMPLES)

	if (IVF_EXAMPLES_GENERIC)
	
		set(IVF_EXAMPLE_DIRS ${IVF_EXAMPLE_DIRS} cube handlers ac3dreader actionctl advgeom culling
		controllers fly lighting lod manip math overlay placement robot robot2 robot_events selection
		texture viewports imui) 
	
	endif (IVF_EXAMPLES_GENERIC)
	
	subdirs(${IVF_EXAMPLE_DIRS})

endif (IVF_EXAMPLES)

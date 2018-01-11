if (IVF_EXAMPLES)

	if (IVF_EXAMPLES_GENERIC)
	
		set(IVF_EXAMPLE_DIRS cube handlers ac3dreader actionctl advgeom culling
		controllers fly lighting lod manip math overlay placement robot robot2 robot_events selection
		texture viewports imui objframe2 fibrescope gleextrusion ) 
	
	endif (IVF_EXAMPLES_GENERIC)
	
	if (IVF_EXAMPLES_ASSIMP)
	
		set(IVF_EXAMPLE_DIRS ${IVF_EXAMPLE_DIRS} assimp)
		
	endif (IVF_EXAMPLES_ASSIMP)
	
	if (IVF_EXAMPLES_FONTS)
	
		set(IVF_EXAMPLE_DIRS ${IVF_EXAMPLE_DIRS} fonts)
		
	endif (IVF_EXAMPLES_FONTS)

	subdirs(${IVF_EXAMPLE_DIRS})

endif (IVF_EXAMPLES)

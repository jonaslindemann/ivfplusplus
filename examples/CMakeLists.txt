# Helper function to create ivfplusplus examples with simplified dependency management
function(add_ivfplusplus_example name)
    cmake_parse_arguments(ARG "" "" "SOURCES;LIBS;DEBUG_LIBS" ${ARGN})
    
    if(NOT ARG_SOURCES)
        set(ARG_SOURCES ${name}.cpp)
    endif()
    
    file(GLOB HEADER_FILES *.h)
    
    if(IVF_DEBUG)
        add_executable(${name}d ${ARG_SOURCES} ${HEADER_FILES})
        target_link_libraries(${name}d PRIVATE ${ARG_DEBUG_LIBS})
        set_target_properties(${name}d PROPERTIES 
            FOLDER "examples"
        )
    else()
        add_executable(${name} ${ARG_SOURCES} ${HEADER_FILES})
        target_link_libraries(${name} PRIVATE ${ARG_LIBS})
        set_target_properties(${name} PROPERTIES 
            FOLDER "examples"
        )
    endif()
endfunction()

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
	if (IVF_EXAMPLES_FLTK)
		add_subdirectory(objframe2)
		add_subdirectory(fibrescope)
	endif (IVF_EXAMPLES_FLTK)
		add_subdirectory(gleextrusion)
	
	endif (IVF_EXAMPLES_GENERIC)
	
	if (IVF_EXAMPLES_ASSIMP)
	
		add_subdirectory(assimp)
		
	endif (IVF_EXAMPLES_ASSIMP)
	
	if (IVF_EXAMPLES_FONTS)
	
		add_subdirectory(fonts)
		
	endif (IVF_EXAMPLES_FONTS)

endif (IVF_EXAMPLES)

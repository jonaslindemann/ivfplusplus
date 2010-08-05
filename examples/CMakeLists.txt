if (USE_GLE)
SUBDIRS(cube handlers objframe2 ac3dreader actionctl advgeom culling controllers extrusion fltkrobot fly lighting lod manip math overlay placement robot robot2 robot_events scene selection texture viewports fibrescope fibreviewc fonts gleextrusion glut) 
else (USE_GLE)
SUBDIRS(cube handlers objframe2 ac3dreader actionctl advgeom culling controllers extrusion fltkrobot fly lighting lod manip math overlay placement robot robot2 robot_events scene selection texture viewports fibrescope fibreviewc fonts glut) 
endif (USE_GLE)


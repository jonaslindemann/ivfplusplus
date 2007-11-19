Ivf++ Examples
--------------

To build the examples in the binary Ivf++ distribution do the following procedure:

  1. Open the Application generator
  2. Make sure the Ivf++ directories point to the correct install points.
  3. Select example dir as the "Target dir"
  4. Change the "Application name" to the name of the example.
  5. Click "Generate".

The application generator will now add the neccesary build files to the example directory. The example can then be configured:

C:\...>configure
C:\...>build

A separate build dir will be created win32_build_[xxx] containing the project files or makefiles.

If no settings are changed the default build is a IDE-based build. The build command will open the default IDE with the created project files.
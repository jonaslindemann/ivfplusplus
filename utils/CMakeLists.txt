# Python utilities are optional and not required for building the library
# These are legacy tools for generating boilerplate code:
# - ivfappgen.py: Generate application templates
# - ivfclassgen.py: Generate class boilerplate
# - update_license.py: Update license headers
# 
# Use them if needed: python utils/ivfappgen.py

IF(NOT IVFWIN32)
	# Optional: Install shell scripts for Unix-like systems
	INSTALL(FILES ivfappgen ivfclassgen DESTINATION bin PERMISSIONS OWNER_WRITE OWNER_READ GROUP_READ WORLD_READ OWNER_EXECUTE GROUP_EXECUTE WORLD_EXECUTE)
ENDIF(NOT IVFWIN32)


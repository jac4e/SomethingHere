#----------------------------------------------------------------
# Generated CMake target import file.
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "FreeGLUT::freeglut" for configuration ""
set_property(TARGET FreeGLUT::freeglut APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(FreeGLUT::freeglut PROPERTIES
  IMPORTED_LOCATION_NOCONFIG "${_IMPORT_PREFIX}/lib64/libglut.so.3.11.0"
  IMPORTED_SONAME_NOCONFIG "libglut.so.3"
  )

list(APPEND _IMPORT_CHECK_TARGETS FreeGLUT::freeglut )
list(APPEND _IMPORT_CHECK_FILES_FOR_FreeGLUT::freeglut "${_IMPORT_PREFIX}/lib64/libglut.so.3.11.0" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)

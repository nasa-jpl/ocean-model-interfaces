#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "ocean_models" for configuration "Debug"
set_property(TARGET ocean_models APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(ocean_models PROPERTIES
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/lib/libocean_models.0.1.dylib"
  IMPORTED_SONAME_DEBUG "@rpath/libocean_models.0.1.dylib"
  )

list(APPEND _IMPORT_CHECK_TARGETS ocean_models )
list(APPEND _IMPORT_CHECK_FILES_FOR_ocean_models "${_IMPORT_PREFIX}/lib/libocean_models.0.1.dylib" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)

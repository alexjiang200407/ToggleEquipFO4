#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "mmio::mmio" for configuration "Release"
set_property(TARGET mmio::mmio APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(mmio::mmio PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/mmio.lib"
  )

list(APPEND _cmake_import_check_targets mmio::mmio )
list(APPEND _cmake_import_check_files_for_mmio::mmio "${_IMPORT_PREFIX}/lib/mmio.lib" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)

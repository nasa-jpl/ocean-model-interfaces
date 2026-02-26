#Sets the following variables
# netCDFCxx_INSTALL_PREFIX
# netCDFCxx_INCLUDE_DIR
# netCDFCxx_LIB_DIR
# netCDFCxx_LIBRARIES

execute_process(COMMAND ncxx4-config --prefix OUTPUT_VARIABLE netCDFCxx_INSTALL_PREFIX)
execute_process(COMMAND ncxx4-config --includedir OUTPUT_VARIABLE netCDFCxx_INCLUDE_DIR)

string(REGEX REPLACE "\n$" "" netCDFCxx_INSTALL_PREFIX "${netCDFCxx_INSTALL_PREFIX}")
string(REGEX REPLACE "\n$" "" netCDFCxx_INCLUDE_DIR "${netCDFCxx_INCLUDE_DIR}")

find_library(netCDFCxx_LIBRARIES 
		NAMES netcdf-cxx4 netcdf_c++4 
		HINTS ${CMAKE_PREFIX_PATH} ${netCDFCxx_INSTALL_PREFIX/lib})

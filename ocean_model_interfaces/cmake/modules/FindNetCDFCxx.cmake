#Sets the following variables
# NetCDFCxx_INSTALL_PREFIX
# NetCDFCxx_INCLUDE_DIR
# NetCDFCxx_LIB_DIR
# NetCDFCxx_LIBRARIES

execute_process(COMMAND ncxx4-config --prefix OUTPUT_VARIABLE NetCDFCxx_INSTALL_PREFIX)
execute_process(COMMAND ncxx4-config --includedir OUTPUT_VARIABLE NetCDFCxx_INCLUDE_DIR)

string(REGEX REPLACE "\n$" "" NetCDFCxx_INSTALL_PREFIX "${NetCDFCxx_INSTALL_PREFIX}")
string(REGEX REPLACE "\n$" "" NetCDFCxx_INCLUDE_DIR "${NetCDFCxx_INCLUDE_DIR}")

find_library(NetCDFCxx_LIBRARIES 
		NAMES netcdf-cxx4 netcdf_c++4 
		HINTS ${CMAKE_PREFIX_PATH} ${NetCDFCxx_INSTALL_PREFIX/lib})

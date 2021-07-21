# Ocean Models Library
C++ library to provide random access to ocean model data. Primarily intended for use with autonomous underwater vehicle simulations.

## Currently included model interfaces
- FVCOM
- Miscellaneous primitive models for simple testing

## FVCOM
See the [FVCOM Website](http://fvcom.smast.umassd.edu/fvcom/) for more information on the model itself. Below is a summary of our implementation to provide quick random access to the FVCOM model.

Due to the size of FVCOM models it is not feasible to load the entire model into memory. Instead we only load the general structure of the model into memory, without the variable data. When a specific location and time is queried, a section, or "chunk", of the model containing that data will be loaded. These chunks are then stored in an LRU Cache. The size of these chunks and the cache size can be specified by the user.

### Interpolation
Linear interpolation in the temporal and z dimensions are performed for all data variables. For the x and y dimensions, barycentric linear interpolation of the variables located at the nodes (temp, salt, etc.) is performed.  For variables located at triangle centers (u,v,w, etc.), interpolation in the x and y dimensions is simply determined based on the containing triangle, similar, although not exactly, like nearest neighbors.

This limitation in the x and y dimension for variables located at triangle centers at is due to the unstructured nature of FVCOM. Triangulation at these points would be required to do baricentric linear interpolation. This is not too difficult of a task, but has yet to be implemented. This limitation does not exist with variables at nodes as the FVCOM structure already provides a triangulation.

### Assumptions and Limitations
- To simplify depth calculations and interpolation, the `center_h` and `center_siglay` FVCOM variables are ignored. `center_h` and `center_siglay` correspond to the water column height and percent depth for each siglay at the center of each triangle. Instead we use barycentric interpolation of the nodes that form each triangle to determine the depth of a specific siglay for points inside of that triangle. This can lead to the case where a queried location is below the true model bathemetry (i.e. below `center_h`), but above the interpolated bathemetry that we actually use to determine depth. In this case the data from the lowest depth siglay is used. In practice this would require a significant difference between the interpolated depth at a triangle center and the depth stored in `center_h`.
- We currently assume the values of the `h` FVCOM variable are positive and the values of the `siglay` FVCOM variable are negative.
- Currently the data retrieved for a given location is the standard u,v,w,temp,salt,depth as well as an additional optional dye variable. It is not currently possible to load other arbitrary data variables.

### Examples
See unit tests at `ocean_models/test/FVCOM_test.cpp`

Generally, only the FVCOM class needs to be used directly. Although, in unique cases or debugging purposes the FVCOMStructure class could be useful.

## Miscellaneous Primitive Models

These models provide a primitive, non-realistic models that are useful for simple testing, debugging, and sanity checking.

Current Primitive Models:
- ConstantModel
- LinearModel
- OceanFrontModel

Examples for the OceanFrontModel can be found in the unit tests at `ocean_models/test/OceanFrontModel_test.cpp`. The others are fairly self-explainatory.

## Build

`cd ocean_models`

`mkdir build`

`cd build`

To install in default libary directory
`cmake ../src`

To install in specific location
`cmake ../src -DCMAKE_INSTALL_PREFIX:PATH=<install_path>`

`make`

`make install`

## Unit Tests
Enable the `BUILD_TESTING` cmake option

`cd ocean_models/build`

`make`

`make test`

Use the following for verbose output of the tests
`make test CTEST_OUTPUT_ON_FAILURE=TRUE`

## Build Documentation
Enable the `BUILD_DOCS` cmake option

`cd ocean_models/build`

`make`

Documentation is output to `build/ocean_models/doc/doc_doxygen`

## Dependencies
### Boost
`sudo apt-get install libboost-system-dev libboost-filesystem-dev`

### NetCDF4 C++

#### NetCDF4 Dependencies
`sudo apt-get install libhdf5-dev libcurl4-gnutls-dev`

#### From Source

Install C library

`wget https://www.unidata.ucar.edu/downloads/netcdf/ftp/netcdf-c-4.7.1.tar.gz`

`tar -zxvf netcdf-c-4.7.1.tar.gz`

`cd ./netcdf-c-4.7.1/`

`mkdir build`

`cd ./build`

`cmake ../ -DCMAKE_BUILD_TYPE=RELEASE`

`sudo make install`


Install C++ interface

`wget https://www.unidata.ucar.edu/downloads/netcdf/ftp/netcdf-cxx4-4.3.1.tar.gz`

`tar -zxvf netcdf-cxx4-4.3.1.tar.gz`

`cd ./netcdf-cxx4-4.3.1/`

`mkdir build`

`cd ./build`

`cmake ../ -DCMAKE_BUILD_TYPE=RELEASE -DCMAKE_C_FLAGS=-I/usr/include/hdf5/serial/`

`sudo make install`

## Linking Instructions

When using cmake:

`find_package(OceanModels)`

`target_link_libraries(<target_name> PRIVATE|PUBLIC|INTERFACE OceanModels::ocean_models)`

When not using cmake link to the `ocean_models` library in the standard way.


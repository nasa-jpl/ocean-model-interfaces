# ocean_models
C++ Interfaces for various ocean models

## Build

`cd ocean_models`

`mkdir build`

To install in default libary directory
`cmake ../src`

To install in specific location
`cmake ../src -DCMAKE_INSTALL_PREFIX:PATH=<install_path>`

`make`

`make install`

## Unit Tests
`cd ocean_models/build`

`make test`

Use the following for verbose output of the tests
`make test CTEST_OUTPUT_ON_FAILURE=TRUE`

## Dependencies
### cmake
### NetCDF4 C++
download stable release of c library and c++ interface from https://www.unidata.ucar.edu/downloads/netcdf/index.jsp


For C library

`sudo apt-get install libcurl4-gnutls-dev`
or
`sudo apt-get install libcurl-ssl-dev`

`sudo apt-get install libhdf5-dev`

`mkdir build`

`cmake ../`

`make`

`sudo make install`


For C++ interface

`./configure`

`make`

`sudo make install`


### Boost
`sudo apt-get install libboost-system-dev`

`sudo apt-get install libboost-filesystem-dev`

# ocean_models
C++ Interfaces for various ocean models

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
`cd ocean_models/build`

`make test`

Use the following for verbose output of the tests
`make test CTEST_OUTPUT_ON_FAILURE=TRUE`

## Dependencies
### cmake
### NetCDF4 C++

`sudo apt-get install libhdf5-dev libcurl4-gnutls-dev`

For C library

`wget https://www.unidata.ucar.edu/downloads/netcdf/ftp/netcdf-c-4.7.1.tar.gz`

`tar -zxvf netcdf-c-4.7.1.tar.gz`

`cd ./netcdf-c-4.7.1/`

`mkdir build`

`cd ./build`

`cmake ../ -DCMAKE_BUILD_TYPE=RELEASE`

`sudo make install`

`cd ../..`

`rm -rf ./netcdf-c-4.7.1/`

`rm netcdf-c-4.7.1.tar.gz`

For C++ interface

`wget https://www.unidata.ucar.edu/downloads/netcdf/ftp/netcdf-cxx4-4.3.1.tar.gz`

`tar -zxvf netcdf-cxx4-4.3.1.tar.gz`

`cd ./netcdf-cxx4-4.3.1/`

`mkdir build`

`cd ./build`

`cmake ../ -DCMAKE_BUILD_TYPE=RELEASE -DCMAKE_C_FLAGS=-I/usr/include/hdf5/serial/`

`sudo make install`

`cd ../..`

`rm -rf ./netcdf-cxx4-4.3.1`

`rm netcdf-cxx4-4.3.1.tar.gz`


### Boost
`sudo apt-get install libboost-system-dev libboost-filesystem-dev`

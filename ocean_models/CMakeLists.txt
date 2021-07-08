cmake_minimum_required(VERSION 3.9)

project(ocean_models VERSION 0.1 DESCRIPTION "C++ library for accessing ocean models")

###
###For Build
###

set (CMAKE_CXX_STANDARD 11)

#Load all the required packages
set (CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules)
find_package(netCDF REQUIRED)
find_package(NetCDFCxx REQUIRED)

set(boost_min_ver 1.50.0)
set(boost_libs system filesystem)
find_package(Boost ${boost_min_ver})

if(Boost_FOUND)
    find_package(Boost ${boost_min_ver} COMPONENTS ${boost_libs})
endif()

#Add all .cpp files to the library
add_library(ocean_models SHARED
    src/fvcom/FVCOM.cpp
    src/fvcom/FVCOMChunk.cpp
    src/fvcom/FVCOMStructure.cpp
    src/general_models/ConstantModel.cpp
    src/general_models/LinearModel.cpp
    src/general_models/OceanFrontModel.cpp
    src/model_interface/ModelInterface.cpp
)

#Set the version of the target
set_target_properties(ocean_models PROPERTIES VERSION ${PROJECT_VERSION})

#Include all required directories
#Some library includes are only here incase they are installed in unconventional locations
target_include_directories(ocean_models PRIVATE ${NetCDFCxx_INCLUDE_DIR} ${netCDF_INCLUDE_DIR} ${Boost_INCLUDE_DIR})
target_include_directories(ocean_models PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include/ocean_models>)

#netCDF_LIBRARIES Does not contain the full path so we add this for if it
#is installed in an unconventional location
link_directories(${netCDF_LIB_DIR}) 
target_link_libraries(ocean_models PRIVATE
    ${netCDF_LIBRARIES} 
    ${Boost_LIBRARIES} 
    ${NetCDFCxx_LIBRARIES}
)

###
###For Install
###
include(GNUInstallDirs)
install(TARGETS ocean_models
    ARCHIVE  DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY  DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME  DESTINATION ${CMAKE_INSTALL_BINDIR})
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

if(BUILD_TESTING) 
    add_subdirectory(test)
endif()

if(BUILD_DOCS) 
    add_subdirectory(doc)
endif()
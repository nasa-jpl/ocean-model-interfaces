cmake_minimum_required(VERSION 3.9)

###
###For Build
###

set (CMAKE_CXX_STANDARD 11)

#Load all the required packages
set (CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake/modules)
find_package(netCDF REQUIRED)
find_package(NetCDFCxx REQUIRED)

set(boost_min_ver 1.50.0)
set(boost_libs system filesystem)
find_package(Boost ${boost_min_ver})

if(Boost_FOUND)
    find_package(Boost ${boost_min_ver} COMPONENTS ${boost_libs})
endif()

#Add all .cpp files to the library
add_library(ocean_model_interfaces SHARED
    src/fvcom/FVCOM.cpp
    src/fvcom/FVCOMChunk.cpp
    src/fvcom/FVCOMStructure.cpp
    src/general_models/ConstantModel.cpp
    src/general_models/LinearModel.cpp
    src/general_models/OceanFrontModel.cpp
    src/model_interface/ModelInterface.cpp
)

#Set the version of the target
set_target_properties(ocean_model_interfaces PROPERTIES VERSION ${PROJECT_VERSION})

#Include all required directories
#Some library includes are only here incase they are installed in unconventional locations
target_include_directories(ocean_model_interfaces PRIVATE ${NetCDFCxx_INCLUDE_DIR} ${netCDF_INCLUDE_DIR} ${Boost_INCLUDE_DIR})
target_include_directories(ocean_model_interfaces PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/ocean_model_interfaces>)

#netCDF_LIBRARIES Does not contain the full path so we add this for if it
#is installed in an unconventional location
link_directories(${netCDF_LIB_DIR})
target_link_libraries(ocean_model_interfaces PRIVATE
    ${netCDF_LIBRARIES} 
    ${Boost_LIBRARIES} 
    ${NetCDFCxx_LIBRARIES}
)

install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

###
###For Install
###
include(GNUInstallDirs)
install(TARGETS ocean_model_interfaces
        EXPORT OceanModelInterfacesTargets
        ARCHIVE  DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY  DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME  DESTINATION ${CMAKE_INSTALL_BINDIR}
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(EXPORT OceanModelInterfacesTargets
        FILE OceanModelInterfacesTargets.cmake
        NAMESPACE OceanModelInterfaces::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/OceanModelInterfaces
)

include(CMakePackageConfigHelpers)

configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/Config.cmake.in
  "${CMAKE_CURRENT_BINARY_DIR}/OceanModelInterfacesConfig.cmake"
  INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/OceanModelInterfaces
)

write_basic_package_version_file(
  ${CMAKE_CURRENT_BINARY_DIR}/OceanModelInterfacesConfigVersion.cmake
  VERSION ${version}
  COMPATIBILITY AnyNewerVersion
  )

install(FILES
        "${CMAKE_CURRENT_BINARY_DIR}/OceanModelInterfacesConfig.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/OceanModelInterfacesConfigVersion.cmake"
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/OceanModelInterfaces
)

if(BUILD_TESTING) 
    add_subdirectory(test)
endif()

if(BUILD_DOCS) 
    add_subdirectory(doc)
endif()
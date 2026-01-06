add_library(ocean_model_interfaces_py SHARED
    src/FVCOMPyInterface.cpp
    src/GeodeticGridPyInterface.cpp
    src/ModelInterfacePyInterface.cpp
)

set_target_properties(ocean_model_interfaces_py PROPERTIES VERSION ${PROJECT_VERSION})


target_include_directories(ocean_model_interfaces_py PRIVATE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)

target_include_directories(ocean_model_interfaces_py PRIVATE ${NetCDFCxx_INCLUDE_DIR} ${netCDF_INCLUDE_DIR} ${Boost_INCLUDE_DIR})

target_link_libraries(ocean_model_interfaces_py PRIVATE
    ocean_model_interfaces
)


include(GNUInstallDirs)
install(TARGETS ocean_model_interfaces_py
        ARCHIVE  DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY  DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME  DESTINATION ${CMAKE_INSTALL_BINDIR}
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
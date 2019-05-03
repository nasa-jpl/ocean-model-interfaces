cmake_minimum_required(VERSION 3.9)

project(ocean_models VERSION 0.1 DESCRIPTION "C++ library for accessing ocean models")
set (CMAKE_CXX_STANDARD 11)
set(CMAKE_BUILD_TYPE Debug)

set (NETCDF_CXX "YES")
find_package(NetCDF REQUIRED)

set(boost_min_ver 1.50.0)
set(boost_libs system filesystem)
find_package(Boost ${boost_min_ver})

if(Boost_FOUND)
    find_package(Boost ${boost_min_ver} COMPONENTS ${boost_libs})
endif()

add_library(ocean_models SHARED
    src/fvcom/FVCOM.cpp
    src/fvcom/FVCOMChunk.cpp
    src/fvcom/FVCOMStructure.cpp
    src/general_models/ConstantModel.cpp
    src/general_models/LinearModel.cpp
    src/model_interface/ModelInterface.cpp
)

target_link_libraries(ocean_models
    PRIVATE
        ${NETCDF_LIBRARIES} 
        ${Boost_LIBRARIES} 
        netcdf_c++4
)

set_target_properties(ocean_models PROPERTIES VERSION ${PROJECT_VERSION})

target_include_directories(ocean_models PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include/ocean_models>)

include(GNUInstallDirs)
install(TARGETS ocean_models
    ARCHIVE  DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY  DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})


###
###For Unit Tests
###

add_executable(FVCOM_test test/FVCOM_test.cpp)
target_link_libraries(FVCOM_test gtest ocean_models)
add_test(NAME FVCOM_test COMMAND FVCOM_test)

add_executable(FVCOMChunk_test test/FVCOMChunk_test.cpp)
target_link_libraries(FVCOMChunk_test gtest ocean_models)
add_test(NAME FVCOMChunk_test COMMAND FVCOMChunk_test)

add_executable(FVCOMStructure_test test/FVCOMStructure_test.cpp)
target_link_libraries(FVCOMStructure_test gtest ocean_models)
add_test(NAME FVCOMStructure_test COMMAND FVCOMStructure_test)

add_executable(lru_cache_test test/lru_cache_test.cpp)
target_link_libraries(lru_cache_test gtest ocean_models)
add_test(NAME lru_cache_test COMMAND lru_cache_test)
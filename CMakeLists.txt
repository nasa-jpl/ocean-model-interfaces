cmake_minimum_required(VERSION 3.9)

set(version 1.0.0)
project(OceanModelInterfaces VERSION ${version} DESCRIPTION "C++ library for accessing ocean models")

set (CMAKE_CXX_STANDARD 11)
include(GNUInstallDirs)
set(CMAKE_BUILD_TYPE Release)

add_compile_options($<BUILD_INTERFACE:-Wall>)

option(BUILD_TESTING "Build unit tests" OFF)
option(BUILD_DOCS "Build documentation" OFF)

if(BUILD_TESTING) 
    enable_testing()
    add_subdirectory(gtest)
endif()

add_subdirectory(ocean_model_interfaces)
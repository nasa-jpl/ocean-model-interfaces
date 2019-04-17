cmake_minimum_required(VERSION 3.9)

project(ocean_models VERSION 0.1 DESCRIPTION "C++ library for accessing ocean models")

set (CMAKE_CXX_STANDARD 11)
include(GNUInstallDirs)

enable_testing()

add_subdirectory(gtest)
add_subdirectory(ocean_models)
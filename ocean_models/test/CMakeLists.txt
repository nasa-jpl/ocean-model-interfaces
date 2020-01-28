cmake_minimum_required(VERSION 3.9)

add_executable(FVCOM_test FVCOM_test.cpp)
target_include_directories(FVCOM_test PRIVATE ${NetCDFCxx_INCLUDE_DIR} ${netCDF_INCLUDE_DIR})
target_link_libraries(FVCOM_test gtest ocean_models ${netCDF_LIBRARIES} ${NetCDFCxx_LIBRARIES})
add_test(NAME FVCOM_test COMMAND FVCOM_test WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})

add_executable(FVCOMChunk_test FVCOMChunk_test.cpp)
target_include_directories(FVCOMChunk_test PRIVATE ${NetCDFCxx_INCLUDE_DIR} ${netCDF_INCLUDE_DIR})
target_link_libraries(FVCOMChunk_test gtest ocean_models ${netCDF_LIBRARIES} ${NetCDFCxx_LIBRARIES})
add_test(NAME FVCOMChunk_test COMMAND FVCOMChunk_test WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})

add_executable(FVCOMStructure_test FVCOMStructure_test.cpp)
target_include_directories(FVCOMStructure_test PRIVATE ${NetCDFCxx_INCLUDE_DIR} ${netCDF_INCLUDE_DIR})
target_link_libraries(FVCOMStructure_test gtest ocean_models ${netCDF_LIBRARIES} ${NetCDFCxx_LIBRARIES})
add_test(NAME FVCOMStructure_test COMMAND FVCOMStructure_test WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})

add_executable(lru_cache_test lru_cache_test.cpp)
target_link_libraries(lru_cache_test gtest ocean_models)
add_test(NAME lru_cache_test COMMAND lru_cache_test WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})

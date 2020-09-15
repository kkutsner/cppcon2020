cmake_minimum_required(VERSION 3.17)
project(value-types)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(mylib src/data_base.cxx src/sql_processor.cxx)

target_precompile_headers(mylib PRIVATE <vector> <array> <string> <stdint.h>)
target_include_directories(mylib PUBLIC 
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>/include
)


add_subdirectory(tests)
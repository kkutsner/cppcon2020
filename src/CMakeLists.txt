add_library(mylib data_base.cxx sql_processor.cxx)

target_precompile_headers(mylib PRIVATE <vector> <array> <string> <stdint.h>)
target_include_directories(mylib PUBLIC  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>/../include)

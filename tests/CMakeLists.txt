
add_executable(test-value-types main.cxx)
target_link_libraries(test-value-types PRIVATE mylib)
target_precompile_headers(test-value-types PRIVATE <vector> <array> <string> <stdint.h>)
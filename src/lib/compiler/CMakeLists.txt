add_library(fortran_compiler compiler.cpp)
target_include_directories(fortran_compiler PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(fortran_compiler PUBLIC fortran::parser)
add_library(fortran::compiler ALIAS fortran_compiler)

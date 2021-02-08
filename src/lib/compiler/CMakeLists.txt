add_library(fortran_compiler compiler.cpp function.cpp)
target_include_directories(fortran_compiler PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(fortran_compiler
  PUBLIC
  fortran::parser
  fortran::virtual_machine)
add_library(fortran::compiler ALIAS fortran_compiler)

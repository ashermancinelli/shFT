add_library(fortran_virtual_machine vm.cpp)
target_include_directories(fortran_virtual_machine PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR})
add_library(fortran::virtual_machine ALIAS fortran_virtual_machine)

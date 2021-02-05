add_subdirectory(ux)
add_subdirectory(virtual_machine)
add_subdirectory(parser)
add_subdirectory(compiler)

add_library(fortran_fortran INTERFACE)
target_link_libraries(fortran_fortran
  INTERFACE
  fortran::parser
  fortran::virtual_machine
  fortran::compiler
  fortran::ux
  )
add_library(fortran::fortran ALIAS fortran_fortran)

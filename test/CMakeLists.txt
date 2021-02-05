add_subdirectory(util)

add_executable(test_parser parser/test_parser.cpp)
target_link_libraries(test_parser PRIVATE fortran::parser fortran::testing::util)

add_executable(test_compiler compiler/test_compiler.cpp)
target_link_libraries(test_compiler PRIVATE fortran::compiler)

add_executable(test_virtual_machine
  virtual_machine/test_virtual_machine.cpp
  virtual_machine/test_fns.cpp
  )
target_link_libraries(test_virtual_machine
  PRIVATE
  fortran::virtual_machine
  fortran::testing::util
  )

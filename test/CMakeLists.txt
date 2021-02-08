add_subdirectory(util)

add_executable(test_parser parser/test_parser.cpp)
target_link_libraries(test_parser PRIVATE shFT::parser shFT::testing::util)

add_executable(test_compiler
  compiler/test_compiler.cpp
  compiler/test_fns.cpp
  )
target_link_libraries(test_compiler 
  PRIVATE 
  shFT::compiler
  shFT::testing::util
  )

add_executable(test_virtual_machine
  virtual_machine/test_virtual_machine.cpp
  virtual_machine/test_fns.cpp
  )
target_link_libraries(test_virtual_machine
  PRIVATE
  shFT::virtual_machine
  shFT::testing::util
  )

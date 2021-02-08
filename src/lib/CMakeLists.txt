add_subdirectory(ux)
add_subdirectory(virtual_machine)
add_subdirectory(parser)
add_subdirectory(compiler)

add_library(shFT_shFT INTERFACE)
target_link_libraries(shFT_shFT
  INTERFACE
  shFT::parser
  shFT::virtual_machine
  shFT::compiler
  shFT::ux
  )
add_library(shFT::shFT ALIAS shFT_shFT)

add_library(shFT_compiler compiler.cpp source_function.cpp)
target_include_directories(shFT_compiler PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(shFT_compiler
  PUBLIC
  shFT::parser
  shFT::virtual_machine)
add_library(shFT::compiler ALIAS shFT_compiler)

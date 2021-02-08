add_library(shFT_virtual_machine vm.cpp)
target_include_directories(shFT_virtual_machine PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR})
add_library(shFT::virtual_machine ALIAS shFT_virtual_machine)

add_library(shFT_ux INTERFACE)
target_include_directories(shFT_ux INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
add_library(shFT::ux ALIAS shFT_ux)

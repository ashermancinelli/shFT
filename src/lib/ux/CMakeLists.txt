add_library(fortran_ux INTERFACE)
target_include_directories(fortran_ux INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
add_library(fortran::ux ALIAS fortran_ux)

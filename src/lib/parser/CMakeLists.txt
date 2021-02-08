add_library(fortran_parser INTERFACE)
target_include_directories(fortran_parser INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(fortran_parser INTERFACE Boost::boost fortran::ux)
add_library(fortran::parser ALIAS fortran_parser)

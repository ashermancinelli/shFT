add_library(fortran_parser expression.cpp statement.cpp)
target_include_directories(fortran_parser PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(fortran_parser PUBLIC Boost::boost fortran::ux)
add_library(fortran::parser ALIAS fortran_parser)

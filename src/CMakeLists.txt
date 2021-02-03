add_subdirectory(lib)

add_executable(${PROJECT_NAME} driver2.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE
  Boost::boost
  fortran::parser
  fortran::compiler)

add_executable(calc calc.cpp)
target_link_libraries(calc PRIVATE Boost::boost)

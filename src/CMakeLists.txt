add_subdirectory(lib)

add_executable(${PROJECT_NAME} driver2.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE
  Boost::boost
  fortran::fortran
  )

add_executable(calc calc.cpp)
target_link_libraries(calc PRIVATE Boost::boost)
add_executable(xml xml.cpp)
target_link_libraries(xml PRIVATE Boost::boost)
add_executable(blog blog.cpp)
target_link_libraries(blog PRIVATE Boost::boost)

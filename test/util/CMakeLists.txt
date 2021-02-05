add_library(fortran_testing_util util.cpp)
target_include_directories(fortran_testing_util PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(fortran_testing_util PUBLIC fortran::ux)
add_library(fortran::testing::util ALIAS fortran_testing_util)

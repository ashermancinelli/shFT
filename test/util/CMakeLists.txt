add_library(shFT_testing_util util.cpp)
target_include_directories(shFT_testing_util PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(shFT_testing_util PUBLIC shFT::ux)
add_library(shFT::testing::util ALIAS shFT_testing_util)

add_subdirectory(lib)

add_executable(${PROJECT_NAME} driver.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE
  Boost::boost
  shFT::shFT
  )

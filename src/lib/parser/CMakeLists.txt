add_library(shFT_parser expression.cpp statement.cpp function.cpp)
target_include_directories(shFT_parser PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(shFT_parser PUBLIC Boost::boost shFT::ux)
add_library(shFT::parser ALIAS shFT_parser)

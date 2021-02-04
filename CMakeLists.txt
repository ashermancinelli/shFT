cmake_minimum_required (VERSION 3.15)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(shFT CXX)

find_package(Boost REQUIRED)

add_subdirectory(src)
add_subdirectory(test)

enable_testing()

add_test(NAME Test_parser COMMAND $<TARGET_FILE:test_parser>)
add_test(NAME Test_compiler COMMAND $<TARGET_FILE:test_compiler>)
add_test(NAME Test_virtual_machine COMMAND $<TARGET_FILE:test_virtual_machine>)

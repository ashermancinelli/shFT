cmake_minimum_required (VERSION 3.15)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(shFT CXX)

find_package(Boost REQUIRED)

add_subdirectory(src)

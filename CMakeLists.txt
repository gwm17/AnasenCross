cmake_minimum_required(VERSION 3.12)

set(CMAKE_CXX_STANDARD 20)

set(ACROSS_BINARY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/bin)

project(AnasenCross)

find_package(ROOT REQUIRED)

add_subdirectory(vendor/catima)
add_subdirectory(src)
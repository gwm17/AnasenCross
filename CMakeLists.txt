cmake_minimum_required(VERSION 3.12)

set(CMAKE_CXX_STANDARD 20)

set(ACROSS_BINARY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/bin)

if(NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_BUILD_TYPE "Release")
    message("Building release")
else()
    message("Building debug")
endif()

project(AnasenCross)

find_package(ROOT REQUIRED)

add_subdirectory(vendor/catima)
add_subdirectory(vendor/fmt)
set(JSON_BuildTests OFF CACHE INTERNAL "")
set(JSON_Install OFF CACHE INTERNAL "")
add_subdirectory(vendor/nlohmann_json)
add_subdirectory(src)
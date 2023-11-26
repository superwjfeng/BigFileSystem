cmake_minimum_required(VERSION 3.2)
project(LargeFileSystem)

set(CMAKE_CXX_COMPILER "g++")
include_directories(${CMAKE_SOURCE_DIR}/include/)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)

add_subdirectory(src)
add_subdirectory(tests)




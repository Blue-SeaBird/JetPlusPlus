cmake_minimum_required(VERSION 3.10)
project(ExampleProj)

set(CMAKE_CXX_STANDARD 17)

# Include directory for the library headers
include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(ExampleProj src/main.cpp)

# Specify the relative path to your library without -l flag
target_link_libraries(ExampleProj PRIVATE ${CMAKE_SOURCE_DIR}/lib/libJetPlusPlusLib.dylib)
# CMakeLists.txt in the root of your library project

cmake_minimum_required(VERSION 3.10)
project(JetPlusPlusLib)

set(CMAKE_CXX_STANDARD 17)

# Collect all source files in the src directory
file(GLOB_RECURSE LIB_SOURCES "src/*.cpp")

# list(APPEND LIB_SOURCES "src/json/jsonConverter.cpp")

# Create the library
add_library(JetPlusPlusLib SHARED ${LIB_SOURCES})

# Add include directories
target_include_directories(JetPlusPlusLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Optionally, you can set additional compiler options or dependencies here
# target_compile_options(JetPlusPlusLib PRIVATE -Wall)

# Optionally, you can link against other libraries
# target_link_libraries(JetPlusPlusLib PRIVATE some_other_library)

# Specify the output directory for the library build
set_target_properties(JetPlusPlusLib PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build)

# Optionally, you can install the library to a system location
# install(TARGETS JetPlusPlusLib DESTINATION /usr/local/lib)

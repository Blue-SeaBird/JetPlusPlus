cmake_minimum_required(VERSION 3.10)
project(JetPlusPlus)

set(CMAKE_CXX_STANDARD 11)

# Specify the main executable
add_executable(main src/main.cpp src/router/Router.cpp src/Server.cpp src/Response.cpp src/Request.cpp src/router/Route.cpp src/methods/methods.cpp src/json/value.cpp)

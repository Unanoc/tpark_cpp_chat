cmake_minimum_required(VERSION 3.9)
project(HttpServer)

set(CMAKE_CXX_STANDARD 14)

add_executable(HttpServer http_server.cpp)

target_link_libraries(HttpServer event)
cmake_minimum_required(VERSION 3.9)
project(HttpServer)

set(CMAKE_CXX_STANDARD 17)

add_executable(
        HttpServer
        database_interaction/database_interaction.hpp
        database_interaction/database_interaction.cpp
        json_converter.h
        json_converter.cpp
        server.h
        server.cpp
        main.cpp
)

target_link_libraries(HttpServer event jansson pqxx pq)

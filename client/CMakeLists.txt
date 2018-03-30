cmake_minimum_required(VERSION 3.10)
project(client)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})


include_directories (${PROJECT_SOURCE_DIR}/HttpClient)
add_subdirectory(HttpClient)

include_directories (${PROJECT_SOURCE_DIR}/GUI)
add_subdirectory(GUI)

add_executable(client main.cpp )

target_link_libraries(client HttpClient)
target_link_libraries(client GUI)

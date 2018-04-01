cmake_minimum_required(VERSION 3.10)

project(client)

set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(TARGET ${CMAKE_PROJECT_NAME})
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/exec)
set(SOURCES main.cpp messenger-window.cpp client.cpp)

find_package(Qt5Widgets REQUIRED)
qt5_wrap_ui(UI_HEADERS messenger-window.ui)

if(WIN32)
    set(CMAKE_WIN32_EXECUTABLE ON)
endif()

add_executable(${TARGET} ${SOURCES} ${UI_HEADERS})

target_link_libraries(${TARGET} Qt5::Widgets)

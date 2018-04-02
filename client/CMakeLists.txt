<<<<<<< HEAD
cmake_minimum_required(VERSION 3.10)
project(client)

set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD 11)

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

#Здесь напиши свой путь
set(CMAKE_PREFIX_PATH  "/usr/lib/x86_64-linux-gnu/qt5/bin")

find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Widgets REQUIRED)


include_directories (${PROJECT_SOURCE_DIR}/HttpClient)
add_subdirectory(HttpClient)

include_directories (${PROJECT_SOURCE_DIR}/GUI)
add_subdirectory(GUI)

add_executable(${PROJECT_NAME} main.cpp )

target_link_libraries(${PROJECT_NAME} HttpClient)
target_link_libraries(${PROJECT_NAME} GUI)
target_link_libraries(${PROJECT_NAME} Qt5::Core)
target_link_libraries(${PROJECT_NAME} Qt5::Gui)
target_link_libraries(${PROJECT_NAME} Qt5::Widgets)
=======
cmake_minimum_required(VERSION 3.10)

project(client)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_CXX_STANDARD 11)

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

if(WIN32)
    set(CMAKE_PREFIX_PATH "C:\\Qt\\Qt5.10.1\\5.10.1\\msvc2015\\")
else()
    set(CMAKE_PREFIX_PATH "/usr/lib/x86_64-linux-gnu/qt5/bin")
endif()

find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Widgets REQUIRED)

include_directories(${PROJECT_SOURCE_DIR}/HttpClient)
add_subdirectory(HttpClient)

include_directories(${PROJECT_SOURCE_DIR}/GUI)
add_subdirectory(GUI)

qt5_wrap_ui(UI_HEADERS GUI/messenger-window.ui)

add_executable(${PROJECT_NAME} main.cpp ${UI_HEADERS})

target_link_libraries(${PROJECT_NAME} HttpClient)
target_link_libraries(${PROJECT_NAME} GUI)
target_link_libraries(${PROJECT_NAME} Qt5::Core)
target_link_libraries(${PROJECT_NAME} Qt5::Gui)
target_link_libraries(${PROJECT_NAME} Qt5::Widgets)
>>>>>>> GUI

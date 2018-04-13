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

find_package(Qt5Core CONFIG REQUIRED Qml Quick Gui)
find_package(Qt5Quick REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Qml REQUIRED)
find_package(Qt5Network REQUIRED)
qt5_add_resources(qml_QRC GUI/qml.qrc GUI/images.qrc GUI/fonts.qrc)


include_directories(${PROJECT_SOURCE_DIR}/HttpClient)
add_subdirectory(HttpClient)

if(WIN32)
	add_executable(${PROJECT_NAME} WIN32 main.cpp ${qml_QRC})
else()
	add_executable(${PROJECT_NAME} main.cpp ${qml_QRC})
endif()

target_link_libraries(${PROJECT_NAME} HttpClient)
target_link_libraries(${PROJECT_NAME} Qt5::Core)
target_link_libraries(${PROJECT_NAME} Qt5::Gui)
target_link_libraries(${PROJECT_NAME} Qt5::Qml)
target_link_libraries(${PROJECT_NAME} Qt5::Quick)
target_link_libraries(${PROJECT_NAME}  Qt5::Network)


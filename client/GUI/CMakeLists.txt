set(SOURCES client.cpp)

add_library(HttpClient STATIC ${SOURCES})

target_link_libraries(HttpClient Qt5::Network)
target_link_libraries(HttpClient Qt5::Core)
target_link_libraries(HttpClient Qt5::Widgets)



add_executable(HttpClientMain main.cpp client.cpp)

target_link_libraries(HttpClientMain Qt5::Network)
target_link_libraries(HttpClientMain Qt5::Core)
target_link_libraries(HttpClientMain Qt5::Widgets)
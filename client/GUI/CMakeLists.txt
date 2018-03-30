set(SOURCES messenger-window.cpp)

add_library(GUI STATIC ${SOURCES})

target_link_libraries (GUI HttpClient)
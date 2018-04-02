set(SOURCES messenger-window.cpp)


add_library(GUI STATIC ${SOURCES})

target_link_libraries(GUI HttpClient)
target_link_libraries(GUI Qt5::Core)
target_link_libraries(GUI Qt5::Gui)
target_link_libraries(GUI Qt5::Widgets)
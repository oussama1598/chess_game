set(FILES)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed
set(CMAKE_AUTOMOC ON)
# Create code from a list of Qt designer ui files
set(CMAKE_AUTOUIC ON)

find_package(Qt5Core CONFIG REQUIRED)
find_package(Qt5Widgets CONFIG REQUIRED)

set(SOURCES main.cpp pages/main/main_window.cpp pages/main/main_window.h pages/states/states_window.cpp pages/states/states_window.h pages/game/game_window.cpp pages/game/game_window.h launcher.cpp launcher.h pages/about/about_window.cpp pages/about/about_window.h pages/about/heart_graphic.cpp pages/about/heart_graphic.h pages/new_game/new_game_window.cpp pages/new_game/new_game_window.h)

add_executable(launcher ${SOURCES} resources/resources.qrc)
target_link_libraries(launcher game renderers ai Qt5::Core Qt5::Widgets)

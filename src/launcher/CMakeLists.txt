set(FILES)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed
set(CMAKE_AUTOMOC ON)
# Create code from a list of Qt designer ui files
set(CMAKE_AUTOUIC ON)

find_package(Qt5Core CONFIG REQUIRED)
find_package(Qt5Widgets CONFIG REQUIRED)

set(SOURCES main.cpp pages/main/mainwindow.cpp pages/main/mainwindow.h pages/states/states_window.cpp pages/states/states_window.h pages/game/game_window.cpp pages/game/game_window.h)

add_executable(launcher ${SOURCES})
target_link_libraries(launcher game renderers Qt5::Core Qt5::Widgets)

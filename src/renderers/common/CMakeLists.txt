find_package(SDL2 REQUIRED CONFIG)

add_library(chess_audio
        sound_manager.cpp
        sound_manager.h)

target_link_libraries(chess_audio PUBLIC SDL2::SDL2)
target_include_directories(chess_audio INTERFACE ..)

find_package(SDL2 REQUIRED CONFIG)
find_package(SDL2_image REQUIRED CONFIG)
find_package(SDL2_ttf REQUIRED CONFIG)

set(FILES
        renderer_2D.h
        renderer_2D.cpp

        text_renderer.cpp
        text_renderer.h)

add_library(2d_engine ${FILES})
target_link_libraries(2d_engine
        PUBLIC
        game
        chess_audio
        SDL2::SDL2
        SDL2_image::SDL2_image
        SDL2_ttf::SDL2_ttf)
target_include_directories(2d_engine INTERFACE ..)

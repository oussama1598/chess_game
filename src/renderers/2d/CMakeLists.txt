set(FILES
        renderer_2D.h
        renderer_2D.cpp

        sound_manager.cpp
        sound_manager.h

        text_renderer.cpp
        text_renderer.h)

add_library(2d_engine ${FILES})
target_link_libraries(2d_engine CONAN_PKG::sdl2 CONAN_PKG::sdl2_image CONAN_PKG::sdl2_ttf game)
target_include_directories(2d_engine INTERFACE ..)

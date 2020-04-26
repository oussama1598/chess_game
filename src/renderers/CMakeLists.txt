set(FILES
        console/console.h
        console/console.cpp

        2d/renderer_2D.h
        2d/renderer_2D.cpp

        2d/sound_manager.cpp
        2d/sound_manager.h

        2d/text_renderer.cpp
        2d/text_renderer.h
        )

add_library(renderers ${FILES})
target_link_libraries(renderers CONAN_PKG::sdl2 CONAN_PKG::sdl2_image CONAN_PKG::sdl2_ttf game)
target_include_directories(renderers INTERFACE ..)

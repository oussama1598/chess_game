set(FILES
        console/console.h
        console/console.cpp

        2d/renderer_2D.h
        2d/renderer_2D.cpp
        )

add_library(renderers ${FILES})
target_link_libraries(renderers CONAN_PKG::sdl2 CONAN_PKG::sdl2_image game)
target_include_directories(renderers INTERFACE ..)

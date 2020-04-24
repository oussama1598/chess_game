set(FILES
        modules/Game.cpp
        modules/Game.h
        modules/Board.h
        modules/Board.cpp
        modules/pieces/Piece.h
        modules/pieces/Piece.cpp

        modules/pieces/King.h
        modules/pieces/King.cpp

        modules/pieces/Queen.h
        modules/pieces/Queen.cpp

        modules/pieces/Rook.h
        modules/pieces/Rook.cpp

        modules/pieces/Bishop.h
        modules/pieces/Bishop.cpp

        modules/pieces/Pawn.h
        modules/pieces/Pawn.cpp

        modules/pieces/Knight.h
        modules/pieces/Knight.cpp

        modules/Player.h
        modules/Errors.h modules/Errors.cpp renderers/console.cpp renderers/console.h renderers/2d/renderer_2D.cpp renderers/2d/renderer_2D.h)

add_library(game ${FILES})
target_link_libraries(game CONAN_PKG::sdl2 CONAN_PKG::sdl2_image)
target_include_directories(game INTERFACE ..)

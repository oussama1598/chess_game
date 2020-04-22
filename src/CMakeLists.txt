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
        modules/Errors.h modules/Errors.cpp renderers/Console.cpp renderers/Console.h)

add_library(game ${FILES})
target_include_directories(game INTERFACE ..)
add_executable(${PROJECT_NAME} main.cpp ${FILES})

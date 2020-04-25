set(FILES
        Game.cpp
        Game.h

        Board.h
        Board.cpp

        pieces/Piece.h
        pieces/Piece.cpp

        pieces/King.h
        pieces/King.cpp

        pieces/Queen.h
        pieces/Queen.cpp

        pieces/Rook.h
        pieces/Rook.cpp

        pieces/Bishop.h
        pieces/Bishop.cpp

        pieces/Pawn.h
        pieces/Pawn.cpp

        pieces/Knight.h
        pieces/Knight.cpp

        Player.h

        Errors.h
        Errors.cpp
)

add_library(game ${FILES})
target_include_directories(game INTERFACE ..)

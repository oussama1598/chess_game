add_executable(chess_test chess_test.cpp pieces/pawn.cpp pieces/king.cpp pieces/king.h pieces/pawn.h pieces/bishop.cpp pieces/bishop.h pieces/rook.cpp pieces/rook.h pieces/queen.cpp pieces/queen.h pieces/knight.cpp pieces/knight.h game.cpp game.h board.cpp board.h)
target_link_libraries(chess_test game CONAN_PKG::Catch2)
add_test(NAME chess_test COMMAND $<TARGET_FILE:chess_test>)

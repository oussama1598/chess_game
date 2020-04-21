add_executable(chess_test game.cpp pieces/pawn.cpp pieces/king.cpp pieces/king.h pieces/pawn.h)
target_link_libraries(chess_test game CONAN_PKG::Catch2)
add_test(NAME chess_test COMMAND $<TARGET_FILE:chess_test>)

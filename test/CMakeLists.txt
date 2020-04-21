add_executable(chess_test game.cpp)
target_link_libraries(chess_test CONAN_PKG::Catch2)
add_test(NAME chess_test COMMAND $<TARGET_FILE:chess_test>)

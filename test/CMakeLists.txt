find_package(Catch2 3 REQUIRED)

add_executable(chess_test chess_test.cpp pieces/pawn.cpp pieces/king.cpp pieces/king.h pieces/pawn.h pieces/bishop.cpp pieces/bishop.h pieces/rook.cpp pieces/rook.h pieces/queen.cpp pieces/queen.h pieces/knight.cpp pieces/knight.h game.cpp game.h board.cpp board.h)
target_link_libraries(chess_test PRIVATE game Catch2::Catch2WithMain)

include(Catch)
catch_discover_tests(chess_test WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

find_program(XVFB_RUN xvfb-run)
if(XVFB_RUN)
    add_executable(renderer_smoke renderers.cpp)
    target_link_libraries(renderer_smoke PRIVATE renderers)
    add_test(
            NAME renderer_startup
            COMMAND bash ${CMAKE_CURRENT_SOURCE_DIR}/run_renderer_smoke.sh
                    $<TARGET_FILE:renderer_smoke>)
    set_tests_properties(
            renderer_startup
            PROPERTIES
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin
            ENVIRONMENT "SDL_AUDIODRIVER=dummy;LIBGL_ALWAYS_SOFTWARE=1")
endif()

add_custom_command(
        TARGET chess_test POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/test/plays/
        ${CMAKE_BINARY_DIR}/bin/plays/)

add_custom_command(
        TARGET chess_test POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/test/plays/
        ${CMAKE_CURRENT_BINARY_DIR}/plays/)

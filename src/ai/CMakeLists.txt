add_library(ai ai_player.cpp ai_player.h)
target_link_libraries(ai game)
target_include_directories(ai INTERFACE ..)


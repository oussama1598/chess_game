find_package(OpenMP)

add_library(ai ai_player.cpp ai_player.h)
target_link_libraries(ai game OpenMP::OpenMP_CXX)
target_include_directories(ai INTERFACE ..)

target_compile_options(ai PUBLIC -fopenmp)

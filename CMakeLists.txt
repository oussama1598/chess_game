cmake_minimum_required(VERSION 3.16)
project(chess_game LANGUAGES CXX)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
include(CTest)


add_compile_options(-Wall -Wextra -fPIC)

if(CMAKE_BUILD_TYPE MATCHES Debug)
    add_compile_options(-DDEBUG)
endif()

add_compile_options(-DQT_NO_OPENGL)

add_subdirectory(src)
if(BUILD_TESTING)
    add_subdirectory(test)
endif()

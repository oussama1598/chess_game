cmake_minimum_required(VERSION 3.13)
project(chess_game)

find_program(CONAN conan)

if (NOT CONAN)
    message(FATAL_ERROR "Needs conan package manager: please visit https://conan.io to install.")
endif()

set(CONAN_SCRIPT_FILE ${CMAKE_SOURCE_DIR}/scripts/conan.cmake)

if(NOT EXISTS ${CONAN_SCRIPT_FILE})
  message(STATUS "Downloading conan.cmake from https://github.com/memsharded/cmake-conan")

  file(DOWNLOAD "https://github.com/conan-io/cmake-conan/raw/master/conan.cmake" ${CONAN_SCRIPT_FILE})
endif()

include(${CONAN_SCRIPT_FILE})

conan_cmake_run(
    CONANFILE conanfile.txt
    BASIC_SETUP CMAKE_TARGETS BUILD missing
)

set(CMAKE_CXX_STANDARD 17)
enable_testing()

add_compile_options(-Wall -Werror -Wextra)

add_subdirectory(test)
add_subdirectory(src)

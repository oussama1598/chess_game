# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /home/red-scule/.local/share/JetBrains/Toolbox/apps/CLion/ch-0/201.6668.126/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /home/red-scule/.local/share/JetBrains/Toolbox/apps/CLion/ch-0/201.6668.126/bin/cmake/linux/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/red-scule/Desktop/projects/cpp_projects/chess_game

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/red-scule/Desktop/projects/cpp_projects/chess_game/cmake-build-debug

# Include any dependencies generated for this target.
include src/CMakeFiles/chess_game.dir/depend.make

# Include the progress variables for this target.
include src/CMakeFiles/chess_game.dir/progress.make

# Include the compile flags for this target's objects.
include src/CMakeFiles/chess_game.dir/flags.make

src/CMakeFiles/chess_game.dir/main.cpp.o: src/CMakeFiles/chess_game.dir/flags.make
src/CMakeFiles/chess_game.dir/main.cpp.o: ../src/main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/red-scule/Desktop/projects/cpp_projects/chess_game/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/CMakeFiles/chess_game.dir/main.cpp.o"
	cd /home/red-scule/Desktop/projects/cpp_projects/chess_game/cmake-build-debug/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/chess_game.dir/main.cpp.o -c /home/red-scule/Desktop/projects/cpp_projects/chess_game/src/main.cpp

src/CMakeFiles/chess_game.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/chess_game.dir/main.cpp.i"
	cd /home/red-scule/Desktop/projects/cpp_projects/chess_game/cmake-build-debug/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/red-scule/Desktop/projects/cpp_projects/chess_game/src/main.cpp > CMakeFiles/chess_game.dir/main.cpp.i

src/CMakeFiles/chess_game.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/chess_game.dir/main.cpp.s"
	cd /home/red-scule/Desktop/projects/cpp_projects/chess_game/cmake-build-debug/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/red-scule/Desktop/projects/cpp_projects/chess_game/src/main.cpp -o CMakeFiles/chess_game.dir/main.cpp.s

src/CMakeFiles/chess_game.dir/modules/Game.cpp.o: src/CMakeFiles/chess_game.dir/flags.make
src/CMakeFiles/chess_game.dir/modules/Game.cpp.o: ../src/modules/Game.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/red-scule/Desktop/projects/cpp_projects/chess_game/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object src/CMakeFiles/chess_game.dir/modules/Game.cpp.o"
	cd /home/red-scule/Desktop/projects/cpp_projects/chess_game/cmake-build-debug/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/chess_game.dir/modules/Game.cpp.o -c /home/red-scule/Desktop/projects/cpp_projects/chess_game/src/modules/Game.cpp

src/CMakeFiles/chess_game.dir/modules/Game.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/chess_game.dir/modules/Game.cpp.i"
	cd /home/red-scule/Desktop/projects/cpp_projects/chess_game/cmake-build-debug/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/red-scule/Desktop/projects/cpp_projects/chess_game/src/modules/Game.cpp > CMakeFiles/chess_game.dir/modules/Game.cpp.i

src/CMakeFiles/chess_game.dir/modules/Game.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/chess_game.dir/modules/Game.cpp.s"
	cd /home/red-scule/Desktop/projects/cpp_projects/chess_game/cmake-build-debug/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/red-scule/Desktop/projects/cpp_projects/chess_game/src/modules/Game.cpp -o CMakeFiles/chess_game.dir/modules/Game.cpp.s

src/CMakeFiles/chess_game.dir/modules/Board.cpp.o: src/CMakeFiles/chess_game.dir/flags.make
src/CMakeFiles/chess_game.dir/modules/Board.cpp.o: ../src/modules/Board.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/red-scule/Desktop/projects/cpp_projects/chess_game/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object src/CMakeFiles/chess_game.dir/modules/Board.cpp.o"
	cd /home/red-scule/Desktop/projects/cpp_projects/chess_game/cmake-build-debug/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/chess_game.dir/modules/Board.cpp.o -c /home/red-scule/Desktop/projects/cpp_projects/chess_game/src/modules/Board.cpp

src/CMakeFiles/chess_game.dir/modules/Board.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/chess_game.dir/modules/Board.cpp.i"
	cd /home/red-scule/Desktop/projects/cpp_projects/chess_game/cmake-build-debug/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/red-scule/Desktop/projects/cpp_projects/chess_game/src/modules/Board.cpp > CMakeFiles/chess_game.dir/modules/Board.cpp.i

src/CMakeFiles/chess_game.dir/modules/Board.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/chess_game.dir/modules/Board.cpp.s"
	cd /home/red-scule/Desktop/projects/cpp_projects/chess_game/cmake-build-debug/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/red-scule/Desktop/projects/cpp_projects/chess_game/src/modules/Board.cpp -o CMakeFiles/chess_game.dir/modules/Board.cpp.s

src/CMakeFiles/chess_game.dir/modules/pieces/Piece.cpp.o: src/CMakeFiles/chess_game.dir/flags.make
src/CMakeFiles/chess_game.dir/modules/pieces/Piece.cpp.o: ../src/modules/pieces/Piece.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/red-scule/Desktop/projects/cpp_projects/chess_game/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object src/CMakeFiles/chess_game.dir/modules/pieces/Piece.cpp.o"
	cd /home/red-scule/Desktop/projects/cpp_projects/chess_game/cmake-build-debug/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/chess_game.dir/modules/pieces/Piece.cpp.o -c /home/red-scule/Desktop/projects/cpp_projects/chess_game/src/modules/pieces/Piece.cpp

src/CMakeFiles/chess_game.dir/modules/pieces/Piece.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/chess_game.dir/modules/pieces/Piece.cpp.i"
	cd /home/red-scule/Desktop/projects/cpp_projects/chess_game/cmake-build-debug/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/red-scule/Desktop/projects/cpp_projects/chess_game/src/modules/pieces/Piece.cpp > CMakeFiles/chess_game.dir/modules/pieces/Piece.cpp.i

src/CMakeFiles/chess_game.dir/modules/pieces/Piece.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/chess_game.dir/modules/pieces/Piece.cpp.s"
	cd /home/red-scule/Desktop/projects/cpp_projects/chess_game/cmake-build-debug/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/red-scule/Desktop/projects/cpp_projects/chess_game/src/modules/pieces/Piece.cpp -o CMakeFiles/chess_game.dir/modules/pieces/Piece.cpp.s

src/CMakeFiles/chess_game.dir/modules/pieces/King.cpp.o: src/CMakeFiles/chess_game.dir/flags.make
src/CMakeFiles/chess_game.dir/modules/pieces/King.cpp.o: ../src/modules/pieces/King.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/red-scule/Desktop/projects/cpp_projects/chess_game/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object src/CMakeFiles/chess_game.dir/modules/pieces/King.cpp.o"
	cd /home/red-scule/Desktop/projects/cpp_projects/chess_game/cmake-build-debug/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/chess_game.dir/modules/pieces/King.cpp.o -c /home/red-scule/Desktop/projects/cpp_projects/chess_game/src/modules/pieces/King.cpp

src/CMakeFiles/chess_game.dir/modules/pieces/King.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/chess_game.dir/modules/pieces/King.cpp.i"
	cd /home/red-scule/Desktop/projects/cpp_projects/chess_game/cmake-build-debug/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/red-scule/Desktop/projects/cpp_projects/chess_game/src/modules/pieces/King.cpp > CMakeFiles/chess_game.dir/modules/pieces/King.cpp.i

src/CMakeFiles/chess_game.dir/modules/pieces/King.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/chess_game.dir/modules/pieces/King.cpp.s"
	cd /home/red-scule/Desktop/projects/cpp_projects/chess_game/cmake-build-debug/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/red-scule/Desktop/projects/cpp_projects/chess_game/src/modules/pieces/King.cpp -o CMakeFiles/chess_game.dir/modules/pieces/King.cpp.s

src/CMakeFiles/chess_game.dir/modules/pieces/Queen.cpp.o: src/CMakeFiles/chess_game.dir/flags.make
src/CMakeFiles/chess_game.dir/modules/pieces/Queen.cpp.o: ../src/modules/pieces/Queen.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/red-scule/Desktop/projects/cpp_projects/chess_game/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object src/CMakeFiles/chess_game.dir/modules/pieces/Queen.cpp.o"
	cd /home/red-scule/Desktop/projects/cpp_projects/chess_game/cmake-build-debug/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/chess_game.dir/modules/pieces/Queen.cpp.o -c /home/red-scule/Desktop/projects/cpp_projects/chess_game/src/modules/pieces/Queen.cpp

src/CMakeFiles/chess_game.dir/modules/pieces/Queen.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/chess_game.dir/modules/pieces/Queen.cpp.i"
	cd /home/red-scule/Desktop/projects/cpp_projects/chess_game/cmake-build-debug/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/red-scule/Desktop/projects/cpp_projects/chess_game/src/modules/pieces/Queen.cpp > CMakeFiles/chess_game.dir/modules/pieces/Queen.cpp.i

src/CMakeFiles/chess_game.dir/modules/pieces/Queen.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/chess_game.dir/modules/pieces/Queen.cpp.s"
	cd /home/red-scule/Desktop/projects/cpp_projects/chess_game/cmake-build-debug/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/red-scule/Desktop/projects/cpp_projects/chess_game/src/modules/pieces/Queen.cpp -o CMakeFiles/chess_game.dir/modules/pieces/Queen.cpp.s

src/CMakeFiles/chess_game.dir/modules/pieces/Rook.cpp.o: src/CMakeFiles/chess_game.dir/flags.make
src/CMakeFiles/chess_game.dir/modules/pieces/Rook.cpp.o: ../src/modules/pieces/Rook.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/red-scule/Desktop/projects/cpp_projects/chess_game/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object src/CMakeFiles/chess_game.dir/modules/pieces/Rook.cpp.o"
	cd /home/red-scule/Desktop/projects/cpp_projects/chess_game/cmake-build-debug/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/chess_game.dir/modules/pieces/Rook.cpp.o -c /home/red-scule/Desktop/projects/cpp_projects/chess_game/src/modules/pieces/Rook.cpp

src/CMakeFiles/chess_game.dir/modules/pieces/Rook.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/chess_game.dir/modules/pieces/Rook.cpp.i"
	cd /home/red-scule/Desktop/projects/cpp_projects/chess_game/cmake-build-debug/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/red-scule/Desktop/projects/cpp_projects/chess_game/src/modules/pieces/Rook.cpp > CMakeFiles/chess_game.dir/modules/pieces/Rook.cpp.i

src/CMakeFiles/chess_game.dir/modules/pieces/Rook.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/chess_game.dir/modules/pieces/Rook.cpp.s"
	cd /home/red-scule/Desktop/projects/cpp_projects/chess_game/cmake-build-debug/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/red-scule/Desktop/projects/cpp_projects/chess_game/src/modules/pieces/Rook.cpp -o CMakeFiles/chess_game.dir/modules/pieces/Rook.cpp.s

src/CMakeFiles/chess_game.dir/modules/pieces/Bishop.cpp.o: src/CMakeFiles/chess_game.dir/flags.make
src/CMakeFiles/chess_game.dir/modules/pieces/Bishop.cpp.o: ../src/modules/pieces/Bishop.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/red-scule/Desktop/projects/cpp_projects/chess_game/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object src/CMakeFiles/chess_game.dir/modules/pieces/Bishop.cpp.o"
	cd /home/red-scule/Desktop/projects/cpp_projects/chess_game/cmake-build-debug/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/chess_game.dir/modules/pieces/Bishop.cpp.o -c /home/red-scule/Desktop/projects/cpp_projects/chess_game/src/modules/pieces/Bishop.cpp

src/CMakeFiles/chess_game.dir/modules/pieces/Bishop.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/chess_game.dir/modules/pieces/Bishop.cpp.i"
	cd /home/red-scule/Desktop/projects/cpp_projects/chess_game/cmake-build-debug/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/red-scule/Desktop/projects/cpp_projects/chess_game/src/modules/pieces/Bishop.cpp > CMakeFiles/chess_game.dir/modules/pieces/Bishop.cpp.i

src/CMakeFiles/chess_game.dir/modules/pieces/Bishop.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/chess_game.dir/modules/pieces/Bishop.cpp.s"
	cd /home/red-scule/Desktop/projects/cpp_projects/chess_game/cmake-build-debug/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/red-scule/Desktop/projects/cpp_projects/chess_game/src/modules/pieces/Bishop.cpp -o CMakeFiles/chess_game.dir/modules/pieces/Bishop.cpp.s

src/CMakeFiles/chess_game.dir/modules/pieces/Pawn.cpp.o: src/CMakeFiles/chess_game.dir/flags.make
src/CMakeFiles/chess_game.dir/modules/pieces/Pawn.cpp.o: ../src/modules/pieces/Pawn.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/red-scule/Desktop/projects/cpp_projects/chess_game/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building CXX object src/CMakeFiles/chess_game.dir/modules/pieces/Pawn.cpp.o"
	cd /home/red-scule/Desktop/projects/cpp_projects/chess_game/cmake-build-debug/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/chess_game.dir/modules/pieces/Pawn.cpp.o -c /home/red-scule/Desktop/projects/cpp_projects/chess_game/src/modules/pieces/Pawn.cpp

src/CMakeFiles/chess_game.dir/modules/pieces/Pawn.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/chess_game.dir/modules/pieces/Pawn.cpp.i"
	cd /home/red-scule/Desktop/projects/cpp_projects/chess_game/cmake-build-debug/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/red-scule/Desktop/projects/cpp_projects/chess_game/src/modules/pieces/Pawn.cpp > CMakeFiles/chess_game.dir/modules/pieces/Pawn.cpp.i

src/CMakeFiles/chess_game.dir/modules/pieces/Pawn.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/chess_game.dir/modules/pieces/Pawn.cpp.s"
	cd /home/red-scule/Desktop/projects/cpp_projects/chess_game/cmake-build-debug/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/red-scule/Desktop/projects/cpp_projects/chess_game/src/modules/pieces/Pawn.cpp -o CMakeFiles/chess_game.dir/modules/pieces/Pawn.cpp.s

src/CMakeFiles/chess_game.dir/modules/pieces/Knight.cpp.o: src/CMakeFiles/chess_game.dir/flags.make
src/CMakeFiles/chess_game.dir/modules/pieces/Knight.cpp.o: ../src/modules/pieces/Knight.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/red-scule/Desktop/projects/cpp_projects/chess_game/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Building CXX object src/CMakeFiles/chess_game.dir/modules/pieces/Knight.cpp.o"
	cd /home/red-scule/Desktop/projects/cpp_projects/chess_game/cmake-build-debug/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/chess_game.dir/modules/pieces/Knight.cpp.o -c /home/red-scule/Desktop/projects/cpp_projects/chess_game/src/modules/pieces/Knight.cpp

src/CMakeFiles/chess_game.dir/modules/pieces/Knight.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/chess_game.dir/modules/pieces/Knight.cpp.i"
	cd /home/red-scule/Desktop/projects/cpp_projects/chess_game/cmake-build-debug/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/red-scule/Desktop/projects/cpp_projects/chess_game/src/modules/pieces/Knight.cpp > CMakeFiles/chess_game.dir/modules/pieces/Knight.cpp.i

src/CMakeFiles/chess_game.dir/modules/pieces/Knight.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/chess_game.dir/modules/pieces/Knight.cpp.s"
	cd /home/red-scule/Desktop/projects/cpp_projects/chess_game/cmake-build-debug/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/red-scule/Desktop/projects/cpp_projects/chess_game/src/modules/pieces/Knight.cpp -o CMakeFiles/chess_game.dir/modules/pieces/Knight.cpp.s

src/CMakeFiles/chess_game.dir/Errors.cpp.o: src/CMakeFiles/chess_game.dir/flags.make
src/CMakeFiles/chess_game.dir/Errors.cpp.o: ../src/Errors.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/red-scule/Desktop/projects/cpp_projects/chess_game/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_11) "Building CXX object src/CMakeFiles/chess_game.dir/Errors.cpp.o"
	cd /home/red-scule/Desktop/projects/cpp_projects/chess_game/cmake-build-debug/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/chess_game.dir/Errors.cpp.o -c /home/red-scule/Desktop/projects/cpp_projects/chess_game/src/Errors.cpp

src/CMakeFiles/chess_game.dir/Errors.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/chess_game.dir/Errors.cpp.i"
	cd /home/red-scule/Desktop/projects/cpp_projects/chess_game/cmake-build-debug/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/red-scule/Desktop/projects/cpp_projects/chess_game/src/Errors.cpp > CMakeFiles/chess_game.dir/Errors.cpp.i

src/CMakeFiles/chess_game.dir/Errors.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/chess_game.dir/Errors.cpp.s"
	cd /home/red-scule/Desktop/projects/cpp_projects/chess_game/cmake-build-debug/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/red-scule/Desktop/projects/cpp_projects/chess_game/src/Errors.cpp -o CMakeFiles/chess_game.dir/Errors.cpp.s

# Object files for target chess_game
chess_game_OBJECTS = \
"CMakeFiles/chess_game.dir/main.cpp.o" \
"CMakeFiles/chess_game.dir/modules/Game.cpp.o" \
"CMakeFiles/chess_game.dir/modules/Board.cpp.o" \
"CMakeFiles/chess_game.dir/modules/pieces/Piece.cpp.o" \
"CMakeFiles/chess_game.dir/modules/pieces/King.cpp.o" \
"CMakeFiles/chess_game.dir/modules/pieces/Queen.cpp.o" \
"CMakeFiles/chess_game.dir/modules/pieces/Rook.cpp.o" \
"CMakeFiles/chess_game.dir/modules/pieces/Bishop.cpp.o" \
"CMakeFiles/chess_game.dir/modules/pieces/Pawn.cpp.o" \
"CMakeFiles/chess_game.dir/modules/pieces/Knight.cpp.o" \
"CMakeFiles/chess_game.dir/Errors.cpp.o"

# External object files for target chess_game
chess_game_EXTERNAL_OBJECTS =

bin/chess_game: src/CMakeFiles/chess_game.dir/main.cpp.o
bin/chess_game: src/CMakeFiles/chess_game.dir/modules/Game.cpp.o
bin/chess_game: src/CMakeFiles/chess_game.dir/modules/Board.cpp.o
bin/chess_game: src/CMakeFiles/chess_game.dir/modules/pieces/Piece.cpp.o
bin/chess_game: src/CMakeFiles/chess_game.dir/modules/pieces/King.cpp.o
bin/chess_game: src/CMakeFiles/chess_game.dir/modules/pieces/Queen.cpp.o
bin/chess_game: src/CMakeFiles/chess_game.dir/modules/pieces/Rook.cpp.o
bin/chess_game: src/CMakeFiles/chess_game.dir/modules/pieces/Bishop.cpp.o
bin/chess_game: src/CMakeFiles/chess_game.dir/modules/pieces/Pawn.cpp.o
bin/chess_game: src/CMakeFiles/chess_game.dir/modules/pieces/Knight.cpp.o
bin/chess_game: src/CMakeFiles/chess_game.dir/Errors.cpp.o
bin/chess_game: src/CMakeFiles/chess_game.dir/build.make
bin/chess_game: src/CMakeFiles/chess_game.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/red-scule/Desktop/projects/cpp_projects/chess_game/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_12) "Linking CXX executable ../bin/chess_game"
	cd /home/red-scule/Desktop/projects/cpp_projects/chess_game/cmake-build-debug/src && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/chess_game.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/CMakeFiles/chess_game.dir/build: bin/chess_game

.PHONY : src/CMakeFiles/chess_game.dir/build

src/CMakeFiles/chess_game.dir/clean:
	cd /home/red-scule/Desktop/projects/cpp_projects/chess_game/cmake-build-debug/src && $(CMAKE_COMMAND) -P CMakeFiles/chess_game.dir/cmake_clean.cmake
.PHONY : src/CMakeFiles/chess_game.dir/clean

src/CMakeFiles/chess_game.dir/depend:
	cd /home/red-scule/Desktop/projects/cpp_projects/chess_game/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/red-scule/Desktop/projects/cpp_projects/chess_game /home/red-scule/Desktop/projects/cpp_projects/chess_game/src /home/red-scule/Desktop/projects/cpp_projects/chess_game/cmake-build-debug /home/red-scule/Desktop/projects/cpp_projects/chess_game/cmake-build-debug/src /home/red-scule/Desktop/projects/cpp_projects/chess_game/cmake-build-debug/src/CMakeFiles/chess_game.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/CMakeFiles/chess_game.dir/depend


# Chess_game

This project was part of my academic projects, its main goal was to build the chess engine from the ground up, that's exactly what was accomplished alongside with two particular renderers, 2D, 3D.

# Project's Structure
  
The project was structured in a very simple yet elegant way, where every major section of it was extracted to its own library. in the end, we had libraries that are separate from each other and can be compiled separately.

#### 1 - Chess engine
The main chess engine was built in a way that does not require any external rendering libraries. it exposes some functionality so that it can be linked to any other rendering engine, whether to the console or 2D/3D.
The engine does perform most of the moves the chess has, it takes an input as a string the "from", "to" that represents a single move on the board.

#### 2 - 2D renderer
The 2d renderer uses SDL to create and populate a window with the required data. It takes that chess engine, and print on the screen the all the data the engine has and performs the movement with a single drag and drop.

### 3 - 3D renderer
The 3d renderer is a lot more complex than the 2D, as it uses the low-level graphics API with the help of OpenGL, it was one of the hardest tasks within this project, as the knowledge requires is far from what I had.
I used some simplest shaders and simplest lighting in this engine but it still was a major component that took 60% of the project's time.

### 4 - The AI

The Ai wasn't pretty hard, as I applied the minimax-alpha-beta algorithm. this algorithm is straightforward and doesn't require a lot, and since the chess engine, had all the functionality needed for it to work, it was as easy as to create a search tree and evaluation the possible movements with a predefined evaluation function.

# Screenshots

| Main Window   |      About Window      |
|-------------|:-------------:|
| ![1](https://github.com/oussama1598/chess_game/blob/master/screenshots/main_window.png?raw=true) |  ![2](https://github.com/oussama1598/chess_game/blob/master/screenshots/about_window.png?raw=true) |


| New Game with human as an opponent and 2d renderer   |      New Game with computer as an opponent and 2d renderer      |
|-------------|:-------------:|
| ![3](https://github.com/oussama1598/chess_game/blob/master/screenshots/new_game_window_human.png?raw=true) |  ![4](https://github.com/oussama1598/chess_game/blob/master/screenshots/new_game_window_computer_2d.png?raw=true) |


| 2d renderer   |      3d renderer      |
|-------------|:-------------:|
| ![5](https://github.com/oussama1598/chess_game/blob/master/screenshots/game_window_2d.png?raw=true) |  ![6](https://github.com/oussama1598/chess_game/blob/master/screenshots/game_window_3d.png?raw=true) |

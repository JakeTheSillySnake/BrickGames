# BrickGames (Snake & Tetris)

_Last updated September 2024_

## Introduction

This document describes how to install, run, check and remove Snake and Tetris on Linux and Unix-like systems. Please note that the program was written and tested on Ubuntu 22.04 LTS, so its behaviour may differ if you use other versions or OS distributions.

</br>

## Installation & Running

### Prerequisites

Correct compilation and running of the program depends on other utilities and libraries. Check that you have their latest versions before proceeding: 

| Compilation targets | Utilities |
| ------------------- | --------- |
| Basics, Command Line Interface | gcc, make |
| Desktop Interface | Qt library, qmake |
| Testing | GTest library |
| GCOV Report | gcov, lcov |
| Leaks Check | valgrind |
| Convert Manual to DVI | texi2dvi |

</br>

### Setup

Download or clone (`git clone <link_to_repo>`) the source repository to where you can easily find it. Then type and run the following commands in the terminal: 

1. `cd <path-to-games-folder>/src`

2. `make install`

Now the program is compiled, placing all necessary files in a single folder named games/. To start either game, run: 

1. For Command Line Interafce: `make tetris_cli` OR `make snake_cli`

2. For Desktop Interface: `make tetris_desk` OR `make snake_desk`

If there are errors, you're likely missing some packages. Check __Prerequisites__.

</br>

### How to play

This program is a reimagining of classic Tetris and Snake. In Tetris, your task is to make horizontal lines from randomly generated falling pieces. Each completed line earns you points, insreasing the level of difficulty and speed. You lose once the pieces reach the upper border. Pay attention to the following controls: 

| Action	| CLI key |	Desktop key |
| ------- | ------- | ----------- |
| (Re)start	| S |	X |
| Pause |	Space |	Space |
| Quit | Q	| Exit button |
| Left |	Left Arrow |	A |
| Right	| Right Arrow	| D |
| Down |	Down Arrow	| S |
| Rotate	| Enter	| W |

In Snake, you control a “snake” that moves around a field. Collecting “apples” that appear on the field earns you points, increasing the snake's length and speed. You lose if the snake crashes into the walls or itself. 

| Action	| CLI key |	Desktop key |
| ------- | ------- | ----------- |
| (Re)start	| S |	X |
| Pause |	Space |	Space |
| Quit | Q	| Exit button |
| Left |	Left Arrow |	A |
| Right	| Right Arrow	| D |
| Down |	Down Arrow	| S |
| Up	| Up Arrow	| W |

</br>

## Game structure & testing

The games stick to the C++17 language and standard libraries, with the desktop interface provided by the Qt5. Both use a finite state machine (FSM) to manage the game's state. The source code can be found in brick_game/tetris and brick_game/snake. It controls switching between the states: 

| State | Definition |
| ----- | ---------- |
| Start | beginning of the game | 
| Spawn | generation of a new tetris piece/apple |
| Shifting | tetris piece shifts downs/snake moves forward after a set time period |
| Moving | tetris piece/snake responds to the user's actions |
| Colliding | tetris piece reaches the bottom or lands on another piece/snake collides with itself or walls |
| Pause | the game stops updating the field |
| Gameover | the game ends when tetris pieces reach the top/snake collides with itself or walls |
| Exit | termination of the game |

The games close if the files storing the high scores can't be accessed. FSM continuously returns updated game field and stats depending on the user's action.

Additionally, Snake uses a VCM model, where the source code is divided between the model, controller and viewer classes. The model contains all the relevant information about the game, the controller accepts the user actions and passes them to the model, and the viewer displays the current model and processes the user input.

The backend libraries can be tested with GTest: 

1. To run tests: `make test_snake` OR `make test_tetris`

2. To display test coverage: `make report_snake` OR `make report_tetris`

3. To check for leaks: `make valgrind`

Running `make` or `make all` will reinstall and compile the program. You can get DVI documentation with `make dvi` or a distribution .tar.gz package with `make dist`.

</br>

## Deinstallation

Simply run make uninstall or make clean. This will remove the games/ directory but not the original download, which can be safely deleted afterwards.

If you wish to suggest an improvement or report a bug, contact me __@ginzburg_jake (Telegram) or JakeTheSillySnake (GitHub)__.

#ifndef BACKEND_H
#define BACKEND_H

#include <time.h>

#include <cstdlib>
#include <iostream>

/*colors for output*/
#define GREEN "\033[0;32m"
#define RED "\033[0;31m"
#define BLUE "\033[0;34m"
#define YELLOW "\033[0;33m"
#define CYAN "\033[0;36m"
#define PURPLE "\033[0;35m"
#define DEFAULT "\033[0m"

/*field logic*/
#define ROWS 20
#define COLUMNS 10
#define POINTS 100
#define SPAWN_X 3
#define SPAWN_Y 2
#define TIMEOUT 300
#define FIG_COUNT 7
#define SCORE_PATH "assets/tetris_score.txt"

#define SUCCESS 0
#define FAILURE 1
#define FERROR "File 'tetris_score.txt' is missing or damaged."

/*matrix cell states*/
#define EMPTY 0
#define STATIC 1
#define MOVING 2

/*game pieces*/
#define SQUARE 0
#define LINE 1
#define LENGTH_MAX 4

/*figure blocks*/
const int blocks[FIG_COUNT][LENGTH_MAX * LENGTH_MAX] = {
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 1, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 1, 0, 0}};

typedef enum {
  Start = 0,
  Pause,
  Terminate,
  Left,
  Right,
  Down,
  Action,
  Nosig
} UserAction_t;

typedef enum {
  START = 0,
  SPAWN,
  MOVE,
  SHIFT,
  ATTACH,
  GAMEOVER,
  EXIT,
  PAUSE
} State_t;

typedef struct {
  int field[ROWS + LENGTH_MAX][COLUMNS] = {0};
  int next[LENGTH_MAX][LENGTH_MAX] = {0};
  const char *colors[6] = {GREEN, RED, BLUE, YELLOW, CYAN, PURPLE};
  int score = 0;
  int high_score = 0;
  int level = 1;
  int speed = 1;
  int pause = 0;
  int id = 0;
  int id_next = 0;
  int length = 0;
  int color_num = 0;
  char *color = 0;
  int x = 0;
  int y = 0;
} GameInfo_t;

namespace s21 {

/*start*/
void init_stats(GameInfo_t *game);

/*user actions*/
void shift(GameInfo_t *game);
void collide(GameInfo_t *game);
void move_left(GameInfo_t *game);
void move_right(GameInfo_t *game);
void rotate_piece(GameInfo_t *game);
void reverse_rows(int **matrix, int size);

/*end of round*/
void generate_piece(GameInfo_t *game);
void spawn_piece(GameInfo_t *game);
void count_points(GameInfo_t *game);
void write_highscore(int score);
void erase_line(GameInfo_t *game, int index);

/*action checks*/
int check_boundaryY(GameInfo_t *game);
int check_boundaryX(GameInfo_t *game, int shift);
int check_rotation(GameInfo_t *game, int **rotated, int size);
}  // namespace s21

#endif
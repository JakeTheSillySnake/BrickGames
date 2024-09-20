#ifndef MODEL_H
#define MODEL_H

#include <cstdlib>
#include <iostream>
#include <vector>

#define ROWS 20
#define COLUMNS 10
#define EMPTY 0
#define SNAKE 1
#define APPLE 2
#define TIMEOUT 300
#define MAX_LENGTH 200

enum { UP = 0, LEFT, DOWN, RIGHT };

typedef enum { START = 0, MOVE, SHIFT, EXIT, PAUSE, GAMEOVER } State_t;

typedef struct SnakeBit_t {
  int x;
  int y;
  int dir;
} SnakeBit_t;

typedef struct GameInfo_t {
  int field[ROWS][COLUMNS];
  std::vector<SnakeBit_t> coords;
  SnakeBit_t tail;
  int apple_x;
  int apple_y;
  int score;
  int high_score;
  int level;
  int speed;
} GameInfo_t;

namespace s21 {
class SnakeModel {
 private:
  GameInfo_t game;

 public:
  SnakeModel() { reset(); };

  void get_score();
  void set_score();
  void spawn();
  void move_left();
  void move_right();
  void shift();
  int check_collide();
  void reset();
  void clear_field();
  GameInfo_t get_data();
};
}  // namespace s21
#endif
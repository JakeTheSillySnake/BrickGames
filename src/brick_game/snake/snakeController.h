#ifndef CONTROL_H
#define CONTROL_H

#include "snakeModel.h"

namespace s21 {
class Controller {
 private:
  SnakeModel *model;
  State_t state;

 public:
  Controller(SnakeModel *m, State_t s = START) : model(m), state(s){};

  void start();
  void move_right();
  void move_left();
  void move_down();
  void move_up();
  void pause();
  void shift();
  void terminate();
  GameInfo_t get_data();
  State_t get_state();
};
}  // namespace s21

#endif
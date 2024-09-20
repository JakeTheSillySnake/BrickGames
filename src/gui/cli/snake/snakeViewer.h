#ifndef CONSOLEVIEW_H
#define CONSOLEVIEW_H

#include <ncurses.h>

#include "../../../brick_game/snake/snakeController.h"

#define GREEN "\033[0;32m"
#define RED "\033[0;31m"
#define DEFAULT "\033[0m"

namespace s21 {
class View {
 private:
  Controller *controller;

 public:
  View(Controller *c) : controller(c) {}
  void print_field();
  void print_head();
  void print_stats(int i, GameInfo_t game);
  void game_over();
  void get_signal(char input);
  GameInfo_t get_data();
  void event_loop();
};
}  // namespace s21

#endif
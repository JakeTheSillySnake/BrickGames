#include "snakeController.h"

using namespace s21;

void Controller::shift() {
  if (state != SHIFT && state != MOVE) return;
  if (!model->check_collide() && get_data().coords.size() != MAX_LENGTH) {
    model->shift();
    state = MOVE;
  } else
    state = GAMEOVER;
}

void Controller::move_left() {
  if (state != MOVE) return;
  if (get_data().coords[0].dir == UP) {
    model->move_left();
  } else if (get_data().coords[0].dir == DOWN) {
    model->move_right();
  }
  state = SHIFT;
}

void Controller::move_right() {
  if (state != MOVE) return;
  if (get_data().coords[0].dir == UP) {
    model->move_right();
  } else if (get_data().coords[0].dir == DOWN) {
    model->move_left();
  }
  state = SHIFT;
}

void Controller::move_down() {
  if (state != MOVE) return;
  if (get_data().coords[0].dir == LEFT) {
    model->move_left();
  } else if (get_data().coords[0].dir == RIGHT) {
    model->move_right();
  }
  state = SHIFT;
}

void Controller::move_up() {
  if (state != MOVE) return;
  if (get_data().coords[0].dir == LEFT) {
    model->move_right();
  } else if (get_data().coords[0].dir == RIGHT) {
    model->move_left();
  }
  state = SHIFT;
}

void Controller::pause() {
  if (state == PAUSE)
    state = SHIFT;
  else if (state != START && state != GAMEOVER)
    state = PAUSE;
}

void Controller::start() {
  if (state == GAMEOVER) {
    model->reset();
    state = SHIFT;
  } else if (state == START)
    state = SHIFT;
}

void Controller::terminate() { state = EXIT; }

GameInfo_t Controller::get_data() { return model->get_data(); }

State_t Controller::get_state() { return state; }
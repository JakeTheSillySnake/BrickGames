#include "tetrisController.h"

/*fsm table*/
action fsm_table[8][8] = {
    {s21::spawn, NULL, s21::exitstate, NULL, NULL, NULL, NULL, NULL},
    {s21::spawn, s21::spawn, s21::spawn, s21::spawn, s21::spawn, s21::spawn,
     s21::spawn, s21::spawn},
    {NULL, s21::stop, s21::exitstate, s21::moveleft, s21::moveright,
     s21::movedown, s21::rotate, s21::shifting},
    {s21::shifting, s21::stop, s21::shifting, s21::shifting, s21::shifting,
     s21::shifting, s21::shifting, s21::shifting},
    {s21::attach, s21::attach, s21::attach, s21::attach, s21::attach,
     s21::attach, s21::attach, s21::attach},
    {s21::spawn, NULL, s21::exitstate, NULL, NULL, NULL, NULL, NULL},
    {s21::exitstate, s21::exitstate, s21::exitstate, s21::exitstate,
     s21::exitstate, s21::exitstate, s21::exitstate, s21::exitstate},
    {NULL, s21::stop, s21::exitstate, NULL, NULL, NULL, NULL, NULL}};

void s21::userInput(UserAction_t signal, GameInfo_t *game, State_t *state) {
  action act = NULL;
  Params_t prms;
  prms.game = game;
  prms.state = state;

  act = fsm_table[*state][signal];
  if (act) {
    act(&prms);
  }
}

void s21::spawn(Params_t *prms) {
  if (*prms->state == GAMEOVER) {
    init_stats(prms->game);
  }
  if (prms->game->id == 0) {
    generate_piece(prms->game);
  }
  *prms->state = MOVE;
  spawn_piece(prms->game);
  generate_piece(prms->game);
}

void s21::shifting(Params_t *prms) {
  if (check_boundaryY(prms->game) == SUCCESS) {
    shift(prms->game);
    *prms->state = MOVE;
  } else {
    collide(prms->game);
    *prms->state = ATTACH;
  }
}

void s21::movedown(Params_t *prms) {
  while (check_boundaryY(prms->game) == SUCCESS) {
    shift(prms->game);
  }
  *prms->state = MOVE;
}

void s21::moveright(Params_t *prms) {
  if (check_boundaryX(prms->game, 1) == SUCCESS) {
    move_right(prms->game);
  }
  *prms->state = SHIFT;
}

void s21::moveleft(Params_t *prms) {
  if (check_boundaryX(prms->game, -1) == SUCCESS) {
    move_left(prms->game);
  }
  *prms->state = SHIFT;
}

void s21::rotate(Params_t *prms) {
  rotate_piece(prms->game);
  *prms->state = SHIFT;
}

void s21::attach(Params_t *prms) {
  if (prms->game->y == SPAWN_Y) {
    *prms->state = GAMEOVER;
  } else {
    count_points(prms->game);
    *prms->state = SPAWN;
  }
}

void s21::stop(Params_t *prms) {
  if (prms->game->pause) {
    prms->game->pause = 0;
    *prms->state = SHIFT;
  } else {
    prms->game->pause = 1;
    *prms->state = PAUSE;
  }
}

void s21::exitstate(Params_t *prms) { *prms->state = EXIT; }

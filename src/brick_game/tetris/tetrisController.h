#ifndef FSM_H
#define FSM_H

#include "tetrisModel.h"

/*fsm table dependants*/
typedef struct {
  GameInfo_t *game;
  State_t *state;
} Params_t;

typedef void (*action)(Params_t *prms);

namespace s21 {
/*state functions*/
void spawn(Params_t *prms);
void movedown(Params_t *prms);
void moveright(Params_t *prms);
void moveleft(Params_t *prms);
void shifting(Params_t *prms);
void rotate(Params_t *prms);
void attach(Params_t *prms);
void stop(Params_t *prms);
void exitstate(Params_t *prms);

/*function for input processing*/
void userInput(UserAction_t signal, GameInfo_t *game, State_t *state);
}  // namespace s21
#endif
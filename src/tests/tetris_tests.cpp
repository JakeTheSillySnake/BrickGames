#include <gtest/gtest.h>

#include "../brick_game/tetris/tetrisController.h"

using namespace s21;

TEST(test_tetris, updateCurrentState_1) {
  GameInfo_t game;
  State_t state = START;
  userInput(Start, &game, &state);
  ASSERT_EQ(state, MOVE);
}

TEST(test_tetris, updateCurrentState_2) {
  GameInfo_t game;
  game.score = 300;
  State_t state = ATTACH;
  userInput(Action, &game, &state);
  ASSERT_EQ(game.score, game.high_score);
}

TEST(test_tetris, updateCurrentState_3) {
  GameInfo_t game;
  State_t state = START;
  userInput(Right, &game, &state);
  ASSERT_EQ(state, START);
}

TEST(test_tetris, updateCurrentState_4) {
  GameInfo_t game;
  State_t state = SHIFT;
  userInput(Nosig, &game, &state);
  ASSERT_EQ(state, MOVE);
}

TEST(test_tetris, updateCurrentState_5) {
  GameInfo_t game;
  State_t state = START;
  userInput(Terminate, &game, &state);
  ASSERT_EQ(state, EXIT);
}

TEST(test_tetris, gameover_1) {
  GameInfo_t game;
  State_t state = ATTACH;
  game.x = SPAWN_X;
  game.y = SPAWN_Y;
  userInput(Nosig, &game, &state);
  ASSERT_EQ(state, GAMEOVER);
}

TEST(test_tetris, init_stats_1) {
  GameInfo_t game;
  init_stats(&game);
  ASSERT_EQ(game.level, 1);
}

TEST(test_tetris, shift_1) {
  GameInfo_t game;
  game.field[0][0] = MOVING;
  shift(&game);
  ASSERT_EQ(game.field[1][0], MOVING);
}

TEST(test_tetris, spawn_1) {
  GameInfo_t game;
  game.id = SQUARE;
  spawn_piece(&game);
  ASSERT_EQ(game.length, 3);
}

TEST(test_tetris, spawn_2) {
  GameInfo_t game;
  game.id_next = LINE;
  spawn_piece(&game);
  ASSERT_EQ(game.length, 4);
}

TEST(test_tetris, rotate_piece_1) {
  GameInfo_t game;
  State_t state = SPAWN;
  userInput(Nosig, &game, &state);
  userInput(Action, &game, &state);
  ASSERT_EQ(state, SHIFT);
}

TEST(test_tetris, rotate_piece_2) {
  GameInfo_t game;
  game.field[0][COLUMNS - 1] = MOVING;
  game.field[1][COLUMNS - 1] = MOVING;
  game.field[2][COLUMNS - 1] = MOVING;
  game.x = COLUMNS - 1;
  game.length = 3;
  rotate_piece(&game);
  ASSERT_EQ(game.field[0][COLUMNS - 1], MOVING);
}

TEST(test_tetris, rotate_piece_3) {
  GameInfo_t game;
  game.field[0][1] = MOVING;
  game.field[1][1] = MOVING;
  game.field[2][1] = MOVING;
  game.x = 0;
  game.length = 3;
  game.id = 2;
  rotate_piece(&game);
  ASSERT_EQ(game.field[0][1], EMPTY);
}

TEST(test_tetris, collide_1) {
  GameInfo_t game;
  State_t state = SHIFT;
  game.field[ROWS + LENGTH_MAX - 1][0] = MOVING;
  userInput(Nosig, &game, &state);
  ASSERT_EQ(state, ATTACH);
}

TEST(test_tetris, collide_2) {
  GameInfo_t game;
  game.field[ROWS + LENGTH_MAX - 1][0] = MOVING;
  collide(&game);
  ASSERT_EQ(game.field[ROWS + LENGTH_MAX - 1][0], STATIC);
}

TEST(test_tetris, count_points_1) {
  GameInfo_t game;
  State_t state = ATTACH;
  for (int j = 0; j < COLUMNS; j++) {
    game.field[ROWS + LENGTH_MAX - 1][j] = MOVING;
  }
  userInput(Nosig, &game, &state);
  ASSERT_EQ(game.score, POINTS);
}

TEST(test_tetris, count_points_2) {
  GameInfo_t game;
  game.level = 11;
  game.score = 300000;
  count_points(&game);
  ASSERT_EQ(game.level, 10);
}

TEST(test_tetris, move_left_1) {
  GameInfo_t game;
  game.field[ROWS + LENGTH_MAX - 1][1] = MOVING;
  move_left(&game);
  ASSERT_EQ(game.field[ROWS + LENGTH_MAX - 1][0], MOVING);
}

TEST(test_tetris, move_left_2) {
  GameInfo_t game;
  State_t state = MOVE;
  game.field[ROWS + LENGTH_MAX - 1][1] = MOVING;
  Params_t prms;
  prms.game = &game;
  prms.state = &state;
  moveleft(&prms);
  ASSERT_EQ(game.field[ROWS + LENGTH_MAX - 1][0], MOVING);
}

TEST(test_tetris, move_right_1) {
  GameInfo_t game;
  game.field[ROWS + LENGTH_MAX - 1][0] = MOVING;
  move_right(&game);
  ASSERT_EQ(game.field[ROWS + LENGTH_MAX - 1][1], MOVING);
}

TEST(test_tetris, move_right_2) {
  GameInfo_t game;
  State_t state = MOVE;
  game.field[ROWS + LENGTH_MAX - 1][0] = MOVING;
  Params_t prms;
  prms.game = &game;
  prms.state = &state;
  moveright(&prms);
  ASSERT_EQ(game.field[ROWS + LENGTH_MAX - 1][1], MOVING);
}

TEST(test_tetris, check_boundaryX_1) {
  GameInfo_t game;
  game.field[ROWS + LENGTH_MAX - 1][0] = MOVING;
  int result = check_boundaryX(&game, -1);
  ASSERT_EQ(result, FAILURE);
}

TEST(test_tetris, check_boundaryX_2) {
  GameInfo_t game;
  game.field[ROWS + LENGTH_MAX - 1][COLUMNS - 1] = MOVING;
  int result = check_boundaryX(&game, 1);
  ASSERT_EQ(result, FAILURE);
}

TEST(test_tetris, check_boundaryX_3) {
  GameInfo_t game;
  game.field[0][0] = MOVING;
  int result = check_boundaryX(&game, 1);
  ASSERT_EQ(result, SUCCESS);
}

TEST(test_tetris, check_boundaryX_4) {
  GameInfo_t game;
  game.field[COLUMNS - 1][COLUMNS - 1] = MOVING;
  int result = check_boundaryX(&game, -1);
  ASSERT_EQ(result, SUCCESS);
}

TEST(test_tetris, move_down_1) {
  GameInfo_t game;
  State_t state = MOVE;
  game.field[0][0] = MOVING;
  Params_t prms;
  prms.game = &game;
  prms.state = &state;
  movedown(&prms);
  ASSERT_EQ(game.field[ROWS + LENGTH_MAX - 1][0], MOVING);
}

TEST(test_tetris, check_boundaryY_1) {
  GameInfo_t game;
  game.field[ROWS + LENGTH_MAX - 1][0] = MOVING;
  int result = check_boundaryY(&game);
  ASSERT_EQ(result, FAILURE);
}

TEST(test_tetris, check_boundaryY_2) {
  GameInfo_t game;
  game.field[0][0] = MOVING;
  int result = check_boundaryY(&game);
  ASSERT_EQ(result, SUCCESS);
}

TEST(test_tetris, pause_1) {
  GameInfo_t game;
  State_t state = SPAWN;
  userInput(Pause, &game, &state);
  ASSERT_EQ(game.pause, 1);
}

TEST(test_tetris, pause_2) {
  GameInfo_t game;
  State_t state = PAUSE;
  game.pause = 1;
  userInput(Pause, &game, &state);
  ASSERT_EQ(state, SHIFT);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
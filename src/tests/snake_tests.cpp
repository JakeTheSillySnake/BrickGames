#include <gtest/gtest.h>

#include "../brick_game/snake/snakeController.h"

using namespace s21;

TEST(test_Controller, constr) {
  SnakeModel model;
  Controller controller(&model);
  ASSERT_TRUE(controller.get_data().level == 1);
  controller.terminate();
  ASSERT_TRUE(controller.get_state() == EXIT);
}

TEST(test_Controller, shift) {
  SnakeModel model;
  Controller controller(&model);
  controller.start();
  ASSERT_TRUE(controller.get_state() == SHIFT);
  while (controller.get_state() != GAMEOVER) controller.shift();
  ASSERT_TRUE(model.get_data().field[0][5] == SNAKE);
  controller.start();
  ASSERT_TRUE(controller.get_state() == SHIFT);
}

TEST(test_Controller, move_left_up) {
  SnakeModel model;
  Controller controller(&model);
  controller.start();
  controller.shift();
  controller.move_left();
  ASSERT_TRUE(controller.get_data().coords[0].dir == LEFT);
  controller.shift();
  controller.move_up();
  ASSERT_TRUE(controller.get_data().coords[0].dir == UP);
}

TEST(test_Controller, move_right_up) {
  SnakeModel model;
  Controller controller(&model);
  controller.start();
  controller.shift();
  controller.move_right();
  ASSERT_TRUE(controller.get_data().coords[0].dir == RIGHT);
  controller.shift();
  controller.move_up();
  ASSERT_TRUE(controller.get_data().coords[0].dir == UP);
}

TEST(test_Controller, move_left_down) {
  SnakeModel model;
  Controller controller(&model);
  controller.start();
  controller.shift();
  controller.move_left();
  controller.shift();
  controller.move_down();
  ASSERT_TRUE(controller.get_data().coords[0].dir == DOWN);
  controller.shift();
  controller.move_left();
  ASSERT_TRUE(controller.get_data().coords[0].dir == LEFT);
}

TEST(test_Controller, move_right_down) {
  SnakeModel model;
  Controller controller(&model);
  controller.start();
  controller.shift();
  controller.move_right();
  controller.shift();
  controller.move_down();
  ASSERT_TRUE(controller.get_data().coords[0].dir == DOWN);
  controller.shift();
  controller.move_right();
  ASSERT_TRUE(controller.get_data().coords[0].dir == RIGHT);
}

TEST(test_Controller, pause_game) {
  SnakeModel model;
  Controller controller(&model);
  controller.start();
  controller.pause();
  ASSERT_TRUE(controller.get_state() == PAUSE);
  controller.pause();
  ASSERT_TRUE(controller.get_state() == SHIFT);
}

TEST(test_Model, set_score) {
  SnakeModel model;
  model.set_score();
  ASSERT_TRUE(model.get_data().score == 1);
  ASSERT_TRUE(model.get_data().level == 1);
  ASSERT_TRUE(model.get_data().speed == 1);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
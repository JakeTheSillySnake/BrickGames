#include "snakeModel.h"

using namespace s21;

void SnakeModel::get_score() {
  FILE *fptr = NULL;
  fptr = fopen("assets/snake_score.txt", "r");
  int res = 0;
  if (fptr) {
    res = fscanf(fptr, "%d", &game.high_score);
    fclose(fptr);
    if (res != 1)
      throw std::runtime_error(
          "Couldn't overwrite contents of 'snake_score.txt.'");
  } else
    throw std::runtime_error("File 'snake_score.txt' is missing or damaged.");
}

void SnakeModel::set_score() {
  if (++game.score > game.high_score) game.high_score++;
  game.level = game.score / 5 + 1;
  game.level = game.level > 10 ? 10 : game.level;
  game.speed = game.level;
  FILE *fptr = NULL;
  fptr = fopen("assets/snake_score.txt", "w");
  if (fptr) {
    fprintf(fptr, "%d", game.high_score);
    fclose(fptr);
  } else
    throw std::runtime_error("File 'snake_score.txt' is missing or damaged.");
}

void SnakeModel::spawn() {
  do {
    game.apple_y = rand() % ROWS;
    game.apple_x = rand() % COLUMNS;
  } while (game.field[game.apple_y][game.apple_x] == SNAKE);
}

void SnakeModel::move_left() {
  if (game.coords[0].dir == UP) {
    game.coords[0].dir = LEFT;
  } else if (game.coords[0].dir == LEFT) {
    game.coords[0].dir = DOWN;
  } else if (game.coords[0].dir == DOWN) {
    game.coords[0].dir = RIGHT;
  } else
    game.coords[0].dir = UP;
}

void SnakeModel::move_right() {
  if (game.coords[0].dir == UP) {
    game.coords[0].dir = RIGHT;
  } else if (game.coords[0].dir == RIGHT) {
    game.coords[0].dir = DOWN;
  } else if (game.coords[0].dir == DOWN) {
    game.coords[0].dir = LEFT;
  } else
    game.coords[0].dir = UP;
}

void SnakeModel::shift() {
  clear_field();
  game.tail = game.coords[game.coords.size() - 1];
  for (int i = game.coords.size() - 1; i >= 0; i--) {
    if (game.coords[i].dir == UP) {
      game.coords[i].y--;
    } else if (game.coords[i].dir == LEFT) {
      game.coords[i].x--;
    } else if (game.coords[i].dir == RIGHT) {
      game.coords[i].x++;
    } else if (game.coords[i].dir == DOWN) {
      game.coords[i].y++;
    }
    game.field[game.coords[i].y][game.coords[i].x] = SNAKE;
    if (i) game.coords[i].dir = game.coords[i - 1].dir;
  }
  if (game.coords[0].y == game.apple_y && game.coords[0].x == game.apple_x) {
    spawn();
    set_score();
    game.coords.push_back(game.tail);
  }
  game.field[game.apple_y][game.apple_x] = APPLE;
}

int SnakeModel::check_collide() {
  bool check = false;
  if ((game.coords[0].dir == UP &&
       (game.coords[0].y == 0 ||
        game.field[game.coords[0].y - 1][game.coords[0].x] == SNAKE)) ||
      (game.coords[0].dir == LEFT &&
       (game.coords[0].x == 0 ||
        game.field[game.coords[0].y][game.coords[0].x - 1] == SNAKE)) ||
      (game.coords[0].dir == RIGHT &&
       (game.coords[0].x == COLUMNS - 1 ||
        game.field[game.coords[0].y][game.coords[0].x + 1] == SNAKE)) ||
      (game.coords[0].dir == DOWN &&
       (game.coords[0].y == ROWS - 1 ||
        game.field[game.coords[0].y + 1][game.coords[0].x] == SNAKE)))
    check = true;
  return check;
}

void SnakeModel::reset() {
  game.score = 0;
  game.level = 1;
  game.speed = 1;
  game.coords.clear();
  get_score();
  clear_field();
  spawn();
  for (int i = 0; i < 4; i++) {
    SnakeBit_t tmp;
    tmp.y = 10 + i;
    tmp.x = 5;
    tmp.dir = UP;
    game.coords.push_back(tmp);
    game.field[tmp.y][tmp.x] = SNAKE;
  }
}

void SnakeModel::clear_field() {
  for (int i = 0; i < ROWS; i++) {
    for (int j = 0; j < COLUMNS; j++) {
      game.field[i][j] = 0;
    }
  }
}

GameInfo_t SnakeModel::get_data() { return game; }
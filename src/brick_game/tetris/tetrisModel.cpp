#include "tetrisModel.h"

void s21::init_stats(GameInfo_t *game) {
  GameInfo_t temp;
  *game = temp;
  FILE *fptr = NULL;
  fptr = fopen(SCORE_PATH, "r");
  if (fptr) {
    int res = fscanf(fptr, "%d", &game->high_score);
    if (res != 1)
      throw std::runtime_error(
          "Couldn't overwrite contents of 'snake_score.txt.'");
    fclose(fptr);
  } else {
    throw std::runtime_error(FERROR);
  }
}

void s21::shift(GameInfo_t *game) {
  game->y++;
  for (int i = ROWS + LENGTH_MAX - 1; i >= 0; i--) {
    for (int j = COLUMNS - 1; j >= 0; j--) {
      if (game->field[i][j] == MOVING) {
        game->field[i + 1][j] = MOVING;
        game->field[i][j] = EMPTY;
      }
    }
  }
}

void s21::collide(GameInfo_t *game) {
  for (int i = 0; i < ROWS + LENGTH_MAX; i++) {
    for (int j = 0; j < COLUMNS; j++) {
      if (game->field[i][j] == MOVING) {
        game->field[i][j] = STATIC;
      }
    }
  }
}

void s21::move_left(GameInfo_t *game) {
  for (int i = 0; i < ROWS + LENGTH_MAX; i++) {
    for (int j = 0; j < COLUMNS; j++) {
      if (game->field[i][j] == MOVING) {
        game->field[i][j - 1] = MOVING;
        game->field[i][j] = EMPTY;
      }
    }
  }
  game->x--;
}

void s21::move_right(GameInfo_t *game) {
  for (int i = ROWS + LENGTH_MAX - 1; i >= 0; i--) {
    for (int j = COLUMNS - 1; j >= 0; j--) {
      if (game->field[i][j] == MOVING) {
        game->field[i][j + 1] = MOVING;
        game->field[i][j] = EMPTY;
      }
    }
  }
  game->x++;
}

void s21::generate_piece(GameInfo_t *game) {
  int figure = rand() % FIG_COUNT, count = 0;
  for (int i = 0; i < LENGTH_MAX; i++) {
    for (int j = 0; j < LENGTH_MAX; j++) {
      game->next[i][j] = blocks[figure][count++];
    }
  }
  game->id_next = figure;
}

void s21::spawn_piece(GameInfo_t *game) {
  for (int i = 0; i < LENGTH_MAX; i++) {
    for (int j = 0; j < LENGTH_MAX; j++) {
      if (game->next[i][j]) {
        game->field[i][j + SPAWN_X] = MOVING;
      }
    }
  }
  game->y = SPAWN_Y;
  game->x = SPAWN_X;
  int choice = rand() % 6;
  game->color = const_cast<char *>(game->colors[choice]);
  game->color_num = choice;

  game->id = game->id_next;
  game->length = 3;
  if (game->id == LINE) {
    game->length = 4;
  }
}

void s21::rotate_piece(GameInfo_t *game) {
  if (game->id == SQUARE) return;
  int size = game->length;
  int **rotated = new int *[size];

  for (int i = 0; i < size; i++) {
    rotated[i] = new int[size];
    for (int j = 0; j < size; j++) rotated[i][j] = 0;
  }
  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++) {
      if (game->field[i + game->y][j + game->x] != STATIC) {
        rotated[j][i] = game->field[i + game->y][j + game->x];  // transposing
      }
    }
  }
  s21::reverse_rows(rotated, size);
  int check = s21::check_rotation(game, rotated, size);
  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++) {
      if (game->field[i + game->y][j + game->x] != STATIC && check == SUCCESS) {
        game->field[i + game->y][j + game->x] = rotated[i][j];
      }
    }
  }
  for (int i = 0; i < size; i++) {
    delete[] rotated[i];
  }
  delete[] rotated;
}

void s21::reverse_rows(int **matrix, int size) {
  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size / 2; j++) {
      int temp = matrix[i][j];
      matrix[i][j] = matrix[i][size - 1 - j];
      matrix[i][size - 1 - j] = temp;
    }
  }
}

void s21::count_points(GameInfo_t *game) {
  for (int i = 0; i < ROWS + LENGTH_MAX; i++) {
    int count = 0;
    for (int j = 0; j < COLUMNS; j++) {
      if (game->field[i][j]) {
        count++;
      }
    }
    if (count == COLUMNS) {
      s21::erase_line(game, i);
      game->score *= 2;
      game->score += POINTS;
    }
  }
  game->level = game->score / 600 + 1;
  if (game->level > 10) {
    game->level = 10;
  }
  game->speed = game->level;
  if (game->score > game->high_score) {
    game->high_score = game->score;
    s21::write_highscore(game->high_score);
  }
}

void s21::write_highscore(int score) {
  FILE *fptr = NULL;
  fptr = fopen(SCORE_PATH, "w");
  if (fptr) {
    fprintf(fptr, "%d", score);
    fclose(fptr);
  } else {
    throw std::runtime_error(FERROR);
  }
}

void s21::erase_line(GameInfo_t *game, int index) {
  for (int j = 0; j < COLUMNS; j++) {
    game->field[index][j] = EMPTY;
    for (int i = index; i > LENGTH_MAX; i--) {
      game->field[i][j] = game->field[i - 1][j];
    }
  }
}

int s21::check_boundaryY(GameInfo_t *game) {
  int result = SUCCESS;
  for (int i = 0; i < ROWS + LENGTH_MAX; i++) {
    for (int j = 0; j < COLUMNS; j++) {
      if (game->field[i][j] == MOVING &&
          (i + 1 >= ROWS + LENGTH_MAX || game->field[i + 1][j] == STATIC)) {
        result = FAILURE;
      }
    }
  }
  return result;
}

int s21::check_boundaryX(GameInfo_t *game, int shift) {
  int result = SUCCESS;
  for (int i = 0; i < ROWS + LENGTH_MAX; i++) {
    for (int j = 0; j < COLUMNS; j++) {
      if (game->field[i][j] == MOVING &&
          (j + shift >= COLUMNS || j + shift < 0 ||
           game->field[i][j + shift] == STATIC)) {
        result = FAILURE;
      }
    }
  }
  return result;
}

int s21::check_rotation(GameInfo_t *game, int **rotated, int size) {
  int result = SUCCESS;
  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++) {
      if (rotated[i][j] == MOVING &&
          (game->field[i + game->y][j + game->x] == STATIC || game->x < 0 ||
           game->x > COLUMNS - game->length)) {
        result = FAILURE;
      }
    }
  }
  return result;
}
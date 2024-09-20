#include <ncurses.h>
#include <unistd.h>

#include "../../../brick_game/tetris/tetrisController.h"

void gameloop(GameInfo_t *game, State_t *state);
void print_field(GameInfo_t *game, State_t state);
void print_stats(GameInfo_t *game, int i, State_t state);
void gameover(GameInfo_t *game);

GameInfo_t updateCurrentState(GameInfo_t game, char input, State_t *state);
UserAction_t get_signal(char input);

int main() {
  GameInfo_t game;
  State_t state = START;
  cbreak();
  initscr();
  noecho();
  nodelay(stdscr, TRUE);
  srand(time(NULL));
  gameloop(&game, &state);
  gameover(&game);
  return 0;
}

void gameloop(GameInfo_t *game, State_t *state) {
  while (1) {
    wtimeout(stdscr, (TIMEOUT - game->speed * 15));
    print_field(game, *state);
    if (*state == EXIT) break;
    *game = updateCurrentState(*game, getch(), state);
  }
}

GameInfo_t updateCurrentState(GameInfo_t game, char input, State_t *state) {
  GameInfo_t temp = game;
  s21::userInput(get_signal(input), &temp, state);
  return temp;
}

UserAction_t get_signal(char input) {
  UserAction_t output = Nosig;

  if (input == '\033') {
    getch();
    char c = getch();
    if (c == 'C') {
      output = Right;
    } else if (c == 'D') {
      output = Left;
    } else if (c == 'B') {
      output = Down;
    }
  } else if (input == ' ') {
    output = Pause;
  } else if (input == 'q' || input == 'Q') {
    output = Terminate;
  } else if (input == '\n') {
    output = Action;
  } else if (input == 'S' || input == 's') {
    output = Start;
  }
  return output;
}

void print_field(GameInfo_t *game, State_t state) {
  clear();
  refresh();
  printf("ARROWS: < > move, ⌄ speed up\n\r");
  printf(" _ _ _ _ _ _ _ _ _ _\n\r");
  for (int i = LENGTH_MAX; i < ROWS + LENGTH_MAX; i++) {
    printf("|");
    for (int j = 0; j < COLUMNS; j++) {
      if (game->field[i][j] == MOVING) {
        printf("%s", game->color);
      }
      if (game->field[i][j]) {
        printf("▪");
        printf(DEFAULT);
      } else {
        printf(" ");
      }
      if (j != COLUMNS - 1) printf(" ");
    }
    printf("|");
    print_stats(game, i, state);
    printf("\n\r");
  }
  printf(" ‾ ‾ ‾ ‾ ‾ ‾ ‾ ‾ ‾ ‾\n\r");
  printf("\n\rSPACE start/pause, ENTER rotate\n\n\rQ quit");
  fflush(stdout);
}

void print_stats(GameInfo_t *game, int i, State_t state) {
  if (i == 4) {
    printf("  BEST SCORE: %d", game->high_score);
  } else if (i == 5) {
    printf("  SCORE: %d", game->score);
  } else if (i == 6) {
    printf("  LEVEL: %d", game->level);
  } else if (i == 8) {
    printf("  NEXT:");
  } else if (i > 9 && i < 12) {
    printf("  ");
    for (int j = 0; j < 4; j++) {
      if (game->next[i - 8][j]) {
        printf("▪");
      } else {
        printf(" ");
      }
      if (j < 3) printf(" ");
    }
  } else if (i == 15 && state == START) {
    printf("  PRESS S TO START");
  } else if (i == 15 && state == GAMEOVER) {
    printf("  GAME OVER! PRESS S TO REPLAY");
  }
}

void gameover(GameInfo_t *game) {
  refresh();
  endwin();
  printf("\nGood game!\nYour Score: %d\nHighest Score: %d\n\n", game->score,
         game->high_score);
}
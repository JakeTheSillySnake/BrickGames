#include "snakeViewer.h"

using namespace s21;

void View::event_loop() {
  cbreak();
  initscr();
  noecho();
  nodelay(stdscr, TRUE);
  while (true) {
    print_field();
    wtimeout(stdscr, (TIMEOUT - get_data().speed * 15));
    get_signal(getch());
    if (controller->get_state() == EXIT) break;
  }
  game_over();
}

void View::get_signal(char input) {
  if (input == '\033') {
    getch();
    char c = getch();
    if (c == 'C') {
      controller->move_right();
    } else if (c == 'D') {
      controller->move_left();
    } else if (c == 'B') {
      controller->move_down();
    } else {
      controller->move_up();
    }
  } else if (input == ' ') {
    controller->pause();
  } else if (input == 'q' || input == 'Q') {
    controller->terminate();
  } else if (input == 's' || input == 'S') {
    controller->start();
  } else if (input == '\n') {
    controller->shift();
    controller->shift();
  } else if (input == -1) {
    controller->shift();
  }
}

void View::print_field() {
  clear();
  refresh();
  printf("ARROWS to move\n\r");
  printf(" _ _ _ _ _ _ _ _ _ _\n\r");
  for (int i = 0; i < ROWS; i++) {
    printf("|");
    for (int j = 0; j < COLUMNS; j++) {
      printf(GREEN);
      if (get_data().coords[0].x == j && get_data().coords[0].y == i) {
        print_head();
      } else if (get_data().field[i][j] == SNAKE) {
        printf("▪");
      } else if (get_data().field[i][j] == APPLE) {
        printf(RED);
        printf("o");
      } else {
        printf(" ");
      }
      if (j < COLUMNS - 1) printf(" ");
    }
    printf(DEFAULT);
    printf("|");
    print_stats(i, get_data());
    printf("\n\r");
  }
  printf(" ‾ ‾ ‾ ‾ ‾ ‾ ‾ ‾ ‾ ‾\n\r");
  printf("\n\rSPACE to start/pause, Q to quit\n\r");
  fflush(stdout);
}

void View::print_head() {
  if (get_data().coords[0].dir == UP)
    printf("▴");
  else if (get_data().coords[0].dir == LEFT)
    printf("◂");
  else if (get_data().coords[0].dir == RIGHT)
    printf("▸");
  else
    printf("▾");
}

void View::print_stats(int i, GameInfo_t game) {
  if (i == 4) {
    printf("  BEST SCORE: %d", game.high_score);
  } else if (i == 5) {
    printf("  SCORE: %d", game.score);
  } else if (i == 6) {
    printf("  LEVEL: %d", game.level);
  } else if (i == 15 && controller->get_state() == START) {
    printf("  PRESS S TO START");
  } else if (i == 15 && controller->get_state() == GAMEOVER &&
             get_data().coords.size() == 200) {
    printf("  YOU WON! PRESS S TO REPLAY");
  } else if (i == 15 && controller->get_state() == GAMEOVER) {
    printf("  GAME OVER! PRESS S TO REPLAY");
  }
}

GameInfo_t View::get_data() { return controller->get_data(); }

void View::game_over() {
  refresh();
  endwin();
  printf("\nGood game!\nYour Score: %d\nHighest Score: %d\n\n",
         get_data().score, get_data().high_score);
}
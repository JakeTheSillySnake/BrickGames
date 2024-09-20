#include "snakeViewer.h"

using namespace s21;

int main() {
  SnakeModel model;
  Controller controller(&model);
  View view(&controller);
  view.event_loop();
  return 0;
}
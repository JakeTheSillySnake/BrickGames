#include <QApplication>

#include "snakeViewer.h"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  SnakeModel model;
  Controller controller(&model);
  MainWindow w(&controller);
  w.setWindowIcon(QIcon(QFileInfo("assets/snake.png").absoluteFilePath()));
  w.show();
  return a.exec();
}

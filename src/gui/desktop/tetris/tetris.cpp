#include <QApplication>

#include "tetrisViewer.h"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  MainWindow w;
  w.setWindowIcon(QIcon(QFileInfo("assets/tetris.png").absoluteFilePath()));
  w.show();
  return a.exec();
}

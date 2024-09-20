#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QFileInfo>
#include <QFont>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsSimpleTextItem>
#include <QGraphicsTextItem>
#include <QGraphicsView>
#include <QKeyEvent>
#include <QMainWindow>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QPainter>
#include <QPainterPath>
#include <QPen>
#include <QTimer>

#include "../../../brick_game/tetris/tetrisController.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  const qreal sceneWidth = 200;
  const qreal sceneHeight = 400;
  const qreal squareSize = 19;

  qreal sceneXCenter = sceneWidth / 2;
  qreal sceneYCenter = sceneHeight / 2;
  qreal square0X = sceneXCenter - (squareSize / 2);
  qreal square0Y = sceneYCenter - (squareSize / 2);

  QColor colors[6] = {Qt::green,  Qt::red,
                      Qt::blue,   QColor::fromRgb(255, 165, 0),
                      Qt::yellow, Qt::magenta};

  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

  void draw_field();
  void draw_stats();
  void draw_head();
  void play_sound();
  void set_timer();
  void game_over();
  void keyPressEvent(QKeyEvent *event);

 private slots:
  void slotTimerAlarm();

 private:
  State_t state;
  GameInfo_t game;
  Ui::MainWindow *ui;
  QMediaPlayer *player;
  QMediaPlaylist *playlist;
  QGraphicsScene *scene;
  QTimer *timer;
};
#endif  // MAINWINDOW_H

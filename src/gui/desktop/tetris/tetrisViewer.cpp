#include "tetrisViewer.h"

#include "ui_tetrisViewer.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  player = new QMediaPlayer(this);
  playlist = new QMediaPlaylist(player);
  scene = new QGraphicsScene(this);
  timer = new QTimer();
  state = START;

  ui->graphicsView->setScene(scene);
  ui->graphicsView->setRenderHint(QPainter::Antialiasing, true);
  QRect content = ui->graphicsView->contentsRect();
  ui->graphicsView->setSceneRect(1, 1, content.width(), content.height());
  play_sound();
  set_timer();
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::set_timer() {
  timer->disconnect();
  connect(timer, SIGNAL(timeout()), this, SLOT(slotTimerAlarm()));
  timer->start(TIMEOUT - game.speed * 15);
}

void MainWindow::slotTimerAlarm() {
  s21::userInput(Nosig, &game, &state);
  draw_field();
  set_timer();
  if (state == GAMEOVER) game_over();
}

void MainWindow::play_sound() {
  player->setPlaylist(playlist);
  playlist->addMedia(QUrl::fromLocalFile(
      QFileInfo("assets/tetris_track.mp3").absoluteFilePath()));
  playlist->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);
  player->setVolume(30);
  player->play();
}

void MainWindow::draw_field() {
  scene->clear();
  for (int i = LENGTH_MAX; i < ROWS + LENGTH_MAX; i++) {
    for (int j = 0; j < COLUMNS; j++) {
      if (game.field[i][j]) {
        QPainterPath path;
        path.addRoundedRect(QRectF(j * (squareSize + 1),
                                   11 + (i - LENGTH_MAX) * (squareSize + 1),
                                   squareSize, squareSize),
                            5, 5);
        auto rect = scene->addPath(path);
        if (game.field[i][j] == MOVING)
          rect->setBrush(colors[game.color_num]);
        else
          rect->setBrush(Qt::cyan);
      }
    }
  }
  draw_stats();
  ui->graphicsView->update();
}

void MainWindow::draw_stats() {
  QFont font;
  font.setBold(true);
  font.setFamily("Ubuntu Mono");
  font.setPixelSize(19);
  auto text = scene->addText(QString(QString::number(game.high_score)));
  text->setPos(335, 5);
  text->setFont(font);
  text = scene->addText(QString(QString::number(game.score)));
  text->setPos(280, 28);
  text->setFont(font);
  text = scene->addText(QString(QString::number(game.level)));
  text->setPos(280, 71);
  text->setFont(font);
  for (int i = 0; i < LENGTH_MAX; i++) {
    for (int j = 0; j < LENGTH_MAX; j++) {
      if (game.next[i][j]) {
        QPainterPath path;
        path.addRoundedRect(QRectF(216 + j * (squareSize + 1),
                                   (i + 10 - LENGTH_MAX) * (squareSize + 1),
                                   squareSize, squareSize),
                            5, 5);
        auto rect = scene->addPath(path);
        rect->setBrush(Qt::cyan);
      }
    }
  }
}

void MainWindow::game_over() {
  QFont font;
  QPen pen(Qt::white);
  pen.setWidth(1);
  pen.setCapStyle(Qt::RoundCap);
  pen.setJoinStyle(Qt::RoundJoin);
  font.setBold(true);
  font.setFamily("Ubuntu Mono");
  font.setPixelSize(30);

  auto text = scene->addSimpleText(QString("GAME OVER!"));
  text->setPos(30, 180);
  text->setFont(font);
  text->setBrush(Qt::white);
  text->setPen(pen);
  ui->graphicsView->update();
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
  switch (event->key()) {
    case Qt::Key_A:
      s21::userInput(Left, &game, &state);
      break;
    case Qt::Key_D:
      s21::userInput(Right, &game, &state);
      break;
    case Qt::Key_S:
      s21::userInput(Down, &game, &state);
      break;
    case Qt::Key_Space:
      s21::userInput(Pause, &game, &state);
      break;
    case Qt::Key_X:
      s21::userInput(Start, &game, &state);
      break;
    case Qt::Key_W:
      s21::userInput(Action, &game, &state);
      break;
  }
}

#include "snakeViewer.h"

#include "ui_snakeViewer.h"

MainWindow::MainWindow(Controller *c, QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), controller(c) {
  ui->setupUi(this);
  player = new QMediaPlayer(this);
  playlist = new QMediaPlaylist(player);
  scene = new QGraphicsScene(this);
  timer = new QTimer();

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
  timer->start(TIMEOUT - get_data().speed * 15);
}

void MainWindow::slotTimerAlarm() {
  controller->shift();
  draw_field();
  set_timer();
  if (controller->get_state() == GAMEOVER) game_over();
}

void MainWindow::play_sound() {
  player->setPlaylist(playlist);
  playlist->addMedia(QUrl::fromLocalFile(
      QFileInfo("assets/snake_track.mp3").absoluteFilePath()));
  playlist->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);
  player->setVolume(30);
  player->play();
}

void MainWindow::draw_field() {
  scene->clear();
  for (int i = 0; i < ROWS; i++) {
    for (int j = 0; j < COLUMNS; j++) {
      if (get_data().field[i][j] == SNAKE) {
        QPainterPath path;
        path.addRoundedRect(QRectF(j * (squareSize + 1), i * (squareSize + 1),
                                   squareSize, squareSize),
                            5, 5);
        auto rect = scene->addPath(path);
        rect->setBrush(Qt::green);
        if (i == get_data().coords[0].y && j == get_data().coords[0].x) {
          draw_head();
        }
      } else if (get_data().field[i][j] == APPLE) {
        auto ellipse =
            scene->addEllipse(j * (squareSize + 1), i * (squareSize + 1),
                              squareSize * 0.8, squareSize * 0.8);
        ellipse->setBrush(Qt::red);
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
  auto text = scene->addText(QString(QString::number(get_data().high_score)));
  text->setPos(335, 3);
  text->setFont(font);
  text = scene->addText(QString(QString::number(get_data().score)));
  text->setPos(280, 26);
  text->setFont(font);
  text = scene->addText(QString(QString::number(get_data().level)));
  text->setPos(280, 69);
  text->setFont(font);
}

void MainWindow::draw_head() {
  int i = get_data().coords[0].y;
  int j = get_data().coords[0].x;
  QPainterPath path;
  path.addEllipse(j * (squareSize + 1) - 1,
                  (i + 1) * (squareSize + 1) - squareSize / 2 - 4, eyeSize,
                  eyeSize);
  path.addEllipse((j + 1) * (squareSize + 1) - eyeSize,
                  (i + 1) * (squareSize + 1) - squareSize / 2 - 4, eyeSize,
                  eyeSize);
  auto eyes = scene->addPath(path);
  eyes->setBrush(Qt::yellow);
  if (get_data().coords[0].dir == LEFT || get_data().coords[0].dir == RIGHT) {
    QPoint center =
        QRect(j * (squareSize + 1) - 1,
              (i + 1) * (squareSize + 1) - squareSize / 2 - 4, squareSize, 5)
            .center();
    eyes->setTransformOriginPoint(center.x(), center.y());
    eyes->setRotation(90);
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

  QGraphicsSimpleTextItem *text;
  if (get_data().coords.size() == 200) {
    text = scene->addSimpleText(QString("YOU WON!"));
    text->setPos(45, 180);
  } else {
    text = scene->addSimpleText(QString("GAME OVER!"));
    text->setPos(30, 180);
  }
  text->setFont(font);
  text->setBrush(Qt::white);
  text->setPen(pen);
  ui->graphicsView->update();
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
  switch (event->key()) {
    case Qt::Key_W:
      controller->move_up();
      break;
    case Qt::Key_A:
      controller->move_left();
      break;
    case Qt::Key_D:
      controller->move_right();
      break;
    case Qt::Key_S:
      controller->move_down();
      break;
    case Qt::Key_Space:
      controller->pause();
      break;
    case Qt::Key_X:
      controller->start();
      break;
    case Qt::Key_Q:
      controller->shift();
      controller->shift();
      break;
  }
}

GameInfo_t MainWindow::get_data() { return controller->get_data(); }

#include "gui/MainWindow.h"
#include <QApplication>

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);

  MainWindow w;
  w.setWindowTitle("Task Manager");
  w.resize(600, 400);
  w.show();

  return app.exec();
}
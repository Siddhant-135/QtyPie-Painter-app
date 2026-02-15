#include <QApplication>
#include "mainwidget.h"

int main(int argc, char* argv[]) {
  QApplication app(argc, argv);
  MainWidget window;
  window.show();
  return app.exec();
}

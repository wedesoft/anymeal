#include <QtCore/QCoreApplication>
#include <QtWidgets/QApplication>
#include "main_window.hh"


int main(int argc, char *argv[]) {
  QCoreApplication::addLibraryPath(".");
  QApplication app(argc, argv);
  setlocale(LC_NUMERIC, "C"); // Change back behaviour of atof.
  MainWindow main_window;
  main_window.show();
  return app.exec();
}

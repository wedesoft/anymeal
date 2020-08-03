#include <QtCore/QCoreApplication>
#include <QtCore/QTranslator>
#include <QtWidgets/QApplication>
#include <QtWidgets/QSplashScreen>
#include "main_window.hh"


int main(int argc, char *argv[]) {
  QCoreApplication::addLibraryPath(".");
  QApplication app(argc, argv);
  QString app_dir = QCoreApplication::applicationDirPath();
  QPixmap pixmap(":/images/splash.png");
  QSplashScreen splash(pixmap);
  splash.show();
  app.processEvents();
  setlocale(LC_NUMERIC, "C"); // Change back behaviour of atof.
  QString country = QLocale::system().name().mid(0, 2);
  QTranslator translator;
#ifdef __MINGW32__
  QString locale_dir = QString("locale/%1").arg(country);
#else
  QString locale_dir = QString("%1/../share/locale/%2/LC_MESSAGES").arg(app_dir).arg(country);
#endif
  translator.load("anymeal_qt", locale_dir);
  app.installTranslator(&translator);
  MainWindow main_window;
  splash.finish(&main_window);
  main_window.show();
  return app.exec();
}

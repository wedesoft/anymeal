/* AnyMeal recipe management software
   Copyright (C) 2020, 2024 Jan Wedekind

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>. */
#include <QtWidgets/QApplication>
#include <QtWidgets/QSplashScreen>
#include "main_window.hh"


int main(int argc, char *argv[]) {
  QCoreApplication::setApplicationName("anymeal");
  QCoreApplication::addLibraryPath(".");
  QApplication app(argc, argv);
  QPixmap pixmap(":/images/splash.png");
  QSplashScreen splash(pixmap);
  splash.show();
  app.processEvents();
  setlocale(LC_NUMERIC, "C"); // Change back behaviour of atof.
  MainWindow main_window;
  splash.finish(&main_window);
  main_window.show();
  return app.exec();
}

#include <unistd.h>
#include <QtCore/QStandardPaths>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QMessageBox>
#include "main_window.hh"


MainWindow::MainWindow(QWidget *parent): QMainWindow(parent) {
  m_ui.setupUi(this);
  connect(m_ui.action_import, &QAction::triggered, this, &MainWindow::import);
  try {
    auto path = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir dir(path);
    dir.mkpath(dir.absolutePath());
    m_database.open(dir.filePath("anymeal.db").toUtf8().constData());
  } catch (database_exception &e) {
    QMessageBox::critical(this, "Error opening database", e.what());
    exit(1);
  };
}

void MainWindow::import(void) {
  QStringList result =
    QFileDialog::getOpenFileNames(this, "Import MealMaster files", "", "MealMaster (*.mm *.MM *.mmf *.MMF);;Text (*.txt *.TXT);;"
                                  "All files (*)");
  if (!result.isEmpty()) {
    for (int i=0; i<result.size(); i++) {
    };
  };
}

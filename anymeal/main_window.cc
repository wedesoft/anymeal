#include <fstream>
#include <sstream>
#include <unistd.h>
#include <QtCore/QStandardPaths>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QProgressDialog>
#include "main_window.hh"
#include "partition.hh"
#include "recode.hh"
#include "mealmaster.hh"


using namespace std;

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), m_titles_model(nullptr) {
  m_ui.setupUi(this);
  connect(m_ui.action_import, &QAction::triggered, this, &MainWindow::import);
  connect(m_ui.title_edit, &QLineEdit::returnPressed, this, &MainWindow::filter);
  connect(m_ui.filter_button, &QPushButton::clicked, this, &MainWindow::filter);
  connect(m_ui.reset_button, &QPushButton::clicked, this, &MainWindow::reset);
  try {
    auto path = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir dir(path);
    dir.mkpath(dir.absolutePath());
    m_database.open(dir.filePath("anymeal.sqlite").toUtf8().constData());
  } catch (database_exception &e) {
    QMessageBox::critical(this, "Error opening database", e.what());
    exit(1);
  };
  m_titles_model = new TitlesModel(this, &m_database);
  m_ui.titles_view->setModel(m_titles_model);
}

void MainWindow::import(void) {
  QStringList result =
    QFileDialog::getOpenFileNames(this, "Import MealMaster files", "", "MealMaster (*.mm *.MM *.mmf *.MMF);;Text (*.txt *.TXT);;"
                                  "All files (*)");
  bool transaction = false;
  if (!result.isEmpty()) {
    try {
      QProgressDialog progress("Importing files ...", "Cancel", 0, result.size() * 100, this);
      progress.setWindowModality(Qt::WindowModal);
      Recoder recoder("latin1..utf8");  // TODO: select input encoding.
      for (int i=0; i<result.size(); i++) {
        m_database.begin();
        transaction = true;
        progress.setLabelText(QString("Processing file %1 ...").arg(result.at(i)));
        ifstream f(result.at(i).toUtf8().constData());
        auto lst = recipes(f);
        int c = 0;
        for (auto recipe=lst.begin(); recipe!=lst.end(); recipe++) {
          progress.setValue(i * 100 + c++ * 100 / lst.size());
          istringstream s(*recipe);
          try {
            auto result = parse_mealmaster(s);
            auto recoded = recoder.process_recipe(result);
            m_database.insert_recipe(recoded);
          } catch (parse_exception &e) {
            // TOOD: output to error file.
          };
          if (progress.wasCanceled())
            break;
        };
        if (progress.wasCanceled()) {
          if (transaction) {
            m_database.rollback();
            transaction = false;
          };
          break;
        };
        m_database.commit();
        transaction = false;
      };
      progress.setValue(result.size() * 100);
    } catch (exception &e) {
      QMessageBox::critical(this, "Error while importing", e.what());
      try {
        if (transaction)
          m_database.rollback();
      } catch (exception &e) {
      };
    };
  };
}

void MainWindow::filter(void) {
  if (!m_ui.title_edit->text().isEmpty()) {
    QGuiApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    m_database.select_by_title(m_ui.title_edit->text().toUtf8().constData());
    m_titles_model->reset();
    m_ui.title_edit->setText("");
    QGuiApplication::restoreOverrideCursor();
  };
}

void MainWindow::reset(void) {
  QGuiApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
  m_database.select_all();
  m_titles_model->reset();
  QGuiApplication::restoreOverrideCursor();
}

#include <fstream>
#include <sstream>
#include <unistd.h>
#include <QtCore/QStandardPaths>
#include <QtCore/QStringListModel>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QProgressDialog>
#include <QtPrintSupport/QPrintPreviewDialog>
#include <QtPrintSupport/QPrintDialog>
#include "main_window.hh"
#include "import_dialog.hh"
#include "partition.hh"
#include "recode.hh"
#include "mealmaster.hh"
#include "html.hh"
#include "config.h"


using namespace std;

MainWindow::MainWindow(QWidget *parent):
  QMainWindow(parent), m_titles_model(nullptr), m_categories_model(nullptr), m_categories_completer(nullptr)
{
  m_ui.setupUi(this);
  connect(m_ui.action_import, &QAction::triggered, this, &MainWindow::import);
  connect(m_ui.action_delete, &QAction::triggered, this, &MainWindow::delete_recipes);
  connect(m_ui.action_preview, &QAction::triggered, this, &MainWindow::preview);
  connect(m_ui.action_print, &QAction::triggered, this, &MainWindow::print);
  connect(m_ui.action_about, &QAction::triggered, this, &MainWindow::about);
  connect(m_ui.title_edit, &QLineEdit::returnPressed, this, &MainWindow::filter);
  connect(m_ui.category_edit, &QLineEdit::returnPressed, this, &MainWindow::filter);
  connect(m_ui.ingredient_edit, &QLineEdit::returnPressed, this, &MainWindow::filter);
  connect(m_ui.filter_button, &QPushButton::clicked, this, &MainWindow::filter);
  connect(m_ui.reset_button, &QPushButton::clicked, this, &MainWindow::reset);
  connect(m_ui.titles_view, &QListView::activated, this, &MainWindow::selected);
  connect(m_ui.titles_view, &QListView::customContextMenuRequested, this, &MainWindow::titles_context_menu);
  connect(m_ui.recipe_browser, &QTextBrowser::customContextMenuRequested, this, &MainWindow::recipe_context_menu);
  m_titles_context_menu = new QMenu(this);
  m_titles_context_menu->addAction(m_ui.action_delete);
  m_recipe_context_menu = new QMenu(this);
  m_recipe_context_menu->addAction(m_ui.action_preview);
  m_recipe_context_menu->addAction(m_ui.action_print);
  try {
    auto path = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir dir(path);
    dir.mkpath(dir.absolutePath());
    m_database.open(dir.filePath("anymeal.sqlite").toUtf8().constData());
    m_titles_model = new TitlesModel(this, &m_database);
    m_ui.titles_view->setModel(m_titles_model);
    m_categories_model = new CategoriesModel(this, &m_database);
    m_categories_completer = new QCompleter(m_categories_model, this);
    m_categories_completer->setCaseSensitivity(Qt::CaseInsensitive);
    m_ui.category_edit->setCompleter(m_categories_completer);
  } catch (exception &e) {
    QMessageBox::critical(this, tr("Error Opening Database"), e.what());
    exit(1);
  };
}

void MainWindow::import(void) {
  bool transaction = false;
  try {
    ImportDialog import_dialog(this);
    int result = import_dialog.exec();
    if (result == QDialog::Accepted) {
      Recoder recoder((import_dialog.encoding() + "..UTF-8").c_str());
      QStringList result =
        QFileDialog::getOpenFileNames(this, tr("Import MealMaster Files"), "", tr("MealMaster (*.mm *.MM *.mmf *.MMF);;Text (*.txt *.TXT);;"
                                      "All files (*)"));
      if (!result.isEmpty()) {
        ofstream error_file(import_dialog.error_file().c_str(), ofstream::binary);
        QProgressDialog progress(tr("Importing files ..."), tr("Cancel"), 0, result.size() * 100, this);
        progress.setWindowModality(Qt::WindowModal);
        for (int i=0; i<result.size(); i++) {
          m_database.begin();
          transaction = true;
          progress.setLabelText(QString(tr("Processing file %1 ...")).arg(result.at(i)));
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
              error_file << tr("Rejected recipe: ").toUtf8().constData() << e.what() << "\r\n";
              error_file << *recipe;
              error_file.flush();
              if (!error_file) {
                ostringstream s;
                s << tr("Error writing to file ").toUtf8().constData() << import_dialog.error_file();
                throw gui_exception(s.str());
              };
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
        m_database.select_all();
        m_titles_model->reset();
        m_categories_model->reset();
      };
    };
  } catch (exception &e) {
    QMessageBox::critical(this, tr("Error While Importing"), e.what());
    try {
      if (transaction)
        m_database.rollback();
    } catch (exception &e) {
    };
  };
}

void MainWindow::about(void) {
  QMessageBox::information(this, PACKAGE_STRING, "AnyMeal recipe database software\n"
      "Copyright © 2020  Jan Wedekind\n\n"
      "This program is free software: you can redistribute it and/or modify "
      "it under the terms of the GNU General Public License as published by "
      "the Free Software Foundation, either version 3 of the License, or "
      "(at your option) any later version.\n\n"
      "This program is distributed in the hope that it will be useful, "
      "but WITHOUT ANY WARRANTY; without even the implied warranty of "
      "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the "
      "GNU General Public License for more details.\n\n"
      "You should have received a copy of the GNU General Public License "
      "along with this program.  If not, see <https://www.gnu.org/licenses/>.");
}

void MainWindow::filter(void) {
  try {
    QGuiApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    if (!m_ui.title_edit->text().isEmpty()) {
      m_database.select_by_title(m_ui.title_edit->text().toUtf8().constData());
      m_titles_model->reset();
      m_categories_model->reset();
      m_ui.title_edit->setText("");
    };
    if (!m_ui.category_edit->text().isEmpty()) {
      m_database.select_by_category(m_ui.category_edit->text().toUtf8().constData());
      m_titles_model->reset();
      m_categories_model->reset();
      m_ui.category_edit->setText("");
    };
    if (!m_ui.ingredient_edit->text().isEmpty()) {
      m_database.select_by_ingredient(m_ui.ingredient_edit->text().toUtf8().constData());
      m_titles_model->reset();
      m_categories_model->reset();
      m_ui.ingredient_edit->setText("");
    };
    QGuiApplication::restoreOverrideCursor();
  } catch (exception &e) {
    QGuiApplication::restoreOverrideCursor();
    QMessageBox::critical(this, tr("Error Filtering Recipes"), e.what());
  };
}

void MainWindow::reset(void) {
  try {
    QGuiApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    m_database.select_all();
    m_titles_model->reset();
    m_categories_model->reset();
    QGuiApplication::restoreOverrideCursor();
  } catch (exception &e) {
    QGuiApplication::restoreOverrideCursor();
    QMessageBox::critical(this, tr("Error Resetting Selection"), e.what());
  };
}

string MainWindow::translate(const char *context, const char *text) {
  return QCoreApplication::translate(context, text).toUtf8().constData();
}

void MainWindow::selected(const QModelIndex &index) {
  try {
    QGuiApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    Recipe recipe = m_database.fetch_recipe(m_titles_model->recipeid(index));
    m_ui.recipe_browser->setHtml(recipe_to_html(recipe, &translate).c_str());
    QGuiApplication::restoreOverrideCursor();
  } catch (exception &e) {
    QGuiApplication::restoreOverrideCursor();
    QMessageBox::critical(this, tr("Error fetching recipe"), e.what());
  };
}

void MainWindow::titles_context_menu(const QPoint &pos) {
  m_titles_context_menu->popup(m_ui.titles_view->viewport()->mapToGlobal(pos));
}

void MainWindow::recipe_context_menu(const QPoint &pos) {
  m_recipe_context_menu->popup(m_ui.recipe_browser->viewport()->mapToGlobal(pos));
}

vector<sqlite3_int64> MainWindow::recipe_ids(void) {
  vector<sqlite3_int64> result;
  auto model = m_ui.titles_view->selectionModel();
  auto lst = model->selectedRows();
  for (auto index=lst.begin(); index!=lst.end(); index++) {
    result.push_back(m_titles_model->recipeid(*index));
  };
  return result;
}

void MainWindow::delete_recipes(void) {
  auto ids = recipe_ids();
  if (!ids.empty()) {
    if (QMessageBox::question(this, tr("Delete Recipes"), tr("Do you want to delete the selected recipes?")) == QMessageBox::Yes) {
      try {
        QGuiApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
        m_database.delete_recipes(ids);
        m_titles_model->reset();
        m_categories_model->reset();
        QGuiApplication::restoreOverrideCursor();
      } catch (exception &e) {
        try {
          m_database.rollback();
        } catch (exception &) {
        };
        QGuiApplication::restoreOverrideCursor();
        QMessageBox::critical(this, tr("Error Deleting Recipes"), e.what());
      };
    };
  };
}

void MainWindow::preview(void) {
  QPrintPreviewDialog dialog;
  connect(&dialog, &QPrintPreviewDialog::paintRequested, this, &MainWindow::render);
  dialog.exec();
}

void MainWindow::print(void) {
  QPrintDialog dialog;
  if (dialog.exec() == QPrintDialog::Accepted) {
    render(dialog.printer());
  };
}

void MainWindow::render(QPrinter *printer) {
  m_ui.recipe_browser->print(printer);
}

// TODO: export MealMaster
// TODO: edit recipe

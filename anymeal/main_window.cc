/* AnyMeal recipe database software
   Copyright (C) 2020 Jan Wedekind

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
#include "export_dialog.hh"
#include "edit_dialog.hh"
#include "partition.hh"
#include "recode.hh"
#include "mealmaster.hh"
#include "html.hh"
#include "export.hh"
#include "config.h"


using namespace std;

MainWindow::MainWindow(QWidget *parent):
  QMainWindow(parent), m_titles_model(nullptr), m_categories_model(nullptr), m_categories_completer(nullptr)
{
  m_ui.setupUi(this);
  connect(m_ui.action_import, &QAction::triggered, this, &MainWindow::import);
  connect(m_ui.action_delete, &QAction::triggered, this, &MainWindow::delete_recipes);
  connect(m_ui.action_export, &QAction::triggered, this, &MainWindow::export_recipes);
  connect(m_ui.action_preview, &QAction::triggered, this, &MainWindow::preview);
  connect(m_ui.action_print, &QAction::triggered, this, &MainWindow::print);
  connect(m_ui.action_edit, &QAction::triggered, this, &MainWindow::edit);
  connect(m_ui.action_about, &QAction::triggered, this, &MainWindow::about);
  connect(m_ui.title_edit, &QLineEdit::returnPressed, this, &MainWindow::filter);
  connect(m_ui.category_edit, &QLineEdit::returnPressed, this, &MainWindow::filter);
  connect(m_ui.ingredient_edit, &QLineEdit::returnPressed, this, &MainWindow::filter);
  connect(m_ui.filter_button, &QPushButton::clicked, this, &MainWindow::filter);
  connect(m_ui.reset_button, &QPushButton::clicked, this, &MainWindow::reset);
  connect(m_ui.titles_view, &QListView::customContextMenuRequested, this, &MainWindow::titles_context_menu);
  connect(m_ui.recipe_browser, &QTextBrowser::customContextMenuRequested, this, &MainWindow::recipe_context_menu);
  m_titles_context_menu = new QMenu(this);
  m_titles_context_menu->addAction(m_ui.action_export);
  m_titles_context_menu->addAction(m_ui.action_edit);
  m_titles_context_menu->addAction(m_ui.action_preview);
  m_titles_context_menu->addAction(m_ui.action_print);
  m_titles_context_menu->addAction(m_ui.action_delete);
  m_recipe_context_menu = new QMenu(this);
  m_recipe_context_menu->addAction(m_ui.action_edit);
  m_recipe_context_menu->addAction(m_ui.action_preview);
  m_recipe_context_menu->addAction(m_ui.action_print);
  try {
    auto path = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir dir(path);
    dir.mkpath(dir.absolutePath());
    m_database.open(dir.filePath("anymeal.sqlite").toUtf8().constData());
    m_titles_model = new TitlesModel(this, &m_database);
    m_ui.titles_view->setModel(m_titles_model);
    connect(m_ui.titles_view->selectionModel(), &QItemSelectionModel::currentChanged, this, &MainWindow::selected);
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
        QFileDialog::getOpenFileNames(this, tr("Import MealMaster Files"), "", tr("MealMaster (*.mm *.MM *.mmf *.MMF);;"
                                      "Text (*.txt *.TXT);;All files (*)"));
      if (!result.isEmpty()) {
        int success = 0;
        int failed = 0;
        ofstream error_file(import_dialog.error_file().c_str(), ofstream::binary);
        QProgressDialog progress(tr("Importing files ..."), tr("Cancel"), 0, result.size() * 100, this);
        progress.setWindowModality(Qt::WindowModal);
        for (int i=0; i<result.size(); i++) {
          m_database.begin();
          transaction = true;
          ifstream f(result.at(i).toUtf8().constData());
          auto lst = recipes(f);
          int c = 0;
          for (auto recipe=lst.begin(); recipe!=lst.end(); recipe++) {
            progress.setLabelText(tr("%1 imported and %2 failed ...").arg(success).arg(failed));
            progress.setValue(i * 100 + c++ * 100 / lst.size());
            istringstream s(*recipe);
            try {
              Recipe result = parse_mealmaster(s);
              Recipe recoded = recoder.process_recipe(result);
              m_database.insert_recipe(recoded);
              success++;
            } catch (parse_exception &e) {
              failed++;
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
        QMessageBox::information(this, tr("Recipes Imported"), tr("%1 imported and %2 failed.").arg(success).arg(failed));
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

void MainWindow::edit(void) {
  QModelIndex index = m_ui.titles_view->currentIndex();
  // TODO: allow editing of new recipe.
  if (index.row() >= 0) {
    sqlite3_int64 recipe_id = m_titles_model->recipeid(index);
    Recipe recipe = m_database.fetch_recipe(recipe_id);
    EditDialog edit_dialog(this);
    // TODO: allow editing of recipe copy.
    edit_dialog.set_recipe(recipe);
    if (edit_dialog.exec() == QDialog::Accepted) {
      // TODO: check recipe for empty ingredient sections.
      Recipe result = edit_dialog.get_recipe();
      bool transaction = false;
      try {
        QGuiApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
        m_database.begin();
        transaction = true;
        vector<sqlite3_int64> ids;
        ids.push_back(recipe_id);
        m_database.delete_recipes(ids);
        sqlite3_int64 recipe_new_id = m_database.insert_recipe(result);
        m_titles_model->edit_entry(index, recipe_new_id, result.title().c_str());
        m_ui.recipe_browser->setHtml(recipe_to_html(result, &translate).c_str());
        m_database.commit();
        QGuiApplication::restoreOverrideCursor();
      } catch (exception &e) {
        try {
          if (transaction)
            m_database.rollback();
        } catch (exception &e) {
        };
        QGuiApplication::restoreOverrideCursor();
        QMessageBox::critical(this, tr("Error While Updating Recipe"), e.what());
      };
    };
  };
}

void MainWindow::about(void) {
  QMessageBox::information(this, tr("About AnyMeal"), "AnyMeal recipe database software\n"
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
    // TODO: show number of recipes in statusbar.
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
    // TODO: show number of recipes in statusbar.
    QGuiApplication::restoreOverrideCursor();
  } catch (exception &e) {
    QGuiApplication::restoreOverrideCursor();
    QMessageBox::critical(this, tr("Error Resetting Selection"), e.what());
  };
}

string MainWindow::translate(const char *context, const char *text) {
  return QCoreApplication::translate(context, text).toUtf8().constData();
}

void MainWindow::selected(const QModelIndex &current, const QModelIndex &) {
  try {
    QGuiApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    Recipe recipe = m_database.fetch_recipe(m_titles_model->recipeid(current));
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

void MainWindow::export_recipes(void) {
  auto ids = recipe_ids();
  if (!ids.empty()) {
    try {
      ExportDialog export_dialog(this);
      int result = export_dialog.exec();
      Recoder recoder((string("UTF-8..") + export_dialog.encoding()).c_str());
      if (result == QDialog::Accepted) {
        QString result =
          QFileDialog::getSaveFileName(this, tr("Export MealMaster File"), "", tr("MealMaster (*.mm *.MM *.mmf *.MMF);;"
                                       "Text (*.txt *.TXT);;All files (*)"));
        if (!result.isEmpty()) {
          int success = 0;
          int failed = 0;
          ofstream output_file(result.toUtf8().constData());
          QProgressDialog progress(tr("Exporting recipes ..."), tr("Cancel"), 0, ids.size(), this);
          progress.setWindowModality(Qt::WindowModal);
          for (unsigned int i=0; i<ids.size(); i++) {
            progress.setValue(i);
            sqlite3_int64 id = ids[i];
            Recipe recipe = m_database.fetch_recipe(id);
            try {
              Recipe recoded = recoder.process_recipe(recipe);
              string txt = recipe_to_mealmaster(recoded);
              output_file << txt << "\r\n";
              success++;
              if (i + 1 < ids.size())
                output_file << "\r\n";
            } catch (recode_exception &e) {
              failed++;
            };
            if (progress.wasCanceled())
              break;
            if (!output_file) {
              ostringstream s;
              s << tr("Error writing to file ").toUtf8().constData() << result.toUtf8().constData();
              throw gui_exception(s.str());
            };
            progress.setLabelText(tr("%1 exported and %2 failed ...").arg(success).arg(failed));
          };
          progress.setValue(ids.size());
          QMessageBox::information(this, tr("Recipes Exported"), tr("%1 exported and %2 failed.").arg(success).arg(failed));
        };
      };
    } catch (exception &e) {
      QMessageBox::critical(this, tr("Error While Exporting"), e.what());
    };
  };
}

void MainWindow::delete_recipes(void) {
  auto ids = recipe_ids();
  if (!ids.empty()) {
    if (QMessageBox::question(this, tr("Delete Recipes"), tr("Do you want to delete the selected recipes?")) == QMessageBox::Yes) {
      bool transaction = false;
      try {
        QGuiApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
        m_database.begin();
        transaction = true;
        m_database.delete_recipes(ids);
        m_database.commit();
        transaction = false;
        m_titles_model->reset();
        m_categories_model->reset();
        QGuiApplication::restoreOverrideCursor();
      } catch (exception &e) {
        try {
          if (transaction)
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

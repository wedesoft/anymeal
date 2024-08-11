/* AnyMeal recipe management software
   Copyright (C) 2021, 2024 Jan Wedekind

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
#include <cassert>
#include <fstream>
#include <sstream>
#include <set>
#include <unistd.h>
#include <QtCore/QCoreApplication>
#include <QtCore/QStandardPaths>
#include <QtCore/QStringListModel>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QProgressDialog>
#include <QtPrintSupport/QPrintPreviewDialog>
#include <QtPrintSupport/QPrintDialog>
#include "main_window.hh"
#include "edit_dialog.hh"
#include "category_dialog.hh"
#include "partition.hh"
#include "recode.hh"
#include "mealmaster.hh"
#include "html.hh"
#include "export.hh"
#include "config.h"


using namespace std;

MainWindow::MainWindow(QWidget *parent):
  QMainWindow(parent), m_settings("wedesoft", "anymeal"), m_translator(NULL), m_converter_window(this), m_import_dialog(this),
  m_export_dialog(this), m_category_picker(this), m_titles_model(NULL), m_categories_model(NULL), m_category_table_model(NULL),
  m_categories_completer(NULL)
{
  m_ui.setupUi(this);
  switch_language(m_settings.value("language", QLocale::system().name().mid(0, 2)).toString());
  m_ui.search_label->hide();
  connect(m_ui.action_new, &QAction::triggered, this, &MainWindow::new_recipe);
  connect(m_ui.action_import, &QAction::triggered, this, &MainWindow::import);
  connect(m_ui.action_delete, &QAction::triggered, this, &MainWindow::delete_recipes);
  connect(m_ui.action_export, &QAction::triggered, this, &MainWindow::export_recipes);
  connect(m_ui.action_preview, &QAction::triggered, this, &MainWindow::preview);
  connect(m_ui.action_print, &QAction::triggered, this, &MainWindow::print);
  connect(m_ui.action_edit, &QAction::triggered, this, &MainWindow::edit);
  connect(m_ui.action_add_to_category, &QAction::triggered, this, &MainWindow::add_to_category);
  connect(m_ui.action_remove_from_category, &QAction::triggered, this, &MainWindow::remove_from_category);
  connect(m_ui.action_deduplicate, &QAction::triggered, this, &MainWindow::remove_duplicates);
  connect(m_ui.action_collect_garbage, &QAction::triggered, this, &MainWindow::collect_garbage);
  connect(m_ui.action_lang_en, &QAction::triggered, this, &MainWindow::language_en);
  connect(m_ui.action_lang_de, &QAction::triggered, this, &MainWindow::language_de);
  connect(m_ui.action_lang_fr, &QAction::triggered, this, &MainWindow::language_fr);
  connect(m_ui.action_lang_it, &QAction::triggered, this, &MainWindow::language_it);
  connect(m_ui.action_lang_nl, &QAction::triggered, this, &MainWindow::language_nl);
  connect(m_ui.action_lang_sl, &QAction::triggered, this, &MainWindow::language_sl);
  connect(m_ui.action_open_converter, &QAction::triggered, this, &MainWindow::open_converter);
  connect(m_ui.action_about, &QAction::triggered, this, &MainWindow::about);
  connect(m_ui.title_edit, &QLineEdit::returnPressed, this, &MainWindow::filter);
  connect(m_ui.category_edit, &QLineEdit::returnPressed, this, &MainWindow::filter);
  connect(m_ui.ingredient_edit, &QLineEdit::returnPressed, this, &MainWindow::filter);
  connect(m_ui.filter_button, &QPushButton::clicked, this, &MainWindow::filter);
  connect(m_ui.reset_button, &QPushButton::clicked, this, &MainWindow::reset);
  connect(m_ui.titles_view, &QListView::customContextMenuRequested, this, &MainWindow::titles_context_menu);
  connect(m_ui.recipe_browser, &QTextBrowser::customContextMenuRequested, this, &MainWindow::recipe_context_menu);
  m_ui.category_edit->installEventFilter(this);
  m_titles_context_menu = new QMenu(this);
  m_titles_context_menu->addAction(m_ui.action_export);
  m_titles_context_menu->addAction(m_ui.action_edit);
  m_titles_context_menu->addAction(m_ui.action_add_to_category);
  m_titles_context_menu->addAction(m_ui.action_remove_from_category);
  m_titles_context_menu->addAction(m_ui.action_preview);
  m_titles_context_menu->addAction(m_ui.action_print);
  m_titles_context_menu->addAction(m_ui.action_delete);
  m_recipe_context_menu = new QMenu(this);
  m_recipe_context_menu->addAction(m_ui.action_edit);
  m_recipe_context_menu->addAction(m_ui.action_preview);
  m_recipe_context_menu->addAction(m_ui.action_print);
  try {
    QString path = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir dir(path);
    dir.mkpath(dir.absolutePath());
    m_database.open(dir.filePath("anymeal.sqlite").toUtf8().constData());
    m_titles_model = new TitlesModel(this, &m_database);
    m_ui.titles_view->setModel(m_titles_model);
    connect(m_ui.titles_view->selectionModel(), &QItemSelectionModel::currentChanged, this, &MainWindow::selected);
    m_categories_model = new CategoriesModel(this, &m_database);
    m_category_table_model = new CategoryTableModel(this, &m_database);
    m_categories_completer = new QCompleter(m_categories_model, this);
    m_categories_completer->setCaseSensitivity(Qt::CaseInsensitive);
    m_ui.category_edit->setCompleter(m_categories_completer);
    show_num_recipes();
  } catch (exception &e) {
    QMessageBox::critical(this, tr("Error Opening Database"), e.what());
    exit(1);
  };
}

void MainWindow::switch_language(const QString &country) {
  if (m_translator) {
    qApp->removeTranslator(m_translator);
  } else {
    m_translator = new QTranslator(this);
  }
#ifdef __MINGW32__
  QString locale_dir = QString("locale/%1").arg(country);
#else
  QString app_dir = QCoreApplication::applicationDirPath();
  QString locale_dir = QString("%1/../share/locale/%2/LC_MESSAGES").arg(app_dir).arg(country);
#endif
  if (m_translator->load("anymeal_qt", locale_dir)) {
    qApp->installTranslator(m_translator);
  }
  m_ui.retranslateUi(this);
  m_converter_window.m_ui.retranslateUi(&m_converter_window);
  m_import_dialog.m_ui.retranslateUi(&m_import_dialog);
  m_export_dialog.m_ui.retranslateUi(&m_export_dialog);
  m_category_picker.m_ui.retranslateUi(&m_category_picker);
  set_recipe(m_recipe);
}

void MainWindow::switch_and_set_language(const char *country) {
  switch_language(country);
  m_settings.setValue("language", country);
}

void MainWindow::set_recipe(Recipe recipe) {
  m_recipe = recipe;
  m_ui.recipe_browser->setHtml(recipe_to_html(recipe, &translate).c_str());
}

void MainWindow::language_en(void)
{
  switch_and_set_language("en");
}

void MainWindow::language_de(void)
{
  switch_and_set_language("de");
}

void MainWindow::language_fr(void)
{
  switch_and_set_language("fr");
}

void MainWindow::language_it(void)
{
  switch_and_set_language("it");
}

void MainWindow::language_nl(void)
{
  switch_and_set_language("nl");
}

void MainWindow::language_sl(void)
{
  switch_and_set_language("sl");
}

bool MainWindow::eventFilter(QObject *object, QEvent *event) {
  if (event->type() == QEvent::FocusIn) {
    if (object == m_ui.category_edit) {
      m_categories_completer->setCompletionPrefix(m_ui.category_edit->text());
      m_categories_completer->complete();
    }
  }
  return false;
}

void MainWindow::show_num_recipes(void) {
  statusBar()->showMessage(tr("Showing %1 recipes ...").arg(m_database.num_recipes()), 5000);
}

void MainWindow::import(void) {
  bool transaction = false;
  try {
    int result = m_import_dialog.exec();
    if (result == QDialog::Accepted) {
      Recoder recoder(m_import_dialog.encoding().c_str(), "UTF-8");
      QStringList result =
        QFileDialog::getOpenFileNames(this, tr("Import MealMaster Files"), "", tr("MealMaster (*.mm *.MM *.mmf *.MMF);;"
                                      "Text (*.txt *.TXT);;All files (*)"));
      if (!result.isEmpty()) {
        int success = 0;
        int failed = 0;
        ofstream error_file(m_import_dialog.error_file().c_str(), ofstream::binary);
        QProgressDialog progress(tr("Importing files ..."), tr("Cancel"), 0, result.size() * 100, this);
        progress.setWindowModality(Qt::WindowModal);
        for (int i=0; i<result.size(); i++) {
          m_database.begin();
          transaction = true;
          ifstream f(result.at(i).toUtf8().constData(), ifstream::binary);
          bool unexpected_eof = false;
          vector<string> lst = recipes(f, &unexpected_eof);
          int c = 0;
          for (vector<string>::iterator recipe=lst.begin(); recipe!=lst.end(); recipe++) {
            progress.setLabelText(tr("%1 imported and %2 failed ...").arg(success).arg(failed));
            progress.setValue(i * 100 + c++ * 100 / lst.size());
            istringstream s(*recipe);
            try {
              Recipe result = parse_mealmaster(s);
              Recipe recoded;
              if (m_import_dialog.encoding() == "UTF-8") {
                recoded = result;
              } else
                recoded = recoder.process_recipe(result);
              m_database.insert_recipe(recoded);
              success++;
            } catch (parse_exception &e) {
              failed++;
              error_file << tr("Rejected recipe: ").toUtf8().constData() << e.what() << "\r\n";
              error_file << *recipe;
              error_file.flush();
              if (!error_file) {
                ostringstream s;
                s << tr("Error writing to file ").toUtf8().constData() << m_import_dialog.error_file();
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
          } else if (unexpected_eof) {
            failed++;
            error_file << tr("Unexpected end of file in %1").arg(result.at(i)).toUtf8().constData() << "\r\n";
            error_file.flush();
            if (!error_file) {
              ostringstream s;
              s << tr("Error writing to file ").toUtf8().constData() << m_import_dialog.error_file();
              throw gui_exception(s.str());
            };
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

EditMode MainWindow::editing_mode(void) {
  QMessageBox message_box(this);
  message_box.setText(tr("Edit current recipe, a copy of the recipe, a new recipe, or cancel?"));
  QAbstractButton *button_current = message_box.addButton(tr("&Edit Current"), QMessageBox::ActionRole);
  QAbstractButton *button_copy = message_box.addButton(tr("Edit C&opy"), QMessageBox::ActionRole);
  QAbstractButton *button_new = message_box.addButton(tr("Edit &New"), QMessageBox::ActionRole);
  QAbstractButton *button_cancel = message_box.addButton(tr("&Cancel"), QMessageBox::RejectRole);
  message_box.exec();
  if (message_box.clickedButton() == button_current) return EDIT_CURRENT;
  if (message_box.clickedButton() == button_copy) return EDIT_COPY;
  if (message_box.clickedButton() == button_new) return EDIT_NEW;
  assert(message_box.clickedButton() == button_cancel);
  return EDIT_CANCEL;
}

void MainWindow::new_recipe(void) {
  edit_recipe(EDIT_NEW);
}

void MainWindow::edit(void) {
  EditMode mode;
  QModelIndex index = m_ui.titles_view->currentIndex();
  if (index.isValid()) {
    mode = editing_mode();
    if (mode == EDIT_CANCEL)
      return;
  } else
    mode = EDIT_NEW;
  edit_recipe(mode);
}

Recipe MainWindow::default_recipe(void)
{
  Recipe result;
  result.set_title(tr("Title").toUtf8().constData());
  result.set_servings(1);
  result.set_servings_unit(tr("Servings").toUtf8().constData());
  return result;
}

void MainWindow::edit_recipe(EditMode mode)
{
  Recipe recipe = default_recipe();
  sqlite3_int64 recipe_id = 0;
  QModelIndex index = m_ui.titles_view->currentIndex();
  if (index.isValid() && mode != EDIT_NEW) {
    recipe_id = m_titles_model->recipeid(index);
    recipe = m_database.fetch_recipe(recipe_id);
  } else if (mode == EDIT_CURRENT)
    return;  // index is not valid, so can't edit current
  EditDialog edit_dialog(this);
  edit_dialog.set_recipe(recipe);
  edit_dialog.set_category_table_model(m_category_table_model);
  if (edit_dialog.exec() == QDialog::Accepted) {
    Recipe result = edit_dialog.get_recipe();
    bool transaction = false;
    try {
      QGuiApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
      m_database.begin();
      transaction = true;
      if (mode == EDIT_CURRENT) {
        assert(recipe_id != 0);
        vector<sqlite3_int64> ids;
        ids.push_back(recipe_id);
        m_database.delete_recipes(ids);
      };
      sqlite3_int64 recipe_new_id = m_database.insert_recipe(result);
      set_recipe(result);
      m_database.commit();
      QModelIndex idx;
      if (mode == EDIT_CURRENT) {
        idx = m_titles_model->edit_entry(index, recipe_new_id, result.title_c_str());
      } else {
        idx = m_titles_model->add_entry(recipe_new_id, result.title_c_str());
      };
      m_ui.titles_view->setCurrentIndex(idx);
      m_categories_model->reset();
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
}

void MainWindow::add_to_category(void) {
  vector<sqlite3_int64> ids = recipe_ids();
  if (!ids.empty()) {
    CategoryDialog category_dialog;
    category_dialog.set_categories_model(m_categories_model);
    if (category_dialog.exec() == QDialog::Accepted) {
      try {
        QGuiApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
        m_database.begin();
        m_database.add_recipes_to_category(ids, category_dialog.category().c_str());
        m_database.commit();
        m_categories_model->reset();
        m_ui.titles_view->setCurrentIndex(QModelIndex());
        QGuiApplication::restoreOverrideCursor();
      } catch (exception &e) {
        try {
          m_database.rollback();
        } catch (exception &e) {
        };
        QGuiApplication::restoreOverrideCursor();
        QMessageBox::critical(this, tr("Error Adding Recipes to Category"), e.what());
      };
    };
  };
}

void MainWindow::remove_from_category(void) {
  vector<sqlite3_int64> ids = recipe_ids();
  if (!ids.empty()) {
    CategoryDialog category_dialog;
    category_dialog.set_categories_model(m_categories_model);
    if (category_dialog.exec() == QDialog::Accepted) {
      try {
        QGuiApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
        m_database.begin();
        m_database.remove_recipes_from_category(ids, category_dialog.category().c_str());
        m_database.commit();
        m_categories_model->reset();
        m_ui.titles_view->setCurrentIndex(QModelIndex());
        QGuiApplication::restoreOverrideCursor();
      } catch (exception &e) {
        try {
          m_database.rollback();
        } catch (exception &e) {
        };
        QGuiApplication::restoreOverrideCursor();
        QMessageBox::critical(this, tr("Error Removing Recipes from Category"), e.what());
      };
    };
  };
}

void MainWindow::collect_garbage(void) {
  try {
    QGuiApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    m_database.begin();
    m_database.garbage_collect();
    m_database.commit();
    QGuiApplication::restoreOverrideCursor();
  } catch (exception &e) {
    try {
      m_database.rollback();
    } catch (exception &) {
    };
    QGuiApplication::restoreOverrideCursor();
    QMessageBox::critical(this, tr("Error Collecting Garbage"), e.what());
  };
}

void MainWindow::open_converter(void) {
  m_converter_window.exec();
}

void MainWindow::about(void) {
  QMessageBox::information(this, tr("About AnyMeal %1").arg(PACKAGE_VERSION), "AnyMeal recipe management software\n"
      "Copyright © 2002 to 2024  Jan Wedekind\n\n"
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

void MainWindow::show_search_history(const char *type, const char *text)
{
  if (!m_ui.search_label->text().isEmpty())
    m_ui.search_label->setText(QString(", ") + m_ui.search_label->text());
  m_ui.search_label->setText(QString("<em>%1:</em>").arg(type) + text + m_ui.search_label->text());
  m_ui.search_label->show();
}

void MainWindow::reset_search_history(void)
{
  m_ui.search_label->hide();
  m_ui.search_label->setText("");
}

void MainWindow::filter(void) {
  try {
    QGuiApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    m_ui.search_label->show();
    if (!m_ui.title_edit->text().isEmpty()) {
      m_database.select_by_title(m_ui.title_edit->text().toUtf8().constData());
      show_search_history(tr("title").toUtf8().constData(), m_ui.title_edit->text().toUtf8().constData());
      m_titles_model->reset();
      m_categories_model->reset();
      m_ui.title_edit->setText("");
    };
    if (!m_ui.category_edit->text().isEmpty()) {
      if (m_ui.with_category_radio->isChecked()) {
        m_database.select_by_category(m_ui.category_edit->text().toUtf8().constData());
        show_search_history(tr("category").toUtf8().constData(), m_ui.category_edit->text().toUtf8().constData());
      } else {
        m_database.select_by_no_category(m_ui.category_edit->text().toUtf8().constData());
        show_search_history(tr("not category").toUtf8().constData(), m_ui.category_edit->text().toUtf8().constData());
      };
      m_titles_model->reset();
      m_categories_model->reset();
      m_ui.category_edit->setText("");
    };
    if (!m_ui.ingredient_edit->text().isEmpty()) {
      if (m_ui.with_ingredient_radio->isChecked()) {
        m_database.select_by_ingredient(m_ui.ingredient_edit->text().toUtf8().constData());
        show_search_history(tr("ingredient").toUtf8().constData(), m_ui.ingredient_edit->text().toUtf8().constData());
      } else {
        m_database.select_by_no_ingredient(m_ui.ingredient_edit->text().toUtf8().constData());
        show_search_history(tr("not ingredient").toUtf8().constData(), m_ui.ingredient_edit->text().toUtf8().constData());
      };
      m_titles_model->reset();
      m_categories_model->reset();
      m_ui.ingredient_edit->setText("");
    };
    show_num_recipes();
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
    reset_search_history();
    m_titles_model->reset();
    m_categories_model->reset();
    show_num_recipes();
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
    if (current.isValid()) {
      Recipe recipe = m_database.fetch_recipe(m_titles_model->recipeid(current));
      set_recipe(recipe);
    } else {
      m_ui.recipe_browser->clear();
    };
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
  QItemSelectionModel *model = m_ui.titles_view->selectionModel();
  QModelIndexList lst = model->selectedRows();
  for (QModelIndexList::iterator index=lst.begin(); index!=lst.end(); index++) {
    result.push_back(m_titles_model->recipeid(*index));
  };
  return result;
}

void MainWindow::export_recipes(void) {
  vector<sqlite3_int64> ids = recipe_ids();
  if (!ids.empty()) {
    try {
      int result = m_export_dialog.exec();
      Recoder recoder("UTF-8", m_export_dialog.encoding().c_str());
      if (result == QDialog::Accepted) {
        QString result =
          QFileDialog::getSaveFileName(this, tr("Export MealMaster File"), "", tr("MealMaster (*.mm *.MM *.mmf *.MMF);;"
                                       "Text (*.txt *.TXT);;All files (*)"));
        if (!result.isEmpty()) {
          int success = 0;
          int failed = 0;
          ofstream error_file(m_export_dialog.error_file().c_str(), ofstream::binary);
          ofstream output_file(result.toUtf8().constData(), ofstream::binary);
          QProgressDialog progress(tr("Exporting recipes ..."), tr("Cancel"), 0, ids.size(), this);
          progress.setWindowModality(Qt::WindowModal);
          for (unsigned int i=0; i<ids.size(); i++) {
            progress.setValue(i);
            sqlite3_int64 id = ids[i];
            Recipe recipe = m_database.fetch_recipe(id);
            try {
              Recipe recoded;
              if (m_export_dialog.encoding() == "UTF-8") {
                recoded = recipe;
              } else
                recoded = recoder.process_recipe(recipe);
              string txt = recipe_to_mealmaster(recoded);
              output_file << txt << "\r\n";
              success++;
              if (i + 1 < ids.size())
                output_file << "\r\n";
            } catch (recode_exception &e) {
              failed++;
              error_file << tr("Failed recipe: ").toUtf8().constData() << e.what() << "\r\n";
              error_file << recipe_to_mealmaster(recipe) << "\r\n";
              error_file.flush();
              if (!error_file) {
                ostringstream s;
                s << tr("Error writing to file ").toUtf8().constData() << m_import_dialog.error_file();
                throw gui_exception(s.str());
              };
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
  vector<sqlite3_int64> ids = recipe_ids();
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
  QGuiApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
  QTextBrowser text_browser;
  vector<Recipe> recipes = m_database.fetch_recipes(recipe_ids());
  text_browser.setHtml(recipes_to_html(recipes, &translate).c_str());
  text_browser.print(printer);
  QGuiApplication::restoreOverrideCursor();
}

void MainWindow::remove_duplicates(void) {
  vector<sqlite3_int64> ids = recipe_ids();
  set<string> recipes;
  vector<sqlite3_int64> recipes_to_delete;
  QProgressDialog progress(tr("Detecting duplicates ..."), tr("Cancel"), 0, ids.size(), this);
  progress.setWindowModality(Qt::WindowModal);
  for (unsigned int i=0; i<ids.size(); i++) {
    progress.setLabelText(tr("Found %1 duplicates ...").arg(recipes_to_delete.size()));
    progress.setValue(i);
    sqlite3_int64 id = ids[i];
    Recipe recipe = m_database.fetch_recipe(id);
    string txt = recipe_to_mealmaster(recipe);
    if (recipes.find(txt) != recipes.end())
      recipes_to_delete.push_back(id);
    else
      recipes.insert(txt);
    if (progress.wasCanceled())
      break;
  };
  if (!progress.wasCanceled()) {
    QGuiApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    m_database.begin();
    m_database.delete_recipes(recipes_to_delete);
    m_database.commit();
    m_titles_model->reset();
    m_categories_model->reset();
    QGuiApplication::restoreOverrideCursor();
  };
}

/* AnyMeal recipe management software
   Copyright (C) 2020, 2023 Jan Wedekind

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
#pragma once
#include <vector>
#include <QtCore/QTranslator>
#include <QtCore/QSettings>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QCompleter>
#include <QtPrintSupport/QPrinter>
#include "ui_main_window.hh"
#include "database.hh"
#include "category_picker.hh"
#include "titles_model.hh"
#include "categories_model.hh"
#include "category_table_model.hh"
#include "converter_window.hh"
#include "import_dialog.hh"
#include "export_dialog.hh"
#include "recipe.hh"


class gui_exception: public std::exception
{
public:
  gui_exception(const std::string &error): m_error(error) {}
  virtual ~gui_exception(void) throw() {}
  virtual const char *what(void) const throw() { return m_error.c_str(); }
protected:
  std::string m_error;
};

typedef enum { EDIT_CURRENT = 0, EDIT_COPY, EDIT_NEW, EDIT_CANCEL } EditMode;

class MainWindow: public QMainWindow
{
  Q_OBJECT
public:
  MainWindow(QWidget *parent=NULL);
  static std::string translate(const char *context, const char *text);
  std::vector<sqlite3_int64> recipe_ids(void);
  void show_num_recipes(void);
  EditMode editing_mode(void);
  Recipe default_recipe(void);
  void edit_recipe(EditMode mode);
  void show_search_history(const char *type, const char *text);
  void reset_search_history(void);
  void switch_language(const QString &country);
  void switch_and_set_language(const char *country);
  void set_recipe(Recipe recipe);
public slots:
  void import(void);
  void new_recipe(void);
  void edit(void);
  void collect_garbage(void);
  void about(void);
  void filter(void);
  void reset(void);
  void selected(const QModelIndex &current, const QModelIndex &previous);
  void titles_context_menu(const QPoint &pos);
  void recipe_context_menu(const QPoint &pos);
  void export_recipes(void);
  void delete_recipes(void);
  void preview(void);
  void print(void);
  void add_to_category(void);
  void remove_from_category(void);
  void render(QPrinter *printer);
  void language_en(void);
  void language_de(void);
  void language_fr(void);
  void language_it(void);
  void language_nl(void);
  void language_sl(void);
  void open_converter(void);
  void remove_duplicates(void);
protected:
  bool eventFilter(QObject *object, QEvent *event);
  Ui::MainWindow m_ui;
  QSettings m_settings;
  Recipe m_recipe;
  QTranslator *m_translator;
  Database m_database;
  ConverterWindow m_converter_window;
  ImportDialog m_import_dialog;
  ExportDialog m_export_dialog;
  CategoryPicker m_category_picker;
  TitlesModel *m_titles_model;
  CategoriesModel *m_categories_model;
  CategoryTableModel *m_category_table_model;
  QCompleter *m_categories_completer;
  QMenu *m_titles_context_menu;
  QMenu *m_recipe_context_menu;
};

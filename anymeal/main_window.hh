#pragma once
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QCompleter>
#include <QtPrintSupport/QPrinter>
#include "ui_main_window.hh"
#include "database.hh"
#include "titles_model.hh"
#include "categories_model.hh"


class gui_exception: public std::exception
{
public:
  gui_exception(const std::string &error): m_error(error) {}
  virtual const char *what(void) const throw() { return m_error.c_str(); }
protected:
  std::string m_error;
};

class MainWindow: public QMainWindow
{
  Q_OBJECT
public:
  MainWindow(QWidget *parent=nullptr);
public slots:
  void import(void);
  void about(void);
  void filter(void);
  void reset(void);
  static std::string translate(const char *context, const char *text);
  void selected(const QModelIndex &index);
  void delete_recipes(void);
  void preview(void);
  void print(void);
  void render(QPrinter *printer);
protected:
  Ui::MainWindow m_ui;
  Database m_database;
  TitlesModel *m_titles_model;
  CategoriesModel *m_categories_model;
  QCompleter *m_categories_completer;
};

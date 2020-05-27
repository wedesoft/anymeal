#pragma once
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QCompleter>
#include "ui_main_window.hh"
#include "database.hh"
#include "titles_model.hh"
#include "categories_model.hh"


class MainWindow: public QMainWindow
{
  Q_OBJECT
public:
  MainWindow(QWidget *parent=nullptr);
public slots:
  void import(void);
  void filter(void);
  void reset(void);
protected:
  Ui::MainWindow m_ui;
  Database m_database;
  TitlesModel *m_titles_model;
  CategoriesModel *m_categories_model;
  QCompleter *m_categories_completer;
};

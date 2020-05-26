#pragma once
#include <QtWidgets/QMainWindow>
#include "ui_main_window.hh"
#include "database.hh"
#include "titles_model.hh"


class MainWindow: public QMainWindow
{
  Q_OBJECT
public:
  MainWindow(QWidget *parent=nullptr);
public slots:
  void import(void);
protected:
  Ui::MainWindow m_ui;
  Database m_database;
  TitlesModel *m_titles_model;
};

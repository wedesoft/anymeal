#pragma once
#include <QtWidgets/QMainWindow>
#include "ui_main_window.hh"


class MainWindow: public QMainWindow
{
  Q_OBJECT
public:
  MainWindow(QWidget *parent=NULL);
protected:
  Ui::MainWindow ui;
};

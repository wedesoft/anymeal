#pragma once
#include <string>
#include <QtWidgets/QDialog>
#include "ui_import_dialog.hh"


class ImportDialog: public QDialog
{
  Q_OBJECT
public:
  ImportDialog(QWidget *parent=nullptr);
  std::string encoding(void);
  std::string error_file(void);
public slots:
  void select_error_file(void);
protected:
  Ui::ImportDialog m_ui;
};

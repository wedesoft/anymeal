#pragma once
#include <QtWidgets/QDialog>
#include "ui_export_dialog.hh"


class ExportDialog: public QDialog
{
  Q_OBJECT
public:
  ExportDialog(QWidget *parent=nullptr);
  std::string encoding(void);
protected:
  Ui::ExportDialog m_ui;
};

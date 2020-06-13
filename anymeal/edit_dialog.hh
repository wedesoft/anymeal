#pragma once
#include <QtWidgets/QDialog>
#include "ui_edit_dialog.hh"


class EditDialog: public QDialog
{
  Q_OBJECT
public:
  EditDialog(QWidget *parent=nullptr);
protected:
  Ui::EditDialog m_ui;
};

/* AnyMeal recipe management software
   Copyright (C) 2020, 2024 Jan Wedekind

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
#include <QtWidgets/QDialog>
#include <QRegularExpressionValidator>
#include "ui_rename_dialog.hh"
#include "category_table_model.hh"

class RenameDialog: public QDialog
{
  Q_OBJECT
public:
  RenameDialog(QWidget *parent=NULL);
  void set_model(CategoryTableModel *model) { m_model = model; }
  void set_name(const std::string &name);
  std::string name(void);
public slots:
  void target_changed(const QString &target);
protected:
  Ui::RenameDialog m_ui;
  CategoryTableModel *m_model;
  QValidator *m_category_validator;
};

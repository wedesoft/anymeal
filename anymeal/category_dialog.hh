/* AnyMeal recipe database software
   Copyright (C) 2020 Jan Wedekind

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
#include <QtWidgets/QCompleter>
#include "ui_category_dialog.hh"
#include "categories_model.hh"


class CategoryDialog: public QDialog
{
  Q_OBJECT
public:
  CategoryDialog(QWidget *parent=nullptr);
  void set_categories_model(CategoriesModel *categories_model);
  const char *category(void);
protected:
  Ui::CategoryDialog m_ui;
  QCompleter *m_categories_completer;
};

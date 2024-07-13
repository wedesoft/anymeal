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
#include <QtCore/QSortFilterProxyModel>
#include <QtWidgets/QDialog>
#include "category_table_model.hh"
#include "ui_category_picker.hh"


class CategoryPicker: public QDialog
{
  Q_OBJECT
public:
  CategoryPicker(QWidget *parent=NULL);
  void set_model(CategoryTableModel *model);
public slots:
  void delete_categories(void);
protected:
  Ui::CategoryPicker m_ui;
  QSortFilterProxyModel m_sort_filter_proxy_model;
  CategoryTableModel *m_model;
};

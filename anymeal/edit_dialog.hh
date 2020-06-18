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
#include "recipe.hh"
#include "ingredient_model.hh"
#include "ui_edit_dialog.hh"


class EditDialog: public QDialog
{
  Q_OBJECT
public:
  EditDialog(QWidget *parent=nullptr);
  void set_recipe(Recipe &recipe);
public slots:
  void select_ingredient(const QModelIndex &current, const QModelIndex &previous);
  void unit_changed(int idx);
  void name_changed(const QString &text);
  void amount_type_changed(int value);
  void amount_int_changed(int value);
  void amount_float_changed(double value);
  void add_ingredient(void);
protected:
  Ui::EditDialog m_ui;
  IngredientModel *m_ingredient_model;
};

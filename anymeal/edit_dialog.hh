/* AnyMeal recipe management software
   Copyright (C) 2020, 2023 Jan Wedekind

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
#include <QtGui/QRegExpValidator>
#include "recipe.hh"
#include "ingredient_model.hh"
#include "instructions_model.hh"
#include "category_picker.hh"
#include "converter_window.hh"
#include "ui_edit_dialog.hh"


class EditDialog: public QDialog
{
  Q_OBJECT
public:
  EditDialog(QWidget *parent=NULL);
  void set_recipe(Recipe &recipe);
  Recipe get_recipe(void);
  void set_category_picker(CategoryPicker *category_picker);
  int fraction_str_length(void);
public slots:
  void select_categories(void);
  void select_ingredient(const QModelIndex &current, const QModelIndex &previous);
  void unit_changed(int idx);
  void ingredient_name_changed(const QString &text);
  void amount_type_changed(int value);
  void amount_int_changed(int value);
  void amount_float_changed(double value);
  void add_ingredient(void);
  void delete_ingredient(void);
  void ingredient_section_changed(const QString &text);
  void add_ingredient_section(void);
  void move_ingredient_up(void);
  void move_ingredient_down(void);
  void select_instruction(const QModelIndex &current, const QModelIndex &previous);
  void section_changed(const QString &text);
  void add_instruction_section(void);
  void remove_instruction_section(void);
  void instructions_text_changed(void);
  void update_ok_button(void);
  void unit_converter(void);
protected:
  Ui::EditDialog m_ui;
  ConverterWindow m_converter_window;
  CategoryPicker *m_category_picker;
  IngredientModel *m_ingredient_model;
  InstructionsModel *m_instructions_model;
  QRegExpValidator *m_title_validator;
  QRegExpValidator *m_servings_unit_validator;
};

/* AnyMeal recipe management software
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
#include <string>
#include <QtWidgets/QDialog>
#include "ui_converter_window.hh"


#define INGREDIENTS 24
#define CUSTOM 23

typedef enum { WEIGHT = 0, VOLUME = 1 } UnitType;

class ConverterWindow: public QDialog
{
  Q_OBJECT
public:
  ConverterWindow(QWidget *parent=nullptr);
  static std::string translate(const char *context, const char *text);
  static double conversion_factor(int unit_index);
  static UnitType unit_type(int unit_index);
  QString ingredient(int index);
  double density(int index);
public slots:
  void update_value(void);
  void change_ingredient(int index);
protected:
  Ui::ConverterWindow m_ui;
};

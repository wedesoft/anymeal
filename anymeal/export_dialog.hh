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

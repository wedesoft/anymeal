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
#include "export_dialog.hh"


using namespace std;

ExportDialog::ExportDialog(QWidget *parent):
  QDialog(parent)
{
  m_ui.setupUi(this);
}

string ExportDialog::encoding(void) {
  return m_ui.encoding_combo->currentText().toUtf8().constData();
}

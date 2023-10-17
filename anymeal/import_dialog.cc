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
#include <QtCore/QStandardPaths>
#include <QtWidgets/QFileDialog>
#include "import_dialog.hh"


using namespace std;

ImportDialog::ImportDialog(QWidget *parent):
  QDialog(parent)
{
  m_ui.setupUi(this);
  connect(m_ui.browse_button, &QPushButton::clicked, this, &ImportDialog::select_error_file);
  QString path = QStandardPaths::writableLocation(QStandardPaths::TempLocation);
  QDir dir(path);
  m_ui.error_file_edit->setText(dir.filePath(tr("errors.mmf")));
}

string ImportDialog::encoding(void) {
  return m_ui.encoding_combo->currentText().toUtf8().constData();
}

string ImportDialog::error_file(void) {
  return m_ui.error_file_edit->text().toUtf8().constData();
}

void ImportDialog::select_error_file(void) {
  QString result = QFileDialog::getSaveFileName(this, tr("Select Error File"), "", tr("MealMaster (*.mm *.MM *.mmf *.MMF);;"
                                                "Text (*.txt *.TXT);;All files (*)"));
  if (!result.isEmpty()) {
    m_ui.error_file_edit->setText(result);
  };
}

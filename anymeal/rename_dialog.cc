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
#include "rename_dialog.hh"

using namespace std;

RenameDialog::RenameDialog(QWidget *parent):
  QDialog(parent), m_model(NULL), m_category_validator(NULL)
{
  m_ui.setupUi(this);
  connect(m_ui.rename_edit, &QLineEdit::textChanged, this, &RenameDialog::target_changed);
  m_category_validator = new QRegularExpressionValidator(QRegularExpression("[^,]*"), this);
  m_ui.rename_edit->setValidator(m_category_validator);
}

void RenameDialog::set_name(const string &name) {
  m_ui.rename_edit->setText(name.c_str());
}

string RenameDialog::name(void) {
  return m_ui.rename_edit->text().toUtf8().constData();
}

void RenameDialog::target_changed(const QString &target) {
  m_ui.button_box->setEnabled(m_model->get_category_id(target.toUtf8().constData()) == 0);
}

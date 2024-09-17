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
#include "add_dialog.hh"

using namespace std;

AddDialog::AddDialog(QWidget *parent):
  QDialog(parent), m_model(NULL), m_category_validator(NULL)
{
  m_ui.setupUi(this);
  connect(m_ui.add_edit, &QLineEdit::textChanged, this, &AddDialog::target_changed);
  m_category_validator = new QRegularExpressionValidator(QRegularExpression("[^,]*"), this);
  m_ui.add_edit->setValidator(m_category_validator);
}

string AddDialog::name(void) {
  return m_ui.add_edit->text().toUtf8().constData();
}

void AddDialog::target_changed(const QString &target) {
  const char *target_str = target.toUtf8().constData();
  bool exist = m_model->get_category_id(target_str);
  m_ui.button_box->setEnabled(!exist);
}

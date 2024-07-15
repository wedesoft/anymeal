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
#include "merge_dialog.hh"

using namespace std;

MergeDialog::MergeDialog(QWidget *parent):
  QDialog(parent), m_model(NULL), m_category_validator(NULL)
{
  m_ui.setupUi(this);
  connect(m_ui.merge_edit, &QLineEdit::textChanged, this, &MergeDialog::target_changed);
  m_category_validator = new QRegExpValidator(QRegExp("[^,]*"), this);
  m_ui.merge_edit->setValidator(m_category_validator);
}

void MergeDialog::set_name(const string &name) {
  m_name = name;
  m_ui.merge_edit->setText(name.c_str());
}

string MergeDialog::name(void) {
  return m_ui.merge_edit->text().toUtf8().constData();
}

void MergeDialog::target_changed(const QString &target) {
  const char *target_str = target.toUtf8().constData();
  bool equal = m_name == target_str;
  bool exist = m_model->get_category_id(target_str);
  m_ui.button_box->setEnabled(!equal && exist);
}

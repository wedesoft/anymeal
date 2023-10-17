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
#include "category_dialog.hh"


using namespace std;

CategoryDialog::CategoryDialog(QWidget *parent):
  QDialog(parent), m_categories_completer(NULL)
{
  m_ui.setupUi(this);
}

void CategoryDialog::set_categories_model(CategoriesModel *categories_model) {
  if (m_categories_completer) {
    delete m_categories_completer;
    m_categories_completer = NULL;
  };
  m_categories_completer = new QCompleter(categories_model, this);
  m_ui.category_edit->setCompleter(m_categories_completer);
}

string CategoryDialog::category(void) {
  return m_ui.category_edit->text().toUtf8().constData();
}

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
#include "categories_model.hh"


CategoriesModel::CategoriesModel(QObject *parent, Database *database): QAbstractListModel(parent), m_database(database) {
  m_categories = m_database->categories();
}

void CategoriesModel::reset(void) {
  beginResetModel();
  m_categories = m_database->categories();
  endResetModel();
}

int CategoriesModel::rowCount(const QModelIndex &) const {
  return m_categories.size();
}

QVariant CategoriesModel::data(const QModelIndex &index, int role) const {
  QVariant result;
  if (role == Qt::DisplayRole || role == Qt::EditRole) {
    int row = index.row();
    result = QString(m_categories[row].c_str());
  };
  return result;
}

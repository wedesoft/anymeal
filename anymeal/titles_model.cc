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
#include "titles_model.hh"


TitlesModel::TitlesModel(QObject *parent, Database *database): QAbstractListModel(parent), m_database(database) {
  m_titles = m_database->recipe_info();
}

void TitlesModel::reset(void) {
  beginResetModel();
  m_titles = m_database->recipe_info();
  endResetModel();
}

int TitlesModel::rowCount(const QModelIndex &) const {
  return m_titles.size();
}

QVariant TitlesModel::data(const QModelIndex &index, int role) const {
  QVariant result;
  if (role == Qt::DisplayRole) {
    int row = index.row();
    result = QString(m_titles[row].second.c_str());
  };
  return result;
}

sqlite_int64 TitlesModel::recipeid(const QModelIndex &index) {
  int row = index.row();
  return m_titles[row].first;
}

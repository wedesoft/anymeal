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
#include "category_table_model.hh"

using namespace std;


CategoryTableModel::CategoryTableModel(QObject *parent, Database *database):
  QAbstractTableModel(parent), m_database(database)
{
}

void CategoryTableModel::reset(const set<string> &selection) {
  beginResetModel();
  m_categories_and_counts = m_database->categories_and_counts();
  m_selection = selection;
  endResetModel();
}

int CategoryTableModel::rowCount(const QModelIndex &) const {
  return m_categories_and_counts.size();
}

int CategoryTableModel::columnCount(const QModelIndex &) const {
  return 2;
}

QVariant CategoryTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
  if (role == Qt::DisplayRole) {
    if (orientation == Qt::Horizontal) {
      switch (section) {
        case 0:
          return QVariant(tr("Category"));
        case 1:
          return QVariant(tr("# Recipes"));
      };
    } else
      return section;
  };
  return QVariant();
}

QVariant CategoryTableModel::data(const QModelIndex &index, int role) const {
  QVariant result;
  int row = index.row();
  int column = index.column();
  if (role == Qt::DisplayRole || role == Qt::EditRole) {
    switch (column) {
      case 0:
        result = QString(m_categories_and_counts[row].first.c_str());
        break;
      case 1:
        result = m_categories_and_counts[row].second;
        break;
    };
  } else if (role == Qt::CheckStateRole && column == 0) {
    string category = m_categories_and_counts[row].first;
    result = m_selection.find(category) != m_selection.end() ? Qt::Checked : Qt::Unchecked;
  };
  return result;
}

Qt::ItemFlags CategoryTableModel::flags(const QModelIndex &index) const
{
  int column = index.column();
  Qt::ItemFlags f = QAbstractTableModel::flags(index);
  if (column == 0)
    f |= Qt::ItemIsUserCheckable;
  return f;
}

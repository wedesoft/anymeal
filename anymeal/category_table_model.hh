/* AnyMeal recipe management software
   Copyright (C) 2020, 2025 Jan Wedekind

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
#include <vector>
#include <set>
#include <string>
#include <QtCore/QAbstractTableModel>
#include "database.hh"


class CategoryTableModel: public QAbstractTableModel
{
  Q_OBJECT
public:
  CategoryTableModel(QObject *parent, Database *database);
  int rowCount(const QModelIndex &parent=QModelIndex()) const;
  int columnCount(const QModelIndex &parent=QModelIndex()) const;
  QVariant headerData(int section, Qt::Orientation orientation, int role) const;
  QVariant data(const QModelIndex &index, int role=Qt::DisplayRole) const;
  bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
  Qt::ItemFlags flags(const QModelIndex &index) const;
  void reset(const std::set<std::string> &selection);
  std::set<std::string> selection(void) { return m_selection; };
  std::string category(int row) { return m_categories_and_counts[row].first; }
  QModelIndex add_category(const std::string &name);
  void delete_category(int row);
  void rename_category(int row, const std::string &name);
  void merge_category(int row, const std::string &name);
  sqlite3_int64 get_category_id(const char *name) { return m_database->get_category_id(name); }
protected:
  Database *m_database;
  std::set<std::string> m_selection;
  std::vector<std::pair<std::string, int> > m_categories_and_counts;
};

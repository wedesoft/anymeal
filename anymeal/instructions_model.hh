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
#pragma once
#include <vector>
#include <string>
#include <QtCore/QAbstractListModel>
#include "recipe.hh"


class InstructionsModel: public QAbstractListModel
{
  Q_OBJECT
public:
  InstructionsModel(QObject *parent, std::vector<std::string> &instructions, std::vector<std::pair<int, std::string>> &sections);
  virtual int rowCount(const QModelIndex &parent=QModelIndex()) const;
  virtual QVariant data(const QModelIndex &index, int role=Qt::DisplayRole) const;
  std::string get_section(const QModelIndex &index) const;
  void set_section(const QModelIndex &index, const char *text);
  QModelIndex add_section(const QModelIndex &idx);
  QModelIndex remove_section(const QModelIndex &idx);
protected:
  std::vector<std::string> m_instructions;
  std::vector<std::pair<int, std::string>> m_sections;
};

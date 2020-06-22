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
#include <QtCore/QAbstractItemModel>
#include <vector>
#include <string>
#include "ingredient.hh"


class IngredientModel: public QAbstractItemModel
{
  Q_OBJECT
public:
  IngredientModel(QObject *parent, std::vector<Ingredient> &ingredients, std::vector<std::pair<int, std::string>> &sections);
  static std::string translate(const char *context, const char *text);
  int columnCount(const QModelIndex &parent = QModelIndex()) const;
  QVariant headerData(int section, Qt::Orientation orientation, int role) const;
  int rowCount(const QModelIndex &parent = QModelIndex()) const;
  QVariant data(const QModelIndex &index, int role=Qt::DisplayRole) const;
  QModelIndex index(int row, int column, const QModelIndex &parent) const;
  QModelIndex parent(const QModelIndex &index) const;
  bool is_ingredient(const QModelIndex &index) const;
  int ingredient_index(const QModelIndex &index) const;
  Ingredient get_ingredient(const QModelIndex &index) const;
  void set_ingredient(const QModelIndex &index, Ingredient &ingredient);
  QModelIndex add_ingredient(const QModelIndex &idx, Ingredient &Ingredient);
  QModelIndex delete_ingredient(const QModelIndex &idx);
  std::string get_ingredient_section(const QModelIndex &index) const;
  void set_ingredient_section(const QModelIndex &index, const char *text);
  QModelIndex add_ingredient_section(const QModelIndex &idx, const char *text);
  void swap_neighbouring_sections(int row1, int row2);
  QModelIndex move_up(const QModelIndex &idx);
  QModelIndex move_down(const QModelIndex &idx);
protected:
  std::vector<Ingredient> m_ingredients;
  std::vector<std::pair<int, std::string>> m_sections;
};

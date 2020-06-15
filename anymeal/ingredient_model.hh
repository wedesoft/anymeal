#pragma once
#include <QtCore/QAbstractItemModel>
#include <vector>
#include <string>
#include "ingredient.hh"


class IngredientModel: public QAbstractItemModel
{
public:
  IngredientModel(QObject *parent, std::vector<Ingredient> &ingredients, std::vector<std::pair<int, std::string>> &sections);
  int columnCount(const QModelIndex &parent = QModelIndex());
protected:
  std::vector<Ingredient> m_ingredients;
  std::vector<std::pair<int, std::string>> m_sections;
};

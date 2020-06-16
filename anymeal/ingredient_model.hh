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
protected:
  std::vector<Ingredient> m_ingredients;
  std::vector<std::pair<int, std::string>> m_sections;
};

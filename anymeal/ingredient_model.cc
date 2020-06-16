#include <cstdint>
#include <sstream>
#include <QtCore/QCoreApplication>
#include "html.hh"
#include "ingredient_model.hh"


using namespace std;

IngredientModel::IngredientModel(QObject *parent, vector<Ingredient> &ingredients, vector<pair<int, string>> &sections):
  QAbstractItemModel(parent), m_ingredients(ingredients), m_sections(sections)
{
}

string IngredientModel::translate(const char *context, const char *text) {
  return QCoreApplication::translate(context, text).toUtf8().constData();
}

int IngredientModel::columnCount(const QModelIndex &) const {
  return 3;
}

QVariant IngredientModel::headerData(int section, Qt::Orientation orientation, int role) const
{
  if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
    switch (section) {
      case 0:
        return QVariant(tr("amount"));
      case 1:
        return QVariant(tr("unit"));
      case 2:
        return QVariant(tr("ingredient"));
    };
  };
  return QVariant();
}

int IngredientModel::rowCount(const QModelIndex &parent) const {
  if (parent.column() > 0)
    return 0;
  if (!parent.isValid())
    return m_sections.size() + 1; // Number of sections plus one for main section.
  intptr_t id = (intptr_t)parent.internalPointer();
  if (id % 10000 == 0) {
    // Return number of ingredients in a section.
    unsigned int section = id / 10000 - 1;
    int a = section == 0 ? 0 : m_sections[section - 1].first;
    int b = section == m_sections.size() ? m_ingredients.size() : m_sections[section].first;
    return b - a;
  };
  return 0;
}

QVariant IngredientModel::data(const QModelIndex &index, int role) const {
  if (!index.isValid())
    return QVariant();
  if (role != Qt::DisplayRole)
    return QVariant();
  intptr_t id = (intptr_t)index.internalPointer();
  if (id % 10000 == 0) {
    if (index.column() == 0) {
      if (index.row() == 0) {
        return QVariant(tr("<Main>"));
      } else {
        return QVariant(m_sections[index.row() - 1].second.c_str());
      };
    } else
      return QVariant();
  };
  int section = id / 10000 - 1;
  int offset = section == 0 ? 0 : m_sections[section - 1].first;
  int idx = offset + index.row();
  Ingredient ingredient = m_ingredients[idx];
  switch (index.column()) {
    case 0:
      return QVariant(html_amount(ingredient).c_str());
    case 1:
      return QVariant(html_unit(ingredient.unit(), &translate).c_str());
    case 2:
      return QVariant(ingredient.text().c_str());
  };
  return QVariant();
}

QModelIndex IngredientModel::index(int row, int column, const QModelIndex &parent) const
{
  if (!parent.isValid()) {
    return createIndex(row, column, (row + 1) * 10000);
  };
  intptr_t id = (intptr_t)parent.internalPointer();
  if (id % 10000 == 0) {
    return createIndex(row, column, id + row + 1);
  };
  return QModelIndex();
}

QModelIndex IngredientModel::parent(const QModelIndex &index) const
{
  if (!index.isValid())
    return QModelIndex();
  intptr_t id = (intptr_t)index.internalPointer();
  if (id % 10000 == 0) {
    // Parent of section is root node.
    return QModelIndex();
  };
  // Parent of ingredient is ingredient section.
  int section = id / 10000 - 1;
  return createIndex(section, 0, (section + 1) * 10000);
}

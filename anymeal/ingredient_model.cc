#include "ingredient_model.hh"


using namespace std;

IngredientModel::IngredientModel(QObject *parent, vector<Ingredient> &ingredients, vector<pair<int, string>> &sections):
  QAbstractItemModel(parent), m_ingredients(ingredients), m_sections(sections)
{
}

int IngredientModel::columnCount(const QModelIndex &) {
  return 3;
}

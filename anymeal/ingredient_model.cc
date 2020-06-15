#include "ingredient_model.hh"


using namespace std;

IngredientModel::IngredientModel(QObject *parent, vector<Ingredient> &ingredients, vector<pair<int, string>> &sections):
  QAbstractItemModel(parent), m_ingredients(ingredients), m_sections(sections)
{
}

int IngredientModel::columnCount(const QModelIndex &) const {
  return 3;
}

QVariant IngredientModel::headerData(int section, Qt::Orientation orientation, int role) const
{
  if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
    switch (section) {
      case 0:
        return QVariant("amount");
      case 1:
        return QVariant("unit");
      case 2:
        return QVariant("ingredient");
    };
  };
  return QVariant();
}

int IngredientModel::rowCount(const QModelIndex &parent) const {
  if (parent.column() > 0)
    return 0;
  if (!parent.isValid())
    return m_sections.size() + 1;
 // TODO: return number of ingredients for section.
 /*
    TreeItem *parentItem;
    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<TreeItem*>(parent.internalPointer());

    return parentItem->childCount();
  */
  return 0;
}

QVariant IngredientModel::data(const QModelIndex &index, int role) const {
  if (!index.isValid())
    return QVariant();
  if (role != Qt::DisplayRole)
    return QVariant();
  return QVariant("test");
}

QModelIndex IngredientModel::index(int row, int column, const QModelIndex &parent) const
{
  if (!hasIndex(row, column, parent))
    return QModelIndex();
  /*
    TreeItem *parentItem;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<TreeItem*>(parent.internalPointer());

    TreeItem *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    return QModelIndex();
   */
  return QModelIndex();
}

QModelIndex IngredientModel::parent(const QModelIndex &index) const
{
  if (!index.isValid())
    return QModelIndex();
  /*
  TreeItem *childItem = static_cast<TreeItem*>(index.internalPointer());
  TreeItem *parentItem = childItem->parentItem();

  if (parentItem == rootItem)
    return QModelIndex();

  return createIndex(parentItem->row(), 0, parentItem);
  */
  return QModelIndex();
}

#include "categories_model.hh"


CategoriesModel::CategoriesModel(QObject *parent, Database *database): QAbstractListModel(parent), m_database(database) {
  m_categories = m_database->categories();
}

void CategoriesModel::reset(void) {
  beginResetModel();
  m_categories = m_database->categories();
  endResetModel();
}

int CategoriesModel::rowCount(const QModelIndex &) const {
  return m_categories.size();
}

QVariant CategoriesModel::data(const QModelIndex &index, int role) const {
  QVariant result;
  if (role == Qt::DisplayRole || role == Qt::EditRole) {
    int row = index.row();
    result = QString(m_categories[row].c_str());
  };
  return result;
}

#include "titles_model.hh"


TitlesModel::TitlesModel(QObject *parent, Database *database): QAbstractListModel(parent), m_database(database) {
  m_titles = m_database->recipe_info();
}

void TitlesModel::reset(void) {
  beginResetModel();
  m_titles = m_database->recipe_info();
  endResetModel();
}

int TitlesModel::rowCount(const QModelIndex &) const {
  return m_titles.size();
}

QVariant TitlesModel::data(const QModelIndex &index, int role) const {
  QVariant result;
  if (role == Qt::DisplayRole) {
    int row = index.row();
    result = QString(m_titles[row].second.c_str());
  };
  return result;
}

sqlite_int64 TitlesModel::recipeid(const QModelIndex &index) {
  int row = index.row();
  return m_titles[row].first;
}

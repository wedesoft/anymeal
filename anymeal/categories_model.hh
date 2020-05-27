#pragma once
#include <vector>
#include <string>
#include <QtCore/QAbstractListModel>
#include "database.hh"


class CategoriesModel: public QAbstractListModel
{
public:
  CategoriesModel(QObject *parent, Database *database);
  void reset(void);
  virtual int rowCount(const QModelIndex &parent=QModelIndex()) const;
  virtual QVariant data(const QModelIndex &index, int role=Qt::DisplayRole) const;
protected:
  Database *m_database;
  std::vector<std::string> m_categories;
};

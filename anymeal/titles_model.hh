#pragma once
#include <vector>
#include <string>
#include <QtCore/QAbstractListModel>
#include "database.hh"


class TitlesModel: public QAbstractListModel
{
public:
  TitlesModel(QObject *parent, Database *database);
  void reset(void);
  virtual int rowCount(const QModelIndex &parent=QModelIndex()) const;
  virtual QVariant data(const QModelIndex &index, int role=Qt::DisplayRole) const;
  virtual QVariant headerData(int section, Qt::Orientation orientation, int role=Qt::DisplayRole) const;
protected:
  Database *m_database;
  std::vector<std::pair<sqlite3_int64, std::string>> m_titles;
};

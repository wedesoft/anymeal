#pragma once
#include <sqlite3.h>


class Database
{
public:
  Database(void);
  virtual ~Database(void);
  void open(const char *filename);
protected:
  sqlite3 *m_db;
};

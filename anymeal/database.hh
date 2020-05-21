#pragma once
#include <string>
#include <sqlite3.h>


class database_exception: public std::exception
{
public:
  database_exception(const std::string &error): m_error(error) {}
  virtual const char *what(void) const throw() { return m_error.c_str(); }
protected:
  std::string m_error;
};

class Database
{
public:
  Database(void);
  virtual ~Database(void);
  void open(const char *filename);
  sqlite3 *db(void) { return m_db; }
protected:
  sqlite3 *m_db;
};

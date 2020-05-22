#pragma once
#include <string>
#include <sqlite3.h>
#include "recipe.hh"


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
  void begin(void);
  void commit(void);
  void rollback(void);
  void insert_recipe(Recipe &recipe);
protected:
  void check(int result, const char *prefix);
  sqlite3 *m_db;
  sqlite3_stmt *m_begin;
  sqlite3_stmt *m_commit;
  sqlite3_stmt *m_rollback;
  sqlite3_stmt *m_insert_recipe;
  sqlite3_stmt *m_add_category;
  sqlite3_stmt *m_recipe_category;
};

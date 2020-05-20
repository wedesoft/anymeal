#include "database.hh"


Database::Database(void): m_db(nullptr) {
}

Database::~Database(void) {
  if (m_db)
    sqlite3_close(m_db);
}

void Database::open(const char *filename) {
  sqlite3_open(filename, &m_db);
  char *error = nullptr;
  sqlite3_exec(m_db,
    "CREATE TABLE IF NOT EXISTS recipes(id INT PRIMARY KEY, NAME title VARCHAR(100) NOT NULL);",
    nullptr, nullptr, &error);
  sqlite3_free(error);
}

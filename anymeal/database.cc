#include "database.hh"


Database::Database(void): m_db(nullptr) {
}

Database::~Database(void) {
  if (m_db)
    sqlite3_close(m_db);
}

void Database::open(const char *filename) {
  sqlite3_open(filename, &m_db);
}

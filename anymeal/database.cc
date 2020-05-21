#include <cassert>
#include <sstream>
#include "database.hh"


using namespace std;

Database::Database(void): m_db(nullptr) {
}

Database::~Database(void) {
  if (m_db)
    sqlite3_close(m_db);
}

void Database::open(const char *filename) {
  if (sqlite3_open(filename, &m_db) != SQLITE_OK) {
    ostringstream s;
    s << "Error opening database \"" << filename << "\": " << sqlite3_errmsg(m_db);
    throw database_exception(s.str());
  };
  char *error = nullptr;
  int result = sqlite3_exec(m_db,
    "CREATE TABLE IF NOT EXISTS recipes(id INT PRIMARY KEY, NAME title VARCHAR(100) NOT NULL);",
    nullptr, nullptr, &error);
  if (result != SQLITE_OK) {
    ostringstream s;
    s << "Error creating database tables: " << error;
    sqlite3_free(error);
    throw database_exception(s.str());
  };
  assert(!error);
}

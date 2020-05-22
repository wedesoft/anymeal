#include <cassert>
#include <sstream>
#include "database.hh"


using namespace std;

Database::Database(void): m_db(nullptr), m_begin(nullptr), m_commit(nullptr), m_insert_recipe(nullptr) {
}

Database::~Database(void) {
  sqlite3_finalize(m_begin);
  sqlite3_finalize(m_commit);
  sqlite3_finalize(m_rollback);
  sqlite3_finalize(m_insert_recipe);
  sqlite3_close(m_db);
}

void Database::check(int result, const char *prefix) {
  if (result != SQLITE_OK && result != SQLITE_DONE && result != SQLITE_ROW) {
    ostringstream s;
    s << prefix << sqlite3_errmsg(m_db);
    throw database_exception(s.str());
  };
}

void Database::open(const char *filename) {
  int result;
  result = sqlite3_open(filename, &m_db);
  check(result, "Error opening database: ");
  result = sqlite3_exec(m_db,
    "CREATE TABLE IF NOT EXISTS recipes(id INTEGER PRIMARY KEY, title VARCHAR(100) NOT NULL);",
    nullptr, nullptr, nullptr);
  check(result, "Error creating database tables: ");
  result = sqlite3_prepare_v2(m_db, "BEGIN;", -1, &m_begin, nullptr);
  check(result, "Error preparing begin transaction statement: ");
  result = sqlite3_prepare_v2(m_db, "COMMIT;", -1, &m_commit, nullptr);
  check(result, "Error preparing commit transaction statement: ");
  result = sqlite3_prepare_v2(m_db, "ROLLBACK;", -1, &m_rollback, nullptr);
  check(result, "Error preparing rollback transaction statement: ");
  result = sqlite3_prepare_v2(m_db, "INSERT INTO recipes VALUES(NULL, ?001);", -1, &m_insert_recipe, nullptr);
  check(result, "Error preparing insert statement: ");
}

void Database::begin(void) {
  int result = sqlite3_step(m_begin);
  check(result, "Error beginning transaction: ");
  result = sqlite3_reset(m_begin);
  check(result, "Error resetting begin transaction statement: ");
}

void Database::commit(void) {
  int result = sqlite3_step(m_commit);
  check(result, "Error committing transaction: ");
  result = sqlite3_reset(m_commit);
  check(result, "Error resetting commit transaction statement: ");
}

void Database::rollback(void) {
  int result = sqlite3_step(m_rollback);
  check(result, "Error rolling back transaction: ");
  result = sqlite3_reset(m_rollback);
  check(result, "Error resetting rollback transaction statement: ");
}

void Database::insert_recipe(Recipe &recipe) {
  assert(m_insert_recipe);
  int result;
  result = sqlite3_bind_text(m_insert_recipe, 1, recipe.title().c_str(), -1, SQLITE_STATIC);
  check(result, "Error binding recipe title: ");
  result = sqlite3_step(m_insert_recipe);
  check(result, "Error executing insert statement: ");
  result = sqlite3_reset(m_insert_recipe);
  check(result, "Error resetting insert statement: ");
}

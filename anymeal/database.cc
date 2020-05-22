#include <cassert>
#include <sstream>
#include "database.hh"


using namespace std;

Database::Database(void):
  m_db(nullptr), m_begin(nullptr), m_commit(nullptr), m_rollback(nullptr), m_insert_recipe(nullptr), m_add_category(nullptr),
  m_recipe_category(nullptr)
{
}

Database::~Database(void) {
  sqlite3_finalize(m_begin);
  sqlite3_finalize(m_commit);
  sqlite3_finalize(m_rollback);
  sqlite3_finalize(m_insert_recipe);
  sqlite3_finalize(m_add_category);
  sqlite3_finalize(m_recipe_category);
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
    "PRAGMA user_version = 1;\n"
    "PRAGMA foreign_keys=ON;\n"
    "CREATE TABLE IF NOT EXISTS recipes(id INTEGER PRIMARY KEY, title VARCHAR(100) NOT NULL);\n"
    "CREATE TABLE IF NOT EXISTS categories(id INTEGER PRIMARY KEY, name VARCHAR(100) UNIQUE NOT NULL);\n"
    "CREATE TABLE IF NOT EXISTS category(recipeid INTEGER NOT NULL, categoryid INTEGER NOT NULL, PRIMARY KEY(recipeid, categoryid), "
    "FOREIGN KEY(recipeid) REFERENCES recipes(id), FOREIGN KEY (categoryid) REFERENCES categories(id));",
    nullptr, nullptr, nullptr);
  check(result, "Error creating database tables: ");
  result = sqlite3_prepare_v2(m_db, "BEGIN;", -1, &m_begin, nullptr);
  check(result, "Error preparing begin transaction statement: ");
  result = sqlite3_prepare_v2(m_db, "COMMIT;", -1, &m_commit, nullptr);
  check(result, "Error preparing commit transaction statement: ");
  result = sqlite3_prepare_v2(m_db, "ROLLBACK;", -1, &m_rollback, nullptr);
  check(result, "Error preparing rollback transaction statement: ");
  result = sqlite3_prepare_v2(m_db, "INSERT INTO recipes VALUES(NULL, ?001);", -1, &m_insert_recipe, nullptr);
  check(result, "Error preparing insert statement for recipes: ");
  result = sqlite3_prepare_v2(m_db, "INSERT OR IGNORE INTO categories VALUES(NULL, ?001);", -1, &m_add_category, nullptr);
  check(result, "Error preparing statement for adding category: ");
  result = sqlite3_prepare_v2(m_db, "INSERT INTO category SELECT ?001, id FROM categories WHERE categories.name = ?002;", -1,
                              &m_recipe_category, nullptr);
  check(result, "Error preparing statement for assigning recipe category: ");
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
  sqlite3_int64 recipe_id = sqlite3_last_insert_rowid(m_db);
  for (auto category=recipe.categories().begin(); category!=recipe.categories().end(); category++) {
    result = sqlite3_bind_text(m_add_category, 1, category->c_str(), -1, SQLITE_STATIC);
    check(result, "Error binding category name: ");
    result = sqlite3_step(m_add_category);
    check(result, "Error adding category: ");
    result = sqlite3_reset(m_add_category);
    check(result, "Error resetting category adding statement: ");
    result = sqlite3_bind_int64(m_recipe_category, 1, recipe_id);
    check(result, "Error binding recipe id: ");
    result = sqlite3_bind_text(m_recipe_category, 2, category->c_str(), -1, SQLITE_STATIC);
    check(result, "Error binding category name: ");
    result = sqlite3_step(m_recipe_category);
    check(result, "Error adding recipe category: ");
    result = sqlite3_reset(m_recipe_category);
    check(result, "Error resetting recipe category statement: ");
  };

}

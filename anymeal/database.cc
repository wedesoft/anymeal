#include <cassert>
#include <sstream>
#include "database.hh"


using namespace std;

Database::Database(void):
  m_db(nullptr), m_begin(nullptr), m_commit(nullptr), m_rollback(nullptr), m_insert_recipe(nullptr), m_add_category(nullptr),
  m_recipe_category(nullptr), m_add_ingredient(nullptr), m_recipe_ingredient(nullptr), m_get_header(nullptr),
  m_get_categories(nullptr), m_get_ingredients(nullptr)
{
}

Database::~Database(void) {
  sqlite3_finalize(m_begin);
  sqlite3_finalize(m_commit);
  sqlite3_finalize(m_rollback);
  sqlite3_finalize(m_insert_recipe);
  sqlite3_finalize(m_add_category);
  sqlite3_finalize(m_recipe_category);
  sqlite3_finalize(m_add_ingredient);
  sqlite3_finalize(m_recipe_ingredient);
  sqlite3_finalize(m_get_header);
  sqlite3_finalize(m_get_categories);
  sqlite3_finalize(m_get_ingredients);
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
  foreign_keys();
  int version = user_version();
  switch (version) {
    case 0:
      create();
      break;
    case 1:
      break;
    default:
      ostringstream s;
      s << "Database version " << version << " was created by more recent release of software.";
      throw database_exception(s.str());
  };
  result = sqlite3_prepare_v2(m_db, "BEGIN;", -1, &m_begin, nullptr);
  check(result, "Error preparing begin transaction statement: ");
  result = sqlite3_prepare_v2(m_db, "COMMIT;", -1, &m_commit, nullptr);
  check(result, "Error preparing commit transaction statement: ");
  result = sqlite3_prepare_v2(m_db, "ROLLBACK;", -1, &m_rollback, nullptr);
  check(result, "Error preparing rollback transaction statement: ");
  result = sqlite3_prepare_v2(m_db, "INSERT INTO recipes VALUES(NULL, ?001, ?002, ?003);", -1, &m_insert_recipe, nullptr);
  check(result, "Error preparing insert statement for recipes: ");
  result = sqlite3_prepare_v2(m_db, "INSERT OR IGNORE INTO categories VALUES(NULL, ?001);", -1, &m_add_category, nullptr);
  check(result, "Error preparing statement for adding category: ");
  result = sqlite3_prepare_v2(m_db, "INSERT OR IGNORE INTO category SELECT ?001, id FROM categories WHERE categories.name = ?002;",
                              -1, &m_recipe_category, nullptr);
  check(result, "Error preparing statement for assigning recipe category: ");
  result = sqlite3_prepare_v2(m_db, "INSERT OR IGNORE into ingredients VALUES(NULL, ?001);", -1, &m_add_ingredient, nullptr);
  check(result, "Error preparing statement for adding ingredient: ");
  result = sqlite3_prepare_v2(m_db, "INSERT INTO ingredient SELECT ?001, ?002, ?003, ?004, ?005, ?006, ?007, ingredients.id "
                                    "FROM ingredients WHERE ingredients.text = ?008;", -1, &m_recipe_ingredient, nullptr);
  check(result, "Error preparing statement for adding ingredient to recipe: ");
  result = sqlite3_prepare_v2(m_db, "SELECT title, servings, servingsunit FROM recipes WHERE id = ?001;", -1, &m_get_header,
                              nullptr);
  check(result, "Error preparing statement for fetching recipe header: ");
  result = sqlite3_prepare_v2(m_db, "SELECT name FROM categories, category WHERE recipeid = ?001 AND id = categoryid ORDER BY name;",
                              -1, &m_get_categories, nullptr);
  check(result, "Error preparing statement for fetching recipe categories: ");
  result = sqlite3_prepare_v2(m_db, "SELECT amountint, amountnum, amountdenom, amountfloat, unit, text "
                              "FROM ingredient, ingredients WHERE recipeid = ?001 ORDER BY line;", -1, &m_get_ingredients, nullptr);
  check(result, "Error preparing statement for fetching recipe ingredients: ");
}

int Database::user_version(void) {
  int result;
  sqlite3_stmt *query;
  result = sqlite3_prepare_v2(m_db, "PRAGMA user_version;", -1, &query, nullptr);
  check(result, "Error preparing user_version query: ");
  result = sqlite3_step(query);
  check(result, "Error querying user_version: ");
  int value = sqlite3_column_int(query, 0);
  sqlite3_finalize(query);
  return value;
}

void Database::foreign_keys(void) {
  int result = sqlite3_exec(m_db, "PRAGMA foreign_keys = ON;", nullptr, nullptr, nullptr);
  check(result, "Error enabling checks for foreign keys: ");
}

void Database::create(void) {
  int result = sqlite3_exec(m_db,
    "PRAGMA user_version = 1;\n"
    "BEGIN;\n"
    "CREATE TABLE recipes(id INTEGER PRIMARY KEY, title VARCHAR(60) NOT NULL, servings INTEGER NOT NULL, "
    "servingsunit VARCHAR(40) NOT NULL);\n"
    "CREATE TABLE categories(id INTEGER PRIMARY KEY, name VARCHAR(40) UNIQUE NOT NULL);\n"
    "CREATE TABLE category(recipeid INTEGER NOT NULL, categoryid INTEGER NOT NULL, PRIMARY KEY(recipeid, categoryid), "
    "FOREIGN KEY(recipeid) REFERENCES recipes(id), FOREIGN KEY(categoryid) REFERENCES categories(id));\n"
    "CREATE TABLE ingredients(id INTEGER PRIMARY KEY, text VARCHAR(60) UNIQUE NOT NULL);\n"
    "CREATE TABLE ingredient(recipeid INTEGER NOT NULL, line INTEGER NOT NULL, amountint INTEGER NOT NULL, "
    "amountnum INTEGER NOT NULL, amountdenom INTEGER NOT NULL, amountfloat REAL NOT NULL, unit CHARACTER(2) NOT NULL, "
    "ingredientid INTEGER NOT NULL, PRIMARY KEY(recipeid, line), FOREIGN KEY(recipeid) REFERENCES recipes(id), "
    "FOREIGN KEY(ingredientid) REFERENCES ingredients(id));\n"
    "COMMIT;\n",
    nullptr, nullptr, nullptr);
  check(result, "Error creating database tables: ");
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
  int c;
  assert(m_insert_recipe);
  int result;
  // Add recipe header.
  result = sqlite3_bind_text(m_insert_recipe, 1, recipe.title().c_str(), -1, SQLITE_STATIC);
  check(result, "Error binding recipe title: ");
  result = sqlite3_bind_int(m_insert_recipe, 2, recipe.servings());
  check(result, "Error binding recipe servings: ");
  result = sqlite3_bind_text(m_insert_recipe, 3, recipe.servings_unit().c_str(), -1, SQLITE_STATIC);
  check(result, "Error binding recipe servings unit: ");
  result = sqlite3_step(m_insert_recipe);
  check(result, "Error executing insert statement: ");
  result = sqlite3_reset(m_insert_recipe);
  check(result, "Error resetting insert statement: ");
  // Get recipe id.
  sqlite3_int64 recipe_id = sqlite3_last_insert_rowid(m_db);
  // Add categories.
  for (auto category=recipe.categories().begin(); category!=recipe.categories().end(); category++) {
    // Create category.
    result = sqlite3_bind_text(m_add_category, 1, category->c_str(), -1, SQLITE_STATIC);
    check(result, "Error binding category name: ");
    result = sqlite3_step(m_add_category);
    check(result, "Error adding category: ");
    result = sqlite3_reset(m_add_category);
    check(result, "Error resetting category adding statement: ");
    // Add recipe to category.
    result = sqlite3_bind_int64(m_recipe_category, 1, recipe_id);
    check(result, "Error binding recipe id: ");
    result = sqlite3_bind_text(m_recipe_category, 2, category->c_str(), -1, SQLITE_STATIC);
    check(result, "Error binding category name: ");
    result = sqlite3_step(m_recipe_category);
    check(result, "Error adding recipe category: ");
    result = sqlite3_reset(m_recipe_category);
    check(result, "Error resetting recipe category statement: ");
  };
  // Add ingredients.
  c = 1;
  for (auto ingredient=recipe.ingredients().begin(); ingredient!=recipe.ingredients().end(); ingredient++) {
    // Create ingredient.
    result = sqlite3_bind_text(m_add_ingredient, 1, ingredient->text().c_str(), -1, SQLITE_STATIC);
    check(result, "Error binding ingredient: ");
    result = sqlite3_step(m_add_ingredient);
    check(result, "Error adding ingredient: ");
    result = sqlite3_reset(m_add_ingredient);
    check(result, "Error resetting ingredient adding statement: ");
    // Add ingredient to recipe.
    result = sqlite3_bind_int64(m_recipe_ingredient, 1, recipe_id);
    check(result, "Error binding recipe id: ");
    result = sqlite3_bind_int(m_recipe_ingredient, 2, c++);
    check(result, "Error binding ingredient index: ");
    result = sqlite3_bind_int(m_recipe_ingredient, 3, ingredient->amount_integer());
    check(result, "Error binding ingredient integer amount: ");
    result = sqlite3_bind_int(m_recipe_ingredient, 4, ingredient->amount_numerator());
    check(result, "Error binding ingredient amount numerator: ");
    result = sqlite3_bind_int(m_recipe_ingredient, 5, ingredient->amount_denominator());
    check(result, "Error binding ingredient amount denominator: ");
    result = sqlite3_bind_double(m_recipe_ingredient, 6, ingredient->amount_float());
    check(result, "Error binding ingredient floating-point amount: ");
    result = sqlite3_bind_text(m_recipe_ingredient, 7, ingredient->unit().c_str(), -1, SQLITE_STATIC);
    check(result, "Error binding ingredient unit: ");
    result = sqlite3_bind_text(m_recipe_ingredient, 8, ingredient->text().c_str(), -1, SQLITE_STATIC);
    check(result, "Error binding ingredient search text: ");
    result = sqlite3_step(m_recipe_ingredient);
    check(result, "Error adding ingredient to recipe: ");
    result = sqlite3_reset(m_recipe_ingredient);
    check(result, "Error resetting statement adding ingredient to recipe: ");
  };
}

Recipe Database::fetch_recipe(sqlite3_int64 id) {
  int result;
  Recipe recipe;
  // Retrieve recipe header.
  result = sqlite3_bind_int64(m_get_header, 1, id);
  check(result, "Error binding recipe id: ");
  result = sqlite3_step(m_get_header);
  check(result, "Error retrieving recipe header: ");
  if (result != SQLITE_ROW) {
    ostringstream s;
    s << "Could not find recipe with id " << id << ".";
    throw database_exception(s.str());
  };
  recipe.set_title((const char *)sqlite3_column_text(m_get_header, 0));
  recipe.set_servings(sqlite3_column_int(m_get_header, 1));
  recipe.set_servings_unit((const char *)sqlite3_column_text(m_get_header, 2));
  result = sqlite3_reset(m_get_header);
  check(result, "Error resetting recipe header query: ");
  // Retrieve recipe categories.
  result = sqlite3_bind_int64(m_get_categories, 1, id);
  check(result, "Error binding recipe id for categories: ");
  while (true) {
    result = sqlite3_step(m_get_categories);
    check(result, "Error retrieving recipe category: ");
    if (result != SQLITE_ROW)
      break;
    recipe.add_category((const char *)sqlite3_column_text(m_get_categories, 0));
  };
  result = sqlite3_reset(m_get_categories);
  check(result, "Error resetting recipe categories query: ");
  // Retrieve recipe ingredients.
  result = sqlite3_bind_int64(m_get_ingredients, 1, id);
  while (true) {
    result = sqlite3_step(m_get_ingredients);
    check(result, "Error retrieving recipe ingredient: ");
    if (result != SQLITE_ROW)
      break;
    Ingredient ingredient;
    ingredient.set_amount_integer(sqlite3_column_int(m_get_ingredients, 0));
    ingredient.set_amount_numerator(sqlite3_column_int(m_get_ingredients, 1));
    ingredient.set_amount_denominator(sqlite3_column_int(m_get_ingredients, 2));
    ingredient.set_amount_float(sqlite3_column_double(m_get_ingredients, 3));
    ingredient.set_unit((const char *)sqlite3_column_text(m_get_ingredients, 4));
    ingredient.add_text((const char *)sqlite3_column_text(m_get_ingredients, 5));
    recipe.add_ingredient(ingredient);
  };
  return recipe;
}

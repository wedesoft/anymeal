#include <cassert>
#include <sstream>
#include "database.hh"


using namespace std;

Database::Database(void):
  m_db(nullptr), m_begin(nullptr), m_commit(nullptr), m_rollback(nullptr), m_insert_recipe(nullptr),
  m_add_category(nullptr), m_recipe_category(nullptr), m_add_ingredient(nullptr), m_recipe_ingredient(nullptr),
  m_get_header(nullptr), m_get_categories(nullptr), m_get_ingredients(nullptr), m_add_instruction(nullptr),
  m_get_instructions(nullptr), m_add_ingredient_section(nullptr), m_get_ingredient_section(nullptr),
  m_add_instruction_section(nullptr), m_get_instruction_section(nullptr), m_count_selected(nullptr), m_get_info(nullptr),
  m_select_title(nullptr), m_category_list(nullptr)
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
  sqlite3_finalize(m_add_instruction);
  sqlite3_finalize(m_get_instructions);
  sqlite3_finalize(m_add_ingredient_section);
  sqlite3_finalize(m_get_ingredient_section);
  sqlite3_finalize(m_add_instruction_section);
  sqlite3_finalize(m_get_instruction_section);
  sqlite3_finalize(m_count_selected);
  sqlite3_finalize(m_select_title);
  sqlite3_finalize(m_category_list);
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
  migrate();
  select_all();
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
                                    "FROM ingredients WHERE ingredients.name = ?008;", -1, &m_recipe_ingredient, nullptr);
  check(result, "Error preparing statement for adding ingredient to recipe: ");
  result = sqlite3_prepare_v2(m_db, "SELECT title, servings, servingsunit FROM recipes WHERE id = ?001;", -1, &m_get_header,
                              nullptr);
  check(result, "Error preparing statement for fetching recipe header: ");
  result = sqlite3_prepare_v2(m_db, "SELECT name FROM categories, category WHERE recipeid = ?001 AND id = categoryid ORDER BY name;",
                              -1, &m_get_categories, nullptr);
  check(result, "Error preparing statement for fetching recipe categories: ");
  result = sqlite3_prepare_v2(m_db, "SELECT amountint, amountnum, amountdenom, amountfloat, unit, name "
                              "FROM ingredient, ingredients WHERE recipeid = ?001 ORDER BY line;", -1, &m_get_ingredients, nullptr);
  check(result, "Error preparing statement for fetching recipe ingredients: ");
  result = sqlite3_prepare_v2(m_db, "INSERT INTO instruction VALUES(?001, ?002, ?003);", -1, &m_add_instruction, nullptr);
  check(result, "Error preparing statement for adding instruction to recipe: ");
  result = sqlite3_prepare_v2(m_db, "SELECT txt FROM instruction WHERE recipeid = ?001 ORDER BY line;", -1, &m_get_instructions,
                              nullptr);
  check(result, "Error preparing statement for fetching recipe instructions: ");
  result = sqlite3_prepare_v2(m_db, "INSERT INTO ingredientsection VALUES(?001, ?002, ?003);", -1, &m_add_ingredient_section,
                              nullptr);
  check(result, "Error preparing statement for storing ingredient section: ");
  result = sqlite3_prepare_v2(m_db, "SELECT line, title FROM ingredientsection WHERE recipeid = ?001 ORDER BY line;", -1,
                              &m_get_ingredient_section, nullptr);
  check(result, "Error preparing statement for retrieving ingredient section: ");
  result = sqlite3_prepare_v2(m_db, "INSERT INTO instructionsection VALUES(?001, ?002, ?003);", -1, &m_add_instruction_section,
                              nullptr);
  check(result, "Error preparing statement for storing instruction section: ");
  result = sqlite3_prepare_v2(m_db, "SELECT line, title FROM instructionsection WHERE recipeid = ?001 ORDER BY line;", -1,
                              &m_get_instruction_section, nullptr);
  check(result, "Error preparing statement for retrieving instruction section: ");
  result = sqlite3_prepare_v2(m_db, "SELECT COUNT(id) FROM selection;", -1, &m_count_selected, nullptr);
  check(result, "Error preparing statement for counting recipes: ");
  result = sqlite3_prepare_v2(m_db, "SELECT selection.id, title from selection, recipes WHERE recipes.id = selection.id "
                              "ORDER BY title COLLATE NOCASE;", -1, &m_get_info, nullptr);
  check(result, "Error preparing statement for retrieving recipe info: ");
  result = sqlite3_prepare_v2(m_db, "DELETE FROM selection WHERE id NOT IN (SELECT id FROM recipes WHERE title "
                              "LIKE '%' || ?001 || '%');", -1, &m_select_title, nullptr);
  check(result, "Error preparing statement for deleting from selection: ");
  result = sqlite3_prepare_v2(m_db, "SELECT name FROM categories, category, selection WHERE categories.id = categoryid AND "
                              "selection.id = recipeid GROUP BY categories.id ORDER BY COUNT(recipeid) DESC;", -1,
                              &m_category_list, nullptr);
  check(result, "Error preparing statement for listing categories: ");
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
    "CREATE TABLE ingredients(id INTEGER PRIMARY KEY, name VARCHAR(60) UNIQUE NOT NULL);\n"
    "CREATE TABLE ingredient(recipeid INTEGER NOT NULL, line INTEGER NOT NULL, amountint INTEGER NOT NULL, "
    "amountnum INTEGER NOT NULL, amountdenom INTEGER NOT NULL, amountfloat REAL NOT NULL, unit CHARACTER(2) NOT NULL, "
    "ingredientid INTEGER NOT NULL, PRIMARY KEY(recipeid, line), FOREIGN KEY(recipeid) REFERENCES recipes(id), "
    "FOREIGN KEY(ingredientid) REFERENCES ingredients(id));\n"
    "CREATE TABLE instruction(recipeid INTEGER NOT NULL, line INTEGER NOT NULL, txt TEXT NOT NULL, "
    "PRIMARY KEY(recipeid, line), FOREIGN KEY(recipeid) REFERENCES recipes(id));\n"
    "CREATE TABLE ingredientsection(recipeid INTEGER NOT NULL, line INTEGER NOT NULL, title VARCHAR(60) NOT NULL, "
    "PRIMARY KEY (recipeid, line), FOREIGN KEY(recipeid) REFERENCES recipes(id));\n"
    "CREATE TABLE instructionsection(recipeid INTEGER NOT NULL, line INTEGER NOT NULL, title VARCHAR(60) NOT NULL, "
    "PRIMARY KEY (recipeid, line), FOREIGN KEY(recipeid) REFERENCES recipes(id));\n"
    "COMMIT;\n",
    nullptr, nullptr, nullptr);
  check(result, "Error creating database tables: ");
}

void Database::migrate(void) {
  int version = user_version();
  if (version <= 0)
    create();
  if (version > 1) {
    ostringstream s;
    s << "Database version " << version << " was created by more recent release of software.";
    throw database_exception(s.str());
  };
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
  // Add ingredient sections.
  for (auto section=recipe.ingredient_sections().begin(); section!=recipe.ingredient_sections().end(); section++) {
    result = sqlite3_bind_int64(m_add_ingredient_section, 1, recipe_id);
    check(result, "Error binding recipe id: ");
    result = sqlite3_bind_int(m_add_ingredient_section, 2, section->first + 1);
    check(result, "Error binding ingredient section line: ");
    result = sqlite3_bind_text(m_add_ingredient_section, 3, section->second.c_str(), -1, SQLITE_STATIC);
    check(result, "Error binding ingredient section title: ");
    result = sqlite3_step(m_add_ingredient_section);
    check(result, "Error adding ingredient section to recipe: ");
    result = sqlite3_reset(m_add_ingredient_section);
    check(result, "Error resetting ingredient section statement: ");
  };
  // Add instructions.
  c = 1;
  for (auto instruction=recipe.instructions().begin(); instruction!=recipe.instructions().end(); instruction++) {
    result = sqlite3_bind_int64(m_add_instruction, 1, recipe_id);
    check(result, "Error binding recipe id: ");
    result = sqlite3_bind_int(m_add_instruction, 2, c++);
    check(result, "Error binding instruction index: ");
    result = sqlite3_bind_text(m_add_instruction, 3, instruction->c_str(), -1, SQLITE_STATIC);
    check(result, "Error binding instruction: ");
    result = sqlite3_step(m_add_instruction);
    check(result, "Error adding instruction to recipe: ");
    result = sqlite3_reset(m_add_instruction);
    check(result, "Error resetting statement for adding instructions to recipe: ");
  };
  // Add instruction sections.
  for (auto section=recipe.instruction_sections().begin(); section!=recipe.instruction_sections().end(); section++) {
    result = sqlite3_bind_int64(m_add_instruction_section, 1, recipe_id);
    check(result, "Error binding recipe id: ");
    result = sqlite3_bind_int(m_add_instruction_section, 2, section->first + 1);
    check(result, "Error binding instruction section line: ");
    result = sqlite3_bind_text(m_add_instruction_section, 3, section->second.c_str(), -1, SQLITE_STATIC);
    check(result, "Error binding instruction section title: ");
    result = sqlite3_step(m_add_instruction_section);
    check(result, "Error adding instruction section to recipe: ");
    result = sqlite3_reset(m_add_instruction_section);
    check(result, "Error resetting instruction section statement: ");
  };
}

int Database::num_recipes(void) {
  int result = sqlite3_step(m_count_selected);
  check(result, "Error counting recipes: ");
  int count = sqlite3_column_int(m_count_selected, 0);
  result = sqlite3_reset(m_count_selected);
  check(result, "Error resetting statement for counting recipes: ");
  return count;
}

vector<pair<sqlite3_int64, string>> Database::recipe_info(void) {
  int result;
  vector<pair<sqlite3_int64, string>> infos;
  while (true) {
    result = sqlite3_step(m_get_info);
    check(result, "Error getting recipe information: ");
    if (result != SQLITE_ROW)
      break;
    pair<sqlite3_int64, string> info(sqlite3_column_int64(m_get_info, 0), (const char *)sqlite3_column_text(m_get_info, 1));
    infos.push_back(info);
  };
  result = sqlite3_reset(m_get_info);
  check(result, "Error resetting statement for getting recipe info: ");
  return infos;
}

vector<string> Database::categories(void) {
  int result;
  vector<string> categories;
  while (true) {
    result = sqlite3_step(m_category_list);
    check(result, "Error getting categories: ");
    if (result != SQLITE_ROW)
      break;
    categories.push_back((const char *)sqlite3_column_text(m_category_list, 0));
  };
  result = sqlite3_reset(m_category_list);
  check(result, "Error resetting statement for getting categories: ");
  return categories;
}

void Database::select_all(void) {
  int result;
  result = sqlite3_exec(m_db, "DROP TABLE IF EXISTS selection;", nullptr, nullptr, nullptr);
  check(result, "Error dropping selection table: ");
  result = sqlite3_exec(m_db, "CREATE TEMPORARY TABLE selection(id INTEGER PRIMARY KEY);", nullptr, nullptr, nullptr);
  check(result, "Error creating selection table: ");
  result = sqlite3_exec(m_db, "INSERT INTO selection SELECT id FROM recipes;", nullptr, nullptr, nullptr);
  check(result, "Error selecting recipes: ");
}

void Database::select_by_title(const char *title) {
  int result;
  result = sqlite3_bind_text(m_select_title, 1, title, -1, SQLITE_STATIC);
  check(result, "Error binding title string: ");
  result = sqlite3_step(m_select_title);
  check(result, "Error filtering recipes by title: ");
  result = sqlite3_reset(m_select_title);
  check(result, "Error resetting statement for filtering recipes by title: ");
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
  check(result, "Error binding recipe id for ingredients: ");
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
  result = sqlite3_reset(m_get_ingredients);
  check(result, "Error resetting recipe ingredients query: ");
  // Retrieve ingredient sections.
  result = sqlite3_bind_int64(m_get_ingredient_section, 1, id);
  check(result, "Error binding recipe id for ingredient sections: ");
  while (true) {
    result = sqlite3_step(m_get_ingredient_section);
    check(result, "Error retrieving ingredient section: ");
    if (result != SQLITE_ROW)
      break;
    recipe.add_ingredient_section(sqlite3_column_int(m_get_ingredient_section, 0) - 1,
                                  (const char *)sqlite3_column_text(m_get_ingredient_section, 1));
  };
  result = sqlite3_reset(m_get_ingredient_section);
  check(result, "Error resetting recipe ingredient section query: ");
  // Retrieve recipe instructions.
  result = sqlite3_bind_int64(m_get_instructions, 1, id);
  while (true) {
    result = sqlite3_step(m_get_instructions);
    check(result, "Error retrieving recipe instruction: ");
    if (result != SQLITE_ROW)
      break;
    recipe.add_instruction((const char *)sqlite3_column_text(m_get_instructions, 0));
  };
  result = sqlite3_reset(m_get_instructions);
  check(result, "Error resetting recipe instructions query: ");
  // Retrieve instruction sections.
  result = sqlite3_bind_int64(m_get_instruction_section, 1, id);
  check(result, "Error binding recipe id for instruction sections: ");
  while (true) {
    result = sqlite3_step(m_get_instruction_section);
    check(result, "Error retrieving instruction section: ");
    if (result != SQLITE_ROW)
      break;
    recipe.add_instruction_section(sqlite3_column_int(m_get_instruction_section, 0) - 1,
                                   (const char *)sqlite3_column_text(m_get_instruction_section, 1));
  };
  result = sqlite3_reset(m_get_instruction_section);
  check(result, "Error resetting recipe instruction section query: ");
  return recipe;
}

/* AnyMeal recipe management software
   Copyright (C) 2020, 2023 Jan Wedekind

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>. */
#include <cassert>
#include <sstream>
#include "database.hh"


using namespace std;

Database::Database(void):
  m_db(NULL), m_begin(NULL), m_commit(NULL), m_rollback(NULL), m_insert_recipe(NULL),
  m_add_category(NULL), m_recipe_category(NULL), m_add_ingredient(NULL), m_recipe_ingredient(NULL),
  m_get_header(NULL), m_get_categories(NULL), m_category_and_count_list(NULL), m_get_ingredients(NULL),
  m_add_instruction(NULL), m_get_instructions(NULL), m_add_ingredient_section(NULL), m_get_ingredient_section(NULL),
  m_add_instruction_section(NULL), m_get_instruction_section(NULL), m_count_selected(NULL), m_get_info(NULL),
  m_select_title(NULL), m_category_list(NULL), m_select_category(NULL), m_select_no_category(NULL),
  m_select_ingredient(NULL), m_select_no_ingredient(NULL), m_delete_recipe(NULL), m_delete_categories(NULL),
  m_delete_ingredients(NULL), m_delete_instructions(NULL), m_delete_ingredient_sections(NULL),
  m_delete_instruction_sections(NULL), m_delete_selection(NULL), m_clean_categories(NULL), m_clean_ingredients(NULL),
  m_select_recipe(NULL), m_remove_recipe_category(NULL), m_rename_category(NULL), m_get_category_id(NULL),
  m_merge_category(NULL), m_delete_category(NULL), m_delete_recipe_category(NULL), m_count_recipes_in_category(NULL)
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
  sqlite3_finalize(m_category_and_count_list);
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
  sqlite3_finalize(m_select_category);
  sqlite3_finalize(m_select_no_category);
  sqlite3_finalize(m_select_ingredient);
  sqlite3_finalize(m_select_no_ingredient);
  sqlite3_finalize(m_delete_recipe);
  sqlite3_finalize(m_delete_categories);
  sqlite3_finalize(m_delete_ingredients);
  sqlite3_finalize(m_delete_instructions);
  sqlite3_finalize(m_delete_ingredient_sections);
  sqlite3_finalize(m_delete_instruction_sections);
  sqlite3_finalize(m_delete_selection);
  sqlite3_finalize(m_rename_category);
  sqlite3_finalize(m_clean_categories);
  sqlite3_finalize(m_clean_ingredients);
  sqlite3_finalize(m_select_recipe);
  sqlite3_finalize(m_remove_recipe_category);
  sqlite3_finalize(m_get_category_id);
  sqlite3_finalize(m_merge_category);
  sqlite3_finalize(m_delete_category);
  sqlite3_finalize(m_delete_recipe_category);
  sqlite3_finalize(m_count_recipes_in_category);
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
  pragmas();
  migrate();
  select_all();
  result = sqlite3_prepare_v2(m_db, "BEGIN;", -1, &m_begin, NULL);
  check(result, "Error preparing begin transaction statement: ");
  result = sqlite3_prepare_v2(m_db, "COMMIT;", -1, &m_commit, NULL);
  check(result, "Error preparing commit transaction statement: ");
  result = sqlite3_prepare_v2(m_db, "ROLLBACK;", -1, &m_rollback, NULL);
  check(result, "Error preparing rollback transaction statement: ");
  result = sqlite3_prepare_v2(m_db, "INSERT INTO recipes VALUES(NULL, ?001, ?002, ?003);", -1, &m_insert_recipe, NULL);
  check(result, "Error preparing insert statement for recipes: ");
  result = sqlite3_prepare_v2(m_db, "INSERT OR IGNORE INTO categories VALUES(NULL, ?001);", -1, &m_add_category, NULL);
  check(result, "Error preparing statement for adding category: ");
  result = sqlite3_prepare_v2(m_db, "INSERT OR IGNORE INTO category SELECT ?001, id FROM categories WHERE categories.name = ?002;",
                              -1, &m_recipe_category, NULL);
  check(result, "Error preparing statement for assigning recipe category: ");
  result = sqlite3_prepare_v2(m_db, "INSERT OR IGNORE into ingredients VALUES(NULL, ?001);", -1, &m_add_ingredient, NULL);
  check(result, "Error preparing statement for adding ingredient: ");
  result = sqlite3_prepare_v2(m_db, "INSERT INTO ingredient SELECT ?001, ?002, ?003, ?004, ?005, ?006, ?007, ingredients.id "
                                    "FROM ingredients WHERE ingredients.name = ?008;", -1, &m_recipe_ingredient, NULL);
  check(result, "Error preparing statement for adding ingredient to recipe: ");
  result = sqlite3_prepare_v2(m_db, "SELECT title, servings, servingsunit FROM recipes WHERE id = ?001;", -1, &m_get_header,
                              NULL);
  check(result, "Error preparing statement for fetching recipe header: ");
  result = sqlite3_prepare_v2(m_db, "SELECT name FROM categories, category WHERE recipeid = ?001 AND id = categoryid ORDER BY name;",
                              -1, &m_get_categories, NULL);
  check(result, "Error preparing statement for fetching recipe categories: ");
  result = sqlite3_prepare_v2(m_db, "SELECT name, COUNT(recipeid) FROM categories LEFT JOIN category ON id = categoryid "
                              "GROUP BY name ORDER BY name;",
                              -1, &m_category_and_count_list, NULL);
  check(result, "Error preparing statement for fetching recipe categories and recipe counts: ");
  result = sqlite3_prepare_v2(m_db, "SELECT amountint, amountnum, amountdenom, amountfloat, unit, name "
                              "FROM ingredient, ingredients WHERE recipeid = ?001 AND ingredientid = ingredients.id ORDER BY line;",
                              -1, &m_get_ingredients, NULL);
  check(result, "Error preparing statement for fetching recipe ingredients: ");
  result = sqlite3_prepare_v2(m_db, "INSERT INTO instruction VALUES(?001, ?002, ?003);", -1, &m_add_instruction, NULL);
  check(result, "Error preparing statement for adding instruction to recipe: ");
  result = sqlite3_prepare_v2(m_db, "SELECT txt FROM instruction WHERE recipeid = ?001 ORDER BY line;", -1, &m_get_instructions,
                              NULL);
  check(result, "Error preparing statement for fetching recipe instructions: ");
  result = sqlite3_prepare_v2(m_db, "INSERT INTO ingredientsection VALUES(?001, ?002, ?003);", -1, &m_add_ingredient_section,
                              NULL);
  check(result, "Error preparing statement for storing ingredient section: ");
  result = sqlite3_prepare_v2(m_db, "SELECT line, title FROM ingredientsection WHERE recipeid = ?001 ORDER BY line;", -1,
                              &m_get_ingredient_section, NULL);
  check(result, "Error preparing statement for retrieving ingredient section: ");
  result = sqlite3_prepare_v2(m_db, "INSERT INTO instructionsection VALUES(?001, ?002, ?003);", -1, &m_add_instruction_section,
                              NULL);
  check(result, "Error preparing statement for storing instruction section: ");
  result = sqlite3_prepare_v2(m_db, "SELECT line, title FROM instructionsection WHERE recipeid = ?001 ORDER BY line;", -1,
                              &m_get_instruction_section, NULL);
  check(result, "Error preparing statement for retrieving instruction section: ");
  result = sqlite3_prepare_v2(m_db, "SELECT COUNT(id) FROM selection;", -1, &m_count_selected, NULL);
  check(result, "Error preparing statement for counting recipes: ");
  result = sqlite3_prepare_v2(m_db, "SELECT selection.id, title from selection, recipes WHERE recipes.id = selection.id "
                              "ORDER BY title COLLATE NOCASE;", -1, &m_get_info, NULL);
  check(result, "Error preparing statement for retrieving recipe info: ");
  result = sqlite3_prepare_v2(m_db, "DELETE FROM selection WHERE id NOT IN (SELECT selection.id FROM selection, recipes WHERE "
                              "selection.id = recipes.id AND title LIKE '%' || ?001 || '%');", -1, &m_select_title, NULL);
  check(result, "Error preparing statement for selecting by title: ");
  result = sqlite3_prepare_v2(m_db, "SELECT name FROM categories, category, selection WHERE categories.id = categoryid AND "
                              "selection.id = recipeid GROUP BY categories.id ORDER BY COUNT(recipeid) DESC, name ASC;", -1,
                              &m_category_list, NULL);
  check(result, "Error preparing statement for listing categories: ");
  result = sqlite3_prepare_v2(m_db, "DELETE FROM selection WHERE id NOT IN (SELECT selection.id FROM selection, category, categories "
                              "WHERE selection.id = recipeid AND categoryid = categories.id AND name LIKE ?001 || '%');", -1,
                              &m_select_category, NULL);
  check(result, "Error preparing statement for selecting by category: ");
  result = sqlite3_prepare_v2(m_db, "DELETE FROM selection WHERE id IN (SELECT selection.id FROM selection, category, categories "
                              "WHERE selection.id = recipeid AND categoryid = categories.id AND name LIKE ?001 || '%');", -1,
                              &m_select_no_category, NULL);
  check(result, "Error preparing statement for excluding category: ");
  result = sqlite3_prepare_v2(m_db, "DELETE FROM selection WHERE id NOT IN (SELECT selection.id FROM selection, ingredient, "
                              "ingredients WHERE selection.id = recipeid AND ingredientid = ingredients.id AND "
                              "name LIKE '%' || ?001 || '%');", -1, &m_select_ingredient, NULL);
  check(result, "Error preparing statement for selecting by ingredient: ");
  result = sqlite3_prepare_v2(m_db, "DELETE FROM selection WHERE id IN (SELECT selection.id FROM selection, ingredient, "
                              "ingredients WHERE selection.id = recipeid AND ingredientid = ingredients.id AND "
                              "name LIKE '%' || ?001 || '%');", -1, &m_select_no_ingredient, NULL);
  check(result, "Error preparing statement for selecting by not having ingredient: ");
  result = sqlite3_prepare_v2(m_db, "DELETE FROM recipes WHERE id = ?001;", -1, &m_delete_recipe, NULL);
  check(result, "Error preparing statement for deleting recipe: ");
  result = sqlite3_prepare_v2(m_db, "DELETE FROM category WHERE recipeid = ?001;", -1, &m_delete_categories, NULL);
  check(result, "Error preparing statement for deleting category: ");
  result = sqlite3_prepare_v2(m_db, "DELETE FROM ingredient WHERE recipeid = ?001;", -1, &m_delete_ingredients, NULL);
  check(result, "Error preparing statement for deleting ingredients: ");
  result = sqlite3_prepare_v2(m_db, "DELETE FROM instruction WHERE recipeid = ?001;", -1, &m_delete_instructions, NULL);
  check(result, "Error preparing statement for deleting instructions: ");
  result = sqlite3_prepare_v2(m_db, "DELETE FROM ingredientsection WHERE recipeid = ?001;", -1, &m_delete_ingredient_sections,
                              NULL);
  check(result, "Error preparing statement for deleting ingredient sections: ");
  result = sqlite3_prepare_v2(m_db, "DELETE FROM instructionsection WHERE recipeid = ?001;", -1, &m_delete_instruction_sections,
                              NULL);
  check(result, "Error preparing statement for deleting instruction sections: ");
  result = sqlite3_prepare_v2(m_db, "DELETE FROM selection WHERE id = ?001;", -1, &m_delete_selection, NULL);
  check(result, "Error preparing statement for deleting recipe from selection: ");
  result = sqlite3_prepare_v2(m_db, "UPDATE categories SET name = ?002 WHERE name = ?001;", -1, &m_rename_category, NULL);
  check(result, "Error preparing statement for renaming category: ");
  result = sqlite3_prepare_v2(m_db, "SELECT id FROM categories WHERE name = ?001;", -1, &m_get_category_id, NULL);
  check(result, "Error preparing statement for getting category id: ");
  result = sqlite3_prepare_v2(m_db, "UPDATE OR REPLACE category SET categoryid = ?002 WHERE categoryid = ?001;", -1, &m_merge_category, NULL);
  check(result, "Error preparing statement for merging categories: ");
  result = sqlite3_prepare_v2(m_db, "DELETE FROM categories WHERE id = ?001;", -1, &m_delete_category, NULL);
  check(result, "Error preparing statement for deleting category: ");
  result = sqlite3_prepare_v2(m_db, "DELETE FROM category WHERE categoryid = ?001;", -1, &m_delete_recipe_category, NULL);
  check(result, "Error preparing statement for deleting recipe category: ");
  result = sqlite3_prepare_v2(m_db, "DELETE FROM categories WHERE id NOT IN (SELECT categoryid FROM category);", -1,
                              &m_clean_categories, NULL);
  check(result, "Error preparing statement for cleaning categories: ");
  result = sqlite3_prepare_v2(m_db, "DELETE FROM ingredients WHERE id NOT IN (SELECT ingredientid FROM ingredient);", -1,
                              &m_clean_ingredients, NULL);
  check(result, "Error preparing statement for cleaning ingredients: ");
  result = sqlite3_prepare_v2(m_db, "INSERT INTO selection VALUES(?001);", -1, &m_select_recipe, NULL);
  check(result, "Error preparing statement for selecting recipe: ");
  result = sqlite3_prepare_v2(m_db, "DELETE FROM category WHERE recipeid = ?001 AND categoryid IN (SELECT id FROM categories "
                              "WHERE name = ?002);", -1, &m_remove_recipe_category, NULL);
  check(result, "Error preparing statement for removing category from recipe: ");
  result = sqlite3_prepare_v2(m_db, "SELECT COUNT(recipeid) FROM category, categories WHERE categoryid = id AND name = ?001;",
                              -1, &m_count_recipes_in_category, NULL);
  check(result, "Error preparing statement for removing category from recipe: ");
}

int Database::user_version(void) {
  int result;
  sqlite3_stmt *query;
  result = sqlite3_prepare_v2(m_db, "PRAGMA user_version;", -1, &query, NULL);
  check(result, "Error preparing user_version query: ");
  result = sqlite3_step(query);
  check(result, "Error querying user_version: ");
  int value = sqlite3_column_int(query, 0);
  sqlite3_finalize(query);
  return value;
}

void Database::pragmas(void) {
  int result = sqlite3_exec(m_db, "PRAGMA foreign_keys = ON;", NULL, NULL, NULL);
  check(result, "Error enabling checks for foreign keys: ");
  result = sqlite3_exec(m_db, "PRAGMA cache_size = -256000;", NULL, NULL, NULL);
  check(result, "Error setting cache size: ");
}

void Database::create_version_1(void) {
  int result = sqlite3_exec(m_db,
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
    "COMMIT;\n"
    "PRAGMA user_version = 1;\n",
    NULL, NULL, NULL);
  check(result, "Error creating database tables: ");
}

void Database::migrate_version_1_to_version_2(void)
{
  int result = sqlite3_exec(m_db,
    "BEGIN;\n"
    "CREATE TABLE locale(language VARCHAR(2) NOT NULL);\n"
    "COMMIT;\n"
    "PRAGMA user_version = 2;\n",
    NULL, NULL, NULL);
  check(result, "Error migrating database to version 2: ");
}

void Database::migrate_version_2_to_version_3(void)
{
  int result = sqlite3_exec(m_db,
    "BEGIN;\n"
    "DROP TABLE locale;\n"
    "COMMIT;\n"
    "PRAGMA user_version = 3;\n",
    NULL, NULL, NULL);
  check(result, "Error migrating database to version 3: ");
}

void Database::migrate(void) {
  int version = user_version();
  if (version <= 0)
    create_version_1();
  if (version <= 1)
    migrate_version_1_to_version_2();
  if (version <= 2)
    migrate_version_2_to_version_3();
  if (version > 3) {
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

void Database::add_category(const char *name) {
  int result = sqlite3_bind_text(m_add_category, 1, name, -1, SQLITE_STATIC);
  check(result, "Error binding category name: ");
  result = sqlite3_step(m_add_category);
  check(result, "Error adding category: ");
  result = sqlite3_reset(m_add_category);
  check(result, "Error resetting category adding statement: ");
}

#include <iostream>

sqlite3_int64 Database::insert_recipe(Recipe &recipe) {
  int c;
  assert(m_insert_recipe);
  int result;
  // Add recipe header.
  string title = recipe.title();
  result = sqlite3_bind_text(m_insert_recipe, 1, recipe.title_c_str(), -1, SQLITE_STATIC);
  check(result, "Error binding recipe title: ");
  result = sqlite3_bind_int(m_insert_recipe, 2, recipe.servings());
  check(result, "Error binding recipe servings: ");
  string servings_unit = recipe.servings_unit();
  result = sqlite3_bind_text(m_insert_recipe, 3, recipe.servings_unit_c_str(), -1, SQLITE_STATIC);
  check(result, "Error binding recipe servings unit: ");
  result = sqlite3_step(m_insert_recipe);
  check(result, "Error executing insert statement: ");
  result = sqlite3_reset(m_insert_recipe);
  check(result, "Error resetting insert statement: ");
  // Get recipe id.
  sqlite3_int64 recipe_id = sqlite3_last_insert_rowid(m_db);
  // Add categories.
  for (set<string>::iterator category=recipe.categories().begin(); category!=recipe.categories().end(); category++) {
    // Create category.
    add_category(category->c_str());
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
  for (vector<Ingredient>::iterator ingredient=recipe.ingredients().begin(); ingredient!=recipe.ingredients().end(); ingredient++) {
    // Create ingredient.
    result = sqlite3_bind_text(m_add_ingredient, 1, ingredient->text_c_str(), -1, SQLITE_STATIC);
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
    string unit = ingredient->unit();
    result = sqlite3_bind_text(m_recipe_ingredient, 7, ingredient->unit_c_str(), -1, SQLITE_STATIC);
    check(result, "Error binding ingredient unit: ");
    string text = ingredient->text();
    result = sqlite3_bind_text(m_recipe_ingredient, 8, ingredient->text_c_str(), -1, SQLITE_STATIC);
    check(result, "Error binding ingredient search text: ");
    result = sqlite3_step(m_recipe_ingredient);
    check(result, "Error adding ingredient to recipe: ");
    result = sqlite3_reset(m_recipe_ingredient);
    check(result, "Error resetting statement adding ingredient to recipe: ");
  };
  // Add ingredient sections.
  for (vector<pair<int, string> >::iterator section=recipe.ingredient_sections().begin(); section!=recipe.ingredient_sections().end(); section++) {
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
  for (vector<string>::iterator instruction=recipe.instructions().begin(); instruction!=recipe.instructions().end(); instruction++) {
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
  for (vector<pair<int, string> >::iterator section=recipe.instruction_sections().begin(); section!=recipe.instruction_sections().end(); section++) {
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
  // Add to selection.
  result = sqlite3_bind_int64(m_select_recipe, 1, recipe_id);
  check(result, "Error binding id for selecting recipe: ");
  result = sqlite3_step(m_select_recipe);
  check(result, "Error selecting recipe: ");
  result = sqlite3_reset(m_select_recipe);
  check(result, "Error resetting statement for selecting recipe: ");
  return recipe_id;
}

int Database::num_recipes(void) {
  int result = sqlite3_step(m_count_selected);
  check(result, "Error counting recipes: ");
  int count = sqlite3_column_int(m_count_selected, 0);
  result = sqlite3_reset(m_count_selected);
  check(result, "Error resetting statement for counting recipes: ");
  return count;
}

int Database::count_recipes(const char *category) {
  int result = sqlite3_bind_text(m_count_recipes_in_category, 1, category, -1, SQLITE_STATIC);
  check(result, "Error binding category string: ");
  result = sqlite3_step(m_count_recipes_in_category);
  check(result, "Error counting recipes in category: ");
  int count = sqlite3_column_int(m_count_recipes_in_category, 0);
  result = sqlite3_reset(m_count_recipes_in_category);
  check(result, "Error resetting statement for counting recipes in category: ");
  return count;
}

vector<pair<sqlite3_int64, string> > Database::recipe_info(void) {
  int result;
  vector<pair<sqlite3_int64, string> > infos;
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

vector<pair<string, int> > Database::categories_and_counts(void) {
  int result;
  vector<pair<string, int> > categories_and_counts;
  while (true) {
    result = sqlite3_step(m_category_and_count_list);
    check(result, "Error getting categories and counts: ");
    if (result != SQLITE_ROW)
      break;
    categories_and_counts.push_back(make_pair((const char *)sqlite3_column_text(m_category_and_count_list, 0),
                                              sqlite3_column_int(m_category_and_count_list, 1)));
  };
  result = sqlite3_reset(m_category_and_count_list);
  check(result, "Error resetting statement for getting categories and counts: ");
  return categories_and_counts;
}

void Database::select_all(void) {
  int result;
  result = sqlite3_exec(m_db, "DROP TABLE IF EXISTS selection;", NULL, NULL, NULL);
  check(result, "Error dropping selection table: ");
  result = sqlite3_exec(m_db, "CREATE TEMPORARY TABLE selection(id INTEGER PRIMARY KEY);", NULL, NULL, NULL);
  check(result, "Error creating selection table: ");
  result = sqlite3_exec(m_db, "INSERT INTO selection SELECT id FROM recipes;", NULL, NULL, NULL);
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

void Database::select_by_category(const char *category) {
  int result;
  result = sqlite3_bind_text(m_select_category, 1, category, -1, SQLITE_STATIC);
  check(result, "Error binding category string: ");
  result = sqlite3_step(m_select_category);
  check(result, "Error filtering recipes by category: ");
  result = sqlite3_reset(m_select_category);
  check(result, "Error resetting statement for filtering recipes by category: ");
}

void Database::select_by_no_category(const char *category) {
  int result;
  result = sqlite3_bind_text(m_select_no_category, 1, category, -1, SQLITE_STATIC);
  check(result, "Error binding category string: ");
  result = sqlite3_step(m_select_no_category);
  check(result, "Error filtering recipes by not in category: ");
  result = sqlite3_reset(m_select_no_category);
  check(result, "Error resetting statement for filtering recipes by not in category: ");
}

void Database::select_by_ingredient(const char *ingredient) {
  int result;
  result = sqlite3_bind_text(m_select_ingredient, 1, ingredient, -1, SQLITE_STATIC);
  check(result, "Error binding ingredient string: ");
  result = sqlite3_step(m_select_ingredient);
  check(result, "Error filtering recipes by ingredient: ");
  result = sqlite3_reset(m_select_ingredient);
  check(result, "Error resetting statement for filtering recipes by ingredient: ");
}

void Database::select_by_no_ingredient(const char *ingredient) {
  int result;
  result = sqlite3_bind_text(m_select_no_ingredient, 1, ingredient, -1, SQLITE_STATIC);
  check(result, "Error binding ingredient string: ");
  result = sqlite3_step(m_select_no_ingredient);
  check(result, "Error filtering recipes by not having ingredient: ");
  result = sqlite3_reset(m_select_no_ingredient);
  check(result, "Error resetting statement for filtering recipes by not having ingredient: ");
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

vector<Recipe> Database::fetch_recipes(const vector<sqlite3_int64> &ids) {
  vector<Recipe> result;
  for (vector<sqlite3_int64>::const_iterator id=ids.begin(); id!=ids.end(); id++) {
    result.push_back(fetch_recipe(*id));
  };
  return result;
}

void Database::delete_recipes(const vector<sqlite3_int64> &ids) {
  int result;
  for (vector<sqlite3_int64>::const_iterator id=ids.begin(); id!=ids.end(); id++) {
    // Delete categories.
    result = sqlite3_bind_int64(m_delete_categories, 1, *id);
    check(result, "Error binding id for deleting categories: ");
    result = sqlite3_step(m_delete_categories);
    check(result, "Error deleting categories: ");
    result = sqlite3_reset(m_delete_categories);
    check(result, "Error resetting statement for deleting categories: ");
    // Delete ingredients.
    result = sqlite3_bind_int64(m_delete_ingredients, 1, *id);
    check(result, "Error binding id for deleting ingredients: ");
    result = sqlite3_step(m_delete_ingredients);
    check(result, "Error deleting ingredients: ");
    result = sqlite3_reset(m_delete_ingredients);
    check(result, "Error resetting statement for deleting ingredients: ");
    // Delete instructions.
    result = sqlite3_bind_int64(m_delete_instructions, 1, *id);
    check(result, "Error binding id for deleting instructions: ");
    result = sqlite3_step(m_delete_instructions);
    check(result, "Error deleting instructions: ");
    result = sqlite3_reset(m_delete_instructions);
    check(result, "Error resetting statement for deleting instructions: ");
    // Delete ingredient sections.
    result = sqlite3_bind_int64(m_delete_ingredient_sections, 1, *id);
    check(result, "Error binding id for deleting ingredient sections: ");
    result = sqlite3_step(m_delete_ingredient_sections);
    check(result, "Error deleting ingredient sections: ");
    result = sqlite3_reset(m_delete_ingredient_sections);
    check(result, "Error resetting statement for deleting ingredient sections: ");
    // Delete instruction sections.
    result = sqlite3_bind_int64(m_delete_instruction_sections, 1, *id);
    check(result, "Error binding id for deleting instruction sections: ");
    result = sqlite3_step(m_delete_instruction_sections);
    check(result, "Error deleting instruction sections: ");
    result = sqlite3_reset(m_delete_instruction_sections);
    check(result, "Error resetting statement for deleting instruction sections: ");
    // Delete selections.
    result = sqlite3_bind_int64(m_delete_selection, 1, *id);
    check(result, "Error binding id for deleting selection: ");
    result = sqlite3_step(m_delete_selection);
    check(result, "Error deleting selection: ");
    result = sqlite3_reset(m_delete_selection);
    check(result, "Error resetting statement for deleting selection: ");
    // Delete recipe.
    result = sqlite3_bind_int64(m_delete_recipe, 1, *id);
    check(result, "Error binding id for deleting recipe: ");
    result = sqlite3_step(m_delete_recipe);
    check(result, "Error deleting recipe: ");
    result = sqlite3_reset(m_delete_recipe);
    check(result, "Error resetting statement for deleting recipe: ");
  };
}

void Database::add_recipes_to_category(const vector<sqlite3_int64> &ids, const char *category) {
  // Create category.
  add_category(category);
  // Add recipes to category.
  for (vector<sqlite3_int64>::const_iterator id=ids.begin(); id!=ids.end(); id++) {
    int result = sqlite3_bind_int64(m_recipe_category, 1, *id);
    check(result, "Error binding recipe id: ");
    result = sqlite3_bind_text(m_recipe_category, 2, category, -1, SQLITE_STATIC);
    check(result, "Error binding category name: ");
    result = sqlite3_step(m_recipe_category);
    check(result, "Error adding recipe category: ");
    result = sqlite3_reset(m_recipe_category);
    check(result, "Error resetting recipe category statement: ");
  };
}

void Database::remove_recipes_from_category(const vector<sqlite3_int64> &ids, const char *category) {
  for (vector<sqlite3_int64>::const_iterator id=ids.begin(); id!=ids.end(); id++) {
    int result = sqlite3_bind_int64(m_remove_recipe_category, 1, *id);
    check(result, "Error binding recipe id: ");
    result = sqlite3_bind_text(m_remove_recipe_category, 2, category, -1, SQLITE_STATIC);
    check(result, "Error binding category name: ");
    result = sqlite3_step(m_remove_recipe_category);
    check(result, "Error adding recipe category: ");
    result = sqlite3_reset(m_remove_recipe_category);
    check(result, "Error resetting recipe category statement: ");
  };
}

void Database::rename_category(const char *current_name, const char *new_name) {
  int result = sqlite3_bind_text(m_rename_category, 1, current_name, -1, SQLITE_STATIC);
  check(result, "Error binding old category name: ");
  result = sqlite3_bind_text(m_rename_category, 2, new_name, -1, SQLITE_STATIC);
  check(result, "Error binding new category name: ");
  result = sqlite3_step(m_rename_category);
  check(result, "Error renaming recipe category: ");
  result = sqlite3_reset(m_rename_category);
  check(result, "Error resetting rename category statement: ");
}

sqlite3_int64 Database::get_category_id(const char *name)
{
  sqlite3_int64 category_id = 0;
  int result = sqlite3_bind_text(m_get_category_id, 1, name, -1, SQLITE_STATIC);
  check(result, "Error binding old category name: ");
  result = sqlite3_step(m_get_category_id);
  check(result, "Error getting category id: ");
  if (result == SQLITE_ROW)
    category_id = sqlite3_column_int64(m_get_category_id, 0);
  result = sqlite3_reset(m_get_category_id);
  check(result, "Error resetting statement for getting category id: ");
  return category_id;
}

void Database::merge_category(const char *category, const char *target) {
  sqlite3_int64 category_id = get_category_id(category);
  sqlite3_int64 target_id = get_category_id(target);
  int result = sqlite3_bind_int64(m_merge_category, 1, category_id);
  check(result, "Error binding category id for merging: ");
  result = sqlite3_bind_int64(m_merge_category, 2, target_id);
  check(result, "Error binding target category id for merging: ");
  result = sqlite3_step(m_merge_category);
  check(result, "Error merging category: ");
  result = sqlite3_reset(m_merge_category);
  check(result, "Error resetting statement for merging category: ");
  delete_category(category);
}

void Database::delete_category(const char *category) {
  sqlite3_int64 category_id = get_category_id(category);
  int result = sqlite3_bind_int64(m_delete_recipe_category, 1, category_id);
  check(result, "Error binding category id for deleting from recipe: ");
  result = sqlite3_step(m_delete_recipe_category);
  check(result, "Error deleting recipe category: ");
  result = sqlite3_reset(m_delete_recipe_category);
  check(result, "Error resetting statement for deleting recipe category: ");
  result = sqlite3_bind_int64(m_delete_category, 1, category_id);
  check(result, "Error binding category id for deleting: ");
  result = sqlite3_step(m_delete_category);
  check(result, "Error deleting category: ");
  result = sqlite3_reset(m_delete_category);
  check(result, "Error resetting statement for deleting category: ");
}

void Database::garbage_collect(void) {
  int result;
  // Clean up categories.
  result = sqlite3_step(m_clean_categories);
  check(result, "Error cleaning categories: ");
  result = sqlite3_reset(m_clean_categories);
  check(result, "Error resetting statement for cleaning categories: ");
  // Clean up ingredients.
  result = sqlite3_step(m_clean_ingredients);
  check(result, "Error cleaning ingredients: ");
  result = sqlite3_reset(m_clean_ingredients);
  check(result, "Error resetting statement for cleaning ingredients: ");
}

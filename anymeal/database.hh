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
#pragma once
#include <string>
#include <vector>
#include <sqlite3.h>
#include "recipe.hh"


class database_exception: public std::exception
{
public:
  database_exception(const std::string &error): m_error(error) {}
  virtual ~database_exception(void) throw() {}
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
  sqlite3_int64 insert_recipe(Recipe &recipe);
  int num_recipes(void);
  int count_recipes(const char *category);
  std::vector<std::pair<sqlite3_int64, std::string> > recipe_info(void);
  std::vector<std::string> categories(void);
  std::vector<std::pair<std::string, int> > categories_and_counts(void);
  void select_all(void);
  void select_by_title(const char *title);
  void select_by_category(const char *category);
  void select_by_no_category(const char *category);
  void select_by_ingredient(const char *ingredient);
  void select_by_no_ingredient(const char *ingredient);
  Recipe fetch_recipe(sqlite3_int64 id);
  std::vector<Recipe> fetch_recipes(const std::vector<sqlite3_int64> &ids);
  void delete_recipes(const std::vector<sqlite3_int64> &ids);
  void add_recipes_to_category(const std::vector<sqlite3_int64> &ids, const char *category);
  void remove_recipes_from_category(const std::vector<sqlite3_int64> &ids, const char *category);
  void rename_category(const char *current_name, const char *new_name);
  sqlite3_int64 get_category_id(const char *name);
  void merge_category(const char *category, const char *target);
  void delete_category(const char *category);
  void garbage_collect(void);
protected:
  void create(void);
  void migrate(void);
  void check(int result, const char *prefix);
  int user_version(void);
  void pragmas(void);
  sqlite3 *m_db;
  sqlite3_stmt *m_begin;
  sqlite3_stmt *m_commit;
  sqlite3_stmt *m_rollback;
  sqlite3_stmt *m_insert_recipe;
  sqlite3_stmt *m_add_category;
  sqlite3_stmt *m_recipe_category;
  sqlite3_stmt *m_add_ingredient;
  sqlite3_stmt *m_recipe_ingredient;
  sqlite3_stmt *m_get_header;
  sqlite3_stmt *m_get_categories;
  sqlite3_stmt *m_category_and_count_list;
  sqlite3_stmt *m_get_ingredients;
  sqlite3_stmt *m_add_instruction;
  sqlite3_stmt *m_get_instructions;
  sqlite3_stmt *m_add_ingredient_section;
  sqlite3_stmt *m_get_ingredient_section;
  sqlite3_stmt *m_add_instruction_section;
  sqlite3_stmt *m_get_instruction_section;
  sqlite3_stmt *m_count_selected;
  sqlite3_stmt *m_get_info;
  sqlite3_stmt *m_select_title;
  sqlite3_stmt *m_category_list;
  sqlite3_stmt *m_select_category;
  sqlite3_stmt *m_select_no_category;
  sqlite3_stmt *m_select_ingredient;
  sqlite3_stmt *m_select_no_ingredient;
  sqlite3_stmt *m_delete_recipe;
  sqlite3_stmt *m_delete_categories;
  sqlite3_stmt *m_delete_ingredients;
  sqlite3_stmt *m_delete_instructions;
  sqlite3_stmt *m_delete_ingredient_sections;
  sqlite3_stmt *m_delete_instruction_sections;
  sqlite3_stmt *m_delete_selection;
  sqlite3_stmt *m_clean_categories;
  sqlite3_stmt *m_clean_ingredients;
  sqlite3_stmt *m_select_recipe;
  sqlite3_stmt *m_remove_recipe_category;
  sqlite3_stmt *m_rename_category;
  sqlite3_stmt *m_get_category_id;
  sqlite3_stmt *m_merge_category;
  sqlite3_stmt *m_delete_category;
  sqlite3_stmt *m_delete_recipe_category;
  sqlite3_stmt *m_count_recipes_in_category;
};

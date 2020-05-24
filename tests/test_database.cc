#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "database.hh"


using namespace testing;

TEST(DatabaseTest, OpenDatabase) {
  Database database;
  database.open(":memory:");
}

int has_row(void *exist, int, char**, char**) {
  *(int *)exist += 1;
  return 0;
}

TEST(DatabaseTest, CreateRecipeTable) {
  Database database;
  database.open(":memory:");
  int exist = 0;
  sqlite3_exec(database.db(), "SELECT name FROM sqlite_master WHERE type='table' AND name='recipes';", &has_row, &exist, nullptr);
  EXPECT_EQ(exist, 1);
}

TEST(DatabaseTest, FailedToCreate) {
  Database database;
  EXPECT_THROW(database.open("/tmp/nosuchdir/anymeal.db"), database_exception);
}

TEST(DatabaseTest, AddRecipeTitle) {
  Database database;
  database.open(":memory:");
  Recipe recipe;
  recipe.set_title("apple pie");
  database.insert_recipe(recipe);
  int exist = 0;
  sqlite3_exec(database.db(), "SELECT title FROM recipes;", &has_row, &exist, nullptr);
  EXPECT_EQ(exist, 1);
}

TEST(DatabaseTest, BeginAndCommit) {
  Database database;
  database.open(":memory:");
  Recipe recipe;
  recipe.set_title("apple pie");
  database.begin();
  database.insert_recipe(recipe);
  database.commit();
  int exist = 0;
  sqlite3_exec(database.db(), "SELECT title FROM recipes;", &has_row, &exist, nullptr);
  EXPECT_EQ(exist, 1);
}

TEST(DatabaseTest, BeginAndRollback) {
  Database database;
  database.open(":memory:");
  Recipe recipe;
  recipe.set_title("apple pie");
  database.begin();
  database.insert_recipe(recipe);
  database.rollback();
  int exist = 0;
  sqlite3_exec(database.db(), "SELECT title FROM recipes;", &has_row, &exist, nullptr);
  EXPECT_EQ(exist, 0);
}

TEST(DatabaseTest, AddCategories) {
  Database database;
  database.open(":memory:");
  Recipe recipe;
  recipe.add_category("Dessert");
  recipe.add_category("Muffins");
  recipe.add_category("Muffins");
  database.insert_recipe(recipe);
  int exist = 0;
  sqlite3_exec(database.db(), "SELECT name FROM categories;", &has_row, &exist, nullptr);
  EXPECT_EQ(exist, 2);
  int recipe_category = 0;
  sqlite3_exec(database.db(), "SELECT categories.name FROM categories, category WHERE category.recipeid = 1 AND "
               "category.categoryid = categories.id", &has_row, &recipe_category, nullptr);
  EXPECT_EQ(recipe_category, 2);
}

TEST(DatabaseTest, NoRecipeFound) {
  Database database;
  database.open(":memory:");
  EXPECT_THROW(database.fetch_recipe(1), database_exception);
}

TEST(DatabaseTest, RecipeHeaderRoundtrip) {
  Database database;
  database.open(":memory:");
  Recipe recipe;
  recipe.set_title("apple pie");
  recipe.set_servings(123);
  recipe.set_servings_unit("servings");
  database.insert_recipe(recipe);
  Recipe result = database.fetch_recipe(1);
  EXPECT_EQ("apple pie", result.title());
  EXPECT_EQ(123, result.servings());
  EXPECT_EQ("servings", result.servings_unit());
}

TEST(DatabaseTest, CategoriesRoundtrip) {
  Database database;
  database.open(":memory:");
  Recipe recipe;
  recipe.add_category("cakes");
  recipe.add_category("sweet");
  database.insert_recipe(recipe);
  Recipe result = database.fetch_recipe(1);
  ASSERT_EQ(2, result.categories().size());
  EXPECT_EQ("cakes", result.categories()[0]);
  EXPECT_EQ("sweet", result.categories()[1]);
}

TEST(DatabaseTest, IngredientRoundtrip) {
  Database database;
  database.open(":memory:");
  Recipe recipe;
  Ingredient ingredient;
  ingredient.set_amount_integer(2);
  ingredient.set_amount_numerator(3);
  ingredient.set_amount_denominator(5);
  ingredient.set_amount_float(2.6);
  ingredient.set_unit("g ");
  ingredient.add_text("apples");
  recipe.add_ingredient(ingredient);
  database.insert_recipe(recipe);
  Recipe result_recipe = database.fetch_recipe(1);
  ASSERT_EQ(1, result_recipe.ingredients().size());
  Ingredient result = result_recipe.ingredients()[0];
  EXPECT_EQ(2, result.amount_integer());
  EXPECT_EQ(3, result.amount_numerator());
  EXPECT_EQ(5, result.amount_denominator());
  EXPECT_EQ(2.6, result.amount_float());
  EXPECT_EQ("g ", result.unit());
  EXPECT_EQ("apples", result.text());
}

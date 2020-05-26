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

TEST(DatabaseTest, InstructionsRoundtrip) {
  Database database;
  database.open(":memory:");
  Recipe recipe;
  recipe.add_instruction("Start cooking.");
  recipe.add_instruction("Finish cooking.");
  database.insert_recipe(recipe);
  Recipe result = database.fetch_recipe(1);
  ASSERT_EQ(2, result.instructions().size());
  EXPECT_EQ("Start cooking.", result.instructions()[0]);
  EXPECT_EQ("Finish cooking.", result.instructions()[1]);
}

TEST(DatabaseTest, IngredientSectionRoundtrip) {
  Database database;
  database.open(":memory:");
  Recipe recipe;
  recipe.add_ingredient_section(3, "paste");
  database.insert_recipe(recipe);
  Recipe result = database.fetch_recipe(1);
  ASSERT_EQ(1, result.ingredient_sections().size());
  EXPECT_EQ(3, result.ingredient_sections()[0].first);
  EXPECT_EQ("paste", result.ingredient_sections()[0].second);
}

TEST(DatabaseTest, InstructionSectionRoundtrip) {
  Database database;
  database.open(":memory:");
  Recipe recipe;
  recipe.add_instruction_section(3, "paste");
  database.insert_recipe(recipe);
  Recipe result = database.fetch_recipe(1);
  ASSERT_EQ(1, result.instruction_sections().size());
  EXPECT_EQ(3, result.instruction_sections()[0].first);
  EXPECT_EQ("paste", result.instruction_sections()[0].second);
}

TEST(DatabaseTest, GetRecipeCountAndTitles) {
  Database database;
  database.open(":memory:");
  Recipe recipe1;
  recipe1.set_title("Recipe B");
  database.insert_recipe(recipe1);
  Recipe recipe2;
  recipe2.set_title("Recipe A");
  database.insert_recipe(recipe2);
  database.select_all();
  ASSERT_EQ(2, database.num_recipes());
  auto info = database.recipe_info();
  EXPECT_EQ(2, info[0].first);
  EXPECT_EQ("Recipe A", info[0].second);
  EXPECT_EQ(1, info[1].first);
  EXPECT_EQ("Recipe B", info[1].second);
}

TEST(DatabaseTest, SelectByTitle) {
  Database database;
  database.open(":memory:");
  Recipe recipe1;
  recipe1.set_title("Recipe B");
  database.insert_recipe(recipe1);
  Recipe recipe2;
  recipe2.set_title("Recipe A");
  database.insert_recipe(recipe2);
  database.select_all();
  database.select_by_title("B");
  ASSERT_EQ(1, database.num_recipes());
  auto info = database.recipe_info();
  EXPECT_EQ(1, info[0].first);
  EXPECT_EQ("Recipe B", info[0].second);
}

#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "database.hh"


using namespace testing;

TEST(DatabaseTest, OpenDatabase) {
  char *tmp = mktemp(strdup("/tmp/anymealXXXXXX"));
  Database database;
  database.open(tmp);
  EXPECT_TRUE(access(tmp, F_OK) != -1);
  free(tmp);
}

int has_row(void *exist, int, char**, char**) {
  *(int *)exist = 1;
  return 0;
}

TEST(DatabaseTest, CreateRecipeTable) {
  char *tmp = mktemp(strdup("/tmp/anymealXXXXXX"));
  Database database;
  database.open(tmp);
  int exist = 0;
  sqlite3_exec(database.db(), "SELECT name FROM sqlite_master WHERE type='table' AND name='recipes';", &has_row, &exist, nullptr);
  EXPECT_EQ(exist, 1);
  free(tmp);
}

TEST(DatabaseTest, FailedToCreate) {
  Database database;
  EXPECT_THROW(database.open("/tmp/nosuchdir/anymeal.db"), database_exception);
}

TEST(DatabaseTest, AddRecipeTitle) {
  char *tmp = mktemp(strdup("/tmp/anymealXXXXXX"));
  Database database;
  database.open(tmp);
  Recipe recipe;
  recipe.set_title("apple pie");
  database.insert_recipe(recipe);
  int exist = 0;
  sqlite3_exec(database.db(), "SELECT title FROM recipes;", &has_row, &exist, nullptr);
  EXPECT_EQ(exist, 1);
  free(tmp);
}

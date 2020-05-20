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

int table_exist(void *exist, int, char**, char**) {
  *(int *)exist = 1;
  return 0;
}

TEST(DatabaseTest, CreateRecipeTable) {
  char *tmp = mktemp(strdup("/tmp/anymealXXXXXX"));
  Database database;
  database.open(tmp);
  char *error = nullptr;
  int exist = 0;
  sqlite3_exec(database.db(), "SELECT name FROM sqlite_master WHERE type='table' AND name='recipes';", &table_exist, &exist, &error);
  EXPECT_EQ(error, nullptr);
  EXPECT_EQ(exist, 1);
  sqlite3_free(error);
  free(tmp);
}

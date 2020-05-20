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

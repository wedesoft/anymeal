#include <sstream>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "partition.hpp"


using namespace testing;
using namespace std;


TEST(PartitionTest, EmptyFile) {
  istringstream s("");
  ASSERT_TRUE(recipes(s).empty());
}

TEST(PartitionTest, MinimalRecipe) {
  istringstream s("MMMMM---MEAL-MASTER Format---\r\nMMMMM");
  EXPECT_EQ(1, recipes(s).size());
}

TEST(PartitionTest, RecipeContent) {
  istringstream s("MMMMM---MEAL-MASTER Format---\r\nMMMMM");
  EXPECT_EQ("MMMMM---MEAL-MASTER Format---\r\nMMMMM\r\n", recipes(s)[0]);
}

TEST(PartitionTest, ConvertUnixLF) {
  istringstream s("MMMMM---MEAL-MASTER Format---\nMMMMM");
  EXPECT_EQ("MMMMM---MEAL-MASTER Format---\r\nMMMMM\r\n", recipes(s)[0]);
}

TEST(PartitionTest, IgnoreStrayText) {
  istringstream s("text");
  ASSERT_TRUE(recipes(s).empty());
}

TEST(PartitionTest, EndOfRecipe) {
  istringstream s("MMMMM---MEAL-MASTER Format---\r\nMMMMM\r\ntext");
  EXPECT_EQ("MMMMM---MEAL-MASTER Format---\r\nMMMMM\r\n", recipes(s)[0]);
}

TEST(PartitionTest, TwoRecipes) {
  istringstream s("MMMMM---MEAL-MASTER Format---\r\nMMMMM\r\nMMMMM---Recipe via Meal-Master\r\nMMMMM");
  auto result = recipes(s);
  EXPECT_EQ(2, result.size());
  EXPECT_EQ("MMMMM---MEAL-MASTER Format---\r\nMMMMM\r\n", result[0]);
  EXPECT_EQ("MMMMM---Recipe via Meal-Master\r\nMMMMM\r\n", result[1]);
}

TEST(PartitionTest, AlternateSeparator) {
  istringstream s("--------MEAL-MASTER Format---\r\n-----");
  EXPECT_EQ("--------MEAL-MASTER Format---\r\n-----\r\n", recipes(s)[0]);
}

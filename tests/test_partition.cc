/* AnyMeal recipe management software
   Copyright (C) 2021, 2023 Jan Wedekind

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
#include <sstream>
#include <gtest/gtest.h>
#include "partition.hh"


using namespace testing;
using namespace std;


TEST(PartitionTest, EmptyFile) {
  istringstream s("");
  ASSERT_TRUE(recipes(s).empty());
}

TEST(PartitionTest, MinimalRecipe) {
  istringstream s("MMMMM---MEAL-MASTER Format---\r\nMMMMM");
  ASSERT_EQ(1, recipes(s).size());
}

TEST(PartitionTest, RecipeContent) {
  istringstream s("MMMMM---MEAL-MASTER Format---\r\nMMMMM");
  ASSERT_EQ("MMMMM---MEAL-MASTER Format---\r\nMMMMM\r\n", recipes(s)[0]);
}

TEST(PartitionTest, ConvertUnixLF) {
  istringstream s("MMMMM---MEAL-MASTER Format---\nMMMMM");
  ASSERT_EQ("MMMMM---MEAL-MASTER Format---\r\nMMMMM\r\n", recipes(s)[0]);
}

TEST(PartitionTest, IgnoreStrayText) {
  istringstream s("text");
  ASSERT_TRUE(recipes(s).empty());
}

TEST(PartitionTest, EndOfRecipe) {
  istringstream s("MMMMM---MEAL-MASTER Format---\r\nMMMMM\r\ntext");
  ASSERT_EQ("MMMMM---MEAL-MASTER Format---\r\nMMMMM\r\n", recipes(s)[0]);
}

TEST(PartitionTest, TwoRecipes) {
  istringstream s("MMMMM---MEAL-MASTER Format---\r\nMMMMM\r\nMMMMM---Recipe via Meal-Master\r\nMMMMM");
  vector<string> result = recipes(s);
  ASSERT_EQ(2, result.size());
  EXPECT_EQ("MMMMM---MEAL-MASTER Format---\r\nMMMMM\r\n", result[0]);
  EXPECT_EQ("MMMMM---Recipe via Meal-Master\r\nMMMMM\r\n", result[1]);
}

TEST(PartitionTest, AlternateSeparator) {
  istringstream s("--------MEAL-MASTER Format---\r\n-----");
  EXPECT_EQ("--------MEAL-MASTER Format---\r\n-----\r\n", recipes(s)[0]);
}

TEST(PartitionTest, RecipeWithSection) {
  istringstream s("MMMMM---MEAL-MASTER Format-----\r\nMMMMM----section-----\r\nMMMMM\r\n");
  EXPECT_EQ("MMMMM---MEAL-MASTER Format-----\r\nMMMMM----section-----\r\nMMMMM\r\n", recipes(s)[0]);
}

TEST(PartitionTest, CheckRecipeIsClosed) {
  istringstream s("MMMMM---MEAL-MASTER Format---\r\nMMMMM");
  bool unexpected_eof = true;
  recipes(s, &unexpected_eof);
  ASSERT_FALSE(unexpected_eof);
}

TEST(PartitionTest, UnexpectedEndOfFile) {
  istringstream s("MMMMM---MEAL-MASTER Format---\r\n");
  bool unexpected_eof = false;
  recipes(s, &unexpected_eof);
  ASSERT_TRUE(unexpected_eof);
}


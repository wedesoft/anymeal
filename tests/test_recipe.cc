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
#include <gtest/gtest.h>
#include "recipe.hh"


using namespace testing;
using namespace std;


TEST(RecipeTest, DefaultValues) {
  EXPECT_EQ(0, Recipe().servings());
}

TEST(RecipeTest, AppendInstruction) {
  Recipe recipe;
  recipe.add_instruction("test");
  recipe.append_instruction("other");
  ASSERT_EQ(1, recipe.instructions().size());
  EXPECT_EQ("test other", recipe.instructions()[0]);
}

TEST(RecipeTest, AppendToEmptyInstruction) {
  Recipe recipe;
  recipe.add_instruction("");
  recipe.append_instruction("other");
  ASSERT_EQ(1, recipe.instructions().size());
  EXPECT_EQ("other", recipe.instructions()[0]);
}

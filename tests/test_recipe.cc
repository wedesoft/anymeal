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

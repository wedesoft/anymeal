#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "html.hh"


using namespace testing;
using namespace std;

TEST(HTMLTest, RecipeTitle) {
  Recipe recipe;
  recipe.set_title("Title");
  auto result = recipe_to_html(recipe);
}

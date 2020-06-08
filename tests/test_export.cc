#include <gtest/gtest.h>
#include "export.hh"


TEST(ExportTest, RecipeHeader) {
  Recipe recipe;
  recipe.set_title("apple pie");
  recipe.add_category("baking");
  recipe.add_category("cakes");
  recipe.set_servings(6);
  recipe.set_servings_unit("person");
  auto result = recipe_to_mealmaster(recipe);
  EXPECT_EQ("MMMMM----------------Meal-Master recipe exported by AnyMeal-----------------\r\n"
            "     Title: apple pie\r\n"
            "Categories: baking, cakes\r\n"
            "     Yield: 6 person\r\n"
            "\r\n"
            "MMMMM", result);
}

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

TEST(ExportTest, Ingredient) {
  Recipe recipe;
  recipe.set_title("apple pie");
  recipe.add_category("cakes");
  recipe.set_servings(6);
  recipe.set_servings_unit("person");
  Ingredient ingredient;
  ingredient.add_text("apples");
  recipe.add_ingredient(ingredient);
  auto result = recipe_to_mealmaster(recipe);
  EXPECT_EQ("MMMMM----------------Meal-Master recipe exported by AnyMeal-----------------\r\n"
            "     Title: apple pie\r\n"
            "Categories: cakes\r\n"
            "     Yield: 6 person\r\n"
            "\r\n"
            "           apples\r\n"
            "\r\n"
            "MMMMM", result);
}

TEST(ExportTest, IngredientUnit) {
  Recipe recipe;
  recipe.set_title("apple pie");
  recipe.add_category("cakes");
  recipe.set_servings(6);
  recipe.set_servings_unit("person");
  Ingredient ingredient;
  ingredient.set_unit("md");
  ingredient.add_text("apples");
  recipe.add_ingredient(ingredient);
  auto result = recipe_to_mealmaster(recipe);
  EXPECT_EQ("MMMMM----------------Meal-Master recipe exported by AnyMeal-----------------\r\n"
            "     Title: apple pie\r\n"
            "Categories: cakes\r\n"
            "     Yield: 6 person\r\n"
            "\r\n"
            "        md apples\r\n"
            "\r\n"
            "MMMMM", result);
}

TEST(ExportTest, IngredientAmountFloat) {
  Recipe recipe;
  recipe.set_title("apple pie");
  recipe.add_category("cakes");
  recipe.set_servings(6);
  recipe.set_servings_unit("person");
  Ingredient ingredient;
  ingredient.set_amount_float(1.5);
  ingredient.set_unit("md");
  ingredient.add_text("apples");
  recipe.add_ingredient(ingredient);
  auto result = recipe_to_mealmaster(recipe);
  EXPECT_EQ("MMMMM----------------Meal-Master recipe exported by AnyMeal-----------------\r\n"
            "     Title: apple pie\r\n"
            "Categories: cakes\r\n"
            "     Yield: 6 person\r\n"
            "\r\n"
            "    1.5 md apples\r\n"
            "\r\n"
            "MMMMM", result);
}

TEST(ExportTest, IngredientCropFloat) {
  Recipe recipe;
  recipe.set_title("apple pie");
  recipe.add_category("cakes");
  recipe.set_servings(6);
  recipe.set_servings_unit("person");
  Ingredient ingredient;
  ingredient.set_amount_float(1.0/3.0);
  ingredient.set_unit("md");
  ingredient.add_text("apples");
  recipe.add_ingredient(ingredient);
  auto result = recipe_to_mealmaster(recipe);
  EXPECT_EQ("MMMMM----------------Meal-Master recipe exported by AnyMeal-----------------\r\n"
            "     Title: apple pie\r\n"
            "Categories: cakes\r\n"
            "     Yield: 6 person\r\n"
            "\r\n"
            "0.33333 md apples\r\n"
            "\r\n"
            "MMMMM", result);
}

TEST(ExportTest, IngredientAmountInteger) {
  Recipe recipe;
  recipe.set_title("apple pie");
  recipe.add_category("cakes");
  recipe.set_servings(6);
  recipe.set_servings_unit("person");
  Ingredient ingredient;
  ingredient.set_amount_integer(100);
  ingredient.set_unit("lg");
  ingredient.add_text("apples");
  recipe.add_ingredient(ingredient);
  auto result = recipe_to_mealmaster(recipe);
  EXPECT_EQ("MMMMM----------------Meal-Master recipe exported by AnyMeal-----------------\r\n"
            "     Title: apple pie\r\n"
            "Categories: cakes\r\n"
            "     Yield: 6 person\r\n"
            "\r\n"
            "    100 lg apples\r\n"
            "\r\n"
            "MMMMM", result);
}

TEST(ExportTest, IngredientAmountFraction) {
  Recipe recipe;
  recipe.set_title("apple pie");
  recipe.add_category("cakes");
  recipe.set_servings(6);
  recipe.set_servings_unit("person");
  Ingredient ingredient;
  ingredient.set_amount_numerator(2);
  ingredient.set_amount_denominator(3);
  ingredient.set_unit("lg");
  ingredient.add_text("apples");
  recipe.add_ingredient(ingredient);
  auto result = recipe_to_mealmaster(recipe);
  EXPECT_EQ("MMMMM----------------Meal-Master recipe exported by AnyMeal-----------------\r\n"
            "     Title: apple pie\r\n"
            "Categories: cakes\r\n"
            "     Yield: 6 person\r\n"
            "\r\n"
            "    2/3 lg apples\r\n"
            "\r\n"
            "MMMMM", result);
}

TEST(ExportTest, IngredientAmountComposite) {
  Recipe recipe;
  recipe.set_title("apple pie");
  recipe.add_category("cakes");
  recipe.set_servings(6);
  recipe.set_servings_unit("person");
  Ingredient ingredient;
  ingredient.set_amount_integer(1);
  ingredient.set_amount_numerator(2);
  ingredient.set_amount_denominator(3);
  ingredient.set_unit("lg");
  ingredient.add_text("apples");
  recipe.add_ingredient(ingredient);
  auto result = recipe_to_mealmaster(recipe);
  EXPECT_EQ("MMMMM----------------Meal-Master recipe exported by AnyMeal-----------------\r\n"
            "     Title: apple pie\r\n"
            "Categories: cakes\r\n"
            "     Yield: 6 person\r\n"
            "\r\n"
            "  1 2/3 lg apples\r\n"
            "\r\n"
            "MMMMM", result);
}

TEST(ExportTest, IngredientMultiLine) {
  Recipe recipe;
  recipe.set_title("apple pie");
  recipe.add_category("cakes");
  recipe.set_servings(6);
  recipe.set_servings_unit("person");
  Ingredient ingredient;
  ingredient.add_text("apples; with a somewhat long description");
  recipe.add_ingredient(ingredient);
  auto result = recipe_to_mealmaster(recipe);
  EXPECT_EQ("MMMMM----------------Meal-Master recipe exported by AnyMeal-----------------\r\n"
            "     Title: apple pie\r\n"
            "Categories: cakes\r\n"
            "     Yield: 6 person\r\n"
            "\r\n"
            "           apples; with a somewhat long\r\n"
            "           -description\r\n"
            "\r\n"
            "MMMMM", result);
}

TEST(ExportTest, IngredientEmergencyBreak) {
  Recipe recipe;
  recipe.set_title("apple pie");
  recipe.add_category("cakes");
  recipe.set_servings(6);
  recipe.set_servings_unit("person");
  Ingredient ingredient;
  ingredient.add_text("apples-with-a-somewhat-long-description");
  recipe.add_ingredient(ingredient);
  auto result = recipe_to_mealmaster(recipe);
  EXPECT_EQ("MMMMM----------------Meal-Master recipe exported by AnyMeal-----------------\r\n"
            "     Title: apple pie\r\n"
            "Categories: cakes\r\n"
            "     Yield: 6 person\r\n"
            "\r\n"
            "           apples-with-a-somewhat-long-\r\n"
            "           -description\r\n"
            "\r\n"
            "MMMMM", result);
}

TEST(ExportTest, IngredientPreserveUTF8) {
  Recipe recipe;
  recipe.set_title("apple pie");
  recipe.add_category("cakes");
  recipe.set_servings(6);
  recipe.set_servings_unit("person");
  Ingredient ingredient;
  ingredient.add_text("apples-with-a-somewhat-longädescription");
  recipe.add_ingredient(ingredient);
  auto result = recipe_to_mealmaster(recipe);
  EXPECT_EQ("MMMMM----------------Meal-Master recipe exported by AnyMeal-----------------\r\n"
            "     Title: apple pie\r\n"
            "Categories: cakes\r\n"
            "     Yield: 6 person\r\n"
            "\r\n"
            "           apples-with-a-somewhat-long\r\n"
            "           -ädescription\r\n"
            "\r\n"
            "MMMMM", result);
}

TEST(ExportTest, IngredientSection) {
  Recipe recipe;
  recipe.set_title("apple pie");
  recipe.add_category("cakes");
  recipe.set_servings(6);
  recipe.set_servings_unit("person");
  recipe.add_ingredient_section(0, "for cake");
  Ingredient ingredient;
  ingredient.set_unit("md");
  ingredient.add_text("apples");
  recipe.add_ingredient(ingredient);
  auto result = recipe_to_mealmaster(recipe);
  EXPECT_EQ("MMMMM----------------Meal-Master recipe exported by AnyMeal-----------------\r\n"
            "     Title: apple pie\r\n"
            "Categories: cakes\r\n"
            "     Yield: 6 person\r\n"
            "\r\n"
            "MMMMM-------------------------------for cake--------------------------------\r\n"
            "        md apples\r\n"
            "\r\n"
            "MMMMM", result);
}

TEST(ExportTest, EmptyLineBeforeIngredientSection) {
  Recipe recipe;
  recipe.set_title("apple pie");
  recipe.add_category("cakes");
  recipe.set_servings(6);
  recipe.set_servings_unit("person");
  Ingredient ingredient1;
  ingredient1.set_unit("md");
  ingredient1.add_text("apples");
  recipe.add_ingredient(ingredient1);
  recipe.add_ingredient_section(1, "for cake");
  Ingredient ingredient2;
  ingredient2.set_unit("lg");
  ingredient2.add_text("almonds");
  recipe.add_ingredient(ingredient2);
  auto result = recipe_to_mealmaster(recipe);
  EXPECT_EQ("MMMMM----------------Meal-Master recipe exported by AnyMeal-----------------\r\n"
            "     Title: apple pie\r\n"
            "Categories: cakes\r\n"
            "     Yield: 6 person\r\n"
            "\r\n"
            "        md apples\r\n"
            "\r\n"
            "MMMMM-------------------------------for cake--------------------------------\r\n"
            "        lg almonds\r\n"
            "\r\n"
            "MMMMM", result);
}

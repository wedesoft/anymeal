#include <gtest/gtest.h>
#include "html.hh"


using namespace testing;
using namespace std;

TEST(HTMLTest, EmptyRecipe) {
  Recipe recipe;
  string result = recipe_to_html(recipe);
  EXPECT_EQ("<html>\n  <head>\n  </head>\n  <body>\n  </body>\n</html>", result);
}

TEST(HTMLTest, RecipeTitle) {
  Recipe recipe;
  recipe.set_title("Title");
  string result = recipe_to_html(recipe);
  EXPECT_EQ("<html>\n  <head>\n    <title>Title</title>\n  </head>\n  <body>\n    <h2>Title</h2>\n  </body>\n</html>", result);
}

TEST(HTMLTest, EscapeTitle) {
  Recipe recipe;
  recipe.set_title("B & B");
  string result = recipe_to_html(recipe);
  EXPECT_EQ("<html>\n  <head>\n    <title>B &amp; B</title>\n  </head>\n"
            "  <body>\n    <h2>B &amp; B</h2>\n  </body>\n</html>", result);
}

TEST(HTMLTest, Instructions) {
  Recipe recipe;
  recipe.add_instruction("Mix & shake.");
  string result = recipe_to_html(recipe);
  EXPECT_EQ("<html>\n  <head>\n  </head>\n  <body>\n    <h3>Instructions</h3>"
            "\n    <p>Mix &amp; shake.</p>\n  </body>\n</html>", result);
}

TEST(HTMLTest, InstructionsLineBreak) {
  Recipe recipe;
  recipe.add_instruction("Line one");
  recipe.add_instruction("Line two");
  string result = recipe_to_html(recipe);
  EXPECT_EQ("<html>\n  <head>\n  </head>\n  <body>\n    <h3>Instructions</h3>"
            "\n    <p>Line one<br/>Line two</p>\n  </body>\n</html>", result);
}

TEST(HTMLTest, Servings) {
  Recipe recipe;
  recipe.set_servings(4);
  recipe.set_servings_unit("servings");
  string result = recipe_to_html(recipe);
  EXPECT_EQ("<html>\n  <head>\n  </head>\n  <body>\n    <p><b>Yield:</b> 4 servings</p>\n  </body>\n</html>", result);
}

TEST(HTMLTest, Categories) {
  Recipe recipe;
  recipe.add_category("bread");
  recipe.add_category("brewing");
  string result = recipe_to_html(recipe);
  EXPECT_EQ("<html>\n  <head>\n  </head>\n  <body>\n    <p><b>Categories:</b> bread, brewing</p>\n  </body>\n</html>", result);
}

TEST(HTMLTest, InstructionSection) {
  Recipe recipe;
  recipe.add_instruction_section(0, "Main");
  recipe.add_instruction("Mix well.");
  string result = recipe_to_html(recipe);
  EXPECT_EQ("<html>\n  <head>\n  </head>\n  <body>\n    <h3>Instructions</h3>\n    <h4>Main</h4>"
            "\n    <p>Mix well.</p>\n  </body>\n</html>", result);
}

TEST(HTMLTest, InstructionSections) {
  Recipe recipe;
  recipe.add_instruction_section(0, "Main");
  recipe.add_instruction("Mix well.");
  recipe.add_instruction_section(1, "More");
  recipe.add_instruction("Cook it.");
  string result = recipe_to_html(recipe);
  EXPECT_EQ("<html>\n  <head>\n  </head>\n  <body>\n    <h3>Instructions</h3>\n    <h4>Main</h4>"
            "\n    <p>Mix well.</p>\n    <h4>More</h4>\n    <p>Cook it.</p>\n  </body>\n</html>", result);
}

TEST(HTMLTest, Ingredient) {
  Recipe recipe;
  Ingredient ingredient;
  ingredient.add_text("flour");
  recipe.add_ingredient(ingredient);
  string result = recipe_to_html(recipe);
  EXPECT_EQ("<html>\n  <head>\n  </head>\n  <body>\n    <h3>Ingredients</h3>\n    <table>\n      <tr style=\"white-space:nowrap;\">\n"
            "        <th style=\"text-align:left\">amount&nbsp;</th>\n        <th style=\"text-align:left\">unit&nbsp;</th>\n"
            "        <th style=\"text-align:left\">ingredient</th>\n      </tr>\n      <tr>\n        <td>&nbsp;</td>\n"
            "        <td>&nbsp;</td>\n        <td>flour</td>\n      </tr>\n    </table>\n  </body>\n</html>", result);
}

TEST(HTMLTest, IngredientUnit) {
  Recipe recipe;
  Ingredient ingredient;
  ingredient.add_text("apple");
  ingredient.set_unit("sm");
  recipe.add_ingredient(ingredient);
  string result = recipe_to_html(recipe);
  EXPECT_EQ("<html>\n  <head>\n  </head>\n  <body>\n    <h3>Ingredients</h3>\n    <table>\n      <tr style=\"white-space:nowrap;\">\n"
            "        <th style=\"text-align:left\">amount&nbsp;</th>\n        <th style=\"text-align:left\">unit&nbsp;</th>\n"
            "        <th style=\"text-align:left\">ingredient</th>\n      </tr>\n      <tr>\n        <td>&nbsp;</td>\n"
            "        <td>small&nbsp;</td>\n        <td>apple</td>\n      </tr>\n    </table>\n  </body>\n</html>", result);
}

TEST(HTMLTest, IngredientAmountRational) {
  Recipe recipe;
  Ingredient ingredient;
  ingredient.add_text("sugar");
  ingredient.set_unit("g ");
  ingredient.set_amount_integer(1);
  ingredient.set_amount_numerator(2);
  ingredient.set_amount_denominator(3);
  recipe.add_ingredient(ingredient);
  string result = recipe_to_html(recipe);
  EXPECT_EQ("<html>\n  <head>\n  </head>\n  <body>\n    <h3>Ingredients</h3>\n    <table>\n      <tr style=\"white-space:nowrap;\">\n"
            "        <th style=\"text-align:left\">amount&nbsp;</th>\n        <th style=\"text-align:left\">unit&nbsp;</th>\n"
            "        <th style=\"text-align:left\">ingredient</th>\n      </tr>\n      <tr>\n        <td>1 2/3&nbsp;</td>\n"
            "        <td>gram&nbsp;</td>\n        <td>sugar</td>\n      </tr>\n    </table>\n  </body>\n</html>", result);
}

TEST(HTMLTest, IngredientAmountFloat) {
  Recipe recipe;
  Ingredient ingredient;
  ingredient.add_text("sugar");
  ingredient.set_unit("g ");
  ingredient.set_amount_float(1.5);
  recipe.add_ingredient(ingredient);
  string result = recipe_to_html(recipe);
  EXPECT_EQ("<html>\n  <head>\n  </head>\n  <body>\n    <h3>Ingredients</h3>\n    <table>\n      <tr style=\"white-space:nowrap;\">\n"
            "        <th style=\"text-align:left\">amount&nbsp;</th>\n        <th style=\"text-align:left\">unit&nbsp;</th>\n"
            "        <th style=\"text-align:left\">ingredient</th>\n      </tr>\n      <tr>\n        <td>1.5&nbsp;</td>\n"
            "        <td>gram&nbsp;</td>\n        <td>sugar</td>\n      </tr>\n    </table>\n  </body>\n</html>", result);
}

TEST(HTMLTest, IngredientSection) {
  Recipe recipe;
  recipe.add_ingredient_section(0, "Main");
  Ingredient ingredient;
  ingredient.add_text("flour");
  recipe.add_ingredient(ingredient);
  string result = recipe_to_html(recipe);
  EXPECT_EQ("<html>\n  <head>\n  </head>\n  <body>\n    <h3>Ingredients</h3>\n    <table>\n      <tr style=\"white-space:nowrap;\">\n"
            "        <th style=\"text-align:left\">amount&nbsp;</th>\n        <th style=\"text-align:left\">unit&nbsp;</th>\n"
            "        <th style=\"text-align:left\">ingredient</th>\n      </tr>\n      <tr>\n"
            "        <td colspan=\"3\"><em>Main</em></td>\n      </tr>\n      <tr>\n        <td>&nbsp;</td>\n"
            "        <td>&nbsp;</td>\n        <td>flour</td>\n      </tr>\n    </table>\n  </body>\n</html>", result);
}

#include <gtest/gtest.h>
#include <gmock/gmock.h>
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

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "recode.hh"


using namespace testing;
using namespace std;

TEST(RecodeTest, Constructor) {
  Recoder r("latin1..utf8");
}

TEST(RecodeTest, NoSuchEncoding) {
  EXPECT_THROW(Recoder("latin1..unknown"), recode_exception);
}

TEST(RecodeTest, RecodeString) {
  Recoder r("latin1..utf8");
  string s("\xc4pfel");
  EXPECT_EQ("Äpfel", r.process(s));
}

TEST(RecodeTest, RecodeException) {
  Recoder r("latin1..ascii");
  string s("\xc4pfel");
  EXPECT_THROW(r.process(s), recode_exception);
}

TEST(RecodeTest, RecodeIngredient) {
  Recoder r("latin1..utf8");
  Ingredient ingredient;
  ingredient.set_amount_type(AMOUNT_RATIONAL);
  ingredient.set_amount_integer(2);
  ingredient.set_amount_numerator(3);
  ingredient.set_amount_denominator(5);
  ingredient.set_amount_float(2.6);
  ingredient.set_unit("g ");
  ingredient.add_text("\xc4pfel");
  Ingredient result = r.process_ingredient(ingredient);
  EXPECT_EQ(AMOUNT_RATIONAL, result.amount_type());
  EXPECT_EQ(2, result.amount_integer());
  EXPECT_EQ(3, result.amount_numerator());
  EXPECT_EQ(5, result.amount_denominator());
  EXPECT_EQ(2.6, result.amount_float());
  EXPECT_EQ("g ", result.unit());
  EXPECT_EQ("Äpfel", result.text());
}

TEST(RecodeTest, RecodeRecipe) {
  Recoder r("latin1..utf8");
  Recipe recipe;
  recipe.set_title("K\xfc""chlein");
  recipe.add_category("Geb\xe4""ck");
  recipe.set_servings(4);
  recipe.set_servings_unit("Porti\xf6nchen");
  Ingredient ingredient;
  ingredient.add_text("\xc4pfel");
  recipe.add_ingredient(ingredient);
  recipe.add_ingredient_section(1, "Teilst\xfc""ck");
  recipe.add_instruction("Teig anr\xfchren");
  recipe.add_instruction_section(1, "R\xfchren");
  Recipe result = r.process_recipe(recipe);
  EXPECT_EQ("Küchlein", result.title());
  ASSERT_EQ(1, result.categories().size());
  EXPECT_EQ("Gebäck", result.categories()[0]);
  EXPECT_EQ(4, result.servings());
  EXPECT_EQ("Portiönchen", result.servings_unit());
  EXPECT_EQ(1, result.ingredients().size());
  EXPECT_EQ("Äpfel", result.ingredients()[0].text());
  EXPECT_EQ(1, result.ingredient_sections().size());
  EXPECT_EQ(1, result.ingredient_sections()[0].first);
  EXPECT_EQ("Teilstück", result.ingredient_sections()[0].second);
  EXPECT_EQ(1, result.instructions().size());
  EXPECT_EQ("Teig anrühren", result.instructions()[0]);
  EXPECT_EQ(1, result.instruction_sections().size());
  EXPECT_EQ(1, result.instruction_sections()[0].first);
  EXPECT_EQ("Rühren", result.instruction_sections()[0].second);
}

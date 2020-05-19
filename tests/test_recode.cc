#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "recode.hh"


using namespace testing;
using namespace std;

TEST(RecodeTest, Constructor) {
  Recoder r("latin1..utf8");
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
  Recipe result = r.process_recipe(recipe);
  EXPECT_EQ("Küchlein", result.title());
  ASSERT_EQ(1, result.categories().size());
  EXPECT_EQ("Gebäck", result.categories()[0]);
}

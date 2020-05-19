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
  EXPECT_EQ(result.amount_type(), AMOUNT_RATIONAL);
  EXPECT_EQ(result.amount_integer(), 2);
  EXPECT_EQ(result.amount_numerator(), 3);
  EXPECT_EQ(result.amount_denominator(), 5);
  EXPECT_EQ(result.amount_float(), 2.6);
  EXPECT_EQ(result.unit(), "g ");
  EXPECT_EQ(result.text(), "Äpfel");
}

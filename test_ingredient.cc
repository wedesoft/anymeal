#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "ingredient.hh"


using namespace testing;
using namespace std;


TEST(IngredientTest, DefaultValues) {
  Ingredient ingredient;
  EXPECT_EQ(AMOUNT_NONE, ingredient.amount_type());
  EXPECT_EQ(0, ingredient.amount_integer());
  EXPECT_EQ(0, ingredient.amount_numerator());
  EXPECT_EQ(1, ingredient.amount_denominator());
}

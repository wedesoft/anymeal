#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "ingredient.hh"


using namespace testing;
using namespace std;


TEST(IngredientTest, DefaultValues) {
  EXPECT_EQ(AMOUNT_NONE, Ingredient().amount_type());
}

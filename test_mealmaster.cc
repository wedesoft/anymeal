#include <sstream>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "mealmaster.hh"


using namespace testing;
using namespace std;


TEST(MealMasterTest, RecipeTitle) {
  istringstream s("MMMMM---Meal-Master---\r\n     Title: test recipe\r\nMMMMM\r\n");
  auto result = parse_mealmaster(s);
  EXPECT_EQ("test recipe", result.title());
}

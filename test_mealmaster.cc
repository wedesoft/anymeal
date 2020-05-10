#include <fstream>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "mealmaster.hh"


using namespace testing;
using namespace std;


TEST(MealMasterTest, RecipeTitle) {
  ifstream f("test.mmf");
  auto result = parse_mealmaster(f);
  EXPECT_EQ("apple pie", result.title());
}

TEST(MealMasterTest, Categories) {
  ifstream f("test.mmf");
  auto result = parse_mealmaster(f);
  ASSERT_EQ(2, result.categories().size());
  EXPECT_EQ("pastries", result.categories()[0]);
  EXPECT_EQ("cakes", result.categories()[1]);
}

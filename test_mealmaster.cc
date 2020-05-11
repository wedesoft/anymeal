#include <fstream>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "mealmaster.hh"


using namespace testing;
using namespace std;


TEST(MealMasterTest, RecipeTitle) {
  ifstream f("test_header.mmf");
  auto result = parse_mealmaster(f);
  EXPECT_EQ("apple pie", result.title());
}

TEST(MealMasterTest, Categories) {
  ifstream f("test_header.mmf");
  auto result = parse_mealmaster(f);
  ASSERT_EQ(2, result.categories().size());
  EXPECT_EQ("pastries", result.categories()[0]);
  EXPECT_EQ("cakes", result.categories()[1]);
}

TEST(MealMasterTest, Servings) {
  ifstream f("test_header2.mmf");
  auto result = parse_mealmaster(f);
  EXPECT_EQ(42, result.servings());
  EXPECT_EQ("servings", result.servings_unit());
}

TEST(MealMasterTest, IntegerIngredient) {
  ifstream f("test_ingredient.mmf");
  auto result = parse_mealmaster(f);
  ASSERT_EQ(1, result.ingredients().size());
  EXPECT_EQ(AMOUNT_INTEGER, result.ingredients()[0].amount_type());
  EXPECT_EQ(250, result.ingredients()[0].amount_integer());
}

TEST(MealMasterTest, IngredientUnit) {
  ifstream f("test_ingredient.mmf");
  auto result = parse_mealmaster(f);
  ASSERT_EQ(1, result.ingredients().size());
  EXPECT_EQ("g ", result.ingredients()[0].unit());
}

TEST(MealMasterTest, IngredientText) {
  ifstream f("test_ingredient.mmf");
  auto result = parse_mealmaster(f);
  ASSERT_EQ(1, result.ingredients().size());
  EXPECT_EQ("brown flour", result.ingredients()[0].text());
}

TEST(MealMasterTest, PreparationText) {
  ifstream f("test_preparation.mmf");
  auto result = parse_mealmaster(f);
  ASSERT_EQ(1, result.ingredients().size());
  EXPECT_EQ("sifted", result.ingredients()[0].preparation());
}

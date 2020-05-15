#include <fstream>
#include <sstream>
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

TEST(MealMasterTest, IntegerAmount) {
  ifstream f("test_ingredient.mmf");
  auto result = parse_mealmaster(f);
  ASSERT_EQ(1, result.ingredients().size());
  EXPECT_EQ(AMOUNT_RATIONAL, result.ingredients()[0].amount_type());
  EXPECT_EQ(250, result.ingredients()[0].amount_integer());
}

TEST(MealMasterTest, FractionAmount) {
  ifstream f("test_fraction.mmf");
  auto result = parse_mealmaster(f);
  ASSERT_EQ(1, result.ingredients().size());
  EXPECT_EQ(AMOUNT_RATIONAL, result.ingredients()[0].amount_type());
  EXPECT_EQ(0, result.ingredients()[0].amount_integer());
  EXPECT_EQ(2, result.ingredients()[0].amount_numerator());
  EXPECT_EQ(3, result.ingredients()[0].amount_denominator());
}

TEST(MealMasterTest, MixedAmount) {
  ifstream f("test_mixed.mmf");
  auto result = parse_mealmaster(f);
  ASSERT_EQ(1, result.ingredients().size());
  EXPECT_EQ(AMOUNT_RATIONAL, result.ingredients()[0].amount_type());
  EXPECT_EQ(1, result.ingredients()[0].amount_integer());
  EXPECT_EQ(2, result.ingredients()[0].amount_numerator());
  EXPECT_EQ(3, result.ingredients()[0].amount_denominator());
}

TEST(MealMasterTest, NoAmount) {
  ifstream f("test_noamount.mmf");
  auto result = parse_mealmaster(f);
  ASSERT_EQ(1, result.ingredients().size());
  EXPECT_EQ(AMOUNT_NONE, result.ingredients()[0].amount_type());
  EXPECT_EQ("  ", result.ingredients()[0].unit());
  EXPECT_EQ("butter", result.ingredients()[0].text());
}

TEST(MealMasterTest, FloatingAmount) {
  ifstream f("test_float.mmf");
  auto result = parse_mealmaster(f);
  ASSERT_EQ(1, result.ingredients().size());
  EXPECT_EQ(AMOUNT_FLOAT, result.ingredients()[0].amount_type());
  EXPECT_EQ(2.5, result.ingredients()[0].amount_float());
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
  EXPECT_EQ("brown flour; sifted", result.ingredients()[0].text());
}

TEST(MealMasterTest, IngredientContinuation) {
  ifstream f("test_continuation.mmf");
  auto result = parse_mealmaster(f);
  ASSERT_EQ(1, result.ingredients().size());
  EXPECT_EQ("butter; molten and stirred", result.ingredients()[0].text());
}

TEST(MealMasterTest, TwoIngredients) {
  ifstream f("test_two_ingredients.mmf");
  auto result = parse_mealmaster(f);
  ASSERT_EQ(2, result.ingredients().size());
  EXPECT_EQ("pastry", result.ingredients()[0].text());
  EXPECT_EQ("cooking apple", result.ingredients()[1].text());
}

TEST(MealMasterTest, Instructions1) {
  ifstream f("test_instructions1.mmf");
  auto result = parse_mealmaster(f);
  ASSERT_EQ(1, result.instructions().size());
  EXPECT_EQ("Cook all the ingredients.", result.instructions()[0]);
}

TEST(MealMasterTest, Instructions2) {
  ifstream f("test_instructions2.mmf");
  auto result = parse_mealmaster(f);
  ASSERT_EQ(1, result.instructions().size());
  EXPECT_EQ("1 cook is used.", result.instructions()[0]);
}

TEST(MealMasterTest, Instructions3) {
  ifstream f("test_instructions3.mmf");
  auto result = parse_mealmaster(f);
  ASSERT_EQ(1, result.instructions().size());
  EXPECT_EQ("       Start cooking.", result.instructions()[0]);
}

TEST(MealMasterTest, Instructions4) {
  ifstream f("test_instructions4.mmf");
  auto result = parse_mealmaster(f);
  ASSERT_EQ(1, result.instructions().size());
  EXPECT_EQ("2.5 apples are used.", result.instructions()[0]);
}

TEST(MealMasterTest, Instructions5) {
  ifstream f("test_instructions5.mmf");
  auto result = parse_mealmaster(f);
  ASSERT_EQ(1, result.instructions().size());
  EXPECT_EQ("           -", result.instructions()[0]);
}

TEST(MealMasterTest, Instructions6) {
  ifstream f("test_instructions6.mmf");
  auto result = parse_mealmaster(f);
  ASSERT_EQ(1, result.instructions().size());
  EXPECT_EQ("2 3 apples.", result.instructions()[0]);
}

TEST(MealMasterTest, Instructions7) {
  ifstream f("test_instructions7.mmf");
  auto result = parse_mealmaster(f);
  ASSERT_EQ(1, result.instructions().size());
  EXPECT_EQ("1/x apples.", result.instructions()[0]);
}

TEST(MealMasterTest, Instructions8) {
  ifstream f("test_instructions8.mmf");
  auto result = parse_mealmaster(f);
  ASSERT_EQ(1, result.instructions().size());
  EXPECT_EQ("1/2 apples.", result.instructions()[0]);
}

TEST(MealMasterTest, Instructions9) {
  ifstream f("test_instructions9.mmf");
  auto result = parse_mealmaster(f);
  ASSERT_EQ(1, result.instructions().size());
  EXPECT_EQ("1 2/x apples.", result.instructions()[0]);
}

TEST(MealMasterTest, Instructions10) {
  ifstream f("test_instructions10.mmf");
  auto result = parse_mealmaster(f);
  ASSERT_EQ(1, result.instructions().size());
  EXPECT_EQ("1 2/3 each.", result.instructions()[0]);
}

TEST(MealMasterTest, TwoInstructions) {
  ifstream f("test_two_instructions.mmf");
  auto result = parse_mealmaster(f);
  ASSERT_EQ(2, result.instructions().size());
  EXPECT_EQ("First line.", result.instructions()[0]);
  EXPECT_EQ("Second line.", result.instructions()[1]);
}

TEST(MealMasterTest, UnexpectedEndOfFile) {
  istringstream s("MMMMM---MEAL-MASTER Format---\r\n");
  EXPECT_THROW(parse_mealmaster(s), parse_exception);
}

TEST(MealMasterTest, ExpectingServings) {
  ifstream f("test_no_servings.mmf");
  EXPECT_THROW(parse_mealmaster(f), parse_exception);
}

TEST(MealMasterTest, IngredientSection) {
  ifstream f("test_ingredient_section.mmf");
  auto result = parse_mealmaster(f);
  ASSERT_EQ(1, result.ingredient_sections().size());
  auto section = result.ingredient_sections()[0];
  EXPECT_EQ(1, section.first);
  EXPECT_EQ("meringue", section.second);
}

TEST(MealMasterTest, SectionWithHyphen) {
  ifstream f("test_section_hyphen.mmf");
  auto result = parse_mealmaster(f);
  ASSERT_EQ(1, result.ingredient_sections().size());
  auto section = result.ingredient_sections()[0];
  EXPECT_EQ(1, section.first);
  EXPECT_EQ("section-hyphen", section.second);
}

TEST(MealMasterTest, SectionWithSpaces) {
  ifstream f("test_section_spaces.mmf");
  auto result = parse_mealmaster(f);
  ASSERT_EQ(1, result.ingredient_sections().size());
  auto section = result.ingredient_sections()[0];
  EXPECT_EQ(1, section.first);
  EXPECT_EQ("section spaces", section.second);
}

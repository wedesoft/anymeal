/* AnyMeal recipe management software
   Copyright (C) 2021, 2023 Jan Wedekind

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>. */
#include <gtest/gtest.h>
#include "recode.hh"


using namespace testing;
using namespace std;

TEST(RecodeTest, Constructor) {
  Recoder r("ISO-8859-1", "UTF-8");
}

TEST(RecodeTest, NoSuchEncoding) {
  EXPECT_THROW(Recoder("ISO-8859-1", "unknown"), recode_exception);
}

TEST(RecodeTest, RecodeString) {
  Recoder r("ISO-8859-1", "UTF-8");
  string s("\xc4pfel");
  EXPECT_EQ("Äpfel", r.process(s));
}

TEST(RecodeTest, RecodeException) {
  Recoder r("ISO-8859-1", "US-ASCII");
  string s("\xc4pfel");
  EXPECT_THROW(r.process(s), recode_exception);
}

TEST(RecodeTest, RecodeIngredient) {
  Recoder r("ISO-8859-1", "UTF-8");
  Ingredient ingredient;
  ingredient.set_amount_integer(2);
  ingredient.set_amount_numerator(3);
  ingredient.set_amount_denominator(5);
  ingredient.set_amount_float(2.6);
  ingredient.set_unit("g ");
  ingredient.add_text("\xc4pfel");
  Ingredient result = r.process_ingredient(ingredient);
  EXPECT_EQ(2, result.amount_integer());
  EXPECT_EQ(3, result.amount_numerator());
  EXPECT_EQ(5, result.amount_denominator());
  EXPECT_EQ(2.6, result.amount_float());
  EXPECT_EQ("g ", result.unit());
  EXPECT_EQ("Äpfel", result.text());
}

TEST(RecodeTest, RecodeRecipe) {
  Recoder r("ISO-8859-1", "UTF-8");
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

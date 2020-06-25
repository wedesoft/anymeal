/* AnyMeal recipe database software
   Copyright (C) 2020 Jan Wedekind

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
#pragma once
#include <utility>
#include <string>
#include <vector>
#include "ingredient.hh"


class Recipe
{
public:
  Recipe(void): m_servings(0) {}
  void set_title(const char *title) { m_title = title; }
  std::string &title(void) { return m_title; }
  std::vector<std::string> &categories(void) { return m_categories; }
  void add_category(const char *category) { m_categories.push_back(category); }
  int servings(void) { return m_servings; }
  void set_servings(int servings) { m_servings = servings; }
  std::string &servings_unit(void) { return m_servings_unit; }
  void set_servings_unit(const char *servings_unit) { m_servings_unit = servings_unit; }
  std::vector<Ingredient> &ingredients(void) { return m_ingredients; }
  void add_ingredient(const Ingredient &ingredient) { m_ingredients.push_back(ingredient); }
  void set_ingredients(const std::vector<Ingredient> &ingredients) { m_ingredients = ingredients; }
  std::vector<std::pair<int, std::string>> &ingredient_sections(void) { return m_ingredient_sections; }
  void add_ingredient_section(int row, const char *title);
  void set_ingredient_sections(const std::vector<std::pair<int, std::string>> &sections) { m_ingredient_sections = sections; }
  std::vector<std::string> &instructions(void) { return m_instructions; }
  void add_instruction(const char *instruction) { m_instructions.push_back(instruction); }
  void append_instruction(const char *instruction);
  std::vector<std::pair<int, std::string>> &instruction_sections(void) { return m_instruction_sections; }
  void add_instruction_section(int row, const char *title);
protected:
  std::string m_title;
  std::vector<std::string> m_categories;
  int m_servings;
  std::string m_servings_unit;
  std::vector<Ingredient> m_ingredients;
  std::vector<std::pair<int, std::string>> m_ingredient_sections;
  std::vector<std::string> m_instructions;
  std::vector<std::pair<int, std::string>> m_instruction_sections;
};

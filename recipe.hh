#pragma once
#include <string>
#include <vector>
#include "ingredient.hh"


class Recipe
{
public:
  Recipe(void);
  void set_title(const char *title) { m_title = title; }
  std::string &title(void) { return m_title; }
  std::vector<std::string> &categories(void) { return m_categories; }
  void add_category(const char *category) { m_categories.push_back(category); }
  int servings(void) { return m_servings; }
  void set_servings(int servings) { m_servings = servings; }
  std::string &servings_unit(void) { return m_servings_unit; }
  void set_servings_unit(const char *servings_unit) { m_servings_unit = servings_unit; }
  std::vector<Ingredient> &ingredients(void) { return m_ingredients; }
  void add_ingredient(Ingredient &ingredient) { m_ingredients.push_back(ingredient); }
  std::vector<std::string> &instructions(void) { return m_instructions; }
  void add_instruction(const char *instruction) { m_instructions.push_back(instruction); }
protected:
  std::string m_title;
  std::vector<std::string> m_categories;
  int m_servings;
  std::string m_servings_unit;
  std::vector<Ingredient> m_ingredients;
  std::vector<std::string> m_instructions;
};

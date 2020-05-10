#pragma once
#include <string>
#include <vector>


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
protected:
  std::string m_title;
  std::vector<std::string> m_categories;
  int m_servings;
  std::string m_servings_unit;
};

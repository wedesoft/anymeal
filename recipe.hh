#pragma once
#include <string>
#include <vector>


class Recipe
{
public:
  Recipe(void) {}
  void set_title(const char *title) { m_title = title; }
  std::string &title(void) { return m_title; }
  std::vector<std::string> &categories(void) { return m_categories; }
  void add_category(const char *category) { m_categories.push_back(category); }
protected:
  std::string m_title;
  std::vector<std::string> m_categories;
};

#pragma once
#include <string>


class Recipe
{
public:
  Recipe(void) {}
  void set_title(const char *title) { m_title = title; }
  std::string title(void) { return m_title; }
protected:
  std::string m_title;
};

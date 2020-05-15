#pragma once
#include <istream>
#include <exception>
#include <string>
#include "recipe.hh"


class parse_exception: public std::exception
{
public:
  parse_exception(int lines, const std::string &error): m_lines(lines), m_error(error) {}
  virtual const char *what(void) const throw() { return m_error.c_str(); }
  int lines(void) { return m_lines; }
protected:
  int m_lines;
  std::string m_error;
};

Recipe parse_mealmaster(std::istream &stream);

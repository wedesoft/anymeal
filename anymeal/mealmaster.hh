#pragma once
#include <istream>
#include <exception>
#include <string>
#include "recipe.hh"


class parse_exception: public std::exception
{
public:
  parse_exception(const std::string &error): m_error(error) {}
  virtual const char *what(void) const throw() { return m_error.c_str(); }
protected:
  std::string m_error;
};

Recipe parse_mealmaster(std::istream &stream);

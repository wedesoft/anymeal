#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <recodext.h>
#include <string>
#include "ingredient.hh"
#include "recipe.hh"


class recode_exception: public std::exception
{
public:
  recode_exception(const std::string &error): m_error(error) {}
  virtual const char *what(void) const throw() { return m_error.c_str(); }
protected:
  std::string m_error;
};

class Recoder
{
public:
  Recoder(const char *request_string);
  virtual ~Recoder(void);
  std::string process(std::string &text);
  Ingredient process_ingredient(Ingredient &ingredient);
  Recipe process_recipe(Recipe &recipe);
protected:
  RECODE_OUTER m_outer;
  RECODE_REQUEST m_request;
};

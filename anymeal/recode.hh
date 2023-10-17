/* AnyMeal recipe management software
   Copyright (C) 2020, 2023 Jan Wedekind

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
  virtual ~recode_exception(void) throw() {}
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

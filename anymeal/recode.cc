/* AnyMeal recipe management software
   Copyright (C) 2020, 2025 Jan Wedekind

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
#include <cassert>
#include <sstream>
#include <errno.h>
#include <string.h>
#include "recode.hh"


using namespace std;

Recoder::Recoder(const char *fromcode, const char *tocode) {
  m_request = iconv_open(tocode, fromcode);
  if (m_request == (iconv_t)-1) {
    ostringstream s;
    s << "Cannot fulfill recoding request from " << fromcode << " to " << tocode << ": " << strerror(errno);
    throw recode_exception(s.str());
  };
}

Recoder::~Recoder(void) {
  iconv_close(m_request);
}

string Recoder::process(const std::string &text) {
  char *inbuf = (char *)text.c_str();
  size_t inbytesleft = text.length();
  size_t outbytesleft = 4 * text.length();
  char *outbuf = (char *)malloc(outbytesleft * sizeof(char));
  char *output = outbuf;
  iconv(m_request, NULL, NULL, NULL, NULL); // reset conversion state.
  size_t result = iconv(m_request, &inbuf, &inbytesleft, &outbuf, &outbytesleft);
  if (result == (size_t)-1) {
    free(output);
    ostringstream s;
    s << "Failed to recode string \"" << text << "\": " << strerror(errno);
    throw recode_exception(s.str());
  };
  size_t output_length = outbuf - output;
  string retval(output, output_length);
  free(output);
  return retval;
}

Ingredient Recoder::process_ingredient(Ingredient &ingredient) {
  Ingredient result;
  result.set_amount_integer(ingredient.amount_integer());
  result.set_amount_numerator(ingredient.amount_numerator());
  result.set_amount_denominator(ingredient.amount_denominator());
  result.set_amount_float(ingredient.amount_float());
  result.set_unit(ingredient.unit_c_str());
  result.add_text(process(ingredient.text()).c_str());
  return result;
}

Recipe Recoder::process_recipe(Recipe &recipe) {
  Recipe result;
  result.set_title(process(recipe.title()).c_str());
  for (set<string>::iterator category=recipe.categories().begin(); category!=recipe.categories().end(); category++)
    result.add_category(process(*category).c_str());
  result.set_servings(recipe.servings());
  result.set_servings_unit(process(recipe.servings_unit()).c_str());
  for (vector<Ingredient>::iterator ingredient=recipe.ingredients().begin(); ingredient!=recipe.ingredients().end(); ingredient++)
    result.add_ingredient(process_ingredient(*ingredient));
  for (vector<pair<int, string> >::iterator section=recipe.ingredient_sections().begin(); section!=recipe.ingredient_sections().end(); section++)
    result.add_ingredient_section(section->first, process(section->second).c_str());
  for (vector<string>::iterator instruction=recipe.instructions().begin(); instruction!=recipe.instructions().end(); instruction++)
    result.add_instruction(process(*instruction).c_str());
  for (vector<pair<int, string> >::iterator section=recipe.instruction_sections().begin(); section!=recipe.instruction_sections().end(); section++)
    result.add_instruction_section(section->first, process(section->second).c_str());
  return result;
}

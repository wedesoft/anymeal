/* AnyMeal recipe management software
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
#include <cassert>
#include "recipe.hh"


using namespace std;

void Recipe::add_ingredient_section(int row, const char *title) {
  m_ingredient_sections.push_back(pair<int, string>(row, title));
}

void Recipe::add_instruction_section(int row, const char *title) {
  m_instruction_sections.push_back(pair<int, string>(row, title));
}

void Recipe::append_instruction(const char *instruction) {
  int n = m_instructions.size();
  assert(n > 0);
  if (!m_instructions[n - 1].empty())
    m_instructions[n - 1] += " ";
  m_instructions[n - 1] += instruction;
}

// TODO: scale amounts
// TODO: convert metric <-> imperial units

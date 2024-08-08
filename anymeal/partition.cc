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
#include <sstream>
#include "partition.hh"


using namespace std;

vector<string> recipes(istream &stream, bool *unexpected_eof) {
  vector<string> result;
  ostringstream recipe;
  string line;
  bool on = false;
  while (getline(stream, line)) {
    if (!line.empty() && *line.rbegin() == '\r')
      line.erase(line.length() - 1, 1);
    if ((line.rfind("MMMMM", 0) == 0 || line.rfind("-----", 0) == 0) && line.length() > 5)
      on = true;
    if (on)
      recipe << line << "\r\n";
    if (line == "MMMMM" || line == "-----") {
      result.push_back(recipe.str());
      recipe.str("");
      recipe.clear();
      on = false;
    };
  };
  if (unexpected_eof)
    *unexpected_eof = on;
  return result;
}

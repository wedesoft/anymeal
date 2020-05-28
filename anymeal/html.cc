#include <cassert>
#include <sstream>
#include "html.hh"


using namespace std;

string html_encode(const string &data) {
  // https://stackoverflow.com/questions/5665231/most-efficient-way-to-escape-xml-html-in-c-string
  string result;
  result.reserve(data.size());
  for(size_t pos=0; pos!=data.size(); pos++) {
    switch(data[pos]) {
      case '&':  result.append("&amp;");       break;
      case '\"': result.append("&quot;");      break;
      case '\'': result.append("&apos;");      break;
      case '<':  result.append("&lt;");        break;
      case '>':  result.append("&gt;");        break;
      default:   result.append(&data[pos], 1); break;
    };
  };
  return result;
}

const char *html_unit(const string &unit) {
  if (unit == "x ") return "per serving";
  if (unit == "sm") return "small";
  if (unit == "md") return "medium";
  if (unit == "lg") return "large";
  if (unit == "cn") return "can";
  if (unit == "pk") return "package";
  if (unit == "pn") return "pinch";
  if (unit == "dr") return "drop";
  if (unit == "ds") return "dash";
  if (unit == "ct") return "carton";
  if (unit == "bn") return "bunch";
  if (unit == "sl") return "slice";
  if (unit == "ea") return "each";
  if (unit == "t ") return "teaspoon";
  if (unit == "ts") return "teaspoon";
  if (unit == "T ") return "tablespoon";
  if (unit == "tb") return "tablespoon";
  if (unit == "fl") return "fluid ounce";
  if (unit == "c ") return "cup";
  if (unit == "pt") return "pint";
  if (unit == "qt") return "quart";
  if (unit == "ga") return "gallon";
  if (unit == "oz") return "ounce";
  if (unit == "lb") return "pound";
  if (unit == "ml") return "milliliter";
  if (unit == "cb") return "cubic cm";
  if (unit == "cl") return "centiliter";
  if (unit == "dl") return "deciliter";
  if (unit == "l ") return "liter";
  if (unit == "mg") return "milligram";
  if (unit == "cg") return "centigram";
  if (unit == "dg") return "decigram";
  if (unit == "g ") return "gram";
  if (unit == "kg") return "kilogram";
  assert(unit == "  " || unit == "");
  return "";
}


string recipe_to_html(Recipe &recipe) {
  ostringstream stream;
  stream << "<html>\n";
  stream << "  <head>\n";
  if (!recipe.title().empty()) {
    stream << "    <title>" << html_encode(recipe.title()) << "</title>\n";
  };
  stream << "  </head>\n";
  stream << "  <body>\n";
  if (!recipe.title().empty()) {
    stream << "    <h2>" << html_encode(recipe.title()) << "</h2>\n";
  };
  if (!recipe.categories().empty()) {
    stream << "    <p><b>Categories:</b> ";
    for (auto category=recipe.categories().begin(); category!=recipe.categories().end(); category++) {
      stream << html_encode(*category);
      if (category + 1 != recipe.categories().end())
        stream << ", ";
    };
    stream << "</p>\n";
  };
  if (recipe.servings() > 0) {
    stream << "    <p><b>Yield:</b> " << recipe.servings() << " " << html_encode(recipe.servings_unit()) << "</p>\n";
  };
  if (!recipe.ingredients().empty()) {
    stream << "    <h3>Ingredients</h3>\n";
    stream << "    <table>\n";
    stream << "      <tr>\n";
    stream << "        <th>amount</th>\n";
    stream << "        <th>unit</th>\n";
    stream << "        <th>ingredient</th>\n";
    stream << "      </tr>\n";
    for (int i=0; i<recipe.ingredients().size(); i++) {
      Ingredient ingredient = recipe.ingredients()[i];
      stream << "      <tr>\n";
      stream << "        <td>";
      if (ingredient.amount_float() > 0) {
        stream << ingredient.amount_float();
      } else {
        if (ingredient.amount_integer() > 0)
          stream << ingredient.amount_integer();
        if (ingredient.amount_numerator() > 0) {
          if (ingredient.amount_integer() > 0)
            stream << " ";
          stream << ingredient.amount_numerator() << "/" << ingredient.amount_denominator();
        };
      };
      stream << "</td>\n";
      stream << "        <td>" << html_unit(ingredient.unit()) << "</td>\n";
      stream << "        <td>" << ingredient.text() << "</td>\n";
      stream << "      </tr>\n";
    };
    stream << "    </table>\n";
  };
  if (!recipe.instructions().empty()) {
    stream << "    <h3>Instructions</h3>\n";
    auto section = recipe.instruction_sections().begin();
    for (int i=0; i<recipe.instructions().size(); i++) {
      while (section != recipe.instruction_sections().end() && section->first == i) {
        stream << "    <h4>" << html_encode(section->second) << "</h4>\n";
        section++;
      };
      stream << "    <p>" << html_encode(recipe.instructions()[i]) << "</p>\n";
    };
  };
  stream << "  </body>\n";
  stream << "</html>";
  return stream.str();
}

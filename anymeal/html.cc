#include <cassert>
#include <sstream>
#include "html.hh"


using namespace std;

#define QT_TRANSLATE_NOOP(scope, x) x

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

string html_unit(const string &unit, string (*translate)(const char *, const char *)) {
  const char *result = "";
  if (unit == "x ") result = QT_TRANSLATE_NOOP("units", "per serving");
  if (unit == "sm") result = QT_TRANSLATE_NOOP("units", "small");
  if (unit == "md") result = QT_TRANSLATE_NOOP("units", "medium");
  if (unit == "lg") result = QT_TRANSLATE_NOOP("units", "large");
  if (unit == "cn") result = QT_TRANSLATE_NOOP("units", "can");
  if (unit == "pk") result = QT_TRANSLATE_NOOP("units", "package");
  if (unit == "pn") result = QT_TRANSLATE_NOOP("units", "pinch");
  if (unit == "dr") result = QT_TRANSLATE_NOOP("units", "drop");
  if (unit == "ds") result = QT_TRANSLATE_NOOP("units", "dash");
  if (unit == "ct") result = QT_TRANSLATE_NOOP("units", "carton");
  if (unit == "bn") result = QT_TRANSLATE_NOOP("units", "bunch");
  if (unit == "sl") result = QT_TRANSLATE_NOOP("units", "slice");
  if (unit == "ea") result = QT_TRANSLATE_NOOP("units", "each");
  if (unit == "t ") result = QT_TRANSLATE_NOOP("units", "teaspoon");
  if (unit == "ts") result = QT_TRANSLATE_NOOP("units", "teaspoon");
  if (unit == "T ") result = QT_TRANSLATE_NOOP("units", "tablespoon");
  if (unit == "tb") result = QT_TRANSLATE_NOOP("units", "tablespoon");
  if (unit == "fl") result = QT_TRANSLATE_NOOP("units", "fluid ounce");
  if (unit == "c ") result = QT_TRANSLATE_NOOP("units", "cup");
  if (unit == "pt") result = QT_TRANSLATE_NOOP("units", "pint");
  if (unit == "qt") result = QT_TRANSLATE_NOOP("units", "quart");
  if (unit == "ga") result = QT_TRANSLATE_NOOP("units", "gallon");
  if (unit == "oz") result = QT_TRANSLATE_NOOP("units", "ounce");
  if (unit == "lb") result = QT_TRANSLATE_NOOP("units", "pound");
  if (unit == "ml") result = QT_TRANSLATE_NOOP("units", "milliliter");
  if (unit == "cb") result = QT_TRANSLATE_NOOP("units", "cubic cm");
  if (unit == "cl") result = QT_TRANSLATE_NOOP("units", "centiliter");
  if (unit == "dl") result = QT_TRANSLATE_NOOP("units", "deciliter");
  if (unit == "l ") result = QT_TRANSLATE_NOOP("units", "liter");
  if (unit == "mg") result = QT_TRANSLATE_NOOP("units", "milligram");
  if (unit == "cg") result = QT_TRANSLATE_NOOP("units", "centigram");
  if (unit == "dg") result = QT_TRANSLATE_NOOP("units", "decigram");
  if (unit == "g ") result = QT_TRANSLATE_NOOP("units", "gram");
  if (unit == "kg") result = QT_TRANSLATE_NOOP("units", "kilogram");
  return (*translate)("units", result);
}

std::string notrans(const char *context, const char *text) {
  return text;
}

string recipe_to_html(Recipe &recipe, string (*translate)(const char *, const char *)) {
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
    stream << "    <p><b>" << (*translate)("recipe", QT_TRANSLATE_NOOP("recipe", "Categories")) << ":</b> ";
    for (auto category=recipe.categories().begin(); category!=recipe.categories().end(); category++) {
      stream << html_encode(*category);
      if (category + 1 != recipe.categories().end())
        stream << ", ";
    };
    stream << "</p>\n";
  };
  if (recipe.servings() > 0) {
    stream << "    <p><b>" << (*translate)("recipe", QT_TRANSLATE_NOOP("recipe", "Yield")) << ":</b> " << recipe.servings()
           << " " << html_encode(recipe.servings_unit()) << "</p>\n";
  };
  if (!recipe.ingredients().empty()) {
    stream << "    <h3>" << (*translate)("recipe", QT_TRANSLATE_NOOP("recipe", "Ingredients")) << "</h3>\n";
    stream << "    <table>\n";
    stream << "      <tr>\n";
    stream << "        <th>" << (*translate)("recipe", QT_TRANSLATE_NOOP("recipe", "amount")) << "</th>\n";
    stream << "        <th>" << (*translate)("recipe", QT_TRANSLATE_NOOP("recipe", "unit")) << "</th>\n";
    stream << "        <th>" << (*translate)("recipe", QT_TRANSLATE_NOOP("recipe", "ingredient")) << "</th>\n";
    stream << "      </tr>\n";
    auto section = recipe.ingredient_sections().begin();
    for (int i=0; i<recipe.ingredients().size(); i++) {
      while (section != recipe.ingredient_sections().end() && section->first == i) {
        stream << "      <tr>\n";
        stream << "        <td colspan=\"3\"><em>" << html_encode(section->second) << "</em></td>\n";
        stream << "      </tr>\n";
        section++;
      };
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
      stream << "        <td>" << html_unit(ingredient.unit(), translate) << "</td>\n";
      stream << "        <td>" << ingredient.text() << "</td>\n";
      stream << "      </tr>\n";
    };
    stream << "    </table>\n";
  };
  if (!recipe.instructions().empty()) {
    stream << "    <h3>" << (*translate)("recipe", QT_TRANSLATE_NOOP("recipe", "Instructions")) << "</h3>\n";
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

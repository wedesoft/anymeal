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

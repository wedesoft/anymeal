#include "recipe.hh"


using namespace std;

void Recipe::add_ingredient_section(int row, const char *title) {
  m_ingredient_sections.push_back(pair<int, string>(row, title));
}

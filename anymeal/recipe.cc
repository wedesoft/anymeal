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

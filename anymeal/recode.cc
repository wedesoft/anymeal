#include <cassert>
#include <sstream>
#include "recode.hh"


using namespace std;

Recoder::Recoder(const char *request_string) {
  m_outer = recode_new_outer(false);
  assert(m_outer);
  m_request = recode_new_request(m_outer);
  assert(m_request);
  bool result = recode_scan_request(m_request, request_string);
  if (!result) {
    recode_delete_request(m_request);
    recode_delete_outer(m_outer);
    ostringstream s;
    s << "Cannot fulfill recoding request \"" << request_string << "\".";
    throw recode_exception(s.str());
  };
}

Recoder::~Recoder(void) {
  recode_delete_request(m_request);
  recode_delete_outer(m_outer);
}

string Recoder::process(std::string &text) {
  char *output = nullptr;
  size_t output_length = 0;
  size_t output_allocated = 0;
  bool ok = recode_buffer_to_buffer(m_request, text.c_str(), text.length(), &output, &output_length, &output_allocated);
  if (!ok) {
    free(output);
    ostringstream s;
    s << "Failed to recode string \"" << text << "\".";
    throw recode_exception(s.str());
  };
  string result(output, output_length);
  free(output);
  return result;
}

Ingredient Recoder::process_ingredient(Ingredient &ingredient) {
  Ingredient result;
  result.set_amount_integer(ingredient.amount_integer());
  result.set_amount_numerator(ingredient.amount_numerator());
  result.set_amount_denominator(ingredient.amount_denominator());
  result.set_amount_float(ingredient.amount_float());
  result.set_unit(ingredient.unit().c_str());
  result.add_text(process(ingredient.text()).c_str());
  return result;
}

Recipe Recoder::process_recipe(Recipe &recipe) {
  Recipe result;
  result.set_title(process(recipe.title()).c_str());
  for (auto category=recipe.categories().begin(); category!=recipe.categories().end(); category++)
    result.add_category(process(*category).c_str());
  result.set_servings(recipe.servings());
  result.set_servings_unit(process(recipe.servings_unit()).c_str());
  for (auto ingredient=recipe.ingredients().begin(); ingredient!=recipe.ingredients().end(); ingredient++)
    result.add_ingredient(process_ingredient(*ingredient));
  for (auto section=recipe.ingredient_sections().begin(); section!=recipe.ingredient_sections().end(); section++)
    result.add_ingredient_section(section->first, process(section->second).c_str());
  for (auto instruction=recipe.instructions().begin(); instruction!=recipe.instructions().end(); instruction++)
    result.add_instruction(process(*instruction).c_str());
  for (auto section=recipe.instruction_sections().begin(); section!=recipe.instruction_sections().end(); section++)
    result.add_instruction_section(section->first, process(section->second).c_str());
  return result;
}

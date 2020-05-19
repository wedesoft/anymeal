#include <cassert>
#include "recode.hh"


using namespace std;

Recoder::Recoder(const char *request_string) {
  m_outer = recode_new_outer(false);
  assert(m_outer);
  m_request = recode_new_request(m_outer);
  bool result = recode_scan_request(m_request, request_string);
  assert(result);
}

Recoder::~Recoder(void) {
  recode_delete_request(m_request);
  recode_delete_outer(m_outer);
}

string Recoder::process(std::string &text) {
  char *output = NULL;
  size_t output_length = 0;
  size_t output_allocated = 0;
  bool result = recode_buffer_to_buffer(m_request, text.c_str(), text.length(), &output, &output_length, &output_allocated);
  if (!result)
    throw recode_exception();
  return string(output, output_length);
}

Ingredient Recoder::process_ingredient(Ingredient &ingredient) {
  Ingredient result;
  result.set_amount_type(ingredient.amount_type());
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
  for (auto category=recipe.categories().begin(); category != recipe.categories().end(); category++)
    result.add_category(process(*category).c_str());
  return result;
}

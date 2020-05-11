#pragma once
#include <string>


typedef enum { AMOUNT_NONE, AMOUNT_INTEGER } amount_type_t;

class Ingredient
{
public:
  Ingredient(void);
  amount_type_t amount_type(void) { return m_amount_type; }
  void set_amount_type(amount_type_t amount_type) { m_amount_type = amount_type; }
  int amount_integer(void) { return m_amount_integer; }
  void set_amount_integer(int amount_integer) { m_amount_integer = amount_integer; }
  std::string unit(void) { return m_unit; }
  void set_unit(const char *unit) { m_unit = unit; }
  std::string text(void) { return m_text; }
  void add_text(const char *text) { m_text += text; }
  std::string preparation(void) { return m_preparation; }
  void add_preparation(const char *preparation) { m_preparation += preparation; }
protected:
  amount_type_t m_amount_type;
  int m_amount_integer;
  std::string m_unit;
  std::string m_text;
  std::string m_preparation;
};

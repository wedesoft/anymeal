#pragma once


typedef enum { AMOUNT_NONE, AMOUNT_INTEGER } amount_type_t;

class Ingredient
{
public:
  Ingredient(void);
  amount_type_t amount_type(void) { return m_amount_type; }
  void set_amount_type(amount_type_t amount_type) { m_amount_type = amount_type; }
  int amount_integer(void) { return m_amount_integer; }
  void set_amount_integer(int amount_integer) { m_amount_integer = amount_integer; }
protected:
  amount_type_t m_amount_type;
  int m_amount_integer;
};

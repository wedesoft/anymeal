#include <sstream>
#include "export.hh"


using namespace std;

string recipe_to_mealmaster(Recipe &recipe) {
  ostringstream result;
  result << "MMMMM----------------Meal-Master recipe exported by AnyMeal-----------------\r\n";
  result << "     Title: " << recipe.title() << "\r\n";
  result << "Categories: ";
  for (auto category=recipe.categories().begin(); category!=recipe.categories().end(); category++) {
    result << *category;
    if (category + 1 != recipe.categories().end())
      result << ", ";
  };
  result << "\r\n";
  result << "     Yield: " << recipe.servings() << " " << recipe.servings_unit() << "\r\n";
  result << "\r\n";
  if (!recipe.ingredients().empty()) {
    for (int i=0; i<recipe.ingredients().size(); i++) {
      // TODO: ingredient sections
      Ingredient ingredient = recipe.ingredients()[i];
      string amount;
      if (ingredient.amount_float() > 0) {
        ostringstream s;
        s << ingredient.amount_float();
        amount = s.str();
      } else if (ingredient.amount_integer() > 0) {
        ostringstream s;
        s << ingredient.amount_integer();
        if (ingredient.amount_numerator() > 0) {
          s << " " << ingredient.amount_numerator() << "/" << ingredient.amount_denominator();
        };
        amount = s.str();
      } else if (ingredient.amount_numerator() > 0) {
        ostringstream s;
        s << ingredient.amount_numerator() << "/" << ingredient.amount_denominator();
        amount = s.str();
      };
      amount = amount.substr(0, 7);
      result << string(7 - amount.length(), ' ') << amount;
      result << " ";
      if (ingredient.unit().empty()) {
        result << "  ";
      } else {
        result << ingredient.unit();
      };
      result << " ";
      string txt = ingredient.text();
      while (txt.length() > 0) {
        if (txt.length() <= 28) {
          result << txt << "\r\n";
          txt = "";
        } else {
          int offset;
          size_t pos = txt.rfind(" ", 28);// TODO: emergency break if there is no spaces
          if (pos == string::npos) {
            pos = 28;
            offset = 0;
          } else
            offset = 1;
          result << txt.substr(0, pos) << "\r\n"
                 << "           -";
          txt = txt.substr(pos + offset, txt.length() - pos - offset);
        };
      };
    };
    result << "\r\n";
  };
  // TODO: instructions
  // TODO: instruction sections
  result << "MMMMM";
  return result.str();
}

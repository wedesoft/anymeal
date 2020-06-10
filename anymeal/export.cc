#include <sstream>
#include "export.hh"


using namespace std;

string recipe_to_mealmaster(Recipe &recipe) {
  ostringstream result;
  // Output recipe header.
  result << "MMMMM----------------Meal-Master recipe exported by AnyMeal-----------------\r\n"
         << "     Title: " << recipe.title() << "\r\n"
         << "Categories: ";
  for (auto category=recipe.categories().begin(); category!=recipe.categories().end(); category++) {
    result << *category;
    if (category + 1 != recipe.categories().end())
      result << ", ";
  };
  result << "\r\n"
         << "     Yield: " << recipe.servings() << " " << recipe.servings_unit() << "\r\n"
         << "\r\n";
  if (!recipe.ingredients().empty()) {
    // Output ingredients.
    auto sections = recipe.ingredient_sections();
    auto section = sections.begin();
    for (int i=0; i<recipe.ingredients().size(); i++) {
      while (section != sections.end() && section->first == i) {
        // Output an ingredient section.
        int n = 71 - section->second.length();
        result << "MMMMM" << string(n / 2, '-') << section->second << string((n + 1) / 2, '-') << "\r\n";
        section++;
      };
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
      // Break up ingredient text using continuation lines if necessary.
      while (!txt.empty()) {
        if (txt.length() <= 28) {
          result << txt << "\r\n";
          txt = "";
        } else {
          int offset;
          // break at space character.
          size_t pos = txt.rfind(" ", 28);
          if (pos == string::npos) {
            // emergency line break if no space character found.
            pos = 28;
            // ensure potential UTF8 sequences are not broken up.
            while (pos > 1 && (txt.at(pos) & 0xC0) == 0x80)
              pos--;
            offset = 0;
          } else
            offset = 1;
          result << txt.substr(0, pos) << "\r\n"
                 << "           -";
          txt = txt.substr(pos + offset, txt.length() - pos - offset);
        };
      };
      if (section != sections.end() && section->first == i + 1) {
        // Newline before next ingredient section.
        result << "\r\n";
      };
    };
    result << "\r\n";
  };
  if (!recipe.instructions().empty()) {
    // Output recipe instructions.
    for (int i=0; i<recipe.instructions().size(); i++) {
      string txt = recipe.instructions()[i];
      if (txt.empty()) {
        result << "\r\n";
      } else {
        while (!txt.empty()) {
          if (txt.length() <= 75) {
            result << "  " << txt << "\r\n";
            txt = "";
          } else {
            // break at space character.
            int offset;
            size_t pos = txt.rfind(" ", 75);
            if (pos == string::npos) {
              // emergency line break if no space character found.
              pos = 75;
              // ensure potential UTF8 sequences are not broken up.
              while (pos > 1 && (txt.at(pos) & 0xC0) == 0x80)
                pos--;
              offset = 0;
            } else
              offset = 1;
            result << "  " << txt.substr(0, pos) << "\r\n";
            txt = txt.substr(pos + offset, txt.length() - pos - offset);
          };
        };
      };
    };
    result << "\r\n";
  };
  // TODO: emergency line break
  // TODO: preserve UTF-8 sequences
  // TODO: instruction sections
  // TODO: forced newlines
  result << "MMMMM";
  return result.str();
}

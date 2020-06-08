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
  result << "MMMMM";
  return result.str();
}

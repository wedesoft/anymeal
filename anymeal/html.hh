#pragma once
#include <string>
#include "recipe.hh"


std::string notrans(const char *context, const char *text);

std::string html_amount(Ingredient &ingredient);

std::string html_unit(const std::string &unit, std::string (*translate)(const char *, const char *));

std::string recipe_to_html(Recipe &recipe, std::string (*translate)(const char *, const char *)=&notrans);

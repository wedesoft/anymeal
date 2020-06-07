#pragma once
#include <string>
#include "recipe.hh"


std::string notrans(const char *context, const char *text);

std::string recipe_to_html(Recipe &recipe, std::string (*translate)(const char *, const char *)=&notrans);

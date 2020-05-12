%{
#include "mealmaster.hh"

std::istream *yystream;
Ingredient ingredient_;
Recipe recipe;

extern int yylex(void);

#define YY_INPUT(buffer, result, max_size) { \
  yystream->read(buffer, max_size); \
  result = yystream->gcount(); \
}

Recipe parse_mealmaster(std::istream &stream) {
  recipe = Recipe();
  yystream = &stream;
  yylex();
  yyrestart(NULL);
  yy_start = 1;
  return recipe;
}

%}

%option noyywrap
%option never-interactive
%option nostdinit

%x title titletext categories categoriestext servings servingsamount servingsunit ingredient unit1 unit2 unit3 ingredienttext
%x amount amount2 fraction ingredientcont

UNIT "x "|"sm"|"md"|"lg"|"cn"|"pk"|"pn"|"dr"|"ds"|"ct"|"bn"|"sl"|"ea"|"t "|"ts"|"T "|"tb"|"fl"|"c "|"pt"|"qt"|"ga"|"oz"|"lb"|"ml"|"cb"|"cl"|"dl"|"l "|"mg"|"cg"|"dg"|"g "|"kg"|"  "

%%

<INITIAL>(MMMMM|-----)[^\r\n]*[Mm][Ee][Aa][Ll]-[Mm][Aa][Ss][Tt][Ee][Rr][^\r\n]*\r?\n {
  BEGIN(title);
}

<INITIAL>\r?\n

<INITIAL>.

<title>[\ \t]
<title>\r?\n
<title>"Title: " {
  BEGIN(titletext);
}

<titletext>[^\r\n]* {
  recipe.set_title(yytext);
}
<titletext>\r?\n {
  BEGIN(categories);
}

<categories>[\ \t]
<categories>"Categories: " {
  BEGIN(categoriestext);
}

<categoriestext>[^\r\n,]* {
  recipe.add_category(yytext);
}
<categoriestext>,\ *
<categoriestext>\r?\n {
  BEGIN(servings);
}

<servings>[\ \t]
<servings>"Yield: "|"Servings: " {
  BEGIN(servingsamount);
}

<servingsamount>[0-9]+ {
  recipe.set_servings(atoi(yytext));
  BEGIN(servingsunit);
}

<servingsunit>[\ \t]
<servingsunit>[^\ \t\r\n][^\r\n]* {
  recipe.set_servings_unit(yytext);
}
<servingsunit>\r?\n {
  ingredient_ = Ingredient();
  BEGIN(ingredient);
}

<ingredient>\r?\n

<ingredient>\ {0,6}[0-9]+ {
  ingredient_.set_amount_type(AMOUNT_RATIONAL);
  ingredient_.set_amount_integer(atoi(yytext));
  if (yyleng < 7) {
    BEGIN(amount);
  } else {
    BEGIN(unit1);
  };
}

<ingredient>\ {0,6}[0-9]*\.[0-9]* {
  ingredient_.set_amount_type(AMOUNT_FLOAT);
  ingredient_.set_amount_float(atof(yytext));
  BEGIN(unit1);
}

<ingredient>\ {7} {
  BEGIN(unit1);
}

<ingredient>\ {11}- {
  BEGIN(ingredientcont);
}

<amount>\/ {
  ingredient_.set_amount_numerator(ingredient_.amount_integer());
  ingredient_.set_amount_integer(0);
  BEGIN(fraction);
}

<amount>\ [0-9]+ {
  ingredient_.set_amount_numerator(atoi(yytext));
  BEGIN(amount2);
}

<amount2>\/ {
  BEGIN(fraction);
}

<fraction>[0-9]+ {
  ingredient_.set_amount_denominator(atoi(yytext));
  BEGIN(unit1);
}

<unit1>\  {
  BEGIN(unit2);
}

<unit2>{UNIT} {
  ingredient_.set_unit(yytext);
  BEGIN(unit3);
}

<unit3>\  {
  BEGIN(ingredienttext);
}

<ingredienttext>[^\r\n]* {
  ingredient_.add_text(yytext);
}
<ingredienttext>\r?\n {
  recipe.add_ingredient(ingredient_);
  BEGIN(ingredient);
}

<ingredientcont>[^\r\n]* {
  int n = recipe.ingredients().size();
  recipe.ingredients()[n - 1].add_text(" ");
  recipe.ingredients()[n - 1].add_text(yytext);
}
<ingredientcont>\r?\n {
  BEGIN(ingredient);
}

<*>(MMMMM|-----)\r?\n {
  BEGIN(INITIAL);
  return 0;
}

%%

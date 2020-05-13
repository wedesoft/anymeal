%{
#include "mealmaster.hh"

std::istream *yystream;
Ingredient ingredient_;
Recipe recipe;
std::string buffer;

extern int yylex(void);

int line_no;

#define YY_INPUT(buffer, result, max_size) { \
  yystream->read(buffer, max_size); \
  result = yystream->gcount(); \
}

Recipe parse_mealmaster(std::istream &stream) {
  recipe = Recipe();
  yystream = &stream;
  line_no = 1;
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
%x amount amount2 fraction ingredientcont instructions

UNIT "x "|"sm"|"md"|"lg"|"cn"|"pk"|"pn"|"dr"|"ds"|"ct"|"bn"|"sl"|"ea"|"t "|"ts"|"T "|"tb"|"fl"|"c "|"pt"|"qt"|"ga"|"oz"|"lb"|"ml"|"cb"|"cl"|"dl"|"l "|"mg"|"cg"|"dg"|"g "|"kg"|"  "

%%

<INITIAL>(MMMMM|-----)[^\r\n]*[Mm][Ee][Aa][Ll]-[Mm][Aa][Ss][Tt][Ee][Rr][^\r\n]*\r?\n {
  line_no++;
  BEGIN(title);
}

<INITIAL>\r?\n {
  line_no++;
}

<INITIAL>.

<title>[\ \t]
<title>\r?\n {
  line_no++;
}
<title>"Title: " {
  BEGIN(titletext);
}

<titletext>[^\r\n]* {
  recipe.set_title(yytext);
}
<titletext>\r?\n {
  line_no++;
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
  line_no++;
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
  line_no++;
  ingredient_ = Ingredient();
  buffer.clear();
  BEGIN(ingredient);
}

<ingredient>\r?\n {
  line_no++;
}
<ingredient>\ {0,6}[0-9]+ {
  buffer += yytext;
  ingredient_.set_amount_type(AMOUNT_RATIONAL);
  ingredient_.set_amount_integer(atoi(yytext));
  if (yyleng < 7) {
    BEGIN(amount);
  } else {
    BEGIN(unit1);
  };
}
<ingredient>\ {0,6}[0-9]*\.[0-9]* {
  buffer += yytext;
  ingredient_.set_amount_type(AMOUNT_FLOAT);
  ingredient_.set_amount_float(atof(yytext));
  BEGIN(unit1);
}
<ingredient>\ {7} {
  buffer += yytext;
  BEGIN(unit1);
}
<ingredient>\ {11}- {
  buffer += yytext;
  int n = recipe.ingredients().size();
  if (n) {
    BEGIN(ingredientcont);
  } else {
    BEGIN(instructions);
  };
}
<ingredient>. {
  unput(*yytext);
  BEGIN(instructions);
}

<amount>\/ {
  buffer += yytext;
  ingredient_.set_amount_numerator(ingredient_.amount_integer());
  ingredient_.set_amount_integer(0);
  BEGIN(fraction);
}
<amount>\ [0-9]+ {
  buffer += yytext;
  ingredient_.set_amount_numerator(atoi(yytext));
  BEGIN(amount2);
}
<amount>. {
  unput(*yytext);
  BEGIN(instructions);
}

<amount2>\/ {
  buffer += yytext;
  BEGIN(fraction);
}
<amount2>[^\/] {
  unput(*yytext);
  BEGIN(instructions);
}

<fraction>[0-9]+ {
  buffer += yytext;
  ingredient_.set_amount_denominator(atoi(yytext));
  BEGIN(unit1);
}
<fraction>[^0-9] {
  unput(*yytext);
  BEGIN(instructions);
}

<unit1>\  {
  buffer += yytext;
  BEGIN(unit2);
}
<unit1>[^ ] {
  unput(*yytext);
  BEGIN(instructions);
}

<unit2>{UNIT} {
  buffer += yytext;
  ingredient_.set_unit(yytext);
  BEGIN(unit3);
}
<unit2>. {
  unput(*yytext);
  BEGIN(instructions);
}

<unit3>\  {
  BEGIN(ingredienttext);
}
<unit3>[^ ] {
  unput(*yytext);
  BEGIN(instructions);
}

<ingredienttext>[^\r\n]* {
  ingredient_.add_text(yytext);
}
<ingredienttext>\r?\n {
  line_no++;
  recipe.add_ingredient(ingredient_);
  ingredient_ = Ingredient();
  buffer.clear();
  BEGIN(ingredient);
}

<ingredientcont>[^\r\n]* {
  int n = recipe.ingredients().size();
  recipe.ingredients()[n - 1].add_text(" ");
  recipe.ingredients()[n - 1].add_text(yytext);
}
<ingredientcont>\r?\n {
  line_no++;
  ingredient_ = Ingredient();
  buffer.clear();
  BEGIN(ingredient);
}

<instructions>[^\r\n]* {
  buffer += yytext;
}
<instructions>\r?\n {
  recipe.add_instruction(buffer.c_str());
  buffer.clear();
  line_no++;
}

<*>(MMMMM|-----)\r?\n {
  line_no++;
  BEGIN(INITIAL);
  return 0;
}

<*>. {
  fprintf(stderr, "Problem in state %d with character '%c' (0x%x) while parsing line %d\n", YY_START, *yytext, (int)*yytext, line_no);
}
%%

%{
#include <sstream>
#include "mealmaster.hh"

std::istream *yystream;
Ingredient ingredient_;
Recipe recipe;
int newlines;
std::string buffer;
std::string section;
std::ostringstream error_message;

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
  error_message.str("");
  error_message.clear();
  newlines = 0;
  int result = yylex();
  yyrestart(NULL);
  yy_start = 1;
  if (result)
    throw parse_exception(line_no, error_message.str());
  return recipe;
}

%}

%option noyywrap
%option never-interactive
%option nostdinit

%x title error titletext categories categoriestext servings servingsamount servingsunit body unit1 unit2 unit3 ingredienttext
%x amount amount2 fraction ingredientcont instruction sectionheader instructionstext

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
  BEGIN(body);
}

<body>\ *\r?\n {
  line_no++;
  if (!recipe.instructions().empty())
    newlines++;
}
<body>\ {0,6}[0-9]+ {
  buffer += yytext;
  ingredient_.set_amount_type(AMOUNT_RATIONAL);
  ingredient_.set_amount_integer(atoi(yytext));
  BEGIN(amount);
}
<body>\ {0,6}[0-9]*\.[0-9]* {
  buffer += yytext;
  ingredient_.set_amount_type(AMOUNT_FLOAT);
  ingredient_.set_amount_float(atof(yytext));
  BEGIN(unit1);
}
<body>\ {7} {
  buffer += yytext;
  BEGIN(unit1);
}
<body>\ {11}- {
  buffer += yytext;
  if (!recipe.ingredients().empty()) {
    BEGIN(ingredientcont);
  } else {
    error_message << "Unexpected ingredient continuation in line " << line_no;
    BEGIN(error);
  };
}
<body>(MMMMM|-----)-+\ * {
  section.clear();
  BEGIN(sectionheader);
}
<body>[^\r\n] {
  unput(*yytext);
  BEGIN(instructionstext);
}
<body>(MMMMM|-----)\r?\n {
  line_no++;
  BEGIN(INITIAL);
  return 0;
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
<amount>\r?\n {
  unput('\n');
  BEGIN(instructionstext);
}
<amount>. {
  unput(*yytext);
  BEGIN(unit1);
}

<amount2>\/ {
  buffer += yytext;
  BEGIN(fraction);
}
<amount2>[^\/] {
  unput(*yytext);
  BEGIN(instructionstext);
}

<fraction>[0-9]+ {
  buffer += yytext;
  ingredient_.set_amount_denominator(atoi(yytext));
  BEGIN(unit1);
}
<fraction>[^0-9] {
  unput(*yytext);
  BEGIN(instructionstext);
}

<unit1>\  {
  buffer += yytext;
  BEGIN(unit2);
}
<unit1>[^ ] {
  unput(*yytext);
  BEGIN(instructionstext);
}

<unit2>{UNIT} {
  buffer += yytext;
  ingredient_.set_unit(yytext);
  BEGIN(unit3);
}
<unit2>. {
  unput(*yytext);
  BEGIN(instructionstext);
}

<unit3>\  {
  BEGIN(ingredienttext);
}
<unit3>[^ ] {
  unput(*yytext);
  BEGIN(instructionstext);
}

<ingredienttext>[^\r\n]* {
  ingredient_.add_text(yytext);
}
<ingredienttext>\r?\n {
  line_no++;
  if (recipe.instructions().empty()) {
    recipe.add_ingredient(ingredient_);
    BEGIN(body);
  } else {
    error_message << "Stray ingredient in line " << line_no;
    BEGIN(error);
  };
  ingredient_ = Ingredient();
  buffer.clear();
}

<ingredientcont>[^\r\n]* {
  recipe.ingredients().back().add_text(" ");
  recipe.ingredients().back().add_text(yytext);
}
<ingredientcont>\r?\n {
  line_no++;
  ingredient_ = Ingredient();
  buffer.clear();
  BEGIN(body);
}

<sectionheader>\ *-*\r?\n {
  line_no++;
  if (recipe.instructions().empty())
    recipe.add_ingredient_section(recipe.ingredients().size(), section.c_str());
  else {
    recipe.add_instruction_section(recipe.instructions().size(), section.c_str());
    recipe.add_instruction("");
  };
  newlines = 0;
  ingredient_ = Ingredient();
  buffer.clear();
  BEGIN(body);
}
<sectionheader>[^- ]* {
  section += yytext;
}
<sectionheader>[- ] {
  section += yytext;
}

<instructionstext>[^\r\n]* {
  buffer += yytext;
}
<instructionstext>\r?\n {
  line_no++;
  if (!buffer.empty() && buffer[0] == ' ')
    buffer = buffer.substr(1, buffer.length() - 1);
  if (!buffer.empty() && buffer[0] == ' ')
    buffer = buffer.substr(1, buffer.length() - 1);
  bool force_newline;
  if (!buffer.empty() && buffer[0] == ':') {
    force_newline = true;
    buffer = buffer.substr(1, buffer.length() - 1);
  } else
    force_newline = false;
  if (newlines >= 1) {
    recipe.add_instruction("");
    recipe.add_instruction(buffer.c_str());
  } else
    if (recipe.instructions().size() && !force_newline)
      recipe.append_instruction(buffer.c_str());
    else
      recipe.add_instruction(buffer.c_str());
  if (!recipe.ingredient_sections().empty()) {
    auto section = recipe.ingredient_sections().back();
    if (section.first == recipe.ingredients().size()) {
      recipe.ingredient_sections().pop_back();
      recipe.instruction_sections().push_back(section);
    };
  };
  newlines = 0;
  ingredient_ = Ingredient();
  buffer.clear();
  BEGIN(body);
}

<error>.
<error>\r?\n {
  line_no++;
}

<*>. {
  error_message << "Problem in state " << YY_START << " and line " << line_no << ": unexpected character '" << *yytext << "'";
  BEGIN(error);
}

<*>\r?\n {
  error_message << "Problem in state " << YY_START << " and line " << line_no << ": unexpected newline";
  BEGIN(error);
}

<*><<EOF>> {
  if (error_message.str().empty())
    error_message << "Unexpected end of file";
  return 1;
}
%%

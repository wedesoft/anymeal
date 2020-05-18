%{
#include <ios>
#include <sstream>
#include "mealmaster.hh"

// ftp://ftp.gnu.org/old-gnu/Manuals/flex-2.5.4/html_mono/flex.html
std::istream *yystream;
Ingredient ingredient_;
std::vector<Ingredient> right_column;
Recipe recipe;
int newlines;
std::string buffer;
std::string section;
std::ostringstream error_message;
int ingredient_column;

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
  ingredient_column = 0;
  right_column.clear();
  int result = yylex();
  yyrestart(NULL);
  yy_start = 1;
  if (result)
    throw parse_exception(line_no, error_message.str());
  return recipe;
}

void flush_right_column(void) {
  for (auto i=right_column.begin(); i!=right_column.end(); i++) {
    recipe.add_ingredient(*i);
  };
  right_column.clear();
}

%}

%option noyywrap
%option never-interactive
%option nostdinit

%x title error titletext categories categoriestext servings servingsamount servingsunit body unit1 unit2 unit3 ingredienttext
%x amount amount2 fraction ingredientcont sectionheader instructionstext

UNIT "x "|"sm"|"md"|"lg"|"cn"|"pk"|"pn"|"dr"|"ds"|"ct"|"bn"|"sl"|"ea"|"t "|"ts"|"T "|"tb"|"fl"|"c "|"pt"|"qt"|"ga"|"oz"|"lb"|"ml"|"cb"|"cl"|"dl"|"l "|"mg"|"cg"|"dg"|"g "|"kg"|"  "

CHAR [ -\xFF]
NOCOMMA [ -+\--\xFF]
NOSPACE [!-\xFF]
NOSPACEMINUS [!-,\.-\xFF]
NOSLASH [ -\.0-\xFF]

%%

<INITIAL>(MMMMM|-----)[^\r\n]*[Mm][Ee][Aa][Ll]-[Mm][Aa][Ss][Tt][Ee][Rr][^\r\n]*\r?\n {
  line_no++;
  BEGIN(title);
}

<INITIAL>\r?\n {
  line_no++;
}

<INITIAL>.

<title>" "
<title>\r?\n {
  line_no++;
}
<title>"Title: " {
  BEGIN(titletext);
}

<titletext>{CHAR}* {
  recipe.set_title(yytext);
}
<titletext>\r?\n {
  line_no++;
  BEGIN(categories);
}

<categories>" "
<categories>"Categories: " {
  BEGIN(categoriestext);
}

<categoriestext>{NOCOMMA}* {
  recipe.add_category(yytext);
}
<categoriestext>,\ *
<categoriestext>\r?\n {
  line_no++;
  BEGIN(servings);
}

<servings>" "
<servings>"Yield: "|"Servings: " {
  BEGIN(servingsamount);
}

<servingsamount>[0-9]+ {
  recipe.set_servings(atoi(yytext));
  BEGIN(servingsunit);
}

<servingsunit>" "
<servingsunit>{NOSPACE}{CHAR}* {
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
  flush_right_column();
  BEGIN(sectionheader);
}
<body>{CHAR} {
  unput(*yytext);
  BEGIN(instructionstext);
}
<body>(MMMMM|-----)\r?\n {
  line_no++;
  flush_right_column();
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
<amount>{CHAR} {
  unput(*yytext);
  BEGIN(unit1);
}

<amount2>\/ {
  buffer += yytext;
  BEGIN(fraction);
}
<amount2>{NOSLASH} {
  unput(*yytext);
  BEGIN(instructionstext);
}
<amount2>\r?\n {
  unput('\n');
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
<fraction>\r?\n {
  unput('\n');
  BEGIN(instructionstext);
}

<unit1>" " {
  buffer += yytext;
  BEGIN(unit2);
}
<unit1>[^ ] {
  unput(*yytext);
  BEGIN(instructionstext);
}
<unit1>\r?\n {
  unput('\n');
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
<unit2>\r?\n {
  unput('\n');
  BEGIN(instructionstext);
}

<unit3>" " {
  buffer += yytext;
  if (buffer.length() == 11) {
    BEGIN(ingredienttext);
  } else {
    BEGIN(instructionstext);
  };
}
<unit3>[^ ] {
  unput(*yytext);
  BEGIN(instructionstext);
}
<unit3>\r?\n {
  unput('\n');
  BEGIN(instructionstext);
}

<ingredienttext>{NOSPACE}* {
  buffer += yytext;
  ingredient_.add_text(yytext);
}
<ingredienttext>" " {
  buffer += yytext;
  if (buffer.length() == 41) {
    while (!ingredient_.text().empty() && ingredient_.text().back() == ' ')
      ingredient_.text() = ingredient_.text().substr(0, ingredient_.text().length() - 1);
    recipe.add_ingredient(ingredient_);
    ingredient_ = Ingredient();
    ingredient_column = 1;
    buffer.clear();
    BEGIN(body);
  } else
    ingredient_.add_text(yytext);
}
<ingredienttext>\r?\n {
  line_no++;
  if (recipe.instructions().empty()) {
    if (ingredient_column)
      right_column.push_back(ingredient_);
    else
      recipe.add_ingredient(ingredient_);
    BEGIN(body);
  } else {
    error_message << "Stray ingredient in line " << line_no;
    BEGIN(error);
  };
  ingredient_column = 0;
  ingredient_ = Ingredient();
  buffer.clear();
}

<ingredientcont>{CHAR}* {
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
  // Add a section to the ingredients or to the instructions.
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
<sectionheader>{NOSPACEMINUS}* {
  section += yytext;
}
<sectionheader>[- ] {
  section += yytext;
}

<instructionstext>{CHAR}* {
  buffer += yytext;
}
<instructionstext>\r?\n {
  line_no++;
  if (ingredient_column) {
    // Overlong ingredient line.
    recipe.ingredients().back().add_text(" ");
    recipe.ingredients().back().add_text(buffer.c_str());
  } else {
    // Remove up to two leading spaces.
    for (int i=0; i<2; i++) {
      if (!buffer.empty() && buffer[0] == ' ')
        buffer = buffer.substr(1, buffer.length() - 1);
    };
    // Remove trailing spaces.
    while (!buffer.empty() && buffer.back() == ' ')
      buffer = buffer.substr(0, buffer.length() - 1);
    bool force_newline;
    // A colon forces a new line.
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
      // If there is a section at the end of the ingredients, it needs to be moved into the list of instruction sections.
      if (section.first == recipe.ingredients().size()) {
        recipe.ingredient_sections().pop_back();
        recipe.instruction_sections().push_back(section);
      };
    };
  };
  newlines = 0;
  ingredient_column = 0;
  ingredient_ = Ingredient();
  buffer.clear();
  BEGIN(body);
}

<error>.
<error>\r?\n {
  line_no++;
}

<*>. {
  error_message << "Problem in state " << YY_START << " and line " << line_no << ": unexpected character ";
  if (*yytext < ' ')
    error_message << "0x" << std::hex << (int)*yytext << std::dec;
  else
    error_message << "'" << *yytext << "'";
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

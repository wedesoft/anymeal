%{
#include "mealmaster.hh"

std::istream *yystream;
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
  return recipe;
}

%}

%option noyywrap
%option stack
%option never-interactive
%option nostdinit

%x title titletext

%%

<INITIAL>(MMMMM|-----)[^\r\n]*[Mm][Ee][Aa][Ll]-[Mm][Aa][Ss][Tt][Ee][Rr][^\r\n]*\r?\n {
  BEGIN(title);
}

<INITIAL>\r?\n

<title>[\ \t]
<title>\r?\n
<title>"Title: " {
  BEGIN(titletext);
}

<titletext>[^\r\n]* {
  recipe.set_title(yytext);
  BEGIN(INITIAL);
}

<*>(MMMMM|-----)\r?\n {
  BEGIN(INITIAL);
  return 0;
}

%%

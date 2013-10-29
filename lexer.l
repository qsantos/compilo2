%{
#define YY_NO_INPUT
int fileno(FILE *stream);
int yywrap(void) { return 1; }

#include "ast.h"
#include "parser.h"
%}

%option nounput

%%
[0-9]+        { yylval.num = atoi(yytext); return integer; }
[+\-*/%(){}]  { return yytext[0]; }
%%

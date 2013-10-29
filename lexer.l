%{
#define YY_NO_INPUT
int fileno(FILE *stream);
int yywrap(void) { return 1; }

#include "ast.h"
#include "parser.h"
%}

%option nounput

%%
[0-9]+        { yylval.i = atoi(yytext); return integer; }
[_a-zA-Z]\w*  { yylval.s = yytext;       return identifier; }
\+\+          { return INC; }
--            { return DEC; }
[+\-*/%(){}]  { return yytext[0]; }
%%

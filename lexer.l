%{
#define YY_NO_INPUT
int fileno(FILE *stream);
int yywrap(void) { return 1; }

#include "ast.h"
#include "parser.h"
%}

%option nounput
%option yylineno


%%
\+\+                   { return INC;   }
--                     { return DEC;   }
char                   { return CHAR;  }
int                    { return INT;   }
if                     { return IF;    }
then                   { return THEN;  }
else                   { return ELSE;  }
while                  { return WHILE; }
[0-9]+                 { yylval.i = atoi(yytext);   return integer; }
[_a-zA-Z][_a-zA-Z0-9]* { yylval.s = strdup(yytext); return id; }
[+\-*/%(){}=,;]        { return yytext[0]; }
[\n\t ]                { }
%%

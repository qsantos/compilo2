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
[_a-zA-Z]\w*  { yylval.s = yytext;       return id; }
\+\+          { return INC;   }
--            { return DEC;   }
char          { return CHAR;  }
int           { return INT;   }
if            { return IF;    }
then          { return THEN;  }
else          { return ELSE;  }
while         { return WHILE; }
[+\-*/%(){}=] { return yytext[0]; }
%%

%{
void yyerror(char* s) { (void) s; }
int yylex(void);
#include "ast.h"
%}
%union { unsigned int num; ast_expr_t* expr; }
%start expr
%token <num> integer;
%type <expr> expr

%%
expr:
  integer         { $$ = expr_imm($1);     }
| expr '+' expr   { $$ = expr_add($1, $3); }
| expr '-' expr   { $$ = expr_sub($1, $3); }
| expr '*' expr   { $$ = expr_mul($1, $3); }
| expr '/' expr   { $$ = expr_div($1, $3); }
| expr '%' expr   { $$ = expr_mod($1, $3); }
%%

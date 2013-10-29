%{
void yyerror(char* s) { (void) s; }
int yylex(void);
#include "ast.h"
extern ast_expr_t* parsed;
%}
%union { unsigned int i; const char* s; ast_lval_t* l; ast_expr_t* e; }
%start prgm
%token INC
%token DEC
%token <i> integer;
%token <s> identifier;
%type  <l> lval;
%type  <e> expr

%%
lval:
  identifier      { $$ = lval_var($1);     }
;

expr:
  '(' expr ')'    { $$ = $2;               }
| integer         { $$ = expr_imm($1);     }
| expr '+' expr   { $$ = expr_add($1, $3); }
| expr '-' expr   { $$ = expr_sub($1, $3); }
| expr '*' expr   { $$ = expr_mul($1, $3); }
| expr '/' expr   { $$ = expr_div($1, $3); }
| expr '%' expr   { $$ = expr_mod($1, $3); }
| lval INC        { $$ = expr_inc($1);     }
| lval DEC        { $$ = expr_dec($1);     }
| lval            { $$ = expr_lva($1);     }
;

prgm: expr { parsed = $1; };
%%

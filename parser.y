%{
void yyerror(char* s) { (void) s; }
int yylex(void);
#include <stdlib.h>
#include "ast.h"
extern ast_fnct_t* parsed;
%}
%union
{
	unsigned int i;
	const char* s;
	ast_type_t  t;
	ast_lval_t* l;
	ast_argl_t* a;
	ast_expr_t* e;
	ast_decl_t* z;
	ast_stmt_t* w;
	ast_stml_t* m;
	ast_blck_t* b;
	ast_dcll_t* d;
	ast_fnct_t* f;
}
%start prgm
%token INC DEC
%token IF THEN ELSE WHILE
%token CHAR INT
%token <i> integer;
%token <s> id;
%type  <t> type;
%type  <l> lval;
%type  <a> argl;
%type  <e> expr
%type  <z> decl;
%type  <w> stmt;
%type  <m> stml;
%type  <b> blck;
%type  <d> dcll;
%type  <f> fcnt;

%%
type:
  CHAR                                { $$ = T_CHAR;                }
| INT                                 { $$ = T_INT;                 }
;

lval:
  id                                  { $$ = lval_var($1);          }
| '*' expr                            { $$ = lval_drf($2);          }
;

argl:
                                      { $$ = NULL;                  }
| expr ',' argl                       { $$ = argl_make($1, $3);     }
;

expr:
  '(' expr ')'                        { $$ = $2;                    }
| integer                             { $$ = expr_imm($1);          }
| expr '+' expr                       { $$ = expr_add($1, $3);      }
| expr '-' expr                       { $$ = expr_sub($1, $3);      }
| expr '*' expr                       { $$ = expr_mul($1, $3);      }
| expr '/' expr                       { $$ = expr_div($1, $3);      }
| expr '%' expr                       { $$ = expr_mod($1, $3);      }
| lval INC                            { $$ = expr_inc($1);          }
| lval DEC                            { $$ = expr_dec($1);          }
| lval                                { $$ = expr_lva($1);          }
| lval '=' expr                       { $$ = expr_asg($1, $3);      }
| id '(' argl ')'                     { $$ = expr_fun($1, $3);      }
;

decl:
  type id                             { $$ = decl_make($1, $2);     }
;

stmt:
  expr                                { $$ = stmt_expr($1);         }
| IF '(' expr ')' THEN stmt           { $$ = stmt_ifth($3, $6);     }
| IF '(' expr ')' THEN stmt ELSE stmt { $$ = stmt_ifte($3, $6, $8); }
| WHILE '(' expr ')' stmt             { $$ = stmt_whil($3, $5);     }
;

stml:
                                      { $$ = NULL;                  }
| stmt ';' stml                       { $$ = stml_make($1, $3);     }
;

blck:
  stml                                { $$ = blck_make($1);         }
;

dcll:
                                      { $$ = NULL;                  }
| decl ',' dcll                       { $$ = dcll_make($1, $3);     }
;

fcnt:
  type id '(' dcll ')' blck           { $$ = fnct_make($2,$4,$1,$6);}

prgm: fcnt { parsed = $1; };
%%

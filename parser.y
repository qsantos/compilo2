%{
int yylex(void);
void yyerror(char* s);
#include <stdlib.h>
#include <stdio.h>
#include "ast.h"
extern ast_prgm_t* parsed;
%}

%union
{
	unsigned int i;
	const char* s;
	ast_type_t* t;
	ast_lval_t* l;
	ast_argl_t* a;
	ast_expr_t* e;
	ast_decl_t* z;
	ast_stmt_t* w;
	ast_stml_t* m;
	ast_blck_t* b;
	ast_dcll_t* d;
	ast_fnct_t* f;
	ast_fctl_t* y;
	ast_prgm_t* p;
}
%start prgm
%token CHAR INT
%token <i> integer
%token <s> id
%left STAR
%left INC DEC
%right IF ELSE WHILE
%left '=' ',' ';'
%left '+' '-'
%left '*' '/' '%'
%type  <t> type
%type  <l> lval
%type  <a> argl
%type  <e> expr
%type  <z> decl
%type  <w> stmt
%type  <m> stml
%type  <b> blck
%type  <d> dcll
%type  <f> fcnt
%type  <y> fctl
%type  <p> prgm


%%
type:
  CHAR                           { $$ = type_char();           }
| INT                            { $$ = type_int ();           }
| type '*'                       { $$ = type_ptr ($1);         }
;

lval:
  id                             { $$ = lval_var($1);          }
| '*' expr %prec STAR            { $$ = lval_drf($2);          }
;

argl:
                                 { $$ = NULL;                  }
| expr                           { $$ = argl_make($1, NULL);   }
| expr ',' argl                  { $$ = argl_make($1, $3);     }
;

expr:
  '(' expr ')'                   { $$ = $2;                    }
| integer                        { $$ = expr_imm($1);          }
| expr '+' expr                  { $$ = expr_add($1, $3);      }
| expr '-' expr                  { $$ = expr_sub($1, $3);      }
| expr '*' expr                  { $$ = expr_mul($1, $3);      }
| expr '/' expr                  { $$ = expr_div($1, $3);      }
| expr '%' expr                  { $$ = expr_mod($1, $3);      }
| lval INC                       { $$ = expr_inc($1);          }
| lval DEC                       { $$ = expr_dec($1);          }
| lval                           { $$ = expr_lva($1);          }
| lval '=' expr                  { $$ = expr_asg($1, $3);      }
| id '(' argl ')'                { $$ = expr_fun($1, $3);      }
;

decl:
  type id                        { $$ = decl_make($1, $2);     }
;

stmt:
  blck                           { $$ = stmt_blck($1);         }
| expr                           { $$ = stmt_expr($1);         }
| decl                           { $$ = stmt_decl($1);         }
| IF '(' expr ')' stmt           { $$ = stmt_ifth($3, $5);     }
| IF '(' expr ')' stmt ELSE stmt { $$ = stmt_ifte($3, $5, $7); }
| WHILE '(' expr ')' stmt        { $$ = stmt_whil($3, $5);     }
;

stml:
                                 { $$ = NULL;                  }
| stmt ';' stml                  { $$ = stml_make($1, $3);     }
;

blck:
  '{' stml '}'                   { $$ = blck_make($2);         }
;

dcll:
                                 { $$ = NULL;                  }
| decl                           { $$ = dcll_make($1, NULL);   }
| decl ',' dcll                  { $$ = dcll_make($1, $3);     }
;

fcnt:
  type id '(' dcll ')' blck      { $$ = fnct_make($2,$4,$1,$6);}
;

fctl:
                                 { $$ = NULL;                  }
| fcnt fctl                      { $$ = fctl_make($1, $2);     }
;

prgm: fctl { parsed = prgm_make($1); };

%%
extern int yylineno;
void yyerror(char* s)
{
	fprintf(stderr, "%s at line %i\n", s, yylineno);
}

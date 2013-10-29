#ifndef PRINT_H
#define PRINT_H

#include "ast.h"

void print_id  (ast_id_t    n);
void print_argl(ast_argl_t* l);
void print_lval(ast_lval_t* l);
void print_expr(ast_expr_t* e);
void print_stmt(ast_stmt_t* s);
void print_stml(ast_stml_t* l);
void print_blck(ast_blck_t* b);
void print_fnct(ast_fnct_t* f);

#endif

/*\
 *  Just Another Compiler Collection
 *  Copyright (C) 2013 Quentin SANTOS
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
\*/

#ifndef PRINT_H
#define PRINT_H

#include "ast.h"

void print_id  (ast_id_t    n);
void print_type(ast_type_t* t);
void print_typl(ast_typl_t* l);
void print_argl(ast_argl_t* l);
void print_lval(ast_lval_t* l);
void print_expr(ast_expr_t* e);
void print_decl(ast_decl_t* d);
void print_stmt(ast_stmt_t* s);
void print_stml(ast_stml_t* l);
void print_blck(ast_blck_t* b);
void print_dcll(ast_dcll_t* l);
void print_fnct(ast_fnct_t* f);
void print_fctl(ast_fctl_t* l);
void print_prgm(ast_prgm_t* p);

#endif

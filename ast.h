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

#ifndef AST_H
#define AST_H

#include <sys/types.h>
#include <stdbool.h>

// defined types

typedef const char*       ast_id_t;
typedef struct ast_type_t ast_type_t;
typedef struct ast_typl_t ast_typl_t;
typedef struct ast_argl_t ast_argl_t;
typedef struct ast_lval_t ast_lval_t;
typedef struct ast_expr_t ast_expr_t;
typedef struct ast_decl_t ast_decl_t;
typedef struct ast_stmt_t ast_stmt_t;
typedef struct ast_stml_t ast_stml_t;
typedef struct ast_blck_t ast_blck_t;
typedef struct ast_dcll_t ast_dcll_t;
typedef struct ast_fnct_t ast_fnct_t;
typedef struct ast_fctl_t ast_fctl_t;
typedef struct ast_prgm_t ast_prgm_t;



// structs

struct ast_type_t
{
	enum
	{
		T_CHAR,
		T_INT,
		T_PTR,
		T_FUN,
	} type;
	union
	{
		struct { ast_type_t* a;                } ptr;
		struct { ast_type_t* r; ast_typl_t* l; } fun;
	} v;
};

struct ast_typl_t
{
	ast_type_t* t;
	ast_typl_t* l;
};

struct ast_lval_t
{
	enum
	{
		L_VAR,
		L_DRF,
	} type;
	union
	{
		struct { ast_id_t    a; } var;
		struct { ast_expr_t* a; } exp;
	} v;

	int line;
};

struct ast_argl_t
{
	ast_expr_t* a;
	ast_argl_t* l;
};

struct ast_expr_t
{
	enum
	{
		E_IMM,
		E_ADD, E_SUB, E_MUL, E_DIV, E_MOD,
		E_INC, E_DEC,
		E_LVA,
		E_ASG,
		E_FUN,
	} type;
	union
	{
		struct { unsigned int v;   } imm;
		struct { ast_expr_t *a,*b; } bin;
		struct { ast_lval_t* a;    } lva;
		struct { ast_lval_t* a; ast_expr_t* b; } asg;
		struct { ast_id_t    n; ast_argl_t* l; } fun;
	} v;

	int line;
};

struct ast_decl_t
{
	ast_type_t* t;
	ast_id_t    n;
};

struct ast_stmt_t
{
	enum
	{
		S_BLK,
		S_EXP,
		S_DEF,
		S_IFT,
		S_ITE,
		S_WHI,
	} type;
	union
	{
		struct { ast_blck_t* a; } blk;
		struct { ast_expr_t* a; } exp;
		struct { ast_decl_t* a; } def;
		struct { ast_expr_t* c; ast_stmt_t *a;}    ift;
		struct { ast_expr_t* c; ast_stmt_t *a,*b;} ite;
		struct { ast_expr_t* c; ast_stmt_t *a;}    whi;
	} v;
};

struct ast_stml_t
{
	struct ast_stmt_t* s;
	struct ast_stml_t* l;
};

struct ast_blck_t
{
	ast_stml_t* l;
};

struct ast_dcll_t
{
	ast_decl_t* d;
	ast_dcll_t* l;
};

struct ast_fnct_t
{
	ast_id_t    n;
	ast_dcll_t* d;
	ast_type_t* r;
	ast_blck_t* c;

	size_t t; // number of local variables
};

struct ast_fctl_t
{
	ast_fnct_t* f;
	ast_fctl_t* l;
};

struct ast_prgm_t
{
	ast_fctl_t* f;
};



// constructors

ast_type_t* type_char(void);
ast_type_t* type_int (void);
ast_type_t* type_ptr (ast_type_t* p);
ast_type_t* type_fun (ast_type_t* r, ast_typl_t* l);

ast_typl_t* typl_make(ast_type_t* t, ast_typl_t* l);

ast_lval_t* lval_var(ast_id_t v);
ast_lval_t* lval_drf(ast_expr_t* e);

ast_argl_t* argl_make(ast_expr_t* a, ast_argl_t* l);

ast_expr_t* expr_imm(unsigned int v);
ast_expr_t* expr_add(ast_expr_t* a, ast_expr_t* b);
ast_expr_t* expr_sub(ast_expr_t* a, ast_expr_t* b);
ast_expr_t* expr_mul(ast_expr_t* a, ast_expr_t* b);
ast_expr_t* expr_div(ast_expr_t* a, ast_expr_t* b);
ast_expr_t* expr_mod(ast_expr_t* a, ast_expr_t* b);
ast_expr_t* expr_inc(ast_lval_t* a);
ast_expr_t* expr_dec(ast_lval_t* a);
ast_expr_t* expr_lva(ast_lval_t* l);
ast_expr_t* expr_asg(ast_lval_t* a, ast_expr_t* b);
ast_expr_t* expr_fun(ast_id_t n, ast_argl_t* l);

ast_decl_t* decl_make(ast_type_t* t, ast_id_t n);

ast_stmt_t* stmt_blck(ast_blck_t* b);
ast_stmt_t* stmt_expr(ast_expr_t* e);
ast_stmt_t* stmt_decl(ast_decl_t* d);
ast_stmt_t* stmt_ifth(ast_expr_t* c, ast_stmt_t* a);
ast_stmt_t* stmt_ifte(ast_expr_t* c, ast_stmt_t* a, ast_stmt_t* b);
ast_stmt_t* stmt_whil(ast_expr_t* c, ast_stmt_t* a);

ast_stml_t* stml_make(ast_stmt_t* s, ast_stml_t* l);
ast_blck_t* blck_make(ast_stml_t* l);
ast_dcll_t* dcll_make(ast_decl_t* d, ast_dcll_t* l);
ast_fnct_t* fnct_make(ast_id_t n, ast_dcll_t* d, ast_type_t* r, ast_blck_t* c);
ast_fctl_t* fctl_make(ast_fnct_t* f, ast_fctl_t* l);
ast_prgm_t* prgm_make(ast_fctl_t* f);



// destructors

void type_del(ast_type_t* t);
void typl_del(ast_typl_t* l);
void lval_del(ast_lval_t* l);
void argl_del(ast_argl_t* l);
void expr_del(ast_expr_t* e);
void decl_del(ast_decl_t* d);
void stmt_del(ast_stmt_t* s);
void blck_del(ast_blck_t* b);
void dcll_del(ast_dcll_t* l);
void fnct_del(ast_fnct_t* f);
void fctl_del(ast_fctl_t* l);
void prgm_del(ast_prgm_t* p);



// methods

bool type_eq(ast_type_t* a, ast_type_t* b);
bool typl_eq(ast_typl_t* a, ast_typl_t* b);

#endif

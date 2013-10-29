#include "ast.h"

#include <stdlib.h>

#include "util.h"

ast_lval_t* lval_var(ast_var_t v)
{
	ast_lval_t* ret = MALLOC(ast_lval_t);
	ret->type = L_VAR;
	ret->v.var.a = v;
	return ret;
}

ast_lval_t* lval_drf(ast_expr_t* e)
{
	ast_lval_t* ret = MALLOC(ast_lval_t);
	ret->type = L_DRF;
	ret->v.exp.a = e;
	return ret;
}

void lval_del(ast_lval_t* l)
{
	switch (l->type)
	{
	case L_VAR:
		break;
	case L_DRF:
		expr_del(l->v.exp.a);
		break;
	}
	free(l);
}

#define EXPR_UNI(N,C) \
ast_expr_t* expr_##N(ast_expr_t* a) \
{ \
	ast_expr_t* ret = MALLOC(ast_expr_t); \
	ret->type = C; \
	ret->v.uni.a = a; \
	return ret; \
} \

EXPR_UNI(inc, E_INC)
EXPR_UNI(dec, E_DEC)

#define EXPR_BIN(N,C) \
ast_expr_t* expr_##N(ast_expr_t* a, ast_expr_t* b) \
{ \
	ast_expr_t* ret = MALLOC(ast_expr_t); \
	ret->type = C; \
	ret->v.bin.a = a; \
	ret->v.bin.b = b; \
	return ret; \
} \

EXPR_BIN(add, E_ADD)
EXPR_BIN(sub, E_SUB)
EXPR_BIN(mul, E_MUL)
EXPR_BIN(div, E_DIV)
EXPR_BIN(mod, E_MOD)

ast_expr_t* expr_lva(ast_lval_t* l)
{
	ast_expr_t* ret = MALLOC(ast_expr_t);
	ret->type = E_LVA;
	ret->v.lva.a = l;
	return ret;
}

void expr_del(ast_expr_t* e)
{
	switch (e->type)
	{
	case E_INC:
	case E_DEC:
		expr_del(e->v.uni.a);
		break;
	case E_ADD:
	case E_SUB:
	case E_MUL:
	case E_DIV:
	case E_MOD:
		expr_del(e->v.bin.a);
		expr_del(e->v.bin.b);
		break;
	case E_LVA:
		lval_del(e->v.lva.a);
		break;
	}
	free(e);
}

ast_stmt_t* stmt_expr(ast_expr_t* e)
{
	ast_stmt_t* ret = MALLOC(ast_stmt_t);
	ret->type = S_EXP;
	ret->v.exp.a = e;
	return ret;
}

ast_stmt_t* stmt_ifth(ast_expr_t* c, ast_stmt_t* a)
{
	ast_stmt_t* ret = MALLOC(ast_stmt_t);
	ret->type = S_IFT;
	ret->v.ift.c = c;
	ret->v.ift.a = a;
	return ret;
}

ast_stmt_t* stmt_ifte(ast_expr_t* c, ast_stmt_t* a, ast_stmt_t* b)
{
	ast_stmt_t* ret = MALLOC(ast_stmt_t);
	ret->type = S_ITE;
	ret->v.ite.c = c;
	ret->v.ite.a = a;
	ret->v.ite.b = b;
	return ret;
}

ast_stmt_t* stmt_whil(ast_expr_t* c, ast_stmt_t* a)
{
	ast_stmt_t* ret = MALLOC(ast_stmt_t);
	ret->type = S_WHI;
	ret->v.whi.c = c;
	ret->v.whi.a = a;
	return ret;
}

void stmt_del(ast_stmt_t* s)
{
	switch (s->type)
	{
	case S_EXP:
		expr_del(s->v.exp.a);
		break;
	case S_IFT:
		expr_del(s->v.ift.c);
		stmt_del(s->v.ift.a);
		break;
	case S_ITE:
		expr_del(s->v.ite.c);
		stmt_del(s->v.ite.a);
		stmt_del(s->v.ite.b);
		break;
	case S_WHI:
		expr_del(s->v.whi.c);
		stmt_del(s->v.whi.a);
		break;
	}
	free(s);
}

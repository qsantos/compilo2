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
	ret->v.expr.a = e;
	return ret;
}

void lval_del(ast_lval_t* l)
{
	switch (l->type)
	{
	case L_VAR:
		break;
	case L_DRF:
		expr_del(l->v.expr.a);
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

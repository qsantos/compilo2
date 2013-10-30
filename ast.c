#include "ast.h"

#include <stdlib.h>
#include <string.h>

#include "util.h"

ast_type_t* type_char(void)
{
	ast_type_t* ret = MALLOC(ast_type_t);
	ret->type = T_CHAR;
	ret->ptr  = NULL;
	return ret;
}
ast_type_t* type_int (void)
{
	ast_type_t* ret = MALLOC(ast_type_t);
	ret->type = T_INT;
	ret->ptr  = NULL;
	return ret;
}

ast_type_t* type_ptr (ast_type_t* p)
{
	ast_type_t* ret = MALLOC(ast_type_t);
	ret->type = T_PTR;
	ret->ptr  = p;
	return ret;
}

void type_del(ast_type_t* t)
{
	if (t->ptr)
		type_del(t->ptr);
	free(t);
}

ast_argl_t* argl_make(ast_expr_t* a, ast_argl_t* l)
{
	ast_argl_t* ret = MALLOC(ast_argl_t);
	ret->a = a;
	ret->l = l;
	return ret;
}

void argl_del(ast_argl_t* l)
{
	expr_del(l->a);
	if (l->l)
		argl_del(l->l);
	free(l);
}

ast_lval_t* lval_var(ast_id_t v)
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

ast_expr_t* expr_imm(unsigned int v)
{
	ast_expr_t* ret = MALLOC(ast_expr_t);
	ret->type = E_IMM;
	ret->v.imm.v = v;
	return ret;
}

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

#define EXPR_LVA(N,C) \
ast_expr_t* expr_##N(ast_lval_t* a) \
{ \
	ast_expr_t* ret = MALLOC(ast_expr_t); \
	ret->type = C; \
	ret->v.lva.a = a; \
	return ret; \
} \

EXPR_LVA(inc, E_INC)
EXPR_LVA(dec, E_DEC)
EXPR_LVA(lva, E_LVA)

ast_expr_t* expr_asg(ast_lval_t* a, ast_expr_t* b)
{
	ast_expr_t* ret = MALLOC(ast_expr_t);
	ret->type = E_ASG;
	ret->v.asg.a = a;
	ret->v.asg.b = b;
	return ret;
}

ast_expr_t* expr_fun(ast_id_t n, ast_argl_t* l)
{
	ast_expr_t* ret = MALLOC(ast_expr_t);
	ret->type = E_FUN;
	ret->v.fun.n = n;
	ret->v.fun.l = l;
	return ret;
}

void expr_del(ast_expr_t* e)
{
	switch (e->type)
	{
	case E_IMM:
		break;
	case E_ADD:
	case E_SUB:
	case E_MUL:
	case E_DIV:
	case E_MOD:
		expr_del(e->v.bin.a);
		expr_del(e->v.bin.b);
		break;
	case E_INC:
	case E_DEC:
	case E_LVA:
		lval_del(e->v.lva.a);
		break;
	case E_ASG:
		lval_del(e->v.asg.a);
		expr_del(e->v.asg.b);
		break;
	case E_FUN:
		argl_del(e->v.fun.l);
		break;
	}
	free(e);
}

ast_decl_t* decl_make(ast_type_t* t, ast_id_t n)
{
	ast_decl_t* ret = MALLOC(ast_decl_t);
	ret->t = t;
	ret->n = n;
	return ret;
}

void decl_del(ast_decl_t* d)
{
	free(d);
}

ast_stmt_t* stmt_blck(ast_blck_t* b)
{
	ast_stmt_t* ret = MALLOC(ast_stmt_t);
	ret->type = S_BLK;
	ret->v.blk.a = b;
	return ret;
}

ast_stmt_t* stmt_expr(ast_expr_t* e)
{
	ast_stmt_t* ret = MALLOC(ast_stmt_t);
	ret->type = S_EXP;
	ret->v.exp.a = e;
	return ret;
}

ast_stmt_t* stmt_decl(ast_decl_t* d)
{
	ast_stmt_t* ret = MALLOC(ast_stmt_t);
	ret->type = S_DEF;
	ret->v.def.a = d;
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
	case S_BLK:
		blck_del(s->v.blk.a);
		break;
	case S_EXP:
		expr_del(s->v.exp.a);
		break;
	case S_DEF:
		decl_del(s->v.def.a);
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

ast_stml_t* stml_make(ast_stmt_t* s, ast_stml_t* l)
{
	ast_stml_t* ret = MALLOC(ast_stml_t);
	ret->s = s;
	ret->l = l;
	return ret;
}

void stml_del(ast_stml_t* l)
{
	stmt_del(l->s);
	if (l->l)
		stml_del(l->l);
	free(l);
}

ast_blck_t* blck_make(ast_stml_t* l)
{
	ast_blck_t* ret = MALLOC(ast_blck_t);
	ret->l = l;
	return ret;
}

void blck_del(ast_blck_t* b)
{
	stml_del(b->l);
	free(b);
}

ast_dcll_t* dcll_make(ast_decl_t* d, ast_dcll_t* l)
{
	ast_dcll_t* ret = MALLOC(ast_dcll_t);
	ret->d = d;
	ret->l = l;
	return ret;
}

void dcll_del(ast_dcll_t* l)
{
	decl_del(l->d);
	if (l->l)
		dcll_del(l->l);
	free(l);
}

ast_fnct_t* fnct_make(ast_id_t n, ast_dcll_t* d, ast_type_t* r, ast_blck_t* c)
{
	ast_fnct_t* ret = MALLOC(ast_fnct_t);
	ret->n = n;
	ret->d = d;
	ret->r = r;
	ret->c = c;
	return ret;
}

void fnct_del(ast_fnct_t* f)
{
	dcll_del(f->d);
	if (f->c)
		blck_del(f->c);
	free(f);
}

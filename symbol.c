#include "symbol.h"

static void aux_lval(ast_lval_t* l);
static void aux_expr(ast_expr_t* e);
static void aux_stmt(ast_stmt_t* s);
static void aux_stml(ast_stml_t* l);
static void aux_blck(ast_blck_t* b);
static void aux_fnct(ast_fnct_t* f);
static void aux_fctl(ast_fctl_t* l);

static void aux_lval(ast_lval_t* l)
{
	switch (l->type)
	{
	case L_VAR:
	// TODO
	case L_DRF:
		aux_expr(l->v.exp.a);
		break;
	}
}
static void aux_expr(ast_expr_t* e)
{
	switch (e->type)
	{
	case E_IMM:
		break;
	case E_ADD: case E_SUB: case E_MUL: case E_DIV: case E_MOD:
		aux_expr(e->v.bin.a);
		aux_expr(e->v.bin.a);
		break;
	case E_INC: case E_DEC:
	case E_LVA:
		aux_lval(e->v.lva.a);
		break;
	case E_ASG:
		aux_lval(e->v.asg.a);
		aux_expr(e->v.asg.b);
		break;
	case E_FUN:
	// TODO
		break;
	}
}
static void aux_stmt(ast_stmt_t* s)
{
	switch (s->type)
	{
	case S_BLK:
		aux_blck(s->v.blk.a);
		break;
	case S_EXP:
		aux_expr(s->v.exp.a);
		break;
	case S_DEF:
	// TODO
	case S_IFT:
		aux_expr(s->v.ift.c);
		aux_stmt(s->v.ift.a);
		break;
	case S_ITE:
		aux_expr(s->v.ite.c);
		aux_stmt(s->v.ite.a);
		aux_stmt(s->v.ite.b);
		break;
	case S_WHI:
		aux_expr(s->v.whi.c);
		aux_stmt(s->v.whi.a);
		break;
	}
}
static void aux_stml(ast_stml_t* l)
{
	if (!l) return;
	aux_stmt(l->s);
	aux_stml(l->l);
}
static void aux_blck(ast_blck_t* b)
{
	aux_stml(b->l);
}
static void aux_fnct(ast_fnct_t* f)
{
	// TODO
	aux_blck(f->c);
}
static void aux_fctl(ast_fctl_t* l)
{
	if (!l) return;
	aux_fnct(l->f);
	aux_fctl(l->l);
}
void ast_analyze_symbols(ast_prgm_t* p)
{
	aux_fctl(p->f);
}

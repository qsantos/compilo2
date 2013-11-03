#include "ast2ir.h"

static ir_aval_t aux_lval(ir_prgm_t* i, ast_lval_t* l);
static ir_aval_t aux_expr(ir_prgm_t* i, ast_expr_t* e);
static void      aux_stmt(ir_prgm_t* i, ast_stmt_t* s);
static void      aux_stml(ir_prgm_t* i, ast_stml_t* l);
static void      aux_blck(ir_prgm_t* i, ast_blck_t* b);
static void      aux_fnct(ir_prgm_t* i, ast_fnct_t* f);
static void      aux_fctl(ir_prgm_t* i, ast_fctl_t* l);
static void      aux_prgm(ir_prgm_t* i, ast_prgm_t* p);

#define BIN(N) \
	case E_##N: \
		a = aux_expr(i, e->v.bin.a); \
		b = aux_expr(i, e->v.bin.b); \
		ir_push3(i, I_##N, O_REG, a, O_REG, a, O_REG, b); \
		return a; \

static ir_aval_t aux_lval(ir_prgm_t* i, ast_lval_t* l)
{
	(void) i;
	(void) l;
	switch (l->type)
	{
	case L_VAR:
	case L_DRF:
		break;
	}
	return 0;
}

static ir_aval_t aux_expr(ir_prgm_t* i, ast_expr_t* e)
{
	ir_aval_t a;
	ir_aval_t b;
	switch (e->type)
	{
	case E_IMM:
		a = ir_reg(i);
		ir_push2(i, I_MOV, O_REG, a, O_IMM, e->v.imm.v);
		return a;
	BIN(ADD)
	BIN(SUB)
	BIN(MUL)
	BIN(DIV)
	BIN(MOD)
	case E_INC:
		a = aux_lval(i, e->v.lva.a);
		ir_push3(i, I_ADD, O_REG, a, O_REG, a, O_IMM, 1);
		return a;
	case E_DEC:
		a = aux_lval(i, e->v.lva.a);
		ir_push3(i, I_SUB, O_REG, a, O_REG, a, O_IMM, 1);
		return a;
	case E_LVA:
		return aux_lval(i, e->v.lva.a);
	case E_ASG: // TODO
	case E_FUN: // TODO
	default:
		return 0;
	}
}

static void aux_stmt(ir_prgm_t* i, ast_stmt_t* s)
{
	switch (s->type)
	{
	case S_BLK:
		aux_blck(i, s->v.blk.a);
		break;
	case S_EXP:
		aux_expr(i, s->v.exp.a);
		break;
	case S_DEF:
		break;
	case S_IFT:
	case S_ITE:
	case S_WHI:
		break;
	}
}

static void aux_stml(ir_prgm_t* i, ast_stml_t* l)
{
	if (!l) return;
	aux_stmt(i, l->s);
	aux_stml(i, l->l);
}

static void aux_blck(ir_prgm_t* i, ast_blck_t* b)
{
	aux_stml(i, b->l);
}

static void aux_fnct(ir_prgm_t* i, ast_fnct_t* f)
{
	ir_push0(i, I_LBL);
	aux_blck(i, f->c);
}

static void aux_fctl(ir_prgm_t* i, ast_fctl_t* l)
{
	if (!l) return;
	aux_fnct(i, l->f);
	aux_fctl(i, l->l);
}

static void aux_prgm(ir_prgm_t* i, ast_prgm_t* p)
{
	aux_fctl(i, p->f);
}

void ast2ir(ir_prgm_t* i, ast_prgm_t* a)
{
	aux_prgm(i, a);
}

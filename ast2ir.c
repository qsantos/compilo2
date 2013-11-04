#include "ast2ir.h"

#include "htable.h"

static ir_aval_t aux_lval(ir_prgm_t* i, ast_lval_t* l, htable_t* h);
static ir_aval_t aux_expr(ir_prgm_t* i, ast_expr_t* e, htable_t* h);
static void      aux_stmt(ir_prgm_t* i, ast_stmt_t* s, htable_t* h);
static void      aux_stml(ir_prgm_t* i, ast_stml_t* l, htable_t* h);
static void      aux_blck(ir_prgm_t* i, ast_blck_t* b, htable_t* h);
static void      aux_fnct(ir_prgm_t* i, ast_fnct_t* f);
static void      aux_fctl(ir_prgm_t* i, ast_fctl_t* l);
static void      aux_prgm(ir_prgm_t* i, ast_prgm_t* p);

static ir_aval_t aux_lval(ir_prgm_t* i, ast_lval_t* l, htable_t* h)
{
	switch (l->type)
	{
	case L_VAR:
		return htable_find(h, l->v.var.a);
	case L_DRF:
		return aux_expr(i, l->v.exp.a, h);
	}
	return 0;
}

#define BIN(N) \
	case E_##N: \
		r = ir_reg(i); \
		a = aux_expr(i, e->v.bin.a, h); \
		b = aux_expr(i, e->v.bin.b, h); \
		ir_push3(i, I_##N, O_REG, r, O_REG, a, O_REG, b); \
		return r; \

static ir_aval_t aux_expr(ir_prgm_t* i, ast_expr_t* e, htable_t* h)
{
	ir_aval_t r;
	ir_aval_t a;
	ir_aval_t b;
	ir_atype_t t;
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
		a = aux_lval(i, e->v.lva.a, h);
		ir_push3(i, I_ADD, O_REG, a, O_REG, a, O_IMM, 1);
		return a;
	case E_DEC:
		a = aux_lval(i, e->v.lva.a, h);
		ir_push3(i, I_SUB, O_REG, a, O_REG, a, O_IMM, 1);
		return a;
	case E_LVA:
		return aux_lval(i, e->v.lva.a, h);
	case E_ASG:
		a = aux_lval(i, e->v.asg.a, h);
		b = aux_expr(i, e->v.asg.b, h);
		t = e->v.asg.a->type == L_VAR ? O_REG : O_REGADDR;
		ir_push2(i, I_MOV, t, a, O_REG, b);
		return b;
	case E_FUN: // TODO
		break;
	}
	return 0;
}

static void aux_stmt(ir_prgm_t* i, ast_stmt_t* s, htable_t* h)
{
	ir_label_t la;
	ir_label_t lb;
	ir_aval_t a;
	switch (s->type)
	{
	case S_BLK:
		aux_blck(i, s->v.blk.a, h);
		break;
	case S_EXP:
		aux_expr(i, s->v.exp.a, h);
		break;
	case S_DEF:
		htable_push(h, s->v.def.a->n);
		break;
	case S_IFT:
		la = ir_label_make(i);

		// if
		a = aux_expr(i, s->v.ift.c, h);
		ir_push3(i, I_JEQ, O_REG, a, O_IMM, 0, O_IMM, la);

		// then
		aux_stmt(i, s->v.ift.a, h);

		// end
		ir_label_push(i, la);
		break;
	case S_ITE:
		la = ir_label_make(i);
		lb = ir_label_make(i);

		// if
		a = aux_expr(i, s->v.ite.c, h);
		ir_push3(i, I_JEQ, O_REG, a, O_IMM, 0, O_IMM, la);

		// then
		aux_stmt(i, s->v.ite.a, h);
		ir_push1(i, I_JMP, O_IMM, lb);

		// else
		ir_label_push(i, la);
		aux_stmt(i, s->v.ite.b, h);

		// end
		ir_label_push(i, lb);
		break;
	case S_WHI:
		break;
	}
}

static void aux_stml(ir_prgm_t* i, ast_stml_t* l, htable_t* h)
{
	if (!l) return;
	aux_stmt(i, l->s, h);
	aux_stml(i, l->l, h);
}

static void aux_blck(ir_prgm_t* i, ast_blck_t* b, htable_t* h)
{
	aux_stml(i, b->l, h);
}

static void aux_fnct(ir_prgm_t* i, ast_fnct_t* f)
{
	htable_t ht;
	htable_init(&ht);

	ir_resreg(i, f->t);
	aux_blck(i, f->c, &ht);

	htable_del(&ht);
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

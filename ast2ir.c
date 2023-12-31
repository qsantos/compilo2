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

#include "ast2ir.h"

#include "htable.h"
#include "symbol.h"

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

static size_t aux_argl(ir_prgm_t* i, ast_argl_t* l, htable_t* h)
{
	if (!l) return 0;
	size_t r = aux_argl(i, l->l, h);
	ir_aval_t a = aux_expr(i, l->a, h);
	ir_push1(i, I_PSH, O_REG, a);
	return r+1;
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
	ir_label_t l;
	size_t n;
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
	case E_FUN:
		n = aux_argl(i, e->v.fun.l, h);
		l = symbol_get_label(e->v.fun.n);
		ir_push1(i, I_CAL, O_IMM, l);
		ir_push1(i, I_MPP, O_IMM, n);
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
		la = ir_label_make(i);
		lb = ir_label_make(i);

		// while
		ir_label_push(i, la);
		a = aux_expr(i, s->v.whi.c, h);
		ir_push3(i, I_JEQ, O_REG, a, O_IMM, 0, O_IMM, lb);

		// do
		aux_stmt(i, s->v.whi.a, h);
		ir_push1(i, I_JMP, O_IMM, la);

		// end
		ir_label_push(i, lb);
		break;
	case S_RET:
		a = aux_expr(i, s->v.exp.a, h);
		ir_push2(i, I_MOV, O_REG, 0, O_REG, a);
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

	ir_label_t l = ir_label_make(i);
	symbol_set_label(f->n, l);
	ir_label_push(i, l);

	ir_resreg(i, f->t);
	aux_blck(i, f->c, &ht);
	ir_push0(i, I_RET);

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

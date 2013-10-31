#include "symbol.h"

#include <stdlib.h>
#include <stdio.h>

#include "util.h"
#include "htable.h"

// symbol table

typedef struct syminfo_t syminfo_t;

static void     push(symbol_t x);
static symbol_t pop (void);

struct syminfo_t
{
	const char* name;
	ast_type_t* type;
};

static size_t     n_symbs = 0;
static size_t     a_symbs = 0;
static syminfo_t* symbs = NULL;


static size_t    a_stack = 0;
static size_t    n_stack = 0;
static symbol_t* stack   = NULL;

static htable_t ht;

static void push(symbol_t x)
{
	if (a_stack == n_stack)
	{
		a_stack = a_stack ? 2*a_stack : 1;
		stack = MREALLOC(stack, symbol_t, a_stack);
	}
	stack[n_stack++] = x;
}

static symbol_t pop(void)
{
	if (!n_stack)
		return 0;

	symbol_t ret = stack[--n_stack];
	if (n_stack <= a_stack / 4)
	{
		a_stack /= 2;
		stack = MREALLOC(stack, symbol_t, a_stack);
	}

	return ret;
}

static void scope_enter(void)
{
	push(0);
}

static void scope_register(const char* name, ast_type_t* type)
{
	symbol_t s = htable_push(&ht, name);
	push(s);

	if (n_symbs == a_symbs)
	{
		a_symbs = a_symbs ? 2*a_symbs : 1;
		symbs = MREALLOC(symbs, syminfo_t, a_symbs);
	}
	symbs[n_symbs].name = name;
	symbs[n_symbs].type = type;
	n_symbs++;
}

static void scope_exit(void)
{
	symbol_t s;
	while ((s = pop()))
		htable_pop(&ht, symbs[s-1].name);
}









// type checking

static ast_type_t* aux_lval(ast_lval_t* l);
static ast_type_t* aux_expr(ast_expr_t* e);
static void        aux_stmt(ast_stmt_t* s);
static void        aux_stml(ast_stml_t* l);
static void        aux_blck(ast_blck_t* b);
static void        aux_fnct(ast_fnct_t* f);
static void        aux_fctl(ast_fctl_t* l);

static ast_type_t* aux_lval(ast_lval_t* l)
{
	switch (l->type)
	{
	case L_VAR:
	{
		symbol_t s = htable_find(&ht, l->v.var.a);
		if (s == 0)
		{
			fprintf(stderr, "Symbol '%s' is not defined at line %i\n", l->v.var.a, l->line);
			exit(1);
			return NULL;
		}
		else
			return symbs[s-1].type;
	}
	case L_DRF:
	{
		ast_type_t* t = aux_expr(l->v.exp.a);
		if (!t || t->type != T_PTR)
		{
			fprintf(stderr, "Cannot dereference expression at line %i\n", l->line);
			exit(1);
			return NULL;
		}
		else
			return t->ptr;
	}
	}
	return NULL;
}
static ast_type_t* aux_expr(ast_expr_t* e)
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
	return NULL;
}
static void aux_decl(ast_decl_t* d)
{
	scope_register(d->n, d->t);
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
		aux_decl(s->v.def.a);
		break;
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
	scope_enter();
	aux_stml(b->l);
	scope_exit();
}
static void aux_dcll(ast_dcll_t* l)
{
	if (!l) return;
	aux_decl(l->d);
	aux_dcll(l->l);
}
static void aux_fnct(ast_fnct_t* f)
{
//	scope_register(f->n); // TODO
	scope_enter();
	aux_dcll(f->d);
	aux_blck(f->c);
	scope_exit();
}
static void aux_fctl(ast_fctl_t* l)
{
	if (!l) return;
	aux_fnct(l->f);
	aux_fctl(l->l);
}
void ast_analyze_symbols(ast_prgm_t* p)
{
	htable_init(&ht);
	aux_fctl(p->f);
	htable_del(&ht);
}

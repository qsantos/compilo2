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

extern bool error;
static ast_type_t local_int = {T_INT, { { NULL } } };

static ast_type_t* aux_lval(ast_lval_t* l);
static ast_type_t* aux_expr(ast_expr_t* e);
static void        aux_stmt(ast_stmt_t* s);
static void        aux_stml(ast_stml_t* l);
static void        aux_blck(ast_blck_t* b);
static void        aux_fnct(ast_fnct_t* f);
static void        aux_fctl(ast_fctl_t* l);

static ast_type_t* aux_lval(ast_lval_t* l)
{
	ast_type_t* t;
	symbol_t    s;
	switch (l->type)
	{
	case L_VAR:
	{
		s = htable_find(&ht, l->v.var.a);
		if (s == 0)
		{
			fprintf(stderr, "Symbol '%s' is not defined at line %i\n", l->v.var.a, l->line);
			error = true;
			return NULL;
		}
		else
		{
			return symbs[s-1].type;
		}
	}
	case L_DRF:
	{
		t = aux_expr(l->v.exp.a);
		if (!t)
		{
			return NULL;
		}
		else if (t->type != T_PTR)
		{
			fprintf(stderr, "Cannot dereference expression at line %i\n", l->line);
			error = true;
			return NULL;
		}
		else
			return t->v.ptr.a;
	}
	}
	return NULL;
}

static void aux_check_argl(unsigned int n, ast_typl_t* t, ast_argl_t* a)
{
	if (a == NULL)
	{
		if (t != NULL)
		{
			fprintf(stderr, "Too few arguments\n");
			error = true;
		}
		return;
	}
	if (t == NULL && a != NULL)
	{
		fprintf(stderr, "Too many arguments at line %u\n", a->a->line);
		error = true;
		return;
	}

	if (!type_eq(t->t, aux_expr(a->a)))
	{
		fprintf(stderr, "Argument no %u is not of the right type at line %u\n", n, a->a->line);
		error = true;
		return;
	}

	if (t->l == NULL && a->l != NULL)
	{
		fprintf(stderr, "Too many arguments at line %u\n", a->a->line);
		error = true;
		return;
	}

	if (t->l != NULL && a->l == NULL)
	{
		fprintf(stderr, "Too few arguments at line %u\n", a->a->line);
		error = true;
		return;
	}

	aux_check_argl(n+1, t->l, a->l);
}
static ast_type_t* aux_expr(ast_expr_t* e)
{
	ast_type_t* a;
	ast_type_t* b;
	symbol_t    s;
	switch (e->type)
	{
	case E_IMM:
		return &local_int;
	case E_ADD: case E_SUB: case E_MUL: case E_DIV: case E_MOD:
		a = aux_expr(e->v.bin.a);
		b = aux_expr(e->v.bin.b);
		if (!a || !b)
		{
			return NULL;
		}
		else if (!type_eq(a,b))
		{
			fprintf(stderr, "Binary operation on different types at line %i\n", e->line);
			error = true;
			return NULL;
		}
		else
			return a;
	case E_INC: case E_DEC:
	case E_LVA:
		return aux_lval(e->v.lva.a);
	case E_ASG:
		a = aux_lval(e->v.asg.a);
		b = aux_expr(e->v.asg.b);
		if (!a || !b)
		{
			return NULL;
		}
		else if (!type_eq(a,b))
		{
			fprintf(stderr, "Assigment on different types at line %i\n", e->line);
			error = true;
			return NULL;
		}
		else
			return a;
	case E_FUN:
		s = htable_find(&ht, e->v.fun.n);
		if (s == 0)
		{
			fprintf(stderr, "Unknown function '%s' at line %i\n", e->v.fun.n, e->line);
			error = true;
			return NULL;
		}
		a = symbs[s-1].type;
		if (a->type != T_FUN)
		{
			fprintf(stderr, "Symbol '%s' is not a function at line %i\n", e->v.fun.n, e->line);
			error = true;
			return NULL;
		}
		aux_check_argl(1, a->v.fun.l, e->v.fun.l);
		return a->v.fun.r;
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
static ast_typl_t* aux_fnct_dcll2typl(ast_dcll_t* d)
{
	if (!d) return NULL;
	return typl_make(d->d->t, aux_fnct_dcll2typl(d->l));
}
static void aux_fnct(ast_fnct_t* f)
{
	ast_type_t* t = type_fun(f->r, aux_fnct_dcll2typl(f->d));
	scope_register(f->n, t);

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

#include "print.h"

#include <stdio.h>

void print_id(ast_id_t n)
{
	printf("%s", n);
}

void print_type(ast_type_t* t)
{
	switch (t->type)
	{
	case T_CHAR:
		printf("char");
		break;
	case T_INT:
		printf("int");
		break;
	case T_PTR:
		print_type(t->ptr);
		printf("*");
		break;
	}
}

void print_argl(ast_argl_t* l)
{
	if (!l) return;
	print_expr(l->a);
	if (l->l)
	{
		printf(", ");
		print_argl(l->l);
	}
}

void print_lval(ast_lval_t* l)
{
	switch (l->type)
	{
	case L_VAR:
		print_id(l->v.var.a);
		break;
	case L_DRF:
		printf("*(");
		print_expr(l->v.exp.a);
		printf(")");
		break;
	}
}

#define BIN_PRINT(OP) \
 printf("("); \
 print_expr(e->v.bin.a); \
 printf("%s", OP); \
 print_expr(e->v.bin.b); \
 printf(")"); \
 break; \

void print_expr(ast_expr_t* e)
{
	switch (e->type)
	{
	case E_IMM: printf("%u", e->v.imm.v); break;
	case E_ADD: BIN_PRINT("+")
	case E_SUB: BIN_PRINT("-")
	case E_MUL: BIN_PRINT("*")
	case E_DIV: BIN_PRINT("/")
	case E_MOD: BIN_PRINT("%")
	case E_INC: print_lval(e->v.lva.a); printf("++"); break;
	case E_DEC: print_lval(e->v.lva.a); printf("--"); break;
	case E_LVA: print_lval(e->v.lva.a); break;
	case E_ASG:
		print_lval(e->v.asg.a);
		printf(" = ");
		print_expr(e->v.asg.b);
		break;
	case E_FUN:
		print_id(e->v.fun.n);
		printf("(");
		print_argl(e->v.fun.l);
		printf(")");
		break;
	}
}

void print_decl(ast_decl_t* d)
{
	print_type(d->t);
	printf(" ");
	print_id(d->n);
}

void print_stmt(ast_stmt_t* s)
{
	switch (s->type)
	{
	case S_BLK:
		print_blck(s->v.blk.a);
		break;
	case S_EXP:
		print_expr(s->v.exp.a);
		break;
	case S_DEF:
		print_decl(s->v.def.a);
		break;
	case S_IFT:
		printf("if (");
		print_expr(s->v.ift.c);
		printf(")\n");
		print_stmt(s->v.ift.a);
		break;
	case S_ITE:
		printf("if (");
		print_expr(s->v.ite.c);
		printf(")\n");
		print_stmt(s->v.ite.a);
		printf("else\n");
		print_stmt(s->v.ite.b);
		break;
	case S_WHI:
		printf("while (");
		print_expr(s->v.ift.c);
		printf(")\n");
		print_stmt(s->v.ift.a);
		break;
	}
}

void print_stml(ast_stml_t* l)
{
	if (!l) return;
	print_stmt(l->s);
	printf(";\n");
	if (l->l)
		print_stml(l->l);
}

void print_blck(ast_blck_t* b)
{
	printf("{\n");
	print_stml(b->l);
	printf("}\n");
}

void print_dcll(ast_dcll_t* l)
{
	if (!l) return;
	print_decl(l->d);
	if (l->l)
	{
		printf(", ");
		print_dcll(l->l);
	}
}

void print_fnct(ast_fnct_t* f)
{
	print_type(f->r);
	printf(" ");
	print_id(f->n);
	printf("(");
	print_dcll(f->d);
	printf(")");
	if (f->c)
	{
		printf("\n");
		print_blck(f->c);
	}
	else
		printf(";\n");
}

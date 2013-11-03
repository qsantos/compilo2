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
		print_type(t->v.ptr.a);
		printf("*");
		break;
	case T_FUN:
		print_type(t->v.fun.r);
		printf("(");
		print_typl(t->v.fun.l);
		printf(")");
	}
}

void print_typl(ast_typl_t* l)
{
	if (!l) return;
	print_type(l->t);
	if (l->l)
	{
		printf(", ");
		print_typl(l->l);
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

void print_fctl(ast_fctl_t* l)
{
	if (!l) return;
	print_fnct(l->f);
	if (l->l)
	{
		printf("\n");
		print_fctl(l->l);
	}
}

void print_prgm(ast_prgm_t* p)
{
	print_fctl(p->f);
}

static void print_op(ir_arg_t* a)
{
	switch (a->t)
	{
	case O_IMM:
		printf("$%#x", a->v);
		break;
	case O_IMMADDR:
		printf("*$%#x", a->v);
		break;
	case O_REG:
		printf("%%r%u", a->v);
		break;
	case O_REGADDR:
		printf("*%%r%u", a->v);
		break;
	}
}
void print_ir(ir_prgm_t* ir)
{
	for (size_t i = 0; i < ir->n_instr; i++)
	{
		if (i) printf("\n");

		ir_instr_t* j = &ir->instrs[i];
		ir_opcode_t t = j->type;
		switch (t)
		{
		case I_NOP: printf("nop"); break;
		case I_HLT: printf("hlt"); break;
		case I_MOV: printf("mov"); break;
		case I_NEG: printf("neg"); break;
		case I_NOT: printf("not"); break;
		case I_LOR: printf("lor"); break;
		case I_AND: printf("and"); break;
		case I_XOR: printf("xor"); break;
		case I_ADD: printf("add"); break;
		case I_SUB: printf("sub"); break;
		case I_MUL: printf("myl"); break;
		case I_DIV: printf("div"); break;
		case I_MOD: printf("mod"); break;
		case I_LBL: printf("lbl"); break;
		case I_JMP: printf("jmp"); break;
		case I_JEQ: printf("jeq"); break;
		case I_JNE: printf("jne"); break;
		case I_JGE: printf("jge"); break;
		case I_JGT: printf("jgt"); break;
		case I_JLE: printf("jle"); break;
		case I_JLT: printf("jlt"); break;
		case I_CAL: printf("cal"); break;
		case I_RET: printf("ret"); break;
		}

		if (t == I_LBL) // TODO
			continue;

		if (t == I_NOP || t == I_HLT || t == I_RET)
			continue;

		printf(" ");
		print_op(&j->op0);

		if (t == I_JMP || t == I_CAL)
			continue;

		printf(", ");
		print_op(&j->op1);

		if (t == I_MOV || t == I_NEG || t == I_NOT)
			continue;

		printf(", ");
		print_op(&j->op2);

	}
	printf("\n");
}

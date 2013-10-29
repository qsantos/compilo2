#include "ast.h"

#include <stdlib.h>
#include <stdio.h>

void* smalloc(size_t size, const char* file, int line)
{
	void* ret = malloc(size);
	if (!ret)
	{
		fprintf(stderr, "Allocation failed at %s:%i\n", file, line);
		exit(1);
	}
	return ret;
}

#define MALLOC(T) ((T*) smalloc(sizeof(T), __FILE__, __LINE__))

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
	}
	free(e);
}

#ifndef AST_H
#define AST_H

typedef struct ast_expr_t ast_expr_t;

struct ast_expr_t
{
	enum
	{
		E_INC,
		E_DEC,
		E_ADD,
		E_SUB,
		E_MUL,
		E_DIV,
		E_MOD,
	} type_t;
	union
	{
		struct { ast_expr_t a;   } uni;
		struct { ast_expr_t a,b; } bin;
	} v;
};

#endif

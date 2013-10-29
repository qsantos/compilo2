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
	} type;
	union
	{
		struct { ast_expr_t *a;    } uni;
		struct { ast_expr_t *a,*b; } bin;
	} v;
};

ast_expr_t* expr_inc(ast_expr_t* a);
ast_expr_t* expr_dec(ast_expr_t* a);

ast_expr_t* expr_add(ast_expr_t* a, ast_expr_t* b);
ast_expr_t* expr_sub(ast_expr_t* a, ast_expr_t* b);
ast_expr_t* expr_mul(ast_expr_t* a, ast_expr_t* b);
ast_expr_t* expr_div(ast_expr_t* a, ast_expr_t* b);
ast_expr_t* expr_mod(ast_expr_t* a, ast_expr_t* b);

#endif

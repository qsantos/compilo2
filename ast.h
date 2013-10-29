#ifndef AST_H
#define AST_H

typedef unsigned int      ast_var_t;
typedef struct ast_lval_t ast_lval_t;
typedef struct ast_expr_t ast_expr_t;
typedef struct ast_stmt_t ast_stmt_t;

struct ast_lval_t
{
	enum
	{
		L_VAR,
		L_DRF,
	} type;
	union
	{
		struct { ast_var_t a;   } var;
		struct { ast_expr_t* a; } exp;
	} v;
};

ast_lval_t* lval_var(ast_var_t v);
ast_lval_t* lval_drf(ast_expr_t* e);

void lval_del(ast_lval_t* l);

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
		E_LVA,
	} type;
	union
	{
		struct { ast_expr_t *a;    } uni;
		struct { ast_expr_t *a,*b; } bin;
		struct { ast_lval_t* a;    } lva;
	} v;
};

ast_expr_t* expr_inc(ast_expr_t* a);
ast_expr_t* expr_dec(ast_expr_t* a);

ast_expr_t* expr_add(ast_expr_t* a, ast_expr_t* b);
ast_expr_t* expr_sub(ast_expr_t* a, ast_expr_t* b);
ast_expr_t* expr_mul(ast_expr_t* a, ast_expr_t* b);
ast_expr_t* expr_div(ast_expr_t* a, ast_expr_t* b);
ast_expr_t* expr_mod(ast_expr_t* a, ast_expr_t* b);

ast_expr_t* expr_lva(ast_lval_t* l);

void expr_del(ast_expr_t* e);

struct ast_stmt_t
{
	enum
	{
		S_EXP,
		S_IFT,
		S_ITE,
		S_WHI,
	} type;
	union
	{
		struct { ast_expr_t* a; } exp;
		struct { ast_expr_t* c; ast_stmt_t *a;}    ift;
		struct { ast_expr_t* c; ast_stmt_t *a,*b;} ite;
		struct { ast_expr_t* c; ast_stmt_t *a;}    whi;
	} v;
};

ast_stmt_t* stmt_expr(ast_expr_t* e);
ast_stmt_t* stmt_ifth(ast_expr_t* c, ast_stmt_t* a);
ast_stmt_t* stmt_ifte(ast_expr_t* c, ast_stmt_t* a, ast_stmt_t* b);
ast_stmt_t* stmt_whil(ast_expr_t* c, ast_stmt_t* a);

void stmt_del(ast_stmt_t* s);

#endif

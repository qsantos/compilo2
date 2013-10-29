#ifndef AST_H
#define AST_H

typedef unsigned int      ast_id_t;
typedef struct ast_argl_t ast_argl_t;
typedef struct ast_lval_t ast_lval_t;
typedef struct ast_expr_t ast_expr_t;
typedef struct ast_stmt_t ast_stmt_t;
typedef struct ast_stml_t ast_stml_t;
typedef struct ast_blck_t ast_blck_t;
typedef struct ast_fnct_t ast_fnct_t;



struct ast_lval_t
{
	enum
	{
		L_VAR,
		L_DRF,
	} type;
	union
	{
		struct { ast_id_t    a; } var;
		struct { ast_expr_t* a; } exp;
	} v;
};

ast_lval_t* lval_var(ast_id_t v);
ast_lval_t* lval_drf(ast_expr_t* e);

void lval_del(ast_lval_t* l);



struct ast_argl_t
{
	ast_expr_t* a;
	ast_argl_t* l;
};

ast_argl_t* argl_make(ast_expr_t* a, ast_argl_t* l);

void argl_del(ast_argl_t* l);



struct ast_expr_t
{
	enum
	{
		E_IMM,
		E_ADD,
		E_SUB,
		E_MUL,
		E_DIV,
		E_MOD,
		E_INC,
		E_DEC,
		E_LVA,
		E_ASG,
		E_FUN,
	} type;
	union
	{
		struct { unsigned int v;   } imm;
		struct { ast_expr_t *a,*b; } bin;
		struct { ast_lval_t* a;    } lva;
		struct { ast_lval_t* a; ast_expr_t* b; } asg;
		struct { ast_id_t    n; ast_argl_t* l; } fun;
	} v;
};

ast_expr_t* expr_imm(unsigned int v);

ast_expr_t* expr_add(ast_expr_t* a, ast_expr_t* b);
ast_expr_t* expr_sub(ast_expr_t* a, ast_expr_t* b);
ast_expr_t* expr_mul(ast_expr_t* a, ast_expr_t* b);
ast_expr_t* expr_div(ast_expr_t* a, ast_expr_t* b);
ast_expr_t* expr_mod(ast_expr_t* a, ast_expr_t* b);

ast_expr_t* expr_inc(ast_lval_t* a);
ast_expr_t* expr_dec(ast_lval_t* a);
ast_expr_t* expr_lva(ast_lval_t* l);

ast_expr_t* expr_asg(ast_lval_t* a, ast_expr_t* b);

ast_expr_t* expr_fun(ast_id_t n, ast_argl_t* l);

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
		struct { ast_expr_t* c; ast_blck_t *a;}    ift;
		struct { ast_expr_t* c; ast_blck_t *a,*b;} ite;
		struct { ast_expr_t* c; ast_blck_t *a;}    whi;
	} v;
};

ast_stmt_t* stmt_expr(ast_expr_t* e);
ast_stmt_t* stmt_ifth(ast_expr_t* c, ast_blck_t* a);
ast_stmt_t* stmt_ifte(ast_expr_t* c, ast_blck_t* a, ast_blck_t* b);
ast_stmt_t* stmt_whil(ast_expr_t* c, ast_blck_t* a);

void stmt_del(ast_stmt_t* s);



struct ast_stml_t
{
	struct ast_stmt_t* s;
	struct ast_stml_t* l;
};

ast_stml_t* stml_make(ast_stmt_t* s, ast_stml_t* l);



struct ast_blck_t
{
	ast_stml_t* l;
};

ast_blck_t* blck_make(ast_stml_t* l);

void blck_del(ast_blck_t* b);



struct ast_fnct_t
{
	ast_id_t    n;
	ast_blck_t* c;
};

ast_fnct_t* fnct_make(ast_id_t n, ast_blck_t* c);

void fnct_del(ast_fnct_t* f);

#endif

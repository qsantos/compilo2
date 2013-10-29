#ifndef AST_H
#define AST_H

typedef const char*       ast_id_t;
typedef struct ast_argl_t ast_argl_t;
typedef struct ast_lval_t ast_lval_t;
typedef struct ast_expr_t ast_expr_t;
typedef struct ast_decl_t ast_decl_t;
typedef struct ast_stmt_t ast_stmt_t;
typedef struct ast_stml_t ast_stml_t;
typedef struct ast_blck_t ast_blck_t;
typedef struct ast_dcll_t ast_dcll_t;
typedef struct ast_fnct_t ast_fnct_t;



typedef enum
{
	T_CHAR,
	T_INT,
} ast_type_t;



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



struct ast_decl_t
{
	ast_type_t t;
	ast_id_t   n;
};

ast_decl_t* decl_make(ast_type_t, ast_id_t n);

void decl_del(ast_decl_t* d);



struct ast_stmt_t
{
	enum
	{
		S_BLK,
		S_EXP,
		S_DEF,
		S_IFT,
		S_ITE,
		S_WHI,
	} type;
	union
	{
		struct { ast_blck_t* a; } blk;
		struct { ast_expr_t* a; } exp;
		struct { ast_decl_t* a; } def;
		struct { ast_expr_t* c; ast_stmt_t *a;}    ift;
		struct { ast_expr_t* c; ast_stmt_t *a,*b;} ite;
		struct { ast_expr_t* c; ast_stmt_t *a;}    whi;
	} v;
};

ast_stmt_t* stmt_blck(ast_blck_t* b);
ast_stmt_t* stmt_expr(ast_expr_t* e);
ast_stmt_t* stmt_decl(ast_decl_t* d);
ast_stmt_t* stmt_ifth(ast_expr_t* c, ast_stmt_t* a);
ast_stmt_t* stmt_ifte(ast_expr_t* c, ast_stmt_t* a, ast_stmt_t* b);
ast_stmt_t* stmt_whil(ast_expr_t* c, ast_stmt_t* a);

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



struct ast_dcll_t
{
	ast_decl_t* d;
	ast_dcll_t* l;
};

ast_dcll_t* dcll_make(ast_decl_t* d, ast_dcll_t* l);

void dcll_del(ast_dcll_t* l);



struct ast_fnct_t
{
	ast_id_t    n;
	ast_dcll_t* d;
	ast_type_t  r;
	ast_blck_t* c;
};

ast_fnct_t* fnct_make(ast_id_t n, ast_dcll_t* d, ast_type_t r, ast_blck_t* c);

void fnct_del(ast_fnct_t* f);

#endif

%{
void yyerror(char* s) { (void) s; }
int yylex(void);
%}
%start rule
%token token1
%token token2

%%
rule:
token1 token2
%%

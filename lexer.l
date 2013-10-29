%{
#define YY_NO_INPUT
int fileno(FILE *stream);
int yywrap(void) { return 1; }
%}

%option nounput

%%
[0-9]+ return 0;
%%

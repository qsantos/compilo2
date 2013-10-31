#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "ast.h"
#include "print.h"
#include "symbol.h"

extern int yyparse(void);

ast_prgm_t* parsed = NULL;
bool        error  = false;

int main(void)
{
	if (yyparse() != 0)
	{
		fprintf(stderr, "Parsing has failed\n");
		exit(1);
	}

	ast_analyze_symbols(parsed);
	if (error)
	{
		fprintf(stderr, "An error occured\n");
		exit(1);
	}

	return 0;
}

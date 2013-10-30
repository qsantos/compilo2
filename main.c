#include <stdlib.h>
#include <stdio.h>

#include "ast.h"
#include "print.h"
#include "symbol.h"

extern int yyparse(void);
ast_prgm_t* parsed = NULL;

int main(void)
{
	if (yyparse() != 0)
	{
		fprintf(stderr, "Parsing has failed\n");
		exit(1);
	}

	print_prgm(parsed);
	ast_analyze_symbols(parsed);

	return 0;
}

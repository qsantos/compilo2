#include <stdlib.h>
#include <stdio.h>

#include "ast.h"
#include "parser.h"
#include "print.h"

ast_prgm_t* parsed = NULL;
int main(void)
{
	if (yyparse() != 0)
	{
		fprintf(stderr, "Parsing has failed\n");
		exit(1);
	}

	print_prgm(parsed);

	return 0;
}

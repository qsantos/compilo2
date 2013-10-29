#include <stdlib.h>
#include <stdio.h>

#include "ast.h"
#include "parser.h"
#include "print.h"

ast_expr_t* parsed = NULL;
int main(void)
{
	if (yyparse() != 0)
	{
		fprintf(stderr, "Parsing has failed\n");
		exit(1);
	}

	print_expr(parsed);

	return 0;
}

#include <stdlib.h>
#include <stdio.h>

#include "ast.h"
#include "parser.h"
#include "print.h"

ast_fnct_t* parsed = NULL;
int main(void)
{
	if (yyparse() != 0)
	{
		fprintf(stderr, "Parsing has failed\n");
		exit(1);
	}

	print_fnct(parsed);

	return 0;
}

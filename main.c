/*\
 *  Just Another Compiler Collection
 *  Copyright (C) 2013 Quentin SANTOS
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
\*/

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "ast.h"
#include "print.h"
#include "symbol.h"
#include "ast2ir.h"

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

	ir_prgm_t ir;
	ast2ir(&ir, parsed);

	return 0;
}

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

#include "util.h"

#include <stdlib.h>
#include <stdio.h>

void* smalloc(size_t size, const char* file, int line)
{
	void* ret = malloc(size);
	if (!ret)
	{
		fprintf(stderr, "Allocation of %zu bytes failed at %s:%i\n", size, file, line);
		exit(1);
	}
	return ret;
}

void* srealloc(void* ptr, size_t size, const char* file, int line)
{
	void* ret = realloc(ptr, size);
	if (!ret)
	{
		fprintf(stderr, "Reallocation of %zu bytes failed at %s:%i\n", size, file, line);
		exit(1);
	}
	return ret;
}

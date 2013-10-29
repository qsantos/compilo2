#include "util.h"

#include <stdlib.h>
#include <stdio.h>

void* smalloc(size_t size, const char* file, int line)
{
	void* ret = malloc(size);
	if (!ret)
	{
		fprintf(stderr, "Allocation failed at %s:%i\n", file, line);
		exit(1);
	}
	return ret;
}

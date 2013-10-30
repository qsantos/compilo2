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

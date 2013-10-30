#include "symbol.h"

#include <stdlib.h>
#include <string.h>

void htable_init(htable_t* ht)
{
	memset(ht->b, 0, sizeof(ht->b));
}

void htable_del(htable_t* ht)
{
	for (size_t i = 0; i < N_BUCKETS; i++)
	{
		bucket_t* b = ht->b[i];
		while (b)
		{
			bucket_t* n = b->next;
			free(b->s);
			free(b);
			b = n;
		}
	}
}

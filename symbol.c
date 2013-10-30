#include "symbol.h"

#include <stdlib.h>
#include <string.h>

#include "util.h"

static symbol_t cursymbol = 0;
static unsigned int hash(const char* str);

void htable_init(htable_t* ht)
{
	memset(ht->b, 0, sizeof(ht->b));
}

static void bucket_del(bucket_t* b)
{
	free(b->name);
	free(b->s);
	free(b);
}
void htable_del(htable_t* ht)
{
	for (size_t i = 0; i < N_BUCKETS; i++)
	{
		bucket_t* b = ht->b[i];
		while (b)
		{
			bucket_t* n = b->next;
			bucket_del(b);
			b = n;
		}
	}
}

static symbol_t bucket_append(bucket_t* b)
{
	if (b->a == b->n)
	{
		b->a <<= 1;
		b->s = MREALLOC(b->s, symbol_t, b->a);
	}
	return (b->s[b->n++] = cursymbol++);
}
symbol_t htable_push(htable_t* ht, const char* name)
{
	bucket_t** b = &ht->b[hash(name)];
	while (*b && strcmp((*b)->name, name) != 0)
		b = &(*b)->next;

	bucket_t* c = *b;
	if (c == NULL)
	{
		bucket_t* n = MALLOC(bucket_t);
		n->name = strdup(name);
		n->next = NULL;
		n->n    = 0;
		n->a    = 10;
		n->s    = MMALLOC(symbol_t, n->a);
		*b = n;
		return bucket_append(n);
	}

	return bucket_append(c);
}

symbol_t htable_pop (htable_t* ht, const char* name)
{
	bucket_t** b = &ht->b[hash(name)];
	while (*b && strcmp((*b)->name, name) != 0)
		b = &(*b)->next;

	bucket_t* c = *b;
	if (!c)
		return 0;

	if (c->n <= 1)
	{
		bucket_t* n = c->next;
		symbol_t  r = c->s[0];
		bucket_del(c);
		*b = n;
		return r;
	}
	else
		return c->s[c->n--];
}

symbol_t htable_find(htable_t* ht, const char* name)
{
	bucket_t** b = &ht->b[hash(name)];
	while (*b && strcmp((*b)->name, name) != 0)
		b = &(*b)->next;

	bucket_t* c = *b;
	if (!c)
		return 0;

	return c->s[c->n];
}

static unsigned int hash(const char* str)
{
   unsigned int h = 1315423911;
   for (;*str;str++)
      h ^= ((h << 5) + (*str) + (h >> 2));
   return h;
}

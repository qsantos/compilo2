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

#include "htable.h"

#include <stdlib.h>
#include <string.h>

#include "util.h"

static unsigned int hash(const char* str);

void htable_init(htable_t* ht)
{
	memset(ht->b, 0, sizeof(ht->b));
	ht->cursymbol = 0;
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

static symbol_t bucket_append(bucket_t* b, symbol_t s)
{
	if (b->a == b->n)
	{
		b->a <<= 1;
		b->s = MREALLOC(b->s, symbol_t, b->a);
	}

	return (b->s[b->n++] = s);
}
symbol_t htable_push(htable_t* ht, const char* name)
{
	bucket_t** b = &ht->b[hash(name)];
	while (*b && strcmp((*b)->name, name) != 0)
		b = &(*b)->next;

	bucket_t* c = *b;
	symbol_t s = ++ht->cursymbol;
	if (c == NULL)
	{
		bucket_t* n = MALLOC(bucket_t);
		n->name = strdup(name);
		n->next = NULL;
		n->n    = 0;
		n->a    = 10;
		n->s    = MMALLOC(symbol_t, n->a);
		*b = n;
		return bucket_append(n, s);
	}

	return bucket_append(c, s);
}

symbol_t htable_pop(htable_t* ht, const char* name)
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
	{
		return c->s[--c->n];
	}
}

symbol_t htable_find(htable_t* ht, const char* name)
{
	bucket_t** b = &ht->b[hash(name)];
	while (*b && strcmp((*b)->name, name) != 0)
		b = &(*b)->next;

	bucket_t* c = *b;
	if (!c)
		return 0;

	return c->s[c->n-1];
}

static unsigned int hash(const char* str)
{
   unsigned int h = 1315423911;
   for (;*str;str++)
      h ^= ((h << 5) + (*str) + (h >> 2));
   return h % N_BUCKETS;
}

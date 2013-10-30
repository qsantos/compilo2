#ifndef SYMBOL_H
#define SYMBOL_H

#define N_BUCKETS 1000

#include <sys/types.h>

typedef unsigned int      symbol_t;
typedef struct bucket_t   bucket_t;
typedef struct htable_t   htable_t;

struct bucket_t
{
	char*     name;
	bucket_t* next;

	// stack
	size_t      n;
	size_t      a;
	symbol_t*   s;
};

struct htable_t
{
	bucket_t* b[N_BUCKETS];
};

void htable_init(htable_t* ht);
void htable_del (htable_t* ht);

symbol_t htable_push(htable_t* ht, const char* name);
symbol_t htable_pop (htable_t* ht, const char* name);
symbol_t htable_find(htable_t* ht, const char* name);

#endif

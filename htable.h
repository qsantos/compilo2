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

#ifndef HTABLE_H
#define HTABLE_H

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

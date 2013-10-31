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

#ifndef UTIL_H
#define UTIL_H

#include <sys/types.h>

void* smalloc (           size_t size, const char* file, int line);
void* srealloc(void* ptr, size_t size, const char* file, int line);

#define   MALLOC(T)     ((T*) smalloc (   sizeof(T),   __FILE__, __LINE__))
#define  MMALLOC(T,N)   ((T*) smalloc (   sizeof(T)*N, __FILE__, __LINE__))

#define  REALLOC(P,T)   ((T*) srealloc(P, sizeof(T),   __FILE__, __LINE__))
#define MREALLOC(P,T,N) ((T*) srealloc(P, sizeof(T)*N, __FILE__, __LINE__))

#endif

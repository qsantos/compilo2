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

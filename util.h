#ifndef UTIL_H
#define UTIL_H

#include <sys/types.h>

void* smalloc(size_t size, const char* file, int line);

#define MALLOC(T) ((T*) smalloc(sizeof(T), __FILE__, __LINE__))

#endif

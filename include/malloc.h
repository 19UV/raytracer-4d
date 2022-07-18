#pragma once

#include <stddef.h>

#define MALLOC( size) malloc_debug( ( size), __FILE__, __LINE__)
#define CALLOC( items, size) calloc_debug( ( items), ( size), __FILE__, __LINE__)
#define REALLOC( ptr, size) realloc_debug( ( ptr), ( size), __FILE__, __LINE__)
#define FREE( ptr) free_debug( ( ptr))

void* malloc_debug(size_t size, const char* file, unsigned int line);
void* calloc_debug(size_t n_items, size_t size, const char* file, unsigned int line);
void* realloc_debug(void* ptr, size_t size, const char* file, unsigned int line);
void free_debug(void* ptr);

void check_memoryleaks(void);

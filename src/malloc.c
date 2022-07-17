#include "malloc.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

// Unsafe min function!
// Don't put a non-pure function in either of the arguments
// You have been warned
#define MIN( a, b) ( ( a) < ( b)) ? ( a) : ( b)

struct MallocHeader {
	const char* file;
	unsigned int line;
	size_t size;

	struct MallocHeader* next, * prev;
};
typedef struct MallocHeader MallocHeader;

static size_t get_size(void* ptr) {
	MallocHeader* header = (MallocHeader*)ptr - 1;

	return header->size;
}

static MallocHeader* _start = NULL,
	* _current = NULL;
static size_t _allocated_bytes = 0;

#ifdef MEM_TRACE
void print_linked_list(void) {
	fprintf(stderr, "\tLinked List: %zu\n", _allocated_bytes);

	MallocHeader* curr = _start;
	while(curr != NULL) {
		fprintf(stderr, "\t\t%p\n", curr);

		curr = curr->next;
	}
}
#endif

void* malloc_debug(size_t size, const char* file, unsigned int line) {
#ifndef NDEBUG
	MallocHeader* memory = malloc(sizeof(MallocHeader) + size);
	if(memory == NULL) {
		return NULL;
	}

	memory->file = file;
	memory->line = line;
	memory->size = size;
	memory->next = NULL;
	memory->prev = _current;

	// Linked List Stuff (add to end)
	_allocated_bytes += size;
	if(_start == NULL) {
		_start = memory;
	} else {
		_current->next = memory;
	}
	_current = memory;

#ifdef MEM_TRACE
	fprintf(stderr, "Malloc: %s %u %zu - %p\n", file, line, size, memory);
	print_linked_list();
#endif

	return (void*)(memory + 1);
#else
	return malloc(size);
#endif
}

void* calloc_debug(size_t n_items, size_t size, const char* file, unsigned int line) {
#ifndef NDEBUG
	const size_t byte_count = n_items * size;

	void* memory = malloc_debug(byte_count, file, line);
	if(memory == NULL) {
		return NULL;
	}

	memset(memory, 0, size);

	return memory;
#else
	return calloc(n_items, size);
#endif
}

void* realloc_debug(void* ptr, size_t size, const char* file, unsigned int line) {
#ifndef NDEBUG
	if(size > 0) {
		void* new = malloc_debug(size, file, line);
		if(new == NULL) {
			return NULL;
		}

		if(ptr != NULL) {
			size_t old_size = get_size(ptr);

			memcpy(new, ptr, MIN(old_size, size));
		}

		if(ptr != NULL) {
			free_debug(ptr);
		}

		return new;
	} else {
		free_debug(ptr);

		return NULL;
	}
#else
	return realloc(ptr, size);
#endif
}

void free_debug(void* ptr) {
#ifndef NDEBUG
	MallocHeader* header = (MallocHeader*)ptr - 1;

	_allocated_bytes -= header->size;

	// Linked List Stuff (removing)
	if(header->prev != NULL) {
		header->prev->next = header->next;
	} else {
		_start = header->next;
	} 

	if(header->next != NULL) {
		header->next->prev = header->prev;
	} else {
		_current = header->prev;
	}

	free(header);

#ifdef MEM_TRACE
	fprintf(stderr, "Free - %p\n", header);
	print_linked_list();
#endif

#else
	free(ptr);
#endif
}


void check_memoryleaks(void) {
#ifndef NDEBUG
	if(_allocated_bytes > 0) {
		fprintf(stderr, "[ERROR] Unfreed Bytes: %zu\n", _allocated_bytes);

		size_t allocated_blocks = 0;

		MallocHeader* curr = _start;
		while(curr != NULL) {
			allocated_blocks++;

			fprintf(stderr, "\tFile: %s, Line: %u, Bytes: %zu\n", curr->file, curr->line, curr->size);

			MallocHeader* next = curr->next;
			free(curr); // Non-whatever free, because alignment
			curr = next;
		}

		fprintf(stderr, "[ERROR] Unfreed Blocks: %zu\n", allocated_blocks);
	}
#endif
}

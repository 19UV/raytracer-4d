#include "objects/group.h"

#include <stdlib.h>
#include <stddef.h>

int group_create(Group* this) {
	this->count = 0;
	this->objects = NULL;

	return 0;
}

void group_destroy(Group* this) {
	if(this->objects != NULL) {
		free(this->objects);
	}
}

int group_add(Group* this, Object* object) {
	this->objects = realloc(this->objects, (this->count + 1) * sizeof(Object*));
	if(this->objects == NULL) {
		// The memory won't be freed, I would like a better handling system for out of memory
		return 1;
	}

	this->objects[this->count++] = object;

	return 0;
}

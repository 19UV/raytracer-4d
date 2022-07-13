#include "objects/group.h"

#include <stdlib.h>
#include <stddef.h>
#include <float.h>
#include <assert.h>

int group_create(Group* this) {
	assert(this != NULL);

	this->type = ObjGroup;
	this->hit = group_hit;

	this->count = 0;
	this->objects = NULL;

	return 0;
}

void group_destroy(Group* this) {
	if(this->objects != NULL) {
		free(this->objects);
	}
}

Hit group_hit(Group* this, Ray* ray) {
	assert(this != NULL);
	assert(this->type == ObjGroup);

	Hit closest_hit = { .has_hit = false, .t = FLT_MAX };
	for(size_t i = 0; i < this->count; i++) {
		Object* object = this->objects[i];
		assert(object != NULL);

		Hit hit = object->hit(object, ray);
		if(hit.has_hit && hit.t < closest_hit.t) {
			closest_hit = hit;
		}
	}

	return closest_hit;
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

#pragma once

#include <stdint.h>

#include "objects/object.h"

struct Group {
	size_t count;
	Object** objects;
};
typedef struct Group Group;

int group_create(Group* this);
void group_destroy(Group* this);

int group_add(Group* this, Object* object);

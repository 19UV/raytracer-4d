#pragma once

#include <stdint.h>

#include "objects/object.h"

struct Group {
	Primitives type;

	Hit (*hit)(struct Group*, Ray*);

	/* --------------- */

	size_t count;
	Object** objects;
};
typedef struct Group Group;

int group_create(Group* this);
void group_destroy(Group* this);

Hit group_hit(Group* this, Ray* ray);

int group_add(Group* this, Object* object);

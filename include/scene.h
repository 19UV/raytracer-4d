#pragma once

#include <stdint.h>

#include "objects/object.h"

struct Scene {
	size_t count;
	Object** objects;
};
typedef struct Scene Scene;

int scene_create(Scene* this);
void scene_destroy(Scene* this);

int scene_add(Scene* this, const Object* object);

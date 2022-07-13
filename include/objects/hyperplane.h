#pragma once

#include <cglm/cglm.h>

#include "image.h"
#include "objects/object.h"

struct Hyperplane {
	Primitives type;

	Hit (*hit)(struct Hyperplane*, Ray*);

	/* --------------- */

	size_t dimension;
	float location;

	Pixel color;
};
typedef struct Hyperplane Hyperplane;

int hyperplane_create(Hyperplane* this, float location, size_t dimension, Pixel color);
Hit hyperplane_hit(Hyperplane* this, Ray* ray);

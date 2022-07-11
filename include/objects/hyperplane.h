#pragma once

#include <cglm/cglm.h>

#include "objects/object.h"

struct Hyperplane {
	Primitives type;

	Hit (*hit)(struct Hyperplane*, Ray*);

	/* --------------- */

	size_t dimension;
	float location;
};
typedef struct Hyperplane Hyperplane;

int hyperplane_create(Hyperplane* this, float location, size_t dimension);
Hit hyperplane_hit(Hyperplane* this, Ray* ray);

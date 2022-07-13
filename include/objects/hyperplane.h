#pragma once

#include <cglm/cglm.h>

#include "material.h"
#include "objects/object.h"

struct Hyperplane {
	Primitives type;

	Hit (*hit)(struct Hyperplane*, Ray*);

	/* --------------- */

	size_t dimension;
	float location;

	Material material;
};
typedef struct Hyperplane Hyperplane;

int hyperplane_create(Hyperplane* this, float location, size_t dimension, Material material);
Hit hyperplane_hit(Hyperplane* this, Ray* ray);

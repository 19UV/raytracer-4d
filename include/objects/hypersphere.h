#pragma once

#include <cglm/cglm.h>

#include "image.h"
#include "objects/object.h"

struct Hypersphere {
	Primitives type;

	Hit (*hit)(struct Hypersphere*, Ray*);

	/* --------------- */

	vec4 position;
	float radius;

	Pixel color;
};
typedef struct Hypersphere Hypersphere;

int hypersphere_create(Hypersphere* this, vec4 position, float radius, Pixel color);
Hit hypersphere_hit(Hypersphere* this, Ray* ray);

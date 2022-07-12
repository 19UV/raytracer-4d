#pragma once

#include <cglm/cglm.h>

#include "objects/object.h"

struct Hypersphere {
	Primitives type;

	Hit (*hit)(struct Hypersphere*, Ray*);

	/* --------------- */

	vec4 position;
	float radius;

	vec3 color;
};
typedef struct Hypersphere Hypersphere;

int hypersphere_create(Hypersphere* this, vec4 position, float radius, vec3 color);
Hit hypersphere_hit(Hypersphere* this, Ray* ray);

#pragma once

#include <stdbool.h>

#include <cglm/cglm.h>

struct Hit {
	bool has_hit;
	
	float t;

	vec4 location;

	// TODO: Should this be Pixel instead?
	vec3 color;
};
typedef struct Hit Hit;

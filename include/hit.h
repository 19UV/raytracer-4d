#pragma once

#include <stdbool.h>

#include <cglm/cglm.h>

#include "image.h"

struct Hit {
	bool has_hit;
	
	float t;

	vec4 location;

	Pixel color;
	vec4 normal;
};
typedef struct Hit Hit;

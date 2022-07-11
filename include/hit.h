#pragma once

#include <stdbool.h>

#include <cglm/cglm.h>

struct Hit {
	bool has_hit;
	
	float t;

	vec4 location;
};
typedef struct Hit Hit;

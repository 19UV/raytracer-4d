#pragma once

#include <stdbool.h>

#include <cglm/cglm.h>

#include "material.h"

struct Hit {
	bool has_hit;
	
	float t;

	vec4 location;

	Material material;
	vec4 normal;
};
typedef struct Hit Hit;

#pragma once

#include <stdbool.h>

#include <cglm/cglm.h>

struct Hit {
	bool has_hit;

	vec4 location;
};
typedef struct Hit Hit;

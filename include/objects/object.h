#pragma once

#include "objects/primitives.h"

#include "ray.h"
#include "hit.h"

struct Object {
	Primitives type;

	Hit (*hit)(struct Object*, Ray*);
};
typedef struct Object Object;

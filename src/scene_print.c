#include "scene_print.h"

#include <stdio.h>
#include <stddef.h>
#include <assert.h>

#include "objects/primitives.h"
#include "objects/object.h"
#include "objects/hypersphere.h"
#include "objects/hyperplane.h"

static const char* primitive_to_str(Primitives primitive) {
	switch(primitive) {
		case Sphere4D: return "Hypersphere";
		case  Plane4D: return "Hyperplane";
	}

	return NULL;
}

static void print_hypersphere(Hypersphere* object) {
	assert(object != NULL);
	assert(object->type == Sphere4D);

	printf("\tPosition: (%f %f %f %f)\n",
		object->position[0],
		object->position[1],
		object->position[2],
		object->position[3]
	);
	printf("\tRadius: %f\n",
		object->radius
	);
}

static void print_hyperplane(Hyperplane* object) {
	assert(object != NULL);
	assert(object->type == Plane4D);

	printf("\tPosition: %f\n",
		object->location
	);

	static char axis_lookup[] = {'X', 'Y', 'Z', 'W'};

	// TODO: Find a better name for this
	printf("\tAxis of Restraint: %c\n",
		axis_lookup[object->dimension]
	);
}

void scene_print(Scene* scene) {
	for(size_t i = 0; i < scene->count; i++) {
		Object* object = scene->objects[i];
		printf("%s <0x%p>\n",
			primitive_to_str(object->type),
			object
		);

		switch(object->type) {
			case Sphere4D: print_hypersphere((Hypersphere*)object); break;
			case  Plane4D: print_hyperplane((Hyperplane*)object); break;
		}
	}
}

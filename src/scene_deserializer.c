#include "scene_deserializer.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <stdbool.h>
#include <assert.h>

#include "objects/object.h"
#include "objects/hypersphere.h"
#include "objects/hyperplane.h"

#define IDENTIFIER_BUFFER_LENGTH 100

static bool parse_float(FILE* file, float* dest) {
	return fscanf(file, "%f", dest) == 1;
}

static bool parse_usize(FILE* file, size_t* dest) {
	return fscanf(file, "%zu", dest) == 1;
}

static bool parse_vec3(FILE* file, vec3 dest) {
	return fscanf(file, "%f %f %f",
		&dest[0], &dest[1], &dest[2]
	) == 3;
}

static bool parse_vec4(FILE* file, vec4 dest) {
	return fscanf(file, "%f %f %f %f",
		&dest[0], &dest[1], &dest[2], &dest[3]
	) == 4;
}

static Hypersphere* parse_hypersphere(FILE* file) {
	Hypersphere* hypersphere = malloc(sizeof(Hypersphere));
	if(hypersphere == NULL) {
		return NULL;
	}

	vec4 position;
	float radius;

	vec3 color;

	assert(parse_vec4(file, position));
	assert(parse_float(file, &radius));
	assert(parse_vec3(file, color));

	hypersphere_create(hypersphere, position, radius, color);

	return hypersphere;
}

static Hyperplane* parse_hyperplane(FILE* file) {
	Hyperplane* hyperplane = malloc(sizeof(Hyperplane));
	if(hyperplane == NULL) {
		return NULL;
	}

	float location;
	size_t axis;
	
	vec3 color;

	assert(parse_float(file, &location));
	assert(parse_usize(file, &axis));
	assert(parse_vec3(file, color));

	hyperplane_create(hyperplane, location, axis, color);

	return hyperplane;
}

int scene_deserialize(Group* scene, const char* file_path) {
	assert(scene != NULL);

	FILE* file = fopen(file_path, "r");
	if(file == NULL) {
		return 1;
	}

	group_create(scene);

	// TODO: This is unsafe, improve
	char identifier_buffer[IDENTIFIER_BUFFER_LENGTH] = {0};
	while(1) {
		fscanf(file, "%s", identifier_buffer);
		if(feof(file)) {
			break;
		}

		Object* object = NULL;
		if(strcmp(identifier_buffer, "Hypersphere") == 0) {
			object = (Object*)parse_hypersphere(file);
		} else if(strcmp(identifier_buffer, "Hyperplane") == 0) {
			object = (Object*)parse_hyperplane(file);
		} else {
			fprintf(stderr, "[ERROR] Unhandled Object \"%s\"\n", identifier_buffer);
			return 1;
		}

		group_add(scene, object);
	}

	fclose(file);

	return 0;
}

#include "util/random.h"

#include <stdlib.h>
#include <time.h>

void init_random(void) {
	time_t t;

	srand((unsigned)time(&t));
}

float float_random(void) {
	return (float)rand() / (float)RAND_MAX;
}

void vec4_random(vec4 vector) {
	vector[0] = float_random();
	vector[1] = float_random();
	vector[2] = float_random();
	vector[3] = float_random();
}

void unit_sphere_4d_random(vec4 vector) {
	vec4_random(vector);
	glm_vec4_subs(vector, 0.5f, vector);
	glm_vec4_normalize(vector);
}

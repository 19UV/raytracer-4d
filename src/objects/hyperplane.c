#include "objects/hyperplane.h"

#include <stddef.h>
#include <assert.h>

int hyperplane_create(Hyperplane* this, float location, size_t dimension, Material material) {
	assert(this != NULL);
	assert((dimension >= 0) && (dimension < 4));

	this->type = Plane4D;

	this->dimension = dimension;
	this->location = location;

	this->material = material;

	this->hit = hyperplane_hit;

	return 0;
}

/**
 * Defining a Plane (3D)
 *
 * Where the plane spans across the XY axes where the z='l', and P is a point.
 * P = (_, _, n)
 *
 * P is on the plane when n = l.
 *
 * ----------------------------------------
 *  Defining the Ray (see hypersphere.c)
 * ----------------------------------------
 * Finding the Ray Intersection
 *
 * P(t) = Dt + O
 * 		Pz = Dz * t + Oz
 *
 * Pz = l; Dz * t + Oz = l
 * 		t = (l - Oz) / Dz
 * 
 * This can be expanded for any number of dimensions/axes
*/

Hit hyperplane_hit(Hyperplane* this, Ray* ray) {
	assert(this != NULL);
	assert(this->type == Plane4D);

	const size_t dimension = this->dimension;

	float t = (this->location - ray->origin[dimension]) / ray->direction[dimension];
	if(t <= 0.0f) {
		return (Hit){
			.has_hit = false
		};
	}

	Hit res;
	res.has_hit = true;
	res.t = t;

	// TODO: Shouldn't this be a helper function?
	// Its being used for every object
	glm_vec4_scale(ray->direction, t, res.location);
	glm_vec4_add(res.location, ray->origin, res.location);

	res.material = this->material;

	vec4 normal = {0.0f, 0.0f, 0.0f, 0.0f};
	normal[dimension] = ray->origin[dimension] > this->location ? 1.0f : -1.0f;
	glm_vec4_copy(normal, res.normal);

	return res;
}

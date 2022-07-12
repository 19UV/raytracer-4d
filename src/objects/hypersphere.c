#include "objects/hypersphere.h"

#include <string.h>
#include <stddef.h>
#include <assert.h>

int hypersphere_create(Hypersphere* this, vec4 position, float radius, vec3 color) {
	assert(this != NULL);

	this->type = Sphere4D;

	glm_vec4_copy(position, this->position);

	assert(radius >= 0.0f);
	this->radius = radius;

	glm_vec3_copy(color, this->color);

	this->hit = hypersphere_hit;

	return 0;
}

/**
 * Defining a Circle (2D)
 *
 * Where C = center of circle, r = radius of circle, and P is a point
 * C = (Cx, Cy), P = (Px, Py)
 *
 * d = distance from P to C
 * (Cx - Px) ^  2 + (Py - Cy) ^ 2 = r ^  2
 *
 * This is equivilent to
 * (P - C) * (P - C) = r ^  2
 * Which is true for any number of dimensions (expand out (P - C) * (P - C) where * is a dot product)
 *
 * ----------------------------------------
 *  Defining the Ray
 *
 *  D = direction multiplier of the ray (vector), O = origin of the ray
 *  t = independant variable (could be called time)
 *
 *  Ray = Dt + O
 *  	P(t) = Dt + O
 *
 * ----------------------------------------
 *  Finding the Ray Intersection
 *
 *  Note that double variables (ex. tt) means t * t or t ^  2
 *
 *  (P(t) - C) * (P(t) - C) = rr
 *  (Dt + O - C) * (Dt + O - C) - rr = 0
 *  DDtt + DOt + DOt - CDt - CDt - CO - CO + OO + CC - rr = 0
 *  DDtt + 2DOt - 2COT - 2CO + OO + CC - rr = 0
 *  (DD)tt + (2DO - 2CO)t + (OO + CC - 2CO - rr) = 0
 *
 *  The equation is the quadratic formula (ax^2 + bx + c)
 *  a = DD; b = 2DO - 2CO; c = OO + CC - 2CO - rr
 *
 *  Zeros can be found using the quadratic formula
 *  t = (-b +/- sqrt(b^2 - 4ac)) / (2a)
 *
 *  By ignoring imaginary solutions (when b^2 - 4ac < 0), we can tell if/how the ray intersects the object
 *  b^2 - 4ac < 0 : The ray doesn't hit the sphere TODO: Can I visualize this? What would it look like?
 *  b^2 - 4ac = 0 : The ray hits at the absolute top (it only hits once)
 *  b^2 - 4ac > 0 : The ray hits the sphere at multiple points
 *
 *  Because we only care about the closest side of objects & we don't want to see the inner part of objects
 *  We're only using (-b - sqrt(b^2 - 4ac)) / (2a)
 *
 *  Lets implement it!
*/
Hit hypersphere_hit(Hypersphere* this, Ray* ray) {
	assert(this != NULL);
	assert(this->type == Sphere4D);

	vec4 oc;
	glm_vec4_sub(ray->origin, this->position, oc);

	// a = D^2 = D * D
	float a = glm_vec4_dot(ray->direction, ray->direction);
	// b = 2DO - 2CD = 2(DO - CD) = 2D(O - C) = 2 * D * oc
	float b = 2.0f * glm_vec4_dot(ray->direction, oc);
	// c = O^2 + C^2 - 2CO - r^2 = (O^2 - 2OC + C^2) - r^2 = (O - C)^2 - r^2 = (oc)^2 - r^2
	float c = glm_vec4_dot(oc, oc) - (this->radius, this->radius);

	float discriminant = (b * b) - (4.0f * a * c);
	if(discriminant < 0.0f) { // Imaginary/No Solutions
		return (Hit){
			.has_hit = false
		};
	}

	float t = (-b - sqrtf(discriminant)) / (2.0f * a);

	Hit res;
	res.has_hit = true;
	res.t = t;

	glm_vec4_scale(ray->direction, t, res.location);
	glm_vec4_add(res.location, ray->origin, res.location);

	glm_vec3_copy(this->color, res.color);

	glm_vec4_sub(res.location, this->position, res.normal);
	glm_vec4_normalize(res.normal);

	return res;
}

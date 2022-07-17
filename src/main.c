#include <stdio.h>
#include <float.h>

#include <cglm/cglm.h>

#include "image.h"

#include "util/random.h"

#include "objects/group.h"
#include "objects/hypersphere.h"
#include "objects/hyperplane.h"

#include "scene_deserializer.h"
#include "scene_print.h"

Pixel trace_ray(Group* scene, Ray* ray, size_t depth) {
	if(depth == 0) {
		return (Pixel){
			.r = 0.0f,
			.g = 0.0f,
			.b = 0.0f
		};
	}

	Hit hit = group_hit(scene, ray);
	if(hit.has_hit) {
		vec4 target;
		unit_sphere_4d_random(target);
		glm_vec4_add(target, hit.normal, target);

		Ray child_ray;
		glm_vec4_copy(hit.location, child_ray.origin);
		glm_vec4_copy(target, child_ray.direction);

		Pixel child_pixel = trace_ray(scene, &child_ray, depth - 1);
		child_pixel.r *= 0.5f;
		child_pixel.g *= 0.5f;
		child_pixel.b *= 0.5f;

		return child_pixel;
	}

	// Don't need to normalize, because we assume that it already has been
	float t = 0.5f * (ray->direction[1] + 1.0f);
	Pixel res;

	Pixel from = { .r = 1.0f, .g = 1.0f, .b = 1.0f };
	Pixel to   = { .r = 0.5f, .g = 0.7f, .b = 1.0f };

	glm_vec3_lerp((float*)&from, (float*)&to, t, (float*)&res);

	return res;
}

int main(int argc, char* argv[]) {
	const size_t image_width = 1080, image_height = 1080;
	const float aspect_ratio = (float)image_width / (float)image_height;

	const size_t max_depth = 500;

	Image image;
	if(image_create(&image, image_width, image_height)) {
		return 1;
	}

	vec4 camera_pos = { 0.0f, 0.0f, 0.0f, 1.0f };
	vec4 sphere_pos = { 0.0f, 0.0f, -5.0f, 0.0f };

	Group scene;
	scene_deserialize(&scene, "./scenes/sphere.txt");
	scene_print(&scene);

	Ray ray;
	glm_vec4_copy(camera_pos, ray.origin);

	for(size_t y = 0; y < image_height; y++) {
		float v = (float)y / (float)image_height;
		for(size_t x = 0; x < image_width; x++) {
			vec4 uv = {
				(((float)x / (float)image_width) * 2.0f - 1.0f) * aspect_ratio,
				 (                            v) * 2.0f - 1.0f,
				 -1.0f,
				 0.0f
			};

			glm_vec4_normalize_to(uv, ray.direction);

			Pixel pixel = trace_ray(&scene, &ray, max_depth);

			image_set(&image, x, y, pixel);
		}
	}

	for(size_t i = 0; i < scene.count; i++) {
		FREE(scene.objects[i]);
	}
	group_destroy(&scene);

	if(image_save_png(&image, "./output.png")) {
		return 1;
	}

	image_destroy(&image);

	check_memoryleaks();

	return 0;
}

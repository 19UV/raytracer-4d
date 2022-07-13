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

		return (Pixel){
			.r = target[0] * 0.25f + 0.5f,
			.g = target[1] * 0.25f + 0.5f,
			.b = target[2] * 0.25f + 0.5f
		};
	} else {
		return (Pixel){
			.r = 0.5f,
			.g = 0.5f,
			.b = 0.5f
		};
	}
}

int main(int argc, char* argv[]) {
	const size_t image_width = 1080, image_height = 1080;
	const float aspect_ratio = (float)image_width / (float)image_height;

	const size_t max_depth = 100;

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
		free(scene.objects[i]);
	}
	group_destroy(&scene);

	if(image_save_png(&image, "./output.png")) {
		return 1;
	}

	image_destroy(&image);

	return 0;
}

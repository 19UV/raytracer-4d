#include <stdio.h>
#include <float.h>

#include <cglm/cglm.h>

#include "image.h"

#include "scene.h"
#include "scene_deserializer.h"
#include "scene_print.h"

#include "objects/hypersphere.h"
#include "objects/hyperplane.h"

int main(int argc, char* argv[]) {
	const size_t image_width = 192, image_height = 108;
	const float aspect_ratio = (float)image_width / (float)image_height;

	Image image;
	if(image_create(&image, image_width, image_height)) {
		return 1;
	}

	vec4 camera_pos = { 0.0f, 0.0f, 0.0f, 1.0f };
	vec4 sphere_pos = { 0.0f, 0.0f, -5.0f, 0.0f };

	Scene scene;
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

			glm_vec4_copy(uv, ray.direction);

			Hit closest_hit = { .has_hit = false, .t = FLT_MAX };
			for(size_t i = 0; i < scene.count; i++) {
				Object* object = scene.objects[i];
				Hit hit = object->hit(object, &ray);
				if(hit.has_hit && hit.t < closest_hit.t) {
					closest_hit = hit;
				}
			}

			if(closest_hit.has_hit) {
				image_set(&image, x, y, (Pixel){
					.r = (closest_hit.normal[0] + 1.0f) / 2.0f,
					.g = (closest_hit.normal[1] + 1.0f) / 2.0f,
					.b = (closest_hit.normal[2] + 1.0f) / 2.0f
				});
			} else {
				image_set(&image, x, y, (Pixel){
					.r = 0.0f,
					.g = 0.0f,
					.b = 0.0f
				});
			}
		}
	}

	for(size_t i = 0; i < scene.count; i++) {
		free(scene.objects[i]);
	}
	scene_destroy(&scene);

	if(image_save_png(&image, "./output.png")) {
		return 1;
	}

	image_destroy(&image);

	return 0;
}

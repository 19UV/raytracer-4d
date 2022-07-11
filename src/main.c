#include <stdio.h>
#include <float.h>

#include <cglm/cglm.h>

#include "image.h"
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

	Hypersphere sphere;
	hypersphere_create(&sphere, sphere_pos, 3.0f);

	Hyperplane plane;
	hyperplane_create(&plane, -4.0f, 1);

	Object* objects[] = {
		&sphere,
		&plane
	};

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
			for(size_t i = 0; i < sizeof(objects) / sizeof(objects[0]); i++) {
				Hit hit = objects[i]->hit(objects[i], &ray);
				if(hit.has_hit && hit.t < closest_hit.t) {
					closest_hit = hit;
				}
			}

			if(closest_hit.has_hit) {
				image_set(&image, x, y, (Pixel){
					.r = 1.0f,
					.g = 0.0f,
					.b = 0.0f
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

	if(image_save_png(&image, "./output.png")) {
		return 1;
	}

	image_destroy(&image);

	return 0;
}

#include <stdio.h>

#include <cglm/cglm.h>

#include "image.h"

int main(int argc, char* argv[]) {
	const size_t image_width = 100, image_height = 100;

	Image image;
	if(image_create(&image, image_width, image_height)) {
		return 1;
	}

	for(size_t y = 0; y < image_height; y++) {
		float v = (float)y / (float)image_height;
		for(size_t x = 0; x < image_width; x++) {
			vec2 uv = {
				(float)x / (float)image_width,
				v
			};

			image_set(&image, x, y, (Pixel){
				.r = uv[0],
				.g = uv[1],
				.b = 0.0f
			});
		}
	}

	if(image_save_png(&image, "./output.png")) {
		return 1;
	}

	image_destroy(&image);

	return 0;
}

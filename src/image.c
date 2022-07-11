#include "image.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <assert.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb/stb_image_write.h>

static size_t get_offset(const Image* this, size_t x, size_t y) {
	assert(this != NULL);
	assert(x < this->width);
	assert(y < this->height);

	return (this->width * y) + x;
}

int image_create(Image* this, size_t width, size_t height) {
	assert(this != NULL);

	const size_t pixel_count = width * height;

	this->width = 0;
	this->height = 0;

	this->data = malloc(pixel_count * sizeof(Pixel));
	if(this->data == NULL) {
		return 1;
	}

	this->width = width;
	this->height = height;

	for(size_t i = 0; i < pixel_count; i++) {
		this->data[i] = (Pixel){
			.r = 0.0f,
			.g = 0.0f,
			.b = 0.0f
		};
	}

	return 0;
}

void image_destroy(Image* this) {
	assert(this != NULL);

	free(this->data);
}

void image_set(Image* this, size_t x, size_t y, Pixel pixel) {
	// Assert that the pixel values are in bounds
	assert((pixel.r >= 0.0f) && (pixel.r <= 255.0f));
	assert((pixel.g >= 0.0f) && (pixel.g <= 255.0f));
	assert((pixel.b >= 0.0f) && (pixel.b <= 255.0f));

	// Offset already asserts that this != NULL
	const size_t offset = get_offset(this, x, y);

	this->data[offset] = pixel;
}

Pixel image_get(const Image* this, size_t x, size_t y) {
	// Offset already asserts
	const size_t offset = get_offset(this, x, y);

	return this->data[offset];
}


int image_save_png(const Image* this, const char* file_path) {
	assert(this != NULL);

	const size_t pixel_count = this->width * this->height;

	const float* input = (const float*)this->data;
	uint8_t* output = malloc(pixel_count * 3 * sizeof(uint8_t));
	if(output == NULL) {
		fprintf(stderr, "[ERROR] Out of Memory.\n");
		return 1;
	}

	for(size_t i = 0; i < pixel_count * 3; i++) {
		output[i] = (uint8_t)(input[i] * 255.0f);
	}

	stbi_flip_vertically_on_write(1);
	if(!stbi_write_png(file_path, (int)this->width, (int)this->height, 3, output, (int)this->width * 3)) {
		fprintf(stderr, "[ERROR] Failed to Write to File.\n");
		return 1;
	}

	free(output);

	return 0;
}
